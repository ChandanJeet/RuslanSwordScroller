#ifndef PLAYGROUND_H
#define PLAYGROUND_H

// Include GLEW
#include <GL/glew.h>
#include "../../../bin/collider.h"


struct float2 {
    float2(float _x = 0.0f, float _y = 0.0f) : x(_x), y(_y) {}
    float x;
    float y;
};

struct float3 {
    float3(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) : x(_x), y(_y), z(_z) {}
    float x;
    float y;
    float z;
};

int main(void); //<<< main function, called at startup

bool Impact_top(collider Solids[], collider Body);
bool Impact_bottom(collider Solids[], collider Body);
bool Impact_left(collider Solids[], collider Body);
bool Impact_right(collider Solids[], collider Body);
bool Impact_sword(collider Blocks[], float TipX, float TipY);

void DrawPerson();
void DrawRectangle(float lenX, float lenY, float posX, float posY, int type);
void DrawAllRect(collider List[], int type);
void Physics();
void Move_block(collider Blocks[]);

template <typename T> int sgn(T val);
#endif

