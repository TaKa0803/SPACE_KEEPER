#include"Reticle.h"
#include<ImGuiManager.h>
#include"math_matrix.h"

Vector2 Esing(const Vector2 zero, const Vector2 one, float t) {
	return {
	    zero.x * (1.0f - t) + one.x * t,
	    zero.y * (1.0f - t) + one.y * t,
	};
}


void Reticle::Initialize(Model* model, const WorldTransform& parent) {
	input_ = Input::GetInstance();
	
	reticleWorld_.Initialize();
	reticleWorld_.translation_ = NormalPos;
	reticleWorld_.parent_ = &parent;

	model_ = model;
}

void Reticle::Move() {

	//キー入力による移動
	if (input_->PushKey(DIK_W)) {
		reticleWorld_.translation_.y += moveNum;
	}
	if (input_->PushKey(DIK_S)) {
		reticleWorld_.translation_.y -= moveNum;
	}
	if (input_->PushKey(DIK_A)) {
		reticleWorld_.translation_.x -= moveNum;
	}
	if (input_->PushKey(DIK_D)) {
		reticleWorld_.translation_.x += moveNum;
	}

	//枠外に出てないかのチェック
	Vector2 zero = {0, 0};
	Vector2 retipos = {reticleWorld_.translation_.x, reticleWorld_.translation_.y};
	Vector2 velo = SubV2(retipos, zero);
	float length = LengV2(velo);

	// 枠外にレティクルが出ていたら
	if (length > area) {
		//長さを正規化して最大値まで伸ばす
		Vector2 newpos = ScaV2(area, NorV2(velo));
		//伸ばした値を代入
		reticleWorld_.translation_ = {newpos.x, newpos.y, reticleWorld_.translation_.z};
	};


}

void Reticle::CameraRotate(WorldTransform& world_) {
	// 距離によって向く量変更
	float X = reticleWorld_.translation_.x / area;
	float Y = reticleWorld_.translation_.y / area;

	//プラスに修正
	X = sqrtf(X * X);
	Y = sqrtf(Y * Y);

	// カメラの回転処理
#pragma region カメラのworldの変更

	//ゼロでなければ回転
	if (reticleWorld_.translation_.y > 0) {
		world_.rotation_.x -= kRotateTheta * Y;
	} else {
		world_.rotation_.x += kRotateTheta * Y;
	}
	// 一周超えたら数値を下げる（オーバーフロー対策
	if (world_.rotation_.x > pi * 2.0f) {
		world_.rotation_.x -= pi * 2.0f;
	} else if (world_.rotation_.x < -(pi * 2.0f)) {
		world_.rotation_.x += pi * 2.0f;
	}

	/// 画面の上下と世界の上下が逆転した際の左右の回転軸の加算処理
	//正面から+1/2Π以上+3/2未満の場合の処理
	if (world_.rotation_.x > pi * (1.0f / 2.0f) && world_.rotation_.x < pi * (3.0f / 2.0f)) {
		
		//レティクルの位置関係から加算する符号を変える
		if (reticleWorld_.translation_.x > 0) {
			world_.rotation_.y -= kRotateTheta * X;
		} else {
			world_.rotation_.y += kRotateTheta * X;
		}
	//それ以外の+角度の場合
	} else if (world_.rotation_.x > 0.0f) {
		if (reticleWorld_.translation_.x > 0) {
			world_.rotation_.y += kRotateTheta * X;
		} else {
			world_.rotation_.y -= kRotateTheta * X;
		}
	}
	// 正面から-1/2Π以上-3/2未満の場合の処理
	if (world_.rotation_.x < -(pi * (1.0f / 2.0f)) && world_.rotation_.x > -(pi * (3.0f / 2.0f))) {
		if (reticleWorld_.translation_.x > 0) {
			world_.rotation_.y -= kRotateTheta * X;
		} else {
			world_.rotation_.y += kRotateTheta * X;
		}
	// それ以外の-角度の場合
	} else if (world_.rotation_.x < 0.0f) {
		if (reticleWorld_.translation_.x > 0) {
			world_.rotation_.y += kRotateTheta * X;
		} else {
			world_.rotation_.y -= kRotateTheta * X;
		}
	}

	// 一周超えたら数値を下げる（叔母風呂対策
	if (world_.rotation_.y > pi * 2.0f) {
		world_.rotation_.y -= pi * 2.0f;
	} else if (world_.rotation_.y < -(pi * 2.0f)) {
		world_.rotation_.y += pi * 2.0f;
	}
#pragma endregion
}

void Reticle::Back() {
	// キー入力のないときに０地点に戻る
	if (!input_->PushKey(DIK_W) && !input_->PushKey(DIK_A) && !input_->PushKey(DIK_S) &&
	    !input_->PushKey(DIK_D)) {
		if (!isBackRetcle_) {
			// レティクル戻す処理フラグ起動
			isBackRetcle_ = true;
			// Z軸を無視したプレイヤーの位置を取得
			Vector2 player = {reticleWorld_.translation_.x, reticleWorld_.translation_.y};

			// もしすでにゼロ地点なら処理をやめる
			if (player.x == 0 && player.y == 0) {
				isBackRetcle_ = false;
				// これ以上やることないので戻る
				return;
			}
			// ゼロ地点から現時点の位置のベクトル算出
			maxBackVec_ = SubV2(player, zeroP_);

			// 初期化のち最大移動地点までベクトルを伸ばす
			maxBackVec_ = ScaV2(area, NorV2(maxBackVec_));

			// 最大値点の長さ
			float Ts = LengV2(maxBackVec_);

			//
			float T = LengV2(player);

			// 割合を出す
			easingT_ = T / Ts;
		}
	} else {
		isBackRetcle_ = false;
	}

	// フラグ作動中の移動処理
	if (isBackRetcle_) {
		// ゼロに向かう
		easingT_ -= addT_;

		// ゼロ以下で終了
		if (easingT_ <= 0.0f) {
			easingT_ = 0.0f;
			isBackRetcle_ = false;
		}
		// イージングして移動
		Vector2 newP = Esing(zeroP_, maxBackVec_, easingT_);

		reticleWorld_.translation_.x = newP.x;
		reticleWorld_.translation_.y = newP.y;
	}
}



void Reticle::Update(WorldTransform&world_) { 
	
	Move();
	CameraRotate(world_);
	reticleWorld_.UpdateMatrix();

}

void Reticle::Draw(ViewProjection view) { 

	model_->Draw(reticleWorld_, view);

}