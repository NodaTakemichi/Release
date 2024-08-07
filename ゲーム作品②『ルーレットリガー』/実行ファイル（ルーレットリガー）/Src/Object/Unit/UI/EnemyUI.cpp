#include "EnemyUI.h"
//HP�Q�[�W�J���[(��)
constexpr COLOR_F HP_GAUGE_COLOR = { 0.8f,0.4f,0.4f,1.0f };
//HP�Q�[�W���[�J�����W
const Vector2 HP_GAUGE_LOCAL_POS = Vector2{ 50,20 };
//�^�[�Q�b�g�摜�̃��[�J�����W
const Vector2 TARGET_IMG_LOCAL_POS = Vector2{ -100,UnitBase::DRAWING_SIZE / 4 };


EnemyUI::EnemyUI(
	Vector2 pos, std::string& name, int& hp, int& nowHp, int& maxHp, bool& active) :
	UnitUI(pos, name, hp, nowHp, maxHp,active)
{
}

EnemyUI::~EnemyUI()
{
}

void EnemyUI::Init(void)
{
	//������
	UnitUI::Init();
}

void EnemyUI::Draw(void)
{
	//���O�`��
	DrawName(name_, unitPos_, HP_GAUGE_COLOR);

	//HP��0�ȉ��̎��AHP�Q�[�W��`�悵�Ȃ�
	if (nowHp_ <= 0)return;

	//�o�t�A�C�R���̕`��
	DrawBuffIcon();

	//���j�b�g�̃T�C�Y
	auto unitSize = UnitBase::DRAWING_SIZE;
	//HP�g�̕\��
	Vector2 pos = unitPos_ + HP_GAUGE_LOCAL_POS;
	pos.x += unitSize;
	DrawHpFrame(pos);

	//HP�V�F�[�_�[
	DrawHpShader(pos, HP_GAUGE_COLOR);

	//�`��
	UnitUI::Draw();
}

void EnemyUI::DrawRockOn(void)
{

	Vector2 pos = unitPos_ + TARGET_IMG_LOCAL_POS;
	DrawGraph(pos.x, pos.y, targetImg_, true);
}
