#include <DxLib.h>
#include <cstdlib>
#include "../Application.h"
#include "../Manager/DataManager/UnitDataManager.h"
#include "../Manager/DataManager/BattleDataManager.h"
#include "../Manager/DataManager/SoundManager.h"
#include "../Manager/DeckManager.h"
#include "../Manager/DataManager/SoundManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Common/Vector2.h"

#include "../Utility/Utility.h"
#include "../Utility/DrawShader.h"
#include "../Utility/Button/RectButton.h"
#include "../Utility/Button/CircleButton.h"
#include"../_debug/_DebugDispOut.h"
#include "SelectScene.h"

using PsType = DrawShader::PS_TYPE;
//�X�e�[�W�A�L�����̑I���{�b�N�X
const Vector2 SELECT_BOX_POS = { 660,180 };
const Vector2 SELECT_BOX_SIZE = { 560,320 };
//�Z���N�g���[�h�摜
const Vector2 SELECT_MOODE_LOGO_POS = { 680,0 };
const Vector2 SELECT_MOODE_LOGO_SIZE = { 550,550 };
//����{�^���̍��W
const Vector2 DECI_BOX_POS = { 820,610 };

//������
const Vector2 DEVIL_POS = { 0,100 };	//���W
const int DEVIL_SHAKE_VALUE = 30;		//���炵�l


//�o�b�N�{�^���̃��[�J�����W
const Vector2 BACK_BUTTON_LOCAL_POS = { 0, 20 };
//�R�����g�{�b�N�X�̍��W
const Vector2 COMMENT_BOX_POS = { 0, 470 };


//�Q�[�����[�h�p�t�H���g
constexpr int MODE_FONT_SIZE = 50;
constexpr int MODE_FONT_THICK = 20;
constexpr int MODE_FONT_COLOR = 0xdddddd;
const Vector2 MODE_FONT_OFFSET_POS = { 0, 60 };
//�R�����g�p�t�H���g
constexpr int COMMNET_FONT_SIZE = 20;
constexpr int COMMNET_FONT_THICK = 20;
constexpr int COMMNET_FONT_COLOR = 0xdddddd;
const Vector2 COMMNET_FONT_OFFSET_POS = { 40,40 };

//�X�N���[�����x
constexpr float SCROLL_SPEED = 0.5f;

//���j�b�g�\��
const Vector2 UNIT_SHOW_SIZE = { 200,200 };	//�T�C�Y
const Vector2 UNIT_SHOW_LEFT_POS = { 50,110 };	//��
const Vector2 UNIT_SHOW_CENTER_POS = { 190,10 };	//��
const Vector2 UNIT_SHOW_RIGHT_POS = { 330,110 };	//�E


SelectScene::SelectScene(void)
{
	scrollTime_=0.0f;
	isMoveWheel_=false;

	 page_= SELECT_PAGE::MODE;
	 mode_= SELECT_MODE::TITLE;

	devilCmt_="";
	nowMode_="";

	selectImgPos_ = Vector2{};
	cmtPos_ = Vector2{};
	devilPos_ = Vector2{};
	modePos_ = Vector2{};

	backImg_=0;
	deciBtnImg_ = 0;
	nowEnemysNum_ = 0;
	nowStageNum_ = 0;
	selectedBoxImg_ = 0;
	selectedBoxMaskImg_ = 0;
	selectMaskScreen_ = 0;
	cmtBox_ = 0;
	cmtFontHandle_ = 0;
	devilImg_ = 0;
	shakeY_ = 0;
	scrollSE_ = 0;
	modeBox_ = 0;
	modeFontHandle_ = 0;
	modeBackBtnImg_ = 0;
	wheelSign_ = 0;
}

SelectScene::~SelectScene(void)
{
}

void SelectScene::Init(void)
{
	int sx = Application::SCREEN_SIZE_Y;
	int sy = Application::SCREEN_SIZE_Y;

	//�{�^���摜
	backImg_ = LoadGraph("Data/Image/UI/BackBtn.png");
	//�o�b�N�{�^���̐���
	backBtn_ = new CircleButton();
	auto backBtn = Vector2{ 0,sy } + BACK_BUTTON_LOCAL_POS;
	backBtn_->Create(backBtn, backImg_, -1);

	//����{�^���摜
	deciBtnImg_ = LoadGraph("Data/Image/UI/Select/DeciBtnBack.png");
	//����{�^���̐���
	deciBtn_ = new RectButton();
	deciBtn_->Create(DECI_BOX_POS,deciBtnImg_);

	//���[�h�{�^��UI����
	CreateModeBtn();

	//�摜�̓o�^
	bgImg_ = LoadGraph("Data/Image/bg/select.jpg");
	modeBox_ = LoadGraph("Data/Image/UI/ModeBox.png");
	cmtBox_ = LoadGraph("Data/Image/UI/CommentBox.png");

	//�I���{�b�N�X�摜
	selectedBoxImg_ = LoadGraph("Data/Image/UI/Select/SelectedBack.png");
	selectedBoxMaskImg_ = LoadGraph("Data/Image/UI/Select/SelectedBackMask.png");

	//�X�e�[�W���̓o�^
	StageDataInit();

	//�I��p�}�X�N�X�N���[��
	selectMaskScreen_ = MakeScreen(
		SELECT_BOX_SIZE.x, SELECT_BOX_SIZE.y, true);

	//�X�e�[�W�A�G�l�~�[�I���֘A������
	isMoveWheel_ = true;		
	selectImgPos_ = { 0,0 };	//�w�i�摜�ʒu
	scrollTime_ = 0.0;			//�X�N���[���o�ߎ���
	wheelSign_ = 0;				//�z�C�[������
	nowStageNum_ = 0;			//���X�e�[�W�ԍ�
	nowEnemysNum_ = 0;			//���G�l�~�[�Y�ԍ�
	
								
	//������
	totalTime_ = 0.0f;
	//���݃y�[�W
	page_ = SELECT_PAGE::MODE;

	//���[�h�{�b�N�X
	modePos_ = { 0,0 };
	nowMode_ = "MODE SELECT";
	modeFontHandle_ = CreateFontToHandle("������", MODE_FONT_SIZE, MODE_FONT_THICK,
		DX_FONTTYPE_ANTIALIASING_4X4);

	//�R�����g�{�b�N�X
	cmtPos_ = COMMENT_BOX_POS;
	SetDevilCmt("�Q�[�����[�h��I�����Ă����ȁI");
	cmtFontHandle_ = CreateFontToHandle("������", COMMNET_FONT_SIZE, COMMNET_FONT_THICK,
		DX_FONTTYPE_ANTIALIASING_4X4);

	//������
	devilImg_ = LoadGraph("Data/Image/UI/������_���J��.png");
	devilPos_ = DEVIL_POS;
	shakeY_ = 0;


	//���[�h
	ChangeSelectMode(SELECT_MODE::MAX);

	//�z�C�[��������
	GetMouseWheelRotVol();

	//�����֘A������
	auto& snd = SoundManager::GetInstance();
	scrollSE_	= snd.LoadSound(SoundManager::SOUND_TYPE::SCROLL);
	//BGM�Đ�
	bgmHandle_= snd.LoadSound(SoundManager::SOUND_TYPE::SELECT_BGM);
	snd.PlaySoundBGM(bgmHandle_);


}

void SelectScene::Update(void)
{
	//�f���^�^�C��
	auto delta = SceneManager::GetInstance().GetDeltaTime();
	totalTime_ += delta;

	//����
	auto& snd = SoundManager::GetInstance();

	//�}�X�R�b�g�摜�̂��炵�l
	int moveS = DEVIL_SHAKE_VALUE;
	shakeY_ = sinf(totalTime_) * moveS;

	//�e���[�h�{�^���X�V
	BtnProcess();

	//���݂̃X�e�[�W���
	StageData& nowStageData =
		stages_.at(static_cast<SELECT_STAGE>(nowStageNum_));
	if (page_ == SELECT_PAGE::MODE)
	{

	}
	else if (page_ == SELECT_PAGE::STAGE)
	{
		//�X�e�[�W�I��
		SelectSceollProcess();
		//�X�e�[�W�摜�X���C�h
		ScrollSelectImg(nowStageNum_, static_cast<int>(SELECT_STAGE::MAX));
		//�X�N���[�����͏������Ȃ�
		if (!isMoveWheel_)return;

		//����{�^��
		deciBtn_->Update();
		if (deciBtn_->ButtonDecision())
		{
			SetDevilCmt("�z�C�[���őΐ푊���I�ׂ邺�I\n���܂�����o�g���X�^�[�g���I");
			//���t�F�[�Y�ɐi��
			page_= SELECT_PAGE::ENEMY;
			return;
		}

		//�o�b�N�{�^��
		backBtn_->Update();
		if (backBtn_->ButtonDecision())
		{
			SetDevilCmt("�����X�^�[�Ɛ키���[�h����\n���Ă鑊���I�Ԃ񂾂�");

			int btnSizeX, btnSizeY;
			GetGraphSize(modeBackBtnImg_, &btnSizeX, &btnSizeY);
			//���[�h�{�^�����W�ύX
			for (auto& mode : modeBtn_)
			{
				mode.second.btn->SetMovedPos({ 0,-btnSizeY });
			}

			//�O�t�F�[�Y�ɖ߂�
			page_= SELECT_PAGE::MODE;
			return;
		}
	}
	else
	{
		//�ΐ푊��I��
		SelectSceollProcess();
		//�ΐ푊��摜�X���C�h
		ScrollSelectImg(nowEnemysNum_, nowStageData.enemys.size());

		//�X�N���[�����͏������Ȃ�
		if (!isMoveWheel_)return;


		//�o�b�N�{�^��
		backBtn_->Update();
		if (backBtn_->ButtonDecision())
		{
			SetDevilCmt("�z�C�[���Ńo�g���X�e�[�W��I�ׂ邺�I\n�G�̎�ނ��ς�邺�I");
			//�O�t�F�[�Y�ɖ߂�
			page_=SELECT_PAGE::STAGE;
			return;
		}

		//����{�^��
		deciBtn_->Update();
		if (deciBtn_->ButtonDecision())
		{
			//�f�b�L�A�o�g�����̃Z�b�g
			auto deck = DeckManager::GetInstance().GetDeck();
			Deck enemys = nowStageData.enemys[nowEnemysNum_];
			BattleDataManager::GetInstance().SetBattleData({
				deck,enemys,nowStageNum_,0 });


			//�V�[���J��
			SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME, true);
			return;
		}
	}

}

void SelectScene::Draw(void)
{
	auto& sx = Application::SCREEN_SIZE_X;
	auto& sy = Application::SCREEN_SIZE_Y;
	auto& ds = DrawShader::GetInstance();

	//�w�i
	ds.DrawExtendGraphToShader(
		{ 0,0 }, { sx,sy }, bgImg_, PsType::TEXTURE, COLOR_F{});

	//�����ܕ`��
	ds.DrawGraphToShader(
		{ devilPos_.x ,devilPos_.y + shakeY_ }, devilImg_, PsType::TEXTURE);

	//���[�h�{�b�N�X�`��
	DrawModeBox();
	//�R�����g�{�b�N�X
	DrawCmtBox();


	//���[�h�{�^��
	for (auto& btn : modeBtn_)
	{
		btn.second.btn->Draw();
	}

	if (page_ == SELECT_PAGE::MODE)
	{
		//�Z���N�g���[�h�摜
		if (mode_ != SELECT_MODE::MAX)
		{
			ds.DrawExtendGraphToShader(
				SELECT_MOODE_LOGO_POS, SELECT_MOODE_LOGO_SIZE, modeBtn_.at(mode_).modeImg);
		}
	}
	else if (page_ == SELECT_PAGE::STAGE)
	{
		//�X�e�[�W�I��
		DrawSelectBoxStage();

		//����{�^��
		deciBtn_->Draw();

		//�o�b�N�{�^��
		backBtn_->Draw();
	}
	else if (page_ == SELECT_PAGE::ENEMY)
	{
		//�I���G�l�~�[�`��
		DrawSelectBoxEnemy();

		//�o�b�N�{�^��
		backBtn_->Draw();

		//����{�^��
		deciBtn_->Draw();
	}
}

void SelectScene::Release(void)
{
	//������~
	StopSoundMem(bgmHandle_);

	//�摜�J��
	DeleteGraph(backImg_);
	DeleteGraph(deciBtnImg_);
	DeleteGraph(bgImg_);
	DeleteGraph(modeBox_);
	DeleteGraph(cmtBox_);
	DeleteGraph(selectedBoxImg_);
	DeleteGraph(modeBackBtnImg_);


	backBtn_->Release();
	delete backBtn_;

	deciBtn_->Release();
	delete deciBtn_;
}

void SelectScene::DrawModeBox(void)
{
	DrawGraph(modePos_.x, modePos_.y, modeBox_, true);
	int x, y;
	GetGraphSize(modeBox_, &x, &y);

	auto len = Utility::CalcStringLength(nowMode_, modeFontHandle_) / 2;
	DrawFormatStringToHandle(
		modePos_.x + (x / 2) - len,
		modePos_.y + MODE_FONT_OFFSET_POS.y,
		MODE_FONT_COLOR, modeFontHandle_, nowMode_.c_str());
}

void SelectScene::DrawCmtBox(void)
{
	DrawGraph(cmtPos_.x, cmtPos_.y, cmtBox_, true);

	Vector2 offset = Vector2{ cmtPos_.x,cmtPos_.y } + COMMNET_FONT_OFFSET_POS;
	DrawFormatStringToHandle(
		offset.x, offset.y,
		COMMNET_FONT_COLOR, cmtFontHandle_, devilCmt_.c_str());

}

void SelectScene::BtnProcess()
{
	//�{�^���X�V
	for (auto& btn : modeBtn_)
	{
		btn.second.btn->Update();
	}

	//�{�^���F�}�E�X���{�^����ɂ��邩���f
	for (auto& btn : modeBtn_)
	{
		if (btn.second.btn->MouseOnButton())
		{
			ChangeSelectMode(btn.first);
		}
	}

	//�{�^���F�N���b�N����
	for (auto& btn : modeBtn_)
	{
		if (!btn.second.btn->ButtonDecision())continue;

		//�{�^���������ꂽ�ꍇ�A���̃{�^���������s��
		SelectBtnProcess();
	}
}

void SelectScene::BattleBtnProcess(void)
{
	SetDevilCmt("�z�C�[���Ńo�g���X�e�[�W��I�ׂ邺�I\n�G�̎�ނ��ς�邺�I");

	//�X�e�[�W�I���ֈړ�
	page_= SELECT_PAGE::STAGE;

	//���[�h�{�^�����W�̕ύX
	int btnSizeX, btnSizeY;
	GetGraphSize(modeBackBtnImg_, &btnSizeX, &btnSizeY);
	for (auto& mode : modeBtn_)
	{
		mode.second.btn->SetMovedPos({ 0,btnSizeY });
	}
}

void SelectScene::EditBtnProcess(void)
{
	//�V�[���J��
	SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::DECK_EDIT, true);
}

void SelectScene::TitleBtnProcess(void)
{
	//�V�[���J��
	SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE, true);
}

void SelectScene::CreateModeBtn(void)
{
	//�X�N���[���T�C�Y
	auto& sx = Application::SCREEN_SIZE_X;
	auto& sy = Application::SCREEN_SIZE_Y;

	//���[�h�{�^���̔w�ʉ摜
	modeBackBtnImg_ = LoadGraph("./Data/Image/UI/ModeBtnImg.png");
	int btnSizeX, btnSizeY;
	GetGraphSize(modeBackBtnImg_, &btnSizeX, &btnSizeY);


	std::string path = "Data/Image/UI/";
	int n = 0;
	// �{�^�����������_��
	auto createBtn = [&](SELECT_MODE mode, std::string btnImgPath, std::string modeImgPath) {

		//���[�h�摜
		std::string fileName = path + modeImgPath;
		int modeImg = LoadGraph(fileName.c_str());

		//���W
		Vector2 pos = {
			(sx / 2) - (btnSizeX ) - (btnSizeX / 2) + (btnSizeX * n)  ,
			sy - btnSizeY };
		//�{�^���摜
		fileName = path + btnImgPath;
		int btnImg = LoadGraph(fileName.c_str());

		//�{�^������
		RectButton* b = new RectButton();
		b->Create(pos, modeBackBtnImg_, btnImg);

		//�}��
		ModeData md = { modeImg,b };
		modeBtn_.emplace(std::make_pair(mode, md));

		n++;
	};


	createBtn(SELECT_MODE::BATTLE,		"�o�g��.png",		"Select/Mode_Battle.png");
	createBtn(SELECT_MODE::DECK_EDIT,	"�f�b�L�ҏW.png",	"Select/Mode_DeckEdit.png");
	createBtn(SELECT_MODE::TITLE,		"�ޏo.png",			"Select/Mode_Title.png");
}

void SelectScene::SelectBtnProcess(void)
{
	switch (mode_)
	{
	case SelectScene::SELECT_MODE::BATTLE:
		BattleBtnProcess();
		break;
	case SelectScene::SELECT_MODE::DECK_EDIT:
		EditBtnProcess();
		break;
	case SelectScene::SELECT_MODE::RULE_BOOK:
		break;
	case SelectScene::SELECT_MODE::CREDIT:
		break;
	case SelectScene::SELECT_MODE::TITLE:
		TitleBtnProcess();
		break;
	case SelectScene::SELECT_MODE::MAX:
		break;
	default:
		break;
	}
}

void SelectScene::ChangeSelectMode(const SELECT_MODE& mode)
{
	//�����������ꍇ�A�������Ȃ�
	if (mode_ == mode)return;

	mode_ = mode;

	//������炷

	switch (mode_)
	{
	case SelectScene::SELECT_MODE::BATTLE:
		SetDevilCmt("�����X�^�[�Ɛ키���[�h����\n���Ă鑊���I�Ԃ񂾂�");

		break;
	case SelectScene::SELECT_MODE::DECK_EDIT:
		SetDevilCmt("�o�g�����j�b�g��I�ׂ邺");

		break;
	case SelectScene::SELECT_MODE::RULE_BOOK:
		SetDevilCmt("���݁A�J�����ł�");

		break;
	case SelectScene::SELECT_MODE::CREDIT:
		SetDevilCmt("���݁A�J�����ł�");

		break;
	case SelectScene::SELECT_MODE::TITLE:
		SetDevilCmt("�^�C�g���ɖ߂邺");

		break;
	case SelectScene::SELECT_MODE::MAX:
		break;
	default:
		break;
	}
}

void SelectScene::SetDevilCmt(const std::string& cmt)
{
	devilCmt_ = cmt;
}

void SelectScene::StageDataInit()
{
	StageData data;

	//��
	data.backImg = LoadGraph("Data/Image/bg/red.jpg");
	data.enemys.push_back({ 0, 1, 2 });
	data.enemys.push_back({ 3, 4, 5 });
	data.enemys.push_back({ 6, 7, 8 });
	data.enemys.push_back({ 9,-1,-1 });
	stages_.insert(std::make_pair(SELECT_STAGE::VOLCANO, data));
	data.enemys.clear();

	//��
	data.backImg = LoadGraph("Data/Image/bg/blue.jpg");
	data.enemys.push_back({ 10,11,12 });
	data.enemys.push_back({ 13,14,15 });
	data.enemys.push_back({ 16,17,18 });
	data.enemys.push_back({ 19,-1,-1 });
	stages_.insert(std::make_pair(SELECT_STAGE::OCEAN, data));
	data.enemys.clear();

	//�n
	data.backImg = LoadGraph("Data/Image/bg/earth.jpg");
	data.enemys.push_back({ 20,21,22 });
	data.enemys.push_back({ 23,24,25 });
	data.enemys.push_back({ 26,27,28 });
	data.enemys.push_back({ 29,-1,-1 });
	stages_.insert(std::make_pair(SELECT_STAGE::WILDRNESS, data));
	data.enemys.clear();

	//��
	data.backImg = LoadGraph("Data/Image/bg/green.jpg");
	data.enemys.push_back({ 30,31,32 });
	data.enemys.push_back({ 33,34,35 });
	data.enemys.push_back({ 36,37,38 });
	data.enemys.push_back({ 39,-1,-1 });
	stages_.insert(std::make_pair(SELECT_STAGE::FOREST, data));
	data.enemys.clear();

	//��
	data.backImg = LoadGraph("Data/Image/bg/holy.jpg");
	data.enemys.push_back({ 40,41,42 });
	data.enemys.push_back({ 43,44,45 });
	data.enemys.push_back({ 46,47,48 });
	data.enemys.push_back({ 49,-1,-1 });
	stages_.insert(std::make_pair(SELECT_STAGE::SANCTUARY, data));
	data.enemys.clear();

	//��
	data.backImg = LoadGraph("Data/Image/bg/dark.jpg");
	data.enemys.push_back({ 50,51,52 });
	data.enemys.push_back({ 53,54,55 });
	data.enemys.push_back({ 56,57,58 });
	data.enemys.push_back({ 59,-1,-1 });
	stages_.insert(std::make_pair(SELECT_STAGE::ABYSS, data));
	data.enemys.clear();

}

void SelectScene::SelectSceollProcess(void)
{
	//�X�N���[�����͏������Ȃ�
	if (!isMoveWheel_)return;

	//�z�C�[������
	int wheelValue = GetMouseWheelRotVol();
	if (wheelValue == 0)return;

	//SE�Đ�
	auto& snd = SoundManager::GetInstance();
	snd.PlaySoundBack(scrollSE_);

	//�摜�̈ړ����������߂�
	wheelSign_ = wheelValue / abs(wheelValue);
	//�z�C�[����������ۏ�Ԃɂ���
	isMoveWheel_ = false;
}

void SelectScene::ScrollSelectImg(int& num, const int& size)
{
	if (isMoveWheel_)return;

	//�f���^�^�C��
	scrollTime_ += SceneManager::GetInstance().GetDeltaTime();

	int movePow = wheelSign_ * SELECT_BOX_SIZE.y; //�ړ���
	//�ړ��̐��`���
	float time = scrollTime_ / SCROLL_SPEED;
	selectImgPos_.y = Utility::Lerp(0, movePow, time);

	if (time >= 1.0f)
	{
		scrollTime_ = 0.0f;
		selectImgPos_.y = 0;
		//�z�C�[������\��Ԃɂ���
		isMoveWheel_ = true;

		//�ԍ��̌���
		num -= wheelSign_;
		num = Utility::Wrap(num, 0, size);
	}
}

void SelectScene::DrawSelectBoxStage(void)
{
	auto& ds = DrawShader::GetInstance();	
	Vector2 pos = SELECT_BOX_POS;
	Vector2 size = SELECT_BOX_SIZE;

	//�I���{�b�N�X(���g)
	ds.DrawExtendGraphToShader(pos, size, selectedBoxImg_);

	//�}�X�N��ʂɔw�i�摜��`��
	SetDrawScreen(selectMaskScreen_);
	ClearDrawScreen();

	//���݃X�e�[�W�摜
	int bgImg = stages_.at(static_cast<SELECT_STAGE>(nowStageNum_)).backImg;
	ds.DrawExtendGraphToShader(selectImgPos_, size, bgImg);

	//�X�N���[����
	if (!isMoveWheel_)
	{
		Vector2 offset = { 0,size.y };
		offset *= wheelSign_ * -1;
		//�X�N���[����̔w�i�摜
		int nextNum = nowStageNum_ - wheelSign_;
		nextNum = Utility::Wrap(
			nextNum, 0, static_cast<int>(SELECT_STAGE::MAX));
		bgImg = stages_.at(static_cast<SELECT_STAGE>(nextNum)).backImg;
		ds.DrawExtendGraphToShader(selectImgPos_ + offset , size, bgImg);
	}

	//�`���w��
	SetDrawScreen(DX_SCREEN_BACK);

	//�؂蔲���摜��`��
	ds.DrawGraphAndSubToShader(
		pos,size,
		selectMaskScreen_, selectedBoxMaskImg_, PsType::MASK, COLOR_F{});
}

void SelectScene::DrawSelectBoxEnemy(void)
{
	auto& ds = DrawShader::GetInstance();
	Vector2 pos = SELECT_BOX_POS;
	Vector2 size = SELECT_BOX_SIZE;

	//�I���{�b�N�X(���g)
	ds.DrawExtendGraphToShader(pos, size, selectedBoxImg_);

	//�}�X�N��ʂɔw�i�摜��`��
	SetDrawScreen(selectMaskScreen_);
	ClearDrawScreen();


	//�o�g���G�l�~�[
	auto& uMng = UnitDataManager::GetInstance();

	StageData& data =
		stages_.at(static_cast<SELECT_STAGE>(nowStageNum_));//���݂̃X�e�[�W���
	auto drawEnemy = [&](int enemysNum ,int unitNum, Vector2 pos) {
		int i = data.enemys[enemysNum].dArray_[unitNum];
		if (i == -1)return;

		//���j�b�g�摜�擾
		int uImg = uMng.GetUnitImg(i);
		ds.DrawExtendGraphToShader(pos, UNIT_SHOW_SIZE,uImg);
	};

	//�G�΃G�l�~�[�摜�̕`��
	drawEnemy(nowEnemysNum_, 2, selectImgPos_ + UNIT_SHOW_RIGHT_POS);
	drawEnemy(nowEnemysNum_, 1, selectImgPos_ + UNIT_SHOW_LEFT_POS);
	drawEnemy(nowEnemysNum_, 0, selectImgPos_ + UNIT_SHOW_CENTER_POS);

	//�X�N���[����
	if (!isMoveWheel_)
	{
		Vector2 offset = { 0,size.y };
		offset *= wheelSign_ * -1;
		//�G�΃G�l�~�[�摜�̕`��
		int nextNum = nowEnemysNum_ - wheelSign_;
		nextNum = Utility::Wrap(nextNum, 0, data.enemys.size());
		drawEnemy(nextNum, 2, selectImgPos_ + offset + UNIT_SHOW_RIGHT_POS);
		drawEnemy(nextNum, 1, selectImgPos_ + offset + UNIT_SHOW_LEFT_POS);
		drawEnemy(nextNum, 0, selectImgPos_ + offset + UNIT_SHOW_CENTER_POS);
	}


	//�`���w��
	SetDrawScreen(DX_SCREEN_BACK);

	//�؂蔲���摜��`��
	ds.DrawGraphAndSubToShader(
		pos, size,
		selectMaskScreen_, selectedBoxMaskImg_, PsType::MASK, COLOR_F{});
}




