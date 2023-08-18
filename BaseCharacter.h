#pragma once
#include <Model.h>
#include <WorldTransform.h>
#include <vector>
class BaseCharacter {
protected:
	// モデルデータ配列
	std::vector<Model*> models_;
	// ワールド変換データ
	WorldTransform worldtransform_;

public:
	/// 初期化
	virtual void Initialize(const std::vector<Model*>& models);

	/// 更新
	virtual void Update();

	// 描画
	virtual void Draw(const ViewProjection& view);

	const WorldTransform& GetWorldTransform() { return worldtransform_; }
};