#include "tgaimage.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);

//比如(80,200)和(70,100)这段特殊点，有两个特殊性
//特殊1：x方向上是减小的而不是增大的
//特殊2：Y幅度更大，应该按照Y轴每步走
//线框模式
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
        followBeginPixel = y0 <= y1 ? y0 : y1;
        followTotalPixel = abs(y1 - y0);
    }
    else
    {
        beginPoint = y0 <= y1 ? y0 : y1;
        endPoint = y0 <= y1 ? y1 : y0;
        followBeginPixel = x0 <= x1 ? x0 : x1;
        followTotalPixel = abs(x1 - x0);
    }
    for (int curPoint = beginPoint; curPoint < endPoint; curPoint++)//beginPoint一定小于endPoint，且curPoint是按照幅度较大的每步走
    {
        float t = (curPoint - beginPoint) / (float)(endPoint - beginPoint);//作者的优化在这里，t几乎是相同的可以拿到循环外面去
        int lesserSideCurPixel = followBeginPixel + t * followTotalPixel;//幅度较小的边目前递进到的位置
        if (!yStep)
            image.set(curPoint, lesserSideCurPixel, color);
        else
            image.set(lesserSideCurPixel, curPoint, color);
    }
}
int main(int argc, char** argv) {
    TGAImage image(100, 100, TGAImage::RGB);
    line(13, 20, 80, 40, image, white);
    line(20, 13, 40, 80, image, red);
    line(80, 40, 13, 20, image, red);
    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("output.tga");
    return 0;
}