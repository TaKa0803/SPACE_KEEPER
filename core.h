#pragma once
#include<Model.h>
#include<WorldTransform.h>
#include"BaseCharacter.h"

enum class WeakCore { 
	center_,
	L1,
	L2,
	R1,
	R2
};


class BCore :public BaseCharacter{
public:
	

	void Initialize(const std::vector<Model*>& models);

	
	void Update()override;

	void Draw(const ViewProjection& view)override;

	void InCollision();

	Vector3 GetmatW() {
		return {
		    worldtransform_.matWorld_.m[3][0], worldtransform_.matWorld_.m[3][1],
		    worldtransform_.matWorld_.m[3][2]};
	}

	
	void UpdateAllMatrix();

	void SetPlayer(const WorldTransform* world) { player_ = world; }

	void SeeTarget();

private:
	const WorldTransform* player_;

	Vector3 GetTargetW() { 
		return {player_->matWorld_.m[3][0], player_->matWorld_.m[3][1], player_->matWorld_.m[3][2]
		};
	}

	
	WeakCore weakC = WeakCore::center_;

	
	WorldTransform head_;
	WorldTransform body_;
	WorldTransform leg_;
	WorldTransform LArm1_;
	WorldTransform LArm2_;
	WorldTransform RArm1_;
	WorldTransform RArm2_;

	WorldTransform Lhand_;
	WorldTransform Rhand_;

	//コア
	WorldTransform C_center_;
	WorldTransform C_L1_;
	WorldTransform C_L2_;
	WorldTransform C_R1_;
	WorldTransform C_R2_;

};