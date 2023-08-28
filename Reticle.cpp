#include"Reticle.h"
#include<ImGuiManager.h>
#include"math_matrix.h"



void Reticle::Initialize(Model* model, const WorldTransform& parent) {
	input_ = Input::GetInstance();
	
	reticleWorld_.Initialize();
	reticleWorld_.translation_ = NormalPos;
	//reticleWorld_.scale_ = {1, 1, 10};
	reticleWorld_.parent_ = &parent;

	model_ = model;
}



void Reticle::Move(float length) {
	float maxfar = 220;
	//+ni
	length = sqrtf(length * length);
	//gap
	float gap = length /maxfar;


	//キー入力による移動
	if (input_->PushKey(DIK_W)) {
		reticleWorld_.translation_.y += moveNum;
	}
	if (input_->PushKey(DIK_S)) {
		reticleWorld_.translation_.y -= moveNum;
	}
	if (input_->PushKey(DIK_A)) {
		reticleWorld_.translation_.x -= moveNum;
	}
	if (input_->PushKey(DIK_D)) {
		reticleWorld_.translation_.x += moveNum;
	}
	
	//max値
	Vector3 Max = {
		90.0f*gap,
		70.0f*gap
	};

	reticleWorld_.translation_.x = min(reticleWorld_.translation_.x, Max.x);
	reticleWorld_.translation_.x = max(reticleWorld_.translation_.x, -Max.x);
	reticleWorld_.translation_.y = min(reticleWorld_.translation_.y, Max.y);
	reticleWorld_.translation_.y = max(reticleWorld_.translation_.y, -Max.y);
	
}


void Reticle::Update(float depth) { 
	#ifdef _DEBUG
	ImGui::Begin("Reticle");
	ImGui::Text("pos :%4.1f/%4.1f/%4.1f", reticleWorld_.translation_.x, reticleWorld_.translation_.y,reticleWorld_.translation_.z);
	ImGui::Text("rotate :%4.1f/%4.1f/%4.1f", reticleWorld_.rotation_.x, reticleWorld_.rotation_.y,reticleWorld_.rotation_.z);
	ImGui::Text("scale :%4.1f/%4.1f/%4.1f", reticleWorld_.scale_.x, reticleWorld_.scale_.y,reticleWorld_.scale_.z);
	ImGui::End();
#endif // _DEBUG
	
	//移動処理
	Move(depth);
	
	//行列更新
	reticleWorld_.UpdateMatrix();

}

void Reticle::Draw(ViewProjection view) { 

	model_->Draw(reticleWorld_, view);

	
}