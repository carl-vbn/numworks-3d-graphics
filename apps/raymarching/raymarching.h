#include <ion/display.h>
#include "math_utils.h"

namespace Raymarching {

    struct RaymarchHit {
        bool hasHit;
        float3 hitPosition;
        KDColor hitColor;

        RaymarchHit(bool _hasHit, float3 _hitPosition, KDColor _hitColor) {
            hasHit = _hasHit;
            hitPosition = _hitPosition;
            hitColor = _hitColor;
        }
    };
    
    struct Sphere {
        float3 position;
        float radius;
        KDColor color;

        Sphere() {
            
        }

        Sphere(float3 pos, float r, KDColor c) {
            position = pos;
            radius = r;
            color = c;
        }

        Sphere(float px, float py, float pz, float r, KDColor c) {
            position = float3(px,py,pz);
            radius = r; 
            color = c;
        }

    };

    float sphereSD(float3 pos, float3 spherePos, float sphereRadius);
    float cubeSD(float3 pos, float3 cubePos, float3 cubeSize);
    
    class RaymarchingScene  {
        public:
            RaymarchingScene(int scene);
            ~RaymarchingScene();
            void render(KDRect rect, bool fast=false);
            void renderSubRect(KDRect rect, KDRect subrect, bool fast=false);

        protected:
            float3 m_camPos;
            float3 m_lightPos;
            Sphere * m_loadedSpheres;
            int m_loadedSphereCount;

            float distanceFromGeometry(float3 pos, KDColor * nearestColor = nullptr);
            float3 estimateNormal(float3 pos);
            RaymarchHit rayMarch(float3 origin, float3 direction);

    };
    void drawLine(float3 pos1, float3 pos2, KDColor color);
    void drawCircle(float3 pos, float radius, KDColor color);
}