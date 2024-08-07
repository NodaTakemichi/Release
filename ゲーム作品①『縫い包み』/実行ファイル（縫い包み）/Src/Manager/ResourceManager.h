#pragma once
#include <map>
#include <string>
#include "../Utility/StaticSingleton.h"
#include "Resource.h"

class ResourceManager:
	public StaticSingleton<ResourceManager>
{

public:

	// リソース名
	enum class SRC
	{
		//3Dモデル
		STAGE_MODEL,
		STAGE_COLL_MODEL,
		DISTORTION_WALL,
		TITLE_STAGE_MODEL,
		GOOL_DOOR_MODEL,
		METALLIC_MODEL,

		TRACKER_MODEL,
		DOOL_MONKEY,
		DOOL_PIG,
		DOOL_RABBIT,
		DOOL_SHEEP,
		AMULET_MODEL,

		//画像
		TITLE_IMG,
		RESULT_IMG,
		GAMEOVER_IMG,
		AMULET_IMG,
		HEIGHT_MAP_IMG,
		NOISE_IMG,
		ACTION_TEXT_BOX_IMG,
		COMMENT_TEXT_BOX_IMG,
		//UI
		CENTER_UI_IMG,
		AMULET_UI_IMG,
		DOOL_UI_IMG,
		GAUGE_MAIN_IMG,
		GAUGE_FRAME_IMG,
		GAUGE_MASK_IMG,
		PAUSE_BACK_IMG,
		//操作ビルボード
		OPERATION__MOVE_IMG,
		OPERATION__DASH_IMG,
		OPERATION__LEFT_CLICK_IMG,
		OPERATION__RIGHT_CLICK_IMG,
		OPERATION__PAUSE_IMG,

		//フォントデータ
		ACTION_TEXT_FONT,
		SELECT_CMD_FONT,
		LOAD_TEXT_FONT,

		//エフェクト
		FIRE_EFF,

		//頂点シェーダー
		NORMAL_MODEL_VS,	//通常モデル

		//ピクセルシェーダー
		BLUR_PS,			//ぼかし
		BURNING_DOOL_PS,	// 焼却ぬいぐるみ
		DISSOLVE_PS,		// ディゾルブ（護符用）
		BURN_GAUGE_PS,		// 焼却ゲージ
		DISTORTION_WALL_PS,	// 歪み壁
		GLITCH_PS,			// グリッチ
		NORMAL_MODEL_PS,	// ノーマルモデル
		NOISE_PS,			// ノイズ
		SPHERE_MAP_PS,		// スフィアマップ
		VIGNETTE_PS,		// ビネット
		LUMINESCENCE_PS,	// 発光モデル
	
		//音声
		FIRE_SND,			//暖炉の炎
		FOOTSTEPS_PLAYER_SND,		//足音：プレイヤー
		FOOTSTEPS_TRACKER_SND,		//足音：トラッカー
		GET_ITEM_SND,		//アイテム取得音
		USE_AMULET_SND,		//護符使用SE
		BURN_AMULET_SND,	//護符焼失SE
		BURN_DOOL_SND,		//ぬいぐるみ焼失SE
		GOAL_SND,			//ゴールSE
		NOISE_SND,			//ノイズSE
		DIS_WALL_SND,		//歪み壁SE
		DIS_WALL_LOST_SND,	//歪み壁消失SE
		CURSOL_MOVE_SND,	//カーソル移動SE

		TITLE_BGM,			//タイトルBGM
		RESULT_BGM,			//リザルトBGM
	};


	// 初期化
	void Init(void);

	// 解放(シーン切替時に一旦解放)
	void Release(void);

	// リソースの完全破棄
	void Destroy(void);

	// リソースのロード
	const Resource& Load(SRC src);

	// リソースの複製ロード(モデル用)
	int LoadModelDuplicate(SRC src);

private:
	//シングルトンの生成
	THIS_CLASS_IS_STATIC_SINGLETON(ResourceManager);

	// リソース管理の対象
	std::map<SRC, Resource*> resourcesMap_;

	// 読み込み済みリソース
	std::map<SRC, Resource&> loadedMap_;

	Resource dummy_;

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	ResourceManager(void);
	~ResourceManager(void) = default;

	// 内部ロード
	Resource& _Load(SRC src);

};
