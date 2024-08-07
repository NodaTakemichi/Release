#include "../../Manager/ResourceManager.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/Camera.h"
#include "../../Utility/Utility.h"
#include "../Common/Transform.h"
#include "GoalDoor.h"

//�S�[���h�A�傫��
constexpr VECTOR GOAL_DOOR_SIZE = { 2.0f,3.0f,0.5f };
//�S�[���h�A���W
constexpr VECTOR GOAL_DOOR_POS = { -7.0f, 450.0f, -3550.0f };

GoalDoor::GoalDoor(void)
{
}

GoalDoor::~GoalDoor(void)
{
}

void GoalDoor::LoadData(void)
{
	transform_.SetModel(
		resMng_.LoadModelDuplicate(ResourceManager::SRC::GOOL_DOOR_MODEL));
}

void GoalDoor::Init(void)
{
	//3D��{���̏�����
	transform_.Init(
		GOAL_DOOR_SIZE,
		GOAL_DOOR_POS,
		Quaternion(),
		Quaternion(),
		Collider::TYPE::GOAL_DOOR
	);

}

void GoalDoor::Update(void)
{

}

void GoalDoor::Draw(void)
{
	MV1DrawModel(transform_.modelId_);

}
