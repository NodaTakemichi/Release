#pragma once
#include <map>
#include <string>
#include "../Utility/StaticSingleton.h"
#include "Resource.h"

class ResourceManager:
	public StaticSingleton<ResourceManager>
{

public:

	// ���\�[�X��
	enum class SRC
	{
		//3D���f��
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

		//�摜
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
		//����r���{�[�h
		OPERATION__MOVE_IMG,
		OPERATION__DASH_IMG,
		OPERATION__LEFT_CLICK_IMG,
		OPERATION__RIGHT_CLICK_IMG,
		OPERATION__PAUSE_IMG,

		//�t�H���g�f�[�^
		ACTION_TEXT_FONT,
		SELECT_CMD_FONT,
		LOAD_TEXT_FONT,

		//�G�t�F�N�g
		FIRE_EFF,

		//���_�V�F�[�_�[
		NORMAL_MODEL_VS,	//�ʏ탂�f��

		//�s�N�Z���V�F�[�_�[
		BLUR_PS,			//�ڂ���
		BURNING_DOOL_PS,	// �ċp�ʂ������
		DISSOLVE_PS,		// �f�B�]���u�i�아�p�j
		BURN_GAUGE_PS,		// �ċp�Q�[�W
		DISTORTION_WALL_PS,	// �c�ݕ�
		GLITCH_PS,			// �O���b�`
		NORMAL_MODEL_PS,	// �m�[�}�����f��
		NOISE_PS,			// �m�C�Y
		SPHERE_MAP_PS,		// �X�t�B�A�}�b�v
		VIGNETTE_PS,		// �r�l�b�g
		LUMINESCENCE_PS,	// �������f��
	
		//����
		FIRE_SND,			//�g�F�̉�
		FOOTSTEPS_PLAYER_SND,		//�����F�v���C���[
		FOOTSTEPS_TRACKER_SND,		//�����F�g���b�J�[
		GET_ITEM_SND,		//�A�C�e���擾��
		USE_AMULET_SND,		//�아�g�pSE
		BURN_AMULET_SND,	//�아�Ď�SE
		BURN_DOOL_SND,		//�ʂ�����ݏĎ�SE
		GOAL_SND,			//�S�[��SE
		NOISE_SND,			//�m�C�YSE
		DIS_WALL_SND,		//�c�ݕ�SE
		DIS_WALL_LOST_SND,	//�c�ݕǏ���SE
		CURSOL_MOVE_SND,	//�J�[�\���ړ�SE

		TITLE_BGM,			//�^�C�g��BGM
		RESULT_BGM,			//���U���gBGM
	};


	// ������
	void Init(void);

	// ���(�V�[���ؑ֎��Ɉ�U���)
	void Release(void);

	// ���\�[�X�̊��S�j��
	void Destroy(void);

	// ���\�[�X�̃��[�h
	const Resource& Load(SRC src);

	// ���\�[�X�̕������[�h(���f���p)
	int LoadModelDuplicate(SRC src);

private:
	//�V���O���g���̐���
	THIS_CLASS_IS_STATIC_SINGLETON(ResourceManager);

	// ���\�[�X�Ǘ��̑Ώ�
	std::map<SRC, Resource*> resourcesMap_;

	// �ǂݍ��ݍς݃��\�[�X
	std::map<SRC, Resource&> loadedMap_;

	Resource dummy_;

	// �f�t�H���g�R���X�g���N�^��private�ɂ��āA
	// �O�����琶���ł��Ȃ��l�ɂ���
	ResourceManager(void);
	~ResourceManager(void) = default;

	// �������[�h
	Resource& _Load(SRC src);

};
