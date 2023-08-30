#include<title.h>
#include<ImGuiManager.h>
#include"math_matrix.h"
#include<numbers>
void TitleS::Initialize(Model* model,Model*tile) { 
	model_ = model;
	tileM_ = tile;
	world_.Initialize();
	world_.translation_ = {0, 20, -300};
	world_.scale_ = {4, 4, 1};

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
	ImGui::Text("count/max : %d/%d", hitCount_, maxHitNum);
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
	world_.translation_ = startpos;
	velo_ = zero;
	//acce_ = velo_;
	isPlay_ = false;
	hitCount_ = 0;
	floatT = 0;
	scene_ = Scene::startAnimation;
	anime_ = NormalAnime::Normal;
}

void TitleS::StartAnimation() { 
	
	velo_.y -= addAcce_;

	world_.translation_ = Add(world_.translation_, velo_);

	if (world_.translation_.y <= NormalY) {
		world_.translation_.y = NormalY;
		velo_ = zero;
		scene_ = Scene::normal;
	}

}

void TitleS::Normal() {

	// 1フレームでのパラメータ加算地
	const float step = (2.0f * (float)std::numbers::pi) / endFrame;

	switch (anime_) {
	case NormalAnime::Normal:
		floatT += step;
		// 2Πを超えたら0に戻す
		floatT = (float)std::fmod(floatT, 2.0f * std::numbers::pi);

		world_.translation_.y = NormalY + std::sin(floatT) * floatheight_;

		break;
	case NormalAnime::HitBoundIni:
		world_.translation_.y = NormalY;
		velo_.y = 1;
		anime_ = NormalAnime::HitBound;
		floatT = 0;
		break;
	case NormalAnime::HitBound:
		world_.translation_ = Add(world_.translation_, velo_);
		velo_.y -= 1.0f/10.0f;

		if (world_.translation_.y <= NormalY) {
			world_.translation_.y = NormalY;
			velo_ = zero;
			anime_ = NormalAnime::Normal;
		}
		break;
	default:
		break;
	}

	//ヒットカウントが数以上でシーン変更
	if (hitCount_ >= maxHitNum) {
		velo_.y = 2;
		scene_ = Scene::endAnimation;
	}

}

void TitleS::EndAnimation() {
	world_.translation_ = Add(world_.translation_, velo_);
	velo_.y -= 1.0f / 10.0f;
	world_.rotation_.y += (1.0f / 30.0f) * (float)std::numbers::pi;
	if (world_.translation_.y <= -40) {
		world_.translation_.y = -40;
		isPlay_ = true;
	}
}

void TitleS::OnCollision() {
	if (scene_ == Scene::normal) {
		hitCount_++;
	}
	anime_ = NormalAnime::HitBoundIni;
}

void TitleS::Draw(ViewProjection view) { 
	model_->Draw(world_, view);
	tileM_->Draw(tile_, view);
}