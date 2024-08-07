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

//�X�|�b�g���C�g
constexpr float LIGHT_OUT_ANGLE_DEG = 65.0f;	//�O���̊p�x
constexpr float LIGHT_IN_ANGLE_DEG = 40.0f;		//�����̊p�x
constexpr float LIGHT_RANGE = 2000.0f;			//�L������
constexpr float LIGHT_ATTEN_0 = 0.2f;			//���������p�����[�^�O
constexpr float LIGHT_ATTEN_1 = 0.00175f;		//���������p�����[�^�P
constexpr float LIGHT_ATTEN_2 = 0.0f;			//���������p�����[�^�Q

//BGM����
constexpr int BGM_VOLUME = 120;
//�X�e�[�W���W
constexpr VECTOR STAGE_POS = VECTOR{ 170,-1970,-800 };
//�ʂ�����݃��f��
constexpr VECTOR DOOL_POS = VECTOR{ -90,-1558,-750 };	//���W
constexpr VECTOR DOOL_SCALE = VECTOR{ 0.2f,0.2f,0.2f };	//�傫��

//�X�e�[�W�\����ʂ̃��[�J�����W
constexpr int STAGE_DRAWING_LOCAL_POS = -250;
//�^�C�g�����S�`����W
constexpr int LOG_DRAWING_POS_X = 550;
constexpr int LOG_DRAWING_POS_Y = 75;

//�{�^��UI
constexpr int BUTTON_UI_DRAWING_POS_X = 1000;
constexpr int BUTTON_UI_DRAWING_POS_Y = 400;
constexpr int SELECT_BUTTON_COLOR = 0xff0f0f;
constexpr int NORMAL_BUTTON_COLOR = 0xeeeeee;
//�{�^������w��
constexpr int BUTTON_OPERATION_DRAWING_UNDER_DIS_Y = 90;
constexpr int OPERATIOM_UI_COLOR = 0xbbbbbb;

//�O���b�`�V�F�[�_�[
constexpr float GLITC_SHADER_TIME_VALUE = 5.0f;		//�^�C�������l
constexpr float GLITC_SHADER_AMPLITUDE_VALUE = 10.0f;		//�U���l


TitleScene::TitleScene(SceneManager& manager):SceneBase(manager)
{
	//�X�V�֐��̃Z�b�g
	updataFunc_ = [&](InputManager& input) {LoadingUpdate(input); };
	//�`��֐��̃Z�b�g
	drawFunc_ = std::bind(&TitleScene::LoadingDraw, this);
}

TitleScene::~TitleScene(void)
{
}

void TitleScene::LoadData(void)
{
	loadingTime_ = 0.0f;
	auto& res = ResourceManager::GetInstance();

	//�񓯊��ǂݍ��݂�true�ɂ���
	SetUseASyncLoadFlag(true);

	//�^�C�g���摜
	titleImg_ = res.Load(ResourceManager::SRC::TITLE_IMG).handleId_;

	//�t�H���g�̓o�^
	res.Load(ResourceManager::SRC::SELECT_CMD_FONT);
	buttnFontHandle_ = CreateFontToHandle(L"EbiharaNoKuseji", FONT_SIZE, 0);

	//���f���̓o�^
	//�^�C�g���p�X�e�[�W���f��
	stageModelHandle_ = res.Load(ResourceManager::SRC::TITLE_STAGE_MODEL).handleId_;
	//�ʂ�����݃��f��
	doolModelHandle_ = res.Load(ResourceManager::SRC::DOOL_MONKEY).handleId_;

	//�}�e���A��
	//�X�e�[�W���f���}�e���A��
	stageModelMaterial_ = std::make_unique<ModelMaterial>();
	stageModelMaterial_->DataLoad(
		ModelMaterial::VS_TYPE::MODEL_NORMAL_1FRAME,
		ModelMaterial::PS_TYPE::MODEL_NORMAL);
	//�ʂ�����݃��f���}�e���A��
	doolModelMaterial_ = std::make_unique<ModelMaterial>();
	doolModelMaterial_->DataLoad(
		ModelMaterial::VS_TYPE::MODEL_NORMAL_1FRAME,
		ModelMaterial::PS_TYPE::MODEL_NORMAL);

	// �O���b�`
	glitchMaterial_ = std::make_unique<PostEffectMaterial>();
	glitchMaterial_->DataLoad(PostEffectMaterial::PS_TYPE::GLITCH);
	int noiseImg = res.Load(ResourceManager::SRC::NOISE_IMG).handleId_;	//�m�C�Y�摜
	glitchMaterial_->SetTextures({ noiseImg });

	//BGM
	bgm_ = std::make_shared<SoundController>();
	bgm_->DataLoad(ResourceManager::SRC::TITLE_BGM,false);
}

void TitleScene::Init(void)
{
	//�{�^���I����
	buttons_.push_back(Button{ L"�n�߂�",std::bind(&TitleScene::StartButtnProcess, this) });
	buttons_.push_back(Button{ L"�Q�[�����[��",std::bind(&TitleScene::InstructButtnProcess, this) });
	buttons_.push_back(Button{ L"�I������",std::bind(&TitleScene::ExitButtnProcess, this) });

	//�I�u�W�F�N�g���f���̐ݒ�
	SettingObject();

	//�J�������[�h�̃Z�b�g�i��]�j
	sceneManager_.GetCamera().ChangeMode(Camera::MODE::ROTATION);
	//�J�����̃^�[�Q�b�g�Z�b�g
	sceneManager_.GetCamera().SetTargetPos(doolPos_);

	//���C�g�̍쐬
	auto& camera = sceneManager_.GetCamera();
	lightHandle_ = CreateSpotLightHandle(
		camera.GetPos(), camera.GetAngles(),
		Utility::Deg2RadF(LIGHT_OUT_ANGLE_DEG), Utility::Deg2RadF(LIGHT_IN_ANGLE_DEG),
		LIGHT_RANGE,
		LIGHT_ATTEN_0, LIGHT_ATTEN_1, LIGHT_ATTEN_2
	);

	//�X�e�[�W�p�X�N���[��
	stageScreen_ = MakeScreen(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y,true);


	//�|�X�g�G�t�F�N�g
	glitchMaterial_->Init();
	// �s�N�Z���V�F�[�_�[�p�̃����_���[
	postEffectRenderer_.push_back(std::move(std::make_unique<PostEffectRenderer>(*glitchMaterial_)));



	//BGM�Đ�
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
	//�^�C�g���p�X�e�[�W���f��
	MV1SetPosition(stageModelHandle_, STAGE_POS);
	MV1SetScale(stageModelHandle_, Utility::VECTOR_ONE);
	MV1SetRotationXYZ(stageModelHandle_, VECTOR{});
	stageModelMaterial_->Init();
	modelRenderers_.push_back(std::make_unique<ModelRenderer>(stageModelHandle_, *stageModelMaterial_));

	//�ʂ�����݃��f��
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
	//PFD�ւ̃����N
	const wchar_t* pdfPath = L"��i�T�v.pdf";
	ShellExecute(NULL, L"open", pdfPath, NULL, NULL, SW_SHOWNORMAL);
}

void TitleScene::ExitButtnProcess(void)
{
	//�Q�[���̏I��
	Application::GetInstance().CloseWindows();
}

void TitleScene::LoadingUpdate(InputManager& input)
{
	bool loadTimeOver = Utility::TimeOver(loadingTime_, LOADING_TIME);

	//���[�h���������������f
	if (GetASyncLoadNum() == 0 && loadTimeOver)
	{
		SetUseASyncLoadFlag(false);

		//����������
		Init();

		//�t�F�[�h�C���J�n
		sceneManager_.StartFadeIn();

		//�X�V�֐��̃Z�b�g
		updataFunc_ = [&](InputManager& input) {NormalUpdate(input); };
		//�`��֐��̃Z�b�g
		drawFunc_ = std::bind(&TitleScene::NormalDraw, this);
	}
}

void TitleScene::NormalUpdate(InputManager& input)
{

	//���C�g�X�V
	auto dir = sceneManager_.GetCamera().GetAngles();
	auto pos = sceneManager_.GetCamera().GetPos();
	SetLightDirectionHandle(lightHandle_, dir);
	SetLightPositionHandle(lightHandle_, pos);

	//�I��������
	BtnSelectProcess(static_cast<int>(buttons_.size()));


	//���f���}�e���A���F�萔�o�b�t�@�̍X�V
	//�t�H�O���̒萔�o�b�t�@�X�V
	VECTOR cameraPos = sceneManager_.GetInstance().GetCamera().GetPos();
	float fogEnd, fogStart;
	GetFogStartEnd(&fogStart, &fogEnd);
	stageModelMaterial_->SetConstBufVS(0, FLOAT4{ cameraPos.x,cameraPos.y,cameraPos.z,fogStart });
	stageModelMaterial_->SetConstBufVS(1, FLOAT4{ fogEnd ,0,0,0 });

	doolModelMaterial_->SetConstBufVS(0, FLOAT4{ cameraPos.x,cameraPos.y,cameraPos.z,fogStart });
	doolModelMaterial_->SetConstBufVS(1, FLOAT4{ fogEnd ,0,0,0 });

	//�O���b�`�F�萔�o�b�t�@�X�V
	auto totalTime = sceneManager_.GetTotalTime();
	glitchMaterial_->SetConstBuf(0, FLOAT4{ totalTime,GLITC_SHADER_TIME_VALUE,GLITC_SHADER_AMPLITUDE_VALUE });	//�^�C�������l�A�U���l


	//����{�^���������f
	bool isTrgDownSpace = input.IsTrgDown(KEY_INPUT_SPACE);
	if (isTrgDownSpace)
	{
		//�J�[�\���ԍ��ɑΉ����������̎��s
		buttons_[cursolNum_].func_();
	}
}

void TitleScene::LoadingDraw(void)
{
	//�unow loading�v�̕`��
	DrawNowLoading();
}

void TitleScene::NormalDraw(void)
{
	//�w�i
	DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, 0x0, true);

	// �`���O���t�B�b�N�̈�̎w��
	sceneManager_.SetDrawingScreen(stageScreen_);

	//���f���`��
	for (const auto& renderer : modelRenderers_)
	{
		renderer->Draw();
	}

	SetDrawScreen(sceneManager_.GetMainScreen());
	DrawGraph(STAGE_DRAWING_LOCAL_POS, 0, stageScreen_, true);

	//UI
	//�^�C�g���摜
	DrawGraph(LOG_DRAWING_POS_X, LOG_DRAWING_POS_Y, titleImg_, true);

	int idx = 0;
	int localDis = LINE_SPACING;
	//�{�^���e�L�X�g�̕`��
	for (const auto& btn : buttons_)
	{
		//�{�^���̐F�w��
		int btnColor = (idx == cursolNum_) ? SELECT_BUTTON_COLOR : NORMAL_BUTTON_COLOR;
		DrawStringToHandle(BUTTON_UI_DRAWING_POS_X, BUTTON_UI_DRAWING_POS_Y + idx * localDis, btn.text_.c_str(), btnColor, buttnFontHandle_);
	
		idx++;
	}

	//�I�y���[�V����UI
	DrawStringFToHandle(
		BUTTON_UI_DRAWING_POS_X, Application::SCREEN_SIZE_Y- BUTTON_OPERATION_DRAWING_UNDER_DIS_Y,
		L"[w][s]�L�[:�I��\nSpace�@�@:����", 
		OPERATIOM_UI_COLOR, buttnFontHandle_);


	//�|�X�g�G�t�F�N�g
	for (const auto& renderer : postEffectRenderer_)
	{
		renderer->Draw();
	}

}


