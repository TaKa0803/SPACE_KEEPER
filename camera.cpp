#include"camera.h"
#include"math_matrix.h"

void Camera::Initialize(const ViewProjection& world) { 

	input_ = Input::GetInstance();

	world_.Initialize();
	

	view_.Initialize();
	view_.farZ = world.farZ;

	cameraWorld_.Initialize();
	// 注目アイテムと距離設定
	cameraWorld_.translation_ = {0, 0, -50};
}

void Camera::Update() { 
	//ターゲットから座標を取得した行列を作る
	
	world_.translation_ = target_->translation_;
	
	//回転軸の処理
	float kRotateTheta = 1.0f / 60.0f;
	if (input_->PushKey(DIK_LEFT)) {
		world_.rotation_.y += kRotateTheta;

	}
	if (input_->PushKey(DIK_RIGHT)) {
		world_.rotation_.y -= kRotateTheta;
	}
	
	world_.UpdateMatrix();

	cameraWorld_.UpdateMatrix();
	cameraWorld_.matWorld_ = Multiply(cameraWorld_.matWorld_, world_.matWorld_);

	view_.matView = Inverse(cameraWorld_.matWorld_);
	view_.TransferMatrix();
}