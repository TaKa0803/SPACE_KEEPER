#pragma once
#include<WorldTransform.h>
#include<ViewProjection.h>
#include<Model.h>

class Camera {
public:
	void Initialize(float farZ, const WorldTransform* target);

	void Update();

	ViewProjection GetView() { return view_; }

	float Getfar() { return cameraWorld_.translation_.z; }

	void Setfar(float Far) { cameraWorld_.translation_.z = Far; }

private:
	//カメラ本体
	WorldTransform cameraWorld_;

	// カメラに渡すビュー
	ViewProjection view_;

	
};