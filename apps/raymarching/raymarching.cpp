#include "raymarching.h"
#include "ion/led.h"

namespace Raymarching
{
    constexpr int16_t RENDER_DISTANCE = 400;
    constexpr int8_t MAX_STEPS = 20;
    constexpr int8_t FOV = 60;
    constexpr int16_t SUBRECT_SIZE = 150;
    constexpr int16_t RESOLUTION = 240; // Between 1 and 240, where 240 is the highest resolution
    constexpr float SURFACE_DIST = 0.5;
    constexpr float EPSILON = 0.01;
    constexpr float DEG_2_RAD = M_PI / 180;
    constexpr float RAD_2_DEG = 180 / M_PI;

    float sphereSD(float3 pos, float3 spherePos, float sphereRadius) {
        return distance(pos, spherePos) - sphereRadius;
    }

    float cubeSD(float3 pos, float3 cubePos, float3 cubeSize) {
        float3 o = difference(abs(difference(pos,cubePos)), cubeSize);
        float ud = max(length(o),0);
        float n = max(max(min(o.x,0),min(o.y,0)), min(o.z,0));
        return ud+n;
    }

    float RaymarchingScene::distanceFromGeometry(float3 pos, Sphere * nearestSpherePtr) {
        if (m_loadedSphereCount < 1) {
            return 0;
        }


        Sphere nearestSphere;
        float minDist = RENDER_DISTANCE;
        for (int i = 0; i<m_loadedSphereCount; i++) {
            Sphere sphere = *(m_loadedSpheres+i);
            float dist = sphereSD(pos, sphere.position, sphere.radius);
            if (dist < minDist) {
                minDist = dist;
                nearestSphere = sphere;
            }
        }
        
        if (nearestSpherePtr != nullptr) {
            *nearestSpherePtr = nearestSphere;
        }
        return minDist;
    }

    RaymarchingScene::RaymarchingScene(int scene) {
        if (scene == 0) {
            m_loadedSpheres = new Sphere[3]{Sphere(135,150,0, 10, KDColorPurple),Sphere(160,150,0, 10),Sphere(185,150,0, 10, KDColorRed)};
            m_loadedSphereCount = 3;

            m_camPos = float3(160,80,-3);
            m_lightPos = float3(180,80,-20);
        } else if (scene == 1) {
            m_loadedSpheres = new Sphere[2]{Sphere(135,170,0, 20, KDColorGreen),Sphere(185,170,0, 20, KDColorBlue)};
            m_loadedSphereCount = 2;

            m_camPos = float3(160,80,-3);
            m_lightPos = float3(140,100,-20);
        }
        
        m_camYaw = 0;
    }

    RaymarchingScene::~RaymarchingScene() {
        delete[] m_loadedSpheres;
    }

    float3 RaymarchingScene::estimateNormal(float3 pos) {
        float d = distanceFromGeometry(pos);
        
        float3 normal = difference(float3(d,d,d), float3(
            distanceFromGeometry(difference(pos, float3(EPSILON,0,0))),
            distanceFromGeometry(difference(pos, float3(0,EPSILON,0))),
            distanceFromGeometry(difference(pos, float3(0,0,EPSILON)))));
        
        return normalize(normal);
    }

    RaymarchHit RaymarchingScene::rayMarch(float3 origin, float3 direction) {
        float d = SURFACE_DIST+1;
        float3 p = origin;
        int8_t steps = 0;
        while (steps < MAX_STEPS && distance(origin, p) < RENDER_DISTANCE) {
            Sphere nearestSphere;
            d = distanceFromGeometry(p, &nearestSphere);
            p = sum(p, multiply(direction, d));

            if (d <= SURFACE_DIST) {
                if (nearestSphere.checkerPattern) {
                    float angle1 = atan2(p.y-nearestSphere.position.y,p.x-nearestSphere.position.x)*RAD_2_DEG;
                    float angle2 = atan2(p.y-nearestSphere.position.y,p.z-nearestSphere.position.z)*RAD_2_DEG;
                    if (angle1 < 0) angle1 = -angle1;
                    if (angle2 < 0) angle2 = -angle2;
                    KDColor checkerColor = KDColorWhite;
                    if (checkerPattern(angle1, angle2, 50))
                        checkerColor = KDColorGray;
                    return RaymarchHit(true, p, checkerColor);
                } else {
                    return RaymarchHit(true, p, nearestSphere.color);
                }
            }

            steps++;
        }

        return RaymarchHit(false, p, KDColorBlack);
    }

    void drawLine(float3 pos1, float3 pos2, KDColor color) {
        float dist = distance(pos1, pos2);
        for (int i = 0; i<dist; i++) {
            int x = lerp(pos1.x, pos2.x, i/dist);
            int y = lerp(pos1.y, pos2.y, i/dist);

            Ion::Display::pushRectUniform(KDRect(x,y,1,1), color);
        }
    }

    void drawCircle(float3 pos, float radius, KDColor color) {
        for (int i = 0; i<360; i++) {
            Ion::Display::pushRectUniform(KDRect(pos.x+sin(i)*radius, pos.y+cos(i)*radius, 1, 1), color);
        }
    }

    void RaymarchingScene::render(KDRect rect, bool fast) {
        for (int i = rect.x(); i<rect.x()+rect.width(); i+=SUBRECT_SIZE) {
            for (int j = rect.y(); j<rect.y()+rect.height(); j+=SUBRECT_SIZE) {
                KDRect subRect = rect.intersectedWith(KDRect(i,j,SUBRECT_SIZE,SUBRECT_SIZE));
                renderSubRect(rect, subRect, fast);
            }
        }
    }
    

   void RaymarchingScene::renderSubRect(KDRect rect, KDRect subrect, bool fast) {
        KDColor * pixelBuffer = new KDColor[subrect.width()*subrect.height()];

        for (int i = 0; i<subrect.width()*subrect.height(); i++)
            *(pixelBuffer+i) = KDColorBlack;

        float ratio = (float)rect.height()/rect.width();
        int V_FOV = FOV * ratio;
        int yawRays = RESOLUTION * (fast ? 0.2 : 1);
        int pitchRays  = RESOLUTION * ratio * (fast ? 0.2 : 1);

        for (int i = 0; i<yawRays; i++) {
            for (int j = 0; j<pitchRays; j++) {
                int u = rect.x()+(int)((float)i/yawRays*rect.width());
                int v = rect.y()+(int)((float)j/pitchRays*rect.height());

                if (!((u >= subrect.x()-2 && u <= subrect.right()+2 && v >= subrect.y()-2 && v <= subrect.bottom()+2)))
                    continue;

                KDRect square = subrect.intersectedWith(KDRect(u,v,rect.width()/yawRays+1, rect.height()/pitchRays+1)); // Basically a big pixel (approaches 1 when the amount of rays increases)
                
                if (subrect.containsRect(square)) {
                    float yaw = (float)i/yawRays*FOV-FOV/2+m_camYaw;
                    float pitch = (float)j/pitchRays*V_FOV-V_FOV/2;

                    float3 rayDir = float3(sin(yaw*DEG_2_RAD), cos(yaw*DEG_2_RAD), sin(pitch*DEG_2_RAD));

                    
                    RaymarchHit hit = rayMarch(m_camPos, rayDir);
                    if (hit.hasHit) {
                        KDColor pixelColor;
                        if (fast) {
                            pixelColor = hit.hitColor;
                        } else {
                            float3 lightVec = normalize(difference(m_lightPos, hit.hitPosition));
                            float l = dot(lightVec, estimateNormal(hit.hitPosition));
                            if (l > 1) l = 1; if (l < 0) l = 0; // Clamp l between 0 and 1
                            pixelColor = KDColor::RGB888(hit.hitColor.red()*l,hit.hitColor.green()*l,hit.hitColor.blue()*l);
                        }
                        for (int m = 0; m<square.width(); m++) {
                            for (int n = 0; n<square.height(); n++) {
                                int offset = (square.y()-subrect.y()+n)*subrect.width()+(square.x()-subrect.x()+m);
                                *(pixelBuffer+offset) = pixelColor;
                            }
                        }
                    }
                    
                }

            }
        }

        Ion::Display::pushRect(subrect, pixelBuffer);

        delete[] pixelBuffer;
    }

    void RaymarchingScene::translateCamera(float3 vector) {
        m_camPos = sum(m_camPos, vector);
    }

    void RaymarchingScene::rotateCamera(float angle) {
        m_camYaw += angle;
    }

}