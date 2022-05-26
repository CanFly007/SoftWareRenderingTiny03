#include "./Shader.h"

void PhongShader::vertex_shader(int nfaces, int nvertex)
{
    Vec3 temp_vertPos = payload.model->GetVertPos(nfaces, nvertex);
    //vertPos[nvertex] = model->GetVertPos(nfaces, nvertex);
    payload.uv_attribute[nvertex] = payload.model->GetVertUV(nfaces, nvertex);
    //vertUV[nvertex] = model->GetVertUV(nfaces, nvertex);

    //testWorldPos[nvertex] = vertPos[nvertex];
    //orthoSpacePos[nvertex] = MVP * Vec4(vertPos[nvertex], 1.0);
    payload.worldSpacePos_varying[nvertex] = temp_vertPos;//省略前乘Model2World矩阵
    payload.clipSpacePos_varying[nvertex] = payload.MVP_uniform * Vec4(temp_vertPos, 1.0);


    //divisionPos[nvertex] = Vec4(orthoSpacePos[nvertex].x / orthoSpacePos[nvertex].w,
    //    orthoSpacePos[nvertex].y / orthoSpacePos[nvertex].w, orthoSpacePos[nvertex].z / orthoSpacePos[nvertex].w, orthoSpacePos[nvertex].w);
    //viewPortPos[nvertex] = viewPort_mat * divisionPos[nvertex];
    //testScreenCoord[nvertex] = Convert_ToVec3(viewPortPos[nvertex]);
}

Vec3 PhongShader::fragment_shader(float alpha, float beta, float gamma)
{
    Vec3 worldPos = payload.worldSpacePos_varying[0] * alpha + payload.worldSpacePos_varying[1] * beta + payload.worldSpacePos_varying[2] * gamma;
    //上面计算插值，在SRender中还除以了clipSpacePos中的w分量
    
    float lambert = payload.lambertTest_varying;
    return Vec3(lambert, lambert, lambert);
    return Vec3(1, 1, 1);
}