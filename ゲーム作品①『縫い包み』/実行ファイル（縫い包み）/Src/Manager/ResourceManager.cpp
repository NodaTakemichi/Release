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

	// �X�e�[�W���f��
	res = new RES(RES_T::MODEL, PATH_MDL + L"Stage/Stage.mv1");
	resourcesMap_.emplace(SRC::STAGE_MODEL, res);
	// �Փ˗p�X�e�[�W���f��
	res = new RES(RES_T::MODEL, PATH_MDL + L"Stage/HitStage.mv1");
	resourcesMap_.emplace(SRC::STAGE_COLL_MODEL, res);
	// �c�ݕǃ��f��
	res = new RES(RES_T::MODEL, PATH_MDL + L"Stage/GateMist.mv1");
	resourcesMap_.emplace(SRC::DISTORTION_WALL, res);
	// �^�C�g���p�X�e�[�W���f��
	res = new RES(RES_T::MODEL, PATH_MDL + L"Stage/TitleStage.mv1");
	resourcesMap_.emplace(SRC::TITLE_STAGE_MODEL, res);
	// �S�[�����f��
	res = new RES(RES_T::MODEL, PATH_MDL + L"Stage/GoolDoor.mv1");
	resourcesMap_.emplace(SRC::GOOL_DOOR_MODEL, res);
	// ���^���b�N�I�u�W�F�N�g���f��
	res = new RES(RES_T::MODEL, PATH_MDL + L"Stage/MetalicObj.mv1");
	resourcesMap_.emplace(SRC::METALLIC_MODEL, res);

	// �ǐՎ҃��f��
	res = new RES(RES_T::MODEL, PATH_MDL + L"Tracker/Tracker_Model.mv1");
	resourcesMap_.emplace(SRC::TRACKER_MODEL, res);
	// �l�`���f��
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
	//�아���f��
	res = new RES(RES_T::MODEL, PATH_MDL + L"Item/Amulet/Amulet.mv1");
	resourcesMap_.emplace(SRC::AMULET_MODEL, res);

	// �^�C�g���摜
	res = new RES(RES_T::IMG, PATH_IMG + L"UI/TitleImage.png");
	resourcesMap_.emplace(SRC::TITLE_IMG, res);
	// ���U���g�摜
	res = new RES(RES_T::IMG, PATH_IMG + L"UI/ResultImg.png");
	resourcesMap_.emplace(SRC::RESULT_IMG, res);
	// �Q�[���I�[�o�[�w�i
	res = new RES(RES_T::IMG, PATH_IMG + L"bg/GameOver.png");
	resourcesMap_.emplace(SRC::GAMEOVER_IMG, res);
	//�아�摜
	res = new RES(RES_T::IMG, PATH_IMG + L"Item/amulet.jpg");
	resourcesMap_.emplace(SRC::AMULET_IMG, res);
	//�n�C�g�}�b�v
	res = new RES(RES_T::IMG, PATH_IMG + L"Shader/HeightMap.png");
	resourcesMap_.emplace(SRC::HEIGHT_MAP_IMG, res);
	//�m�C�Y�摜
	res = new RES(RES_T::IMG, PATH_IMG + L"Shader/noise.png");
	resourcesMap_.emplace(SRC::NOISE_IMG, res);

	//�Ə��摜
	res = new RES(RES_T::IMG, PATH_IMG + L"UI/center.png");
	resourcesMap_.emplace(SRC::CENTER_UI_IMG, res);
	//�A�N�V�����e�L�X�g�{�b�N�X�摜
	res = new RES(RES_T::IMG, PATH_IMG + L"UI/TextBox.png");
	resourcesMap_.emplace(SRC::ACTION_TEXT_BOX_IMG, res);
	//�R�����g�e�L�X�g�{�b�N�X�摜
	res = new RES(RES_T::IMG, PATH_IMG + L"UI/TextBox_red.png");
	resourcesMap_.emplace(SRC::COMMENT_TEXT_BOX_IMG, res);
	//�아UI�摜
	res = new RES(RES_T::IMG, PATH_IMG + L"UI/AmuletUI.png");
	resourcesMap_.emplace(SRC::AMULET_UI_IMG, res);
	//�ʂ������UI�摜
	res = new RES(RES_T::IMG, PATH_IMG + L"UI/doolUI.png");
	resourcesMap_.emplace(SRC::DOOL_UI_IMG, res);
	//���C���Q�[�WUI�摜
	res = new RES(RES_T::IMG, PATH_IMG + L"UI/GaugeMain.png");
	resourcesMap_.emplace(SRC::GAUGE_MAIN_IMG, res);
	//�Q�[�W�t���[��UI�摜
	res = new RES(RES_T::IMG, PATH_IMG + L"UI/GaugeFrame.png");
	resourcesMap_.emplace(SRC::GAUGE_FRAME_IMG, res);
	//�Q�[�W�}�X�NUI�摜
	res = new RES(RES_T::IMG, PATH_IMG + L"UI/GaugeMask.png");
	resourcesMap_.emplace(SRC::GAUGE_MASK_IMG, res);
	//�|�[�Y���j���[�w�i�摜
	res = new RES(RES_T::IMG, PATH_IMG + L"UI/PauseBack.png");
	resourcesMap_.emplace(SRC::PAUSE_BACK_IMG, res);

	//����UI
	//�ړ�����FUI�摜
	res = new RES(RES_T::IMG, PATH_IMG + L"UI/operation_move.png");
	resourcesMap_.emplace(SRC::OPERATION__MOVE_IMG, res);
	//�_�b�V���FUI�摜
	res = new RES(RES_T::IMG, PATH_IMG + L"UI/operation_dash.png");
	resourcesMap_.emplace(SRC::OPERATION__DASH_IMG , res);
	//���N���b�N�FUI�摜
	res = new RES(RES_T::IMG, PATH_IMG + L"UI/operation_left_click.png");
	resourcesMap_.emplace(SRC::OPERATION__LEFT_CLICK_IMG, res);
	//�E�N���b�N�FUI�摜
	res = new RES(RES_T::IMG, PATH_IMG + L"UI/operation_right_click.png");
	resourcesMap_.emplace(SRC::OPERATION__RIGHT_CLICK_IMG, res);
	//�|�[�Y�FUI�摜
	res = new RES(RES_T::IMG, PATH_IMG + L"UI/operation_pause.png");
	resourcesMap_.emplace(SRC::OPERATION__PAUSE_IMG, res);

	//�t�H���g�f�[�^
	//�A�N�V�����e�L�X�g
	res = new RES(RES_T::FONT, PATH_FONT + L"NemukeMedium.otf");
	resourcesMap_.emplace(SRC::ACTION_TEXT_FONT, res);
	//�Z���N�g�R�}���h
	res = new RES(RES_T::FONT, PATH_FONT + L"EbiharaNoKuseji.ttf");
	resourcesMap_.emplace(SRC::SELECT_CMD_FONT, res);
	//���[�h�e�L�X�g
	res = new RES(RES_T::FONT, PATH_FONT + L"NemukeMedium.otf");
	resourcesMap_.emplace(SRC::LOAD_TEXT_FONT, res);

	//�G�t�F�N�g�f�[�^
	//�g�F�̉�
	res = new RES(RES_T::EFFEKSEER, PATH_EFF + L"Fire/fire.efkefc");
	resourcesMap_.emplace(SRC::FIRE_EFF, res);

	//���_�V�F�[�_�[
	//�@���}�b�v�t��
	res = new RES(RES_T::VERTEX_SHADER, PATH_VS + L"ModelNormal1FrameVertexShader.vso");
	resourcesMap_.emplace(SRC::NORMAL_MODEL_VS, res);

	//�s�N�Z���V�F�[�_�[
	//�ڂ���
	res = new RES(RES_T::PIXEL_SHADER, PATH_PS + L"Blur.pso");
	resourcesMap_.emplace(SRC::BLUR_PS, res);
	//�ċp�ʂ������
	res = new RES(RES_T::PIXEL_SHADER, PATH_PS + L"BurningDool.pso");
	resourcesMap_.emplace(SRC::BURNING_DOOL_PS, res);
	//�f�B�]���u�i�아�p�j
	res = new RES(RES_T::PIXEL_SHADER, PATH_PS + L"Dissolve.pso");
	resourcesMap_.emplace(SRC::DISSOLVE_PS, res);
	//�ċp�Q�[�W
	res = new RES(RES_T::PIXEL_SHADER, PATH_PS + L"BurnGauge.pso");
	resourcesMap_.emplace(SRC::BURN_GAUGE_PS, res);
	//�c�ݕ�
	res = new RES(RES_T::PIXEL_SHADER, PATH_PS + L"DistortionWall.pso");
	resourcesMap_.emplace(SRC::DISTORTION_WALL_PS, res);
	//�O���b�`
	res = new RES(RES_T::PIXEL_SHADER, PATH_PS + L"Glitch.pso");
	resourcesMap_.emplace(SRC::GLITCH_PS, res);
	//�m�[�}�����f��
	res = new RES(RES_T::PIXEL_SHADER, PATH_PS + L"ModelNormalPixelShader.pso");
	resourcesMap_.emplace(SRC::NORMAL_MODEL_PS, res);
	//�m�C�Y
	res = new RES(RES_T::PIXEL_SHADER, PATH_PS + L"Noise.pso");
	resourcesMap_.emplace(SRC::NOISE_PS, res);
	//�X�t�B�A�}�b�v
	res = new RES(RES_T::PIXEL_SHADER, PATH_PS + L"SphereMap.pso");
	resourcesMap_.emplace(SRC::SPHERE_MAP_PS, res);
	//�r�l�b�g
	res = new RES(RES_T::PIXEL_SHADER, PATH_PS + L"Vignette.pso");
	resourcesMap_.emplace(SRC::VIGNETTE_PS, res);
	//����
	res = new RES(RES_T::PIXEL_SHADER, PATH_PS + L"LuminescenceNormalModel.pso");
	resourcesMap_.emplace(SRC::LUMINESCENCE_PS, res);

	//����
	//�g�F�̉�
	res = new RES(RES_T::SOUND, PATH_SND + L"Enviromental/fire.mp3");
	resourcesMap_.emplace(SRC::FIRE_SND, res);
	//�����F�v���C���[
	res = new RES(RES_T::SOUND, PATH_SND + L"Enviromental/footsteps_Player.mp3");
	resourcesMap_.emplace(SRC::FOOTSTEPS_PLAYER_SND, res);
	//�����F�g���b�J�[
	res = new RES(RES_T::SOUND, PATH_SND + L"Enviromental/footsteps_Tracker.mp3");
	resourcesMap_.emplace(SRC::FOOTSTEPS_TRACKER_SND, res);
	//�A�C�e���擾
	res = new RES(RES_T::SOUND, PATH_SND + L"Enviromental/GetItem.mp3");
	resourcesMap_.emplace(SRC::GET_ITEM_SND, res);
	//�아�g�pSE
	res = new RES(RES_T::SOUND, PATH_SND + L"Enviromental/UseAmuletSE.mp3");
	resourcesMap_.emplace(SRC::USE_AMULET_SND, res);
	//�아�Ď�SE
	res = new RES(RES_T::SOUND, PATH_SND + L"Enviromental/BurnAmulet.mp3");
	resourcesMap_.emplace(SRC::BURN_AMULET_SND, res);
	//�ʂ�����ݏĎ�SE
	res = new RES(RES_T::SOUND, PATH_SND + L"Enviromental/BurnDool.mp3");
	resourcesMap_.emplace(SRC::BURN_DOOL_SND, res);
	//�S�[��SE
	res = new RES(RES_T::SOUND, PATH_SND + L"System/GoalSE.mp3");
	resourcesMap_.emplace(SRC::GOAL_SND, res);
	//�m�C�YSE
	res = new RES(RES_T::SOUND, PATH_SND + L"System/noise.mp3");
	resourcesMap_.emplace(SRC::NOISE_SND, res);
	//�c�ݕ�SE
	res = new RES(RES_T::SOUND, PATH_SND + L"Enviromental/DisWallSE.mp3");
	resourcesMap_.emplace(SRC::DIS_WALL_SND, res);
	//�c�ݕǏĎ�SE
	res = new RES(RES_T::SOUND, PATH_SND + L"Enviromental/LostDisWallSE.mp3");
	resourcesMap_.emplace(SRC::DIS_WALL_LOST_SND, res);
	//�J�[�\���ړ�SE
	res = new RES(RES_T::SOUND, PATH_SND + L"System/cursol_move.mp3");
	resourcesMap_.emplace(SRC::CURSOL_MOVE_SND, res);

	//�^�C�g��BGM
	res = new RES(RES_T::SOUND, PATH_SND + L"BGM/Title.mp3");
	resourcesMap_.emplace(SRC::TITLE_BGM, res);
	//���U���gBGM
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
	//������
	Init();
}

Resource& ResourceManager::_Load(SRC src)
{

	// ���[�h�ς݃`�F�b�N
	const auto& lPair = loadedMap_.find(src);
	if (lPair != loadedMap_.end())
	{
		return *resourcesMap_.find(src)->second;
	}

	// ���\�[�X�o�^�`�F�b�N
	const auto& rPair = resourcesMap_.find(src);
	if (rPair == resourcesMap_.end())
	{
		// �o�^����Ă��Ȃ�
		return dummy_;
	}

	// ���[�h����
	rPair->second->Load();

	// �O�̂��߃R�s�[�R���X�g���N�^
	loadedMap_.emplace(src, *rPair->second);

	return *rPair->second;

}
