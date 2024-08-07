#pragma once
#include<DxLib.h>
#include<unordered_map>

class Vector2;

class DrawShader
{
public:

	enum class PS_TYPE
	{
		TEXTURE,
		REVERSE_TEXTURE,
		MONOTONE,
		NORMAL_MAP,
		MASK,
		APPEARANCE,
		DISTORTION,
		SIMPLE_SEPIA,
		FADE,
		FADE_TEXTURE,

		ON_BUTTON,
		ON_CIRCLE_BUTTON,
		CARD_FRAME,
		IUMI_FRAME,
		HP_SHADER,
		DEATH_SHADER,
		DEATH_UNIT,

		STATUS_UP,
		STATUS_DOWN,
		PARALYSIS,
		POISON,
		BAYER_DITHE
	};


	// �����I�ɃC���X�e���X�𐶐�����
	static void CreateInstance(void);

	// �ÓI�C���X�^���X�̎擾
	static DrawShader& GetInstance(void);

	// ������
	void Init(void);

	// ���(�V�[���ؑ֎��Ɉ�U���)
	void Release(void);

	//�摜�ʏ�`��
	void DrawGraph(
		const Vector2& pos, const int& handle);
	//�摜�`��i�萔�Ȃ��j
	void DrawGraphToShader(
		const Vector2& pos,const int& handle, const PS_TYPE& type);
	//�摜�`��i�萔�t���j
	void DrawGraphToShader(
		const Vector2& pos,const int& handle, const PS_TYPE& type,
		const COLOR_F& buf,const COLOR_F& subBuf = COLOR_F{});
	//�T�C�Y�w��`��i�ʏ�j
	void DrawExtendGraphToShader(
		const Vector2& pos, const Vector2& size , const int& handle);
	//�T�C�Y�w��`��
	void DrawExtendGraphToShader(
		const Vector2& pos, const Vector2& size , 
		const int& handle, const PS_TYPE& type,
		const COLOR_F& buf, const COLOR_F& subBuf = COLOR_F{});
	
	
	//�T�u�e�N�X�`���t���`��
	void DrawGraphAndSubToShader(
		const Vector2& pos,
		const int& mHandle, const int& sHandle,
		const PS_TYPE& type, const COLOR_F& buf, const COLOR_F& subBuf = COLOR_F{});

	//�T�u�e�N�X�`���t���`��(�}�X�N�T�C�Y)
	void DrawGraphAndSubToShader(
		const Vector2& pos, const Vector2& size,
		const int& mHandle, const int& sHandle,
		const PS_TYPE& type, const COLOR_F& buf, const COLOR_F& subBuf = COLOR_F{});


private:
	// �s�N�Z���V�F�[�_�p�I���W�i���萔�o�b�t�@�̎g�p�J�n�X���b�g
	static constexpr int CONSTANT_BUF_SLOT_BEGIN_PS = 3;
	// ���_��
	static constexpr int NUM_VERTEX = 4;
	// ���_�C���f�b�N�X��
	static constexpr int NUM_VERTEX_IDX = 6;
	// �|���S����
	static constexpr int NUM_POLYGON = 2;

	// �ÓI�C���X�^���X
	static DrawShader* instance_;

	//���_���
	VERTEX2DSHADER vertex_[NUM_VERTEX];
	WORD index_[NUM_VERTEX_IDX];

	//�V�F�[�_�[�萔�o�b�t�@
	int psConstBuf_;

	//�V�F�[�_�[�̃��\�[�X�}�b�v
	std::unordered_map< PS_TYPE, int>psMap_;

	//�`��p�̎l�p���_�̍쐬
	void MakeSquereVertex(Vector2 pos, Vector2 size);
	
	//�V�F�[�_�[�̃��[�h
	void InitPS(void);

	//�V�F�[�_�[�̌���
	const int& SearchPS(const PS_TYPE& ps)const;


	DrawShader(void);
	~DrawShader(void);

};

