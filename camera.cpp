#include"camera.h"
#include"math_matrix.h"
#include<ImGuiManager.h>

void Camera::Initialize(const ViewProjection& world,Model*model) { 

	input_ = Input::GetInstance();

	world_.Initialize();
	

	view_.Initialize();
	view_.farZ = world.farZ*2.0f;

	cameraWorld_.Initialize();
	// 注目アイテムと距離設定
	cameraWorld_.translation_ = {0, 0, -50};

	reticleWorld_.Initialize();
	reticleWorld_.translation_ = {0, 0, 30};
	reticleWorld_.parent_ = &world_;

	model_ = model;
}







void Camera::cameraRotate(float num) { 
	Vector2 zero = {0, 0};
	Vector2 retipos = {reticleWorld_.translation_.x, reticleWorld_.translation_.y};
	Vector2 velo = {retipos.x - zero.x, retipos.y - zero.y};
	float length = sqrtf(velo.x*velo.x+velo.y*velo.y);

	//枠外にレティクルが出ていたら
	if (length > num) {
		Vector2 newpos = {
		    num*(velo.x / length),
		    num*(velo.y / length),
		};
		reticleWorld_.translation_ = {newpos.x, newpos.y, reticleWorld_.translation_.z};
	};
	

	/*
	if (reticleWorld_.translation_.x > num) {
		reticleWorld_.translation_.x = num;
		world_.rotation_.y += kRotateTheta;

	} else if (reticleWorld_.translation_.x < -num) {
		reticleWorld_.translation_.x = -num;
		world_.rotation_.y -= kRotateTheta;
	}

	if (reticleWorld_.translation_.y > num) {
		reticleWorld_.translation_.y = num;
		world_.rotation_.x -= kRotateTheta;
	
	} else if (reticleWorld_.translation_.y < -num) {
		reticleWorld_.translation_.y = -num;
		world_.rotation_.x += kRotateTheta;
	}
	*/
}

void Camera::Update() { 
	//ターゲットから座標を取得した行列を作る
	
	world_.translation_ = target_->translation_;
	
	// キー入力のないときに０地点に戻る
	if (!input_->PushKey(DIK_W) && !input_->PushKey(DIK_A) && !input_->PushKey(DIK_S) &&
	    !input_->PushKey(DIK_D)) {
		reticleWorld_.translation_ = {0, 0, 30};
	}

	if (input_->PushKey(DIK_W)) {
		reticleWorld_.translation_.y += 1.0f;
	}
	if (input_->PushKey(DIK_S)) {
		reticleWorld_.translation_.y -= 1.0f;
	}
	if (input_->PushKey(DIK_A)) {
		reticleWorld_.translation_.x -= 1.0f;
	}
	if (input_->PushKey(DIK_D)) {
		reticleWorld_.translation_.x += 1.0f;
	}

#ifdef _DEBUG
	ImGui::Begin("reticle");
	ImGui::DragFloat3("pos", &reticleWorld_.translation_.x, 0.01f);
	ImGui::DragFloat3("rotate", &reticleWorld_.rotation_.x, 0.01f);
	ImGui::DragFloat3("scale", &reticleWorld_.scale_.x, 0.01f);
	ImGui::End();
#endif // _DEBUG
	float area = 30;
	// エリア超えていたらカメラも回転
	cameraRotate(area);
	
	reticleWorld_.UpdateMatrix();



#ifdef _DEBUG
	ImGui::Begin("camera");
	ImGui::DragFloat3("rotation",&world_.rotation_.x,0.01f);
	ImGui::DragFloat("far",&cameraWorld_.translation_.z, 0.01f);
	ImGui::End();
#endif // _DEBUG



	world_.UpdateMatrix();

	cameraWorld_.UpdateMatrix();
	cameraWorld_.matWorld_ = Multiply(cameraWorld_.matWorld_, world_.matWorld_);

	view_.matView = Inverse(cameraWorld_.matWorld_);
	view_.TransferMatrix();
}

void Camera::Draw(ViewProjection view) { model_->Draw(reticleWorld_, view); }