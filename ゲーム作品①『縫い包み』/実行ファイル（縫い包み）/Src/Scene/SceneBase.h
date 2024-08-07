#pragma once
#include<vector>
#include<functional>
class SceneManager;
class InputManager;
class Button;

//最低ローディング時間
constexpr float LOADING_TIME = 2.0f;
//フォントサイズ
constexpr int FONT_SIZE = 28;
//ボタンUIの行間
constexpr int LINE_SPACING = 80;

//ローディング
constexpr int COMMA_MAX_NUM = 7;		//「now loading......」のコンマの数
constexpr float COMMA_TIME = 0.5f;		//「now loading......」のコンマ数を増やす時間
constexpr int LOADING_STRING_POS_X = 950;	//「now loading......」の座標X
constexpr int LOADING_STRING_POS_Y = 750;	//「now loading......」の座標Y


class SceneBase
{
public:

	SceneManager& sceneManager_;

	SceneBase(SceneManager& scene);
	virtual ~SceneBase(void);

	virtual void LoadData(void) = 0;
	virtual void Init(void) = 0;
	virtual void Update(InputManager& input) = 0;
	virtual void Draw(void) = 0;

protected:

	std::function<void(InputManager&)> updataFunc_;
	std::function<void(void)> drawFunc_;

	//ローディング経過時間
	float loadingTime_;

	//選択ボタンフォントハンドル
	int buttnFontHandle_;
	//カーソル位置
	int cursolNum_ ;

	//ボタン選択操作
	void BtnSelectProcess(const int& btnNum);

	//「now loading......」の描画
	void DrawNowLoading(void);

private:

};