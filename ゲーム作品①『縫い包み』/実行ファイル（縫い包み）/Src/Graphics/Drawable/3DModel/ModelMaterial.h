#pragma once
#include<DxLib.h>
#include<vector>
#include<map>
#include<string>
#include"../../../Manager/ResourceManager.h"
#include"../MaterialBase.h"

class ModelMaterial:
	MaterialBase
{
public:	
	enum class VS_TYPE
	{
		MODEL_NORMAL_1FRAME,		//�m�[�}��
	};

	enum class PS_TYPE
	{
		MODEL_NORMAL,		//�m�[�}��
		BURNING_DOOL,		//�ċp�ʂ������	
		DISTORTION_WALL,	//�c�ݕ�
		SPHERE_MAP,			//�X�t�B�A�}�b�v
		LUMINESCENCE		//�������f��
	};

	ModelMaterial(void);
	~ModelMaterial(void);

	/// <summary>
	/// �V�F�[�_�[�f�[�^�̃��[�h
	/// </summary>
	/// <param name="vsType">���_�V�F�[�_�[�̃^�C�v</param>
	/// <param name="psType">�s�N�Z���V�F�[�_�[�̃^�C�v</param>
	void DataLoad(const VS_TYPE& vsType, const PS_TYPE& psType);
	//������
	void Init(void);


	//�e�N�X�`���̍X�V
	void SetTextures(int slot ,int texDiffuse);

	//�萔�o�b�t�@�̒ǉ�
	void AddConstBufVS(const FLOAT4& constBuf);
	void AddConstBufPS(const FLOAT4& constBuf);

	//�萔�o�b�t�@�̍X�V
	void SetConstBufVS(int idx ,const FLOAT4& constBuf);
	void SetConstBufPS(int idx ,const FLOAT4& constBuf);

	//�V�F�[�_�[�f�[�^�̎擾
	const ShaderData& GetShaderDataVS(void)const { return vsData_; }
	const ShaderData& GetShaderDataPS(void)const { return psData_; }

	//�e�N�X�`���z��̎擾
	const std::map<int,int>& GetTextures(void)const { return texture_; }

	//�e�N�X�`���A�h���X���擾
	TEXADDRESS GetTextureAddress(void)const { return texAddress_; }
	//�e�N�X�`���A�h���X��ݒ�
	void SetTextureAddress(TEXADDRESS texA) { texAddress_ = texA; }

private:

	//�V�F�[�_�[�f�[�^
	ShaderData vsData_;
	ShaderData psData_;


	//�摜
	std::map<int,int> texture_;


};

