#include"player.h"
#include<ImGuiManager.h>
#include<cmath>
#include"math_matrix.h"
#include<GameScene.h>
#include<iostream>

void Player::SetStart() {
	hp_ = 10;
	isDead_ = false;
	worldtransform_.translation_ = {0, -3.5f, 0};
	worldtransform_.rotation_ = {0, 0, 0};
	worldtransform_.scale_ = {1, 1, 1};
	// ターゲットとの距離（Z)
	playerMoveW.translation_ = {0, 0, -40.0f};
	playerMoveW.rotation_ = {0, 0, 0};

	



	canBulletShot_ = true;
	shotcooltime_ = 0;
	PushingCount_ = 0;

	isShot = false;
	beforeIsShot = isShot;


	targetW_.rotation_ = {0, 0, 0};
	
}

void Player::Initialize(const std::vector<Model*>& models, const uint32_t HP) { 
	input_ = Input::GetInstance();
	BaseCharacter::Initialize(models, HP);
	worldtransform_.translation_ = {0, -3.5f, 0};

	playerMoveW.Initialize();
	//ターゲットとの距離（Z)
	playerMoveW.translation_ = {0, 0, -40.0f};

	targetW_.Initialize();
	ammo = models_[9];

	reticle_ = new Reticle();
	reticle_->Initialize(models_[9], playerMoveW);
	const int texheight = 720 - 40;
	float x = 1280.0f - (64.0f * 10.0f);
	x /= 2;
	hptex_ = TextureManager::Load("HPplayer.png");

	for (int i = 0; i < 10; i++) {
		hpsprite[i] =
		    Sprite::Create(hptex_, {x + 64 * i, texheight}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f});
	}



	for (int i = 0; i < partsNum; i++) {
		parts[i].Initialize();
	}

	//BODY
	parts[Parts::Body].parent_ = &worldtransform_;
	parts[Parts::BodyUnder].parent_ = &parts[Parts::Body];
	parts[Parts::Head].parent_ = &parts[Parts::Body];

	parts[Parts::LArm1].parent_ = &parts[Parts::Body];
	parts[Parts::LArm2].parent_ = &parts[Parts::LArm1];
	parts[Parts::LHand].parent_ = &parts[Parts::LArm2];

	parts[Parts::RArm1].parent_ = &parts[Parts::Body];
	parts[Parts::RArm2].parent_ = &parts[Parts::RArm1];
	parts[Parts::RHand].parent_ = &parts[Parts::RArm2];

	parts[Parts::LLeg1].parent_ = &parts[Parts::BodyUnder];
	parts[Parts::LLeg2].parent_ = &parts[Parts::LLeg1];
	parts[Parts::LFoot].parent_ = &parts[Parts::LLeg2];

	parts[Parts::RLeg1].parent_ = &parts[Parts::BodyUnder];
	parts[Parts::RLeg2].parent_ = &parts[Parts::RLeg1];
	parts[Parts::RFoot].parent_ = &parts[Parts::RLeg2];





	parts[Parts::Body].translation_ = {0, 0, 0};
	parts[Parts::BodyUnder].translation_ = {0, 0, 0};
	parts[Parts::Head].translation_ = {0, 2.6f, 0};

	parts[Parts::LArm1].translation_ = {-0.8f, 1.57f, 0};
	parts[Parts::LArm2].translation_ = {-1.73f, 0, 0};
	parts[Parts::LHand].translation_ = {-2.37f, 0, 0};

	parts[Parts::RArm1].translation_ = {0.8f, 1.57f, 0};
	parts[Parts::RArm2].translation_ = {1.73f, 0, 0};
	parts[Parts::RHand].translation_ = {2.37f, 0, 0};

	parts[Parts::LLeg1].translation_ = {-0.3f, -1.7f, 0};
	parts[Parts::LLeg2].translation_ = {0, -2.2f, 0};
	parts[Parts::LFoot].translation_ = {-0.12f, -2.2f, 0};

	parts[Parts::RLeg1].translation_ = {0.3f, -1.7f, 0};
	parts[Parts::RLeg2].translation_ = {0, -2.2f, 0};
	parts[Parts::RFoot].translation_ = {0.12f, -2.2f, 0};
}

void Player::GetStatus() {
	//過去フレームの状態取得
	beforeIsShot = isShot;


	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
		isShot = true;
	}else {
		isShot = false;
		PushingCount_ = 0;
	}

}

void Player::SetParent(const WorldTransform* world) { 
	//ターゲット先のWTを取得
	target_ = world;
	//座標を代入
	targetW_.translation_ = target_->translation_;
	//ターゲット座標のみが入ったWTを親にする
	playerMoveW.parent_ = &targetW_;
	//
	worldtransform_.parent_ = &playerMoveW;
}


void Player::OnCollision() {
	canBulletShot_ = false;
	shotcooltime_ = 120;
	if (--hp_ <= 0) {
		isDead_ = true;
	}
}

void Player::Attack() {
	//スペースキーでレティクルに弾発射
	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER&&canBulletShot_) {
		
		if (++PushingCount_ > maxCount) {
			PushingCount_ = maxCount;
		}

		//発射クールタイムを求める
		canBulletShot_ = false;

		shotcooltime_ = notShotT / PushingCount_;

		Vector3 velocity = Subtract(reticle_->GetmatW(), GetplayermatTranslate());
		
		
		velocity = Multiply(kBulletSpeed, Normalize(velocity));

		//回転量を求める
		Vector2 rt = CheckRotateFromVelo(velocity);
		Vector3 rotate = {rt.x, rt.y, 0};

		// 弾を生成して初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(ammo, GetplayermatTranslate(), velocity,rotate);

		gameScene_->AddPlayerBullet(newBullet);


	}
}

void Player::EndUpdate(bool ismove, float Clong, float et) { 
	if (!canBulletShot_) {
		if (--shotcooltime_ <= 0) {
			shotcooltime_ = 0;
			canBulletShot_ = true;
		}
	}

	GetStatus();
	// 基準点の座標を取得(y軸は除く
	targetW_.translation_ = target_->translation_;


	if (ismove) {
		//前に進む
		playerMoveW.translation_ = Esing(stP, edP, et);

		
	

		
	}

	
	
	

	// 行列更新まとめ
	UpdateAllMatrix();

	// カメラからターゲットまでのZ
	float length = playerMoveW.translation_.z + Clong;
	// 震度情報送る
	reticle_->SetDepth(-playerMoveW.translation_.z);
	reticle_->Update(length);

}

void Player::Move() {
	
	Vector3 move = {0, 0, 0};

	float moveNum;

	if (isShot) {
		if (PushingCount_ != 0) {
			moveNum = moveNumN/PushingCount_;
		} else {
			moveNum = moveNumN;
		}
	} else {
		moveNum = moveNumN;
	}

	Vector2 moveV2 = {0, 0};

	
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		moveV2 = {
		    (float)joyState.Gamepad.sThumbLX / SHRT_MAX,
		    (float)joyState.Gamepad.sThumbLY / SHRT_MAX
		};
		moveV2 = ScaV2(moveNum, moveV2);

		move.x = moveV2.x;
		move.z = moveV2.y;
	}

	

#ifdef _DEBUG
	ImGui::Text("player");
	ImGui::DragFloat3("p position", &worldtransform_.translation_.x, 0.01f);
	ImGui::DragFloat3("p ro", &worldtransform_.rotation_.x, 0.01f);
	ImGui::DragFloat3("p scale", &worldtransform_.scale_.x, 0.01f);

	ImGui::Text("player stand area");
	ImGui::Text("pos   : %4.1f/%4.1f/%4.1f", playerMoveW.translation_.x, 0.01f);
	ImGui::Text("rotate: %4.1f/%4.1f/%4.1f", playerMoveW.rotation_.x, 0.01f);
	ImGui::Text("scale : %4.1f/%4.1f/%4.1f", playerMoveW.scale_.x, 0.01f);
	
	ImGui::Text("player stand area Move Normal");
	ImGui::DragFloat3("rotate", &targetW_.rotation_.x, 0.01f);
	ImGui::DragFloat("far to target", &playerMoveW.translation_.z);

#endif // _DEBUG
	
	worldtransform_.translation_ = Add(worldtransform_.translation_,move);

	Vector2 zero = {0, 0};
	Vector2 pos = {worldtransform_.translation_.x, worldtransform_.translation_.z};
	Vector2 overvelo = {0, 0};
	//範囲内に収める処理が動いた場合移動する
	if (SetAreaEllipse(zero, pos, area, overvelo)) {

		if (isShot) {
			if (PushingCount_ != maxCount) {
				if (PushingCount_ != 0) {
					// 左右移動
					targetW_.rotation_.y -= overvelo.x * maxMoveTheta / PushingCount_;
					// 上下移動
					playerMoveW.translation_.z += overvelo.y * moveNum / PushingCount_;
				} else {
					// 左右移動
					targetW_.rotation_.y -= overvelo.x * maxMoveTheta / 2;
					// 上下移動
					playerMoveW.translation_.z += overvelo.y * moveNum / 2;
				}
			}
		} else {
			// 左右移動
			targetW_.rotation_.y -= overvelo.x * maxMoveTheta;
			// 上下移動
			playerMoveW.translation_.z += overvelo.y * moveNum;
		}
		
		worldtransform_.translation_.x = pos.x;
		worldtransform_.translation_.z = pos.y;

	}

	if (playerMoveW.translation_.z > -110) {
		playerMoveW.translation_.z = -110;
	} else if (playerMoveW.translation_.z < -250) {
		playerMoveW.translation_.z = -250;
	}

	

#ifdef _DEBUG
	ImGui::Text("PARTS");
	ImGui::DragFloat3("BT", &parts[Parts::Body].translation_.x, 0.01f);
	ImGui::DragFloat3("HT", &parts[Parts::Head].translation_.x, 0.01f);
	ImGui::DragFloat3("BUT", &parts[Parts::BodyUnder].translation_.x, 0.01f);

	ImGui::DragFloat3("LA1T", &parts[Parts::LArm1].translation_.x, 0.01f);
	ImGui::DragFloat3("LA2T", &parts[Parts::LArm2].translation_.x, 0.01f);
	ImGui::DragFloat3("Lha", &parts[Parts::LHand].translation_.x, 0.01f);

	ImGui::DragFloat3("RA1T", &parts[Parts::RArm1].translation_.x, 0.01f);
	ImGui::DragFloat3("RA2T", &parts[Parts::RArm2].translation_.x, 0.01f);
	ImGui::DragFloat3("Rha", &parts[Parts::RHand].translation_.x, 0.01f);

	ImGui::DragFloat3("LL1T", &parts[Parts::LLeg1].translation_.x, 0.01f);
	ImGui::DragFloat3("LL2T", &parts[Parts::LLeg2].translation_.x, 0.01f);
	ImGui::DragFloat3("LFT", &parts[Parts::LFoot].translation_.x, 0.01f);

	ImGui::DragFloat3("RL1T", &parts[Parts::RLeg1].translation_.x, 0.01f);
	ImGui::DragFloat3("RL2T", &parts[Parts::RLeg2].translation_.x, 0.01f);
	ImGui::DragFloat3("RFT", &parts[Parts::RFoot].translation_.x, 0.01f);

	ImGui::Text("PARTSR");
	ImGui::DragFloat3("BR", &parts[Parts::Body].rotation_.x, 0.01f);
	ImGui::DragFloat3("HR", &parts[Parts::Head].rotation_.x, 0.01f);
	ImGui::DragFloat3("BUR", &parts[Parts::BodyUnder].rotation_.x, 0.01f);

	ImGui::DragFloat3("LA1R", &parts[Parts::LArm1].rotation_.x, 0.01f);
	ImGui::DragFloat3("LA2R", &parts[Parts::LArm2].rotation_.x, 0.01f);
	ImGui::DragFloat3("Lha", &parts[Parts::LHand].rotation_.x, 0.01f);

	ImGui::DragFloat3("RA1R", &parts[Parts::RArm1].rotation_.x, 0.01f);
	ImGui::DragFloat3("RA2R", &parts[Parts::RArm2].rotation_.x, 0.01f);
	ImGui::DragFloat3("RhaR", &parts[Parts::RHand].rotation_.x, 0.01f);

	ImGui::DragFloat3("LL1R", &parts[Parts::LLeg1].rotation_.x, 0.01f);
	ImGui::DragFloat3("LL2R", &parts[Parts::LLeg2].rotation_.x, 0.01f);
	ImGui::DragFloat3("LFR", &parts[Parts::LFoot].rotation_.x, 0.01f);

	ImGui::DragFloat3("RL1R", &parts[Parts::RLeg1].rotation_.x, 0.01f);
	ImGui::DragFloat3("RL2R", &parts[Parts::RLeg2].rotation_.x, 0.01f);
	ImGui::DragFloat3("RFR", &parts[Parts::RFoot].rotation_.x, 0.01f);

#endif // _DEBUG
	
	

	UpdateAllMatrix();

}

/// <summary>
/// 更新
/// </summary>
void Player::Update() {
	Input::GetInstance()->GetJoystickState(0, joyState);

	if (!canBulletShot_) {
		if (--shotcooltime_ <= 0) {
			shotcooltime_ = 0;
			canBulletShot_ = true;
		}
	}
	GetStatus();
	//基準点の座標を取得
	targetW_.translation_ = target_->translation_;
#ifdef _DEBUG
	ImGui::Begin("Player");

	
#endif // _DEBUG
	Move();
	//camera to fov
	float length = playerMoveW.translation_.z+(-20);
	//震度情報送る
	reticle_->SetDepth(-playerMoveW.translation_.z);
	reticle_->Update(length);
	Attack();

#ifdef _DEBUG
	ImGui::End();
#endif // _DEBUG
	
	
}

//タイトルの時の更新
void Player::TitleUpdate(float Clong) {
	Input::GetInstance()->GetJoystickState(0, joyState);

	if (!canBulletShot_) {
		if (--shotcooltime_ <= 0) {
			shotcooltime_ = 0;
			canBulletShot_ = true;
		}
	}

	GetStatus();
	// 基準点の座標を取得(y軸は除く
	targetW_.translation_ = {target_->translation_.x,0,target_->translation_.z};
#ifdef _DEBUG
	ImGui::Begin("Player");
	ImGui::Text("player");
	ImGui::DragFloat3("position", &worldtransform_.translation_.x, 0.01f);
	ImGui::DragFloat3("ro", &worldtransform_.rotation_.x, 0.01f);
	ImGui::DragFloat3("scale", &worldtransform_.scale_.x, 0.01f);

	ImGui::Text("player stand area");
	ImGui::Text("pos   : %4.1f/%4.1f/%4.1f", playerMoveW.translation_.x, 0.01f);
	ImGui::Text("rotate: %4.1f/%4.1f/%4.1f", playerMoveW.rotation_.x, 0.01f);
	ImGui::Text("scale : %4.1f/%4.1f/%4.1f", playerMoveW.scale_.x, 0.01f);

	ImGui::Text("player stand area Move Normal");
	ImGui::DragFloat3("rotate", &targetW_.rotation_.x, 0.01f);
	ImGui::DragFloat("far to target", &playerMoveW.translation_.z);
	ImGui::DragFloat3("pos", &playerMoveW.translation_.x, 0.01f);
	
	ImGui::Text("pushcount:canShot:shotcooltime / %d:%d:%d", PushingCount_,canBulletShot_,shotcooltime_);
	ImGui::End();
#endif // _DEBUG
	
	#ifdef _DEBUG
	ImGui::Text("PARTS");
	ImGui::DragFloat3("BT", &parts[Parts::Body].translation_.x, 0.01f);
	ImGui::DragFloat3("HT", &parts[Parts::Head].translation_.x, 0.01f);
	ImGui::DragFloat3("BUT", &parts[Parts::BodyUnder].translation_.x, 0.01f);

	ImGui::DragFloat3("LA1T", &parts[Parts::LArm1].translation_.x, 0.01f);
	ImGui::DragFloat3("LA2T", &parts[Parts::LArm2].translation_.x, 0.01f);
	ImGui::DragFloat3("Lha", &parts[Parts::LHand].translation_.x, 0.01f);

	ImGui::DragFloat3("RA1T", &parts[Parts::RArm1].translation_.x, 0.01f);
	ImGui::DragFloat3("RA2T", &parts[Parts::RArm2].translation_.x, 0.01f);
	ImGui::DragFloat3("Rha", &parts[Parts::RHand].translation_.x, 0.01f);

	ImGui::DragFloat3("LL1T", &parts[Parts::LLeg1].translation_.x, 0.01f);
	ImGui::DragFloat3("LL2T", &parts[Parts::LLeg2].translation_.x, 0.01f);
	ImGui::DragFloat3("LFT", &parts[Parts::LFoot].translation_.x, 0.01f);

	ImGui::DragFloat3("RL1T", &parts[Parts::RLeg1].translation_.x, 0.01f);
	ImGui::DragFloat3("RL2T", &parts[Parts::RLeg2].translation_.x, 0.01f);
	ImGui::DragFloat3("RFT", &parts[Parts::RFoot].translation_.x, 0.01f);

	ImGui::Text("PARTSR");
	ImGui::DragFloat3("BR", &parts[Parts::Body].rotation_.x, 0.01f);
	ImGui::DragFloat3("HR", &parts[Parts::Head].rotation_.x, 0.01f);
	ImGui::DragFloat3("BUR", &parts[Parts::BodyUnder].rotation_.x, 0.01f);

	ImGui::DragFloat3("LA1R", &parts[Parts::LArm1].rotation_.x, 0.01f);
	ImGui::DragFloat3("LA2R", &parts[Parts::LArm2].rotation_.x, 0.01f);
	ImGui::DragFloat3("LhaR", &parts[Parts::LHand].rotation_.x, 0.01f);

	ImGui::DragFloat3("RA1R", &parts[Parts::RArm1].rotation_.x, 0.01f);
	ImGui::DragFloat3("RA2R", &parts[Parts::RArm2].rotation_.x, 0.01f);
	ImGui::DragFloat3("RhaR", &parts[Parts::RHand].rotation_.x, 0.01f);

	ImGui::DragFloat3("LL1R", &parts[Parts::LLeg1].rotation_.x, 0.01f);
	ImGui::DragFloat3("LL2R", &parts[Parts::LLeg2].rotation_.x, 0.01f);
	ImGui::DragFloat3("LFR", &parts[Parts::LFoot].rotation_.x, 0.01f);

	ImGui::DragFloat3("RL1R", &parts[Parts::RLeg1].rotation_.x, 0.01f);
	ImGui::DragFloat3("RL2R", &parts[Parts::RLeg2].rotation_.x, 0.01f);
	ImGui::DragFloat3("RFR", &parts[Parts::RFoot].rotation_.x, 0.01f);

#endif // _DEBUG

	//行列更新まとめ
	UpdateAllMatrix();
	
	//カメラからターゲットまでのZ
	float length = playerMoveW.translation_.z + Clong;
	// 震度情報送る
	reticle_->SetDepth(-playerMoveW.translation_.z);
	reticle_->Update(length);

	Attack();
}

//行列更新まとめ
void Player::UpdateAllMatrix() {
	// 行列更新
	targetW_.UpdateMatrix();
	playerMoveW.UpdateMatrix();
	// playermoveを中心として移動
	worldtransform_.UpdateMatrix();

	for (int i = 0; i < partsNum; i++) {
		parts[i].UpdateMatrix();
	}
}


/// <summary>
/// 描画
/// </summary>
void Player::Draw(const ViewProjection& view) { 
	//仮モデルの描画
	//models_[0]->Draw(worldtransform_, view);
	//レティクル描画
	reticle_->Draw(view);


	for (int i = 0; i < partsNum; i++) {
		models_[i]->Draw(parts[i], view);
	}
	//models_[Parts::Head]->Draw(parts[Parts::Head], view);
}

void Player::DrawUI() { 
	
	reticle_->DrawSPrite();

	

	for (int i = 0; i < (int)hp_; i++) {
		const int texheight = 720 - 40;

		float x = 1280.0f - (64.0f * (float)hp_);
		x /= 2;

		Vector2 pos = {x+64*i, texheight};
		hpsprite[i]->SetPosition(pos);

		hpsprite[i]->Draw();
		
	}

}