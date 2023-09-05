#include"BaseCharacter.h"

void BaseCharacter::Initialize(const std::vector<Model*>& models,const uint32_t HP) {
	models_ = models;
	worldtransform_.Initialize();
	hp_ = HP;
	isDead_ = false;
}

void BaseCharacter::Update() { worldtransform_.UpdateMatrix(); }

void BaseCharacter::Draw(const ViewProjection& view) {
	for (Model* model : models_) {
		model->Draw(worldtransform_, view);
	}
}