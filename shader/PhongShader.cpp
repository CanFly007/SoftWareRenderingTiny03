#include "./Shader.h"

void PhongShader::vertex_shader(int nfaces, int nvertex)
{
    Vec3 temp_vertPos = payload.model->GetVertPos(nfaces, nvertex);
    
    payload.uv_attribute[nvertex] = payload.model->GetVertUV(nfaces, nvertex);
    payload.normal_attribute[nvertex] = payload.model->GetVertNormal(nfaces, nvertex);

    payload.worldSpacePos_varying[nvertex] = temp_vertPos;//ʡ��ǰ��Model2World����
    payload.clipSpacePos_varying[nvertex] = payload.MVP_uniform * Vec4(temp_vertPos, 1.0);
}

Vec3 PhongShader::fragment_shader(float alpha, float beta, float gamma)
{
    Vec3 worldPos = payload.worldSpacePos_varying[0] * alpha + payload.worldSpacePos_varying[1] * beta + payload.worldSpacePos_varying[2] * gamma;
    Vec2 uv = payload.uv_attribute[0] * alpha + payload.uv_attribute[1] * beta + payload.uv_attribute[2] * gamma;
    Vec3 normal = payload.normal_attribute[0] * alpha + payload.normal_attribute[1] * beta + payload.normal_attribute[2] * gamma;
    //��������ֵ����SRender�л�������clipSpacePos�е�w����
    
    Vec3 diffuseMap = payload.model->SamplerDiffseColor(uv);//����[0,1]
    
    Vec3 faceNormal = normalize(payload.testFaceNormal_varying);
    Vec3 worldNormal = normalize(normal);
    Vec3 worldLightDir = normalize(Vec3(0, 0, 1));//��faceNomal�ĳɶ���normal���ɹ������ֵ�ˣ���������ϵ��shadingPoint����
    float ndotL = worldNormal * worldLightDir;
    if (ndotL < 0)
        ndotL = 0;

    return(diffuseMap * ndotL * 255.0);
    float lambert = payload.lambertTest_varying;
    return (diffuseMap * lambert);
    return Vec3(lambert, lambert, lambert);
    return Vec3(1, 1, 1);
}