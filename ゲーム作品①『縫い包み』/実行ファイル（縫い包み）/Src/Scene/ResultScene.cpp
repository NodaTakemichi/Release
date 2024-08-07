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

//�X�|�b�g���C�g
constexpr float LIGHT_OUT_ANGLE_DEG = 65.0f;	//�O���̊p�x
constexpr float LIGHT_IN_ANGLE_DEG = 40.0f;		//�����̊p�x
constexpr float LIGHT_RANGE = 2000.0f;			//�L������
constexpr float LIGHT_ATTEN_0 = 0.2f;			//���������p�����[�^�O
constexpr float LIGHT_ATTEN_1 = 0.00175f;		//���������p�����[�^�P
constexpr float LIGHT_ATTEN_2 = 0.0f;			//���������p�����[�^�Q

//�J�����̃^�[�Q�b�g���W
constexpr VECTOR CAMERA_TARGET_POS = VECTOR{ -90, -1558, -750 };
//BGM����
constexpr int BGM_VOLUME = 120;
//�X�e�[�W���W
constexpr VECTOR STAGE_POS = VECTOR{ 170,-1970,-800 };

//�X�e�[�W�`����W
constexpr int STAGE_DRAWING_LOCAL_POS = -250;
//���ʕ`����W
constexpr int RESULT_DRAWING_POS_X = 300;
constexpr int RESULT_DRAWING_POS_Y = 50;

//�{�^��UI
constexpr int BUTTON_UI_DRAWING_POS_X = 1000;
constexpr int BUTTON_UI_DRAWING_POS_Y = 400;
constexpr int SELECT_BUTTON_COLOR = 0xff0f0f;
constexpr int NORMAL_BUTTON_COLOR = 0xeeeeee;
//�{�^������w��
constexpr int BUTTON_OPERATION_DRAWING_UNDER_DIS_Y = 90;
constexpr int OPERATIOM_UI_COLOR = 0xbbbbbb;

ResultScene::ResultScene(SceneManager& manager) :SceneBase(manager)
{
	//�X�V�֐��̃Z�b�g
	updataFunc_ = [&](InputManager& input) {LoadingUpdate(input); };
	//�`��֐��̃Z�b�g
	drawFunc_ = std::bind(&ResultScene::LoadingDraw, this);
}

ResultScene::~ResultScene(void)
{
}

void ResultScene::LoadData(void)
{
	auto& res = ResourceManager::GetInstance();

	//���U���g�摜
	resultImg_ = res.Load(ResourceManager::SRC::RESULT_IMG).handleId_;

	//�t�H���g�̓o�^
	res.Load(ResourceManager::SRC::SELECT_CMD_FONT);
	buttnFontHandle_ = CreateFontToHandle(L"EbiharaNoKuseji", FONT_SIZE, 0);

	//�^�C�g���p�X�e�[�W���f��
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
	//�{�^���I����
	buttons_.push_back(Button{ L"�^�C�g����",std::bind(&ResultScene::TitleButtnProcess, this) });
	buttons_.push_back(Button{ L"�I������",std::bind(&ResultScene::ExitButtnProcess, this) });

	//�J�������[�h�̃Z�b�g�i��_�J�����j
	sceneManager_.GetCamera().ChangeMode(Camera::MODE::ROTATION);
	//�J�����̃^�[�Q�b�g�Z�b�g
	sceneManager_.GetCamera().SetTargetPos(CAMERA_TARGET_POS);

	//���f���쐬
	SettingObject();

	//���C�g�̍쐬
	auto& camera = sceneManager_.GetCamera();
	lightHandle_ = CreateSpotLightHandle(
		camera.GetPos(), camera.GetAngles(),
		Utility::Deg2RadF(LIGHT_OUT_ANGLE_DEG), Utility::Deg2RadF(LIGHT_IN_ANGLE_DEG),
		LIGHT_RANGE,
		LIGHT_ATTEN_0, LIGHT_ATTEN_1, LIGHT_ATTEN_2
	);

	//�X�e�[�W�p�X�N���[��
	stageScreen_ = MakeScreen(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, true);

	//BGM�Đ�
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
	//�Q�[���̏I��
	Application::GetInstance().CloseWindows();
}

void ResultScene::LoadingUpdate(InputManager& input)
{
	//������
	Init();

	//�t�F�[�h�C���J�n
	sceneManager_.StartFadeIn();

	//�X�V�֐��̃Z�b�g
	updataFunc_ = [&](InputManager& input) {NormalUpdate(input); };
	//�`��֐��̃Z�b�g
	drawFunc_ = std::bind(&ResultScene::NormalDraw, this);

}

void ResultScene::NormalUpdate(InputManager& input)
{
	//���C�g�X�V
	auto dir = sceneManager_.GetCamera().GetAngles();
	auto pos = sceneManager_.GetCamera().GetPos();
	SetLightDirectionHandle(lightHandle_, dir);
	SetLightPositionHandle(lightHandle_, pos);

	//���f���}�e���A���F�萔�o�b�t�@�̍X�V
	//�t�H�O���̒萔�o�b�t�@�X�V
	VECTOR cameraPos = sceneManager_.GetInstance().GetCamera().GetPos();
	float fogEnd, fogStart;
	GetFogStartEnd(&fogStart, &fogEnd);
	stageModelMaterial_->SetConstBufVS(0, FLOAT4{ cameraPos.x,cameraPos.y,cameraPos.z,fogStart });
	stageModelMaterial_->SetConstBufVS(1, FLOAT4{ fogEnd ,0,0,0 });

	//�I��������
	BtnSelectProcess(static_cast<int>(buttons_.size()));

	//����{�^���������f
	bool isTrgDownSpace = input.IsTrgDown(KEY_INPUT_SPACE);
	if (isTrgDownSpace)
	{
		//�J�[�\���ԍ��ɑΉ����������̎��s
		buttons_[cursolNum_].func_();
	}
}

void ResultScene::LoadingDraw(void)
{
}

void ResultScene::NormalDraw(void)
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


	//UI�`��
	//���U���g�摜
	DrawGraph(RESULT_DRAWING_POS_X, RESULT_DRAWING_POS_Y, resultImg_, true);

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

}

void ResultScene::SettingObject(void)
{
	//�^�C�g���p�X�e�[�W���f��
	MV1SetPosition(stageModelHandle_, STAGE_POS);
	MV1SetScale(stageModelHandle_, Utility::VECTOR_ONE);
	MV1SetRotationXYZ(stageModelHandle_, VECTOR{});
	stageModelMaterial_->Init();
	modelRenderers_.push_back(std::make_unique<ModelRenderer>(stageModelHandle_, *stageModelMaterial_));
}
