#pragma once
#include<WorldTransform.h>
#include<Model.h>
#include<Vector2.h>
#include<Input.h>


class Reticle {
public:
	void Initialize(Model*model,const WorldTransform&parent );

	void Update(float length);

	void Draw(ViewProjection view);

	const WorldTransform& GetRW() { return reticleWorld_; }

	const Vector3 GetmatW() {
		return {
		    reticleWorld_.matWorld_.m[3][0], reticleWorld_.matWorld_.m[3][1],
		    reticleWorld_.matWorld_.m[3][2]};
	}

	void SetDepth(float depth) { reticleWorld_.translation_.z = depth; }

private:
#pragma region まとめ
	void Move(float length);

#pragma endregion

	// キー入力
	Input* input_ = nullptr;


	// レティクルのワールド
	WorldTransform reticleWorld_;
	Model* model_;

	//移動量
	const float moveNum = 1.0f;

	// 制限範囲
	const float area = 100;

	//レティクル原点
	const Vector3 NormalPos = {0, 0, 30};

};


