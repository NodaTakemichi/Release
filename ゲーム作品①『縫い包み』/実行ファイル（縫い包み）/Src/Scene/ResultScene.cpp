#include "../Application.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/SoundManager.h"
#include "../Manager/Camera.h"
#include "../Graphics/Drawable/3DModel/ModelMaterial.h"
#include "../Graphics/Drawable/3DModel/ModelRenderer.h"
#include "../Sound/SoundController.h"
#include "../Utility/Utility.h"
#include "../UI/Common/Button.h"
#include "ResultScene.h"

//スポットライト
constexpr float LIGHT_OUT_ANGLE_DEG = 65.0f;	//外側の角度
constexpr float LIGHT_IN_ANGLE_DEG = 40.0f;		//内側の角度
constexpr float LIGHT_RANGE = 2000.0f;			//有効距離
constexpr float LIGHT_ATTEN_0 = 0.2f;			//距離減衰パラメータ０
constexpr float LIGHT_ATTEN_1 = 0.00175f;		//距離減衰パラメータ１
constexpr float LIGHT_ATTEN_2 = 0.0f;			//距離減衰パラメータ２

//カメラのターゲット座標
constexpr VECTOR CAMERA_TARGET_POS = VECTOR{ -90, -1558, -750 };
//BGM音量
constexpr int BGM_VOLUME = 120;
//ステージ座標
constexpr VECTOR STAGE_POS = VECTOR{ 170,-1970,-800 };

//ステージ描画座標
constexpr int STAGE_DRAWING_LOCAL_POS = -250;
//結果描画座標
constexpr int RESULT_DRAWING_POS_X = 300;
constexpr int RESULT_DRAWING_POS_Y = 50;

//ボタンUI
constexpr int BUTTON_UI_DRAWING_POS_X = 1000;
constexpr int BUTTON_UI_DRAWING_POS_Y = 400;
constexpr int SELECT_BUTTON_COLOR = 0xff0f0f;
constexpr int NORMAL_BUTTON_COLOR = 0xeeeeee;
//ボタン操作指示
constexpr int BUTTON_OPERATION_DRAWING_UNDER_DIS_Y = 90;
constexpr int OPERATIOM_UI_COLOR = 0xbbbbbb;

ResultScene::ResultScene(SceneManager& manager) :SceneBase(manager)
{
	//更新関数のセット
	updataFunc_ = [&](InputManager& input) {LoadingUpdate(input); };
	//描画関数のセット
	drawFunc_ = std::bind(&ResultScene::LoadingDraw, this);
}

ResultScene::~ResultScene(void)
{
}

void ResultScene::LoadData(void)
{
	auto& res = ResourceManager::GetInstance();

	//リザルト画像
	resultImg_ = res.Load(ResourceManager::SRC::RESULT_IMG).handleId_;

	//フォントの登録
	res.Load(ResourceManager::SRC::SELECT_CMD_FONT);
	buttnFontHandle_ = CreateFontToHandle(L"EbiharaNoKuseji", FONT_SIZE, 0);

	//タイトル用ステージモデル
	stageModelHandle_ = res.Load(ResourceManager::SRC::TITLE_STAGE_MODEL).handleId_;
	stageModelMaterial_ = std::make_unique<ModelMaterial>();
	stageModelMaterial_->DataLoad(
		ModelMaterial::VS_TYPE::MODEL_NORMAL_1FRAME,
		ModelMaterial::PS_TYPE::MODEL_NORMAL);

	//BGM
	bgm_ = std::make_shared<SoundController>();
	bgm_->DataLoad(ResourceManager::SRC::RESULT_BGM, false);

}

void ResultScene::Init(void)
{
	//ボタン選択肢
	buttons_.push_back(Button{ L"タイトルへ",std::bind(&ResultScene::TitleButtnProcess, this) });
	buttons_.push_back(Button{ L"終了する",std::bind(&ResultScene::ExitButtnProcess, this) });

	//カメラモードのセット（定点カメラ）
	sceneManager_.GetCamera().ChangeMode(Camera::MODE::ROTATION);
	//カメラのターゲットセット
	sceneManager_.GetCamera().SetTargetPos(CAMERA_TARGET_POS);

	//モデル作成
	SettingObject();

	//ライトの作成
	auto& camera = sceneManager_.GetCamera();
	lightHandle_ = CreateSpotLightHandle(
		camera.GetPos(), camera.GetAngles(),
		Utility::Deg2RadF(LIGHT_OUT_ANGLE_DEG), Utility::Deg2RadF(LIGHT_IN_ANGLE_DEG),
		LIGHT_RANGE,
		LIGHT_ATTEN_0, LIGHT_ATTEN_1, LIGHT_ATTEN_2
	);

	//ステージ用スクリーン
	stageScreen_ = MakeScreen(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, true);

	//BGM再生
	bgm_->Init(BGM_VOLUME);
	bgm_->PlayLoop();

}

void ResultScene::Update(InputManager& input)
{
	updataFunc_(input);
	return;
}

void ResultScene::Draw(void)
{
	drawFunc_();
	return;
}

void ResultScene::TitleButtnProcess(void)
{
	sceneManager_.ChangeScene(SceneManager::SCENE_ID::TITLE);
}

void ResultScene::ExitButtnProcess(void)
{
	//ゲームの終了
	Application::GetInstance().CloseWindows();
}

void ResultScene::LoadingUpdate(InputManager& input)
{
	//初期化
	Init();

	//フェードイン開始
	sceneManager_.StartFadeIn();

	//更新関数のセット
	updataFunc_ = [&](InputManager& input) {NormalUpdate(input); };
	//描画関数のセット
	drawFunc_ = std::bind(&ResultScene::NormalDraw, this);

}

void ResultScene::NormalUpdate(InputManager& input)
{
	//ライト更新
	auto dir = sceneManager_.GetCamera().GetAngles();
	auto pos = sceneManager_.GetCamera().GetPos();
	SetLightDirectionHandle(lightHandle_, dir);
	SetLightPositionHandle(lightHandle_, pos);

	//モデルマテリアル：定数バッファの更新
	//フォグ情報の定数バッファ更新
	VECTOR cameraPos = sceneManager_.GetInstance().GetCamera().GetPos();
	float fogEnd, fogStart;
	GetFogStartEnd(&fogStart, &fogEnd);
	stageModelMaterial_->SetConstBufVS(0, FLOAT4{ cameraPos.x,cameraPos.y,cameraPos.z,fogStart });
	stageModelMaterial_->SetConstBufVS(1, FLOAT4{ fogEnd ,0,0,0 });

	//選択肢操作
	BtnSelectProcess(static_cast<int>(buttons_.size()));

	//決定ボタン押下判断
	bool isTrgDownSpace = input.IsTrgDown(KEY_INPUT_SPACE);
	if (isTrgDownSpace)
	{
		//カーソル番号に対応した処理の実行
		buttons_[cursolNum_].func_();
	}
}

void ResultScene::LoadingDraw(void)
{
}

void ResultScene::NormalDraw(void)
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


	//UI描画
	//リザルト画像
	DrawGraph(RESULT_DRAWING_POS_X, RESULT_DRAWING_POS_Y, resultImg_, true);

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

}

void ResultScene::SettingObject(void)
{
	//タイトル用ステージモデル
	MV1SetPosition(stageModelHandle_, STAGE_POS);
	MV1SetScale(stageModelHandle_, Utility::VECTOR_ONE);
	MV1SetRotationXYZ(stageModelHandle_, VECTOR{});
	stageModelMaterial_->Init();
	modelRenderers_.push_back(std::make_unique<ModelRenderer>(stageModelHandle_, *stageModelMaterial_));
}
