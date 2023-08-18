#include"skydome.h"

void Skydome::Initialize(Model* model, float farZ) { 
	model_ = model;
	world_.Initialize();
	world_.scale_ = {farZ, farZ, farZ};
}

void Skydome::Update() { world_.UpdateMatrix(); }

void Skydome::Draw(ViewProjection view) { model_->Draw(world_,view); }