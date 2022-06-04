#pragma once
#include "../shader/shader.h"

Vec3 texture_sample(Vec2 uv, TGAImage* image);
Vec3 cubemap_sample(Vec3 direction, cubemap_t* cubemap);
