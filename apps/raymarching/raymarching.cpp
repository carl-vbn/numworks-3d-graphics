#include "raymarching.h"

namespace Raymarching
{
    constexpr int16_t RENDER_DISTANCE = 200;
    constexpr int8_t MAX_STEPS = 20;
    constexpr int8_t FOV = 60;
    constexpr int16_t RESOLUTION = 240; // Between 1 and 240, where 240 is the highest resolution
    constexpr float SURFACE_DIST = 0.5;
    constexpr float EPSILON = 0.01;
    constexpr float DEG_2_RAD = M_PI / 180;
    constexpr float RAD_2_DEG = 180 / M_PI;

    constexpr KDColor KDColorGray = KDColor::RGB24(0x666666);

    float sphereSD(float3 pos, float3 spherePos, float sphereRadius) {
        return distance(pos, spherePos) - sphereRadius;
    }

    float cubeSD(float3 pos, float3 cubePos, float3 cubeSize) {
        float3 q = difference(abs(difference(pos,cubePos)), cubeSize);
        return max(length(q),0) + min(max(q.x,max(q.y,q.z)),0.0);
    }

    float RaymarchingScene::distanceFromGeometry(float3 pos, RMObject * nearestSpherePtr) {
        if (m_loadedObjectCount < 1) {
            return 0;
        }


        RMObject nearestSphere;
        float minDist = RENDER_DISTANCE;
        for (int i = 0; i<m_loadedObjectCount; i++) {
            RMObject rm_obj = *(m_loadedObjects+i);
            float dist = RENDER_DISTANCE;
            switch (rm_obj.distanceFunction) {
                case SPHERE_SDF:
                    dist = sphereSD(pos, rm_obj.position, rm_obj.radius);
                    break;
                case CUBE_SDF:
                    
                    break;
            }    
            if (dist < minDist) {
                minDist = dist;
                nearestSphere = rm_obj;
            }
        }
        
        if (nearestSpherePtr != nullptr) {
            *nearestSpherePtr = nearestSphere;
        }
        return minDist;
    }

    RaymarchingScene::RaymarchingScene(int scene) {
        if (scene == 0) {
            m_loadedObjects = new RMObject[3]{RMObject(CUBE_SDF, 135,150,0, 10, KDColorPurple),RMObject(SPHERE_SDF, 160,150,0, 10),RMObject(SPHERE_SDF, 185,150,0, 10, KDColorRed)};
            m_loadedObjectCount = 3;

            m_camPos = float3(160,80,-3);
            m_lightPos = float3(180,80,-20);
        } else if (scene == 1) {
            m_loadedObjects = new RMObject[2]{RMObject(SPHERE_SDF, 135,170,0, 20, KDColorGreen),RMObject(SPHERE_SDF, 185,170,0, 20, KDColorBlue)};
            m_loadedObjectCount = 2;

            m_camPos = float3(160,80,-3);
            m_lightPos = float3(140,100,-20);
        } else if (scene == 2) {

        }
        
        m_camYaw = 0;
        m_camPitch = 0;
    }

    RaymarchingScene::~RaymarchingScene() {
        delete[] m_loadedObjects;
    }

    float3 RaymarchingScene::estimateNormal(float3 pos) {
        float d = distanceFromGeometry(pos);
        
        float3 normal = difference(float3(d,d,d), float3(
            distanceFromGeometry(difference(pos, float3(EPSILON,0,0))),
            distanceFromGeometry(difference(pos, float3(0,EPSILON,0))),
            distanceFromGeometry(difference(pos, float3(0,0,EPSILON)))));
        
        normalize(&normal);
        return normal;
    }

    RaymarchHit RaymarchingScene::rayMarch(float3 origin, float3 direction) {
        float d = SURFACE_DIST+1;
        float3 p = origin;
        int8_t steps = 0;
        float marchedDistance = 0;
        while (steps < MAX_STEPS && marchedDistance < RENDER_DISTANCE) {
            RMObject nearestSphere;
            d = distanceFromGeometry(p, &nearestSphere);
            add(&p, multiply(direction, d));
            marchedDistance+= d;

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
        if (!fast)
            Ion::Display::pushRectUniform(rect, KDColorWhite);
        float ratio = (float)rect.height()/rect.width();
        int V_FOV = FOV * ratio;
        int yawRays = RESOLUTION * (fast ? 0.2 : 1);
        int pitchRays  = RESOLUTION * ratio * (fast ? 0.2 : 1);

        for (int i = 0; i<yawRays; i++) {
            for (int j = 0; j<pitchRays; j++) {
                int u = rect.x()+(int)((float)i/yawRays*rect.width());
                int v = rect.y()+(int)((float)j/pitchRays*rect.height());

                KDRect square = KDRect(u,v,rect.width()/yawRays+1, rect.height()/pitchRays+1);
                
                float yaw = (float)i/yawRays*FOV-FOV/2+m_camYaw;
                float pitch = (float)j/pitchRays*V_FOV-V_FOV/2+m_camPitch;

                float3 rayDir = float3(sin(yaw*DEG_2_RAD), cos(yaw*DEG_2_RAD), sin(pitch*DEG_2_RAD));

                
                RaymarchHit hit = rayMarch(m_camPos, rayDir);
                if (hit.hasHit) {
                    KDColor pixelColor;
                    if (fast) {
                        pixelColor = hit.hitColor;
                    } else {
                        float3 lightVec = difference(m_lightPos, hit.hitPosition);
                        normalize(&lightVec);
                        float l = dot(lightVec, estimateNormal(hit.hitPosition));
                        if (l > 1) l = 1; if (l < 0) l = 0; // Clamp l between 0 and 1
                        pixelColor = KDColor::RGB888(hit.hitColor.red()*l,hit.hitColor.green()*l,hit.hitColor.blue()*l);
                    }
                    Ion::Display::pushRectUniform(square, pixelColor);
                } else {
                    Ion::Display::pushRectUniform(square, KDColorBlack);
                }
            }
        }
    }

    void RaymarchingScene::translateCamera(float3 vector) {
        add(&m_camPos, vector);
    }

    void RaymarchingScene::rotateCamera(float yaw, float pitch) {
        m_camYaw += yaw;
        m_camPitch += pitch;
    }

    float3 RaymarchingScene::getCameraVector(float yawOffset) {
        return float3(sin((m_camYaw+yawOffset)*DEG_2_RAD), cos((m_camYaw+yawOffset)*DEG_2_RAD), 0);
    }

}