#include <DxLib.h>
#include "../Manager/ResourceManager.h"
#include "OperationUI.h"

//������@�̉摜���S�ʒu
constexpr float OPERATION_DRAWING_CENTER = 0.5f;
//������@�̕`��T�C�Y
constexpr float OPERATION_DRAWING_SIZE = 140.0f;
//������@�̕`����WY
constexpr float OPERATION_DRAWING_POS_Y = 490.0f;

//������@�̕`����W
//�ړ�����
constexpr VECTOR OPERATION_MOVE_DRAWING_POS = { -230.0f, OPERATION_DRAWING_POS_Y, -2500.0f };
//�_�b�V������
constexpr VECTOR OPERATION_DASH_DRAWING_POS = { -230.0f, OPERATION_DRAWING_POS_Y, -2300.0f };
//�|�[�Y����
constexpr VECTOR OPERATION_PAUSE_DRAWING_POS = { -230.0f, OPERATION_DRAWING_POS_Y, -2100.0f };
//�u���ׂ�v����
constexpr VECTOR OPERATION_CHECK_DRAWING_POS = { 220.0f, OPERATION_DRAWING_POS_Y, -2300.0f };
//�u�아�̎g�p�v����
constexpr VECTOR OPERATION_USE_DRAWING_POS = { 220.0f, OPERATION_DRAWING_POS_Y, -2100.0f };

OperationUI::OperationUI(void)
{
	opeMoveImg_ = 0;
	opeDashImg_ = 0;
	opeLeftClickImg_ = 0;
	opeRightClickImg_ = 0;
	opePauseImg_ = 0;
}

OperationUI::~OperationUI(void)
{
}

void OperationUI::LoadData(void)
{
	auto& res = ResourceManager::GetInstance();
	opeMoveImg_ = res.Load(ResourceManager::SRC::OPERATION__MOVE_IMG).handleId_;
	opeDashImg_ = res.Load(ResourceManager::SRC::OPERATION__DASH_IMG).handleId_;
	opeLeftClickImg_ = res.Load(ResourceManager::SRC::OPERATION__LEFT_CLICK_IMG).handleId_;
	opeRightClickImg_ = res.Load(ResourceManager::SRC::OPERATION__RIGHT_CLICK_IMG).handleId_;
	opePauseImg_ = res.Load(ResourceManager::SRC::OPERATION__PAUSE_IMG).handleId_;

}

void OperationUI::Init(void)
{
}

void OperationUI::Update(void)
{
}

void OperationUI::Draw(void)
{
	const float cx = OPERATION_DRAWING_CENTER, cy = OPERATION_DRAWING_CENTER;
	const float size = OPERATION_DRAWING_SIZE;
	const float angle = 0.0f;

	DrawBillboard3D(
		OPERATION_MOVE_DRAWING_POS,
		cx, cy, size, angle,
		opeMoveImg_, true);
	DrawBillboard3D(
		OPERATION_DASH_DRAWING_POS,
		cx, cy, size, angle,
		opeDashImg_, true);
	DrawBillboard3D(
		OPERATION_PAUSE_DRAWING_POS,
		cx, cy, size, angle,
		opePauseImg_, true);

	DrawBillboard3D(
		OPERATION_CHECK_DRAWING_POS,
		cx, cy, size, angle,
		opeLeftClickImg_, true);
	DrawBillboard3D(
		OPERATION_USE_DRAWING_POS,
		cx, cy, size, angle,
		opeRightClickImg_, true);
}
