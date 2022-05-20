#include "tgaimage.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);

//����(80,200)��(70,100)�������㣬������������
//����1��x�������Ǽ�С�Ķ����������
//����2��Y���ȸ���Ӧ�ð���Y��ÿ����
//�߿�ģʽ
void line(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color) 
{
    bool yStep = false;
    if (abs(x0 - x1) < abs(y0 - y1))
        yStep = true;

    int beginPoint = x0;
    int endPoint = x1;
    //���Ƚ�С�߼�¼���Ա��������ѭ���и���ݽ��������Ƚϴ��ÿ�ݽ�1�����أ���С�ıߵݽ�����1�����أ�
    int followBeginPixel = y0;//���Žϴ�ߵݽ��ĳ�ʼλ��
    int followTotalPixel = y1 - y0;//�ܹ��Ĳ���
    if (!yStep)
    {
        beginPoint = x0 <= x1 ? x0 : x1;//��С����
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
    for (int curPoint = beginPoint; curPoint < endPoint; curPoint++)//beginPointһ��С��endPoint����curPoint�ǰ��շ��Ƚϴ��ÿ����
    {
        float t = (curPoint - beginPoint) / (float)(endPoint - beginPoint);//���ߵ��Ż������t��������ͬ�Ŀ����õ�ѭ������ȥ
        int lesserSideCurPixel = followBeginPixel + t * followTotalPixel;//���Ƚ�С�ı�Ŀǰ�ݽ�����λ��
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