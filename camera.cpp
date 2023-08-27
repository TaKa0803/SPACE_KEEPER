#include"camera.h"
#include"math_matrix.h"
#include<ImGuiManager.h>

void Camera::Initialize(float farZ, const WorldTransform* target) { 

	
	view_.farZ = farZ*2.0f+500.0f;
	view_.Initialize();
	
	
	cameraWorld_.Initialize();
	// 注目アイテムと距離設定
	cameraWorld_.translation_ = {0, 3, -20};

	//reticle_ =new Reticle();
	//reticle_->Initialize(model, world_);
	//ターゲット設定
	cameraWorld_.parent_ = target;
}

void Camera::Update() { 
#ifdef _DEBUG
	ImGui::Begin("MainCamera");
#endif // _DEBUG

#ifdef _DEBUG
	ImGui::Text("main camera");
	ImGui::DragFloat3("pos", &cameraWorld_.translation_.x, 0.01f);
	ImGui::Text("rotate : %4.1f/%4.1f/%4.1f", &cameraWorld_.rotation_.x, 0.01f);
	ImGui::DragFloat3("scale", &cameraWorld_.scale_.x, 0.01f);
	ImGui::DragFloat("far", &cameraWorld_.translation_.z);
#endif // _DEBUG

	//行列更新
	cameraWorld_.UpdateMatrix();
	view_.matView = Inverse(cameraWorld_.matWorld_);
	view_.TransferMatrix();

#ifdef _DEBUG
	ImGui::End();
#endif // _DEBUG
}

