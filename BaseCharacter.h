#pragma once
#include <Model.h>
#include <WorldTransform.h>
#include <vector>

class GameScene;

class BaseCharacter {
protected:
	// モデルデータ配列
	std::vector<Model*> models_;
	// ワールド変換データ
	WorldTransform worldtransform_;

	uint32_t hp_;

	bool isDead_;

	// ゲームシーン
	GameScene* gameScene_ = nullptr;

public:
	/// 初期化
	virtual void Initialize(const std::vector<Model*>& models, const uint32_t HP);

	/// 更新
	virtual void Update();

	// 描画
	virtual void Draw(const ViewProjection& view);

	const WorldTransform& GetWorldTransform() { return worldtransform_; }

	const Vector3 GetmatPos() {
		return {
		    worldtransform_.matWorld_.m[3][0], worldtransform_.matWorld_.m[3][1],
		    worldtransform_.matWorld_.m[3][2]
		};
	}

	const bool& IsDead() { return isDead_; };
	// ゲームシーン
	void SetgameScene(GameScene* gameScene) { gameScene_ = gameScene; }
};