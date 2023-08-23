#pragma once
#include<WorldTransform.h>
#include<Model.h>
#include<Vector2.h>
#include<Input.h>
class Reticle {
public:
	void Initialize(Model*model,const WorldTransform&parent );

	void Update(WorldTransform& world_);

	void Draw(ViewProjection view);

	const WorldTransform& GetRW() { return reticleWorld_; }

private:
#pragma region まとめ
	void Move();
	void CameraRotate(WorldTransform& world_);
	void Back();
#pragma endregion

	// キー入力
	Input* input_ = nullptr;


	// レティクルのワールド
	WorldTransform reticleWorld_;
	Model* model_;

	const float moveNum = 1.0f;

	// 制限範囲
	const float area = 30;

	const float pi = 3.14f;

	// 回転量の最大値
	const float kRotateTheta = (1.0f / 120.0f) * 3.14f;


	//レティクル原点
	const Vector3 NormalPos = {0, 0, 30};


	//レティクルの戻る処理に使う
	const Vector2 zeroP_ = {0, 0};

	// レティクルを戻すときのベクトル
	Vector2 maxBackVec_;

	// レティクル戻る処理のフラグ
	bool isBackRetcle_ = false;

	// イージング用T
	float easingT_ = 0;

	// フレーム加算するイージング
	const float addT_ = 1.0f / 60.0f;

};


