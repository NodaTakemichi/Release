#pragma once
#include<DxLib.h>
#include "../Manager/ResourceManager.h"

class SoundController
{
public:
	enum class SOUND_MODE
	{
		SOUND_2D,
		SOUND_3D
	};

	enum class REVERB_TYPE
	{
		DEFAULT,	//�f�t�H���g
		GENERIC,	//��ʓI�ȋ��
		PADDEDCELL,	//���_�a�Ҏ�
		ROOM,		//����
		BATHROOM,	//�o�X���[��
		LIVINGROOM,	//���r���O���[��
		SMALLROOM,	//������
		MEDIUMROOM,	//������
		LARGEROOM,	//�啔��
		PLATE		//��
	};

	SoundController(void);
	~SoundController(void);

	//�f�[�^���[�h
	void DataLoad(const ResourceManager::SRC& soundSrouce, bool is3DSound = true);

	/// <summary>
	/// �T�E���h�̏������i2D�j
	/// </summary>
	/// <param name="volume">����</param>
	void Init(const int& volume);

	/// <summary>
	/// �T�E���h�̏������i3D�j
	/// </summary>
	/// <param name="volume">����</param>
	/// <param name="hearingArea">�����͂�����</param>
	/// <param name="pos">�����ʒu</param>
	/// <param name="type">���o�[�u�G�t�F�N�g�̃^�C�v</param>
	void Init(const int& volume ,const float& hearingArea,
		const VECTOR& pos, REVERB_TYPE type);
	void Update(void);
	void Draw(void);

	/// <summary>
	/// �����Đ��i��񂫂�j
	/// </summary>
	void Play(void);

	/// <summary>
	/// �����Đ��i���[�v�j
	/// </summary>
	void PlayLoop(void);

	/// <summary>
	/// �Đ����łȂ���΁A�������Đ�����
	/// </summary>
	void PlayNonPlaying(void);

	/// <summary>
	/// �Đ����łȂ���΁A���������[�v�Đ�����
	/// </summary>
	void PlayNonPlayingLoop(void);

	/// <summary>
	/// ������~
	/// </summary>
	void Stop(void);

	/// <summary>
	/// �Đ��������f
	/// </summary>
	/// <returns>true:�Đ����@false:�Đ����ł͂Ȃ�</returns>
	bool CheckPlaySound(void);

	/// <summary>
	/// �����ʒu�̐ݒ�
	/// </summary>
	/// <param name="soundPos">�������W</param>
	void SetSoundPos(const VECTOR& soundPos);	

	/// <summary>
	/// ���ʂ̐ݒ�
	/// </summary>
	/// <param name="volume">����</param>
	void SetVolume(const int& volume);

	/// <summary>
	/// ������������͈͂̐ݒ�
	/// </summary>
	/// <param name="area">�����������锼�a</param>
	void SetHearingArea(const float& area);

	/// <summary>
	/// �Đ����g���̐ݒ�
	/// </summary>
	/// <param name="frequencyPal">�ݒ肷��Đ����g��</param>
	void SetSoundFrequency(const int& frequencyPal);

	/// <summary>
	/// ���ʔ{���̕ύX
	/// </summary>
	/// <param name="frequencyPal">���ʔ{��</param>
	void ChangeVolumeRate(const float& rate);


private:
	int soundHandle_;

	//�����������鋗��
	float hearingArea_;
	//����
	int volume_;
	//���W
	VECTOR pos_;

	//���o�[�u�G�t�F�N�g�̑I��
	const int SelectReverbType(const REVERB_TYPE& type);
	
};

