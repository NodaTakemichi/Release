#pragma once
#include <string>

class Application
{

public:

	// �X�N���[���T�C�Y
	static constexpr int SCREEN_SIZE_X = 1280;
	static constexpr int SCREEN_SIZE_Y = 800;


	// �����I�ɃC���X�e���X�𐶐�����
	static void CreateInstance(void);

	// �ÓI�C���X�^���X�̎擾
	static Application& GetInstance(void);

	// ������
	void Init(void);
	// �Q�[�����[�v�̊J�n
	void Run(void);
	// ���\�[�X�̉��
	void Release(void);
	//�j��
	void Destroy(void);

	// �����������^���s�̔���
	bool IsInitFail(void) const;

	// ��������^���s�̔���
	bool IsReleaseFail(void) const;

	//�E�B���h�E�̏I��
	void CloseWindows(void);

private:

	// �ÓI�C���X�^���X
	static Application* instance_;

	// ���������s
	bool isInitFail_;

	// ������s
	bool isReleaseFail_;

	//�E�B���h�E�Y�̏I�����f	true/�I���@false/�p��
	bool isExitWindows_;

	Application(void);
	Application(const Application&);
	~Application(void);

	//Effekseer�̏�����
	void InitEffekseer(void);

};