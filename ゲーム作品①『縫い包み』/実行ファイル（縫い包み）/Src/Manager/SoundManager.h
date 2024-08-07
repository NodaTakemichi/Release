#pragma once
#include<DxLib.h>
#include<memory>
#include<vector>
#include "../Utility/StaticSingleton.h"
class SoundController;

class SoundManager:
	public StaticSingleton<SoundManager>
{
public:

	void Init(void);
	void Update(void);

	void AddSoundController(std::shared_ptr<SoundController> sound);

	//�S�����̍Đ��X�g�b�v
	void StopAllSound(void);

	//�����f�[�^���N���A����
	void ClearSpounds(void);

	//�}�X�^�[�{�����[���̕ύX
	void ChangeMasterVolume(const float& rate);


private:
	//�V���O���g��
	THIS_CLASS_IS_STATIC_SINGLETON(SoundManager);

	//�T�E���h�Ǘ�
	std::vector<std::shared_ptr<SoundController>> sounds_;
	
	//���ʔ{��
	float volumeRate_;

	SoundManager(void);
	~SoundManager(void);


};

