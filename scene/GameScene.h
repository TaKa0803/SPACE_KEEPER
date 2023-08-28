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
#include"core.h"
#include <list>
#include"Enemy.h"
#include"title.h"
#include"Plane.h"


enum class GScene {
	Title,
	TitleToIngame,
	InGame,
	InGameToClear,
	Clear,
	BackTitle,
};


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

	void CheckAllCollision();

	void UpdateDelete();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	void AddPlayerBullet(PlayerBullet* playerBullet);


private: 	
#pragma region 各種クラス内呼び出し関数
	void LoadModel();
	void LoadClass();
	void LoadTexture();
	void DrawModel();
	void DrawSprite();

	void EnemyPop();

	void AddEnemy(Vector3 pos);

	void TitlrUpdate();

	void InGameUpdate();
	
	void ClearUpdate();
#pragma endregion
	
	// メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	GScene scene_ = GScene::InGame;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	/// 
	ViewProjection view_;

	float farZ = 2000;

	std::unique_ptr<Player> player_ = nullptr;
	// プレイヤーの弾
	std::list<PlayerBullet*> playerbullets_;

	std::unique_ptr<Camera> camera_ = nullptr;

	std::unique_ptr<Skydome> skydome_ = nullptr;

	//地面
	std::unique_ptr<Plane> plane_ = nullptr;
	// 地面(敵
	std::unique_ptr<Plane> plane_E = nullptr;



	std::unique_ptr<Core> core_ = nullptr;

	//敵
	std::list<Enemy*> enemy_;

	std::unique_ptr<TitleS> title_ = nullptr;


#pragma region モデル
	// 仮model
	std::unique_ptr<Model> baseModel_ = nullptr;
	//天球
	std::unique_ptr<Model> sky_ = nullptr;

	//コアのモデル
	std::unique_ptr<Model> coreModel_ = nullptr;

	//地面
	std::unique_ptr<Model> planeModel_=nullptr;

	std::vector<Model*> enemyModels_;

	

	std::unique_ptr<Model> pbody_;
	std::unique_ptr<Model> phead_;
	std::unique_ptr<Model> plhand_;
	std::unique_ptr<Model> prhand_;
	std::unique_ptr<Model> plleg_;
	std::unique_ptr<Model> prleg_;

	std::unique_ptr<Model> pweapon_;

	std::unique_ptr<Model> jettpack_;

	std::unique_ptr<Model> fire_;

	std::unique_ptr<Model> pAmmo_;

	std::unique_ptr<Model> eAmmo_;

	std::unique_ptr<Model> titleModel_;

	std::unique_ptr<Model> titleTileM_;
#pragma endregion


#pragma region texture
	uint32_t basicTex_;
#pragma endregion

	
};
