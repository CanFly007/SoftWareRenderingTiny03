#include "Shader.h"
#include "../core/Sample.h"

void PBRShader::vertex_shader(int nfaces, int nvertex)
{
    Vec3 temp_vertPos = payload.model->GetVertPos(nfaces, nvertex);

    payload.uv_attribute[nvertex] = payload.model->GetVertUV(nfaces, nvertex);
    payload.normal_attribute[nvertex] = payload.model->GetVertNormal(nfaces, nvertex);

    payload.worldSpacePos_varying[nvertex] = temp_vertPos;//省略前乘Model2World矩阵
    payload.clipSpacePos_varying[nvertex] = payload.MVP_uniform * Vec4(temp_vertPos, 1.0);
    //为了齐次裁剪
    payload.outClipSpacePos[nvertex] = payload.clipSpacePos_varying[nvertex];//还没齐次裁剪前，第一个裁剪坐标是正常的Clip坐标，是三个点
    payload.outWorldSpacePos[nvertex] = payload.worldSpacePos_varying[nvertex];
}

static float floatACES(float value)
{
    float a = 2.51f;
    float b = 0.03f;
    float c = 2.43f;
    float d = 0.59f;
    float e = 0.14f;
    value = (value * (a * value + b)) / (value * (c * value + d) + e);
    return float_clamp(value, 0, 1);
}

//根据三角形三个点的坐标位置，和三个点的UV值，可以算出T和B向量
//注意：施密特正交化需要用到这个像素的法线信息（即顶点法线插值的normal）
//已知三个点的position和uv，已经该像素的normal，求该像素 的切线和副切线，并组成TBN矩阵
static Mat3 Cal_TangentToWorldMat(Vec3* worldPosArray, const Vec2* uvArray, Vec3& normal)
{
    //按照切线算法印象笔记
    // [T] = [t1 b1]的逆 * [AB]
    // [B] = [t2 b2]       [AC]
    //其中 t1 = B.u - A.u; b1 = B.v - A.v; t2 = C.u - A.u; b2 = C.v - A.v
    Vec3 AB = worldPosArray[1] - worldPosArray[0];
    Vec3 AC = worldPosArray[2] - worldPosArray[0];
    float t1 = uvArray[1].u - uvArray[0].u;//AB向量的U和V差
    float b1 = uvArray[1].v - uvArray[0].v;
    float t2 = uvArray[2].u - uvArray[0].u;//AC向量的U和V差
    float b2 = uvArray[2].v - uvArray[0].v;

    float determinant = t1 * b2 - b1 * t2;
    Vec3 worldSpaceT = b2 * AB - b1 * AC;
    Vec3 worldSpaceB = -t2 * AB + t1 * AC;
    worldSpaceT = worldSpaceT / determinant;
    worldSpaceB = worldSpaceB / determinant;

    //施密特正交化
    normal = normalize(normal);
    worldSpaceT = normalize(worldSpaceT - (worldSpaceT * normal) * normal);
    worldSpaceB = normalize(worldSpaceB - (worldSpaceB * normal) * normal - (worldSpaceB * worldSpaceT) * worldSpaceT);

    //TBN按列排序，构成 T->W的矩阵
    Mat3 tangent2World;
    tangent2World[0][0] = worldSpaceT.x; tangent2World[0][1] = worldSpaceB.x; tangent2World[0][2] = normal.x;
    tangent2World[1][0] = worldSpaceT.y; tangent2World[1][1] = worldSpaceB.y; tangent2World[1][2] = normal.y;
    tangent2World[2][0] = worldSpaceT.z; tangent2World[2][1] = worldSpaceB.z; tangent2World[2][2] = normal.z;
    return tangent2World;
}

Vec3 PBRShader::fragment_shader(float alpha, float beta, float gamma)
{
    Vec3 viewSpaceZ = { -payload.clipSpacePos_varying[0].w,
    -payload.clipSpacePos_varying[1].w,-payload.clipSpacePos_varying[2].w };//三个顶点观察空间Z的值
    //计算公式中的Zt
    float Zt = 1.0 / (alpha / viewSpaceZ[0] + beta / viewSpaceZ[1] + gamma / viewSpaceZ[2]);
    Vec3 worldPos = (payload.worldSpacePos_varying[0] * alpha / viewSpaceZ[0] +
        payload.worldSpacePos_varying[1] * beta / viewSpaceZ[1] + payload.worldSpacePos_varying[2] * gamma / viewSpaceZ[2]) * Zt;
    Vec2 uv = (payload.uv_attribute[0] * alpha / viewSpaceZ[0] +
        payload.uv_attribute[1] * beta / viewSpaceZ[1] + payload.uv_attribute[2] * gamma / viewSpaceZ[2]) * Zt;
    Vec3 normal = (payload.normal_attribute[0] * alpha / viewSpaceZ[0] +
        payload.normal_attribute[1] * beta / viewSpaceZ[1] + payload.normal_attribute[2] * gamma / viewSpaceZ[2]) * Zt;
    //计算插值，透视矫正插值。由三个点和重心坐标算出这个像素的属性

    Vec3 worldNormal = normal;
    if (payload.model->normalMap)//有normalMap,计算切线空间转换矩阵
    {
        Mat3 tangent2World = Cal_TangentToWorldMat(payload.worldSpacePos_varying, payload.uv_attribute, normal);
        Vec3 normalMap = texture_sample(uv, payload.model->normalMap);//TGA是bgr顺序，texture_sample函数里面转成rgb顺序，且范围缩小到[0,1]
        normalMap = normalMap * 2 - 1;//Unity中的UnpackNormal函数是把[0,1]变成[-1,1]
        worldNormal = tangent2World * normalMap;
    }

    worldNormal = normalize(worldNormal);
    Vec3 worldViewDir = normalize(payload.camera->eye - worldPos);
    float ndotV = float_max(worldNormal * worldViewDir, 0);

    Vec3 color(0.0f, 0.0f, 0.0f);
    //if (ndotV > 0)
    {
        float roughness = texture_sample(uv, payload.model->roughnessMap).x;
        float metalness = texture_sample(uv, payload.model->metalnessMap).x;
        Vec3 emission = texture_sample(uv, payload.model->emissionMap);

        Vec3 albedo = texture_sample(uv, payload.model->diffuseMap);//在PBR中称之为albedo、而不是diffuse
        //metalness==0时候即绝缘体，diffuse就是albedo，specular颜色是(0.4f,0.4f,0.4f)
        //metalness==1时候即金属材质，diffuse就是0即金属会全部吸收的折射光子导致无法再散射出来，金属材质的颜色完全取决于specular，specular颜色就是albedo
        //diffuse = albedo * (1 - metalness)
        //specular = lerp(0.04, albedo, metalness)
        Vec3 isolatorSpec = Vec3(0.04, 0.04, 0.04);
        Vec3 f0 = Vec3_lerp(isolatorSpec, albedo, metalness);//这里的f0就是菲涅尔公式中的F0基础反射率

        //Diffuse
        float r1 = float_max(1.0 - roughness, f0[0]);
        //F = F0 + (1-F0)*(1-cos)的五次方  theta从0度变到90度，F从F0变到1
        Vec3 F = f0 + (Vec3(r1, r1, r1) - f0) * pow(1 - ndotV, 5.0f);//Schlick Fresenl近似加上粗糙度的影响
        Vec3 kd = (Vec3(1.0, 1.0, 1.0) - F) * (1 - metalness);
        cubemap_t* irradianceMap = payload.iblMap->irradianceMap;
        Vec3 irradiance = cubemap_sample(worldNormal, irradianceMap);//normal为因变量进行查表
        for (int i = 0; i < 3; i++)
            irradiance[i] = pow(irradiance[i], 2.0f);
        Vec3 diffuse = irradiance * kd * albedo;

        //Specular
        Vec2 lutUV = Vec2(ndotV, roughness);
        Vec3 lutSample = texture_sample(lutUV, payload.iblMap->brdfLut);//x和y分量分别对应：F0*积分1 + 积分2式子中的，积分1和积分2
        Vec3 brdfPart = f0 * lutSample.x + Vec3(lutSample.y, lutSample.y, lutSample.y);//BRDF * 余弦部分的积分结果
        //下面求光照部分，即光照在BRDF积分域内的积分，再给Normalized，相当于把Light给模糊了
        //可以预滤波光照，滤波核的大小取决于BRDF的大小，即BRDF区域越大，得到的结果越模糊
        int mipMapLevel = (int)(roughness * (payload.iblMap->mipMapLevels - 1) + 0.5f);//根据模糊程度，在哪层预滤波贴图上采样
        Vec3 reflectDir = normalize(2.0 * (worldViewDir * worldNormal) * worldNormal - worldViewDir);
        Vec3 lightingPart = cubemap_sample(reflectDir, payload.iblMap->prefilter_maps[mipMapLevel]);
        for (int i = 0; i < 3; i++)
            lightingPart[i] = pow(lightingPart[i], 2.0f);
        Vec3 specular = CwiseProduct(lightingPart, brdfPart);
        color = emission + diffuse + specular;
    }
    
    for (int i = 0; i < 3; i++)
    {
        color[i] = floatACES(color[i]);

        color[i] = pow(color[i], 0.45f);
    }
    return color * 255.0f;
}