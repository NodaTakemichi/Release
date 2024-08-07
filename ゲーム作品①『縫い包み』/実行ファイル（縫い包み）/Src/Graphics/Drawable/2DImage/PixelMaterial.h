#pragma once
#include<DxLib.h>
#include<vector>
#include<string>
#include"../MaterialBase.h"
class PixelMaterial
	:public MaterialBase
{
public:
	enum class PS_TYPE
	{
		BURN_GAUGE,	//�ċp�Q�[�W
		DISTORTION,	//�c�ݕ�
	};

	PixelMaterial(void);
	~PixelMaterial(void);

	/// <summary>
	/// �V�F�[�_�[�f�[�^�̃��[�h
	/// </summary>
	/// <param name="psType">�s�N�Z���V�F�[�_�[�̃^�C�v</param>
	void DataLoad(const PS_TYPE& psType);
	//������
	void Init(void);

	//�V�F�[�_�[�f�[�^�̎擾
	const ShaderData& GetShaderDataPS(void)const { return psData_; }

	//�萔�o�b�t�@�̍X�V
	void SetConstBuf(const std::vector<FLOAT4>& constBuf) { psData_ .constBufs=constBuf; }

	//�e�N�X�`���Z�b�g
	void SetTextures(const std::vector<int>& texs) { texture_ = texs; };
	//�e�N�X�`���z��̎擾
	const std::vector<int>& GetTextures(void)const { return texture_; }

private:
	//�V�F�[�_�[�f�[�^
	ShaderData psData_;

	//�摜
	std::vector<int> texture_;

};

