#include<core.h>
#include<cassert>
#include<ImGuiManager.h>
void Core::Initialize(Model* model) {
	assert(model);
	model_ = model;
	

	world_.Initialize();
	world_.translation_ = {0, 0, 0};
	world_.scale_ = {50, 50, 50};

	isdead_ = false;
	hp_ = 1000;
}

void Core::Update() { 
	#ifdef _DEBUG
	ImGui::Begin("Core");
	ImGui::DragFloat3("pos", &world_.translation_.x, 0.01f);
	ImGui::DragFloat3("rotate", &world_.rotation_.x, 0.01f);
	ImGui::DragFloat3("Scale" ,&world_.scale_.x, 0.01f);
	ImGui::Text("HP/%d :isdead/%d ", hp_, isdead_);
	ImGui::End();
#endif // _DEBUG



	world_.rotation_.y += (1.0f / 120.0f) * 3.14f;

	world_.UpdateMatrix();
}

void Core::InCollision() { 
	hp_--;
	if (hp_ <= 0) {
		isdead_ = true;
	}
}

void Core::Draw(ViewProjection view) { model_->Draw(world_, view); }