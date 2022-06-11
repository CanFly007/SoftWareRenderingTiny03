#include "Shader.h"
#include "../core/Sample.h"

void PBRShader::vertex_shader(int nfaces, int nvertex)
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

Vec3 PBRShader::fragment_shader(float alpha, float beta, float gamma)
{
    Vec3 viewSpaceZ = { -payload.clipSpacePos_varying[0].w,
    -payload.clipSpacePos_varying[1].w,-payload.clipSpacePos_varying[2].w };//��������۲�ռ�Z��ֵ
    //���㹫ʽ�е�Zt
    float Zt = 1.0 / (alpha / viewSpaceZ[0] + beta / viewSpaceZ[1] + gamma / viewSpaceZ[2]);
    Vec3 worldPos = (payload.worldSpacePos_varying[0] * alpha / viewSpaceZ[0] +
        payload.worldSpacePos_varying[1] * beta / viewSpaceZ[1] + payload.worldSpacePos_varying[2] * gamma / viewSpaceZ[2]) * Zt;
    Vec2 uv = (payload.uv_attribute[0] * alpha / viewSpaceZ[0] +
        payload.uv_attribute[1] * beta / viewSpaceZ[1] + payload.uv_attribute[2] * gamma / viewSpaceZ[2]) * Zt;
    Vec3 normal = (payload.normal_attribute[0] * alpha / viewSpaceZ[0] +
        payload.normal_attribute[1] * beta / viewSpaceZ[1] + payload.normal_attribute[2] * gamma / viewSpaceZ[2]) * Zt;
    //�����ֵ��͸�ӽ�����ֵ����������������������������ص�����

    Vec3 worldNormal = normal;
    if (payload.model->normalMap)//��normalMap,�������߿ռ�ת������
    {
        Mat3 tangent2World = Cal_TangentToWorldMat(payload.worldSpacePos_varying, payload.uv_attribute, normal);
        Vec3 normalMap = texture_sample(uv, payload.model->normalMap);//TGA��bgr˳��texture_sample��������ת��rgb˳���ҷ�Χ��С��[0,1]
        normalMap = normalMap * 2 - 1;//Unity�е�UnpackNormal�����ǰ�[0,1]���[-1,1]
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

        Vec3 albedo = texture_sample(uv, payload.model->diffuseMap);//��PBR�г�֮Ϊalbedo��������diffuse
        //metalness==0ʱ�򼴾�Ե�壬diffuse����albedo��specular��ɫ��(0.4f,0.4f,0.4f)
        //metalness==1ʱ�򼴽������ʣ�diffuse����0��������ȫ�����յ�������ӵ����޷���ɢ��������������ʵ���ɫ��ȫȡ����specular��specular��ɫ����albedo
        //diffuse = albedo * (1 - metalness)
        //specular = lerp(0.04, albedo, metalness)
        Vec3 isolatorSpec = Vec3(0.04, 0.04, 0.04);
        Vec3 f0 = Vec3_lerp(isolatorSpec, albedo, metalness);//�����f0���Ƿ�������ʽ�е�F0����������

        //Diffuse
        float r1 = float_max(1.0 - roughness, f0[0]);
        //F = F0 + (1-F0)*(1-cos)����η�  theta��0�ȱ䵽90�ȣ�F��F0�䵽1
        Vec3 F = f0 + (Vec3(r1, r1, r1) - f0) * pow(1 - ndotV, 5.0f);//Schlick Fresenl���Ƽ��ϴֲڶȵ�Ӱ��
        Vec3 kd = (Vec3(1.0, 1.0, 1.0) - F) * (1 - metalness);
        cubemap_t* irradianceMap = payload.iblMap->irradianceMap;
        Vec3 irradiance = cubemap_sample(worldNormal, irradianceMap);//normalΪ��������в��
        for (int i = 0; i < 3; i++)
            irradiance[i] = pow(irradiance[i], 2.0f);
        Vec3 diffuse = irradiance * kd * albedo;

        //Specular
        Vec2 lutUV = Vec2(ndotV, roughness);
        Vec3 lutSample = texture_sample(lutUV, payload.iblMap->brdfLut);//x��y�����ֱ��Ӧ��F0*����1 + ����2ʽ���еģ�����1�ͻ���2
        Vec3 brdfPart = f0 * lutSample.x + Vec3(lutSample.y, lutSample.y, lutSample.y);//BRDF * ���Ҳ��ֵĻ��ֽ��
        //��������ղ��֣���������BRDF�������ڵĻ��֣��ٸ�Normalized���൱�ڰ�Light��ģ����
        //����Ԥ�˲����գ��˲��˵Ĵ�Сȡ����BRDF�Ĵ�С����BRDF����Խ�󣬵õ��Ľ��Խģ��
        int mipMapLevel = (int)(roughness * (payload.iblMap->mipMapLevels - 1) + 0.5f);//����ģ���̶ȣ����Ĳ�Ԥ�˲���ͼ�ϲ���
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