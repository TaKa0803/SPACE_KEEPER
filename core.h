#pragma once
#include<Model.h>
#include<WorldTransform.h>
class Core {
public:
	void Initialize(Model* model);

	void Update();

	void Draw(ViewProjection view);

	void InCollision();

	bool IsDead() { return isdead_; }


private:
	Model* model_ = nullptr;

	
	WorldTransform world_;

	int32_t hp_;

	bool isdead_;
};