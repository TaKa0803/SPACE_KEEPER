#pragma once
#include<Model.h>
#include<WorldTransform.h>


enum class BType {
	Normal,
	Chase,
};

enum class Cmode {
	start,
	chase,
	none,
};

class EnemyBullet {
public:
	void Initialize(
	    Model* model, const Vector3& position, const Vector3& velocityN,
	    const WorldTransform* taargetW, BType type, bool under50,float scale);

	void Update();

	void Draw(ViewProjection view);

	bool IsDead() const { return isDead_; }

	void SetDead() { isDead_ = true; }

	Vector3 GetW() {
		return {world_.matWorld_.m[3][0], world_.matWorld_.m[3][1], world_.matWorld_.m[3][2]};
	}

	float GetScale() { return world_.scale_.x; }
	
	void OnCollision() { isDead_ = true; }


private:
	
	Vector3 GetTW() {
		return {
		    targetW_->matWorld_.m[3][0], targetW_->matWorld_.m[3][1], targetW_->matWorld_.m[3][2]};
	}

	bool isDead_ = false;

	WorldTransform world_;
	Model* model_;

	Vector3 velocity_;

	static const int32_t kLifeTime = 60 * 10;

	float BulletSpd_;

	int32_t deathTimer = kLifeTime;

	// ターゲットのワールド座標
	const WorldTransform* targetW_;

	BType BulletType;

	int32_t chasewaitT = 30;

	bool isChase_ = false;

	Cmode cmode_ = Cmode::start;

	float T = 0;


	bool isAngry_;
};