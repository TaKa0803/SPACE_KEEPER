#pragma once
#include<Model.h>
#include<WorldTransform.h>
#include"baseChara.h"
#include<Input.h>
#include<optional>
#include"Glovalv.h"

class Player : public BaseCharacter {
private:
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


public:
	// モデルと画像
	std::vector<Model*> models_;
	// キー入力
	Input* input_ = nullptr;


};