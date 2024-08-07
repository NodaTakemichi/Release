#include <DxLib.h>
#include "../Application.h"
#include "Resource.h"
#include "ResourceManager.h"

void ResourceManager::Init(void)
{

	using RES = Resource;
	using RES_T = RES::TYPE;
	const std::wstring PATH_IMG = L"Data/Image/";
	const std::wstring PATH_MDL = L"Data/Model/";
	const std::wstring PATH_EFF = L"Data/Effect/";
	const std::wstring PATH_SHADER = L"Data/Shader/";
	const std::wstring PATH_FONT = L"Data/Font/";
	const std::wstring PATH_VS = L"Data/Shader/Vertex/";
	const std::wstring PATH_PS = L"Data/Shader/Pixel/";
	const std::wstring PATH_SND = L"Data/Sound/";



	Resource* res;

	// ステージモデル
	res = new RES(RES_T::MODEL, PATH_MDL + L"Stage/Stage.mv1");
	resourcesMap_.emplace(SRC::STAGE_MODEL, res);
	// 衝突用ステージモデル
	res = new RES(RES_T::MODEL, PATH_MDL + L"Stage/HitStage.mv1");
	resourcesMap_.emplace(SRC::STAGE_COLL_MODEL, res);
	// 歪み壁モデル
	res = new RES(RES_T::MODEL, PATH_MDL + L"Stage/GateMist.mv1");
	resourcesMap_.emplace(SRC::DISTORTION_WALL, res);
	// タイトル用ステージモデル
	res = new RES(RES_T::MODEL, PATH_MDL + L"Stage/TitleStage.mv1");
	resourcesMap_.emplace(SRC::TITLE_STAGE_MODEL, res);
	// ゴールモデル
	res = new RES(RES_T::MODEL, PATH_MDL + L"Stage/GoolDoor.mv1");
	resourcesMap_.emplace(SRC::GOOL_DOOR_MODEL, res);
	// メタリックオブジェクトモデル
	res = new RES(RES_T::MODEL, PATH_MDL + L"Stage/MetalicObj.mv1");
	resourcesMap_.emplace(SRC::METALLIC_MODEL, res);

	// 追跡者モデル
	res = new RES(RES_T::MODEL, PATH_MDL + L"Tracker/Tracker_Model.mv1");
	resourcesMap_.emplace(SRC::TRACKER_MODEL, res);
	// 人形モデル
	//res = new RES(RES_T::MODEL, PATH_MDL + L"pbrmodel/sphere.mv1");
	//res = new RES(RES_T::MODEL, PATH_MDL + L"pbrmodel/roundbox.mv1");
	res = new RES(RES_T::MODEL, PATH_MDL + L"Item/Dool/MONKEY.mv1");
	resourcesMap_.emplace(SRC::DOOL_MONKEY, res);
	res = new RES(RES_T::MODEL, PATH_MDL + L"Item/Dool/PIG.mv1");
	resourcesMap_.emplace(SRC::DOOL_PIG, res);
	res = new RES(RES_T::MODEL, PATH_MDL + L"Item/Dool/RABBIT.mv1");
	resourcesMap_.emplace(SRC::DOOL_RABBIT, res);
	res = new RES(RES_T::MODEL, PATH_MDL + L"Item/Dool/SHEEP.mv1");
	resourcesMap_.emplace(SRC::DOOL_SHEEP, res);
	//護符モデル
	res = new RES(RES_T::MODEL, PATH_MDL + L"Item/Amulet/Amulet.mv1");
	resourcesMap_.emplace(SRC::AMULET_MODEL, res);

	// タイトル画像
	res = new RES(RES_T::IMG, PATH_IMG + L"UI/TitleImage.png");
	resourcesMap_.emplace(SRC::TITLE_IMG, res);
	// リザルト画像
	res = new RES(RES_T::IMG, PATH_IMG + L"UI/ResultImg.png");
	resourcesMap_.emplace(SRC::RESULT_IMG, res);
	// ゲームオーバー背景
	res = new RES(RES_T::IMG, PATH_IMG + L"bg/GameOver.png");
	resourcesMap_.emplace(SRC::GAMEOVER_IMG, res);
	//護符画像
	res = new RES(RES_T::IMG, PATH_IMG + L"Item/amulet.jpg");
	resourcesMap_.emplace(SRC::AMULET_IMG, res);
	//ハイトマップ
	res = new RES(RES_T::IMG, PATH_IMG + L"Shader/HeightMap.png");
	resourcesMap_.emplace(SRC::HEIGHT_MAP_IMG, res);
	//ノイズ画像
	res = new RES(RES_T::IMG, PATH_IMG + L"Shader/noise.png");
	resourcesMap_.emplace(SRC::NOISE_IMG, res);

	//照準画像
	res = new RES(RES_T::IMG, PATH_IMG + L"UI/center.png");
	resourcesMap_.emplace(SRC::CENTER_UI_IMG, res);
	//アクションテキストボックス画像
	res = new RES(RES_T::IMG, PATH_IMG + L"UI/TextBox.png");
	resourcesMap_.emplace(SRC::ACTION_TEXT_BOX_IMG, res);
	//コメントテキストボックス画像
	res = new RES(RES_T::IMG, PATH_IMG + L"UI/TextBox_red.png");
	resourcesMap_.emplace(SRC::COMMENT_TEXT_BOX_IMG, res);
	//護符UI画像
	res = new RES(RES_T::IMG, PATH_IMG + L"UI/AmuletUI.png");
	resourcesMap_.emplace(SRC::AMULET_UI_IMG, res);
	//ぬいぐるみUI画像
	res = new RES(RES_T::IMG, PATH_IMG + L"UI/doolUI.png");
	resourcesMap_.emplace(SRC::DOOL_UI_IMG, res);
	//メインゲージUI画像
	res = new RES(RES_T::IMG, PATH_IMG + L"UI/GaugeMain.png");
	resourcesMap_.emplace(SRC::GAUGE_MAIN_IMG, res);
	//ゲージフレームUI画像
	res = new RES(RES_T::IMG, PATH_IMG + L"UI/GaugeFrame.png");
	resourcesMap_.emplace(SRC::GAUGE_FRAME_IMG, res);
	//ゲージマスクUI画像
	res = new RES(RES_T::IMG, PATH_IMG + L"UI/GaugeMask.png");
	resourcesMap_.emplace(SRC::GAUGE_MASK_IMG, res);
	//ポーズメニュー背景画像
	res = new RES(RES_T::IMG, PATH_IMG + L"UI/PauseBack.png");
	resourcesMap_.emplace(SRC::PAUSE_BACK_IMG, res);

	//操作UI
	//移動操作：UI画像
	res = new RES(RES_T::IMG, PATH_IMG + L"UI/operation_move.png");
	resourcesMap_.emplace(SRC::OPERATION__MOVE_IMG, res);
	//ダッシュ：UI画像
	res = new RES(RES_T::IMG, PATH_IMG + L"UI/operation_dash.png");
	resourcesMap_.emplace(SRC::OPERATION__DASH_IMG , res);
	//左クリック：UI画像
	res = new RES(RES_T::IMG, PATH_IMG + L"UI/operation_left_click.png");
	resourcesMap_.emplace(SRC::OPERATION__LEFT_CLICK_IMG, res);
	//右クリック：UI画像
	res = new RES(RES_T::IMG, PATH_IMG + L"UI/operation_right_click.png");
	resourcesMap_.emplace(SRC::OPERATION__RIGHT_CLICK_IMG, res);
	//ポーズ：UI画像
	res = new RES(RES_T::IMG, PATH_IMG + L"UI/operation_pause.png");
	resourcesMap_.emplace(SRC::OPERATION__PAUSE_IMG, res);

	//フォントデータ
	//アクションテキスト
	res = new RES(RES_T::FONT, PATH_FONT + L"NemukeMedium.otf");
	resourcesMap_.emplace(SRC::ACTION_TEXT_FONT, res);
	//セレクトコマンド
	res = new RES(RES_T::FONT, PATH_FONT + L"EbiharaNoKuseji.ttf");
	resourcesMap_.emplace(SRC::SELECT_CMD_FONT, res);
	//ロードテキスト
	res = new RES(RES_T::FONT, PATH_FONT + L"NemukeMedium.otf");
	resourcesMap_.emplace(SRC::LOAD_TEXT_FONT, res);

	//エフェクトデータ
	//暖炉の炎
	res = new RES(RES_T::EFFEKSEER, PATH_EFF + L"Fire/fire.efkefc");
	resourcesMap_.emplace(SRC::FIRE_EFF, res);

	//頂点シェーダー
	//法線マップ付き
	res = new RES(RES_T::VERTEX_SHADER, PATH_VS + L"ModelNormal1FrameVertexShader.vso");
	resourcesMap_.emplace(SRC::NORMAL_MODEL_VS, res);

	//ピクセルシェーダー
	//ぼかし
	res = new RES(RES_T::PIXEL_SHADER, PATH_PS + L"Blur.pso");
	resourcesMap_.emplace(SRC::BLUR_PS, res);
	//焼却ぬいぐるみ
	res = new RES(RES_T::PIXEL_SHADER, PATH_PS + L"BurningDool.pso");
	resourcesMap_.emplace(SRC::BURNING_DOOL_PS, res);
	//ディゾルブ（護符用）
	res = new RES(RES_T::PIXEL_SHADER, PATH_PS + L"Dissolve.pso");
	resourcesMap_.emplace(SRC::DISSOLVE_PS, res);
	//焼却ゲージ
	res = new RES(RES_T::PIXEL_SHADER, PATH_PS + L"BurnGauge.pso");
	resourcesMap_.emplace(SRC::BURN_GAUGE_PS, res);
	//歪み壁
	res = new RES(RES_T::PIXEL_SHADER, PATH_PS + L"DistortionWall.pso");
	resourcesMap_.emplace(SRC::DISTORTION_WALL_PS, res);
	//グリッチ
	res = new RES(RES_T::PIXEL_SHADER, PATH_PS + L"Glitch.pso");
	resourcesMap_.emplace(SRC::GLITCH_PS, res);
	//ノーマルモデル
	res = new RES(RES_T::PIXEL_SHADER, PATH_PS + L"ModelNormalPixelShader.pso");
	resourcesMap_.emplace(SRC::NORMAL_MODEL_PS, res);
	//ノイズ
	res = new RES(RES_T::PIXEL_SHADER, PATH_PS + L"Noise.pso");
	resourcesMap_.emplace(SRC::NOISE_PS, res);
	//スフィアマップ
	res = new RES(RES_T::PIXEL_SHADER, PATH_PS + L"SphereMap.pso");
	resourcesMap_.emplace(SRC::SPHERE_MAP_PS, res);
	//ビネット
	res = new RES(RES_T::PIXEL_SHADER, PATH_PS + L"Vignette.pso");
	resourcesMap_.emplace(SRC::VIGNETTE_PS, res);
	//発光
	res = new RES(RES_T::PIXEL_SHADER, PATH_PS + L"LuminescenceNormalModel.pso");
	resourcesMap_.emplace(SRC::LUMINESCENCE_PS, res);

	//音声
	//暖炉の炎
	res = new RES(RES_T::SOUND, PATH_SND + L"Enviromental/fire.mp3");
	resourcesMap_.emplace(SRC::FIRE_SND, res);
	//足音：プレイヤー
	res = new RES(RES_T::SOUND, PATH_SND + L"Enviromental/footsteps_Player.mp3");
	resourcesMap_.emplace(SRC::FOOTSTEPS_PLAYER_SND, res);
	//足音：トラッカー
	res = new RES(RES_T::SOUND, PATH_SND + L"Enviromental/footsteps_Tracker.mp3");
	resourcesMap_.emplace(SRC::FOOTSTEPS_TRACKER_SND, res);
	//アイテム取得
	res = new RES(RES_T::SOUND, PATH_SND + L"Enviromental/GetItem.mp3");
	resourcesMap_.emplace(SRC::GET_ITEM_SND, res);
	//護符使用SE
	res = new RES(RES_T::SOUND, PATH_SND + L"Enviromental/UseAmuletSE.mp3");
	resourcesMap_.emplace(SRC::USE_AMULET_SND, res);
	//護符焼失SE
	res = new RES(RES_T::SOUND, PATH_SND + L"Enviromental/BurnAmulet.mp3");
	resourcesMap_.emplace(SRC::BURN_AMULET_SND, res);
	//ぬいぐるみ焼失SE
	res = new RES(RES_T::SOUND, PATH_SND + L"Enviromental/BurnDool.mp3");
	resourcesMap_.emplace(SRC::BURN_DOOL_SND, res);
	//ゴールSE
	res = new RES(RES_T::SOUND, PATH_SND + L"System/GoalSE.mp3");
	resourcesMap_.emplace(SRC::GOAL_SND, res);
	//ノイズSE
	res = new RES(RES_T::SOUND, PATH_SND + L"System/noise.mp3");
	resourcesMap_.emplace(SRC::NOISE_SND, res);
	//歪み壁SE
	res = new RES(RES_T::SOUND, PATH_SND + L"Enviromental/DisWallSE.mp3");
	resourcesMap_.emplace(SRC::DIS_WALL_SND, res);
	//歪み壁焼失SE
	res = new RES(RES_T::SOUND, PATH_SND + L"Enviromental/LostDisWallSE.mp3");
	resourcesMap_.emplace(SRC::DIS_WALL_LOST_SND, res);
	//カーソル移動SE
	res = new RES(RES_T::SOUND, PATH_SND + L"System/cursol_move.mp3");
	resourcesMap_.emplace(SRC::CURSOL_MOVE_SND, res);

	//タイトルBGM
	res = new RES(RES_T::SOUND, PATH_SND + L"BGM/Title.mp3");
	resourcesMap_.emplace(SRC::TITLE_BGM, res);
	//リザルトBGM
	res = new RES(RES_T::SOUND, PATH_SND + L"BGM/Result.mp3");
	resourcesMap_.emplace(SRC::RESULT_BGM, res);

}

void ResourceManager::Release(void)
{
	for (auto& p : loadedMap_)
	{
		p.second.Release();
	}

	loadedMap_.clear();
}

void ResourceManager::Destroy(void)
{
	Release();
	for (auto& res : resourcesMap_)
	{
		res.second->Release();
		delete res.second;
	}
	resourcesMap_.clear();
}

const Resource& ResourceManager::Load(SRC src)
{
	Resource& res = _Load(src);
	if (res.type_ == Resource::TYPE::NONE)
	{
		return dummy_;
	}
	return res;
}

int ResourceManager::LoadModelDuplicate(SRC src)
{
	Resource& res = _Load(src);
	if (res.type_ == Resource::TYPE::NONE)
	{
		return -1;
	}

	int duId = MV1DuplicateModel(res.handleId_);
	res.duplicateModelIds_.push_back(duId);

	return duId;
}

ResourceManager::ResourceManager(void)
{
	//初期化
	Init();
}

Resource& ResourceManager::_Load(SRC src)
{

	// ロード済みチェック
	const auto& lPair = loadedMap_.find(src);
	if (lPair != loadedMap_.end())
	{
		return *resourcesMap_.find(src)->second;
	}

	// リソース登録チェック
	const auto& rPair = resourcesMap_.find(src);
	if (rPair == resourcesMap_.end())
	{
		// 登録されていない
		return dummy_;
	}

	// ロード処理
	rPair->second->Load();

	// 念のためコピーコンストラクタ
	loadedMap_.emplace(src, *rPair->second);

	return *rPair->second;

}
