#include "./Pipeline.h"

static Vec3 Compute_Barycentric2D(Vec3* screenCoordArray,Vec2 P)
{
    Vec2 A = Vec2(screenCoordArray[0]);//屏幕空间
    Vec2 B = Vec2(screenCoordArray[1]);
    Vec2 C = Vec2(screenCoordArray[2]);
    float areaABC = Cross(B - A, C - A);
    float c1 = Cross(B - P, C - P) / areaABC;
    float c2 = Cross(C - P, A - P) / areaABC;
    return Vec3(c1, c2, 1 - c1 - c2);
}
Vec3 barycentric(Vec2* pts, Vec2 P)
{
    //不管三角形P1 P2 P3顺序按照顺时针还是逆时针（可画图）
    //分母三角形面积都可以按照P1P2叉乘P1P3来算
    //分子三角形也是PP1 PP2 PP3顺序
    //P1P2 P1P3
    float triangleArea = 0.5 * Cross((pts[1] - pts[0]) , (pts[2] - pts[0]));
    //PP2 PP3
    float uTriangle = 0.5 * Cross((pts[1] - P) , (pts[2] - P));
    //PP3 PP1
    float vTriangle = 0.5 * Cross((pts[2] - P) , (pts[0] - P));
    //PP1 PP2
    float wTriangle = 0.5 * Cross((pts[0] - P) , (pts[1] - P));

    float u = uTriangle / triangleArea;
    float v = vTriangle / triangleArea;
    float w = wTriangle / triangleArea;
    if (u < 0 || v < 0 || w < 0)
        return Vec3(-1, -1, -1);
    return Vec3(u, v, w);
}


void triangle(Vec3* pts, float* zbuffer, Vec2* uvArray,Model* model, TGAImage& image, TGAColor color)
{
    Vec2 bboxmin(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    Vec2 bboxmax(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
    Vec2 clamp(image.get_width() - 1, image.get_height() - 1);
    for (int i = 0; i < 3; i++)
    {
        //看里面就行，外面一层是保护不出界
        bboxmin.x = std::max(0.0f, std::min(bboxmin.x, pts[i].x));
        bboxmin.y = std::max(0.0f, std::min(bboxmin.y, pts[i].y));
        bboxmax.x = std::min(clamp.x, std::max(bboxmax.x, pts[i].x));
        bboxmax.y = std::min(clamp.y, std::max(bboxmax.y, pts[i].y));
    }

    //Vec2 p2[3] = { Vec2(pts[0].x,pts[0].y),Vec2(pts[1].x,pts[1].y), Vec2(pts[2].x,pts[2].y) };
    Vec3 P;
    for (P.x = bboxmin.x; P.x <= bboxmax.x; P.x++)
    {
        for (P.y = bboxmin.y; P.y <= bboxmax.y; P.y++)
        {
            
            Vec3 bc_screen = Compute_Barycentric2D(pts, P);//bc_screen是P像素的重心坐标
            if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0)
                continue;
            P.z = 0;
            P.z += pts[0].z * bc_screen.x;
            P.z += pts[1].z * bc_screen.y;
            P.z += pts[2].z * bc_screen.z;//这个是P像素的z值
            //for (int i = 0; i < 3; i++)
            //    P.z += pts[i].z * bc_screen[i];//这个是P点的z值

            if (zbuffer[int(P.x + P.y * WINDOW_WIDTH)] < P.z)
            {
                zbuffer[int(P.x + P.y * WINDOW_WIDTH)] = P.z;
                //image.set(P.x, P.y, color);
                Vec2 uv = uvArray[0] * bc_screen.x + 
                    uvArray[1] * bc_screen.y +
                    uvArray[2] * bc_screen.z;
                TGAColor diffuseColor = model->SamplerDiffseColor(uv);
                image.set(P.x, P.y, diffuseColor);
            }
        }
    }
}


void Draw_Triangles(Vec3* screenCoordArray,float* zBuffer,TGAImage& image,TGAColor color)
{
    Vec2 boxMin = Vec2(WINDOW_WIDTH, WINDOW_HEIGHT);
    Vec2 boxMax = Vec2(0, 0);
    for (int i = 0; i < 3; i++)
    {
        Vec2 curPoint = Vec2(screenCoordArray[i]);
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

            int index = i * WINDOW_WIDTH + j;
            float z = barCoord[0] * screenCoordArray[0].z + barCoord[1] * screenCoordArray[1].z + barCoord[2] * screenCoordArray[2].z;
            if (z > zBuffer[index])
            {
                zBuffer[index] = z;
                image.set(i, j, color);
            }
            //image.set(i, j, TGAColor(barCoord.x *255, barCoord.y*255, barCoord.z*255, 255));
        }
    }
}