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
        bool checkerPattern;

        Sphere() {
            checkerPattern = true;
        }

        Sphere(float3 pos, float r, KDColor c) {
            position = pos;
            radius = r;
            color = c;
            checkerPattern = false;
        }

        Sphere(float3 pos, float r) {
            position = pos;
            radius = r;
            checkerPattern = true;
        }

        Sphere(float px, float py, float pz, float r, KDColor c) {
            position = float3(px,py,pz);
            radius = r; 
            color = c;
            checkerPattern = false;
        }

        Sphere(float px, float py, float pz, float r) {
            position = float3(px,py,pz);
            radius = r; 
            checkerPattern = true;
        }

    };

    float sphereSD(float3 pos, float3 spherePos, float sphereRadius);
    float cubeSD(float3 pos, float3 cubePos, float3 cubeSize);
    
    class RaymarchingScene  {
        public:
            RaymarchingScene(int scene);
            ~RaymarchingScene();
            void render(KDRect rect, bool fast=false);
            void translateCamera(float3 vec);
            void rotateCamera(float yaw, float pitch=0);

            float3 getCameraVector(float yawOffset=0);

        protected:
            float3 m_camPos;
            float m_camYaw;
            float m_camPitch;
            float3 m_lightPos;
            Sphere * m_loadedSpheres;
            int m_loadedSphereCount;

            float distanceFromGeometry(float3 pos, Sphere * nearestSphere = nullptr);
            float3 estimateNormal(float3 pos);
            RaymarchHit rayMarch(float3 origin, float3 direction);

    };
    void drawLine(float3 pos1, float3 pos2, KDColor color);
    void drawCircle(float3 pos, float radius, KDColor color);
}