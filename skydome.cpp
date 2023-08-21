#include"skydome.h"

void Skydome::Initialize(Model* model, float farZ) { 
	model_ = model;
	world_.Initialize();
	farZ;
	world_.scale_ = {2, 2, 2};
}

void Skydome::Update() { world_.UpdateMatrix(); }

void Skydome::Draw(ViewProjection view) { model_->Draw(world_,view); }