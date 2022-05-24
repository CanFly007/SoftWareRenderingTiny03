#ifndef PIPELINE_H
#define PIPELINE_H

#include "./Maths.h"

const int WINDOW_WIDTH = 600;
const int WINDOW_HEIGHT = 600;

#include "../tgaimage.h"
#include "Model.h"
void Draw_Triangles(Vec3* screenCoordArray, unsigned char* framebuffer, float* zBuffer, TGAColor color);
void triangle(Vec3* pts, float* zbuffer, Vec2* uvArray,Model* model, TGAImage& image, TGAColor color);
#endif // !PIPELINE_H
