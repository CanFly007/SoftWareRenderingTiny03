#include "tgaimage.h"
#include "core/Model.h"
#include "core/Pipeline.h"
#include "core/Camera.h"
#include "platform/win32.h"

using namespace std;

const Vec3 EYE(0, 5, 5);
const Vec3 UP(0, 1, 0);
const Vec3 TARGET(0, 0, 0);

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

Vec3 TestWorldToScreen(const Vec3& worldPos)
{
    //return Vec3(int((worldPos.x + 1.0) * WINDOW_WIDTH / 2.0 + 0.5), int((worldPos.y + 1.0) * WINDOW_HEIGHT / 2.0 + 0.5), worldPos.z);

    int x = (worldPos.x + 1.0) * 0.5 * WINDOW_WIDTH;//[-1,1]->[0,WIDTH]
    int y = (worldPos.y + 1.0) * 0.5 * WINDOW_HEIGHT;
    float z = worldPos.z;//[-1,1]
    return Vec3(x, y, z);
}
//�ӿڱ任��[-1,1]�䵽��Ļ��[0,width],z���䣬����[-1,1]
Mat4 viewport()
{
    //return Vec3f((worldPos.x + 1.0) * 0.5 * width, (worldPos.y + 1.0) * 0.5 * height, worldPos.z);
    Mat4 m = Mat4::identity();
    m[0][0] = WINDOW_WIDTH / 2.0;
    m[0][3] = WINDOW_WIDTH / 2.0;
    m[1][1] = WINDOW_HEIGHT / 2.0;
    m[1][3] = WINDOW_HEIGHT / 2.0;
    return m;
}

void ClearFramebuffer(int width, int height, unsigned char* framebuffer);
void ClearZbuffer(int width, int height, float* zbuffer);

int main() 
{
    int width = WINDOW_WIDTH, height = WINDOW_HEIGHT;
    float* zbuffer = (float*)malloc(sizeof(float) * width * height);
    unsigned char* framebuffer = (unsigned char*)malloc(sizeof(unsigned char) * width * height * 4);
    memset(framebuffer, 0, sizeof(unsigned char) * width * height * 4);//ȫ����Ϊ0

    //�������
    float cameraWidth = 3.0;
    float cameraHeight = 3.0;
    float cameraFarPlane = 100.0;
    float cameraNearPlane = 1.0;
    Camera camera(EYE, TARGET, UP, (float)(width) / height);
    //ת������
    Mat4 model_mat = Mat4::identity();
    Mat4 view_mat = WorldToViewMat(camera.eye, camera.target, camera.up);
    Mat4 perspective_mat = OrthoProjection(cameraWidth, cameraHeight, cameraNearPlane, cameraFarPlane);
    Mat4 MVP = perspective_mat * view_mat * model_mat;
    Mat4 viewPort_mat = viewport();

    //��ʼ��ģ�ͺ�shader
    model = new Model("obj/african_head.obj");
    Vec3 vertPos[3];//һ���������������position
    Vec3 testScreenCoord[3];
    Vec3 testWorldPos[3];
    Vec2 vertUV[3];
    Vec4 orthoSpacePos[3];
    Vec4 divisionPos[3];
    Vec4 viewPortPos[3];

    window_init(WINDOW_WIDTH, WINDOW_HEIGHT, L"SRender");

    //render loop
    int num_frames = 0;
    float print_time = platform_get_time();
    while (!window->is_close)
    {
        float curr_time = platform_get_time();
        // clear buffer
        ClearFramebuffer(width, height, framebuffer);
        ClearZbuffer(width, height, zbuffer);

        // handle events and update view, perspective matrix
        handle_events(camera);
        //update_matrix(camera, view_mat, perspective_mat, shader_model, shader_skybox);

        //Draw Models
        //TGAImage image(WINDOW_WIDTH, WINDOW_HEIGHT, TGAImage::RGB);
        for (int i = 0; i < model->nfaces(); i++)
        {
            for (int j = 0; j < 3; j++)
            {
                vertPos[j] = model->GetVertPos(i, j);
                vertUV[j] = model->GetVertUV(i, j);
                testScreenCoord[j] = TestWorldToScreen(vertPos[j]);
                testWorldPos[j] = vertPos[j];

                orthoSpacePos[j] = MVP * Vec4(vertPos[j], 1.0);
                divisionPos[j] = Vec4(orthoSpacePos[j].x / orthoSpacePos[j].w,
                    orthoSpacePos[j].y / orthoSpacePos[j].w, orthoSpacePos[j].z / orthoSpacePos[j].w, orthoSpacePos[j].w);
                viewPortPos[j] = viewPort_mat * divisionPos[j];
                testScreenCoord[j] = Convert_ToVec3(viewPortPos[j]);
            }

            Vec3 normal = Cross((testWorldPos[2] - testWorldPos[0]), (testWorldPos[1] - testWorldPos[0]));
            float ndotL = normalize(normal) * normalize(Vec3(0, 0, -1));
            ndotL = ndotL > 0 ? ndotL : 0;
            float lambert = ndotL * 255;
            TGAColor color = TGAColor(lambert, lambert, lambert, 255);
            Draw_Triangles(testScreenCoord, framebuffer, zbuffer, color);
        }        
        //��ʾFPS
        num_frames += 1;
        if (curr_time - print_time >= 1)//ÿ1�룬ִ���˶���frames
        {
            int sum_millis = (int)((curr_time - print_time) * 1000);
            int avg_millis = sum_millis / num_frames;
            printf("fps: %3d, avg: %3d ms\n", num_frames, avg_millis);
            num_frames = 0;
            print_time = curr_time;
        }

        // send framebuffer to window 
        window_draw(framebuffer);
        msg_dispatch();
    }
    //std::cout << print_time << std::endl;
    window_destroy();
    system("pause");
    delete model;
    return 0;
}

void ClearZbuffer(int width, int height, float* zbuffer)
{
    for (int i = 0; i < width * height; i++)
        zbuffer[i] = 9999.99;//std::numeric_limits<float>::max();
}
void ClearFramebuffer(int width, int height, unsigned char* framebuffer)
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int index = (i * width + j) * 4;

            framebuffer[index + 2] = 80;
            framebuffer[index + 1] = 56;
            framebuffer[index] = 56;
        }
    }
}