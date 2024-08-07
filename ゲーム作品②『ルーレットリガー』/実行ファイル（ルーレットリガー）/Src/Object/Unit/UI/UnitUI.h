#pragma once
#include<string>
#include<vector>
#include"../../../Common/Vector2.h"
#include "../../../Utility/Utility.h"
#include "../Status/Buff.h"
#include"../UnitBase.h"

class UnitUI
{
public:

	UnitUI(Vector2 pos, std::string& name,int& hp,int& nowHp, int& maxHp, bool& active);
	virtual ~UnitUI();


	virtual void Init(void);
	virtual void Draw(void);
	void Release(void);


	//ロックオン
	virtual void DrawRockOn(void) = 0;
	//バフをセットする
	void SetBuff(std::vector<Buff*> buffs);


	//ダメージ表記
	void SetDmgValue(const bool& drawing, const int& dmg);
	//回復量表記
	void SetHealValue(const bool& drawing, const int& heal);

private:

protected:

	//ユニット座標
	Vector2 unitPos_;
	//ユニットネーム
	std::string& name_;
	//最大HP,減少中HP
	int& hp_;
	int& maxHp_;
	int& nowHp_;
	bool& active_;

	//ネームフレーム画像
	int nameFrameImg_;
	//ターゲット画像
	int targetImg_;

	//ダメージフレーム画像
	int dmgFrameImg_;
	//ダメージ描画判定
	bool dmgNumDrawing_;	//ダメージを true:表示 , false:非表示
	//回復値フレーム画像
	int healFrameImg_;
	//回復値描画判定
	bool healNumDrawing_;	//ダメージを true:表示 , false:非表示
	//ダメージ値
	int dmg_;

	//ダメージフォント
	int dmgFontHandle_;
	//回復フォント
	int healFontHandle_;
	//ユニットフォント
	int unitFontHandle_;

	//合計時間
	float totalTime_;

	//画像アイコン
	int buffIconHandles_[static_cast<int>(Buff::BUFF_TYPE::MAX)];
	//所有バフの取得
	std::vector<Buff*> buffs_;

	//HPのシェーダー描画
	void DrawHpShader(const Vector2& pos ,const COLOR_F& color);
	//HP枠の描画
	void DrawHpFrame(const Vector2& pos);
	//名前の描画
	void DrawName(const std::string& name,const Vector2& uPos,const COLOR_F& color);
	//バフアイコンの描画
	void DrawBuffIcon();
	



};

