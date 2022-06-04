#include "./Shader.h"
#include "../core/Sample.h"

void SkyboxShader::vertex_shader(int nfaces, int nvertex)
{
    Vec3 temp_vertPos = payload.model->GetVertPos(nfaces, nvertex);

    payload.uv_attribute[nvertex] = payload.model->GetVertUV(nfaces, nvertex);
    payload.normal_attribute[nvertex] = payload.model->GetVertNormal(nfaces, nvertex);

    payload.worldSpacePos_varying[nvertex] = temp_vertPos;
    payload.clipSpacePos_varying[nvertex] = payload.MVP_uniform * Vec4(temp_vertPos, 1.0);
    //为了齐次裁剪
    payload.outClipSpacePos[nvertex] = payload.clipSpacePos_varying[nvertex];//还没齐次裁剪前，第一个裁剪坐标是正常的Clip坐标，是三个点
    payload.outWorldSpacePos[nvertex] = payload.worldSpacePos_varying[nvertex];
}

Vec3 SkyboxShader::fragment_shader(float alpha, float beta, float gamma)
{
    Vec3 worldPos = payload.worldSpacePos_varying[0] * alpha + payload.worldSpacePos_varying[1] * beta + payload.worldSpacePos_varying[2] * gamma;
    //上面计算插值，在SRender中还除以了clipSpacePos中的w分量

    Vec3 result_color;
    result_color = cubemap_sample(worldPos, payload.model->environment_map);
    return result_color * 255.f;
}