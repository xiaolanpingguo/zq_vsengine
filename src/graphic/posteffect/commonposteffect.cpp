#include "graphic/posteffect/commonposteffect.h"
#include "graphic/core/graphicinclude.h"
using namespace zq;
COMMON_POST_EFFECT_FUNCTION_IMPLEMENT(Gray)

COMMON_POST_EFFECT_FUNCTION_IMPLEMENT(GuassBlur)

COMMON_POST_EFFECT_FUNCTION_IMPLEMENT_ONE(Saturate, Saturation)

COMMON_POST_EFFECT_FUNCTION_IMPLEMENT(OldPhoto)

COMMON_POST_EFFECT_FUNCTION_IMPLEMENT(DownFilter4)

COMMON_POST_EFFECT_FUNCTION_IMPLEMENT(TexAdd)

COMMON_POST_EFFECT_FUNCTION_IMPLEMENT_THREE(Bloom, MiddleGray, WhiteCutoff, Luminance)


