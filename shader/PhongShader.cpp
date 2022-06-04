#include "./Shader.h"
#include "../core/Sample.h"

void PhongShader::vertex_shader(int nfaces, int nvertex)
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

Vec3 PhongShader::fragment_shader(float alpha, float beta, float gamma)
{
    Vec3 worldPos = payload.worldSpacePos_varying[0] * alpha + payload.worldSpacePos_varying[1] * beta + payload.worldSpacePos_varying[2] * gamma;
    Vec2 uv = payload.uv_attribute[0] * alpha + payload.uv_attribute[1] * beta + payload.uv_attribute[2] * gamma;
    Vec3 normal = payload.normal_attribute[0] * alpha + payload.normal_attribute[1] * beta + payload.normal_attribute[2] * gamma;
    //上面计算插值，在SRender中还除以了clipSpacePos中的w分量
    
    Vec3 diffuseMap = payload.model->SamplerDiffseColor(uv);//返回[0,1]

    Vec3 worldNormal = normal;
    if (payload.model->normalMap)//有normalMap,计算切线空间转换矩阵
    {
        Mat3 tangent2World = Cal_TangentToWorldMat(payload.worldSpacePos_varying, payload.uv_attribute, normal);
        Vec3 normalMap = texture_sample(uv, payload.model->normalMap);//TGA是bgr顺序，texture_sample函数里面转成rgb顺序，且范围缩小到[0,1]
        normalMap = normalMap * 2 - 1;//Unity中的UnpackNormal函数是把[0,1]变成[-1,1]
        worldNormal = tangent2World * normalMap;
    }
 
    Vec3 worldLightDir = normalize(Vec3(1, 1, 1));//从faceNomal改成顶点normal，成功变成正值了，右手坐标系从shadingPoint出发
    float ndotL = worldNormal * worldLightDir;

    Vec3 worldViewDir = normalize(payload.camera->eye - worldPos);
    Vec3 halfDir = normalize(worldViewDir + worldLightDir);
    float ndotH = worldNormal * halfDir;
    
    Vec3 ambient = 0.35 * diffuseMap;
    Vec3 diffuse = 0.9 * diffuseMap * float_max(0.0, ndotL);
    Vec3 specular = Vec3(0.8, 0.8, 0.8) * 0.15 * float_max(0.0, pow(ndotH, 150));//0.8指的是高光的颜色
    return(ambient + diffuse + specular) * 255.0;
}