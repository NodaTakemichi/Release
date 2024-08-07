#include <algorithm>
#include "../../Manager/ResourceManager.h"
#include "../../Manager/Resource.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/InputManager.h"
#include "../../Manager/SoundManager.h"
#include "../../Manager/Camera.h"
#include "../../Utility/Utility.h"
#include "../../Common/Quaternion.h"
#include "../../Sound/SoundController.h"
#include "../../_debug/_DebugConOut.h"
#include "../../_debug/_DebugDispOut.h"
#include "../Common/Transform.h"
#include "../Common/Capsule.h"

#include "../Item/ItemBase.h"
#include "../Stage/Stage.h"
#include "../Stage/GoalGate.h"
#include "Player.h"

// プレイヤーの半径
constexpr float RADIOUS = 40.0f;
// プレイヤーの目の高さ
constexpr float EYE_LEVEL = 170.0f;
//ゲームオーバー時の目線の高さ
constexpr float GAME_OVER_EYE_LEVEL = 190.0f;
// プレイヤーのカプセルコライダー座標
constexpr VECTOR CAPSULE_LOCAL_POS_DOWN = { 0, RADIOUS,0 };  //下
constexpr VECTOR CAPSULE_LOCAL_POS_UP = { 0,EYE_LEVEL,0 };   //上

// オブジェクト衝突用ray座標距離
constexpr float RAY_LOCAL_DIS_FRONT = 10.0f; //手前
constexpr float RAY_LOCAL_DIS_BACK = 200.0f; //奥
// 移動スピード
constexpr float SPEED_MOVE = 4.5f;   //歩き
constexpr float SPEED_RUN = 7.0f;    //走り
// 暖炉範囲
constexpr float BURN_RADIOUS = 50.0f;

//プレイヤー書記座標
constexpr VECTOR PLAYER_POS = { 5.0f, 330.0f, -2530.0f };

//スポットライト
constexpr float LIGHT_OUT_ANGLE_DEG = 55.0f;	//外側の角度
constexpr float LIGHT_IN_ANGLE_DEG = 35.0f;		//内側の角度
constexpr float LIGHT_RANGE = 2000.0f;			//有効距離
constexpr float LIGHT_ATTEN_0 = 0.28f;			//距離減衰パラメータ０
constexpr float LIGHT_ATTEN_1 = 0.002f;		    //距離減衰パラメータ１
constexpr float LIGHT_ATTEN_2 = 0.0f;			//距離減衰パラメータ２

// 足音音声の再生速度（RUN）
constexpr int FOOTSTEP_SOUND_SPEE_RUN = 75000;
//足音
constexpr int FOOTSTEPS_SE_VOLUME = 200;		        //音量
constexpr int FOOTSTEPS_SE_HEARING_AREA = 300;	        //音が届く距離
//アイテム取得音
constexpr int GET_ITEM_SE_VOLUME = 175;		            //音量
constexpr int GET_ITEM_SE_HEARING_AREA = 300; 	        //音が届く距離
//護符使用SE
constexpr int USE_AMULET_SE_VOLUME = 200;		        //音量
constexpr int USE_AMULET_SE_HEARING_AREA = 300;	        //音が届く距離
//ぬいぐるみ焼却SE 
constexpr int BURN_DOOL_SE_VOLUME = 225;		        //音量
constexpr int BURN_DOOL_SE_HEARING_AREA = 300;	        //音が届く距離

Player::Player(void)
{
	//更新関数のセット
	updateFunc_ = std::bind(& Player::UpdateSearch,this);
}

Player::~Player(void)
{
}


void Player::LoadData(void)
{

	transform_.SetModel(-1);

	//足音サウンドの作成
	footstepsSound_ = std::make_shared<SoundController>();
	footstepsSound_->DataLoad(ResourceManager::SRC::FOOTSTEPS_PLAYER_SND);
	//取得サウンドの作成
	getItemSound_ = std::make_shared<SoundController>();
	getItemSound_->DataLoad(ResourceManager::SRC::GET_ITEM_SND);
	//護符使用サウンドの作成
	useAmuletSound_ = std::make_shared<SoundController>();
	useAmuletSound_->DataLoad(ResourceManager::SRC::USE_AMULET_SND);
	//護符焼却サウンドの作成
	burnAmuletSound_ = std::make_shared<SoundController>();
	burnAmuletSound_->DataLoad(ResourceManager::SRC::BURN_AMULET_SND);
}

void Player::Init(void)
{
	//3D基本情報の初期化
	transform_.Init(
		Utility::VECTOR_ONE,
		PLAYER_POS,
		Quaternion(),
		Quaternion(),
		Collider::TYPE::NONE
	);

	//カプセルコライダー
	capsule_ = std::make_unique<Capsule>(transform_);
	capsule_->SetRadius(RADIOUS);
	capsule_->SetLocalPosDown(CAPSULE_LOCAL_POS_DOWN);
	capsule_->SetLocalPosTop(CAPSULE_LOCAL_POS_UP);

	//衝突コライダーのクリア
	ClearStageCollider();
	ClearItemInfos();

	//テキスト
	SetActionText(L"");
	SetCommentText(L"");

	//護符のクールタイム
	amuletCoolTime_ = AMULET_USE_COOL_TIME;

	//スポットライトの作成
	auto& camera = scnMng_.GetCamera();
	lightHandle_ = CreateSpotLightHandle(
		camera.GetPos(), camera.GetForward(),
		Utility::Deg2RadF(LIGHT_OUT_ANGLE_DEG), Utility::Deg2RadF(LIGHT_IN_ANGLE_DEG),
		LIGHT_RANGE,
		LIGHT_ATTEN_0, LIGHT_ATTEN_1, LIGHT_ATTEN_2);

	//脱出判定
	isEscape_ = false;

	//サウンドの設定
	//足音音
	footstepsSound_->Init(FOOTSTEPS_SE_VOLUME, FOOTSTEPS_SE_HEARING_AREA,
		transform_.pos_, SoundController::REVERB_TYPE::PADDEDCELL);
	//アイテム取得音
	getItemSound_->Init(
		GET_ITEM_SE_VOLUME, GET_ITEM_SE_HEARING_AREA,
		transform_.pos_, SoundController::REVERB_TYPE::PADDEDCELL);
	//護符使用SE
	useAmuletSound_->Init(
		USE_AMULET_SE_VOLUME, USE_AMULET_SE_HEARING_AREA,
		transform_.pos_, SoundController::REVERB_TYPE::PADDEDCELL);
	//護符焼却SE
	burnAmuletSound_->Init(
		BURN_DOOL_SE_VOLUME, BURN_DOOL_SE_HEARING_AREA,
		transform_.pos_, SoundController::REVERB_TYPE::PADDEDCELL);

}

void Player::Update(void)
{
	//ライト更新
	auto& camera = scnMng_.GetCamera();
	SetLightDirectionHandle(lightHandle_, camera.GetForward());
	SetLightPositionHandle(lightHandle_, camera.GetPos());

	//テキストを常時nullにする
	SetActionText(L"");
	SetCommentText(L"");

	//サウンドの座標更新
	footstepsSound_->SetSoundPos(transform_.pos_);
	useAmuletSound_->SetSoundPos(transform_.pos_);
	burnAmuletSound_->SetSoundPos(transform_.pos_);


	//現在の更新関数の呼び出し
	updateFunc_();
	//3D情報の更新
	transform_.Update();
	return;
}

void Player::Draw(void)
{
#ifdef _DEBUG
	//デバック用
	//capsule_->Draw();

	//プレイヤー座標中心
	DrawSphere3D(transform_.pos_, 3, 5, 0xff0000, 0xff0000, true);

	//プレイヤー座標
	DrawFormatString(0, 30, 0xffffff,
		L"プレイヤー座標：(%0.2f, %0.2f, %0.2f)", 
		transform_.pos_.x, transform_.pos_.y, transform_.pos_.z);

	//地面判定用ライン
	DrawLine3D(gravHitPosUp_, gravHitPosDown_, 0xffff00);
#endif // _DEBUG
}

const VECTOR Player::GetDirection(void) const
{
	return VNorm(scnMng_.GetCamera().GetAngles());
}

void Player::AddItemInfo(const std::weak_ptr<ItemBase> coll)
{
	itemInfos_.push_back(coll);
}

void Player::ClearItemInfos(void)
{
	itemInfos_.clear();
}

void Player::SetGameOverInfo(const VECTOR& opponentPos)
{
	//敵に捕まった時、敵方向へカメラを回転させる
	VECTOR rotateAngle = opponentPos;
	rotateAngle.y += GAME_OVER_EYE_LEVEL;			//目線（カメラ方向）を上げる
	scnMng_.GetCamera().SetGameOverRot(rotateAngle);
	//カメラモードのチェンジ
	scnMng_.GetCamera().ChangeMode(Camera::MODE::GAME_OVER_AUTO);

	//ゲームオーバー状態に変更
	updateFunc_ = std::bind(&Player::UpdateGameOver, this);
}

void Player::ProcessMove(void)
{
	//入力
	auto& ins = InputManager::GetInstance();
	
	// X軸回転を除いた、重力方向に垂直なカメラ角度(XZ平面)を取得
	Quaternion cameraRot = 
		SceneManager::GetInstance().GetCamera().GetQuaRotOutX();

	//移動方向
	VECTOR dir = Utility::VECTOR_ZERO;

	//移動操作
	//前方向
	if (ins.IsNew(KEY_INPUT_W))				
	{
		dir = VAdd(dir,cameraRot.GetForward());
	}
	//後方向
	if (ins.IsNew(KEY_INPUT_S))		
	{
		dir = VAdd(dir, cameraRot.GetBack());
	}
	//左方向
	if (ins.IsNew(KEY_INPUT_A))		
	{
		dir = VAdd(dir, cameraRot.GetLeft());
	}
	//右方向
	if (ins.IsNew(KEY_INPUT_D))		
	{
		dir = VAdd(dir, cameraRot.GetRight());
	}

	if (!Utility::EqualsVZero(dir)) {
		//移動方向の正規化
		dir = VNorm(dir);

		// 移動処理
		speed_ = SPEED_MOVE;
		if (ins.IsNew(KEY_INPUT_LSHIFT))
		{
			speed_ = SPEED_RUN;
			//足音音声の再生速度変更
			footstepsSound_->SetSoundFrequency(FOOTSTEP_SOUND_SPEE_RUN);

		}
		moveDir_ = dir;
		movePow_ = VScale(moveDir_, speed_);

		//足音サウンドが再生していなれば再生
		footstepsSound_->PlayNonPlayingLoop();
	}
	else 
	{
		//足音サウンドの停止
		if (footstepsSound_->CheckPlaySound())
		{
			footstepsSound_->Stop();
		}
	}
}

void Player::ProcessObtainItem(void)
{
	//アイテムとのRay衝突判定
	CollisionItem();
}

void Player::ProcessUseItem(void)
{
	//護符の使用
	//常に護符は未使用状態（false）にする
	SetUseAmulet(false);

	//クールタイム
	if (!Utility::TimeOver(amuletCoolTime_, AMULET_USE_COOL_TIME))return;

	//入力
	auto& ins = InputManager::GetInstance();
	//ボタンを押した時
	if (ins.IsClickMouseRight())
	{
		//護符を１枚以上所持している時
		if (haveAmuletNum_ >= 1)
		{
			//①護符の枚数を減らす
			haveAmuletNum_--;

			//②護符の使用判断
			SetUseAmulet(true);

			//③クールタイムの設定
			amuletCoolTime_ = 0.0f;

			//④SE再生
			useAmuletSound_->Play();
			burnAmuletSound_->Play();
		}
		else
		{
			SetCommentText(L"護符を持ってない");
		}
	}
}

void Player::CollisionItem(void)
{
	//入力情報
	auto& ins = InputManager::GetInstance();
	// カメラ
	const auto& camera = scnMng_.GetCamera();

	//rayの座標
	objHitRayPosFront_ = camera.GetPos();					//カメラ座標
	objHitRayPosBack_ = VAdd(objHitRayPosFront_, 
		VScale(camera.GetForward(), RAY_LOCAL_DIS_BACK));	//カメラ視線先

	for (const auto& item : itemInfos_)
	{
		if(item.lock()->GetHasItem())continue;

		// アイテムとの衝突
		auto hit = MV1CollCheck_Line(
			item.lock()->GetTransform().modelId_, -1, objHitRayPosFront_, objHitRayPosBack_);

		//衝突していない場合
		if (hit.HitFlag <= 0)continue;

		//①アイテムと衝突している
		SetActionText(L"左クリック：手に取る");

		//②衝突しているときに、ボタンを押すと”入手”する
		if (ins.IsTrgMouseLeft())
		{
			//変数にアイテムの獲得を教える←種類別に別処理を行う
			switch (item.lock()->GetItemType()) {
			case ItemBase::ITEM_TYPE::AMULET:
				//護符の枚数を加算
				TRACE("護符の取得\n");
				haveAmuletNum_++;
				break;
			default:
				//人形の獲得
				if (!haveTheDoll_)
				{
					TRACE("ぬいぐるみの取得\n");
					haveTheDoll_ = true;
					haveDoll_ = item;
				}
				else
				{
					SetCommentText(L"一つしか持てそうにない");
					continue;
				}
				break;
			}

			//アイテムを取得状態にする
			item.lock()->SetHasItem(true);

			//アイテム取得音再生
			getItemSound_->SetSoundPos(transform_.pos_);
			getItemSound_->Play();
		}
	}
}

void Player::CollisionGoalWall(void)
{
	//視線座標と歪み壁の衝突判定
	if (CollCheckLine_Stage(objHitRayPosFront_, objHitRayPosBack_, Collider::TYPE::DIS_WALL))
	{
		SetActionText(L"調べる");

		//左クリックが押下される
		auto& input = InputManager::GetInstance();
		if (input.IsTrgMouseLeft())
		{
			//ぬいぐるみの全滅判定
			if (CheckDeadAllDoll())
			{
				//歪み壁のディゾルブ開始
				goalGate_.lock()->SetOpenGate();

				//歪み壁削除
				std::vector<std::shared_ptr<Collider>>stageColls;
				for (const auto& coll : stageColliders_)
				{
					if (coll->type_ != Collider::TYPE::DIS_WALL)
					{
						stageColls.push_back(coll);
					}
				}
				stageColliders_ = stageColls;
			}
			else
			{
				SetCommentText(L"まだ通れなさそうだ");
			}
		}
	}
}

void Player::CollisionGoalDoor(void)
{
	//視線座標と歪み壁の衝突判定
	if (CollCheckLine_Stage(objHitRayPosFront_, objHitRayPosBack_, Collider::TYPE::GOAL_DOOR))
	{
		SetActionText(L"脱出する");

		//左クリックが押下される
		auto& input = InputManager::GetInstance();
		if (input.IsTrgMouseLeft())
		{
			//脱出判定
			isEscape_ = true;
		}
	}
}

bool Player::CheckBurnTheDoll(void)
{
	//①Rayの奥座標と暖炉座標の差分距離が範囲内
	auto diff = Utility::Distance(Stage::FIREPLACE_HIT_POS, objHitRayPosBack_);
	if (diff <= BURN_RADIOUS)
	{
		if (haveTheDoll_)SetActionText(L"ぬいぐるみを燃やす");
		else SetActionText(L"調べる");

		//②左クリックが押下される
		auto& input = InputManager::GetInstance();
		if (input.IsTrgMouseLeft())
		{
			//③ぬいぐるみを所持している
			if (haveTheDoll_)
			{
				//足音音声の停止
				footstepsSound_->Stop();

				TRACE("ぬいぐるみの焼却処理の開始\n");

				//ぬいぐるみの焼失演出の開始
				haveDoll_.lock()->StartBurnDirection();

				//ぬいぐるみを未所持状態にする
				haveTheDoll_ = false;

				//焼却処理の開始
				return true;
			}
			else
			{
				TRACE("ぬいぐるみを所持していません\n");
				SetCommentText(L"何か燃やせそうだ");
			}
		}
	}

	return false;
}

void Player::SetActionText(std::wstring text)
{
	if (text == L"")actionText_.clear();
	actionText_ = text; 
}

void Player::SetCommentText(std::wstring text)
{
	if (text == L"")commentText_.clear();
	commentText_ = text;
}

bool Player::CheckDeadAllDoll(void)
{
	for (const auto& item : itemInfos_)
	{
		if (item.lock()->GetItemType() == ItemBase::ITEM_TYPE::AMULET)continue;
		if (item.lock()->IsAlive())
		{
			return false;
		}
	}

	//全滅
	return true;
}

void Player::UpdateSearch(void)
{
	beforePos_ = transform_.pos_;		// 移動前座標
	movePow_ = Utility::VECTOR_ZERO;	// 移動量をゼロ
	ProcessMove();						// 移動処理
	CalcGravityPow();					// 重力による移動量
	Collision();						// 衝突判定

	ProcessObtainItem();				//アイテムの入手操作
	CollisionGoalWall();				//ゴール壁との衝突操作
	CollisionGoalDoor();				//ゴールとの衝突操作
	ProcessUseItem();					//アイテムの使用
	transform_.quaRot_ = actorRotY_;	// 回転させる


	//ぬいぐるみの焼却判断
	if (CheckBurnTheDoll())
	{
		//焼却中にする
		updateFunc_ = std::bind(&Player::UpdateBurning, this);
		return;
	}
}

void Player::UpdateBurning(void)
{	
	movePow_ = Utility::VECTOR_ZERO;	// 移動量をゼロ
	CalcGravityPow();					// 重力による移動量
	Collision();						// 衝突判定
	ProcessUseItem();					// アイテムの使用
	transform_.quaRot_ = actorRotY_;	// 回転させる


	//焼却時間の終了
	if (Utility::TimeOver(burnTime_, BURN_END_TIME))
	{
		//全焼か判定
		if (CheckDeadAllDoll())
		{
			SetCommentText(L"ぬいぐるみを全て燃やしたようだ");
		}


		//ぬいぐるみを未所持状態にする
		haveTheDoll_ = false;
		//焼却時間のリセット
		burnTime_ = 0.0f;

		//状態遷移
		updateFunc_ = std::bind(&Player::UpdateSearch, this);
		return;
	}
}

void Player::UpdateGameOver(void)
{
	//回転の球面補完
	rotateTime_ += SceneManager::GetInstance().GetDeltaTime();
	float par = rotateTime_ / 0.075f;
	actorRotY_ = Quaternion::Slerp(actorRotY_, goalQuaRot_, par);
	transform_.quaRot_ = actorRotY_;	// 回転させる

}
