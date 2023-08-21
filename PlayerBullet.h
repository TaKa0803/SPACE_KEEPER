#pragma once

#include<Model.h>
#include <Vector3.h>
#include <WorldTransform.h>

class PlayerBullet {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	/// <summary>
	/// 　更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection& view);

	bool IsDead() const { return isdead_; }

	// 当たったら呼び出されるCB関数
	void OnCollision() { isdead_ = true; };

	Vector3 GetWorldT() {
		return {
		    worldTransform_.matWorld_.m[3][0],
		    worldTransform_.matWorld_.m[3][1],
		    worldTransform_.matWorld_.m[3][2],
		};
	}

private:
	WorldTransform worldTransform_;

	Model* model_ = nullptr;
	//uint32_t textureHandle_ = 0u;

	// 速度
	Vector3 velocity_;

	// 寿命
	static const int32_t kLifeTime = 60 * 5;

	// デスタイマー
	int32_t deathTimer_ = kLifeTime;
	// フラグ
	bool isdead_ = false;
};