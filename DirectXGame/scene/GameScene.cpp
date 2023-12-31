#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include"math_matrix.h"
#include<ImGuiManager.h>

GameScene::GameScene() {}

GameScene::~GameScene() {
	for (PlayerBullet* bullet : playerbullets_) {
		delete bullet;
		bullet =nullptr;
	}

	for (EnemyBullet* eb : enemyBullets_) {
		delete eb;
		eb = nullptr;
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

	clear_ = TextureManager::Load("Clear.png");
	sprite_.reset(Sprite::Create(clear_, {0, 0}));
	
	gametex_ = TextureManager::Load("Gameover.png");
	gameoverSprite_.reset(Sprite::Create(gametex_, {0,0}));


	red_ = TextureManager::Load("Weak.png");

	Rtex_ = TextureManager::Load("Rkey.png");
	Rtexsp_.reset(Sprite::Create(Rtex_, {900,520}));

	

		

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

	//敵
	ebody_.reset(Model::CreateFromOBJ("Ebody"));
	ehead_.reset(Model::CreateFromOBJ("Ehead"));
	eleg.reset(Model::CreateFromOBJ("Eleg"));
	eL1A_.reset(Model::CreateFromOBJ("ELArm1"));
	eL2A_.reset(Model::CreateFromOBJ("ELArm2"));
	eLhand_.reset(Model::CreateFromOBJ("ELhand"));

	eR1A_.reset(Model::CreateFromOBJ("ERArm1"));
	eR2A_.reset(Model::CreateFromOBJ("ERArm2"));
	eRhand_.reset(Model::CreateFromOBJ("ERhand"));

	theCore_.reset(Model::CreateFromOBJ("WeakP"));
	weekCore_.reset(Model::CreateFromOBJ("WeekRED"));

	eLammo_.reset(Model::CreateFromOBJ("Lammo"));
	eRammo_.reset(Model::CreateFromOBJ("Rammo"));
}

//クラスのロードまとめ
void GameScene::LoadClass() {
	std::vector<Model*> playerModels = {pweapon_.get(), pbody_.get(),  phead_.get(), plhand_.get(),  prhand_.get(),  plleg_.get(), prleg_.get(), jettpack_.get(), fire_.get(),  pAmmo_.get()};
	
	// title
	title_ = std::make_unique<TitleS>();
	title_->Initialize(titleModel_.get(), titleTileM_.get());


	std::vector<Model*> CoreModels = {ehead_.get(),  ebody_.get(),  eleg.get(),  eL1A_.get(),
	                                  eL2A_.get(),   eLhand_.get(),  eR1A_.get(),     eR2A_.get(),
	                                  eRhand_.get(), theCore_.get(), weekCore_.get(), eLammo_.get(),
	                                  eRammo_.get()
	};

	// 巨大ボスのコア
	core_ = std::make_unique<BCore>();
	core_->Initialize(CoreModels);
	core_->SetgameScene(this);


	//プレイヤー
	player_ = std::make_unique<Player>();
	player_->Initialize(playerModels,10);
	player_->SetgameScene(this);
	//プレイヤーの移動の中心を設定
	//player_->SetParent(&core_->GetWorldTransform());
	player_->SetParent(&title_->GetRW());
	player_->SetView(&view_);

	core_->SetPlayer(&player_->GetWorldTransform());

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

void GameScene::SetStartUP() 
{ 
	title_->SetStartSta();
	player_->SetStart();
	player_->SetParent(&title_->GetRW());
	camera_->Setfar(-10.0f);
	
	
	core_->SetStart();
}

//プレイヤーの弾追加
void GameScene::AddPlayerBullet(PlayerBullet* playerBullet) {
	// リストに登録する
	playerbullets_.push_back(playerBullet);
}

void GameScene::ShotEN(Model* model, const Vector3 pos, const Vector3 velo,float scale) {
	EnemyBullet* newEB = new EnemyBullet();

	bool angry = core_->Getangly();

	newEB->Initialize(model, pos, velo, &player_->GetWorldTransform(), BType::Normal,angry,scale);
	enemyBullets_.push_back(newEB);
}

void GameScene::ShotEC(Model* model, const Vector3 pos, const Vector3 velo,float scale) {
	EnemyBullet* newEB = new EnemyBullet();
	bool angry = core_->Getangly();
	newEB->Initialize(model, pos, velo, &player_->GetWorldTransform(), BType::Chase,angry,scale);
	enemyBullets_.push_back(newEB);
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
		plane_->Update();
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
	#ifdef _DEBUG
	ImGui::Begin("game");
	ImGui::DragFloat("t", &et, 0.01f);
	ImGui::End();
#endif // _DEBUG

	

	title_->Update();
	if (title_->GetScene() != Scene::endAnimation) {
		plane_->Update();
		player_->TitleUpdate(camera_->Getfar());
	}

	if (title_->GetScene() == Scene::endAnimation) {
		EndAnime();
	}

	//最初以外更新する
	if (title_->GetScene() ==Scene::normal||title_->GetScene()==Scene::endAnimation) {
		//plane_E->Update();
		core_->Update();
	}

	core_->Update();

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
	

#pragma region 
	// 当たり判定
	if (!title_->IsPlay()) {
		for (PlayerBullet* bullet : playerbullets_) {
			
				Vector3 Bpos = bullet->GetWorldT();

				if (-20 <= Bpos.x && Bpos.x <= 20 && -10 <= Bpos.y && Bpos.y <= 15 &&
				    -305 <= Bpos.z && Bpos.z <= -295) {
					bullet->OnCollision();
					title_->OnCollision();
				}
			
		}
	}
#pragma endregion
	


	// 弾の時間経過削除
	playerbullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});


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


#pragma region 自分の弾更新
	// 自分の弾の更新
	for (PlayerBullet* bullet : playerbullets_) {
		bullet->Update();
	}
#pragma endregion
	for (EnemyBullet* eb : enemyBullets_) {
		eb->Update();
	}


	CheckAllCollision();

	
	//しんだらシーン変換
	if (core_->IsDead()) {


		for (PlayerBullet* bullet : playerbullets_) {
			    bullet->SetDead();
		}

		for (EnemyBullet* eb : enemyBullets_) {
			    eb->SetDead();
		}
		scene_ = GScene::Clear;
	}

	if (player_->IsDead()) {
		for (PlayerBullet* bullet : playerbullets_) {
			    bullet->SetDead();
		}

		for (EnemyBullet* eb : enemyBullets_) {
			    eb->SetDead();
		}
		scene_ = GScene::GameOver;
	
	}
	
}

void GameScene::ClearUpdate() { 
	
	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
		
		scene_ = GScene::Title;
		SetStartUP();

		
	}

}
void GameScene::GameOverUpdate() {
	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
		scene_ = GScene::Title;
		SetStartUP();
	}
}


//更新処理
void GameScene::Update() { 
	skydome_->Update();
	Input::GetInstance()->GetJoystickState(0, joyState);

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
	case GScene::GameOver:
		GameOverUpdate();
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
	
	
#pragma region 自分の弾とコアとの当たり判定
	for (PlayerBullet* bullet : playerbullets_) {
		Vector3 Bpos = bullet->GetWorldT();
		if (CheckHitSphere(core_->GetmatCore(), Bpos, 1, 5)) {
			// 敵の当たり判定
			core_->InCollision();
			
			// 弾の判定
			bullet->OnCollision();

			
		}
	}
#pragma endregion
#pragma region 自機と敵の弾の当たり判定
	for (EnemyBullet* eb : enemyBullets_) {
		Vector3 epos = eb->GetW();
		if (CheckHitSphere(player_->GetmatPos(), epos, 0.5f, 5 * eb->GetScale())) {
			player_->OnCollision();

			eb->OnCollision();
		}
	}
#pragma endregion
}

void GameScene::UpdateDelete() {
	// 弾の時間経過削除
	playerbullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	enemyBullets_.remove_if([](EnemyBullet* eb) {
		if (eb->IsDead()) {
			delete eb;
			return true;
		} else {
			return false;
		}
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
			//plane_->Draw(view_);
			player_->Draw(view_);

			for (PlayerBullet* bullet : playerbullets_) {
				bullet->Draw(view_);
			}		
		}
		if (title_->GetScene() != Scene::start && title_->GetScene() != Scene::startAnimation) {
			//plane_E->Draw(view_);
			//core_->Draw(view_);
		}

		//plane_E->Draw(view_);
		core_->Draw(view_);
		for (EnemyBullet* eb : enemyBullets_) {
			eb->Draw(view_);
		}
		break;
	case GScene::InGame:
	//	plane_->Draw(view_);
	//	plane_E->Draw(view_);
		
		player_->Draw(view_);
		core_->Draw(view_);

		

		for (PlayerBullet* bullet : playerbullets_) {
			bullet->Draw(view_);
		}
		for (EnemyBullet* eb : enemyBullets_) {
			eb->Draw(view_);
		}

		break;
	case GScene::Clear:
		break;
	case GScene::GameOver:
		break;

	default:
		break;
	}
	
}

//スプライト描画
void GameScene::DrawSprite() {

	switch (scene_) {
	case GScene::Title:
		
		player_->DrawUI();

		Rtexsp_->Draw();
		break;
	case GScene::TitleToIngame:
		break;
	case GScene::InGame:
		player_->DrawUI();
		core_->DrawSprite();
		break;
	case GScene::InGameToClear:
		break;
	case GScene::Clear:
		sprite_->Draw();
		break;
	case GScene::BackTitle:
		break;
	case GScene::GameOver:
		gameoverSprite_->Draw();
		break;

	default:
		break;
	}

}
