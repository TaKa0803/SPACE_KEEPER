#include"Reticle.h"
#include<ImGuiManager.h>
#include"math_matrix.h"



void Reticle::Initialize(Model* model, const WorldTransform& parent) {
	input_ = Input::GetInstance();
	
	reticleWorld_.Initialize();
	reticleWorld_.translation_ = NormalPos;
	reticleWorld_.parent_ = &parent;

	model_ = model;
}



void Reticle::Move(float length) {

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

	
	//枠外に出てないかのチェック
	Vector2 zero = {0, 0};
	Vector2 retipos = {reticleWorld_.translation_.x, reticleWorld_.translation_.y};
	Vector2 overvelo = {0, 0};


	
	float checkArea = area * (length/200);

	SetAreaEllipse(zero, retipos,	checkArea,overvelo);

	// 伸ばした値を代入
	reticleWorld_.translation_ = {retipos.x, retipos.y, reticleWorld_.translation_.z};
}


void Reticle::Update(float length) { 
	#ifdef _DEBUG
	ImGui::Begin("Reticle");
	ImGui::Text("pos :%4.1f/%4.1f/%4.1f", reticleWorld_.translation_.x, reticleWorld_.translation_.y,reticleWorld_.translation_.z);
	ImGui::Text("rotate :%4.1f/%4.1f/%4.1f", reticleWorld_.rotation_.x, reticleWorld_.rotation_.y,reticleWorld_.rotation_.z);
	ImGui::Text("scale :%4.1f/%4.1f/%4.1f", reticleWorld_.scale_.x, reticleWorld_.scale_.y,reticleWorld_.scale_.z);
	ImGui::End();
#endif // _DEBUG
	
	reticleWorld_.translation_.z = length;

	//Back();
	//移動処理
	Move(length);
	//カメラ回転処理
	//CameraRotate(world_);
	
	//行列更新
	reticleWorld_.UpdateMatrix();

}

void Reticle::Draw(ViewProjection view) { 

	model_->Draw(reticleWorld_, view);

	
}