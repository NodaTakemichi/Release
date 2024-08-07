#pragma once
#include"StageBase.h"
class ModelMaterial;
class ModelRenderer;
class SoundController;

class GoalGate : public StageBase
{
public:
	GoalGate(void);
	virtual ~GoalGate(void);
	void LoadData(void)override;
	void Init(void)override;
	void Update(void)override;
	void Draw(void)override;

	//ƒS[ƒ‹ŠJ•ú
	void SetOpenGate(void);

private:
	float dissolveTime_;
	bool openGate_ ;

	std::unique_ptr<ModelMaterial>modelMaterial_;
	std::unique_ptr<ModelRenderer>modelRenderer_;

	//˜c‚İSE
	std::shared_ptr< SoundController>disSound_;
	//Á¸SE
	std::shared_ptr< SoundController>loastWallSound_;

};
