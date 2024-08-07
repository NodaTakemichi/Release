#pragma once
#include"../../Common/Vector2.h"
class ButtonUI
{
public:
	ButtonUI();
	virtual ~ButtonUI();

	virtual void Init(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;
	virtual void Release(void) = 0;



	//ボタンの上にマウスが乗っているかどうか
	virtual bool MouseOnButton(void) = 0;
	//クリック判定(押した瞬間)
	bool PushButton(void);
	//決定（ボタンから離したとき）
	bool ButtonDecision(void);


protected:
	//ボタンフレームの色の濃さ（通常）
	static constexpr float FRAME_COLOR_DEPTH_NORMAL = 1.0f;
	//ボタンフレームの色の濃さ（クリック時）
	static constexpr float FRAME_COLOR_DEPTH_CLICK = 2.0f;

	//座標
	Vector2 pos_;

	//背面画像
	int backImg_;
	//手前画像
	int frontImg_;
	
	
	//ボタンフレームの濃さ
	float frameColorDepth_;

	//ボタンの上にマウスが乗っているかどうか
	bool onBtnBefor_;
	bool onBtn_;

	//ボタン決定SE
	int clickSE_;
	//オンボタンSE
	int onBtnSE_;

	//オンボタンSE再生
	void PlayOnBtnSE(void);
	//決定ボタンSE再生
	void PlayDecSE(void);

private:

};

