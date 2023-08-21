#include"camera.h"
#include"math_matrix.h"
#include<ImGuiManager.h>

void Camera::Initialize(const ViewProjection& world,Model*model) { 

	input_ = Input::GetInstance();

	world_.Initialize();
	
	view_.farZ = 2000;
	view_.Initialize();
	
	world;
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
	// 距離によって向く量変更
	float X = reticleWorld_.translation_.x / num;
	float Y = reticleWorld_.translation_.y / num;

	//
	X = sqrtf(X * X);
	Y = sqrtf(Y * Y);

	//カメラ方向処理
	
	if (reticleWorld_.translation_.y > 0) {
		world_.rotation_.x -= kRotateTheta * Y;
	} else {
		world_.rotation_.x += kRotateTheta * Y;
	}
	
	//一周超えたら数値を下げる（叔母風呂対策
	if (world_.rotation_.x > pi * 2.0f) {
		world_.rotation_.x -= pi * 2.0f;
	} else if (world_.rotation_.x < -(pi * 2.0f)) {
		world_.rotation_.x += pi * 2.0f;
	}

	//プラス方向
	if (world_.rotation_.x > pi * (1.0f / 2.0f) && world_.rotation_.x < pi* (3.0f / 2.0f)) {
		if (reticleWorld_.translation_.x > 0) {
			world_.rotation_.y -= kRotateTheta * X;
		} else {
			world_.rotation_.y += kRotateTheta * X;
		}
	} else if(world_.rotation_.x>0.0f){
		if (reticleWorld_.translation_.x > 0) {
			world_.rotation_.y += kRotateTheta * X;
		} else {
			world_.rotation_.y -= kRotateTheta * X;
		}
	} 
	
	if(world_.rotation_.x <-( pi * (1.0f / 2.0f) )&& world_.rotation_.x >-( pi * (3.0f / 2.0f))) {
		if (reticleWorld_.translation_.x > 0) {
			world_.rotation_.y -= kRotateTheta * X;
		} else {
			world_.rotation_.y += kRotateTheta * X;
		}
	} else if(world_.rotation_.x<0.0f){
		if (reticleWorld_.translation_.x > 0) {
			world_.rotation_.y += kRotateTheta * X;
		} else {
			world_.rotation_.y -= kRotateTheta * X;
		}
	}


	// 一周超えたら数値を下げる（叔母風呂対策
	if (world_.rotation_.y > pi * 2.0f) {
		world_.rotation_.y -= pi * 2.0f;
	} else if (world_.rotation_.y < -(pi * 2.0f)) {
		world_.rotation_.y += pi * 2.0f;
	}
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