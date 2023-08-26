#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include"math_matrix.h"

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


	enemyModels_ = {baseModel_.get()};
}

//クラスのロードまとめ
void GameScene::LoadClass() {
	std::vector<Model*> playerModels = {baseModel_.get()};

	player_ = std::make_unique<Player>();
	player_->Initialize(playerModels,10);
	player_->SetgameScene(this);
	
	camera_ = std::make_unique<Camera>();
	camera_->Initialize(baseModel_.get(), farZ);
	camera_->SetTarget(&player_->GetWorldTransform());
	player_->SetReticle(&camera_->GetreticleW());

	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(sky_.get(), farZ);

	core_ = std::make_unique<Core>();
	core_->Initialize(baseModel_.get());

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

void GameScene::EnemyPop() {
	//条件クリアで出現
	if (input_->TriggerKey(DIK_0)) {
		// 敵出現
		Vector3 pos1 = {100, 100, 100};
		Vector3 pos2 = {-100, 100, 100};
		Vector3 pos3 = {100, 100, -100};
		Vector3 pos4 = {-100, 100, -100};

		Vector3 pos5 = {100, -100, 100};
		Vector3 pos6 = {-100, -100, 100};
		Vector3 pos7 = {100, -100, -100};
		Vector3 pos8 = {-100, -100, -100};

		AddEnemy(pos1);
		AddEnemy(pos2);
		AddEnemy(pos3);
		AddEnemy(pos4);
		AddEnemy(pos5);
		AddEnemy(pos6);
		AddEnemy(pos7);
		AddEnemy(pos8);

		
	}

}


//更新処理
void GameScene::Update() { 
	EnemyPop();

	skydome_->Update();
	
	camera_->Update();
	view_.matView = camera_->GetView().matView;
	view_.matProjection = camera_->GetView().matProjection;
	view_.TransferMatrix();

	player_->Update();

	core_->Update();

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
	skydome_->Draw(view_);
	player_->Draw(view_);
	camera_->Draw(view_);
	core_->Draw(view_);

	for (Enemy* enemy : enemy_) {
		enemy->Draw(view_);
	}

	for (PlayerBullet* bullet : playerbullets_) {
		bullet->Draw(view_);
	}
}

//スプライト描画
void GameScene::DrawSprite() {



}
