#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include"math_matrix.h"
#include<ImGuiManager.h>

GameScene::GameScene() {}

GameScene::~GameScene() {
	for (PlayerBullet* bullet : playerbullets_) {
		delete bullet;
	}

	for (Enemy* enemy : enemy_) {
		delete enemy;
	}
}

//初期化
void GameScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	view_.farZ = farZ*2.0f;
	view_.Initialize();
	LoadTexture();
	LoadModel();
	LoadClass();
}

//モデルのロードまとめ
void GameScene::LoadModel() {
	baseModel_.reset(Model::Create());
	sky_.reset(Model::CreateFromOBJ("sssssk"));
	planeModel_.reset(Model::CreateFromOBJ("Plane"));

	coreModel_.reset(Model::CreateFromOBJ("core"));

	enemyModels_ = {baseModel_.get()};

	pbody_.reset(Model::CreateFromOBJ("PBody"));
	phead_.reset(Model::CreateFromOBJ("Phead"));
	plhand_.reset(Model::CreateFromOBJ("PLhand"));
	prhand_.reset(Model::CreateFromOBJ("PRhand"));
	plleg_.reset(Model::CreateFromOBJ("PLleg"));
	prleg_.reset(Model::CreateFromOBJ("PRleg"));
	pweapon_.reset(Model::CreateFromOBJ("PWeapon"));


	fire_.reset(Model::CreateFromOBJ("fire"));
	jettpack_.reset(Model::CreateFromOBJ("jettpack"));

	pAmmo_.reset(Model::CreateFromOBJ("ammo"));
	eAmmo_.reset(Model::CreateFromOBJ("Eammo"));
	//タイトル	
	titleModel_.reset(Model::CreateFromOBJ("Title"));
	titleTileM_.reset(Model::CreateFromOBJ("TitleTile"));
}

//クラスのロードまとめ
void GameScene::LoadClass() {
	std::vector<Model*> playerModels = {pweapon_.get(), pbody_.get(),  phead_.get(), plhand_.get(),  prhand_.get(),  plleg_.get(), prleg_.get(), jettpack_.get(),
	                                    fire_.get(),    pAmmo_.get()};
	
	// title
	title_ = std::make_unique<TitleS>();
	title_->Initialize(titleModel_.get(), titleTileM_.get());

	// 巨大ボスのコア
	core_ = std::make_unique<Core>();
	core_->Initialize(coreModel_.get());

	//プレイヤー
	player_ = std::make_unique<Player>();
	player_->Initialize(playerModels,10);
	player_->SetgameScene(this);
	//プレイヤーの移動の中心を設定
	//player_->SetParent(&core_->GetWorldTransform());
	player_->SetParent(&title_->GetRW());
	
	//カメラ
	camera_ = std::make_unique<Camera>();
	//コアをターゲットにして初期化
	camera_->Initialize(farZ,&player_->GetplayerBaseW());
	camera_->Setfar(-10.0f);
	
	//スカイドーム
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(sky_.get(), farZ);
	
	//地面
	plane_ = std::make_unique<Plane>();
	plane_->Initialize(planeModel_.get(), &player_->GetWorldTransform());

	// 地面
	plane_E = std::make_unique<Plane>();
	plane_E->Initialize(planeModel_.get(), &core_->GetWorldTransform());

	
}

//画像ロード
void GameScene::LoadTexture() { basicTex_ = TextureManager::Load("uvChecker.png"); }


//プレイヤーの弾追加
void GameScene::AddPlayerBullet(PlayerBullet* playerBullet) {
	// リストに登録する
	playerbullets_.push_back(playerBullet);
}

void GameScene::AddEnemy(Vector3 pos) {
	Enemy* enemy = new Enemy();
	enemy->Initialize(enemyModels_,1,pos);
	enemy->SetgameScene(this);
	enemy_.push_back(enemy);
}

void GameScene::EndAnime() {
	
	reddy_ = title_->IsPlay();
	if (!reddy_) {
		//セット
		player_->SetParent(&core_->GetWorldTransform());
		player_->Setfar(-340);
		et = 0;
		camT = 0;
		camNear = false;
	} else {
		
		//移動
		player_->EndUpdate(title_->IsPlay(),camera_->Getfar(),et);
		
		float cam;
		if (!camNear) {
			//カメラの距離設定
			cam = (-10.0f) * (1.0f - camT) + (-30.0f) * camT;
			// 1/3
			camT += 1.0f / 40.0f;
			if (camT >= 1.0f) {
				camT = 0.0f;
				camNear = true;
			}
		} else {
			// カメラの距離設定
			cam = (-30.0f) * (1.0f - camT) + (-20.0f) * camT;
			//2/3
			camT += 1.0f / 80.0f;
			if (camT >= 1.0f) {
				camT = 1.0f;
			}
		}
		//設定
		camera_->Setfar(cam);

		et += (1.0f / 120.0f);
		if (et > 1.0f) {
			et = 1.0f;
		}
	}
	if (et>=1.0f) {
		scene_ = GScene::InGame;
	}
	
}

void GameScene::TitlrUpdate() { 
	ImGui::Begin("game");
	ImGui::DragFloat("t", &et, 0.01f);
	ImGui::End();

	title_->Update();
	if (title_->GetScene() != Scene::start || title_->GetScene() != Scene::startAnimation&&title_->GetScene()!=Scene::endAnimation) {
		plane_->Update();
		player_->TitleUpdate(camera_->Getfar());

		
	}
	if (title_->GetScene() == Scene::endAnimation) {
		EndAnime();
	}

	if (title_->GetScene() != Scene::start && title_->GetScene() != Scene::startAnimation) {

		plane_E->Update();
		core_->Update();
	}

	camera_->Update();
	view_.matView = camera_->GetView().matView;
	view_.matProjection = camera_->GetView().matProjection;
	view_.TransferMatrix();

	#pragma region 自分の弾更新
	// 自分の弾の更新
	for (PlayerBullet* bullet : playerbullets_) {
		bullet->Update();
	}
#pragma endregion
	if (input_->PushKey(DIK_0)) {
		title_->OnCollision();
	}
}



void GameScene::InGameUpdate() {
	// 親子関係の親から順に更新
	core_->Update();
	plane_E->Update();

	player_->Update();
	plane_->Update();

	camera_->Update();
	view_.matView = camera_->GetView().matView;
	view_.matProjection = camera_->GetView().matProjection;
	view_.TransferMatrix();

#pragma region 敵
	for (Enemy* enemy : enemy_) {
		enemy->Update();
	}
#pragma endregion
#pragma region 自分の弾更新
	// 自分の弾の更新
	for (PlayerBullet* bullet : playerbullets_) {
		bullet->Update();
	}
#pragma endregion
	CheckAllCollision();

	

	
}

void GameScene::ClearUpdate() {


}

//更新処理
void GameScene::Update() { 
	skydome_->Update();
	
	switch (scene_) {
	case GScene::Title:
		TitlrUpdate();
		break;
	case GScene::InGame:
		InGameUpdate();
		break;
	case GScene::Clear:
		ClearUpdate();
		break;
	default:
		break;
	}
	
	UpdateDelete();
}

bool CheckHitSphere(const Vector3& v1, const Vector3& v2, float r1, float r2) {
	Vector3 v = Subtract(v1, v2);
	float length = Length(v);
	if (length <= r1 + r2) {
		return true;
	} else {

		return false;
	}
}


void GameScene::CheckAllCollision() {
	//プレイヤー座標
	Vector3 posP = player_->GetmatPos();
	
	#pragma region 自分の弾と敵キャラの当たり判定
	for (Enemy* enemy : enemy_) {
		for (PlayerBullet* bullet : playerbullets_) {
			Vector3 Bpos = bullet->GetWorldT();
			if (CheckHitSphere(enemy->GetmatPos(), Bpos, 1, 1)) {
				// 敵の当たり判定
				enemy->OnCollision();
				// 弾の判定
				bullet->OnCollision();
			}
		}
	}
#pragma endregion
#pragma region 自分の弾とコアとの当たり判定
	for (PlayerBullet* bullet : playerbullets_) {
		Vector3 Bpos = bullet->GetWorldT();
		if (CheckHitSphere(core_->GetmatW(), Bpos, 1, 1)) {
			// 敵の当たり判定
			core_->InCollision();
			
			// 弾の判定
			bullet->OnCollision();

			
		}
	}
#pragma endregion



}

void GameScene::UpdateDelete() {

	// 死亡チェック
	enemy_.remove_if([](Enemy* enemy) {
		if (enemy->IsDead()) {
			delete enemy;
			return true;
		}
		return false;
	});

	// 弾の時間経過削除
	playerbullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

}



void GameScene::Draw() {
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion
#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	DrawModel();
	
	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion
#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);
	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	DrawSprite();
	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion
}







//モデルの描画
void GameScene::DrawModel() { 
	//天球
	skydome_->Draw(view_);

	switch (scene_) {
	case GScene::Title:
		title_->Draw(view_);
		if (title_->GetScene() != Scene::start ||title_->GetScene() != Scene::startAnimation) {
			//タイトルアニメーション終わってから描画
			plane_->Draw(view_);
			player_->Draw(view_);

			for (PlayerBullet* bullet : playerbullets_) {
				bullet->Draw(view_);
			}		
		}
		if (title_->GetScene() != Scene::start && title_->GetScene() != Scene::startAnimation) {
			plane_E->Draw(view_);
			core_->Draw(view_);
		}
		
		break;
	case GScene::InGame:
		plane_->Draw(view_);
		plane_E->Draw(view_);
		
		player_->Draw(view_);
		core_->Draw(view_);

		for (Enemy* enemy : enemy_) {
			enemy->Draw(view_);
		}

		for (PlayerBullet* bullet : playerbullets_) {
			bullet->Draw(view_);
		}
		break;
	case GScene::Clear:
		break;
	default:
		break;
	}
	
}

//スプライト描画
void GameScene::DrawSprite() {



}
