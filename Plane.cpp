#include<Plane.h>

void Plane::Initialize(Model* model, const WorldTransform* target) { 
	model_ = model;
	world_.Initialize(); 
	world_.translation_.y = -1;
	world_.parent_ = target;
}

void Plane::Update() { 
	world_.rotation_.y += 1.0f;
	world_.UpdateMatrix();
}

void Plane::Draw(ViewProjection view) { model_->Draw(world_,view); }