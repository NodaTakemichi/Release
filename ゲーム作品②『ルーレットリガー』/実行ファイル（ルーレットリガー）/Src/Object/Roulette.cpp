#include<DxLib.h>
#include<math.h>
#include"../Application.h"
#include"../Utility/Utility.h"
#include"../Manager/SceneManager.h"
#include"../Manager/InputManager.h"
#include"../Manager/DataManager/SoundManager.h"
#include"./Unit/Status/Command.h"
#include"../_debug/_DebugConOut.h"
#include"../_debug/_DebugDispOut.h"
#include "Roulette.h"

//ルーレットの回転
constexpr float ROT_SPEED_MAX = -10.0f;		//速度
constexpr float ROT_BRAKE = 120.0f;			//ブレーキ力
constexpr float ROT_STOP_SPEED = 0.1f;			//回転停止判断速度
//オートでのルーレット停止するまでの待機時間
constexpr float STOP_WAIT_TIME_ROU = 0.7f;
//ルーレット用フォント
constexpr int ROULETTE_FONT_SIZE = 20;
constexpr int ROULETTE_FONT_THICK = 10;
constexpr int ROULETTE_FONT_COLOR = 0xe6e6e6;
//矢印位置
const Vector2 ARROW_SHOW_POS = Vector2{ -50,150 };
//ルーレット位置
const Vector2 ROULETTE_SHOW_POS = Vector2{ 0,530 };
//コマンド名表示
const Vector2 ROTATE_LOCAL_CENTER_POS = Vector2{190,10};
constexpr int CMD_NAME_COLOR = 0x220022;
//一周
constexpr float ONE_LAP = 360.0f;


Roulette::Roulette()
{
	rouletteImg_= 0;	
	rouFrame_= 0;	
	center_= 0;		
	arrowImg_= 0;
	fontHandle_= 0;
	rotateSE_= 0;
	stopingSE_= 0;
	stopBtnSE_= 0;
	angle_=0.0;
	rotPower_=0.0;
	isRouSpin_=true;
	isStop_=false;
	totalTime_=0.0f;
}

Roulette::~Roulette()
{
}

void Roulette::Init(void)
{
	//初期化
	angle_ = 0.0;
	rotPower_ = 0.0;
	isStop_ = false;
	isRouSpin_ = true;
	totalTime_ = 0.0f;

	//画像登録
	arrowImg_ = LoadGraph("./Data/Image/UI/arrow.png");
	rouletteImg_ = LoadGraph("./Data/Image/UI/roulette.png");
	rouFrame_ = LoadGraph("./Data/Image/UI/RouFrame.png");
	center_ = LoadGraph("./Data/Image/UI/center.png");

	//フォントの変更
	fontHandle_=CreateFontToHandle("@ＭＳ 明朝", ROULETTE_FONT_SIZE, ROULETTE_FONT_THICK);

	//SEの登録
	auto& snd = SoundManager::GetInstance();
	rotateSE_ = snd.LoadSound(SoundManager::SOUND_TYPE::ROU_ROTATE);
	stopingSE_ = snd.LoadSound(SoundManager::SOUND_TYPE::ROU_STOPING);
	stopBtnSE_ = snd.LoadSound(SoundManager::SOUND_TYPE::ROU_STOP_BTN);

}

void Roulette::Update(void)
{
	//ルーレットの回転処理
	RotateProcess();
}

void Roulette::Draw(void)
{
	int cx = Application::SCREEN_SIZE_X;
	int cy = Application::SCREEN_SIZE_Y;


	//矢印描画
	DrawGraph(cx / 2+ ARROW_SHOW_POS.x, ARROW_SHOW_POS.y, arrowImg_, true);
	//ルーレット座標
	int rouPosX = cx / 2+ ROULETTE_SHOW_POS.x;
	int rouPosY = ROULETTE_SHOW_POS.y;

	//ルーレット枠描画
	int x, y;
	GetGraphSize(rouFrame_, &x, &y);
	DrawGraph(rouPosX - x / 2, rouPosY - y / 2, rouFrame_, true);

	//バイリニア補間モード
	SetDrawMode(DX_DRAWMODE_BILINEAR);

	//ルーレット描画
	DrawRotaGraph(rouPosX, rouPosY,
		1.0f, Utility::Deg2RadF(angle_), rouletteImg_, true, false);

	//各コマンド技の回転幅、相対回転
	auto rotSpan = Utility::Deg2RadF(
		ONE_LAP / static_cast<float>(cmdNames_.size()));
	auto rotReltive = rotSpan / 2;

	//コマンド技の回転描画
	int i = 0;
	for (auto& cmdName : cmdNames_)
	{
		auto name = cmdName->GetName().c_str();

		//コマンド名描画
		DrawRotaStringToHandle(
			rouPosX, rouPosY,		//座標
			1.0, 1.0,				//拡大率
			ROTATE_LOCAL_CENTER_POS.x, ROTATE_LOCAL_CENTER_POS.y,	//回転の中心座標（相対座標）
			Utility::Deg2RadF(angle_) + (i * rotSpan) + rotReltive,	//角度
			CMD_NAME_COLOR, fontHandle_, 0x0,	//フォント
			true, name);

		//角度をずらす
		i++;
	}


	//ネアレストネイバー法
	SetDrawMode(DX_DRAWMODE_NEAREST);

	//ルーレット中央
	GetGraphSize(center_, &x, &y);
	DrawGraph(rouPosX - x / 2, rouPosY - y / 2, center_, true);

}

void Roulette::Release(void)
{
	//音声の開放
	StopSoundMem(rotateSE_);

	//画像の開放
	DeleteGraph(arrowImg_);
	DeleteGraph(rouletteImg_);
	DeleteGraph(rouFrame_);
	DeleteGraph(center_);

	cmdNames_.clear();
}

void Roulette::StopRoulette(const bool& autom)
{
	if (!isRouSpin_)return;		//既にブレーキ状態なら、処理をしない

	bool stop = false;
	//手動か自動か判断する
	if (autom)
	{
		//時間が来たらブレーキ状態にする
		stop = Utility::OverTime(totalTime_, STOP_WAIT_TIME_ROU);
	}
	else
	{
		//左クリック検知時、ブレーキ状態にする
		auto& ins = InputManager::GetInstance();
		stop = ins.IsTrgMouseLeft();
	}

	if (stop)
	{
		//ルーレット停止
		isRouSpin_ = false;

		//SE再生
		auto& snd = SoundManager::GetInstance();
		snd.PlaySoundBack(stopingSE_);
		snd.PlaySoundBack(stopBtnSE_);
		//回転中SEの停止
		StopSoundMem(rotateSE_);
	}
}

void Roulette::SetCommand(std::vector<Command*> cmd)
{
	cmdNames_ = cmd;
}

void Roulette::ResetRouSpin(void)
{
	//angle = 0.0;		//ルーレットの回転状態を戻さない
	rotPower_ = 0.0;
	isStop_ = false;
	isRouSpin_ = true;
	totalTime_ = 0.0f;

	//回転SEの再生
	auto& snd = SoundManager::GetInstance();
	snd.PlaySoundBGM(rotateSE_);

}

Command* Roulette::GetCmd(void)
{
	//回転量
	auto rotAmount = fmod(angle_, ONE_LAP) * -1;
	//コマンドの数
	int num = static_cast<double>(cmdNames_.size());
	//決定されたコマンドを判別する
	double angle = 0.0;
	double onwAngle = ONE_LAP / num;	//一個分の角度
	//角度からコマンドを取得する
	for (size_t i = 0; i <= num; i++)
	{
		//角度の以下以上
		if (angle < rotAmount && rotAmount <= angle + onwAngle)
		{
			//コマンドを返す
			return cmdNames_[i];
		}

		//角度を加算
		angle += onwAngle;
	}
}

void Roulette::RotateProcess(void)
{
	//停止までのブレーキ力
	const float stopSpeed = -ROT_SPEED_MAX / ROT_BRAKE;

	//回転量の計算（回転かブレーキで速度変更）
	//回転状態：最高速度、ブレーキ状態：減速した値
	rotPower_ = isRouSpin_ ? ROT_SPEED_MAX : rotPower_ + stopSpeed;

	//停止状態か判断
	if (rotPower_ < ROT_STOP_SPEED)
	{
		//回転量を加える
		angle_ += rotPower_;
	}
	else
	{
		//ルーレット停止状態にする
		isStop_ = true;
	}
}
