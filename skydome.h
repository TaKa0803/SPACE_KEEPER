#pragma once
#include<Model.h>
#include<WorldTransform.h>

class Skydome {
public:
	void Initialize(Model* model, float farZ);

	void Update();

	void Draw(ViewProjection view);

	private:

	Model* model_;
	WorldTransform world_;
};