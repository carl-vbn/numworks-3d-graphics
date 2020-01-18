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
        float3 color;

        Sphere(float3 pos, float r, float3 c) {
            position = pos;
            radius = r;
            color = c;
        }

        Sphere(float px, float py, float pz, float r, float cr, float cg, float cb) {
            position = float3(px,py,pz);
            radius = r; 
            color = float3(cr,cg,cb);
        }
    };

    void loadScene(int scene);
    void render(KDRect rect);
    void renderSubRect(KDRect rect, KDRect subrect);

    float sphereSD(float3 pos, float3 spherePos, float sphereRadius);
    float cubeSD(float3 pos, float3 cubePos, float3 cubeSize);

    float distanceFromGeometry(float3 pos);
    RaymarchHit rayMarch(float3 origin, float3 direction);

    void drawLine(float3 pos1, float3 pos2, KDColor color);
    void drawCircle(float3 pos, float radius, KDColor color);
}