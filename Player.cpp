#include"player.h"
#include<ImGuiManager.h>
#include<cmath>
#include"math_matrix.h"
#include<GameScene.h>
#include<iostream>

void Player::SetStart() {
	hp_ = 10;
	worldtransform_.translation_ = {0, -3.5f, 0};
	worldtransform_.rotation_ = {0, 0, 0};
	worldtransform_.scale_ = {1, 1, 1};
	// ターゲットとの距離（Z)
	playerMoveW.translation_ = {0, 0, -40.0f};
	playerMoveW.rotation_ = {0, 0, 0};

	bodyW_.translation_ = {0, 0.5f, 0};
	bodyW_.rotation_ = {0, 0, 0};
	headW_.translation_ = {0, 0.3f, 0};
	LlegW_.translation_ = {-0.18f, -0.6f, 0};
	RlegW_.translation_ = {0.18f, -0.6f, 0};
	jettpack_.translation_ = {0, -0.1f, -0.64f};

	fire_.translation_ = {0, -0.3f, 0};
	fire_.scale_ = {0.8f, 1, 0.8f};

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


	bodyW_.Initialize();
	bodyW_.translation_ = {0, 0.5f, 0};
	headW_.Initialize();
	headW_.translation_ = {0, 0.3f, 0};
	LhandW_.Initialize();
	RhandW_.Initialize();
	LlegW_.Initialize();
	LlegW_.translation_ = {-0.18f, -0.6f, 0};
	RlegW_.Initialize();
	RlegW_.translation_ = {0.18f, -0.6f, 0};
	weaponW_.Initialize();

	jettpack_.Initialize();
	jettpack_.translation_ = {0, -0.1f, -0.64f};
	fire_.Initialize();
	fire_.translation_ = {0, -0.3f, 0};
	fire_.scale_ = {0.8f, 1, 0.8f};
	bodyW_.parent_ = &worldtransform_;
	headW_.parent_ = &bodyW_;
	LhandW_.parent_ = &bodyW_;
	RhandW_.parent_ = &bodyW_;
	LlegW_.parent_ = &bodyW_;
	RlegW_.parent_ = &bodyW_;

	weaponW_.parent_ = &bodyW_;

	jettpack_.parent_ = &bodyW_;
	fire_.parent_ = &jettpack_;
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
	if (--hp <= 0) {
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

		weaponW_.rotation_.x = 0.5f;
		LhandW_.rotation_.x = 0.5f;
		RhandW_.rotation_.x = 0.5f;
	

		
	}

	
	
	fire_.rotation_.y += (1.0f / 30.0f) * pi;

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
	ImGui::DragFloat3("p rotate", &worldtransform_.rotation_.x, 0.01f);
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
	ImGui::DragFloat3("head p", &headW_.rotation_.x, 0.01f);
	ImGui::DragFloat3("body p", &bodyW_.rotation_.x, 0.01f);

	ImGui::DragFloat3("LH p", &LhandW_.rotation_.x, 0.01f);
	ImGui::DragFloat3("RH p", &RhandW_.rotation_.x, 0.01f);

	ImGui::DragFloat3("LL p", &LlegW_.rotation_.x, 0.01f);
	ImGui::DragFloat3("RL p", &RlegW_.rotation_.x, 0.01f);

	ImGui::DragFloat3("weapon p", &weaponW_.rotation_.x, 0.01f);

	ImGui::DragFloat3("jett p " ,&jettpack_.translation_.x, 0.01f);
	ImGui::DragFloat3("jett p ", &fire_.translation_.x, 0.01f);

#endif // _DEBUG
	if (isShot) {
		weaponW_.rotation_.x = 0.0f;
		LhandW_.rotation_.x = 0.0f;
		RhandW_.rotation_.x = 0.0f;
	} else {
		weaponW_.rotation_.x = 0.5f;
		LhandW_.rotation_.x = 0.5f;
		RhandW_.rotation_.x = 0.5f;
	
	}
	//プレイヤーのいちによって傾き
	bodyW_.rotation_.z = (worldtransform_.translation_.x/-area)*pi*(1.0f/6.0f);
	bodyW_.rotation_.x = (worldtransform_.translation_.z / area) * pi * (1.0f / 10.0f);

	fire_.rotation_.y += (1.0f / 30.0f) * pi;

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
	ImGui::DragFloat3("rotate", &worldtransform_.rotation_.x, 0.01f);
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
	if (isShot) {
		weaponW_.rotation_.x = 0.0f;
		LhandW_.rotation_.x = 0.0f;
		RhandW_.rotation_.x = 0.0f;
	} else {
		weaponW_.rotation_.x = 0.5f;
		LhandW_.rotation_.x = 0.5f;
		RhandW_.rotation_.x = 0.5f;
	}
	fire_.rotation_.y += (1.0f / 30.0f) * pi;

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

	bodyW_.UpdateMatrix();
	headW_.UpdateMatrix();
	LhandW_.UpdateMatrix();
	RhandW_.UpdateMatrix();
	LlegW_.UpdateMatrix();
	RlegW_.UpdateMatrix();
	weaponW_.UpdateMatrix();

	jettpack_.UpdateMatrix();
	fire_.UpdateMatrix();
}

/// <summary>
/// 描画
/// </summary>
void Player::Draw(const ViewProjection& view) { 
	//仮モデルの描画
	//models_[0]->Draw(worldtransform_, view);
	//レティクル描画
	reticle_->Draw(view);

	//武器
	models_[0]->Draw(weaponW_, view);
	//身体
	models_[1]->Draw(bodyW_, view);
	//頭
	models_[2]->Draw(headW_, view);
	//腕
	models_[3]->Draw(LhandW_, view);
	models_[4]->Draw(RhandW_, view);
	//足
	models_[5]->Draw(LlegW_, view);
	models_[6]->Draw(RlegW_, view);

	//じぇっぱ
	models_[7]->Draw(jettpack_, view);


	if (! isShot) {
		// 炎
		models_[8]->Draw(fire_, view);
	}
}

void Player::DrawUI() { reticle_->DrawSPrite(); }