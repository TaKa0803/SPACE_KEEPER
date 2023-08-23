#include "math_matrix.h"
#include <PlayerBullet.h>
#include <cassert>
void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	assert(model);
	model_ = model;
	//textureHandle_ = TextureManager::Load("Green.png");
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.scale_ = {1.5f, 1.5f, 1.5f};
	velocity_ = velocity;
}

void PlayerBullet::Update() {
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);

	// 時間経過でデス
	if (--deathTimer_ <= 0) {
		isdead_ = true;
	}

	worldTransform_.UpdateMatrix();
}

void PlayerBullet::Draw(ViewProjection& view) {

	model_->Draw(worldTransform_, view);
}