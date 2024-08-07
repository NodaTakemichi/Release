#pragma once
#include<memory>
#include<string>
class Player;
class PixelMaterial;
class PixelRenderer;

class PlayerUI
{
public:

	PlayerUI(const Player& player);
	~PlayerUI(void);

	void LoadData(void);
	void Init(void);
	void Update(void);
	void Draw(void);


private:
	const Player& player_;

	//画面中央画像
	int centerImg_;

	//護符UI画像
	int amiletUiImage_;
	//ぬいぐるみUI画像
	int doolUiImage_;

	//フォントハンドル
	int fontHandle_;
	//アクションテキスト背景画像
	int actionTextBackImg_;
	//アクションテキスト（手に取る、燃やす.....）
	std::wstring actionText_;

	//コメントテキスト背景画像
	int commnetTextBackImg_;
	//コメントテキスト
	std::wstring commnetText_;

	//コメントの表示時間カウント
	float displayCoundTime_;
	//コメントの表示判断
	bool isDisplayCommentText_;


	// 護符のディゾルブシェーダー
	std::unique_ptr<PixelMaterial>amuletMaterial_;
	std::unique_ptr<PixelRenderer> amuletRenderer_;
	float amuletBurnTime_ ;
	//護符の消費演出中か判断
	bool isBurningAmulet_ ;

	// 焼却ゲージ
	std::unique_ptr<PixelMaterial>gaugeMaterial_;
	std::unique_ptr<PixelRenderer> gaugeRenderer_;

	//アクションテキストの描画
	void DrawTextUI(void);

	//焼却時間の描画
	void DrawBurningTime(void);

	//護符の消費演出
	void DrawBurningAmulet(void);

	//所持アイテムの表示
	void DrawHasItem(void);
};

