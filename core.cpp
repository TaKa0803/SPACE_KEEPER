#include<core.h>
#include<cassert>
#include<ImGuiManager.h>
#include"math_matrix.h"

void BCore::Initialize(const std::vector<Model*>& models) {
	//assert(model);
	BaseCharacter::Initialize(models, 1000);
	
	worldtransform_.translation_ = {0, 0, 0};
	worldtransform_.scale_ = {5, 5, 5};


	body_.Initialize();
	head_.Initialize();
	leg_.Initialize();
	LArm1_.Initialize();
	LArm2_.Initialize();
	Lhand_.Initialize();

	RArm1_.Initialize();
	RArm2_.Initialize();
	Rhand_.Initialize();

	C_center_.Initialize();
	C_L1_.Initialize();
	C_L2_.Initialize();

	C_R1_.Initialize();
	C_R2_.Initialize();

	//親子関係

	body_.parent_ = &worldtransform_;
	head_.parent_ = &body_;
	C_center_.parent_ = &body_;
	leg_.parent_ = &C_center_;

	C_L1_.parent_ = &body_;
	LArm1_.parent_ = &C_L1_;
	C_L2_.parent_ = &C_L1_;
	LArm2_.parent_ = &C_L2_;
	Lhand_.parent_ = &C_L2_;

	C_R1_.parent_ = &body_;
	RArm1_.parent_ = &C_R1_;
	C_R2_.parent_ = &C_R1_;
	RArm2_.parent_ = &C_R2_;
	Rhand_.parent_ = &C_R2_;


	head_.translation_ = {0, 1.6f, 0};
	C_center_.translation_ = {0, -3, 0};
	leg_.translation_ = {0, -1.2f, 0};

	LArm1_.translation_ = {-1.3f, 0, 0};
	LArm2_.translation_ = {-1.3f, 0, 0};
	Lhand_.translation_ = {-7, 0, 0};

	C_L1_.translation_ = {-3.0f, 0, 0};
	C_L2_.translation_ = {-7.0f, 0, 0};

	RArm1_.translation_ = {1.3f, 0, 0};
	RArm2_.translation_ = {1.3f, 0, 0};
	Rhand_.translation_ = {7, 0, 0};

	C_R1_.translation_ = {3.0f, 0, 0};
	C_R2_.translation_ = {7.0f, 0, 0};
}

//プレイヤーのほうを向く
void BCore::SeeTarget() { 
	Vector3 velo = Subtract(GetTargetW(), GetmatW());
	Vector2 v2 = {velo.x, velo.z};
	worldtransform_.rotation_.y = CheckR_F_Y(v2);
}


void BCore::Update() { 
	SeeTarget();

	#ifdef _DEBUG
	ImGui::Begin("Core");
	ImGui::DragFloat3("pos", &worldtransform_.translation_.x, 0.01f);
	ImGui::DragFloat3("rotate", &worldtransform_.rotation_.x, 0.01f);
	ImGui::DragFloat3("Scale", &worldtransform_.scale_.x, 0.01f);
	ImGui::Text("HP/%d :isdead/%d ", hp_, isDead_);




	ImGui::DragFloat3("p body", &body_.rotation_.x, 0.01f);
	ImGui::DragFloat3("p head", &head_.rotation_.x, 0.01f);

	ImGui::DragFloat3("p core", &C_center_.rotation_.x, 0.01f);

	ImGui::DragFloat3("p leg", &leg_.rotation_.x, 0.01f);

	/*
	ImGui::DragFloat3("p LA1", &LArm1_.rotation_.x, 0.01f);
	ImGui::DragFloat3("p LA2", &LArm2_.rotation_.x, 0.01f);
	ImGui::DragFloat3("p Lhand", &Lhand_.rotation_.x, 0.01f);

	ImGui::DragFloat3("p RA1", &RArm1_.rotation_.x, 0.01f);
	ImGui::DragFloat3("p RA2", &RArm2_.rotation_.x, 0.01f);
	ImGui::DragFloat3("p Rhand", &Rhand_.rotation_.x, 0.01f);
	*/

	ImGui::DragFloat3("p CL1", &C_L1_.rotation_.x, 0.01f);
	ImGui::DragFloat3("p CL2", &C_L2_.rotation_.x, 0.01f);

	ImGui::DragFloat3("p CR1", &C_R1_.rotation_.x, 0.01f);
	ImGui::DragFloat3("p CR2", &C_R2_.rotation_.x, 0.01f);

	ImGui::End();
#endif // _DEBUG



	//world_.rotation_.y += (1.0f / 120.0f) * 3.14f;

	//world_.UpdateMatrix();

	UpdateAllMatrix();
}

void BCore::UpdateAllMatrix() { 
	worldtransform_.UpdateMatrix();

	body_.UpdateMatrix();
	head_.UpdateMatrix();
	C_center_.UpdateMatrix();
	leg_.UpdateMatrix();

	
	C_L1_.UpdateMatrix();
	LArm1_.UpdateMatrix();
	C_L2_.UpdateMatrix();
	LArm2_.UpdateMatrix();
	Lhand_.UpdateMatrix();
	
	C_R1_.UpdateMatrix();
	RArm1_.UpdateMatrix();
	C_R2_.UpdateMatrix();
	RArm2_.UpdateMatrix();
	Rhand_.UpdateMatrix();
	
}

void BCore::InCollision() { 
	hp_--;
	if (hp_ <= 0) {
		isDead_ = true;
	}
}

void BCore::Draw(const ViewProjection& view) { 
	//models_[0]->Draw(world_, view);
	models_[0]->Draw(head_, view);
	models_[1]->Draw(body_, view);
	models_[2]->Draw(leg_, view);

	models_[3]->Draw(LArm1_, view);
	models_[4]->Draw(LArm2_, view);
	models_[5]->Draw(Lhand_, view);

	models_[6]->Draw(RArm1_, view);
	models_[7]->Draw(RArm2_, view);
	models_[8]->Draw(Rhand_, view);

	
	//コア
	if (weakC == WeakCore::center_) {
		models_[10]->Draw(C_center_, view);
	} else {
		models_[9]->Draw(C_center_, view);
	}
	
	if (weakC == WeakCore::L1) {
		models_[10]->Draw(C_L1_, view);
	} else {
		models_[9]->Draw(C_L1_, view);
	}
	
	if (weakC == WeakCore::L2) {
		models_[10]->Draw(C_L2_, view);
	} else {
		models_[9]->Draw(C_L2_, view);
	}
	if (weakC == WeakCore::R1) {
		models_[10]->Draw(C_R1_, view);
	} else {
		models_[9]->Draw(C_R1_, view);
	}
	if (weakC == WeakCore::R2) {
		models_[10]->Draw(C_R2_, view);
	} else {
		models_[9]->Draw(C_R2_, view);
	}
	
}