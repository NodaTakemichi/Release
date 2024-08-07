#pragma once
class Collider
{

public :

	// 衝突種別
	enum class TYPE
	{
		NONE,
		STAGE,
		GOAL_DOOR,
		DIS_WALL,
		ITEM,
		FACILITY,
		ENEMY
	}; 

	// コンストラクタ
	Collider(TYPE type, int modelId);

	// デストラクタ
	~Collider(void);

	// 衝突種別
	TYPE type_;

	// モデルのハンドルID
	int modelId_;

};
