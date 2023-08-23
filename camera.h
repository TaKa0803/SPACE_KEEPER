#pragma once
#include<WorldTransform.h>
#include<ViewProjection.h>
#include<Input.h>
#include<Model.h>

#include"Reticle.h"
class Camera {
public:
	void Initialize(const ViewProjection& world, Model* model, float farZ);

	void Update();

	void Draw(ViewProjection view);

	const WorldTransform& GetWorldTransform() { return world_; }

	ViewProjection GetView() { return view_; }

	void SetTarget(const WorldTransform* target) { target_ = target; }

	const WorldTransform& GetreticleW() { return reticleWorld_; }

private:
	void cameraRotate();

	void BackCamera();
	// キー入力
	Input* input_ = nullptr;

	// ビューのターゲットポイント
	const WorldTransform* target_;

	// カメラの回転レベル
	// 回転のみ編集可能
	WorldTransform world_;

	WorldTransform cameraWorld_;

	// カメラに渡すビュー
	ViewProjection view_;

	// 回転軸の処理
	float kRotateTheta = (1.0f / 120.0f) * 3.14f;

	float pi = 3.14f;

	
	// レティクルのワールド
	WorldTransform reticleWorld_;
	Model* model_;

	//制限範囲
	const float area = 30;



	const Vector2 zeroP_ = {0, 0};
	
	//レティクルを戻すときのベクトル
	Vector2 maxBackVec_;
	//レティクル戻る処理のフラグ
	bool isBackRetcle_=false;

	//イージング用T
	float easingT_ = 0;

	//フレーム加算するイージング
	const float addT_ = 1.0f / 60.0f;
};