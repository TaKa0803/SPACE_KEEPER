#pragma once
#include<Model.h>
#include<workspaceax.h>

class Plane {
public:
	void Initialize(Model* model, const WorldTransform* target);

	void Update();

	void Draw(ViewProjection view);
	private:

	WorldTransform world_;

	Model* model_;
};