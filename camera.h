#pragma once
#include<WorldTransform.h>
#include<ViewProjection.h>

#include<Model.h>

#include"Reticle.h"
class Camera {
public:
	void Initialize(Model* model, float farZ);

	void Update();

	void Draw(ViewProjection view);

	const WorldTransform& GetWorldTransform() { return world_; }

	ViewProjection GetView() { return view_; }

	void SetTarget(const WorldTransform* target) { target_ = target; }

	const WorldTransform& GetreticleW() { return reticle_->GetRW(); }

private:
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

	
	Reticle *reticle_ = nullptr;
	
};