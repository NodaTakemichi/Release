#pragma once
#include "ItemBase.h"
class ModelMaterial;
class ModelRenderer;
class SoundController;

class Dool :
    public ItemBase
{
public:
    Dool(void);
    virtual ~Dool(void);
    void LoadData(void)override;
    void Init(void)override;
    void Update(void)override;
    void Draw(void)override;


private:
  
    std::unique_ptr<ModelMaterial>modelMaterial_;
    std::unique_ptr<ModelRenderer>modelRenderer_;

    //�Ď��o�ߎ���
    float burnTime_=0.0f;

    //�Ď�SE
    std::shared_ptr< SoundController>burnDoolSound_;

};

