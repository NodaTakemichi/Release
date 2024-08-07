#include "MaterialBase.h"

MaterialBase::MaterialBase(void)
{
    texAddress_= TEXADDRESS::NONE;
}

MaterialBase::~MaterialBase(void)
{
}

ShaderData MaterialBase::SetShaderData(const int& shaderHandle, const int& constBufSize)
{
    ShaderData ret = {};

    ret.handle = shaderHandle;
    ret.constBufFloat4Size = constBufSize;

    //��̒萔�o�b�t�@���T�C�Y���A�쐬
    for (int i = 0; i < ret.constBufFloat4Size; i++)
    {
        if (ret.constBufFloat4Size > ret.constBufs.size())
        {
            ret.constBufs.emplace_back(FLOAT4{});
        }
    }

    return ret;
}
