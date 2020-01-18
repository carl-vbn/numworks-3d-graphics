#ifndef RAYMARCHING_MATH_UTILS_H
#define RAYMARCHING_MATH_UTILS_H

#include "math.h"

struct float3 {
    float x;
    float y;
    float z;

    float3() {
        x = 0; 
        y = 0;
        z = 0;
    }

    float3(float _x, float _y, float _z) {
        x = _x;
        y = _y;
        z = _z;
    }
};

float distance(float3 pos1, float3 pos2);
float length(float3 vector);
float max(float a, float b);
float min(float a, float b);
float lerp(float a, float b, float alpha);
float3 multiply(float3 vec, float scalar);
float3 difference(float3 a, float3 b);
float3 sum(float3 a, float3 b);
float3 abs(float3 x);
float3 normalize(float3 vec);

#endif