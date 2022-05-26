#ifndef PIPELINE_H
#define PIPELINE_H

#include "./Maths.h"
#include "../shader/shader.h"
#include "../platform/win32.h"

const int WINDOW_WIDTH = 600;
const int WINDOW_HEIGHT = 600;

#include "../tgaimage.h"
#include "Model.h"
void Rasterize_singlethread(Vec4*, unsigned char*, float*, IShader&);
void Draw_Triangles(Vec3* screenCoordArray, unsigned char* framebuffer, float* zBuffer, TGAColor color);
//void triangle(Vec3* pts, float* zbuffer, Vec2* uvArray,Model* model, TGAImage& image, TGAColor color);
void Draw_Triangles(unsigned char* framebuffer, float* zBuffer, IShader& shader, int nface);
#endif // !PIPELINE_H
