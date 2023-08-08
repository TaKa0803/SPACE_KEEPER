#include"player.h"

void Player::Initialize(const std::vector<Model*>& models) { 
	BaseCharacter::Initialize(models);
	input_ = Input::GetInstance();
}

/// <summary>
/// 更新
/// </summary>
void Player::Update() { 
	if (input_->PushKey(DIK_W)) {
		worldtransform_.translation_.y += 0.5;
	}
	if (input_->PushKey(DIK_S)) {
		worldtransform_.translation_.y -= 0.5;
	}
	if (input_->PushKey(DIK_A)) {
		worldtransform_.translation_.x -= 0.5;
	}
	if (input_->PushKey(DIK_D)) {
		worldtransform_.translation_.x += 0.5;
	}
	worldtransform_.UpdateMatrix();
}
/// <summary>
/// 描画
/// </summary>
void Player::Draw(const ViewProjection& view) { 
	//仮モデルの描画
	models_[0]->Draw(worldtransform_, view);
}