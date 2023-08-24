#pragma once
#include<Model.h>
#include<WorldTransform.h>
#include"baseCharacter.h"
#include<Input.h>
#include<optional>
#include"Glovalv.h"

class GameScene;

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

	// ゲームシーン
	void SetgameScene(GameScene* gameScene) { gameScene_ = gameScene; }


	void InCollision();

private:
#pragma region まとめ
	void Move();

	Vector3 GetplayermatTranslate() {
		return {
		    worldtransform_.matWorld_.m[3][0], worldtransform_.matWorld_.m[3][1],
		    worldtransform_.matWorld_.m[3][2]};
	}
	
	Vector3 GetReticleMat() {
		return {
		    Reticle_->matWorld_.m[3][0], Reticle_->matWorld_.m[3][1], Reticle_->matWorld_.m[3][2]};
	}

	void Attack();
#pragma endregion

	// キー入力
	Input* input_ = nullptr;

	//弾モデル
	Model* ammo = nullptr;
	
	//プレイヤーのレティクルを戻す処理のt
	float t = 1;

	// 回転軸の処理 
	float kRotateTheta = 1.0f / 60.0f;
	//プレイヤー情報をもらう場所
	const WorldTransform* Reticle_;

	// ゲームシーン
	GameScene* gameScene_ = nullptr;
};