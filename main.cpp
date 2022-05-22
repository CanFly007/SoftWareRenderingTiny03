#include "tgaimage.h"
#include "core/Model.h"
#include "core/Pipeline.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);



Model* model = NULL;


//比如(80,200)和(70,100)这段特殊点，有两个特殊性
//特殊1：x方向上是减小的而不是增大的
//特殊2：Y幅度更大，应该按照Y轴每步走
//线框模式，参数是两个点的屏幕坐标[0,width],color是这条直线的颜色
void line(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color) 
{
    bool yStep = false;
    if (abs(x0 - x1) < abs(y0 - y1))
        yStep = true;

    int beginPoint = x0;
    int endPoint = x1;
    //幅度较小边记录，以便在下面的循环中跟随递进（即幅度较大的每递进1个像素，较小的边递进不满1个像素）
    int followBeginPixel = y0;//跟着较大边递进的初始位置
    int followTotalPixel = y1 - y0;//总共的步长
    if (!yStep)
    {
        beginPoint = x0 <= x1 ? x0 : x1;//从小到大
        endPoint = x0 <= x1 ? x1 : x0;
        followBeginPixel = x0 <= x1 ? y0 : y1;//如果begin是x0，跟随点就选y0.如果begin是x1，跟随点就选y1
        followTotalPixel = abs(y1 - y0);
        //判断跟随递进边是递增还是递减，c++没有取符号方法，所以用下面的
        int signCheck = x0 <= x1 ? (y1 - y0) : (y0 - y1);
        signCheck = signCheck >= 0 ? 1 : -1;
        followTotalPixel *= signCheck;
    }
    else
    {
        beginPoint = y0 <= y1 ? y0 : y1;
        endPoint = y0 <= y1 ? y1 : y0;
        followBeginPixel = y0 <= y1 ? x0 : x1;
        followTotalPixel = abs(x1 - x0);
        
        int signCheck = y0 <= y1 ? (x1 - x0) : (x0 - x1);
        signCheck = signCheck >= 0 ? 1 : -1;
        followTotalPixel *= signCheck;
    }
    for (int curPoint = beginPoint; curPoint <= endPoint; curPoint++)//beginPoint一定小于endPoint，且curPoint是按照幅度较大的每步走
    {
        float t = (curPoint - beginPoint) / (float)(endPoint - beginPoint);//作者的优化在这里，t几乎是相同的可以拿到循环外面去
        int lesserSideCurPixel = followBeginPixel + t * followTotalPixel;//幅度较小的边目前递进到的位置
        if (!yStep)
            image.set(curPoint, lesserSideCurPixel, color);
        else
            image.set(lesserSideCurPixel, curPoint, color);
    }
}

Vec3 TestWorldToScreen(const Vec3& worldPos)
{
    //return Vec3(int((worldPos.x + 1.0) * WINDOW_WIDTH / 2.0 + 0.5), int((worldPos.y + 1.0) * WINDOW_HEIGHT / 2.0 + 0.5), worldPos.z);

    int x = (worldPos.x + 1.0) * 0.5 * WINDOW_WIDTH;//[-1,1]->[0,WIDTH]
    int y = (worldPos.y + 1.0) * 0.5 * WINDOW_HEIGHT;
    float z = worldPos.z;//[-1,1]
    return Vec3(x, y, z);
}

int main(int argc, char** argv) 
{
    int width = WINDOW_WIDTH, height = WINDOW_HEIGHT;
    float* zbuffer = (float*)malloc(sizeof(float) * width * height);
    for (int i = 0; i < width * height; i++)
        zbuffer[i] = -std::numeric_limits<float>::max();

    if (2 == argc) {
        model = new Model(argv[1]);
    }
    else {
        model = new Model("obj/african_head.obj");
        //model = new Model("obj/test.obj");
    }

    TGAImage image(WINDOW_WIDTH, WINDOW_HEIGHT, TGAImage::RGB);
    for (int i = 0; i < model->nfaces(); i++)
    {
        Vec3 vertPos[3];//一个面上三个顶点的position
        Vec3 testScreenCoord[3];
        Vec3 testWorldPos[3];
        Vec2 vertUV[3];
        for (int j = 0; j < 3; j++)
        {
            vertPos[j] = model->GetVertPos(i, j);
            vertUV[j] = model->GetVertUV(i, j);
            testScreenCoord[j] = TestWorldToScreen(vertPos[j]);
            testWorldPos[j] = vertPos[j];
        }
        Vec3 normal = Cross((testWorldPos[2] - testWorldPos[0]), (testWorldPos[1] - testWorldPos[0]));
        float ndotL = normalize(normal) * normalize(Vec3(0, 0, -1));
        ndotL = ndotL > 0 ? ndotL : 0;
        //if (ndotL > 0)

        {
            float lambert = ndotL * 255;
            TGAColor color = TGAColor(lambert, lambert, lambert, 255);
            Draw_Triangles(testScreenCoord, zbuffer, image, color);
            //triangle(testScreenCoord,zbuffer,vertUV,model, image, color);
        }

    }
    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("output.tga");

    delete model;
    return 0;
}