#pragma once
#include<WorldTransform.h>
#include<ViewProjection.h>
#include<Input.h>
#include<Model.h>
class Camera {
public:
	void Initialize(const ViewProjection& world,Model*model);

	void Update();

	void Draw(ViewProjection view);

	const WorldTransform& GetWorldTransform() { return world_; }

	ViewProjection GetView() { return view_; }

	void SetTarget(const WorldTransform* target) { target_ = target; }

	const WorldTransform& GetreticleW() { return reticleWorld_;}

private:
	void cameraRotate(float num);

	//キー入力
	Input* input_ = nullptr;

	// ビューのターゲットポイント
	const WorldTransform* target_;

	//カメラの回転レベル
	//回転のみ編集可能
	WorldTransform world_;

	
	WorldTransform cameraWorld_;

	//カメラに渡すビュー
	ViewProjection view_;

	//回転軸の処理 
	float kRotateTheta = (1.0f / 120.0f)*3.14f;

	float pi = 3.14f;

	

	// レティクルのワールド
	WorldTransform reticleWorld_;
	Model* model_;



};