#pragma once
#include<Model.h>
#include<WorldTransform.h>
class Core {
public:
	void Initialize(Model* model, uint32_t textu);

	void Update();

	void Draw(ViewProjection view);

	void InCollision();

private:
	Model* model_ = nullptr;

	uint32_t texture_;
	WorldTransform world_;

	int32_t hp_;

	bool isdead_;
};