#include <DxLib.h>
#include "../Manager/DataManager/UnitDataManager.h"
#include "../Manager/DataManager/BattleDataManager.h"
#include "../Manager/DataManager/SoundManager.h"
#include "../Manager/DeckManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Utility/Utility.h"
#include "../Utility/DrawShader.h"
#include "../Utility/Button/UnitButton.h"
#include "../Common/Vector2.h"
#include "../Application.h"
#include "../_debug/_DebugDispOut.h"
#include "ResultScene.h"

//�R�����g�p�t�H���g
constexpr int COMMNET_FONT_SIZE = 20;
constexpr int COMMNET_FONT_THICK = 20;
constexpr int COMMNET_FONT_COLOR = 0xdddddd;
const Vector2 COMMNET_POS = { 300,120 };
//�R�����g�{�b�N�X
const Vector2 COMMNET_BOX_POS = Vector2{ 280,100 };
const Vector2 COMMNET_BOX_SIZE = Vector2{ 360,60 };
//�g����
const Vector2 DEVIL_SIZE = Vector2{ 360,360 };
//�w���e�L�X�g�I�t�Z�b�g
constexpr int OPERATOR_TEXT_OFFSET_Y = 30;
constexpr float OPERATOR_TEXT_SHOW_TIME = 3.0f;	//�\������

//���j�b�g�摜
const Vector2 UNIT_SHOW_BASE_POS = Vector2{ 630,330 };	//�\�����W�F�x�[�X
const int UNIT_SHOW_OFFSET_X= 200;	//�I�t�Z�b�gX


ResultScene::ResultScene(void)
{
	fontHandle_=0;
	devilImg_ = 0;
	devilPos_ = Vector2{};
	cmtBoxImg_ = 0;
	unitBackImg_ = 0;

}

ResultScene::~ResultScene(void)
{
}

void ResultScene::Init(void)
{
	//�w�i
	bgImg_ = LoadGraph("Data/Image/bg/result.png");

	//�t�H���g
	fontHandle_ = CreateFontToHandle("������", COMMNET_FONT_SIZE, COMMNET_FONT_THICK,
		DX_FONTTYPE_ANTIALIASING_4X4);
	//�R�����g�{�b�N�X�摜
	cmtBoxImg_ = LoadGraph("Data/Image/UI/CommentBox.png");
	//������
	devilImg_ = LoadGraph("Data/Image/UI/������_���J��.png");
	devilPos_ = { -20,50 };

	//���j�b�g�J�[�h��
	unitBackImg_ = LoadGraph("Data/Image/UI/UnitBack.png");
	//�������j�b�g�̃{�^������
	CreateUnitBtn();


	//�T�E���h�̓o�^
	auto& snd = SoundManager::GetInstance();
	bgmHandle_ = snd.LoadSound(SoundManager::SOUND_TYPE::RESULT_BGN);
	//BGM�̍Đ�
	snd.PlaySoundBGM(bgmHandle_);

	//�G���j�b�g���f�b�L�ɃZ�b�g
	auto& bd = BattleDataManager::GetInstance().GetBattleData();
	int num = bd.eDeck.dArray_[0];
	DeckManager::GetInstance().AddUnit(num);
	num = bd.eDeck.dArray_[1];
	DeckManager::GetInstance().AddUnit(num);
	num = bd.eDeck.dArray_[2];
	DeckManager::GetInstance().AddUnit(num);

}

void ResultScene::Update(void)
{
	//�f���^�^�C��
	auto delta = SceneManager::GetInstance().GetDeltaTime();
	totalTime_ += delta;

	//�V�[���J��
	auto& ins = InputManager::GetInstance();
	if (ins.IsTrgMouseLeft())
	{
		SceneManager::GetInstance().ChangeScene(
			SceneManager::SCENE_ID::SELECT,true);
		return;
	}

	//�{�^���X�V
	for (auto& btn : unitBtns_)
	{
		btn->Update();
	}
}

void ResultScene::Draw(void)
{
	int sx = Application::SCREEN_SIZE_X;
	int sy = Application::SCREEN_SIZE_Y;
	auto& ds = DrawShader::GetInstance();

	//�w�i�`��
	ds.DrawExtendGraphToShader(
		{ 0,0 }, { sx,sy }, bgImg_);

	//�R�����g�{�b�N�X
	ds.DrawExtendGraphToShader(
		COMMNET_BOX_POS, COMMNET_BOX_SIZE, cmtBoxImg_);
	//�R�����g
	DrawFormatStringFToHandle(
		COMMNET_POS.x, COMMNET_POS.y, COMMNET_FONT_COLOR, fontHandle_, "�V�������Ԃ����������");
	//�g����
	ds.DrawExtendGraphToShader(
		devilPos_, DEVIL_SIZE, devilImg_);

	//�������j�b�g�{�^���̕`��
	DrawGetUnits();


	//�N���b�N�w��
	std::string inst = "�N���b�N���āA�Z���N�g��ʂ֖߂�";
	auto len = Utility::CalcStringLength(inst, fontHandle_);
	if (fmodf(totalTime_, OPERATOR_TEXT_SHOW_TIME) >= 1.0f)
	{
		DrawFormatStringFToHandle(
			sx - len, sy - OPERATOR_TEXT_OFFSET_Y, COMMNET_FONT_COLOR, fontHandle_, inst.c_str());
	}

#ifdef DEBUG
	DrawPosition();
#endif // _DEBUG

}

void ResultScene::Release(void)
{
	DeleteGraph(bgImg_);

	//BGM�̒�~
	StopSoundMem(bgmHandle_);
}

void ResultScene::CreateUnitBtn(void)
{
	auto& bDataMng = BattleDataManager::GetInstance();
	auto& uDataMng = UnitDataManager::GetInstance();

	Vector2 basePos = UNIT_SHOW_BASE_POS;


	//���j�b�g�{�^���̐���
	int count = 0;
	auto& enemyUnits = bDataMng.GetBattleData().eDeck.dArray_;
	for (auto& num : enemyUnits)
	{
		if (num <= 0)continue;

		//���j�b�g�f�[�^
		auto uData = uDataMng.GetUnitData(num);
		//�\�����W�����炷
		Vector2 drawPos = basePos;
		if (count == 1)
		{
			//���ɂ��炷
			drawPos += {-UNIT_SHOW_OFFSET_X, 0};
		}
		if (count == 2)
		{
			//�E�ɂ��炷
			drawPos += {UNIT_SHOW_OFFSET_X, 0};
		}

		//�{�^������
		UnitButton* ub = new UnitButton();
		ub->Create(drawPos, unitBackImg_, uData);
		//�}��
		unitBtns_.emplace_back(ub);

		count++;
	}
}

void ResultScene::DrawGetUnits(void)
{
	for (auto& btn : unitBtns_)
	{
		btn->Draw();
	}
}
