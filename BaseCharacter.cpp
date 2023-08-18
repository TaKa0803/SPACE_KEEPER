#include"BaseCharacter.h"

void BaseCharacter::Initialize(const std::vector<Model*>& models) {
	models_ = models;
	worldtransform_.Initialize();
}

void BaseCharacter::Update() { worldtransform_.UpdateMatrix(); }

void BaseCharacter::Draw(const ViewProjection& view) {
	for (Model* model : models_) {
		model->Draw(worldtransform_, view);
	}
}