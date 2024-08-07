#include <DxLib.h>
#include "../Application.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "../Utility/Utility.h"
#include "../UI/Common/Button.h"
#include "PauseScene.h"

//シーン状態変更間隔
constexpr float SCENE_STATE_CHANGE_INTERVAL = 0.8f;

//ポーズフレームサイズ
constexpr int PAUSE_FRAME_WIDTH = 600;
constexpr int PAUSE_FRAME_HEIGHT = 500;

//フレーム画像の描画座標
constexpr int FRAME_DRAWING_POS_X = 0;
constexpr int FRAME_DRAWING_POS_Y = 120;
//ボタンUIの描画座標
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

	//フォントの登録
	res.Load(ResourceManager::SRC::SELECT_CMD_FONT);
	buttnFontHandle_ = CreateFontToHandle(L"EbiharaNoKuseji", FONT_SIZE, 0);

	//背景画像
	pauseBackImg_ = res.Load(ResourceManager::SRC::PAUSE_BACK_IMG).handleId_;

	//各ボタン
	buttons_.push_back(Button{ L"ゲームルール", std::bind(&PauseScene::RuleBtnProcess, this) });
	buttons_.push_back(Button{ L"タイトルへ戻る", std::bind(&PauseScene::TitleBtnProcess, this) });
	buttons_.push_back(Button{ L"ゲームを終了", std::bind(&PauseScene::ExitBtnProcess, this) });
	//buttons_.push_back(Button{ L"閉じる", std::bind(&PauseScene::CloseBtnProcess, this) });
}

void PauseScene::Update(InputManager& input)
{
	//更新関数の呼び出し
	updataFunc_(input);
	return;
}

void PauseScene::Draw(void)
{
	//描画関数の呼び出し
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

		//カーソルモードの変更
		input.ChangeCurrsolMode(true);
	}
}

void PauseScene::NormalUpdate(InputManager& input)
{
	//ESCAPEボタンでポーズ画面を閉じる
	if (input.IsTrgDown(KEY_INPUT_ESCAPE))
	{		
		//カーソルモードの変更
		input.ChangeCurrsolMode(false);

		CloseBtnProcess();
		return;
	}

	//選択肢操作
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

	//ボタンテキストの描画
	int idx = 0;
	for (const auto& btn : buttons_)
	{
		//X座標
		auto halfLen = Utility::GetStringLength(btn.text_, buttnFontHandle_) / 2;
		//画面中央
		int posX = Application::SCREEN_SIZE_X / 2 - halfLen;

		//フォント色
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
	//PDFへのリンク
	const wchar_t* pdfPath = L"作品概要.pdf";
	ShellExecute(NULL, L"open", pdfPath, NULL, NULL, SW_SHOWNORMAL);
}

void PauseScene::TitleBtnProcess(void)
{
	updataFunc_ = [&](InputManager& input) {
		DefaultUpdate(input);
	};

	sceneManager_.ChangeScene(SceneManager::SCENE_ID::TITLE);

	//カーソルモードの変更
	InputManager::GetInstance().ChangeCurrsolMode(false);

}

void PauseScene::ExitBtnProcess(void)
{
	//ゲームの終了
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
