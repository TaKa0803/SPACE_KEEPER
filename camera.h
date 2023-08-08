#pragma once
#include<WorldTransform.h>
#include<ViewProjection.h>
#include<Input.h>
class Camera {
public:
	void Initialize(const ViewProjection& world);

	void Update();

	ViewProjection GetView() { return view_; }

	void SetTarget(const WorldTransform* target) { target_ = target; }

private:
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
};