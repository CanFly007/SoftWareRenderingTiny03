#ifndef PIPELINE_H
#define PIPELINE_H

#include "./Maths.h"

const int WINDOW_WIDTH = 600;
const int WINDOW_HEIGHT = 600;

#include "../tgaimage.h"
void Draw_Triangles(Vec2* screenCoordArray, TGAImage& image);
#endif // !PIPELINE_H
