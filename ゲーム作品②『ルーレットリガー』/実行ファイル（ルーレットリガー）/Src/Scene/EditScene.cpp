#include <DxLib.h>
#include <cmath>
#include "../Application.h"
#include "../Manager/SceneManager.h"
#include "../Manager/DeckManager.h"
#include "../Manager/DataManager/SoundManager.h"
#include "../Utility/Utility.h"
#include "../Utility/Button/CircleButton.h"
#include "../Utility/Button/RectButton.h"
#include "../Utility/Button/UnitButton.h"
#include "../Utility/Button/CmdButton.h"

#include"../_debug/_DebugConOut.h"
#include"../_debug/_DebugDispOut.h"
#include "EditScene.h"

//�}�C�f�b�L�̕\�����W�A�I�t�Z�b�g�l
const Vector2 FIRST_UNIT_POOL_POS = { 106, 291 };
const Vector2 OFFSET_UNIT_POOL_POS = { 151, 207 };
//�J�[�h�v�[���̕\�����W�A�I�t�Z�b�g�l
const Vector2 FIRST_UNIT_DECK_POS = { 336, 47 };
constexpr int OFFSET_UNIT_DECK_POS = 148;
//�f�b�L�v�[���̍ő�\����
constexpr int DECK_POOL_MAX = 10;
constexpr int DECK_POOL_WIGTH_NUM = 5;	//�������̍ő吔


//�y�[�W�{�^�����W
const Vector2 NEXT_PAUGE_BTN_POS = { 840,440 };		//���y�[�W�{�^���F���W
const Vector2 BACK_PAUGE_BTN_POS = { 60,440 };		//�O�y�[�W�{�^���F���W
const Vector2 PAUGE_BTN_SIZE = { 30, 80 };		//�傫��

//�X�e�[�^�X�p�t�H���g
constexpr int STUTAS_FONT_SIZE = 20;
constexpr int STUTAS_FONT_THICK = 20;
constexpr int STUTAS_FONT_COLOR = 0xe6e6e6;

//�R�}���h�\��
const Vector2 CMD_SHOE_SIZE = { 163,40 };			//�傫��
const Vector2 CMD_SHOE_BASE_POS = { 915, 500 };		//�x�[�X���W
constexpr int CMD_SHOE_NUM = 8;						//�R�}���h��
//�X�e�[�^�X�\��
const Vector2 UNIT_NAME_SHOE_POS = { 1080,80 };			//���j�b�g���\�����W
const Vector2 UNIT_IMAGE_SHOE_POS = { 950,120 };		//���j�b�g�摜�\�����W
const Vector2 STATUS_SHOE_OFFSET = { 140,50 };			//�X�e�[�^�X�I�t�Z�b�g�l
const Vector2 ICON_SHOW_POS = { 960,390 };				//�A�C�R���\�����W
const Vector2 STATUS_TEXT_SHOW_OFFSET = { 40,5 };		//�X�e�[�^�X�l�\���I�t�Z�b�g
constexpr int STATUS_WIDTH_NUM = 2;		//�X�e�[�^�X�����ɕ��ׂ��鐔



EditScene::EditScene(void)
{
	int backImg_=0;
	int nextPB_=0;
	int backPB_=0;
	int unitBackImg_=0;
	int hpIcon_=0;
	int attackIcon_=0;
	int speedIcon_=0;
	int pickUpUnit_=0;
	int sFontHandle_=0;
	int sFontColor_=0;
	int page_=0;
}

EditScene::~EditScene(void)
{
}

void EditScene::Init(void)
{
	//�摜�̓o�^
	//�w�i
	bgImg_ = LoadGraph("Data/Image/bg/EditScene_bg.png");
	//���j�b�g�J�[�h��
	unitBackImg_ = LoadGraph("Data/Image/UI/UnitBack.png");
	//�{�^���摜
	backImg_ = LoadGraph("Data/Image/UI/BackBtn.png");

	//�A�C�R���摜
	hpIcon_ = LoadGraph("Data/Image/Icon/HP.png");
	attackIcon_ = LoadGraph("Data/Image/Icon/Attack.png");
	speedIcon_ = LoadGraph("Data/Image/Icon/Speed.png");

	//�o�b�N�{�^���̐���
	int sy = Application::SCREEN_SIZE_Y;
	backBtn_ = new CircleButton();
	backBtn_->Create({ 0,sy }, backImg_, 0);


	//�O�y�[�W�{�^������
	backPB_ = LoadGraph("Data/Image/UI/Edit/backPageBtn.png");
	pageBtn_[0] = new RectButton();
	pageBtn_[0]->Create(BACK_PAUGE_BTN_POS, PAUGE_BTN_SIZE, backPB_);
	//���y�[�W�{�^������
	nextPB_ = LoadGraph("Data/Image/UI/Edit/nextPageBtn.png");
	pageBtn_[1] = new RectButton();
	pageBtn_[1]->Create(NEXT_PAUGE_BTN_POS, PAUGE_BTN_SIZE, nextPB_);


	//�f�b�L���j�b�g�A�������j�b�g�̏�����
	InitMyUnit();
	//�X�e�[�^�X�R�}���h�{�^������
	CreateCmd();

	//�X�e�[�^�X��ʃt�H���g
	sFontHandle_= CreateFontToHandle("������", STUTAS_FONT_SIZE, STUTAS_FONT_THICK,
		DX_FONTTYPE_ANTIALIASING_4X4);
	sFontColor_ = STUTAS_FONT_COLOR;

	//BGM�Đ�
	auto& snd = SoundManager::GetInstance();
	bgmHandle_ = snd.LoadSound(SoundManager::SOUND_TYPE::EDIT_BGM);
	snd.PlaySoundBGM(bgmHandle_);

	
	//�s�b�N�A�b�v���j�b�g
	pickUpUnit_ = -1;

	//�y�[�W
	page_ = 1;
}

void EditScene::Update(void)
{
	//�o�b�N����
	if (backBtn_->ButtonDecision())
	{
		//�f�b�L�̊m��
		DeckDecision();
		//�V�[���J��
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::SELECT, true);
		return;
	}


	//�{�^���X�V
	//�o�b�N�{�^��
	backBtn_->Update();

	//�y�[�W�{�^��
	PageBtnProcess();

	//�f�b�L
	for (auto& deck : myDeckBtn_)
	{
		deck.second->Update();
	}
	//���j�b�g�J�[�h�v�[��
	for (auto& card : unitCards_)
	{
		card.second->Update();
	}

	//�Z�ꗗ
	for (auto& cmdBtn : cmdBtns_)
	{
		cmdBtn->Update();
	}


	//�f�b�L�ҏW����
	DeckEditProcess();
	//���������j�b�g�J�[�h���s�b�N�A�b�v�ɃZ�b�g
	SelectPickUpUnit();
}

void EditScene::Draw(void)
{
	//�w�i
	DrawGraph(0, 0, bgImg_, true);


	//�{�^��UI
	//�}�C�f�b�L
	for (auto& deck : myDeckBtn_)
	{
		deck.second->Draw();
	}
	//���j�b�g�v�[��
	for (auto& card : unitCards_)
	{
		card.second->Draw();
	}

	//�o�b�N�{�^��
	backBtn_->Draw();

	//�y�[�W�{�^��
	for (auto& pb : pageBtn_)
	{
		pb->Draw();
	}

	//���j�b�g���
	DrawUnitStatus();

#ifdef DEBUG
	auto cx = Application::SCREEN_SIZE_X;
	auto cy = Application::SCREEN_SIZE_Y;
	auto span = 20;
	for (size_t i = 0; i < 100; i++)
	{
		//X
		DrawLine(0, i * span, cx, i * span, 0xff0000);

		//Y
		DrawLine(i * span, 0, i * span, cy, 0x0000ff);
	}
#endif // _DEBUG

}

void EditScene::Release(void)
{
	//�摜�̉��
	DeleteGraph(bgImg_);
	DeleteGraph(unitBackImg_);
	DeleteGraph(backImg_);

	DeleteGraph(hpIcon_);
	DeleteGraph(attackIcon_);
	DeleteGraph(speedIcon_);

	//�{�^���N���X�J��
	backBtn_->Release();
	delete backBtn_;

	//�T�E���h�̏I��
	StopSoundMem(bgmHandle_);


	for (auto& du : myDeckBtn_)
	{
		du.second->Release();
		delete du.second;
	}
	myDeckBtn_.clear();

	for (auto& cu : unitCards_)
	{
		cu.second->Release();
		delete cu.second;
	}
	unitCards_.clear();

	for (auto& cb : cmdBtns_)
	{
		cb->Release();
		delete cb;
	}
	cmdBtns_.clear();
}

void EditScene::InitMyUnit(void)
{
	//���j�b�g���
	auto& deckMng = DeckManager::GetInstance();
	auto& dataMng = UnitDataManager::GetInstance();

	//�f�b�L���j�b�g�{�^���̐���
	int noDeck = 0;
	for (auto& num : deckMng.GetDeck().dArray_)
	{
		//���j�b�g�f�[�^
		auto u = dataMng.GetUnitData(num);
		//�\�����W
		Vector2 cardPos = FIRST_UNIT_DECK_POS;
		cardPos.x += OFFSET_UNIT_DECK_POS * noDeck;

		//�{�^������
		UnitButton* ub = new UnitButton();
		ub->Create(cardPos, unitBackImg_, u);
		myDeckBtn_.emplace(std::make_pair(num, ub));

		noDeck++;
	}

	//�������j�b�g�ԍ��擾
	unitCardNums_ = deckMng.GetUnitPool();
	//�������j�b�g�{�^���̐���
	int no = 0;
	for (auto& num : unitCardNums_)
	{
		//�������̏���m�F
		if (no >= DECK_POOL_MAX)break;


		//���j�b�g�f�[�^
		auto u = dataMng.GetUnitData(num);
		//�\�����W
		Vector2 cardPos = FIRST_UNIT_POOL_POS;
		if (no < DECK_POOL_WIGTH_NUM)
		{
			cardPos.x += OFFSET_UNIT_POOL_POS.x * no;
		}
		else
		{
			cardPos.x += OFFSET_UNIT_POOL_POS.x * (no - DECK_POOL_WIGTH_NUM);
			cardPos.y += OFFSET_UNIT_POOL_POS.y;
		}
		//�{�^������
		UnitButton* ub = new UnitButton();
		ub->Create(cardPos, unitBackImg_, u);
		unitCards_.emplace(std::make_pair(num, ub));

		no++;
	}

}

void EditScene::CreateCmd(void)
{
	//�R�}���h�{�^���̐���
	int noX, noY;
	noX = noY = 0;
	Vector2 size = CMD_SHOE_SIZE;
	Vector2 posBase = CMD_SHOE_BASE_POS;
	for (int i = 0; i < CMD_SHOE_NUM;i++)
	{
		CmdButton* b = new CmdButton();

		//���W����
		Vector2 pos = { 
			posBase.x + size.x * noX, 
			posBase.y + size.y * noY };
		b->Create(pos, size, 0);
		cmdBtns_.push_back(b);

		//���W���炵�B�R�}���h���̔������\������
		noY++;
		if (noY >= CMD_SHOE_NUM/2)
		{
			noY = 0;
			noX++;
		}
	}
}

void EditScene::DrawUnitStatus(void)
{
	if (pickUpUnit_ == -1)return;


	//������́i�����́j�������擾
	auto halfLen = Utility::CalcStringLength(pickUpUnitData_.name, sFontHandle_) / 2;
	//���O
	DrawStringToHandle(
		UNIT_NAME_SHOE_POS.x - halfLen, UNIT_NAME_SHOE_POS.y,
		pickUpUnitData_.name.c_str(), sFontColor_, sFontHandle_);

	//���j�b�g�摜
	DrawGraph(UNIT_IMAGE_SHOE_POS.x, UNIT_IMAGE_SHOE_POS.y, pickUpUnitData_.imgHandle, true);

	//�X�e�[�^�X
	int noX = 0;
	int noY = 0;
	int offsetX = STATUS_SHOE_OFFSET.x;
	int offsetY = STATUS_SHOE_OFFSET.y;

	auto drawIcon = [&](int icon,int value) {

		Vector2 iconPos = { ICON_SHOW_POS.x + (offsetX * noX), ICON_SHOW_POS.y + (offsetY * noY) };

		DrawGraph(iconPos.x, iconPos.y, icon, true);
		DrawFormatStringToHandle(
			iconPos.x + STATUS_TEXT_SHOW_OFFSET.x,
			iconPos.y + STATUS_TEXT_SHOW_OFFSET.y,
			sFontColor_, sFontHandle_, "%d", value);

		//��2��ɕ��ׂ�
		noX++;
		if (noX >= STATUS_WIDTH_NUM)
		{
			noX = 0;
			noY++;
		}

		return;
	};

	drawIcon(hpIcon_, pickUpUnitData_.hp);
	drawIcon(attackIcon_, pickUpUnitData_.attack);
	drawIcon(speedIcon_, pickUpUnitData_.speed);

	//�Z�ꗗ
	for (auto& cmdBtn : cmdBtns_)
	{
		cmdBtn->Draw();
	}
	//�Z�ڍ׈ꗗ
	for (auto& cmdBtn : cmdBtns_)
	{
		cmdBtn->DrawCmdDetail();
	}
}

void EditScene::DeckEditProcess(void)
{
	if (pickUpUnit_ == -1)return;

	//�s�b�N�A�b�v���j�b�g���Z�b�g����
	for (auto& deck : myDeckBtn_)
	{
		if (!deck.second->ButtonDecision())continue;

		//�f�b�L�ɓ������j�b�g�����݂�����A�X�L�b�v
		auto u = myDeckBtn_.find(pickUpUnit_);
		if (u != myDeckBtn_.end())return;


		//�Â��L�[�̒l���擾
		UnitButton* c = myDeckBtn_[deck.first];
		//�폜
		myDeckBtn_.erase(deck.first);

		//�{�^���̃f�[�^�X�V
		c->ChengeUnitData(pickUpUnitData_);

		//�V�����L�[�ŗv�f��ǉ�
		myDeckBtn_[pickUpUnit_] = c;

		return;
	}
}

void EditScene::SelectPickUpUnit(void)
{
	for (auto& card : unitCards_)
	{
		//������Ă��Ȃ��ꍇ�A���������Ȃ�
		if (!card.second->ButtonDecision())continue;
		
		//�ԍ����Z�b�g
		pickUpUnit_ = card.first;

		//���j�b�g�f�[�^�̎擾
		auto& dataMng = UnitDataManager::GetInstance();
		pickUpUnitData_ = dataMng.GetUnitData(pickUpUnit_);
		//�R�}���h�̃Z�b�g
		int n = 0;
		for (auto& cmd : pickUpUnitData_.cmdNum)
		{
			cmdBtns_[n]->SetCmd(cmd);
			n++;
		}
	}
}

void EditScene::DeckDecision(void)
{
	auto& dMng = DeckManager::GetInstance();
	std::vector<int>  num;
	for (auto& deck : myDeckBtn_)
	{
		num.push_back(deck.first);
	}
	dMng.SetDeck({ num[0],num[1],num[2] });
}

void EditScene::PageBtnProcess(void)
{
	//�X�V
	//�y�[�W�{�^��
	for (auto& pb : pageBtn_)
	{
		pb->Update();
	}

	//�������j�b�g��
	int uNum = unitCardNums_.size();
	//�ő�y�[�W��
	int maxPage = ceil(static_cast<float>(uNum) / static_cast<float>(DECK_POOL_MAX));

	//�O�{�^���N���b�N����
	if (pageBtn_[0]->ButtonDecision())
	{
		page_--;
		page_=Utility::Wrap(page_, 1, maxPage + 1);
		ChangeDeckPoolPage();
		return;
	}

	//���{�^���N���b�N����
	if (pageBtn_[1]->ButtonDecision())
	{
		page_++;
		page_=Utility::Wrap(page_, 1, maxPage + 1);
		ChangeDeckPoolPage();
		return;
	}

}

void EditScene::ChangeDeckPoolPage(void)
{
	auto& uDataMng = UnitDataManager::GetInstance();

	//���ꕨ
	std::unordered_map<int, UnitButton*> unitCards;

	int number = 0;
	int count = 0;
	for (auto& uCardNum : unitCardNums_)
	{
		//
		if (number >= unitCardNums_.size())break;

		//�ȏ�̏ꍇ�A�����I��
		if (number >= page_ * DECK_POOL_MAX)break;

		//����
		int n = number - ((page_ - 1) * DECK_POOL_MAX);
		if (0 > n)
		{ 
			number++;
			continue;
		}

		//���j�b�g�f�[�^
		auto uData = uDataMng.GetUnitData(uCardNum);
		//�\�����W
		Vector2 cardPos = FIRST_UNIT_POOL_POS;
		//���ɕ��ׂ�J�[�h�̐�
		if (count < DECK_POOL_WIGTH_NUM)
		{
			cardPos.x += OFFSET_UNIT_POOL_POS.x * count;
		}
		else
		{
			cardPos.x += OFFSET_UNIT_POOL_POS.x * (count - DECK_POOL_WIGTH_NUM);
			cardPos.y += OFFSET_UNIT_POOL_POS.y;
		}
		//�V�����y�[�W�̃{�^������
		UnitButton* ub = new UnitButton();
		ub->Create(cardPos, unitBackImg_, uData);
		unitCards.emplace(std::make_pair(uCardNum, ub));

		number++;
		count++;
	}
	
	//�O�y�[�W�̃{�^���̉������
	for (auto& btn : unitCards_) {
		btn.second->Release();
	}
	//�z��̍폜
	unitCards_.clear();

	//�z��̑��
	unitCards_ = unitCards;
}
