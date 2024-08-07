#pragma once
#include "../ObjBase.h"
class ItemBase :
    public ObjBase
{
public:
    enum class ITEM_TYPE {
        NONE,
        RABBIT,
        PIG,
        MONKEY,
        SHEEP,
        AMULET
    };

    ItemBase(void);
    virtual ~ItemBase(void);
    virtual void LoadData(void) = 0;
    virtual void Init(void) = 0;
    virtual void Update(void) = 0;
    virtual void Draw(void) = 0;

    /// <summary>
    /// �A�C�e���I�u�W�F�N�g�̏��ݒ�
    /// </summary>
    /// <param name="type">�A�C�e���^�C�v</param>
    /// <param name="scl">�I�u�W�F�N�g�̑傫��</param>
    /// <param name="pos">�I�u�W�F�N�g�̈ʒu</param>
    /// <param name="rot">�I�u�W�F�N�g�̊p�x</param>
    /// <returns></returns>
    const void SetModelInfo(
        const ITEM_TYPE& type,
        const VECTOR& scl,
        const VECTOR& pos,
        const VECTOR& rot);

    const bool GetHasItem(void)const { return hasItem_; }
    const void SetHasItem(const bool& hasItem) { hasItem_ = hasItem; }
    const bool IsAlive(void)const { return isAlive_; }
    const void SetIsAlive(const bool& alive) { isAlive_ = alive; }
    const ITEM_TYPE GetItemType(void)const { return type_; }

    //�Ď����o�̊J�n
    void StartBurnDirection(void);

protected:
    bool hasItem_;      //�擾����Ă��邩�ǂ���
    bool isAlive_;      //�R�₳�ꂽ���ǂ���

    //�A�C�e�����
    ITEM_TYPE type_;
    //�ċp���o���f
    bool isBurnDirection_;


private:
    //���f���̑I��
    int SelectModelHandle(void);

};

