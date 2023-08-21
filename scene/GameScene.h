#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#include"player.h"
#include"camera.h"
#include"skydome.h"
#include"PlayerBullet.h"
#include<sstream>

#include <list>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	void AddPlayerBullet(PlayerBullet* playerBullet);


private: 	
#pragma region 各種クラス内呼び出し関数
	void LoadModel();
	void LoadClass();
	void DrawModel();
	void DrawSprite();
#pragma endregion
	
	// メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	/// 
	ViewProjection view_;

	float farZ = 2000;

	std::unique_ptr<Player> player_ = nullptr;

	std::unique_ptr<Camera> camera_ = nullptr;

	std::unique_ptr<Skydome> skydome_ = nullptr;

	// プレイヤーの弾
	std::list<PlayerBullet*> playerbullets_;




	//model
	std::unique_ptr<Model> model_ = nullptr;

	std::unique_ptr<Model> sky_ = nullptr;
	
};
