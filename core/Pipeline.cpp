#include "./Pipeline.h"

static Vec3 Compute_Barycentric2D(Vec3* screenCoordArray,Vec2 P)
{
    Vec2 A = Vec2(screenCoordArray[0]);//��Ļ�ռ�
    Vec2 B = Vec2(screenCoordArray[1]);
    Vec2 C = Vec2(screenCoordArray[2]);
    float areaABC = Cross(B - A, C - A);
    if (areaABC == 0)//B��C����һ��ʱ��BA��CA��һ��ֱ����
        return Vec3(-1, -1, -1);
    float c1 = Cross(B - P, C - P) / areaABC;
    float c2 = Cross(C - P, A - P) / areaABC;
    return Vec3(c1, c2, 1 - c1 - c2);
}
Vec3 barycentric(Vec2* pts, Vec2 P)
{
    //����������P1 P2 P3˳����˳ʱ�뻹����ʱ�루�ɻ�ͼ��
    //��ĸ��������������԰���P1P2���P1P3����
    //����������Ҳ��PP1 PP2 PP3˳��
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
//        //��������У�����һ���Ǳ���������
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
//            Vec3 bc_screen = Compute_Barycentric2D(pts, P);//bc_screen��P���ص���������
//            if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0)
//                continue;
//            P.z = 0;
//            P.z += pts[0].z * bc_screen.x;
//            P.z += pts[1].z * bc_screen.y;
//            P.z += pts[2].z * bc_screen.z;//�����P���ص�zֵ
//            //for (int i = 0; i < 3; i++)
//            //    P.z += pts[i].z * bc_screen[i];//�����P���zֵ
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
    //��֮ǰ������βü��޳���TODO�����޳�
    //͸�ӳ���
    Vec3 ndcPosArray[3];//������ÿ��Ԫ�ش���һ��ndcλ��
    for (int i = 0; i < 3; i++)
    {
        ndcPosArray[i].x = clipSpacePos_varying[i].x / clipSpacePos_varying[i].w;
        ndcPosArray[i].y = clipSpacePos_varying[i].y / clipSpacePos_varying[i].w;
        ndcPosArray[i].z = clipSpacePos_varying[i].z / clipSpacePos_varying[i].w;
    }
    //NDC -> �ӿ�ת��
    //return Vec3f((worldPos.x + 1.0) * 0.5 * width, (worldPos.y + 1.0) * 0.5 * height, worldPos.z);
    Vec3 screenSpacePosArray[3];
    int width = window->width;
    int height = window->height;
    bool isSkybox = shader.payload.model->isSkyboxModel;
    for (int i = 0; i < 3; i++)
    {
        screenSpacePosArray[i].x = (ndcPosArray[i].x + 1.0) * 0.5 * width;
        screenSpacePosArray[i].y = (ndcPosArray[i].y + 1.0) * 0.5 * height;
        screenSpacePosArray[i].z = ndcPosArray[i].z;
        if (isSkybox)//skybox��Ҫ�㷨�����⣿��z��������Զ�ĵط�
            screenSpacePosArray[i].z = 1000;//�������պУ���Ȳ���NDC�е�z��ֱ�Ӹ��ǳ�1000��ʾ����Զ
    }
    //ͨ���������귨���ҵ�AABB������Ҫ��Ⱦ������
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
        //Ҫ����boxMin.xy��boxMax.xy��[0,width-1]�У���ȻzBuffer����ʱ���������������ұ߻�ȡ��600��Ӧ��ֻ��ȡ��599����)
        boxMin.x = boxMin.x < 0 ? 0 : boxMin.x;
        boxMin.y = boxMin.y < 0 ? 0 : boxMin.y;
        boxMax.x = boxMax.x > WINDOW_WIDTH - 1 ? WINDOW_WIDTH - 1 : boxMax.x;
        boxMax.y = boxMax.y > WINDOW_HEIGHT - 1 ? WINDOW_HEIGHT - 1 : boxMax.y;
    }
    Vec3 barCoord;
    for (int i = boxMin.x; i <= boxMax.x; i++)//������(0��0) ������(WINDOW_WIDTH-1,WINDOW_HEIGHT-1)
    {
        for (int j = boxMin.y; j <= boxMax.y; j++)
        {
            Vec2 curPixel = Vec2(i, j);
            barCoord = Compute_Barycentric2D(screenSpacePosArray, curPixel);
            if (barCoord.x < 0 || barCoord.y < 0 || barCoord.z < 0)
                continue;

            int index = i * WINDOW_WIDTH + j;
            //ֱ�Ӳ�ֵ��Ļ�ռ������ǲ��Եģ���Ҫ͸�ӽ�����ֵ����ViewSpace�²�ֵ���Z
            //ClipSpace��w��ֵ����ViewSpace��-Zeye��ֵ��ȡ���͵õ�Zeye
            float z = 1.0 / (barCoord[0] / -clipSpacePos_varying[0].w + barCoord[1] / -clipSpacePos_varying[1].w + barCoord[2] / -clipSpacePos_varying[2].w);
            //����Ӧ�����������������ֵ�ɣ�����Ҫ͸�ӽ�����ֵ
            //float z = barCoord[0] * screenSpacePosArray[0].z + barCoord[1] * screenSpacePosArray[1].z + barCoord[2] * screenSpacePosArray[2].z;
            //��Ȳ���
            if (z < zBuffer[index])
            {
                zBuffer[index] = z;//���д��
                //ִ��ƬԪ��ɫ��
                Vec3 color = shader.fragment_shader(barCoord[0], barCoord[1], barCoord[2]);//���[0,255]
                for (int i = 0; i < 3; i++)
                {
                    //c[i] = (int)color[i];//[0,255]
                    c[i] = (int)float_clamp(color[i], 0.0, 255.0);//Vec3 -> unsigned char c[3];
                }
                //Vec3 c = Vec3(color[0], color[1], color[2]);
                set_color(framebuffer, i, j, c);
            }
        }
    }
}

typedef enum
{
    W_PLANE,
    X_LEFT,
    X_RIGHT,
    Y_DOWN,
    Y_UP,
    Z_NEAR,
    Z_FAR
} ClipPlane;

static bool IsInsidePlane(ClipPlane clipPlane, Vec4 clipSpacePos)
{
    switch (clipPlane)
    {
    case W_PLANE:
        return true;// clipSpacePos.w >= EPSILON;//��ֹ����0������һ��wƽ��Ĳü�����0.001�󼴿ɱ�ʾ��wƽ����
    case X_LEFT:
        return clipSpacePos.x >= -clipSpacePos.w;
    case X_RIGHT:
        return clipSpacePos.x <= clipSpacePos.w;
    case Y_DOWN:
        return clipSpacePos.y >= -clipSpacePos.w;
    case Y_UP:
        return clipSpacePos.y <= clipSpacePos.w;
    case Z_NEAR:
        return clipSpacePos.z >= -clipSpacePos.w;
    case Z_FAR:
        return clipSpacePos.z <= clipSpacePos.w;
    default:
        return true;
    }
}

static float GetIntersectRatio(Vec4 start, Vec4 end, ClipPlane clipPlane)
{
    switch (clipPlane)
    {
    case W_PLANE:
        return (EPSILON - start.w) / (end.w - start.w);
    case X_LEFT:
        return (-start.x - start.w) / (end.w - start.w + end.x - start.x);
    case X_RIGHT:
        return (start.x - start.w) / (end.w - start.w - end.x + start.x);
    case Y_DOWN:
        return (-start.y - start.w) / (end.w - start.w + end.y - start.y);
    case Y_UP:
        return (start.y - start.w) / (end.w - start.w - end.y + start.y);
    case Z_NEAR:
        return (-start.z - start.w) / (end.w - start.w + end.z - start.z);
    case Z_FAR:
        return (start.z - start.w) / (end.w - start.w - end.z + start.z);
    default:
        return 0;
    }
}

static int ClipWithPlane(ClipPlane clipPlane, int num_vertex, payload_t& payload)
{
    if (num_vertex == 0)//����һ���ü�ƽ�棬�����μ�������Ľ��㶼���ϸ��ü�ƽ���⣬����ֱ�ӱ��ϸ��ü�ƽ��ü��ˣ����������������
        return 0;

    int out_num_vertex = 0;
    //��ΪinClipSpacePos��һ��ָ�룬���Ե�outClipSpacePosֵ���ı���´�ѭ�����inClipSpacePosҲ���ɴ����
    Vec4 inClipSpacePos[MAX_VERTEX];
    for (int i = 0; i < MAX_VERTEX; i++)
        inClipSpacePos[i] = payload.outClipSpacePos[i];//��һ���ü�ƽ��ü���Ķ���λ������
    //Vec4* inClipSpacePos = payload.outClipSpacePos;
    Vec3 inWorldSpacePos[MAX_VERTEX];
    for (int i = 0; i < MAX_VERTEX; i++)
        inWorldSpacePos[i] = payload.outWorldSpacePos[i];
    //Vec3* inWorldSpacePos = payload.outWorldSpacePos;

    for (int i = 0; i < num_vertex; i++)//�����3���� start�ֱ���0 1 2
    {
        int startIndex = i;
        int endIndex = (i + 1) % num_vertex;//endIndex�Ƿ�Ҫ�жϻ᲻���Ҳ������أ�
        Vec4 start_ClipSpacePos = inClipSpacePos[startIndex];//���ClipSapcePosӦ����ǰһ���ü��ռ��¶�Ӧ������λ�ã�������VertexShader�����������λ��
        Vec4 end_ClipSpacePos = inClipSpacePos[endIndex];
        //ֻ��start in end out��start out end in�����Ҫ�󽻵㡣��start��end����in����start�㣬start��end����out����nothing
        bool isStartInside = IsInsidePlane(clipPlane, start_ClipSpacePos);
        bool isEndInside = IsInsidePlane(clipPlane, end_ClipSpacePos);
        if (isStartInside)//�ȷ���start�ٷ��ؽ���
        {
            payload.outClipSpacePos[out_num_vertex] = start_ClipSpacePos;
            payload.outWorldSpacePos[out_num_vertex] = inWorldSpacePos[startIndex];
            out_num_vertex++;
        }
        if (isStartInside != isEndInside)
        {
            float t = GetIntersectRatio(start_ClipSpacePos, end_ClipSpacePos, clipPlane);
            payload.outClipSpacePos[out_num_vertex] = Vec4_lerp(start_ClipSpacePos, end_ClipSpacePos, t);
            payload.outWorldSpacePos[out_num_vertex] = Vec3_lerp(inWorldSpacePos[startIndex], inWorldSpacePos[endIndex], t);
            out_num_vertex++;
        }
    }
    return out_num_vertex;
}
int HomogeneousClipping(payload_t& payload)
{
    int num_vertex = 3;//�ʼ��3���㣬���ξ���6���ü�ƽ��ü���ʣ�¼�������
    num_vertex = ClipWithPlane(W_PLANE, num_vertex, payload);
    num_vertex = ClipWithPlane(X_LEFT, num_vertex, payload);//�����ϴβü���ʣ�µĶ�������������βü���õ��Ķ���
    num_vertex = ClipWithPlane(X_RIGHT, num_vertex, payload);
    num_vertex = ClipWithPlane(Y_DOWN, num_vertex, payload);
    num_vertex = ClipWithPlane(Y_UP, num_vertex, payload);
    num_vertex = ClipWithPlane(Z_NEAR, num_vertex, payload);
    num_vertex = ClipWithPlane(Z_FAR, num_vertex, payload);
    return num_vertex;
}
static void IntersectAssembly(payload_t& payload, int index0, int index1, int index2)//������н���ģ���Ҫ�������������滻֮ǰ��varying����
{
    payload.clipSpacePos_varying[0] = payload.outClipSpacePos[index0];
    payload.clipSpacePos_varying[1] = payload.outClipSpacePos[index1];
    payload.clipSpacePos_varying[2] = payload.outClipSpacePos[index2];//����н��㣬�򽻵��ԭ���ĵ��������µ�������

    payload.worldSpacePos_varying[0] = payload.outWorldSpacePos[index0];
    payload.worldSpacePos_varying[1] = payload.outWorldSpacePos[index1];
    payload.worldSpacePos_varying[2] = payload.outWorldSpacePos[index2];

}
void Draw_Triangles(unsigned char* framebuffer, float* zBuffer, IShader& shader, int nface)
{
    for (int j = 0; j < 3; j++)
    {
        shader.vertex_shader(nface, j);
    }

    //homogeneous clipping ��βü�����ClipSpace�н��У���͸�ӳ����䵽NDC֮ǰ��
    //��ÿ���ü�ƽ��ü��󣬿��ܷ���0���㣬3�����4����
    int num_vertex = HomogeneousClipping(shader.payload);

    //�����澭����βü���õ����¶��㣬���»�������
    for (int i = 0; i < num_vertex - 2; i++)//3����ѭ��1�� 4����ѭ��2�� 5����ѭ��3����3�������Σ��Դ�����
    {
        int index0 = 0;//ע���������ε���λ����Զ��0
        int index1 = i + 1;
        int index2 = i + 2;
        //����װ����βü���Ķ������ԣ���������Ͳü����н��㣬��ôҪ�㽻������Դ����µ���������
        IntersectAssembly(shader.payload, index0, index1, index2);

        Rasterize_singlethread(shader.payload.clipSpacePos_varying, framebuffer, zBuffer, shader);
    }
}