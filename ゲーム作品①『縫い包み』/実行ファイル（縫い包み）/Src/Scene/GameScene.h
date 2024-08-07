#pragma once
#include<memory>
#include<vector>
#include "SceneBase.h"
class Stage;
class GoalGate;
class Player;
class Tracker;
class ItemBase;
class PlayerUI;
class OperationUI;
class GoalDoor;
class WarpGate;

class PostEffectMaterial;
class PostEffectRenderer;

class GameScene :
	public SceneBase
{
public:

	GameScene(SceneManager& manager);
	~GameScene(void);

	virtual void LoadData(void)override;	
	virtual void Init(void) override;
	virtual void Update(InputManager& input) override;
	virtual void Draw(void) override;

private:
	// �X�e�[�W
	std::unique_ptr<Stage> stage_;
	// �S�[���Q�[�g
	std::shared_ptr<GoalGate> goalGate_;
	// �S�[���h�A
	std::unique_ptr<GoalDoor> goalDoor_;
	// �v���C���[(�L�����N�^�[)
	std::unique_ptr<Player> player_;
	// �H��(�L�����N�^�[)
	std::unique_ptr<Tracker> tracker_;
	// �A�C�e��
	std::vector<std::shared_ptr<ItemBase>> items_;
	// ���[�v�Q�[�g
	std::vector<std::shared_ptr<WarpGate>> warpGates_;

	// �v���C���[UI
	std::shared_ptr<PlayerUI> playerUI_;
	// ����UI
	std::shared_ptr<OperationUI> operationUI_;

	//�Q�[���I�[�o�[����	true:�Q�[���I�[�o�[/false:�Q�[���I�[�o�[�ł͂Ȃ�
	bool isGameOver_=false;
	//�Q�[���I�[�o�[����
	float gameOverTime_ = 0.0f;

	//BGM
	std::shared_ptr<SoundController>goalSE_;

	//�|�X�g�G�t�F�N�g�̏�����
	void InitPE(void);

	//�A�C�e���̐���
	void CreateItems(void);
	//�A�C�e���̃��[�h
	void LoadItem(void);

	//Update�֐�
	void LoadingUpdate(InputManager& input) ;
	void NormalUpdate(InputManager& input) ;
	//�`��֐�
	void LoadingDraw(void) ;
	void NormalDraw(void) ;

	//�Q�[���I�[�o�[����
	bool CheckGameOver(void);


};

