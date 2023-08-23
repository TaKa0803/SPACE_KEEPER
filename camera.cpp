#include"camera.h"
#include"math_matrix.h"
#include<ImGuiManager.h>


Vector2 Esing(const Vector2 zero,const Vector2 one, float t) {
	return {
	    zero.x * (1.0f - t) + one.x * t,
	    zero.y * (1.0f - t) + one.y * t,
	};
}

void Camera::Initialize(const ViewProjection& world,Model*model,float farZ) { 

	input_ = Input::GetInstance();

	world_.Initialize();
	
	view_.farZ = farZ*2.0f+500.0f;
	view_.Initialize();
	
	world;
	cameraWorld_.Initialize();
	// 注目アイテムと距離設定
	cameraWorld_.translation_ = {0, 0, -50};

	reticleWorld_.Initialize();
	reticleWorld_.translation_ = {0, 0, 30};
	reticleWorld_.parent_ = &world_;

	model_ = model;

	maxBackVec_ = {0, 0};
}









void Camera::cameraRotate() { 


	Vector2 zero = {0, 0};
	Vector2 retipos = {reticleWorld_.translation_.x, reticleWorld_.translation_.y};
	Vector2 velo = {retipos.x - zero.x, retipos.y - zero.y};
	float length = sqrtf(velo.x*velo.x+velo.y*velo.y);

	

	//枠外にレティクルが出ていたら
	if (length > area) {
		Vector2 newpos = {
		    area*(velo.x / length),
		    area*(velo.y / length),
		};
		reticleWorld_.translation_ = {newpos.x, newpos.y, reticleWorld_.translation_.z};
	};
	// 距離によって向く量変更
	float X = reticleWorld_.translation_.x / area;
	float Y = reticleWorld_.translation_.y / area;

	//
	X = sqrtf(X * X);
	Y = sqrtf(Y * Y);

	//カメラ方向処理
#pragma region カメラのworldの変更
	if (reticleWorld_.translation_.y > 0) {
		world_.rotation_.x -= kRotateTheta * Y;
	} else {
		world_.rotation_.x += kRotateTheta * Y;
	}

	// 一周超えたら数値を下げる（叔母風呂対策
	if (world_.rotation_.x > pi * 2.0f) {
		world_.rotation_.x -= pi * 2.0f;
	} else if (world_.rotation_.x < -(pi * 2.0f)) {
		world_.rotation_.x += pi * 2.0f;
	}

	// プラス方向
	if (world_.rotation_.x > pi * (1.0f / 2.0f) && world_.rotation_.x < pi * (3.0f / 2.0f)) {
		if (reticleWorld_.translation_.x > 0) {
			world_.rotation_.y -= kRotateTheta * X;
		} else {
			world_.rotation_.y += kRotateTheta * X;
		}
	} else if (world_.rotation_.x > 0.0f) {
		if (reticleWorld_.translation_.x > 0) {
			world_.rotation_.y += kRotateTheta * X;
		} else {
			world_.rotation_.y -= kRotateTheta * X;
		}
	}

	if (world_.rotation_.x < -(pi * (1.0f / 2.0f)) && world_.rotation_.x > -(pi * (3.0f / 2.0f))) {
		if (reticleWorld_.translation_.x > 0) {
			world_.rotation_.y -= kRotateTheta * X;
		} else {
			world_.rotation_.y += kRotateTheta * X;
		}
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
void Camera::BackCamera() {
	// キー入力のないときに０地点に戻る
	if (!input_->PushKey(DIK_W) &&
		!input_->PushKey(DIK_A) &&
		!input_->PushKey(DIK_S) &&
	    !input_->PushKey(DIK_D)
		) 
	{
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

	//フラグ作動中の移動処理
	if (isBackRetcle_) {
		//ゼロに向かう
		easingT_ -= addT_;

		//ゼロ以下で終了
		if (easingT_ <= 0.0f) {
			easingT_ = 0.0f;
			isBackRetcle_ =false;
		}
		//イージングして移動
		Vector2 newP = Esing(zeroP_, maxBackVec_,easingT_);

		reticleWorld_.translation_.x = newP.x;
		reticleWorld_.translation_.y = newP.y;
	}


}

void Camera::Update() { 
	#ifdef _DEBUG
	ImGui::Begin("MainCamera");
#endif // _DEBUG



	//ターゲットから座標を取得した行列を作る
	
	world_.translation_ = target_->translation_;
	
	if (input_->PushKey(DIK_W)) {
		reticleWorld_.translation_.y += 1.0f;
	}
	if (input_->PushKey(DIK_S)) {
		reticleWorld_.translation_.y -= 1.0f;
	}
	if (input_->PushKey(DIK_A)) {
		reticleWorld_.translation_.x -= 1.0f;
	}
	if (input_->PushKey(DIK_D)) {
		reticleWorld_.translation_.x += 1.0f;
	}

#ifdef _DEBUG
	ImGui::Text("Reticle");
	ImGui::DragFloat3("pos", &reticleWorld_.translation_.x, 0.01f);
	ImGui::DragFloat3("rotate", &reticleWorld_.rotation_.x, 0.01f);
	ImGui::DragFloat3("scale", &reticleWorld_.scale_.x, 0.01f);
#endif // _DEBUG

	BackCamera();
	
	// エリア超えていたらカメラも回転
	cameraRotate();
	
	reticleWorld_.UpdateMatrix();



#ifdef _DEBUG
	ImGui::Text("camera");
	ImGui::DragFloat3("rotation",&world_.rotation_.x,0.01f);
	ImGui::DragFloat("far",&cameraWorld_.translation_.z, 0.01f);
	
#endif // _DEBUG



	world_.UpdateMatrix();

	cameraWorld_.UpdateMatrix();
	cameraWorld_.matWorld_ = Multiply(cameraWorld_.matWorld_, world_.matWorld_);

	
	view_.matView = Inverse(cameraWorld_.matWorld_);
	view_.TransferMatrix();

#ifdef _DEBUG
	ImGui::End();
#endif // _DEBUG
}

void Camera::Draw(ViewProjection view) { model_->Draw(reticleWorld_, view); }