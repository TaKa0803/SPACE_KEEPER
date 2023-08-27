#pragma once
#include<Model.h>
#include<WorldTransform.h>
#include"baseCharacter.h"
#include<Input.h>
#include<optional>
#include"Glovalv.h"
#include<Reticle.h>


class GameScene;

class Player : public BaseCharacter {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const std::vector<Model*>& models, const uint32_t HP) override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& view) override;

	const WorldTransform& GetplayerBaseW() { return playerMoveW; }

	void SetParent(const WorldTransform *world);

	void OnCollision();

private:
#pragma region まとめ
	void GetStatus();

	void Move();

	const Vector3 GetplayermatTranslate() {
		return {
		    worldtransform_.matWorld_.m[3][0], worldtransform_.matWorld_.m[3][1],
		    worldtransform_.matWorld_.m[3][2]};
	}
	
	const Vector3 GetTargetmatW() {
		return {targetW_.matWorld_.m[3][0], targetW_.matWorld_.m[3][1], targetW_.matWorld_.m[3][2]};
	}

	void Attack();
#pragma endregion

	// キー入力
	Input* input_ = nullptr;

	//弾モデル
	Model* ammo = nullptr;
	
	// 弾の速度
	const float kBulletSpeed = 10.0f;

	//攻撃するボスの座標のみ取得
	const WorldTransform  *target_;

	//座標のみを取得したWorld,回転軸を編集して変えれる
	WorldTransform targetW_;

	//ターゲットの子関係のプレイヤー活動エリア(基本的に編集しない
	WorldTransform playerMoveW;

	// 移動量
	const float moveNumN = 0.5f;

	//移動制限
	const float area = 4.0f;

	//半径
	
	//レティクル
	Reticle* reticle_;

	const float pi = 3.14f;

	//移動時の回転量
	const float maxMoveTheta = (1.0f / 240.0f)*pi;
	
	//発射している状態
	bool isShot = false;

	WorldTransform bodyW_;
	WorldTransform headW_;
	WorldTransform LhandW_;
	WorldTransform RhandW_;
	WorldTransform LlegW_;
	WorldTransform RlegW_;
	WorldTransform weaponW_;

	WorldTransform jettpack_;
	WorldTransform fire_;

};