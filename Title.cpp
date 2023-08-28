#include<title.h>
#include<ImGuiManager.h>
void TitleS::Initialize(Model* model,Model*tile) { 
	model_ = model;
	tileM_ = tile;
	world_.Initialize();
	world_.translation_ = {0, 0, -300};
	tile_.Initialize();
	tile_.parent_ = &world_;
	tile_.translation_ = {0, 0, -0.6f};
	tile_.scale_ = {3.0f, 3.0f, 1};
}

void TitleS::Update() { 
	#ifdef _DEBUG
	ImGui::Begin("Title");
	ImGui::DragFloat3("pos", &world_.translation_.x, 0.01f);
	ImGui::DragFloat3("rotate", &world_.rotation_.x, 0.01f);
	ImGui::DragFloat3("scale", &world_.scale_.x, 0.01f);
	ImGui::Text("tile");
	ImGui::DragFloat3("tpos", &tile_.translation_.x, 0.01f);
	ImGui::DragFloat3("trotate", &tile_.rotation_.x, 0.01f);
	ImGui::DragFloat3("tscale", &tile_.scale_.x, 0.01f);
	ImGui::End();
#endif // _DEBUG
	switch (scene_) {
	case Scene::start:
		SetStart();
		break;
	case Scene::startAnimation:
		StartAnimation();
		break;
	case Scene::normal:
		Normal();
		break;
	case Scene::endAnimation:
		EndAnimation();
		break;
	default:
		break;
	}
	
	world_.UpdateMatrix();
	tile_.UpdateMatrix();
}

void TitleS::SetStart() {

}

void TitleS::StartAnimation() {

}

void TitleS::Normal() {

}

void TitleS::EndAnimation() {

}



void TitleS::OnCollision() {

}

void TitleS::Draw(ViewProjection view) { 
	model_->Draw(world_, view);
	tileM_->Draw(tile_, view);
}