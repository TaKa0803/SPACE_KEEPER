#pragma once
#include"EnemyBullet.h"

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

#include"title.h"
#include"Plane.h"


enum class GScene {
	Title,
	TitleToIngame,
	InGame,
	InGameToClear,
	Clear,
	BackTitle,
	GameOver,
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

	void SetStartUP();


	void ShotEN(Model* model, const Vector3 pos, const Vector3 velo, float scale);

	void ShotEC(Model* model, const Vector3 pos, const Vector3 velo, float scale);

private:
#pragma region 各種クラス内呼び出し関数
	void LoadModel();
	void LoadClass();
	void LoadTexture();
	void DrawModel();
	void DrawSprite();

	
	
	void TitlrUpdate();

	void InGameUpdate();
	
	void ClearUpdate();

	void GameOverUpdate();

	void EndAnime();
#pragma endregion
	
	// メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	GScene scene_ = GScene::Title;

	XINPUT_STATE joyState;
	

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	/// 
	ViewProjection view_;

	float farZ = 2000;

	bool reddy_ = false;

	float et = 0;

	float camT = 0;

	bool camNear = false;

	std::unique_ptr<Player> player_ = nullptr;
	// プレイヤーの弾
	std::list<PlayerBullet*> playerbullets_;

	std::unique_ptr<Camera> camera_ = nullptr;

	std::unique_ptr<Skydome> skydome_ = nullptr;

	//地面
	std::unique_ptr<Plane> plane_ = nullptr;
	// 地面(敵
	std::unique_ptr<Plane> plane_E = nullptr;



	std::unique_ptr<BCore> core_ = nullptr;

	std::list<EnemyBullet*> enemyBullets_;
	
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

	
	std::unique_ptr<Model> playerModel[partsNum];
	

	std::unique_ptr<Model> pweapon_;

	std::unique_ptr<Model> jettpack_;

	std::unique_ptr<Model> fire_;

	std::unique_ptr<Model> pAmmo_;

	std::unique_ptr<Model> eAmmo_;

	std::unique_ptr<Model> titleModel_;

	std::unique_ptr<Model> titleTileM_;


#pragma region bos
	std::unique_ptr<Model> theCore_;
	std::unique_ptr<Model> weekCore_;

	std::unique_ptr<Model> ebody_;
	std::unique_ptr<Model> ehead_;
	
	std::unique_ptr<Model> eleg;
	std::unique_ptr<Model> eL1A_;
	std::unique_ptr<Model> eL2A_;
	std::unique_ptr<Model> eLhand_;

	std::unique_ptr<Model> eR1A_;
	std::unique_ptr<Model> eR2A_;
	std::unique_ptr<Model> eRhand_;

	std::unique_ptr<Model> eLammo_;
	std::unique_ptr<Model> eRammo_;

#pragma endregion




#pragma endregion


#pragma region texture
	uint32_t basicTex_;

	uint32_t clear_;
	std::unique_ptr<Sprite> sprite_ = nullptr;

	uint32_t red_;

	uint32_t gametex_;
	std::unique_ptr<Sprite> gameoverSprite_ = nullptr;

	uint32_t Rtex_;
	std::unique_ptr<Sprite> Rtexsp_ = nullptr;
#pragma endregion

	
};
