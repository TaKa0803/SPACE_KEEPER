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

	Vector3 GetmatW() {
		return {world_.matWorld_.m[3][0], world_.matWorld_.m[3][1], world_.matWorld_.m[3][2]};
	}

	const WorldTransform& GetWorldTransform() { return world_; }


private:
	Model* model_ = nullptr;

	
	WorldTransform world_;

	int32_t hp_;

	bool isdead_;
};