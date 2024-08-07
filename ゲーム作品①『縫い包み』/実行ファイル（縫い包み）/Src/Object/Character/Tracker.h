#pragma once
#include <list>
#include <unordered_map>
#include "ActorBase.h"
class Capsule;
class Collider;
class AnimationController;
class SoundController;
class Player;

class Tracker :
    public ActorBase
{
public:

    struct MovePoint
    {
        int goledNum;   //�S�[��������
        VECTOR pos;     //���W
        bool warpPoint; //���[�v�|�C���g���ǂ���
    };

    // �A�j���[�V�������
    enum class ANIM_TYPE
    {
        IDLE,
        RUN,
        FAST_RUN,
        LOOKING,
        STUN,
        CHACH,
        LOST,
    };


    Tracker(const Player& player);
    virtual ~Tracker(void);
    void LoadData(void)override;
    void Init(void)override;
    void Update(void)override;
    void Draw(void)override;

    //�Q�[���I�[�o�[�̐ݒ�
    void SetGameOverInfo(const VECTOR& opponentPos)override;

    //�C���Ԃ��擾
    const bool GetFainting(void)const { return isFainting_; }

private:
    //�v���C���[���
    const Player& player_;

    //�A�j���[�V�����R���g���[���[
    std::unique_ptr<AnimationController> animController_;

    //�����T�E���h
    std::shared_ptr<SoundController>footstepsSound_;

    //�ړ��|�C���g
    std::vector<MovePoint>movePoints_;
    //�ړ��\�|�C���g�ԍ�
    std::vector<int>moveablePointNums_;
    //�ړ��|�C���g�ԍ�
    int currentMovePointNum_;

    //�S�[�����W
    VECTOR goalPos_ = {};
    //�C�⎞��
    float faintingTime_ = 0.0f;
    //�C�₵�Ă��邩���f
    bool isFainting_;

    //�ړ������̎擾
    VECTOR GetMoveDir(const VECTOR& goalPos);

    //�ړ��\�|�C���g������
    void InitMovePoint(void);
    //�ړ��\�|�C���g�̌���
    void SearchMoveablePoint(void);
    //�ړ��\�|�C���g����S�[��������
    void DecisionGoalPoint(void);

    //���[�v�\�����f
    bool IsWarping(void);
    //���[�v����
    void ProcessWarp(void);

    //�S�[������
    bool IsGoal(void);
    //�v���C���[�̔�������
    bool IsDiscoveryPlayer(void);
    //�v���C���[������������
    bool IsLoseSightOfPlayer(void);
    //�v���C���[�̔����G���A�`��i�f�o�b�N�p�j
    void DrawDiscoveryArea(void);
        
    void ProcessMove(void);     //�ړ�����
    void ProcessTrack(void);    //�ǐՏ���
    void ProcessRotatePortal(void);    //��]����(����)
    void ProcessRotateTrack(void);     //��]����(�ǐՒ�)

    //�X�V����
    void UpdateNormal(void);                 //�ʏ��Ԃ̍X�V
    void UpdatePatrol(void);                 //�����Ԃ̍X�V
    void UpdateTracking(void);               //�ǐՏ�Ԃ̍X�V
    void UpdateFainting(void);               //�C���Ԃ̍X�V
    void UpdateLostPlayer(void);             //���X�g��Ԃ̍X�V
    void UpdateGameOver(void)override;       //�Q�[���I�[�o�[��Ԃ̍X�V


};

