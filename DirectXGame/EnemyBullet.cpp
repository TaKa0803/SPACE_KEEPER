
#include"math_matrix.h"
#include"EnemyBullet.h"


void EnemyBullet::Initialize(
    Model* model, const Vector3& position, const Vector3& velocityN, const WorldTransform* taargetW,BType type,bool under50,float scale) {

	model_ = model;
	world_.Initialize();
	world_.translation_ = position;
	world_.scale_ = {5*scale, 5*scale, 5*scale};
	
	
	velocity_ = velocityN;

	targetW_ = taargetW;

	BulletType = type;

	isAngry_ = under50;

	if (!under50) {
		if (type == BType::Normal) {
			BulletSpd_ = 5;
		} else {
			BulletSpd_ = 1;
			chasewaitT = 30;
		}
	} else {
		if (type == BType::Normal) {
			BulletSpd_ = 7;
		} else {
			BulletSpd_ = 4;
			chasewaitT = 5;
		}
	}
}





void EnemyBullet::Update() {

	if (!isAngry_) {

		velocity_ = Multiply(BulletSpd_, Normalize(velocity_));

		Vector2 rt;
		Vector3 rotate;

		Vector3 pvelo;
		esing E;

		// float pi2 = (3.14f / 2);
		float pi2 = 0;

		switch (BulletType) {
		case BType::Normal:
			world_.translation_ = Add(world_.translation_, velocity_);

			rt = CheckRotateFromVelo(velocity_);
			rotate = {rt.x, rt.y, pi2};
			world_.rotation_ = rotate;

			break;
		case BType::Chase:

#pragma region chase
			Vector3 velo;

			switch (cmode_) {
			case Cmode::start:

				velo = velocity_;
				if (--chasewaitT <= 0) {
					cmode_ = Cmode::chase;
				}

				break;
			case Cmode::chase:

				pvelo = Subtract(GetTW(), GetW());
				pvelo = Multiply(BulletSpd_, Normalize(pvelo));

				E = {velocity_, pvelo};
				// イージングしたベクトル
				velo = ES(E, T);

				T += 1.0f / 60.0f;

				if (T > 1.0f) {
					velocity_ = velo;
					cmode_ = Cmode::none;
				}

				break;
			case Cmode::none:
				velo = velocity_;
				break;
			default:
				break;
			}

			world_.translation_ = Add(world_.translation_, velo);

			rt = CheckRotateFromVelo(velo);
			rotate = {rt.x, rt.y, pi2};
			world_.rotation_ = rotate;
#pragma endregion

			break;
		default:
			break;
		}
	} else {
		velocity_ = Multiply(BulletSpd_, Normalize(velocity_));

		Vector2 rt;
		Vector3 rotate;

		Vector3 pvelo;
		esing E;

		// float pi2 = (3.14f / 2);
		float pi2 = 0;

		switch (BulletType) {
		case BType::Normal:
			world_.translation_ = Add(world_.translation_, velocity_);

			rt = CheckRotateFromVelo(velocity_);
			rotate = {rt.x, rt.y, pi2};
			world_.rotation_ = rotate;

			break;
		case BType::Chase:

#pragma region chase
			Vector3 velo;

			switch (cmode_) {
			case Cmode::start:

				velo = velocity_;
				if (--chasewaitT <= 0) {
					cmode_ = Cmode::chase;
				}

				break;
			case Cmode::chase:

				pvelo = Subtract(GetTW(), GetW());
				pvelo = Multiply(BulletSpd_, Normalize(pvelo));

				E = {velocity_, pvelo};
				// イージングしたベクトル
				velo = ES(E, T);

				T += 1.0f / 60.0f;

				if (T > 1.0f) {
					velocity_ = velo;
					cmode_ = Cmode::none;
				}

				break;
			case Cmode::none:
				velo = velocity_;
				break;
			default:
				break;
			}

			world_.translation_ = Add(world_.translation_, velo);

			rt = CheckRotateFromVelo(velo);
			rotate = {rt.x, rt.y, pi2};
			world_.rotation_ = rotate;
#pragma endregion

			break;
		default:
			break;
		}
	}

	world_.UpdateMatrix();


	if (--deathTimer <= 0) {
		isDead_ = true;
	}
}

void EnemyBullet::Draw(ViewProjection view) { 
	model_->Draw(world_, view);
}