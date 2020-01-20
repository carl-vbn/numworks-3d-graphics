#include "math_utils.h"

float distance(float3 pos1, float3 pos2) {
    float xd = pos1.x - pos2.x;
    float yd = pos1.y - pos2.y;
    float zd = pos1.z - pos2.z;

    return sqrt(xd*xd+yd*yd+zd*zd);
}

float length(float3 vector) {
    return sqrt(vector.x*vector.x+vector.y*vector.y+vector.z*vector.z);
}

float max(float a, float b) {
    if (a < b)
        return b;
    else
        return a;
}

float min(float a, float b) {
    if (a > b)
        return b;
    else
        return a;
}

float lerp(float a, float b, float alpha) {
    return a * (1 - alpha) + b * alpha;
}

float3 difference(float3 a, float3 b) {
    struct float3 diff;
    diff.x = a.x - b.x;
    diff.y = a.y - b.y;
    diff.z = a.z - b.z;
    return diff;
}

float3 sum(float3 a, float3 b) {
    struct float3 s;
    s.x = a.x + b.x;
    s.y = a.y + b.y;
    s.z = a.z + b.z;
    return s;
}


float3 abs(float3 x) {
    struct float3 x_abs = float3(x);
    if (x.x < 0)
        x.x = -x.x;
    if (x.y < 0)
        x.y = -x.y;
    if (x.z < 0)
        x.z = -x.z;
    
    return x_abs;
}

float3 normalize(float3 vec) {
    float magnitude = length(vec);
    return float3(vec.x / magnitude, vec.y / magnitude, vec.z / magnitude);
}

float3 multiply(float3 vec, float scalar) {
    return float3(vec.x*scalar, vec.y*scalar,vec.z*scalar);
}

float dot(float3 a, float3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

bool checkerPattern(int x, int y, int size) {

    return (x % size*2 <size)^(y % size*2 <size);
}