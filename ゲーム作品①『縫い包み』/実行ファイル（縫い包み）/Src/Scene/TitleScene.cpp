#include <Windows.h>
#include <cstdlib>
#include <string>
#include <DxLib.h>
#include "../Application.h"
#include "../Utility/Utility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/SoundManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/Camera.h"
#include "../Sound/SoundController.h"
#include "../UI/Common/Button.h"
#include "GameScene.h"
#include "TitleScene.h"

//スポットライト
constexpr float LIGHT_OUT_ANGLE_DEG = 65.0f;	//外側の角度
constexpr float LIGHT_IN_ANGLE_DEG = 40.0f;		//内側の角度
constexpr float LIGHT_RANGE = 2000.0f;			//有効距離
constexpr float LIGHT_ATTEN_0 = 0.2f;			//距離減衰パラメータ０
constexpr float LIGHT_ATTEN_1 = 0.00175f;		//距離減衰パラメータ１
constexpr float LIGHT_ATTEN_2 = 0.0f;			//距離減衰パラメータ２

//BGM音量
constexpr int BGM_VOLUME = 120;
//ステージ座標
constexpr VECTOR STAGE_POS = VECTOR{ 170,-1970,-800 };
//ぬいぐるみモデル
constexpr VECTOR DOOL_POS = VECTOR{ -90,-1558,-750 };	//座標
constexpr VECTOR DOOL_SCALE = VECTOR{ 0.2f,0.2f,0.2f };	//大きさ

//ステージ表示画面のローカル座標
constexpr int STAGE_DRAWING_LOCAL_POS = -250;
//タイトルロゴ描画座標
constexpr int LOG_DRAWING_POS_X = 550;
constexpr int LOG_DRAWING_POS_Y = 75;

//ボタンUI
constexpr int BUTTON_UI_DRAWING_POS_X = 1000;
constexpr int BUTTON_UI_DRAWING_POS_Y = 400;
constexpr int SELECT_BUTTON_COLOR = 0xff0f0f;
constexpr int NORMAL_BUTTON_COLOR = 0xeeeeee;
//ボタン操作指示
constexpr int BUTTON_OPERATION_DRAWING_UNDER_DIS_Y = 90;
constexpr int OPERATIOM_UI_COLOR = 0xbbbbbb;

//グリッチシェーダー
constexpr float GLITC_SHADER_TIME_VALUE = 5.0f;		//タイム調整値
constexpr float GLITC_SHADER_AMPLITUDE_VALUE = 10.0f;		//振幅値


TitleScene::TitleScene(SceneManager& manager):SceneBase(manager)
{
	//更新関数のセット
	updataFunc_ = [&](InputManager& input) {LoadingUpdate(input); };
	//描画関数のセット
	drawFunc_ = std::bind(&TitleScene::LoadingDraw, this);
}

TitleScene::~TitleScene(void)
{
}

void TitleScene::LoadData(void)
{
	loadingTime_ = 0.0f;
	auto& res = ResourceManager::GetInstance();

	//非同期読み込みをtrueにする
	SetUseASyncLoadFlag(true);

	//タイトル画像
	titleImg_ = res.Load(ResourceManager::SRC::TITLE_IMG).handleId_;

	//フォントの登録
	res.Load(ResourceManager::SRC::SELECT_CMD_FONT);
	buttnFontHandle_ = CreateFontToHandle(L"EbiharaNoKuseji", FONT_SIZE, 0);

	//モデルの登録
	//タイトル用ステージモデル
	stageModelHandle_ = res.Load(ResourceManager::SRC::TITLE_STAGE_MODEL).handleId_;
	//ぬいぐるみモデル
	doolModelHandle_ = res.Load(ResourceManager::SRC::DOOL_MONKEY).handleId_;

	//マテリアル
	//ステージモデルマテリアル
	stageModelMaterial_ = std::make_unique<ModelMaterial>();
	stageModelMaterial_->DataLoad(
		ModelMaterial::VS_TYPE::MODEL_NORMAL_1FRAME,
		ModelMaterial::PS_TYPE::MODEL_NORMAL);
	//ぬいぐるみモデルマテリアル
	doolModelMaterial_ = std::make_unique<ModelMaterial>();
	doolModelMaterial_->DataLoad(
		ModelMaterial::VS_TYPE::MODEL_NORMAL_1FRAME,
		ModelMaterial::PS_TYPE::MODEL_NORMAL);

	// グリッチ
	glitchMaterial_ = std::make_unique<PostEffectMaterial>();
	glitchMaterial_->DataLoad(PostEffectMaterial::PS_TYPE::GLITCH);
	int noiseImg = res.Load(ResourceManager::SRC::NOISE_IMG).handleId_;	//ノイズ画像
	glitchMaterial_->SetTextures({ noiseImg });

	//BGM
	bgm_ = std::make_shared<SoundController>();
	bgm_->DataLoad(ResourceManager::SRC::TITLE_BGM,false);
}

void TitleScene::Init(void)
{
	//ボタン選択肢
	buttons_.push_back(Button{ L"始める",std::bind(&TitleScene::StartButtnProcess, this) });
	buttons_.push_back(Button{ L"ゲームルール",std::bind(&TitleScene::InstructButtnProcess, this) });
	buttons_.push_back(Button{ L"終了する",std::bind(&TitleScene::ExitButtnProcess, this) });

	//オブジェクトモデルの設定
	SettingObject();

	//カメラモードのセット（回転）
	sceneManager_.GetCamera().ChangeMode(Camera::MODE::ROTATION);
	//カメラのターゲットセット
	sceneManager_.GetCamera().SetTargetPos(doolPos_);

	//ライトの作成
	auto& camera = sceneManager_.GetCamera();
	lightHandle_ = CreateSpotLightHandle(
		camera.GetPos(), camera.GetAngles(),
		Utility::Deg2RadF(LIGHT_OUT_ANGLE_DEG), Utility::Deg2RadF(LIGHT_IN_ANGLE_DEG),
		LIGHT_RANGE,
		LIGHT_ATTEN_0, LIGHT_ATTEN_1, LIGHT_ATTEN_2
	);

	//ステージ用スクリーン
	stageScreen_ = MakeScreen(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y,true);


	//ポストエフェクト
	glitchMaterial_->Init();
	// ピクセルシェーダー用のレンダラー
	postEffectRenderer_.push_back(std::move(std::make_unique<PostEffectRenderer>(*glitchMaterial_)));



	//BGM再生
	bgm_->Init(BGM_VOLUME);
	bgm_->PlayLoop();

}

void TitleScene::Update(InputManager& input)
{
	updataFunc_(input);
	return;
}

void TitleScene::Draw(void)
{
	drawFunc_();
	return;
}

void TitleScene::SettingObject(void)
{
	//タイトル用ステージモデル
	MV1SetPosition(stageModelHandle_, STAGE_POS);
	MV1SetScale(stageModelHandle_, Utility::VECTOR_ONE);
	MV1SetRotationXYZ(stageModelHandle_, VECTOR{});
	stageModelMaterial_->Init();
	modelRenderers_.push_back(std::make_unique<ModelRenderer>(stageModelHandle_, *stageModelMaterial_));

	//ぬいぐるみモデル
	doolPos_ = DOOL_POS;
	MV1SetPosition(doolModelHandle_, doolPos_);
	MV1SetScale(doolModelHandle_, DOOL_SCALE);
	MV1SetRotationXYZ(doolModelHandle_, VECTOR{});
	doolModelMaterial_->Init();
	modelRenderers_.push_back(std::make_unique<ModelRenderer>(doolModelHandle_, *doolModelMaterial_));
}

void TitleScene::StartButtnProcess(void)
{
	sceneManager_.ChangeScene(SceneManager::SCENE_ID::GAME);
}

void TitleScene::InstructButtnProcess(void)
{
	//PFDへのリンク
	const wchar_t* pdfPath = L"作品概要.pdf";
	ShellExecute(NULL, L"open", pdfPath, NULL, NULL, SW_SHOWNORMAL);
}

void TitleScene::ExitButtnProcess(void)
{
	//ゲームの終了
	Application::GetInstance().CloseWindows();
}

void TitleScene::LoadingUpdate(InputManager& input)
{
	bool loadTimeOver = Utility::TimeOver(loadingTime_, LOADING_TIME);

	//ロードが完了したか判断
	if (GetASyncLoadNum() == 0 && loadTimeOver)
	{
		SetUseASyncLoadFlag(false);

		//初期化処理
		Init();

		//フェードイン開始
		sceneManager_.StartFadeIn();

		//更新関数のセット
		updataFunc_ = [&](InputManager& input) {NormalUpdate(input); };
		//描画関数のセット
		drawFunc_ = std::bind(&TitleScene::NormalDraw, this);
	}
}

void TitleScene::NormalUpdate(InputManager& input)
{

	//ライト更新
	auto dir = sceneManager_.GetCamera().GetAngles();
	auto pos = sceneManager_.GetCamera().GetPos();
	SetLightDirectionHandle(lightHandle_, dir);
	SetLightPositionHandle(lightHandle_, pos);

	//選択肢操作
	BtnSelectProcess(static_cast<int>(buttons_.size()));


	//モデルマテリアル：定数バッファの更新
	//フォグ情報の定数バッファ更新
	VECTOR cameraPos = sceneManager_.GetInstance().GetCamera().GetPos();
	float fogEnd, fogStart;
	GetFogStartEnd(&fogStart, &fogEnd);
	stageModelMaterial_->SetConstBufVS(0, FLOAT4{ cameraPos.x,cameraPos.y,cameraPos.z,fogStart });
	stageModelMaterial_->SetConstBufVS(1, FLOAT4{ fogEnd ,0,0,0 });

	doolModelMaterial_->SetConstBufVS(0, FLOAT4{ cameraPos.x,cameraPos.y,cameraPos.z,fogStart });
	doolModelMaterial_->SetConstBufVS(1, FLOAT4{ fogEnd ,0,0,0 });

	//グリッチ：定数バッファ更新
	auto totalTime = sceneManager_.GetTotalTime();
	glitchMaterial_->SetConstBuf(0, FLOAT4{ totalTime,GLITC_SHADER_TIME_VALUE,GLITC_SHADER_AMPLITUDE_VALUE });	//タイム調整値、振幅値


	//決定ボタン押下判断
	bool isTrgDownSpace = input.IsTrgDown(KEY_INPUT_SPACE);
	if (isTrgDownSpace)
	{
		//カーソル番号に対応した処理の実行
		buttons_[cursolNum_].func_();
	}
}

void TitleScene::LoadingDraw(void)
{
	//「now loading」の描画
	DrawNowLoading();
}

void TitleScene::NormalDraw(void)
{
	//背景
	DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, 0x0, true);

	// 描画先グラフィック領域の指定
	sceneManager_.SetDrawingScreen(stageScreen_);

	//モデル描画
	for (const auto& renderer : modelRenderers_)
	{
		renderer->Draw();
	}

	SetDrawScreen(sceneManager_.GetMainScreen());
	DrawGraph(STAGE_DRAWING_LOCAL_POS, 0, stageScreen_, true);

	//UI
	//タイトル画像
	DrawGraph(LOG_DRAWING_POS_X, LOG_DRAWING_POS_Y, titleImg_, true);

	int idx = 0;
	int localDis = LINE_SPACING;
	//ボタンテキストの描画
	for (const auto& btn : buttons_)
	{
		//ボタンの色指定
		int btnColor = (idx == cursolNum_) ? SELECT_BUTTON_COLOR : NORMAL_BUTTON_COLOR;
		DrawStringToHandle(BUTTON_UI_DRAWING_POS_X, BUTTON_UI_DRAWING_POS_Y + idx * localDis, btn.text_.c_str(), btnColor, buttnFontHandle_);
	
		idx++;
	}

	//オペレーションUI
	DrawStringFToHandle(
		BUTTON_UI_DRAWING_POS_X, Application::SCREEN_SIZE_Y- BUTTON_OPERATION_DRAWING_UNDER_DIS_Y,
		L"[w][s]キー:選択\nSpace　　:決定", 
		OPERATIOM_UI_COLOR, buttnFontHandle_);


	//ポストエフェクト
	for (const auto& renderer : postEffectRenderer_)
	{
		renderer->Draw();
	}

}


