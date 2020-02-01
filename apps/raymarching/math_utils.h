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
float dot(float3 a, float3 b);
float max(float a, float b);
float min(float a, float b);
float lerp(float a, float b, float alpha);
float3 multiply(float3 vec, float scalar);
float3 difference(float3 a, float3 b);
float3 abs(float3 x);
float3 sum(float3 a, float3 b);
void add(float3 * vec, float3 addend);
void normalize(float3 * vec);

/* Used to texture an object with a checkerboard pattern. Returns true every other cell */
bool checkerPattern(int x, int y, int size);

#endif