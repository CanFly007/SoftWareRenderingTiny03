#include "./Pipeline.h"

static Vec3 Compute_Barycentric2D(Vec2* screenCoordArray,Vec2 P)
{
    Vec2 A = Vec2(screenCoordArray[0]);//фад╩©у╪Д
    Vec2 B = Vec2(screenCoordArray[1]);
    Vec2 C = Vec2(screenCoordArray[2]);
    float areaABC = Cross(B - A, C - A);
    float c1 = Cross(C - B, P - B) / areaABC;
    float c2 = Cross(P - A, C - A) / areaABC;
    return Vec3(c1, c2, 1 - c1 - c2);
}

void Draw_Triangles(Vec2* screenCoordArray,TGAImage& image)
{
    Vec2 boxMin = Vec2(WINDOW_WIDTH, WINDOW_HEIGHT);
    Vec2 boxMax = Vec2(0, 0);
    for (int i = 0; i < 3; i++)
    {
        Vec2 curPoint = screenCoordArray[i];
        boxMin.x = boxMin.x <= curPoint.x ? boxMin.x : curPoint.x;
        boxMin.y = boxMin.y <= curPoint.y ? boxMin.y : curPoint.y;
        boxMax.x = boxMax.x >= curPoint.x ? boxMax.x : curPoint.x;
        boxMax.y = boxMax.y >= curPoint.y ? boxMax.y : curPoint.y;
    }
    Vec3 barCoord;
    for (int i = boxMin.x; i <= boxMax.x; i++)
    {
        for (int j = boxMin.y; j <= boxMax.y; j++)
        {
            Vec2 curPixel = Vec2(i, j);
            barCoord = Compute_Barycentric2D(screenCoordArray, curPixel);
            if (barCoord.x < 0 || barCoord.y < 0 || barCoord.z < 0)
                continue;
            image.set(i, j, TGAColor(barCoord.x *255, barCoord.y*255, barCoord.z*255, 255));
        }
    }
}