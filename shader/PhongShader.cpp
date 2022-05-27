#include "./Shader.h"
#include "../core/Sample.h"

void PhongShader::vertex_shader(int nfaces, int nvertex)
{
    Vec3 temp_vertPos = payload.model->GetVertPos(nfaces, nvertex);
    
    payload.uv_attribute[nvertex] = payload.model->GetVertUV(nfaces, nvertex);
    payload.normal_attribute[nvertex] = payload.model->GetVertNormal(nfaces, nvertex);

    payload.worldSpacePos_varying[nvertex] = temp_vertPos;//省略前乘Model2World矩阵
    payload.clipSpacePos_varying[nvertex] = payload.MVP_uniform * Vec4(temp_vertPos, 1.0);
}

static Vec3 cal_normal(Vec3& normal, Vec3* world_coords, const Vec2* uvs, const Vec2& uv, TGAImage* normal_map)
{
    // calculate the difference in UV coordinate
    float x1 = uvs[1][0] - uvs[0][0];
    float y1 = uvs[1][1] - uvs[0][1];
    float x2 = uvs[2][0] - uvs[0][0];
    float y2 = uvs[2][1] - uvs[0][1];
    float det = (x1 * y2 - x2 * y1);

    // calculate the difference in world pos
    Vec3 e1 = world_coords[1] - world_coords[0];
    Vec3 e2 = world_coords[2] - world_coords[0];

    Vec3 t = e1 * y2 + e2 * (-y1);
    Vec3 b = e1 * (-x2) + e2 * x1;
    t =t/ det;
    b =b/ det;

    // schmidt orthogonalization
    normal = normalize(normal);
    t = normalize(t - (t* normal) * normal);
    b = normalize(b - (b* normal) * normal - (b* t) * t);

    Vec3 sample = texture_sample(uv, normal_map);
    // modify the range from 0 ~ 1 to -1 ~ +1
    sample = Vec3(sample[0] * 2 - 1, sample[1] * 2 - 1, sample[2] * 2 - 1);

    Vec3 normal_new = t * sample[0] + b * sample[1] + normal * sample[2];
    return normal_new;
}

Vec3 PhongShader::fragment_shader(float alpha, float beta, float gamma)
{
    Vec3 worldPos = payload.worldSpacePos_varying[0] * alpha + payload.worldSpacePos_varying[1] * beta + payload.worldSpacePos_varying[2] * gamma;
    Vec2 uv = payload.uv_attribute[0] * alpha + payload.uv_attribute[1] * beta + payload.uv_attribute[2] * gamma;
    Vec3 normal = payload.normal_attribute[0] * alpha + payload.normal_attribute[1] * beta + payload.normal_attribute[2] * gamma;
    //上面计算插值，在SRender中还除以了clipSpacePos中的w分量
    
    Vec3 diffuseMap = payload.model->SamplerDiffseColor(uv);//返回[0,1]

    if (payload.model->normalMap)//有normalMap
        normal = cal_normal(normal, payload.worldSpacePos_varying, 
            payload.uv_attribute, uv, payload.model->normalMap);
    
    Vec3 faceNormal = normalize(payload.testFaceNormal_varying);
    Vec3 worldNormal = normalize(normal);
    Vec3 worldLightDir = normalize(Vec3(0, 1, 1));//从faceNomal改成顶点normal，成功变成正值了，右手坐标系从shadingPoint出发
    float ndotL = worldNormal * worldLightDir;
    if (ndotL < 0)
        ndotL = 0;

    return(diffuseMap * ndotL * 255.0);
    float lambert = payload.lambertTest_varying;
    return (diffuseMap * lambert);
    return Vec3(lambert, lambert, lambert);
    return Vec3(1, 1, 1);
}