#pragma once
#include<vector>
#include<functional>
class SceneManager;
class InputManager;
class Button;

//�ŒჍ�[�f�B���O����
constexpr float LOADING_TIME = 2.0f;
//�t�H���g�T�C�Y
constexpr int FONT_SIZE = 28;
//�{�^��UI�̍s��
constexpr int LINE_SPACING = 80;

//���[�f�B���O
constexpr int COMMA_MAX_NUM = 7;		//�unow loading......�v�̃R���}�̐�
constexpr float COMMA_TIME = 0.5f;		//�unow loading......�v�̃R���}���𑝂₷����
constexpr int LOADING_STRING_POS_X = 950;	//�unow loading......�v�̍��WX
constexpr int LOADING_STRING_POS_Y = 750;	//�unow loading......�v�̍��WY


class SceneBase
{
public:

	SceneManager& sceneManager_;

	SceneBase(SceneManager& scene);
	virtual ~SceneBase(void);

	virtual void LoadData(void) = 0;
	virtual void Init(void) = 0;
	virtual void Update(InputManager& input) = 0;
	virtual void Draw(void) = 0;

protected:

	std::function<void(InputManager&)> updataFunc_;
	std::function<void(void)> drawFunc_;

	//���[�f�B���O�o�ߎ���
	float loadingTime_;

	//�I���{�^���t�H���g�n���h��
	int buttnFontHandle_;
	//�J�[�\���ʒu
	int cursolNum_ ;

	//�{�^���I�𑀍�
	void BtnSelectProcess(const int& btnNum);

	//�unow loading......�v�̕`��
	void DrawNowLoading(void);

private:

};