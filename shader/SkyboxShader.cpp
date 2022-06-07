#include "./Shader.h"
#include "../core/Sample.h"

void SkyboxShader::vertex_shader(int nfaces, int nvertex)
{
    Vec3 temp_vertPos = payload.model->GetVertPos(nfaces, nvertex);

    payload.uv_attribute[nvertex] = payload.model->GetVertUV(nfaces, nvertex);
    payload.normal_attribute[nvertex] = payload.model->GetVertNormal(nfaces, nvertex);

    payload.worldSpacePos_varying[nvertex] = temp_vertPos;
    payload.clipSpacePos_varying[nvertex] = payload.MVP_uniform * Vec4(temp_vertPos, 1.0);
    //Ϊ����βü�
    payload.outClipSpacePos[nvertex] = payload.clipSpacePos_varying[nvertex];//��û��βü�ǰ����һ���ü�������������Clip���꣬��������
    payload.outWorldSpacePos[nvertex] = payload.worldSpacePos_varying[nvertex];
}

Vec3 SkyboxShader::fragment_shader(float alpha, float beta, float gamma)
{
    //��պ�ҲҪ����͸�ӽ�����ֵ����Ȼ�ῴ�������Ե
    float Zt = 1.0 / (alpha / -payload.clipSpacePos_varying[0].w + 
        beta / -payload.clipSpacePos_varying[1].w + gamma / -payload.clipSpacePos_varying[2].w);
    Vec3 worldPos = (payload.worldSpacePos_varying[0] * alpha / -payload.clipSpacePos_varying[0].w +
        payload.worldSpacePos_varying[1] * beta / -payload.clipSpacePos_varying[1].w +
        payload.worldSpacePos_varying[2] * gamma / -payload.clipSpacePos_varying[2].w) * Zt;
    //Vec3 worldPos = payload.worldSpacePos_varying[0] * alpha + 
    //    payload.worldSpacePos_varying[1] * beta + payload.worldSpacePos_varying[2] * gamma;
    //��պеĲ�������worldPosҲ��Ҫ��͸�ӽ�����ֵ������ע�ͽ�ע���������й۲���죨���εľ���û�н���ʱ���Ť����
    //��������ֵ����SRender�л�������clipSpacePos�е�w����

    Vec3 result_color;
    result_color = cubemap_sample(worldPos, payload.model->environment_map);
    return result_color * 255.f;
}