#include"Enemy.h"
#include<ImGuiManager.h>
void Enemy::Initialize(const std::vector<Model*>& models, const uint32_t HP, Vector3 pos) {
	BaseCharacter::Initialize(models, HP);
	worldtransform_.translation_ = pos;
}

void Enemy::Update() { 
	#ifdef _DEBUG
	ImGui::Begin("Enemy");
	ImGui::Text("pos:HP:isDead/%4.1f,%4.1f,%4.1f", worldtransform_.translation_.x,
	    worldtransform_.translation_.y, worldtransform_.translation_.z);
	ImGui::Text("HP:isDead/%d:%d", hp_, isDead_);
	ImGui::End();
#endif // _DEBUG



	worldtransform_.UpdateMatrix();
}

void Enemy::Draw(const ViewProjection& view) { 
	//
	models_[0]->Draw(worldtransform_, view);
}

void Enemy::InCollision() {
	if (hp_-- <= 0) {
		isDead_ = true;
	}
}