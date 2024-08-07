#pragma once
#include "StageBase.h"
class ModelMaterial;
class ModelRenderer;
class WarpGate :
    public StageBase
{
public:
	WarpGate(void);
	virtual ~WarpGate(void);

	void LoadData(void)override;
	void Init(void)override;
	void Update(void)override;
	void Draw(void)override;

	//座標セット
	const void SetPos(const VECTOR& pos);
	//角度セット
	const void SetRot(const VECTOR& rot);

private:

	std::unique_ptr<ModelMaterial>modelMaterial_;	//モデルマテリアル
	std::unique_ptr<ModelRenderer>modelRenderer_;	//モデルレンダラー

};

