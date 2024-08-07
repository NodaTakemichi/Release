#pragma once
#include<DxLib.h>
#include<vector>
#include<string>
#include"../MaterialBase.h"
class PostEffectMaterial:
	public MaterialBase
{
public:
	enum class PS_TYPE
	{
		NOISE,		//�m�C�Y
		VIGNETTE,	//�r�l�b�g
		GLITCH,		//�O���b�`
		BLUR		//�u���[
	};

	PostEffectMaterial(void);
	~PostEffectMaterial(void);

	/// <summary>
	/// �V�F�[�_�[�f�[�^�̃��[�h
	/// </summary>
	/// <param name="psType">�s�N�Z���V�F�[�_�[�̃^�C�v</param>
	void DataLoad(const PS_TYPE& psType);
	//������
	void Init(void);

	//�萔�o�b�t�@�̃Z�b�g
	void SetConstBuf(int idx, const FLOAT4& constBuf);

	//�V�F�[�_�[�f�[�^�̎擾
	const ShaderData& GetShaderDataPS(void)const { return psData_; }

	//�|�X�g�G�t�F�N�g�p�X�N���[���̎擾
	const int& GetPEScreen(void)const { return postEffectScreen_; }

	//�e�N�X�`���Z�b�g
	void SetTextures(const std::vector<int>& texs) { texture_ = texs; };
	//�e�N�X�`���z��̎擾
	const std::vector<int>& GetTextures(void)const { return texture_; }
private:

	//�V�F�[�_�[�f�[�^
	ShaderData psData_;

	//�摜
	std::vector<int> texture_;

	//�|�X�g�G�t�F�N�g�p�X�N���[��
	int postEffectScreen_;

	//�X�N���[�����W�̍쐬
	void MakePostEffecctScreen(void);

};

