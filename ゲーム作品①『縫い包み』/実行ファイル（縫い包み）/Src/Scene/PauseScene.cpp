#include <DxLib.h>
#include "../Application.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "../Utility/Utility.h"
#include "../UI/Common/Button.h"
#include "PauseScene.h"

//�V�[����ԕύX�Ԋu
constexpr float SCENE_STATE_CHANGE_INTERVAL = 0.8f;

//�|�[�Y�t���[���T�C�Y
constexpr int PAUSE_FRAME_WIDTH = 600;
constexpr int PAUSE_FRAME_HEIGHT = 500;

//�t���[���摜�̕`����W
constexpr int FRAME_DRAWING_POS_X = 0;
constexpr int FRAME_DRAWING_POS_Y = 120;
//�{�^��UI�̕`����W
constexpr int BUTTN_DRAWING_POS_X = 0;
constexpr int BUTTN_DRAWING_POS_Y = 200;

PauseScene::PauseScene(SceneManager& manager):SceneBase(manager)
{
	updataFunc_ = [&](InputManager& input) {
		BeginUpdate(input);
	};

	drawFunc_ = std::bind(&PauseScene::BeginDraw, this);
}

PauseScene::~PauseScene(void)
{
}

void PauseScene::LoadData(void)
{
}

void PauseScene::Init(void)
{
	auto& res = ResourceManager::GetInstance();

	//�t�H���g�̓o�^
	res.Load(ResourceManager::SRC::SELECT_CMD_FONT);
	buttnFontHandle_ = CreateFontToHandle(L"EbiharaNoKuseji", FONT_SIZE, 0);

	//�w�i�摜
	pauseBackImg_ = res.Load(ResourceManager::SRC::PAUSE_BACK_IMG).handleId_;

	//�e�{�^��
	buttons_.push_back(Button{ L"�Q�[�����[��", std::bind(&PauseScene::RuleBtnProcess, this) });
	buttons_.push_back(Button{ L"�^�C�g���֖߂�", std::bind(&PauseScene::TitleBtnProcess, this) });
	buttons_.push_back(Button{ L"�Q�[�����I��", std::bind(&PauseScene::ExitBtnProcess, this) });
	//buttons_.push_back(Button{ L"����", std::bind(&PauseScene::CloseBtnProcess, this) });
}

void PauseScene::Update(InputManager& input)
{
	//�X�V�֐��̌Ăяo��
	updataFunc_(input);
	return;
}

void PauseScene::Draw(void)
{
	//�`��֐��̌Ăяo��
	drawFunc_();
	return;
}

void PauseScene::DefaultUpdate(InputManager& input)
{
}

void PauseScene::BeginUpdate(InputManager& input)
{
	if (Utility::TimeOver(frameTime_, SCENE_STATE_CHANGE_INTERVAL))
	{
		updataFunc_ = [&](InputManager& input) {
			NormalUpdate(input);
		};

		drawFunc_ = std::bind(&PauseScene::NormalDraw, this);

		//�J�[�\�����[�h�̕ύX
		input.ChangeCurrsolMode(true);
	}
}

void PauseScene::NormalUpdate(InputManager& input)
{
	//ESCAPE�{�^���Ń|�[�Y��ʂ����
	if (input.IsTrgDown(KEY_INPUT_ESCAPE))
	{		
		//�J�[�\�����[�h�̕ύX
		input.ChangeCurrsolMode(false);

		CloseBtnProcess();
		return;
	}

	//�I��������
	BtnSelectProcess(static_cast<int>(buttons_.size()));
	if (input.IsTrgDown(KEY_INPUT_SPACE))
	{
		buttons_[cursolNum_].func_();
	}
}

void PauseScene::EndUpdate(InputManager& input)
{
	if (Utility::TimeOver(frameTime_, SCENE_STATE_CHANGE_INTERVAL)) {
		sceneManager_.PopScene();
	}
}

void PauseScene::BeginDraw(void)
{
	int halfScreenX = Application::SCREEN_SIZE_X / 2;
	int screenY = Application::SCREEN_SIZE_Y;

	float rate = static_cast<float>(frameTime_) / static_cast<float>(SCENE_STATE_CHANGE_INTERVAL);
	DrawExtendGraph(
		halfScreenX - PAUSE_FRAME_WIDTH / 2, FRAME_DRAWING_POS_Y,
		halfScreenX + PAUSE_FRAME_WIDTH / 2, static_cast<int>(FRAME_DRAWING_POS_Y + PAUSE_FRAME_HEIGHT * rate),
		pauseBackImg_, true);
}

void PauseScene::NormalDraw(void)
{
	BeginDraw();

	//�{�^���e�L�X�g�̕`��
	int idx = 0;
	for (const auto& btn : buttons_)
	{
		//X���W
		auto halfLen = Utility::GetStringLength(btn.text_, buttnFontHandle_) / 2;
		//��ʒ���
		int posX = Application::SCREEN_SIZE_X / 2 - halfLen;

		//�t�H���g�F
		int color = 0xeeeeee;
		if (idx == cursolNum_)color = 0xff0f0f;
		DrawStringToHandle(
			posX, 
			BUTTN_DRAWING_POS_Y + idx * LINE_SPACING, 
			btn.text_.c_str(), color, buttnFontHandle_);
		idx++;
	}
}

void PauseScene::EndDraw(void)
{
	int halfScreenX = Application::SCREEN_SIZE_X / 2;
	int screenY = Application::SCREEN_SIZE_Y;

	float rate = static_cast<float>(frameTime_) / static_cast<float>(SCENE_STATE_CHANGE_INTERVAL);
	DrawExtendGraph(
		halfScreenX - PAUSE_FRAME_WIDTH / 2, FRAME_DRAWING_POS_Y,
		halfScreenX + PAUSE_FRAME_WIDTH / 2, static_cast<int>(FRAME_DRAWING_POS_Y + PAUSE_FRAME_HEIGHT * (1.0f - rate)),
		pauseBackImg_, true);
}

void PauseScene::RuleBtnProcess(void)
{
	//PDF�ւ̃����N
	const wchar_t* pdfPath = L"��i�T�v.pdf";
	ShellExecute(NULL, L"open", pdfPath, NULL, NULL, SW_SHOWNORMAL);
}

void PauseScene::TitleBtnProcess(void)
{
	updataFunc_ = [&](InputManager& input) {
		DefaultUpdate(input);
	};

	sceneManager_.ChangeScene(SceneManager::SCENE_ID::TITLE);

	//�J�[�\�����[�h�̕ύX
	InputManager::GetInstance().ChangeCurrsolMode(false);

}

void PauseScene::ExitBtnProcess(void)
{
	//�Q�[���̏I��
	Application::GetInstance().CloseWindows();
}

void PauseScene::CloseBtnProcess(void)
{
	updataFunc_ = [&](InputManager& input) {
		EndUpdate(input);
	};

	drawFunc_ = std::bind(&PauseScene::EndDraw, this);
	frameTime_ = 0;
}
