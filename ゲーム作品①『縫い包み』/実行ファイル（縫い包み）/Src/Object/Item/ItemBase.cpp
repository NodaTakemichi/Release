#include "../../Manager/ResourceManager.h"
#include "../../Utility/Utility.h"
#include "../Stage/Stage.h"
#include "ItemBase.h"

//�ʂ�����ݏċp
constexpr VECTOR BURN_DOOL_LOCAL_POS = { -10.0f,30.0f,0.0f };	//���[�J�����W
constexpr float BURN_DOOL_ANGLE_Y = 90.0f * (DX_PI_F / 180.0f);	//�p�x

ItemBase::ItemBase(void)
{
	type_ = ITEM_TYPE::NONE;
	hasItem_ = false;
	isAlive_ = true;
	isBurnDirection_ = false;
}

ItemBase::~ItemBase(void)
{
}

const void ItemBase::SetModelInfo(const ITEM_TYPE& type, const VECTOR& scl, const VECTOR& pos, const VECTOR& rot)
{
	type_ = type;
	transform_.SetModel(SelectModelHandle());

	//3D��{���̏�����
	transform_.Init(
		scl,
		pos,
		Quaternion::Euler({
			Utility::Deg2RadF(rot.x),
			Utility::Deg2RadF(rot.y),
			Utility::Deg2RadF(rot.z) 
		}),
		Quaternion(),
		Collider::TYPE::ITEM
	);

	//�擾����
	hasItem_ = false;
	//��������
	isAlive_ = true;

	//���[�h����
	LoadData();

	//������
	Init();
}

int ItemBase::SelectModelHandle(void)
{
	int modelHandle = -1;

	switch (type_) {
	case ITEM_TYPE::AMULET:
		modelHandle= resMng_.LoadModelDuplicate(
			ResourceManager::SRC::AMULET_MODEL);
		break;
	case ITEM_TYPE::MONKEY:
		modelHandle= resMng_.LoadModelDuplicate(
			ResourceManager::SRC::DOOL_MONKEY);
		break;
	case ITEM_TYPE::PIG:
		modelHandle= resMng_.LoadModelDuplicate(
			ResourceManager::SRC::DOOL_PIG);
		break;
	case ITEM_TYPE::RABBIT:
		modelHandle= resMng_.LoadModelDuplicate(
			ResourceManager::SRC::DOOL_RABBIT);
		break;
	case ITEM_TYPE::SHEEP:
		modelHandle= resMng_.LoadModelDuplicate(
			ResourceManager::SRC::DOOL_SHEEP);
		break;
	}

	return modelHandle;
}

void ItemBase::StartBurnDirection(void)
{
	//���W�A�p�x�̃Z�b�g
	transform_.pos_ = VAdd(Stage::FIREPLACE_POS, BURN_DOOL_LOCAL_POS);
	transform_.quaRot_ = Quaternion::Euler(VECTOR{0.0f,BURN_DOOL_ANGLE_Y,0.0f});
	transform_.Update();

	//�Ď����o�J�n
	isBurnDirection_ = true;

	//�A�C�e���̎��S�ݒ�
	SetIsAlive(false);
}
