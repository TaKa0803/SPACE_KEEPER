#pragma once
#include<Model.h>
#include<WorldTransform.h>
#include"baseCharacter.h"
#include<Input.h>
#include<optional>
#include"Glovalv.h"

class Player : public BaseCharacter {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const std::vector<Model*>& models) override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& view) override;

	void SetReticle(const WorldTransform* reticle) { Reticle_ = reticle; }

private:
#pragma region まとめ
	void ReticleMove();
	void Move();

	Vector3 GetplayermatTranslate() {
		return {
		    worldtransform_.matWorld_.m[3][0], worldtransform_.matWorld_.m[3][1],
		    worldtransform_.matWorld_.m[3][2]};
	}
	
#pragma endregion

	// キー入力
	Input* input_ = nullptr;

	
	//プレイヤーのレティクルを戻す処理のt
	float t = 1;

	// 回転軸の処理 
	float kRotateTheta = 1.0f / 60.0f;


	const WorldTransform* Reticle_;
};