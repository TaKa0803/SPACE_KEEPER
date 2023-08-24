#include"camera.h"
#include"math_matrix.h"
#include<ImGuiManager.h>

void Camera::Initialize(Model*model,float farZ) { 

	input_ = Input::GetInstance();

	world_.Initialize();
	
	view_.farZ = farZ*2.0f+500.0f;
	view_.Initialize();
	
	
	cameraWorld_.Initialize();
	// 注目アイテムと距離設定
	cameraWorld_.translation_ = {0, 10, -50};

	reticle_ =new Reticle();
	reticle_->Initialize(model, world_);
}

void Camera::Update() { 
	#ifdef _DEBUG
	ImGui::Begin("MainCamera");
#endif // _DEBUG

	//ターゲットから座標を取得した行列を作る
	world_.translation_ = target_->translation_;
	
	//レティクル更新
	reticle_->Update(world_);
	
#ifdef _DEBUG
	ImGui::Text("camera");
	ImGui::DragFloat3("position", &world_.translation_.x, 0.01f);
	ImGui::DragFloat3("rotation",&world_.rotation_.x,0.01f);
	ImGui::DragFloat("far",&cameraWorld_.translation_.z, 0.01f);
#endif // _DEBUG


	//行列更新
	world_.UpdateMatrix();
	cameraWorld_.UpdateMatrix();
	cameraWorld_.matWorld_ = Multiply(cameraWorld_.matWorld_, world_.matWorld_);
	view_.matView = Inverse(cameraWorld_.matWorld_);
	view_.TransferMatrix();

#ifdef _DEBUG
	ImGui::End();
#endif // _DEBUG
}

void Camera::Draw(ViewProjection view) { reticle_->Draw(view); }