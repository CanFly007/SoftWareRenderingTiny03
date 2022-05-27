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


//void triangle(Vec3* pts, float* zbuffer, Vec2* uvArray,Model* model, TGAImage& image, TGAColor color)
//{
//    Vec2 bboxmin(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
//    Vec2 bboxmax(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
//    Vec2 clamp(image.get_width() - 1, image.get_height() - 1);
//    for (int i = 0; i < 3; i++)
//    {
//        //看里面就行，外面一层是保护不出界
//        bboxmin.x = std::max(0.0f, std::min(bboxmin.x, pts[i].x));
//        bboxmin.y = std::max(0.0f, std::min(bboxmin.y, pts[i].y));
//        bboxmax.x = std::min(clamp.x, std::max(bboxmax.x, pts[i].x));
//        bboxmax.y = std::min(clamp.y, std::max(bboxmax.y, pts[i].y));
//    }
//
//    //Vec2 p2[3] = { Vec2(pts[0].x,pts[0].y),Vec2(pts[1].x,pts[1].y), Vec2(pts[2].x,pts[2].y) };
//    Vec3 P;
//    for (P.x = bboxmin.x; P.x <= bboxmax.x; P.x++)
//    {
//        for (P.y = bboxmin.y; P.y <= bboxmax.y; P.y++)
//        {
//            
//            Vec3 bc_screen = Compute_Barycentric2D(pts, P);//bc_screen是P像素的重心坐标
//            if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0)
//                continue;
//            P.z = 0;
//            P.z += pts[0].z * bc_screen.x;
//            P.z += pts[1].z * bc_screen.y;
//            P.z += pts[2].z * bc_screen.z;//这个是P像素的z值
//            //for (int i = 0; i < 3; i++)
//            //    P.z += pts[i].z * bc_screen[i];//这个是P点的z值
//
//            if (zbuffer[int(P.x + P.y * WINDOW_WIDTH)] < P.z)
//            {
//                zbuffer[int(P.x + P.y * WINDOW_WIDTH)] = P.z;
//                //image.set(P.x, P.y, color);
//                Vec2 uv = uvArray[0] * bc_screen.x + 
//                    uvArray[1] * bc_screen.y +
//                    uvArray[2] * bc_screen.z;
//                Vec3 diffuseColor = model->SamplerDiffseColor(uv);
//                TGAColor color = TGAColor(diffuseColor.x * 255, diffuseColor.y * 255, diffuseColor.z * 255, 255);
//                image.set(P.x, P.y, color);
//            }
//        }
//    }
//}

static void set_color(unsigned char* framebuffer, int x, int y, unsigned char color[])
{
    int i;
    int index = ((WINDOW_HEIGHT - y - 1) * WINDOW_WIDTH + x) * 4; // the origin for pixel is bottom-left, but the framebuffer index counts from top-left

    for (i = 0; i < 3; i++)
        framebuffer[index + i] = color[i];
}
void Draw_Triangles(Vec3* screenCoordArray, unsigned char* framebuffer,float* zBuffer,TGAColor color)
{
    unsigned char c[3];

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
            if (z < zBuffer[index])
            {
                zBuffer[index] = z;
                for (int i = 0; i < 3; i++)
                {
                    c[i] = (int)color[i];//[0,255]
                }
                //Vec3 c = Vec3(color[0], color[1], color[2]);
                set_color(framebuffer, i, j, c);
                //image.set(i, j, color);
            }
            //image.set(i, j, TGAColor(barCoord.x *255, barCoord.y*255, barCoord.z*255, 255));
        }
    }
}

void Rasterize_singlethread(Vec4* clipSpacePos_varying, unsigned char* framebuffer, float* zBuffer, IShader& shader)
{
    //透视除法
    Vec3 ndcPosArray[3];//数组中每个元素代表一个ndc位置
    for (int i = 0; i < 3; i++)
    {
        ndcPosArray[i].x = clipSpacePos_varying[i].x / clipSpacePos_varying[i].w;
        ndcPosArray[i].y = clipSpacePos_varying[i].y / clipSpacePos_varying[i].w;
        ndcPosArray[i].z = clipSpacePos_varying[i].z / clipSpacePos_varying[i].w;
    }
    //NDC -> 视口转换
    //return Vec3f((worldPos.x + 1.0) * 0.5 * width, (worldPos.y + 1.0) * 0.5 * height, worldPos.z);
    Vec3 screenSpacePosArray[3];
    int width = window->width;
    int height = window->height;
    for (int i = 0; i < 3; i++)
    {
        screenSpacePosArray[i].x = (ndcPosArray[i].x + 1.0) * 0.5 * width;
        screenSpacePosArray[i].y = (ndcPosArray[i].y + 1.0) * 0.5 * height;
        screenSpacePosArray[i].z = ndcPosArray[i].z;
    }
    //通过重心坐标法，找到AABB矩形中要渲染的像素
    unsigned char c[3];
    Vec2 boxMin = Vec2(WINDOW_WIDTH, WINDOW_HEIGHT);
    Vec2 boxMax = Vec2(0, 0);
    for (int i = 0; i < 3; i++)
    {
        Vec2 curPoint = Vec2(screenSpacePosArray[i]);
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
            barCoord = Compute_Barycentric2D(screenSpacePosArray, curPixel);
            if (barCoord.x < 0 || barCoord.y < 0 || barCoord.z < 0)
                continue;

            int index = i * WINDOW_WIDTH + j;
            float z = barCoord[0] * screenSpacePosArray[0].z + barCoord[1] * screenSpacePosArray[1].z + barCoord[2] * screenSpacePosArray[2].z;
            //深度测试
            if (z < zBuffer[index])
            {
                zBuffer[index] = z;//深度写入
                //执行片元着色器
                Vec3 color = shader.fragment_shader(barCoord[0], barCoord[1], barCoord[2]);//输出[0,255]
                for (int i = 0; i < 3; i++)
                {
                    //c[i] = (int)color[i];//[0,255]
                    c[i] = color[i];//Vec3 -> unsigned char c[3];
                }
                //Vec3 c = Vec3(color[0], color[1], color[2]);
                set_color(framebuffer, i, j, c);
            }
        }
    }
}

void Draw_Triangles(unsigned char* framebuffer, float* zBuffer, IShader& shader, int nface)
{
    for (int j = 0; j < 3; j++)
    {
        shader.vertex_shader(nface, j);
    }
    //测试fragment是否正常工作，所以在这里虚拟算下faceNormal
    Vec3 testWorldPos[3] = { shader.payload.worldSpacePos_varying[0],shader.payload.worldSpacePos_varying[1],
        shader.payload.worldSpacePos_varying[2] };
    Vec3 faceNormal = Cross((testWorldPos[2] - testWorldPos[0]), (testWorldPos[1] - testWorldPos[0]));
    float ndotL = normalize(faceNormal) * normalize(Vec3(0, 0, -1));
    ndotL = ndotL > 0 ? ndotL : 0;
    float lambert = ndotL * 255;
    shader.payload.lambertTest_varying = lambert;
    shader.payload.testFaceNormal_varying = normalize(faceNormal);

    Rasterize_singlethread(shader.payload.clipSpacePos_varying, framebuffer, zBuffer, shader);
}