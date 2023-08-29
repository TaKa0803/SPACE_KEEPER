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

	void TitleUpdate(float Clong);

	void EndUpdate(bool ismove, float Clong,float et);

	void UpdateAllMatrix();

	void Setfar(float Far) { playerMoveW.translation_.z = Far; }

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

	const Vector3 GetplayerMovematW() {
		return {
		    playerMoveW.matWorld_.m[3][0], playerMoveW.matWorld_.m[3][1],
		    playerMoveW.matWorld_.m[3][2]};
	}

	void Attack();
#pragma endregion

	// キー入力
	Input* input_ = nullptr;

	XINPUT_STATE joyState;
	

	//弾モデル
	Model* ammo = nullptr;
	
	// 弾の速度
	const float kBulletSpeed = 1.0f;

	bool canBulletShot_ = true;

	uint32_t shotcooltime_ = 0;

	const uint32_t notShotT = 30;

	uint32_t PushingCount_ = 0;

	const uint32_t maxCount = 8;

	//攻撃するボスの座標のみ取得
	const WorldTransform  *target_;

	//座標のみを取得したWorld,回転軸を編集して変えれる
	WorldTransform targetW_;

	//ターゲットの子関係のプレイヤー活動エリア(基本的に編集しない
	WorldTransform playerMoveW;

	// 移動量
	const float moveNumN = 0.5f;

	//移動制限
	const float area = 2.0f;

	//
	Vector3 stP = {0,0,-340};

	Vector3 edP = {0,0,-100};

	//レティクル
	Reticle* reticle_;

	const float pi = 3.14f;

	//移動時の回転量
	const float maxMoveTheta = (1.0f / 240.0f)*pi;
	
	//発射している状態
	bool isShot = false;
	bool beforeIsShot = isShot;

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