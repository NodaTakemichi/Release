#include "../../Manager/SceneManager.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/SoundManager.h"
#include "../../Utility/Utility.h"
#include "../Common/AnimationController.h"
#include "../Common/Capsule.h"
#include "../../Sound/SoundController.h"
#include "Player.h"
#include "Tracker.h"

// 追跡者の衝突判定の半径
constexpr float RADIOUS = 50.0f;
// 追跡者の目の高さ
constexpr float EYE_LEVEL = 160.0f;
// 追跡者のカプセルコライダー座標（下）
constexpr VECTOR CAPSULE_LOCAL_POS_DOWN = { 0, RADIOUS,0 }; //下
constexpr VECTOR CAPSULE_LOCAL_POS_UP = { 0,EYE_LEVEL,0 };  //上
// 移動スピード
constexpr float SPEED_WALK = 4.35f; //歩き
constexpr float SPEED_RUN = 9.0f;   //走り
//方向転換スピード
constexpr float TROCKER_CHANGE_DIR_SPEED = 0.05f;

//視野
constexpr float VIEW_RANGE = 1000.0f;   //視野の距離
constexpr float VIEW_ANGLE = 50.0f;     //視野角
constexpr float VIEW_ANGLE_RAD = (VIEW_ANGLE * (DX_PI_F / 180.0f));  //視野角

//プレイヤーを見失う距離
constexpr float DIS_LOSE_SIGHT_OF_PLAYER = 900.0f;

//方向転換完了時間
constexpr float ROTATION_COMPLETED_TIME = 2.5f;
//気絶時間
constexpr float FAINT_TIME = 3.0f;

//アニメーション再生速度
constexpr float ANIM_PLAY_SPEED_BASE = 30.0f;   //基本
constexpr float ANIM_PLAY_SPEED_WALK = 33.0f;    //歩く
constexpr float ANIM_PLAY_SPEED_CHACH = 14.0f;  //捕まえる

//追跡者
constexpr VECTOR TRACKER_SCL = { 1.5f,1.5f,1.5f };           //大きさ
constexpr VECTOR TRACKER_POS = { -870.0f, 360.0f, 900.0f };  //座標
constexpr VECTOR TRACKER_LOCAL_ROT = { 0.0f,180.0f * (DX_PI_F / 180.0f),0.0f };  //ローカル角度

//足音
constexpr int FOOTSTEPS_SE_VOLUME = 200;		//音量
constexpr float FOOTSTEPS_SE_HEARING_AREA = 1000.0f;	//音が届く距離

//移動ポイントの衝突判定の半径
constexpr float MOVE_POINT_R = 20.0f;
//移動ポイント座標(追跡者が徘徊するときに使う座標)
#pragma region 移動ポイント座標
constexpr float FRONT_MOVE_POINT_Y = 330.0f;	    //一階のY座標
constexpr float BASEMENT_MOVE_POINT_Y = -155.0f;	//地下のY座標
//一階
constexpr VECTOR MOVE_POINT_POS_1 = { 5.0f,FRONT_MOVE_POINT_Y,-710.0f };
constexpr VECTOR MOVE_POINT_POS_2 = { 5.0f,FRONT_MOVE_POINT_Y,-1700.0f };
constexpr VECTOR MOVE_POINT_POS_3 = { 5.0f,FRONT_MOVE_POINT_Y,-2570.0f };
//大部屋
constexpr VECTOR MOVE_POINT_POS_4 = { -590.0f,FRONT_MOVE_POINT_Y,-300.0f };
constexpr VECTOR MOVE_POINT_POS_5 = { -590.0f,FRONT_MOVE_POINT_Y,-200.0f };
constexpr VECTOR MOVE_POINT_POS_6 = { -590.0f,FRONT_MOVE_POINT_Y,150.0f };
constexpr VECTOR MOVE_POINT_POS_7 = { 550.0f,FRONT_MOVE_POINT_Y,-300.0f };
constexpr VECTOR MOVE_POINT_POS_8 = { 550.0f,FRONT_MOVE_POINT_Y,5.0f };
constexpr VECTOR MOVE_POINT_POS_9 = { 550.0f,FRONT_MOVE_POINT_Y,150.0f };
//左廊下
constexpr VECTOR MOVE_POINT_POS_10 = { -910.0f,FRONT_MOVE_POINT_Y,1600.0f };
constexpr VECTOR MOVE_POINT_POS_11 = { -910.0f,FRONT_MOVE_POINT_Y,900.0f };
constexpr VECTOR MOVE_POINT_POS_12 = { -910.0f,FRONT_MOVE_POINT_Y,10.0f };
constexpr VECTOR MOVE_POINT_POS_13 = { -910.0f,FRONT_MOVE_POINT_Y,-200.0f };
constexpr VECTOR MOVE_POINT_POS_14 = { -910.0f,FRONT_MOVE_POINT_Y,-710.0f };
constexpr VECTOR MOVE_POINT_POS_15 = { -910.0f,FRONT_MOVE_POINT_Y,-1200.0f };
constexpr VECTOR MOVE_POINT_POS_16 = { -910.0f,FRONT_MOVE_POINT_Y,-1700.0f };
//右廊下
constexpr VECTOR MOVE_POINT_POS_17 = { 910.0f,FRONT_MOVE_POINT_Y,1600.0f };
constexpr VECTOR MOVE_POINT_POS_18 = { 910.0f,FRONT_MOVE_POINT_Y,900.0f };
constexpr VECTOR MOVE_POINT_POS_19 = { 910.0f,FRONT_MOVE_POINT_Y,5.0f };
constexpr VECTOR MOVE_POINT_POS_20 = { 910.0f,FRONT_MOVE_POINT_Y,-200.0f };
constexpr VECTOR MOVE_POINT_POS_21 = { 910.0f,FRONT_MOVE_POINT_Y,-710.0f };
constexpr VECTOR MOVE_POINT_POS_22 = { 910.0f,FRONT_MOVE_POINT_Y,-1700.0f };
//小部屋
constexpr VECTOR MOVE_POINT_POS_23 = { -500.0f,FRONT_MOVE_POINT_Y,-1200.0f };		//手前の小部屋
constexpr VECTOR MOVE_POINT_POS_24 = { -500.0f,FRONT_MOVE_POINT_Y,1600.0f };		//奥の小部屋（右）

//地下
constexpr VECTOR MOVE_POINT_POS_25 = { -40.0f,BASEMENT_MOVE_POINT_Y,1000.0f };
constexpr VECTOR MOVE_POINT_POS_26 = { 270.0f,BASEMENT_MOVE_POINT_Y,1000.0f };

constexpr VECTOR MOVE_POINT_POS_27 = { -40.0f,BASEMENT_MOVE_POINT_Y,200.0f };
constexpr VECTOR MOVE_POINT_POS_28 = { 270.0f,BASEMENT_MOVE_POINT_Y,200.0f };
constexpr VECTOR MOVE_POINT_POS_29 = { -300.0f,BASEMENT_MOVE_POINT_Y,30.0f };
constexpr VECTOR MOVE_POINT_POS_30 = { 270.0f,BASEMENT_MOVE_POINT_Y,-200.0f };

constexpr VECTOR MOVE_POINT_POS_31 = { -40.0f,BASEMENT_MOVE_POINT_Y,-700.0f };
constexpr VECTOR MOVE_POINT_POS_32 = { -245.0f,BASEMENT_MOVE_POINT_Y,-700.0f };

//階段：左
constexpr VECTOR MOVE_POINT_POS_33 = { -1300.0f,BASEMENT_MOVE_POINT_Y,30.0f };
constexpr VECTOR MOVE_POINT_POS_34 = { -1300.0f,175.0f,-690.0f };
constexpr VECTOR MOVE_POINT_POS_35 = { -1500.0f,155.0f,-690.0f };
constexpr VECTOR MOVE_POINT_POS_36 = { -1500.0f,FRONT_MOVE_POINT_Y + 20.0f,10.0f };
//階段：右
constexpr VECTOR MOVE_POINT_POS_37 = { 1300.0f,BASEMENT_MOVE_POINT_Y,-200.0f };
constexpr VECTOR MOVE_POINT_POS_38 = { 1300.0f,175.0f,500.0f };
constexpr VECTOR MOVE_POINT_POS_39 = { 1500.0f,155.0f,500.0f };
constexpr VECTOR MOVE_POINT_POS_40 = { 1500.0f,FRONT_MOVE_POINT_Y + 20.0f,-200.0f };

//ワープポイント
constexpr VECTOR MOVE_POINT_POS_41 = { 910.0f,FRONT_MOVE_POINT_Y,2600.0f };
constexpr VECTOR MOVE_POINT_POS_42 = { -910.0f,FRONT_MOVE_POINT_Y,2600.0f };
constexpr VECTOR MOVE_POINT_POS_43 = { 910.0f,FRONT_MOVE_POINT_Y,-2700.0f };
constexpr VECTOR MOVE_POINT_POS_44 = { -910.0f,FRONT_MOVE_POINT_Y,-2700.0f };
constexpr VECTOR MOVE_POINT_POS_45 = { -910.0f,BASEMENT_MOVE_POINT_Y,1000.0f };
constexpr VECTOR MOVE_POINT_POS_46 = { 940.0f,BASEMENT_MOVE_POINT_Y,200.0f };


#pragma endregion


Tracker::Tracker(const Player& player):player_(player)
{
	//更新する関数のセット
	updateFunc_ = std::bind(& Tracker::UpdateNormal,this);
}

Tracker::~Tracker(void)
{
}

void Tracker::LoadData(void)
{
	//モデル登録
	transform_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::TRACKER_MODEL));
	//アニメ登録
	std::string path = "Data/Model/Tracker/";
	animController_ = std::make_unique<AnimationController>(transform_.modelId_);
	animController_->Add((int)ANIM_TYPE::RUN, path + "Tracker_Anim_Walk.mv1", ANIM_PLAY_SPEED_WALK);
	animController_->Add((int)ANIM_TYPE::FAST_RUN, path + "Tracker_Anim_Tracking.mv1", ANIM_PLAY_SPEED_BASE);
	animController_->Add((int)ANIM_TYPE::STUN, path + "Tracker_Anim_Stun.mv1", ANIM_PLAY_SPEED_BASE);
	animController_->Add((int)ANIM_TYPE::CHACH, path + "Tracker_Anim_Attack.mv1", ANIM_PLAY_SPEED_CHACH);
	animController_->Add((int)ANIM_TYPE::LOST, path + "Tracker_Anim_Looking.mv1", ANIM_PLAY_SPEED_BASE);

	//足音サウンドの作成
	footstepsSound_ = std::make_shared<SoundController>();
	footstepsSound_->DataLoad(ResourceManager::SRC::FOOTSTEPS_TRACKER_SND);
}

void Tracker::Init(void)
{
	//3D基本情報の初期化
	transform_.Init(
		TRACKER_SCL,
		TRACKER_POS,
		Quaternion(),
		Quaternion::Euler(TRACKER_LOCAL_ROT),
		Collider::TYPE::ENEMY
	);

	//カプセルコライダーの作成
	capsule_ = std::make_unique<Capsule>(transform_);
	capsule_->SetRadius(RADIOUS);
	capsule_->SetLocalPosDown(CAPSULE_LOCAL_POS_DOWN);
	capsule_->SetLocalPosTop(CAPSULE_LOCAL_POS_UP);

	//衝突コライダーのクリア
	ClearStageCollider();

	//移動可能ポイントの初期化
	InitMovePoint();

	//初期アニメーション
	animController_->Play((int)ANIM_TYPE::RUN);

	//気絶状態
	isFainting_ = false;

	//足音の初期化
	footstepsSound_->Init(
		FOOTSTEPS_SE_VOLUME, FOOTSTEPS_SE_HEARING_AREA,
		transform_.pos_, SoundController::REVERB_TYPE::PADDEDCELL);
}

void Tracker::Update(void)
{
	//現在の更新関数の呼び出し
	updateFunc_();

	//音源位置の設定
	footstepsSound_->SetSoundPos(transform_.pos_);

	//3D情報の更新
	transform_.Update();
	// アニメーション再生
	animController_->Update();
}

void Tracker::Draw(void)
{
	//モデルの描画
	MV1DrawModel(transform_.modelId_);


#ifdef _DEBUG
	//デバック用
	capsule_->Draw();

		//座標
		DrawFormatString(900, 10, 0xffffff,
			L"追跡者：座標（%.2f,%.2f,%.2f)",
			transform_.pos_.x,
			transform_.pos_.y,
			transform_.pos_.z);

		//移動ポイント
		int sphereNum = 0;
		int count = 0;
		for (const auto& point : movePoints_)
		{
			count++;
			//DrawFormatString(10, 50+count*16, 0xffffff,
			//	L"ゴール回数：%d", point.goledNum);

			int color = 0xff0000;
			if (point.warpPoint)color = 0xccaa00;
			if (currentMovePointNum_ == sphereNum)color = 0x00aaff;
			DrawSphere3D(point.pos, MOVE_POINT_R, 10, color, color, false);
			sphereNum++;
		}
		DrawFormatString(900,26,0xffffff,
			L"移動可能ポイントの数：%d", moveablePointNums_.size());
		DrawFormatString(900, 42, 0xffffff,
			L"移動ポイント（%.2f,%.2f,%.2f)",
			movePoints_[currentMovePointNum_].pos.x,
			movePoints_[currentMovePointNum_].pos.y,
			movePoints_[currentMovePointNum_].pos.z);

		//発見エリア
		DrawDiscoveryArea();

#endif // _DEBUG
}

void Tracker::SetGameOverInfo(const VECTOR& opponentPos)
{
	//ゴール方向のクォータニオンをセット
	goalQuaRot_ = CalcQuaRad(opponentPos, transform_.pos_);
	//回転時間の初期化
	rotateTime_ = 0.0f;

	//アニメーションの変更
	animController_->Play((int)ANIM_TYPE::CHACH, true, 0.0f, -1.0f, false, false, 0.0f);

	//ゲームオーバー状態に変更
	updateFunc_ = std::bind(&Tracker::UpdateGameOver, this);

}

void Tracker::InitMovePoint(void)
{
	//現在の移動ポイント
	currentMovePointNum_ = 0;

	//移動ポイント座標のセット
	currentMovePointNum_ = 0;
	auto makePoint = [&](VECTOR pos, bool warp = false)
	{
		movePoints_.push_back(MovePoint{ 0,pos ,warp });
	};

	//一階
	makePoint(MOVE_POINT_POS_1);
	makePoint(MOVE_POINT_POS_2);
	makePoint(MOVE_POINT_POS_3);
	//大部屋
	makePoint(MOVE_POINT_POS_4);
	makePoint(MOVE_POINT_POS_5);
	makePoint(MOVE_POINT_POS_6);
	makePoint(MOVE_POINT_POS_7);
	makePoint(MOVE_POINT_POS_8);
	makePoint(MOVE_POINT_POS_9);
	//左廊下
	makePoint(MOVE_POINT_POS_10);
	makePoint(MOVE_POINT_POS_11);
	makePoint(MOVE_POINT_POS_1);
	makePoint(MOVE_POINT_POS_13);
	makePoint(MOVE_POINT_POS_14);
	makePoint(MOVE_POINT_POS_15);
	makePoint(MOVE_POINT_POS_16);
	//右廊下
	makePoint(MOVE_POINT_POS_17);
	makePoint(MOVE_POINT_POS_18);
	makePoint(MOVE_POINT_POS_19);
	makePoint(MOVE_POINT_POS_20);
	makePoint(MOVE_POINT_POS_21);
	makePoint(MOVE_POINT_POS_22);
	//小部屋
	makePoint(MOVE_POINT_POS_23);
	makePoint(MOVE_POINT_POS_24);

	//地下
	makePoint(MOVE_POINT_POS_25);
	makePoint(MOVE_POINT_POS_26);

	makePoint(MOVE_POINT_POS_27);
	makePoint(MOVE_POINT_POS_28);
	makePoint(MOVE_POINT_POS_29);
	makePoint(MOVE_POINT_POS_30);

	makePoint(MOVE_POINT_POS_31);
	makePoint(MOVE_POINT_POS_32);

	//階段：左
	makePoint(MOVE_POINT_POS_33);
	makePoint(MOVE_POINT_POS_34);
	makePoint(MOVE_POINT_POS_35);
	makePoint(MOVE_POINT_POS_36);
	//階段：右
	makePoint(MOVE_POINT_POS_37);
	makePoint(MOVE_POINT_POS_38);
	makePoint(MOVE_POINT_POS_39);
	makePoint(MOVE_POINT_POS_40);

	//ワープポイント
	makePoint(MOVE_POINT_POS_41, true);
	makePoint(MOVE_POINT_POS_42, true);
	makePoint(MOVE_POINT_POS_43, true);
	makePoint(MOVE_POINT_POS_44, true);
	makePoint(MOVE_POINT_POS_45, true);
	makePoint(MOVE_POINT_POS_46, true);

}

void Tracker::SearchMoveablePoint(void)
{
	moveablePointNums_.clear();

	VECTOR trackerEyePos = transform_.pos_;
	trackerEyePos.y += EYE_LEVEL;				//追跡者の目線座標

	int count = -1;	//移動ポイント番号
	for (const auto& point : movePoints_)
	{
		count++;
		//現在の移動ポイントは除外
		if (currentMovePointNum_ == count)continue;

		//ステージとLienが衝突していない場合、移動可能ポイントに追加する
		if (!CollCheckLine_AllStage(trackerEyePos, point.pos))
		{
			moveablePointNums_.emplace_back(count);
		}
	}
	assert(moveablePointNums_.size() > 0);		//移動可能ポイントが存在しない

	//ゴールポイントの決定
	DecisionGoalPoint();

	//ゴール方向のクォータニオンをセット
	goalQuaRot_ = CalcQuaRad(goalPos_, transform_.pos_);
	//回転時間の初期化
	rotateTime_ = 0.0f;
}

void Tracker::DecisionGoalPoint(void)
{
	int golaNum = 0;

	// 各変数の値を逆数（小さいほど大きな値になる）を重みとして扱う
	std::vector<double> weights;
	for (int value : moveablePointNums_) {
		weights.push_back(1.0 / (static_cast<double>(movePoints_[value].goledNum) + 1));
	}

	// 重みの合計を計算
	double totalWeight = 0.0;
	for (double weight : weights) {
		totalWeight += weight;
	}

	// 0.0からtotalWeightまでの範囲のランダムな値を生成
	double randomValue = (std::rand() / static_cast<double>(RAND_MAX)) * totalWeight;

	// 重みの累積を計算し、ランダムな値を超える最初のインデックスを選択
	double cumulativeWeight = 0.0;
	for (int i = 0; i < moveablePointNums_.size(); ++i) {
		cumulativeWeight += weights[i];
		if (randomValue < cumulativeWeight) {
			golaNum = i;
			break;
		}
	}


	//移動ポイント番号を決定
	currentMovePointNum_ = moveablePointNums_[golaNum];

	//ゴール座標のセット
	goalPos_ = movePoints_[currentMovePointNum_].pos;
	//ゴール回数の加算
	movePoints_[currentMovePointNum_].goledNum++;
}

bool Tracker::IsWarping(void)
{
	return movePoints_[currentMovePointNum_].warpPoint;
}

void Tracker::ProcessWarp(void)
{
	//ワープ先の検索
	moveablePointNums_.clear();

	int count = -1;	//移動ポイント番号
	for (const auto& point : movePoints_)
	{
		count++;

		//ワープポイント以外は除外
		if (!point.warpPoint)continue;

		//現在の移動ポイントは除外
		if (currentMovePointNum_ == count)continue;

		//ワープ先を移動可能ポイントに追加する
		moveablePointNums_.emplace_back(count);
	}
	assert(moveablePointNums_.size() > 0);	//移動可能ポイントが存在しない

	//ゴールポイントの決定
	DecisionGoalPoint();
	

	//ゴール座標にワープさせる
	transform_.pos_ = movePoints_[currentMovePointNum_].pos;

	printf("ワープした\n");

}

VECTOR Tracker::GetMoveDir(const VECTOR& goalPos) 
{
	auto pos = VSub(goalPos, transform_.pos_);
	pos.y = 0;
	return VNorm(pos);
}  

bool Tracker::IsGoal(void)
{
	bool result = false;

	// モデルと移動ポイント球体の衝突判定
	auto hit = MV1CollCheck_Sphere(
		transform_.modelId_, -1, goalPos_, MOVE_POINT_R);

	if (hit.HitNum >= 1) {
		printf("ゴール\n");
		result = true;
	}
	// 検出したポリゴン情報の後始末
	MV1CollResultPolyDimTerminate(hit);
	
	return result;
	
}

bool Tracker::IsDiscoveryPlayer(void)
{
	//プレイヤーの座標取得
	auto playerPos = player_.GetTransform().pos_;
	auto trackerPos = transform_.pos_;
	//距離
	float diff = (playerPos.z - trackerPos.z) * (playerPos.z - trackerPos.z) + 
		(playerPos.x - trackerPos.x) * (playerPos.x - trackerPos.x);	//2乗の値

	//①視野範囲（距離）に操作キャラクターが入っていない場合,false
	if (diff > VIEW_RANGE * VIEW_RANGE)return false;

	//②プレイヤーとの間をステージが挟んでいる場合、false
	playerPos.y += EYE_LEVEL;
	trackerPos.y += EYE_LEVEL;
	if(CollCheckLine_AllStage(playerPos, trackerPos))return false;
	

	//Enemyから見たプレイヤーの角度
	float playerPosRad = atan2f(playerPos.x - trackerPos.x, playerPos.z - trackerPos.z);
	float viewDeg = transform_.rot_.y;
	//③視野範囲（視野角）に操作キャラクターが入っていない場合、false
	if (!((viewDeg - VIEW_ANGLE_RAD <= playerPosRad) &&
		playerPosRad <= (viewDeg + VIEW_ANGLE_RAD)))return false;

	//プレイヤーを発見
	return true;
}

bool Tracker::IsLoseSightOfPlayer(void)
{
	//プレイヤーと壁に阻まれた時
	auto playerPos = player_.GetTransform().pos_;
	auto trackerPos = transform_.pos_;
	playerPos.y += EYE_LEVEL;
	trackerPos.y += EYE_LEVEL;
	if (CollCheckLine_AllStage(playerPos, trackerPos)&& 
		(Utility::Distance(playerPos, trackerPos)>=DIS_LOSE_SIGHT_OF_PLAYER))
	{
		return true;
	}
	return false;

}

void Tracker::DrawDiscoveryArea(void)
{
#ifdef DEBUG
	//デバック用

	//それぞれの座標
	VECTOR pos0 = transform_.pos_;
	VECTOR pos1 = VGet(0.0f, 0.0f, 0.0f);
	VECTOR pos2 = VGet(0.0f, 0.0f, 0.0f);
	VECTOR pos3 = VGet(0.0f, 0.0f, 0.0f);

	//エネミーの視野位置の座標を取得
	auto calcPos = [&](float angleY, VECTOR mPos) {

		//向いている方向を取得
		float dirX = sinf(angleY);
		float dirZ = cosf(angleY);
		//単位ベクトルを取得
		VECTOR dir = VNorm({ dirX, 0.0f, dirZ });
		//移動距離を取得
		VECTOR movePos = VScale(dir, VIEW_RANGE);
		//移動距離を加算
		VECTOR posVal = VAdd(mPos, movePos);
		posVal.y += 2;		//テスト用：見えるようにするため、地面から少し浮かす

		return posVal;
	};

	pos1 = calcPos(transform_.rot_.y - VIEW_ANGLE_RAD, transform_.pos_);
	pos2 = calcPos(transform_.rot_.y, transform_.pos_);
	pos3 = calcPos(transform_.rot_.y + VIEW_ANGLE_RAD, transform_.pos_);

	pos0.y += 2;
	DrawLine3D(pos0, pos1, 0xff00ff);	//左側に30度の線）
	DrawLine3D(pos0, pos2, 0xff00ff);	//まっすぐ線
	DrawLine3D(pos0, pos3, 0xff00ff);	//右側に30度の線）
	DrawLine3D(pos1, pos2, 0xff00ff);	
	DrawLine3D(pos2, pos3, 0xff00ff);	

	//検知判断
	if (IsDiscoveryPlayer())
	{
		//三角形の描画(座標は時計回りを順に引数を入力する)
		DrawTriangle3D(pos0, pos1, pos2, 0xffff00, true);
		DrawTriangle3D(pos0, pos2, pos3, 0xffff00, true);
	}

#endif // DEBUG


}

void Tracker::ProcessMove(void)
{
	//移動方向（移動ポイント座標）
	moveDir_ = GetMoveDir(goalPos_);
	speed_ = SPEED_WALK;
	movePow_ = VAdd(movePow_, VScale(moveDir_, speed_));
}

void Tracker::ProcessTrack(void)
{
	//移動場所
	goalPos_ = player_.GetTransform().pos_;

	moveDir_ = GetMoveDir(goalPos_);	//移動方向(プレイヤー座標)
	speed_ = SPEED_RUN;					//移動スピード
	movePow_ = VAdd(movePow_, VScale(moveDir_, speed_));	//移動量加算
}

void Tracker::ProcessRotatePortal(void)
{
	//目的方向のクォータニオンをセット
	actorRotY_ = CalcQuaRad(goalPos_, transform_.pos_);
}

void Tracker::ProcessRotateTrack(void)
{
	//回転の球面補完
	rotateTime_ += SceneManager::GetInstance().GetDeltaTime();
	float par = rotateTime_ / ROTATION_COMPLETED_TIME;
	actorRotY_ = Quaternion::Slerp(actorRotY_, goalQuaRot_, par);
}

void Tracker::UpdateNormal(void)
{
	SearchMoveablePoint();
	updateFunc_ = std::bind(&Tracker::UpdatePatrol,this);
}

void Tracker::UpdatePatrol(void)
{
	beforePos_ = transform_.pos_;		//移動前座標
	movePow_ = Utility::VECTOR_ZERO;	// 移動量をゼロ
	ProcessMove();
	CalcGravityPow();
	Collision();
	ProcessRotateTrack();
	// 回転させる
	transform_.quaRot_ = actorRotY_;

	//足音サウンドが再生していなれば再生
	footstepsSound_->PlayNonPlayingLoop();
	footstepsSound_->SetSoundFrequency(45000);


	//護符が使用された場合
	if (player_.IsUseAmulet())
	{
		//気絶処理
		isFainting_ = true;
		animController_->Play((int)ANIM_TYPE::STUN);
		updateFunc_ = std::bind(&Tracker::UpdateFainting, this);
		return;
	}

	//プレイヤー発見判定
	if (IsDiscoveryPlayer())
	{
		animController_->Play((int)ANIM_TYPE::FAST_RUN);
		updateFunc_ = std::bind(&Tracker::UpdateTracking, this);
		return;
	}

	//ゴール判定
	if (IsGoal())
	{
		//ゴール地点がワープポイントか判断
		if (IsWarping())
		{
			ProcessWarp();
		}

		SearchMoveablePoint();
		return;
	}
}

void Tracker::UpdateTracking(void)
{
	beforePos_ = transform_.pos_;		//移動前座標
	movePow_ = Utility::VECTOR_ZERO;	// 移動量をゼロ
	ProcessTrack();				// 追跡処理
	CalcGravityPow();			// 重力による移動量
	Collision();				// 衝突判定
	ProcessRotatePortal();		// 回転処理
	transform_.quaRot_ = actorRotY_;	// 回転させる

	//足音サウンドが再生していなれば再生
	footstepsSound_->PlayNonPlayingLoop();
	footstepsSound_->SetSoundFrequency(88000);


	//プレイヤーを見失ったかどうか判断
	if (IsLoseSightOfPlayer())
	{
		//ロスト状態
		animController_->Play((int)ANIM_TYPE::LOST,false);
		updateFunc_ = std::bind(&Tracker::UpdateLostPlayer, this);
		return;
	}

	//護符が使用された場合
	if (player_.IsUseAmulet())
	{
		//気絶処理
		isFainting_ = true;
		animController_->Play((int)ANIM_TYPE::STUN);
		updateFunc_ = std::bind(&Tracker::UpdateFainting, this);
		return;
	}
}

void Tracker::UpdateFainting(void)
{
	footstepsSound_->Stop();

	//気絶状態
	auto timeOver = Utility::TimeOver(faintingTime_, FAINT_TIME);
	if (timeOver)
	{
		faintingTime_ = 0.0f;
		isFainting_ = false;

		//ロスト状態
		animController_->Play((int)ANIM_TYPE::LOST, false);
		updateFunc_ = std::bind(&Tracker::UpdateLostPlayer, this);
		return;
	}
}

void Tracker::UpdateLostPlayer(void)
{
	footstepsSound_->Stop();

	if (animController_->IsEnd())
	{
		SearchMoveablePoint();
		animController_->Play((int)ANIM_TYPE::RUN);
		updateFunc_ = std::bind(&Tracker::UpdatePatrol, this);
		return;
	}
}

void Tracker::UpdateGameOver(void)
{

	//回転の球面補完
	rotateTime_ += SceneManager::GetInstance().GetDeltaTime();
	float par = rotateTime_ / TROCKER_CHANGE_DIR_SPEED;	//方向転換スピード調整
	actorRotY_ = Quaternion::Slerp(actorRotY_, goalQuaRot_, par);
	transform_.quaRot_ = actorRotY_;	// 回転させる

}

