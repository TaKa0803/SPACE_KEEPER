#include"player.h"
#include<ImGuiManager.h>
#include<cmath>
#include"math_matrix.h"
#include<GameScene.h>



void Player::Initialize(const std::vector<Model*>& models) { 
	BaseCharacter::Initialize(models);
	input_ = Input::GetInstance();
	ammo = models_[0];


}


void Player::Attack() {
	//スペースキーでレティクルに弾発射
	if (input_->PushKey(DIK_SPACE)) {
		// 弾の速度
		const float kBulletSpeed = 4.0f;
		Vector3 velocity = Subtract(GetReticleMat(), GetplayermatTranslate());
		velocity = Multiply(kBulletSpeed, Normalize(velocity));
		// 弾を生成して初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(ammo, GetplayermatTranslate(), velocity);

		gameScene_->AddPlayerBullet(newBullet);
	}
}


void Player::Move() {
	
	Vector3 move = {0, 0, 0};

	//プレイヤー位置からレティクルの方向に移動
	Vector3 velo = Subtract(GetReticleMat(), GetplayermatTranslate());
	velo= Normalize(velo);
	move = Multiply(1.0f/2.0f, velo);
#ifdef _DEBUG
	ImGui::Text("player");
	ImGui::DragFloat3("position", &worldtransform_.translation_.x, 0.01f);
	ImGui::DragFloat3("rotate", &worldtransform_.rotation_.x, 0.01f);
	ImGui::DragFloat3("scale", &worldtransform_.scale_.x, 0.01f);

	ImGui::Text("Reticle");
	ImGui::Text("pos %4.1f/%4.1f", Reticle_->translation_.x, Reticle_->translation_.y);
#endif // _DEBUG
	//worldtransform_.translation_ = Add(worldtransform_.translation_,move);


	worldtransform_.UpdateMatrix();
}

/// <summary>
/// 更新
/// </summary>
void Player::Update() { 
#ifdef _DEBUG
	ImGui::Begin("Player");
#endif // _DEBUG

	
	Move();
	Attack();

#ifdef _DEBUG
	ImGui::End();
#endif // _DEBUG

	
}



/// <summary>
/// 描画
/// </summary>
void Player::Draw(const ViewProjection& view) { 
	//仮モデルの描画
	models_[0]->Draw(worldtransform_, view);
	
}