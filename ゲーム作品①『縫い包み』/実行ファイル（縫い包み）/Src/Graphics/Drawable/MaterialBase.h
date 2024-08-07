#pragma once
#include<DxLib.h>
#include<vector>

struct ShaderData
{	
	int handle;					//�V�F�[�_�[�n���h��
	int constBufFloat4Size;		//FLOAT4�^�̒萔�o�b�t�@�̌�
	int constBufHandle ;			//�萔�o�b�t�@�n���h��
	std::vector<FLOAT4>constBufs;	//�萔�o�b�t�@�z��
};


class MaterialBase
{
public:
	// �e�N�X�`���A�h���X�^�C�v
	enum class TEXADDRESS
	{
		NONE = 0,
		WRAP,
		MIRROR,
		CLAMP,
		BORDER,
		MAX
	};

	MaterialBase(void);
	virtual ~MaterialBase(void);

	virtual void Init(void) = 0;

	//�e�N�X�`���A�h���X���擾
	TEXADDRESS GetTextureAddress(void)const { return texAddress_; }
	//�e�N�X�`���A�h���X��ݒ�
	void SetTextureAddress(TEXADDRESS texA) { texAddress_ = texA; }

protected:

	/// <summary>
	/// �V�F�[�_�[�f�[�^�̐ݒ�
	/// </summary>
	/// <param name="shaderHandle">�V�F�[�_�[�n���h��</param>
	/// <param name="constBufSize">�萔�o�b�t�@�̃T�C�Y</param>
	/// <returns></returns>
	ShaderData SetShaderData(const int& shaderHandle, const int& constBufSize);

	//�e�N�X�`���A�h���X
	TEXADDRESS texAddress_;

private:

};

