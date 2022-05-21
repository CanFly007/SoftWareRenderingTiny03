#include "tgaimage.h"
#include "core/Model.h"
#include "core/Pipeline.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);



Model* model = NULL;


//����(80,200)��(70,100)�������㣬������������
//����1��x�������Ǽ�С�Ķ����������
//����2��Y���ȸ���Ӧ�ð���Y��ÿ����
//�߿�ģʽ�����������������Ļ����[0,width],color������ֱ�ߵ���ɫ
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
        followBeginPixel = x0 <= x1 ? y0 : y1;//���begin��x0��������ѡy0.���begin��x1��������ѡy1
        followTotalPixel = abs(y1 - y0);
        //�жϸ���ݽ����ǵ������ǵݼ���c++û��ȡ���ŷ����������������
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
    for (int curPoint = beginPoint; curPoint <= endPoint; curPoint++)//beginPointһ��С��endPoint����curPoint�ǰ��շ��Ƚϴ��ÿ����
    {
        float t = (curPoint - beginPoint) / (float)(endPoint - beginPoint);//���ߵ��Ż������t��������ͬ�Ŀ����õ�ѭ������ȥ
        int lesserSideCurPixel = followBeginPixel + t * followTotalPixel;//���Ƚ�С�ı�Ŀǰ�ݽ�����λ��
        if (!yStep)
            image.set(curPoint, lesserSideCurPixel, color);
        else
            image.set(lesserSideCurPixel, curPoint, color);
    }
}

Vec2 TestWorldToScreen(const Vec3& worldPos)
{
    int x = (worldPos.x + 1.0) * 0.5 * WINDOW_WIDTH;//[-1,1]->[0,WIDTH]
    int y = (worldPos.y + 1.0) * 0.5 * WINDOW_HEIGHT;
    return Vec2(x, y);
}

int main(int argc, char** argv) {
    if (2 == argc) {
        model = new Model(argv[1]);
    }
    else {
        model = new Model("obj/african_head.obj");
    }

    TGAImage image(WINDOW_WIDTH, WINDOW_HEIGHT, TGAImage::RGB);
    for (int i = 0; i < model->nfaces(); i++)
    {
        Vec3 vertPos[3];//һ���������������position
        Vec2 testScreenCoord[3];
        for (int j = 0; j < 3; j++)
        {
            vertPos[j] = model->GetVertPos(i, j);
            testScreenCoord[j] = TestWorldToScreen(vertPos[j]);
        }
        Draw_Triangles(testScreenCoord, image);
    }
    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("output.tga");

    delete model;
    return 0;
}