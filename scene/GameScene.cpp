#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::LoadModel() { 
	model_.reset(Model::Create());
	sky_.reset(Model::CreateFromOBJ("sssssk"));
}

void GameScene::LoadClass() {
	std::vector<Model*> playerModels = {model_.get()};
	player_ = std::make_unique<Player>();
	player_->Initialize(playerModels);

	camera_ = std::make_unique<Camera>();
	camera_->Initialize(view_,model_.get());
	camera_->SetTarget(&player_->GetWorldTransform());
	player_->SetReticle(&camera_->GetreticleW());

	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(sky_.get(),farZ);
}

void GameScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	view_.Initialize();
	view_.farZ = farZ;
	LoadModel();
	LoadClass();
}

void GameScene::Update() { 
	skydome_->Update();
	

	player_->Update();

	camera_->Update();
	view_.matView = camera_->GetView().matView;
	view_.matProjection = camera_->GetView().matProjection;
	view_.TransferMatrix();

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

void GameScene::DrawModel() { 
	skydome_->Draw(view_);
	player_->Draw(view_);
	camera_->Draw(view_);
}

void GameScene::DrawSprite() {



}
