#include "raymarching.h"
#include "ion/led.h"
#include <kandinsky/framebuffer.h>

namespace Raymarching
{
    constexpr int16_t RENDER_DISTANCE = 400;
    constexpr int8_t MAX_STEPS = 20;
    constexpr int8_t FOV = 60;
    constexpr int16_t SUBRECT_SIZE = 150;
    constexpr int16_t YAW_RAYS = 100;
    constexpr float SURFACE_DIST = 0.5;

    Sphere * loadedSpheres;
    int loadedSphereCount;

    float sphereSD(float3 pos, float3 spherePos, float sphereRadius) {
        return distance(pos, spherePos) - sphereRadius;
    }

    float cubeSD(float3 pos, float3 cubePos, float3 cubeSize) {
        float3 o = difference(abs(difference(pos,cubePos)), cubeSize);
        float ud = max(length(o),0);
        float n = max(max(min(o.x,0),min(o.y,0)), min(o.z,0));
        return ud+n;
    }

    float distanceFromGeometry(float3 pos) {
        return sphereSD(pos, float3(160,150,0), 10);
    }

    void loadScene(int scene) {
        if (scene == 0) {
            Sphere sphere = Sphere(0,1,6,10,1,0,0);
            loadedSpheres = &sphere;
            loadedSphereCount = 1;
        }
    }

    RaymarchHit rayMarch(float3 origin, float3 direction) {
        float d = SURFACE_DIST+1;
        float3 p = origin;
        int8_t steps = 0;
        while (steps < MAX_STEPS && distance(origin, p) < RENDER_DISTANCE) {
            d = distanceFromGeometry(p);
            p = sum(p, multiply(direction, d));

            if (d <= SURFACE_DIST) {
                return RaymarchHit(true, p, KDColorRed);
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

    void render(KDRect rect) {
        for (int i = rect.x(); i<rect.x()+rect.width(); i+=SUBRECT_SIZE) {
            for (int j = rect.y(); j<rect.y()+rect.height(); j+=SUBRECT_SIZE) {
                KDRect subRect = rect.intersectedWith(KDRect(i,j,SUBRECT_SIZE,SUBRECT_SIZE));
                renderSubRect(rect, subRect);
            }
        }
    }
    

   void renderSubRect(KDRect rect, KDRect subrect) {
        KDColor * pixelBuffer = new KDColor[subrect.width()*subrect.height()];

        for (int i = 0; i<subrect.width()*subrect.height(); i++)
            *(pixelBuffer+i) = KDColorBlack;

        float3 camPos = float3(160,80,-3);
        float deg2rad = M_PI / 180;
        float ratio = (float)rect.height()/rect.width();
        int V_FOV = FOV * ratio;
        int PITCH_RAYS  = YAW_RAYS * ratio;

        for (int i = 0; i<YAW_RAYS; i++) {
            for (int j = 0; j<PITCH_RAYS; j++) {
                int u = rect.x()+(int)((float)i/YAW_RAYS*rect.width());
                int v = rect.y()+(int)((float)j/PITCH_RAYS*rect.height());

                if (!subrect.contains(KDPoint(u,v)))
                    continue;

                KDRect square = subrect.intersectedWith(KDRect(u,v,rect.width()/YAW_RAYS, rect.height()/PITCH_RAYS)); // Basically a big pixel (approaches 1 when the amount of rays increases)
                
                if (subrect.containsRect(square)) {
                    float yaw = (float)i/YAW_RAYS*FOV-FOV/2;
                    float pitch = (float)j/PITCH_RAYS*V_FOV-V_FOV/2;

                    float3 rayDir = float3(sin(yaw*deg2rad), cos(yaw*deg2rad), sin(pitch*deg2rad));

                    RaymarchHit hit = rayMarch(camPos, rayDir);
                    if (hit.hasHit) {
                        for (int m = 0; m<square.width(); m++) {
                            for (int n = 0; n<square.height(); n++) {
                                int offset = (square.y()-subrect.y()+n)*subrect.width()+(square.x()-subrect.x()+m);
                                *(pixelBuffer+offset) = hit.hasHit ? hit.hitColor : KDColorYellow;
                            }
                        }
                    }
                }

            }
        }

        Ion::Display::pushRect(subrect, pixelBuffer);

        delete[] pixelBuffer;
    }

}