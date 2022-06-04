#include "./Shader.h"
#include "../core/Sample.h"

void PhongShader::vertex_shader(int nfaces, int nvertex)
{
    Vec3 temp_vertPos = payload.model->GetVertPos(nfaces, nvertex);
    
    payload.uv_attribute[nvertex] = payload.model->GetVertUV(nfaces, nvertex);
    payload.normal_attribute[nvertex] = payload.model->GetVertNormal(nfaces, nvertex);

    payload.worldSpacePos_varying[nvertex] = temp_vertPos;//ʡ��ǰ��Model2World����
    payload.clipSpacePos_varying[nvertex] = payload.MVP_uniform * Vec4(temp_vertPos, 1.0);
    //Ϊ����βü�
    payload.outClipSpacePos[nvertex] = payload.clipSpacePos_varying[nvertex];//��û��βü�ǰ����һ���ü�������������Clip���꣬��������
    payload.outWorldSpacePos[nvertex] = payload.worldSpacePos_varying[nvertex];
}

//���������������������λ�ã����������UVֵ���������T��B����
//ע�⣺ʩ������������Ҫ�õ�������صķ�����Ϣ�������㷨�߲�ֵ��normal��
//��֪�������position��uv���Ѿ������ص�normal��������� �����ߺ͸����ߣ������TBN����
static Mat3 Cal_TangentToWorldMat(Vec3* worldPosArray, const Vec2* uvArray, Vec3& normal)
{
    //���������㷨ӡ��ʼ�
    // [T] = [t1 b1]���� * [AB]
    // [B] = [t2 b2]       [AC]
    //���� t1 = B.u - A.u; b1 = B.v - A.v; t2 = C.u - A.u; b2 = C.v - A.v
    Vec3 AB = worldPosArray[1] - worldPosArray[0];
    Vec3 AC = worldPosArray[2] - worldPosArray[0];
    float t1 = uvArray[1].u - uvArray[0].u;//AB������U��V��
    float b1 = uvArray[1].v - uvArray[0].v;
    float t2 = uvArray[2].u - uvArray[0].u;//AC������U��V��
    float b2 = uvArray[2].v - uvArray[0].v;

    float determinant = t1 * b2 - b1 * t2;
    Vec3 worldSpaceT = b2 * AB - b1 * AC;
    Vec3 worldSpaceB = -t2 * AB + t1 * AC;
    worldSpaceT = worldSpaceT / determinant;
    worldSpaceB = worldSpaceB / determinant;

    //ʩ����������
    normal = normalize(normal);
    worldSpaceT = normalize(worldSpaceT - (worldSpaceT * normal) * normal);
    worldSpaceB = normalize(worldSpaceB - (worldSpaceB * normal) * normal - (worldSpaceB * worldSpaceT) * worldSpaceT);

    //TBN�������򣬹��� T->W�ľ���
    Mat3 tangent2World;
    tangent2World[0][0] = worldSpaceT.x; tangent2World[0][1] = worldSpaceB.x; tangent2World[0][2] = normal.x;
    tangent2World[1][0] = worldSpaceT.y; tangent2World[1][1] = worldSpaceB.y; tangent2World[1][2] = normal.y;
    tangent2World[2][0] = worldSpaceT.z; tangent2World[2][1] = worldSpaceB.z; tangent2World[2][2] = normal.z;
    return tangent2World;
}

Vec3 PhongShader::fragment_shader(float alpha, float beta, float gamma)
{
    Vec3 worldPos = payload.worldSpacePos_varying[0] * alpha + payload.worldSpacePos_varying[1] * beta + payload.worldSpacePos_varying[2] * gamma;
    Vec2 uv = payload.uv_attribute[0] * alpha + payload.uv_attribute[1] * beta + payload.uv_attribute[2] * gamma;
    Vec3 normal = payload.normal_attribute[0] * alpha + payload.normal_attribute[1] * beta + payload.normal_attribute[2] * gamma;
    //��������ֵ����SRender�л�������clipSpacePos�е�w����
    
    Vec3 diffuseMap = payload.model->SamplerDiffseColor(uv);//����[0,1]

    Vec3 worldNormal = normal;
    if (payload.model->normalMap)//��normalMap,�������߿ռ�ת������
    {
        Mat3 tangent2World = Cal_TangentToWorldMat(payload.worldSpacePos_varying, payload.uv_attribute, normal);
        Vec3 normalMap = texture_sample(uv, payload.model->normalMap);//TGA��bgr˳��texture_sample��������ת��rgb˳���ҷ�Χ��С��[0,1]
        normalMap = normalMap * 2 - 1;//Unity�е�UnpackNormal�����ǰ�[0,1]���[-1,1]
        worldNormal = tangent2World * normalMap;
    }
 
    Vec3 worldLightDir = normalize(Vec3(1, 1, 1));//��faceNomal�ĳɶ���normal���ɹ������ֵ�ˣ���������ϵ��shadingPoint����
    float ndotL = worldNormal * worldLightDir;

    Vec3 worldViewDir = normalize(payload.camera->eye - worldPos);
    Vec3 halfDir = normalize(worldViewDir + worldLightDir);
    float ndotH = worldNormal * halfDir;
    
    Vec3 ambient = 0.35 * diffuseMap;
    Vec3 diffuse = 0.9 * diffuseMap * float_max(0.0, ndotL);
    Vec3 specular = Vec3(0.8, 0.8, 0.8) * 0.15 * float_max(0.0, pow(ndotH, 150));//0.8ָ���Ǹ߹����ɫ
    return(ambient + diffuse + specular) * 255.0;
}