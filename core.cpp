#include<core.h>
#include<cassert>
#include<ImGuiManager.h>
#include<numbers>
#include"GameScene.h"

void BCore::ShotNormal(float scale) { 
	Vector3 velo;



	velo = Normalize(Subtract(GetRHW(), GetR2CW()));

	gameScene_->ShotEN(models_[12],GetRHW(),velo,scale); }

void BCore::ShotChase(float scale) { 
	Vector3 velo;
	velo = Normalize(Subtract(GetLHW(), GetL2CW()));
	gameScene_->ShotEC(models_[11], GetLHW(), velo,scale); 
}


void BCore::SetStart() {

	SeeTarget();

	hp_ = maxHP;
	worldtransform_.translation_ = {0, 0, 0};
	worldtransform_.scale_ = {5, 5, 5};

	head_.translation_ = {0, 1.6f, 0};
	C_center_.translation_ = {0, -3, 0};
	leg_.translation_ = {0, -1.2f, 0};

	LArm1_.translation_ = {-1.3f, 0, 0};
	LArm2_.translation_ = {-1.3f, 0, 0};
	Lhand_.translation_ = {-7, 0, 0};

	C_L1_.translation_ = {-3.0f, 0, 0};
	C_L2_.translation_ = {-7.0f, 0, 0};

	RArm1_.translation_ = {1.3f, 0, 0};
	RArm2_.translation_ = {1.3f, 0, 0};
	Rhand_.translation_ = {7, 0, 0};

	C_R1_.translation_ = {3.0f, 0, 0};
	C_R2_.translation_ = {7.0f, 0, 0};

	head_.rotation_ = {1.0f, 0, 0};
	body_.rotation_ = {0, 0, 0};
	leg_.rotation_ = {0, 0, 0};
	LArm1_.rotation_ = {0, 0, 0};
	LArm2_.rotation_ = {0, 0, 0};
	RArm1_.rotation_ = {0, 0, 0};
	RArm2_.rotation_ = {0, 0, 0};
	Lhand_.rotation_ = {0, 0, 0};
	Rhand_.rotation_ = {0, 0, 0};

	C_center_.rotation_ = {0, 0, 0};
	C_L1_.rotation_ = {0, 0, 1.5f};
	C_L2_.rotation_ = {-1.5f, 0, 0};

	C_R1_.rotation_ = {0, 0, -1.5f};
	C_R2_.rotation_ = {-1.5f, 0, 0};

	Lhand_.scale_ = {1, 1, 1};
	Rhand_.scale_ = {1, 1, 1};

	state_ = StateCore::Start;

	weakC = WeakCore::L1;

	MoveWave_ = MoveE::attackWait;

	T = 0;
	firstAction_ = false;
	isDead_ = false;

	cgangecount = 0;

	UpdateAllMatrix();
}

void BCore::Initialize(const std::vector<Model*>& models) {
	//assert(model);
	BaseCharacter::Initialize(models, maxHP);
	
	worldtransform_.translation_ = {0, 0, 0};
	worldtransform_.scale_ = {5, 5, 5};


	body_.Initialize();
	head_.Initialize();
	leg_.Initialize();
	LArm1_.Initialize();
	LArm2_.Initialize();
	Lhand_.Initialize();

	RArm1_.Initialize();
	RArm2_.Initialize();
	Rhand_.Initialize();

	C_center_.Initialize();
	C_L1_.Initialize();
	C_L2_.Initialize();

	C_R1_.Initialize();
	C_R2_.Initialize();

	//親子関係

	body_.parent_ = &worldtransform_;
	head_.parent_ = &body_;
	C_center_.parent_ = &body_;
	leg_.parent_ = &C_center_;

	C_L1_.parent_ = &body_;
	LArm1_.parent_ = &C_L1_;
	C_L2_.parent_ = &C_L1_;
	LArm2_.parent_ = &C_L2_;
	Lhand_.parent_ = &C_L2_;

	C_R1_.parent_ = &body_;
	RArm1_.parent_ = &C_R1_;
	C_R2_.parent_ = &C_R1_;
	RArm2_.parent_ = &C_R2_;
	Rhand_.parent_ = &C_R2_;


	head_.translation_ = {0, 1.6f, 0};
	C_center_.translation_ = {0, -3, 0};
	leg_.translation_ = {0, -1.2f, 0};

	LArm1_.translation_ = {-1.3f, 0, 0};
	LArm2_.translation_ = {-1.3f, 0, 0};
	Lhand_.translation_ = {-7, 0, 0};

	C_L1_.translation_ = {-3.0f, 0, 0};
	C_L2_.translation_ = {-7.0f, 0, 0};

	RArm1_.translation_ = {1.3f, 0, 0};
	RArm2_.translation_ = {1.3f, 0, 0};
	Rhand_.translation_ = {7, 0, 0};

	C_R1_.translation_ = {3.0f, 0, 0};
	C_R2_.translation_ = {7.0f, 0, 0};

	head_.rotation_ = {1.0f, 0, 0};
	body_.rotation_ = {0, 0, 0};
	leg_.rotation_ = {0, 0, 0};
	LArm1_.rotation_ = {0, 0, 0};
	LArm2_.rotation_ = {0, 0, 0};
	RArm1_.rotation_ = {0, 0, 0};
	RArm2_.rotation_ = {0, 0, 0};
	Lhand_.rotation_ = {0, 0, 0};
	Rhand_.rotation_ = {0, 0, 0};

	C_center_.rotation_ = {0, 0, 0};
	C_L1_.rotation_ = {0, 0, 1.5f};
	C_L2_.rotation_ = {-1.5f, 0, 0};

	C_R1_.rotation_ = {0, 0, -1.5f};
	C_R2_.rotation_ = {-1.5f, 0, 0};

	Lhand_.scale_ = {1, 1, 1};
	Rhand_.scale_ = {1, 1, 1};
	

	UpdateAllMatrix();
}

//プレイヤーのほうを向く
void BCore::SeeTarget() { 
	Vector3 velo = Subtract(GetTargetW(), GetmatW());
	Vector2 v2 = {velo.x, velo.z};
	worldtransform_.rotation_.y = CheckR_F_Y(v2);
}

void BCore::Start() {
	if (!firstAction_) {
		SeeTarget();
		firstAction_ = true;
	}

	head_.rotation_ = {1.0f, 0, 0};
	body_.rotation_ = {0, 0, 0};
	leg_.rotation_ = {0, 0, 0};
	LArm1_.rotation_ = {0, 0, 0};
	LArm2_.rotation_ = {0, 0, 0};
	RArm1_.rotation_ = {0, 0, 0};
	RArm2_.rotation_ = {0, 0, 0};
	Lhand_.rotation_ = {0, 0, 0};
	Rhand_.rotation_ = {0, 0, 0};

	C_center_.rotation_ = {0, 0, 0};
	C_L1_.rotation_ = {0, 0, 1.5f};
	C_L2_.rotation_ = {-1.5f, 0, 0};

	C_R1_.rotation_ = {0, 0, -1.5f};
	C_R2_.rotation_ = {-1.5f, 0, 0};

	if (hp_ < maxHP) {
		state_ = StateCore::StartAnimation;
		firstAction_ = false;
		MoveWave_ = MoveE::attackWait;
	}
}

void BCore::StartAnime() {
switch (MoveWave_) {
	case MoveE::attackWait:
#pragma region atkWait
		// 初期設定したか
		if (!firstAction_) {
			firstAction_ = true;
			weakC = WeakCore::None;
			// 攻撃予備動作
			T = 0;

			H_ = {
			    head_.rotation_, {-0.2f, 0, 0}
            };
			B_ = {body_.rotation_, body_.rotation_};
			Leg_ = {leg_.rotation_, leg_.rotation_};
			LA1_ = {LArm1_.rotation_, LArm1_.rotation_};
			LA2_ = {LArm2_.rotation_, LArm2_.rotation_};
			LH_ = {Lhand_.rotation_, Lhand_.rotation_};

			RA1_ = {RArm1_.rotation_, RArm1_.rotation_};
			RA2_ = {RArm2_.rotation_, RArm2_.rotation_};
			RH_ = {Rhand_.rotation_, Rhand_.rotation_};

			CC = {C_center_.rotation_, C_center_.rotation_};
			CL1_ = {
			    C_L1_.rotation_, {0, 0.2f, 0.2f}
            };
			CL2_ = {
			    C_L2_.rotation_, {-3.0f, 0, -0.4f}
            };

			CR1_ = {
			    C_R1_.rotation_, {0, -0.2f, -0.2f}
            };
			CR2_ = {
			    C_R2_.rotation_, {-3.0f, 0, 0.4f}
            };
		} else {
			// 設定したら更新
			head_.rotation_ = ES(H_, T);
			body_.rotation_ = ES(B_, T);
			leg_.rotation_ = ES(Leg_, T);
			LArm1_.rotation_ = ES(LA1_, T);
			LArm2_.rotation_ = ES(LA2_, T);
			Lhand_.rotation_ = ES(LH_, T);

			RArm1_.rotation_ = ES(RA1_, T);
			RArm2_.rotation_ = ES(RA2_, T);
			Rhand_.rotation_ = ES(RH_, T);

			C_center_.rotation_ = ES(CC, T);
			C_L1_.rotation_ = ES(CL1_, T);
			C_L2_.rotation_ = ES(CL2_, T);
			C_R1_.rotation_ = ES(CR1_, T);
			C_R2_.rotation_ = ES(CR2_, T);

			// T追加
			T += addWaitT;
			if (T > 1.0f) {
				T = 0;
				// 次のシーンに移動
				firstAction_ = false;
				MoveWave_ = MoveE::attack;
			}
		}
#pragma endregion	
		break;
	case MoveE::attack:
#pragma region attack
		if (!firstAction_) {
			firstAction_ = true;
			// 攻撃予備動作
			T = 0;
			H_ = {
			    head_.rotation_, {-0.2f, 0, 0}
            };
			B_ = {
			    body_.rotation_, {-0.2f, 0, 0}
            };
			Leg_ = {leg_.rotation_, leg_.rotation_};
			LA1_ = {LArm1_.rotation_, LArm1_.rotation_};
			LA2_ = {LArm2_.rotation_, LArm2_.rotation_};
			LH_ = {Lhand_.rotation_, Lhand_.rotation_};

			RA1_ = {RArm1_.rotation_, RArm1_.rotation_};
			RA2_ = {RArm2_.rotation_, RArm2_.rotation_};
			RH_ = {Rhand_.rotation_, Rhand_.rotation_};

			CC = {
			    C_center_.rotation_, {0.3f, 0, 0}
            };
			CL1_ = {
			    C_L1_.rotation_, {0, -0.1f, 0.2f}
            };
			CL2_ = {
			    C_L2_.rotation_, {-3.0f, 0, -0.1f}
            };

			CR1_ = {
			    C_R1_.rotation_, {0, -0.1f, -0.2f}
            };
			CR2_ = {
			    C_R2_.rotation_, {-3.0f, 0, 0.1f}
            };

		} else {

			// 設定したら更新
			head_.rotation_ = ES(H_, T);
			body_.rotation_ = ES(B_, T);
			leg_.rotation_ = ES(Leg_, T);
			LArm1_.rotation_ = ES(LA1_, T);
			LArm2_.rotation_ = ES(LA2_, T);
			Lhand_.rotation_ = ES(LH_, T);

			RArm1_.rotation_ = ES(RA1_, T);
			RArm2_.rotation_ = ES(RA2_, T);
			Rhand_.rotation_ = ES(RH_, T);

			C_center_.rotation_ = ES(CC, T);
			C_L1_.rotation_ = ES(CL1_, T);
			C_L2_.rotation_ = ES(CL2_, T);
			C_R1_.rotation_ = ES(CR1_, T);
			C_R2_.rotation_ = ES(CR2_, T);

			// T追加
			T += addAttackT;
			if (T > 1.0f) {
				T = 0;
				// 次のシーンに移動
				firstAction_ = false;
				MoveWave_ = MoveE::back;
			}
		}
#pragma endregion	
		break;
	case MoveE::back:
#pragma region back
		if (!firstAction_) {
			firstAction_ = true;
			// 攻撃予備動作
			T = 0;
			H_ = {head_.rotation_, NorRotate_.H};
			B_ = {body_.rotation_, NorRotate_.B};
			Leg_ = {leg_.rotation_, NorRotate_.leg};
			LA1_ = {LArm1_.rotation_, NorRotate_.LA1};
			LA2_ = {LArm2_.rotation_, NorRotate_.LA2};
			LH_ = {Lhand_.rotation_, NorRotate_.Lha};

			RA1_ = {RArm1_.rotation_, NorRotate_.RA1};
			RA2_ = {RArm2_.rotation_, NorRotate_.RA2};
			RH_ = {Rhand_.rotation_, NorRotate_.Rha};

			CC = {C_center_.rotation_, NorRotate_.CC};
			CL1_ = {C_L1_.rotation_, NorRotate_.CL1};
			CL2_ = {C_L2_.rotation_, NorRotate_.CL2};

			CR1_ = {C_R1_.rotation_, NorRotate_.CR1};
			CR2_ = {C_R2_.rotation_, NorRotate_.CR2};
		} else {
			// 設定したら更新
			head_.rotation_ = ES(H_, T);
			body_.rotation_ = ES(B_, T);
			leg_.rotation_ = ES(Leg_, T);
			LArm1_.rotation_ = ES(LA1_, T);
			LArm2_.rotation_ = ES(LA2_, T);
			Lhand_.rotation_ = ES(LH_, T);

			RArm1_.rotation_ = ES(RA1_, T);
			RArm2_.rotation_ = ES(RA2_, T);
			Rhand_.rotation_ = ES(RH_, T);

			C_center_.rotation_ = ES(CC, T);
			C_L1_.rotation_ = ES(CL1_, T);
			C_L2_.rotation_ = ES(CL2_, T);
			C_R1_.rotation_ = ES(CR1_, T);
			C_R2_.rotation_ = ES(CR2_, T);

			// T追加
			T += addAttackT;
			if (T > 1.0f) {

				// Normalにする
				T = 0;
				firstAction_ = false;
				MoveWave_ = MoveE::attackWait;
				// シーン変換
				state_ = StateCore::Normal;
				weakC = WeakCore::center_;
			}
		}
#pragma endregion	
		break;
	default:
		break;
	}
}


void BCore::Normal() { 
	switch (ATKType) {
	case ATKtype::Eye_Layzer:
		EyeLayzer();
		break;
	case ATKtype::Punch_missiles:
		Punch_M();
		break;
	case ATKtype::Beam:
		Beam();
		break;
	case ATKtype::Armswinging:
		ArmSwing();
		break;
	case ATKtype::ChaseBullet:
		ChaseBullet();
		break;
	case ATKtype::None:
		//ShotNormal();
		//ShotChase();


		//何もしない
		if (--nextATKCount <= 0) {
			//アクションを起こす
			int num = GetRandomNum(15, false);

			//ランダムで行動
			if (num == 0) {
			//	ATKType = ATKtype::Eye_Layzer;
				ATKType = ATKtype::Punch_missiles;
			}
			if (num == 1) {
			//	ATKType = ATKtype::Eye_Layzer;
				ATKType = ATKtype::Punch_missiles;
			}
			if (num == 2) {
			//	ATKType = ATKtype::Eye_Layzer;
				ATKType = ATKtype::Punch_missiles;
			}
			if (num == 3) {
				ATKType = ATKtype::Punch_missiles;
			}
			if (num == 4) {
				ATKType = ATKtype::Punch_missiles;
			}
			if (num == 5) {
				ATKType = ATKtype::Punch_missiles;
			}
			if (num == 6) {
				ATKType = ATKtype::Armswinging;
			}
			if (num == 7) {
				ATKType = ATKtype::Armswinging;
			}
			if (num == 8) {
				//ATKType = ATKtype::Beam;
				ATKType = ATKtype::Punch_missiles;
			}
			if (num == 9) {
				ATKType = ATKtype::Armswinging;
			}

			if (num == 10) {
				ATKType = ATKtype::ChaseBullet;
			}
			if (num == 11) {
				ATKType = ATKtype::ChaseBullet;
			}
			if (num == 12) {
				ATKType = ATKtype::ChaseBullet;
			}
			if (num == 13) {
				ATKType = ATKtype::ChaseBullet;
			}
			if (num == 14) {
				ATKType = ATKtype::ChaseBullet;
			}
			
			//ATKType = ATKtype::Punch_missiles;

			SeeTarget();
			//フラグ処理の初期化
			firstAction_ = false;
			T = 0;
			MoveWave_ = MoveE::attackWait;
		}
		break;
	default:
		break;
	}
}


void BCore::EyeLayzer() {
	switch (MoveWave_) {
	case MoveE::attackWait:
#pragma region atkWait
		if (!firstAction_) {
			firstAction_ = true;
			// 攻撃予備動作
			T = 0;

			H_ = {head_.rotation_, head_.rotation_};
			B_ = {body_.rotation_, body_.rotation_};
			Leg_ = {leg_.rotation_, leg_.rotation_};
			LA1_ = {LArm1_.rotation_, LArm1_.rotation_};
			LA2_ = {LArm2_.rotation_, LArm2_.rotation_};
			LH_ = {Lhand_.rotation_, Lhand_.rotation_};

			RA1_ = {RArm1_.rotation_, RArm1_.rotation_};
			RA2_ = {RArm2_.rotation_, RArm2_.rotation_};
			RH_ = {Rhand_.rotation_, Rhand_.rotation_};

			CC = {C_center_.rotation_, C_center_.rotation_};

			CL1_ = {
			    C_L1_.rotation_, {0, 1, 0.4f}
            };
			CL2_ = {
			    C_L2_.rotation_, {0, 2, 0}
            };

			CR1_ = {
			    C_R1_.rotation_, {0, -1, -0.24f}
            };
			CR2_ = {
			    C_R2_.rotation_, {0, -2, 0}
            };
		} else {
			// 設定したら更新
			head_.rotation_ = ES(H_, T);
			body_.rotation_ = ES(B_, T);
			leg_.rotation_ = ES(Leg_, T);
			LArm1_.rotation_ = ES(LA1_, T);
			LArm2_.rotation_ = ES(LA2_, T);
			Lhand_.rotation_ = ES(LH_, T);

			RArm1_.rotation_ = ES(RA1_, T);
			RArm2_.rotation_ = ES(RA2_, T);
			Rhand_.rotation_ = ES(RH_, T);

			C_center_.rotation_ = ES(CC, T);
			C_L1_.rotation_ = ES(CL1_, T);
			C_L2_.rotation_ = ES(CL2_, T);
			C_R1_.rotation_ = ES(CR1_, T);
			C_R2_.rotation_ = ES(CR2_, T);

			// T追加
			T += addAttackT;
			if (T > 1.0f) {
				T = 1.0f;

				// 次のアニメーションへ
				T = 0;
				firstAction_ = false;
				MoveWave_ = MoveE::attack;
			}
		}
#pragma endregion		
		SeeTarget();
		break;
	case MoveE::attack:
#pragma region atk
		if (!firstAction_) {
			firstAction_ = true;
			// 攻撃予備動作
			T = 0;

			H_ = {head_.rotation_, head_.rotation_};
			B_ = {body_.rotation_, body_.rotation_};
			Leg_ = {leg_.rotation_, leg_.rotation_};
			LA1_ = {LArm1_.rotation_, LArm1_.rotation_};
			LA2_ = {LArm2_.rotation_, LArm2_.rotation_};
			LH_ = {Lhand_.rotation_, Lhand_.rotation_};

			RA1_ = {RArm1_.rotation_, RArm1_.rotation_};
			RA2_ = {RArm2_.rotation_, RArm2_.rotation_};
			RH_ = {Rhand_.rotation_, Rhand_.rotation_};

			CC = {C_center_.rotation_, C_center_.rotation_};

			CL1_ = {
			    C_L1_.rotation_, {0, -0.5f, 0.2f}
            };
			CL2_ = {
			    C_L2_.rotation_, {0, 1.5f, 0}
            };

			CR1_ = {
			    C_R1_.rotation_, {0, 0.5f, -0.2f}
            };
			CR2_ = {
			    C_R2_.rotation_, {0, -1.5f, 0}
            };
		} else {
			// 設定したら更新
			head_.rotation_ = ES(H_, T);
			body_.rotation_ = ES(B_, T);
			leg_.rotation_ = ES(Leg_, T);
			LArm1_.rotation_ = ES(LA1_, T);
			LArm2_.rotation_ = ES(LA2_, T);
			Lhand_.rotation_ = ES(LH_, T);

			RArm1_.rotation_ = ES(RA1_, T);
			RArm2_.rotation_ = ES(RA2_, T);
			Rhand_.rotation_ = ES(RH_, T);

			C_center_.rotation_ = ES(CC, T);
			C_L1_.rotation_ = ES(CL1_, T);
			C_L2_.rotation_ = ES(CL2_, T);
			C_R1_.rotation_ = ES(CR1_, T);
			C_R2_.rotation_ = ES(CR2_, T);

			// T追加
			T += addAttackT;
			if (T > 1.0f) {
				T = 0;
				firstAction_ = false;
				// 次のアニメーションへ
				MoveWave_ = MoveE::back;
			}
		}
#pragma endregion	
		break;
	case MoveE::back:
#pragma region back
		if (!firstAction_) {
			firstAction_ = true;
			// 攻撃予備動作
			T = 0;
			H_ = {head_.rotation_, NorRotate_.H};
			B_ = {body_.rotation_, NorRotate_.B};
			Leg_ = {leg_.rotation_, NorRotate_.leg};
			LA1_ = {LArm1_.rotation_, NorRotate_.LA1};
			LA2_ = {LArm2_.rotation_, NorRotate_.LA2};
			LH_ = {Lhand_.rotation_, NorRotate_.Lha};

			RA1_ = {RArm1_.rotation_, NorRotate_.RA1};
			RA2_ = {RArm2_.rotation_, NorRotate_.RA2};
			RH_ = {Rhand_.rotation_, NorRotate_.Rha};

			CC = {C_center_.rotation_, NorRotate_.CC};
			CL1_ = {C_L1_.rotation_, NorRotate_.CL1};
			CL2_ = {C_L2_.rotation_, NorRotate_.CL2};

			CR1_ = {C_R1_.rotation_, NorRotate_.CR1};
			CR2_ = {C_R2_.rotation_, NorRotate_.CR2};
		} else {
			// 設定したら更新
			head_.rotation_ = ES(H_, T);
			body_.rotation_ = ES(B_, T);
			leg_.rotation_ = ES(Leg_, T);
			LArm1_.rotation_ = ES(LA1_, T);
			LArm2_.rotation_ = ES(LA2_, T);
			Lhand_.rotation_ = ES(LH_, T);

			RArm1_.rotation_ = ES(RA1_, T);
			RArm2_.rotation_ = ES(RA2_, T);
			Rhand_.rotation_ = ES(RH_, T);

			C_center_.rotation_ = ES(CC, T);
			C_L1_.rotation_ = ES(CL1_, T);
			C_L2_.rotation_ = ES(CL2_, T);
			C_R1_.rotation_ = ES(CR1_, T);
			C_R2_.rotation_ = ES(CR2_, T);

			// T追加
			T += addAttackT;
			if (T > 1.0f) {

				// Normalにする
				T = 0;
				firstAction_ = false;
				MoveWave_ = MoveE::attackWait;
				// シーン変換
				ATKType = ATKtype::None;
				nextATKCount = maxATKWait;
			}
		}
#pragma endregion	
		break;
	default:
		break;
	}
}

void BCore::Punch_M() {
	switch (MoveWave_) {
	case MoveE::attackWait:
#pragma region atkWait
		if (!firstAction_) {
			firstAction_ = true;
			// 攻撃予備動作
			T = 0;

			H_ = {head_.rotation_, head_.rotation_};
			B_ = {body_.rotation_, body_.rotation_};
			Leg_ = {leg_.rotation_, leg_.rotation_};
			LA1_ = {LArm1_.rotation_, LArm1_.rotation_};
			LA2_ = {LArm2_.rotation_, LArm2_.rotation_};
			LH_ = {Lhand_.rotation_, Lhand_.rotation_};

			RA1_ = {RArm1_.rotation_, RArm1_.rotation_};
			RA2_ = {RArm2_.rotation_, RArm2_.rotation_};
			RH_ = {Rhand_.rotation_, Rhand_.rotation_};

			CC = {C_center_.rotation_, C_center_.rotation_};

			CL1_ = {
			    C_L1_.rotation_, {0, 1.4f, 0}
            };
			CL2_ = {
			    C_L2_.rotation_, {0, 1.7f, 0}
            };

			CR1_ = {
			    C_R1_.rotation_, {0, -1.43f, 0}
            };
			CR2_ = {
			    C_R2_.rotation_, {0, -0.2f, 0}
            };

			Stock1_ = {
			    Rhand_.scale_, {2, 2, 2}
            };
		} else {
			// 設定したら更新
			head_.rotation_ = ES(H_, T);
			body_.rotation_ = ES(B_, T);
			leg_.rotation_ = ES(Leg_, T);
			LArm1_.rotation_ = ES(LA1_, T);
			LArm2_.rotation_ = ES(LA2_, T);
			Lhand_.rotation_ = ES(LH_, T);

			RArm1_.rotation_ = ES(RA1_, T);
			RArm2_.rotation_ = ES(RA2_, T);
			Rhand_.rotation_ = ES(RH_, T);

			C_center_.rotation_ = ES(CC, T);
			C_L1_.rotation_ = ES(CL1_, T);
			C_L2_.rotation_ = ES(CL2_, T);
			C_R1_.rotation_ = ES(CR1_, T);
			C_R2_.rotation_ = ES(CR2_, T);

			Rhand_.scale_ = ES(Stock1_, T);

			// T追加
			T += addAttackT/2;
			if (T > 1.0f) {
				T = 1.0f;

				// 次のアニメーションへ
				T = 0;
				firstAction_ = false;
				MoveWave_ = MoveE::attack;
			}
		}
#pragma endregion
		SeeTarget();
		break;
	case MoveE::attack:
#pragma region atk
		if (!firstAction_) {
			firstAction_ = true;
			// 攻撃予備動作
			T = 0;

			H_ = {head_.rotation_, head_.rotation_};
			B_ = {body_.rotation_, body_.rotation_};
			Leg_ = {leg_.rotation_, leg_.rotation_};
			LA1_ = {LArm1_.rotation_, LArm1_.rotation_};
			LA2_ = {LArm2_.rotation_, LArm2_.rotation_};
			LH_ = {Lhand_.rotation_, Lhand_.rotation_};

			RA1_ = {RArm1_.rotation_, RArm1_.rotation_};
			RA2_ = {RArm2_.rotation_, RArm2_.rotation_};
			RH_ = {Rhand_.rotation_, Rhand_.rotation_};

			CC = {C_center_.rotation_, C_center_.rotation_};

			CL1_ = {
			    C_L1_.rotation_, {0, 1, 0}
            };
			CL2_ = {
			    C_L2_.rotation_, {0, 2, 0}
            };

			CR1_ = {
			    C_R1_.rotation_, {0, -1, 0}
            };
			CR2_ = {
			    C_R2_.rotation_, {0, -0.55f, 0}
            };

			Rhand_.scale_ = {0.01f, 0.01f, 0.01f};
			ShotCount = 0;
		} else {
			// 設定したら更新
			head_.rotation_ = ES(H_, T);
			body_.rotation_ = ES(B_, T);
			leg_.rotation_ = ES(Leg_, T);
			LArm1_.rotation_ = ES(LA1_, T);
			LArm2_.rotation_ = ES(LA2_, T);
			Lhand_.rotation_ = ES(LH_, T);

			RArm1_.rotation_ = ES(RA1_, T);
			RArm2_.rotation_ = ES(RA2_, T);
			Rhand_.rotation_ = ES(RH_, T);

			C_center_.rotation_ = ES(CC, T);
			C_L1_.rotation_ = ES(CL1_, T);
			C_L2_.rotation_ = ES(CL2_, T);
			C_R1_.rotation_ = ES(CR1_, T);
			C_R2_.rotation_ = ES(CR2_, T);

			if (ShotCount == 0) {
				ShotNormal(2);
			}
			ShotCount++;
			// T追加
			T += addAttackT*2;
			if (T > 1.0f) {
				T = 0;
				firstAction_ = false;
				// 次のアニメーションへ
				MoveWave_ = MoveE::back;
			}
		}
#pragma endregion
		break;
	case MoveE::back:
#pragma region back
		if (!firstAction_) {
			firstAction_ = true;
			// 攻撃予備動作
			T = 0;
			H_ = {head_.rotation_, NorRotate_.H};
			B_ = {body_.rotation_, NorRotate_.B};
			Leg_ = {leg_.rotation_, NorRotate_.leg};
			LA1_ = {LArm1_.rotation_, NorRotate_.LA1};
			LA2_ = {LArm2_.rotation_, NorRotate_.LA2};
			LH_ = {Lhand_.rotation_, NorRotate_.Lha};

			RA1_ = {RArm1_.rotation_, NorRotate_.RA1};
			RA2_ = {RArm2_.rotation_, NorRotate_.RA2};
			RH_ = {Rhand_.rotation_, NorRotate_.Rha};

			CC = {C_center_.rotation_, NorRotate_.CC};
			CL1_ = {C_L1_.rotation_, NorRotate_.CL1};
			CL2_ = {C_L2_.rotation_, NorRotate_.CL2};

			CR1_ = {C_R1_.rotation_, NorRotate_.CR1};
			CR2_ = {C_R2_.rotation_, NorRotate_.CR2};

			Stock1_ = {Rhand_.scale_, {1, 1, 1}};
		} else {
			// 設定したら更新
			head_.rotation_ = ES(H_, T);
			body_.rotation_ = ES(B_, T);
			leg_.rotation_ = ES(Leg_, T);
			LArm1_.rotation_ = ES(LA1_, T);
			LArm2_.rotation_ = ES(LA2_, T);
			Lhand_.rotation_ = ES(LH_, T);

			RArm1_.rotation_ = ES(RA1_, T);
			RArm2_.rotation_ = ES(RA2_, T);
			Rhand_.rotation_ = ES(RH_, T);

			C_center_.rotation_ = ES(CC, T);
			C_L1_.rotation_ = ES(CL1_, T);
			C_L2_.rotation_ = ES(CL2_, T);
			C_R1_.rotation_ = ES(CR1_, T);
			C_R2_.rotation_ = ES(CR2_, T);

			Rhand_.scale_ = ES(Stock1_, T);
			// T追加
			T += addAttackT;
			if (T > 1.0f) {
				Rhand_.scale_ = {1, 1, 1};
				// Normalにする
				T = 0;
				firstAction_ = false;
				MoveWave_ = MoveE::attackWait;
				// シーン変換
				ATKType = ATKtype::None;
				nextATKCount = maxATKWait;
			}
		}
#pragma endregion
		break;
	default:
		break;
	}
}

void BCore::Beam() {
	switch (MoveWave_) {
	case MoveE::attackWait:
#pragma region atkWait
		if (!firstAction_) {
			firstAction_ = true;
			// 攻撃予備動作
			T = 0;

			WaitShotBeam = 0;

			H_ = {
			    head_.rotation_, {-0.7f, 0, 0}
            };
			B_ = {
			    body_.rotation_, {0.7f, 0, 0}
            };
			Leg_ = {leg_.rotation_, leg_.rotation_};
			LA1_ = {LArm1_.rotation_, LArm1_.rotation_};
			LA2_ = {LArm2_.rotation_, LArm2_.rotation_};
			LH_ = {Lhand_.rotation_, Lhand_.rotation_};

			RA1_ = {RArm1_.rotation_, RArm1_.rotation_};
			RA2_ = {RArm2_.rotation_, RArm2_.rotation_};
			RH_ = {Rhand_.rotation_, Rhand_.rotation_};

			CC = {
			    C_center_.rotation_, {0.5f, 0, 0}
            };
		CL1_ = {
		    C_L1_.rotation_, {-1.7f, 1, -1.3f}};
		CL2_ = {
		    C_L2_.rotation_, {0.4f, 0.5f, 0}};

		CR1_ = {
		    C_R1_.rotation_, {-1.7f, -1, 1.3f}};
		CR2_ = {
		    C_R2_.rotation_, {0.4f, -0.5f, 0}};
		
		} else {

		SeeTarget();

			// 設定したら更新
			head_.rotation_ = ES(H_, T);
			body_.rotation_ = ES(B_, T);
			leg_.rotation_ = ES(Leg_, T);
			LArm1_.rotation_ = ES(LA1_, T);
			LArm2_.rotation_ = ES(LA2_, T);
			Lhand_.rotation_ = ES(LH_, T);

			RArm1_.rotation_ = ES(RA1_, T);
			RArm2_.rotation_ = ES(RA2_, T);
			Rhand_.rotation_ = ES(RH_, T);

			C_center_.rotation_ = ES(CC, T);
			C_L1_.rotation_ = ES(CL1_, T);
			C_L2_.rotation_ = ES(CL2_, T);
			C_R1_.rotation_ = ES(CR1_, T);
			C_R2_.rotation_ = ES(CR2_, T);

			// T追加
			T += addAttackT / 4;
			if (T > 1.0f) {
				T = 1.0f;
				if (++WaitShotBeam >= 300) {
					// 次のアニメーションへ
					T = 0;
					firstAction_ = false;
					MoveWave_ = MoveE::attack;
				}
			}
		}
#pragma endregion
		
		break;
	case MoveE::attack:
#pragma region atk
		if (!firstAction_) {
			// 打っている間
			T += 1.0F / 300.0F;

			// 時間超えたら
			if (T > 1.0f) {
				firstAction_ = true;
				// 攻撃予備動作
				T = 0;
				weakC = WeakCore::center_;
#pragma region 打ち終わった後のモーション処理
				H_ = {
				    head_.rotation_, {0.83f, 0, 0}};
				B_ = {
				    body_.rotation_, {0.5f, 0, 0}};
				Leg_ = {leg_.rotation_, leg_.rotation_};
				LA1_ = {LArm1_.rotation_, LArm1_.rotation_};
				LA2_ = {LArm2_.rotation_, LArm2_.rotation_};
				LH_ = {Lhand_.rotation_, Lhand_.rotation_};

				RA1_ = {RArm1_.rotation_, RArm1_.rotation_};
				RA2_ = {RArm2_.rotation_, RArm2_.rotation_};
				RH_ = {Rhand_.rotation_, Rhand_.rotation_};

				CC = {
				    C_center_.rotation_, {-0.8f, 0, 0}};
				CL1_ = {
				    C_L1_.rotation_, {0, 0.7f, 1.5f}};
				CL2_ = {
				    C_L2_.rotation_, {0, 0, 0.1f}};

				CR1_ = {
				    C_R1_.rotation_, {0, -0.7f, -1.5f}};
				CR2_ = {
				    C_R2_.rotation_, {0, 0, -0.1f}};

#pragma endregion
			}
		} else {
			// 設定したら更新
			head_.rotation_ = ES(H_, T);
			body_.rotation_ = ES(B_, T);
			leg_.rotation_ = ES(Leg_, T);
			LArm1_.rotation_ = ES(LA1_, T);
			LArm2_.rotation_ = ES(LA2_, T);
			Lhand_.rotation_ = ES(LH_, T);

			RArm1_.rotation_ = ES(RA1_, T);
			RArm2_.rotation_ = ES(RA2_, T);
			Rhand_.rotation_ = ES(RH_, T);

			C_center_.rotation_ = ES(CC, T);
			C_L1_.rotation_ = ES(CL1_, T);
			C_L2_.rotation_ = ES(CL2_, T);
			C_R1_.rotation_ = ES(CR1_, T);
			C_R2_.rotation_ = ES(CR2_, T);

			// T追加
			T += addAttackT;
			if (T > 1.0f) {
				T = 0;
				firstAction_ = false;
				// 次のアニメーションへ
				MoveWave_ = MoveE::back;
			}
		}
#pragma endregion
		break;
	case MoveE::back:
#pragma region back
		if (!firstAction_) {
			// 気絶時間まで待つ
			T += 1.0f / 300.0f;

			// 時間が終わったら
			if (T > 1.0f) {
				weakC = WeakCore::center_;
				firstAction_ = true;
				// 攻撃予備動作
				T = 0;
				H_ = {head_.rotation_, NorRotate_.H};
				B_ = {body_.rotation_, NorRotate_.B};
				Leg_ = {leg_.rotation_, NorRotate_.leg};
				LA1_ = {LArm1_.rotation_, NorRotate_.LA1};
				LA2_ = {LArm2_.rotation_, NorRotate_.LA2};
				LH_ = {Lhand_.rotation_, NorRotate_.Lha};

				RA1_ = {RArm1_.rotation_, NorRotate_.RA1};
				RA2_ = {RArm2_.rotation_, NorRotate_.RA2};
				RH_ = {Rhand_.rotation_, NorRotate_.Rha};

				CC = {C_center_.rotation_, NorRotate_.CC};
				CL1_ = {C_L1_.rotation_, NorRotate_.CL1};
				CL2_ = {C_L2_.rotation_, NorRotate_.CL2};

				CR1_ = {C_R1_.rotation_, NorRotate_.CR1};
				CR2_ = {C_R2_.rotation_, NorRotate_.CR2};
			}
		} else {
			// 設定したら更新
			head_.rotation_ = ES(H_, T);
			body_.rotation_ = ES(B_, T);
			leg_.rotation_ = ES(Leg_, T);
			LArm1_.rotation_ = ES(LA1_, T);
			LArm2_.rotation_ = ES(LA2_, T);
			Lhand_.rotation_ = ES(LH_, T);

			RArm1_.rotation_ = ES(RA1_, T);
			RArm2_.rotation_ = ES(RA2_, T);
			Rhand_.rotation_ = ES(RH_, T);

			C_center_.rotation_ = ES(CC, T);
			C_L1_.rotation_ = ES(CL1_, T);
			C_L2_.rotation_ = ES(CL2_, T);
			C_R1_.rotation_ = ES(CR1_, T);
			C_R2_.rotation_ = ES(CR2_, T);

			// T追加
			T += addAttackT;
			if (T > 1.0f) {
				Rhand_.scale_ = {1, 1, 1};
				// Normalにする
				T = 0;
				firstAction_ = false;
				MoveWave_ = MoveE::attackWait;
				// シーン変換
				ATKType = ATKtype::None;
				nextATKCount = maxATKWait;
			}
		}
#pragma endregion
		break;
	default:
		break;
	}

}

void BCore::ArmSwing() {
	switch (MoveWave_) {
	case MoveE::attackWait:
#pragma region atkWait
		if (!firstAction_) {
			firstAction_ = true;
			// 攻撃予備動作
			T = 0;

			H_ = {head_.rotation_, head_.rotation_};
			B_ = {body_.rotation_, body_.rotation_};
			Leg_ = {leg_.rotation_, leg_.rotation_};
			LA1_ = {LArm1_.rotation_, LArm1_.rotation_};
			LA2_ = {LArm2_.rotation_, LArm2_.rotation_};
			LH_ = {Lhand_.rotation_, Lhand_.rotation_};

			RA1_ = {RArm1_.rotation_, RArm1_.rotation_};
			RA2_ = {RArm2_.rotation_, RArm2_.rotation_};
			RH_ = {Rhand_.rotation_, Rhand_.rotation_};

			CC = {C_center_.rotation_, C_center_.rotation_};

			CL1_ = {
			    C_L1_.rotation_, {0, 0, 0}
            };
			CL2_ = {
			    C_L2_.rotation_, {0, 0, 0}
            };

			CR1_ = {
			    C_R1_.rotation_, {0, 0, 0}
            };
			CR2_ = {
			    C_R2_.rotation_, {0, 0, 0}
            };
		} else {
			// 設定したら更新
			head_.rotation_ = ES(H_, T);
			body_.rotation_ = ES(B_, T);
			leg_.rotation_ = ES(Leg_, T);
			LArm1_.rotation_ = ES(LA1_, T);
			LArm2_.rotation_ = ES(LA2_, T);
			Lhand_.rotation_ = ES(LH_, T);

			RArm1_.rotation_ = ES(RA1_, T);
			RArm2_.rotation_ = ES(RA2_, T);
			Rhand_.rotation_ = ES(RH_, T);

			C_center_.rotation_ = ES(CC, T);
			C_L1_.rotation_ = ES(CL1_, T);
			C_L2_.rotation_ = ES(CL2_, T);
			C_R1_.rotation_ = ES(CR1_, T);
			C_R2_.rotation_ = ES(CR2_, T);

			// T追加
			T += addAttackT;
			if (T > 1.0f) {
				T = 1.0f;

				// 次のアニメーションへ
				T = 0;
				firstAction_ = false;
				MoveWave_ = MoveE::attack;
			}
		}
#pragma endregion
		SeeTarget();
		break;
	case MoveE::attack:
#pragma region atk
		if (!firstAction_) {

			firstAction_ = true;
			// 攻撃予備動作
			T = 0;

			//何回転するか
			float R_Y = ((float)std::numbers::pi * 2) * 2.0f;

			H_ = {head_.rotation_, head_.rotation_};
			B_ = {body_.rotation_, {0,R_Y,0}};
			Leg_ = {leg_.rotation_, leg_.rotation_};
			LA1_ = {LArm1_.rotation_, LArm1_.rotation_};
			LA2_ = {LArm2_.rotation_, LArm2_.rotation_};
			LH_ = {Lhand_.rotation_, Lhand_.rotation_};

			RA1_ = {RArm1_.rotation_, RArm1_.rotation_};
			RA2_ = {RArm2_.rotation_, RArm2_.rotation_};
			RH_ = {Rhand_.rotation_, Rhand_.rotation_};

			CC = {C_center_.rotation_, C_center_.rotation_};

			CL1_ = {
			    C_L1_.rotation_, {0, 0, 0}
            };
			CL2_ = {
			    C_L2_.rotation_, {0, 0, 0}
            };

			CR1_ = {
			    C_R1_.rotation_, {0, 0, 0}
            };
			CR2_ = {
			    C_R2_.rotation_, {0, 0, 0}
            };

			ShotCount = 0;
		} else {
			// 設定したら更新
			head_.rotation_ = ES(H_, T);
			body_.rotation_ = ES(B_, T);
			leg_.rotation_ = ES(Leg_, T);
			LArm1_.rotation_ = ES(LA1_, T);
			LArm2_.rotation_ = ES(LA2_, T);
			Lhand_.rotation_ = ES(LH_, T);

			RArm1_.rotation_ = ES(RA1_, T);
			RArm2_.rotation_ = ES(RA2_, T);
			Rhand_.rotation_ = ES(RH_, T);

			C_center_.rotation_ = ES(CC, T);
			C_L1_.rotation_ = ES(CL1_, T);
			C_L2_.rotation_ = ES(CL2_, T);
			C_R1_.rotation_ = ES(CR1_, T);
			C_R2_.rotation_ = ES(CR2_, T);

			if (Getangly()) {
				if (T != 0) {

					if (ShotCount % 21 == 0) {
						ShotNormal(1);
					}

					if (ShotCount % 80 == 0) {
						ShotChase(1);
					}
				}

			} else {
				if (T != 0) {

					if (ShotCount % 50==0) {
						ShotNormal(1);
					}

					if (ShotCount % 120==0) {
						ShotChase(1);
					}
				}
			}

			ShotCount++;

			// T追加
			T +=1.0f/300.0f;
			if (T > 1.0f) {
				T = 0;
				firstAction_ = false;
				// 次のアニメーションへ
				MoveWave_ = MoveE::back;
			}
		}
#pragma endregion
		break;
	case MoveE::back:
#pragma region back
		if (!firstAction_) {
			firstAction_ = true;
			// 攻撃予備動作
			T = 0;
			H_ = {head_.rotation_, NorRotate_.H};
			B_ = {{0,0,0}, NorRotate_.B};
			Leg_ = {leg_.rotation_, NorRotate_.leg};
			LA1_ = {LArm1_.rotation_, NorRotate_.LA1};
			LA2_ = {LArm2_.rotation_, NorRotate_.LA2};
			LH_ = {Lhand_.rotation_, NorRotate_.Lha};

			RA1_ = {RArm1_.rotation_, NorRotate_.RA1};
			RA2_ = {RArm2_.rotation_, NorRotate_.RA2};
			RH_ = {Rhand_.rotation_, NorRotate_.Rha};

			CC = {C_center_.rotation_, NorRotate_.CC};
			CL1_ = {C_L1_.rotation_, NorRotate_.CL1};
			CL2_ = {C_L2_.rotation_, NorRotate_.CL2};

			CR1_ = {C_R1_.rotation_, NorRotate_.CR1};
			CR2_ = {C_R2_.rotation_, NorRotate_.CR2};
		} else {
			// 設定したら更新
			head_.rotation_ = ES(H_, T);
			body_.rotation_ = ES(B_, T);
			leg_.rotation_ = ES(Leg_, T);
			LArm1_.rotation_ = ES(LA1_, T);
			LArm2_.rotation_ = ES(LA2_, T);
			Lhand_.rotation_ = ES(LH_, T);

			RArm1_.rotation_ = ES(RA1_, T);
			RArm2_.rotation_ = ES(RA2_, T);
			Rhand_.rotation_ = ES(RH_, T);

			C_center_.rotation_ = ES(CC, T);
			C_L1_.rotation_ = ES(CL1_, T);
			C_L2_.rotation_ = ES(CL2_, T);
			C_R1_.rotation_ = ES(CR1_, T);
			C_R2_.rotation_ = ES(CR2_, T);

			// T追加
			T += addAttackT;
			if (T > 1.0f) {

				// Normalにする
				T = 0;
				firstAction_ = false;
				MoveWave_ = MoveE::attackWait;
				// シーン変換
				ATKType = ATKtype::None;
				nextATKCount = maxATKWait;
			}
		}
#pragma endregion
		break;
	default:
		break;
	}
}

void BCore::ChaseBullet() {
	switch (MoveWave_) {
	case MoveE::attackWait:
#pragma region atkWait
		if (!firstAction_) {
			firstAction_ = true;
			// 攻撃予備動作
			T = 0;

			H_ = {head_.rotation_, head_.rotation_};
			B_ = {body_.rotation_, body_.rotation_};
			Leg_ = {leg_.rotation_, leg_.rotation_};
			LA1_ = {LArm1_.rotation_, LArm1_.rotation_};
			LA2_ = {LArm2_.rotation_, LArm2_.rotation_};
			LH_ = {Lhand_.rotation_, Lhand_.rotation_};

			RA1_ = {RArm1_.rotation_, RArm1_.rotation_};
			RA2_ = {RArm2_.rotation_, RArm2_.rotation_};
			RH_ = {Rhand_.rotation_, Rhand_.rotation_};

			CC = {C_center_.rotation_, C_center_.rotation_};

			CL1_ = {
			    C_L1_.rotation_, {0, -0.5f, 0.7f}
            };
			CL2_ = {
			    C_L2_.rotation_, {-3, 0.4f, -2.4f}
            };

			CR1_ = {
			    C_R1_.rotation_, {0, 0.7f, 0}
            };
			CR2_ = {
			    C_R2_.rotation_, {0, -2.4f, 0}
            };
		} else {
			// 設定したら更新
			head_.rotation_ = ES(H_, T);
			body_.rotation_ = ES(B_, T);
			leg_.rotation_ = ES(Leg_, T);
			LArm1_.rotation_ = ES(LA1_, T);
			LArm2_.rotation_ = ES(LA2_, T);
			Lhand_.rotation_ = ES(LH_, T);

			RArm1_.rotation_ = ES(RA1_, T);
			RArm2_.rotation_ = ES(RA2_, T);
			Rhand_.rotation_ = ES(RH_, T);

			C_center_.rotation_ = ES(CC, T);
			C_L1_.rotation_ = ES(CL1_, T);
			C_L2_.rotation_ = ES(CL2_, T);
			C_R1_.rotation_ = ES(CR1_, T);
			C_R2_.rotation_ = ES(CR2_, T);

			// T追加
			T += addAttackT;
			if (T > 1.0f) {
				T = 1.0f;

				// 次のアニメーションへ
				T = 0;
				firstAction_ = false;
				MoveWave_ = MoveE::attack;
			}
		}
#pragma endregion
		SeeTarget();
		break;
	case MoveE::attack:
#pragma region atk
		if (!firstAction_) {
			firstAction_ = true;
			// 攻撃予備動作
			T = 0;

			H_ = {head_.rotation_, head_.rotation_};
			B_ = {body_.rotation_, body_.rotation_};
			Leg_ = {leg_.rotation_, leg_.rotation_};
			LA1_ = {LArm1_.rotation_, LArm1_.rotation_};
			LA2_ = {LArm2_.rotation_, LArm2_.rotation_};
			LH_ = {Lhand_.rotation_, Lhand_.rotation_};

			RA1_ = {RArm1_.rotation_, RArm1_.rotation_};
			RA2_ = {RArm2_.rotation_, RArm2_.rotation_};
			RH_ = {Rhand_.rotation_, Rhand_.rotation_};

			CC = {C_center_.rotation_, C_center_.rotation_};

			CL1_ = {
			    C_L1_.rotation_, C_L1_.rotation_
            };
			CL2_ = {
			    C_L2_.rotation_, C_L2_.rotation_
            };

			CR1_ = {
			    C_R1_.rotation_, C_R1_.rotation_
            };
			CR2_ = {
			    C_R2_.rotation_, C_R2_.rotation_
            };

			Lhand_.scale_ = {0, 0, 0};
			ShotCount = 0;
		} else {
			SeeTarget();
			// 設定したら更新
			head_.rotation_ = ES(H_, T);
			body_.rotation_ = ES(B_, T);
			leg_.rotation_ = ES(Leg_, T);
			LArm1_.rotation_ = ES(LA1_, T);
			LArm2_.rotation_ = ES(LA2_, T);
			Lhand_.rotation_ = ES(LH_, T);

			RArm1_.rotation_ = ES(RA1_, T);
			RArm2_.rotation_ = ES(RA2_, T);
			Rhand_.rotation_ = ES(RH_, T);

			C_center_.rotation_ = ES(CC, T);
			C_L1_.rotation_ = ES(CL1_, T);
			C_L2_.rotation_ = ES(CL2_, T);
			C_R1_.rotation_ = ES(CR1_, T);
			C_R2_.rotation_ = ES(CR2_, T);


			if (Getangly()) {
				if (ShotCount % 10 == 0) {
					ShotChase(1);
				}
			} else {
				if (ShotCount % 30 == 0) {
					ShotChase(1);
				}
			}
			ShotCount++;

			// T追加
			T += 1.0f / 60.0f;
			
			if (T > 1.0f) {
				T = 0;
				firstAction_ = false;
				// 次のアニメーションへ
				MoveWave_ = MoveE::back;
			}
		}
#pragma endregion
		break;
	case MoveE::back:
#pragma region back
		if (!firstAction_) {
			firstAction_ = true;
			// 攻撃予備動作
			T = 0;
			H_ = {head_.rotation_, NorRotate_.H};
			B_ = {body_.rotation_, NorRotate_.B};
			Leg_ = {leg_.rotation_, NorRotate_.leg};
			LA1_ = {LArm1_.rotation_, NorRotate_.LA1};
			LA2_ = {LArm2_.rotation_, NorRotate_.LA2};
			LH_ = {Lhand_.rotation_, NorRotate_.Lha};

			RA1_ = {RArm1_.rotation_, NorRotate_.RA1};
			RA2_ = {RArm2_.rotation_, NorRotate_.RA2};
			RH_ = {Rhand_.rotation_, NorRotate_.Rha};

			CC = {C_center_.rotation_, NorRotate_.CC};
			CL1_ = {C_L1_.rotation_, NorRotate_.CL1};
			CL2_ = {C_L2_.rotation_, NorRotate_.CL2};

			CR1_ = {C_R1_.rotation_, NorRotate_.CR1};
			CR2_ = {C_R2_.rotation_, NorRotate_.CR2};


			Stock1_ = {
			    Lhand_.scale_, {1, 1, 1}
            };
		} else {
			// 設定したら更新
			head_.rotation_ = ES(H_, T);
			body_.rotation_ = ES(B_, T);
			leg_.rotation_ = ES(Leg_, T);
			LArm1_.rotation_ = ES(LA1_, T);
			LArm2_.rotation_ = ES(LA2_, T);
			Lhand_.rotation_ = ES(LH_, T);

			RArm1_.rotation_ = ES(RA1_, T);
			RArm2_.rotation_ = ES(RA2_, T);
			Rhand_.rotation_ = ES(RH_, T);

			C_center_.rotation_ = ES(CC, T);
			C_L1_.rotation_ = ES(CL1_, T);
			C_L2_.rotation_ = ES(CL2_, T);
			C_R1_.rotation_ = ES(CR1_, T);
			C_R2_.rotation_ = ES(CR2_, T);

			Lhand_.scale_ = ES(Stock1_, T);

			// T追加
			T += addAttackT;
			if (T > 1.0f) {

				Lhand_.scale_ = {1, 1, 1};
				// Normalにする
				T = 0;
				firstAction_ = false;
				MoveWave_ = MoveE::attackWait;
				// シーン変換
				ATKType = ATKtype::None;
				nextATKCount = maxATKWait/2;
			}
		}
#pragma endregion
		break;
	default:
		break;
	}
}
    //死ぬときのアニメーション
void BCore::BreakAnime() {

}

void BCore::Move() {
	switch (state_) {
	case StateCore::Start:
		Start();
		break;
	case StateCore::StartAnimation:
		StartAnime();
		break;
	case StateCore::Normal:
		Normal();
		break;
	case StateCore::breakAnimetion:
		BreakAnime();
		break;
	default:
		break;
	}

}

void BCore::Update() { 
	if (Getangly()) {
		maxATKWait = 60;

	} else {
		maxATKWait = 120;
	}
	Move();
#ifdef _DEBUG
	ImGui::Begin("Core");
	ImGui::DragFloat3("pos", &worldtransform_.translation_.x, 0.01f);
	ImGui::DragFloat3("rotate", &worldtransform_.rotation_.x, 0.01f);
	ImGui::DragFloat3("Scale", &worldtransform_.scale_.x, 0.01f);
	ImGui::Text("HP/%d :isdead/%d ", hp_, isDead_);

	ImGui::DragFloat3("p body", &body_.rotation_.x, 0.01f);
	ImGui::DragFloat3("p head", &head_.rotation_.x, 0.01f);

	ImGui::DragFloat3("p core", &C_center_.rotation_.x, 0.01f);

	ImGui::DragFloat3("p leg", &leg_.rotation_.x, 0.01f);

	/*
	ImGui::DragFloat3("p LA1", &LArm1_.rotation_.x, 0.01f);
	ImGui::DragFloat3("p LA2", &LArm2_.rotation_.x, 0.01f);
	ImGui::DragFloat3("p Lhand", &Lhand_.rotation_.x, 0.01f);

	ImGui::DragFloat3("p RA1", &RArm1_.rotation_.x, 0.01f);
	ImGui::DragFloat3("p RA2", &RArm2_.rotation_.x, 0.01f);
	ImGui::DragFloat3("p Rhand", &Rhand_.rotation_.x, 0.01f);
	*/

	ImGui::DragFloat3("p CL1", &C_L1_.rotation_.x, 0.01f);
	ImGui::DragFloat3("p CL2", &C_L2_.rotation_.x, 0.01f);

	ImGui::DragFloat3("p CR1", &C_R1_.rotation_.x, 0.01f);
	ImGui::DragFloat3("p CR2", &C_R2_.rotation_.x, 0.01f);


	ImGui::DragFloat3("LH scale", &Lhand_.scale_.x, 0.01f);
	ImGui::DragFloat3("RH scale", &Rhand_.scale_.x, 0.01f);

	ImGui::End();
#endif // _DEBUG
	UpdateAllMatrix();
}

void BCore::UpdateAllMatrix() { 
	worldtransform_.UpdateMatrix();

	body_.UpdateMatrix();
	head_.UpdateMatrix();
	C_center_.UpdateMatrix();
	leg_.UpdateMatrix();

	
	C_L1_.UpdateMatrix();
	LArm1_.UpdateMatrix();
	C_L2_.UpdateMatrix();
	LArm2_.UpdateMatrix();
	Lhand_.UpdateMatrix();
	
	C_R1_.UpdateMatrix();
	RArm1_.UpdateMatrix();
	C_R2_.UpdateMatrix();
	RArm2_.UpdateMatrix();
	Rhand_.UpdateMatrix();
	
}

void BCore::InCollision() { 
	hp_--;
	if (hp_ <= 0) {
		isDead_ = true;
	}
}

void BCore::ChangeCorePos() { cgangecount++;
	if (cgangecount >= 60 * 5) {
		cgangecount = 0;
		int num = GetRandomNum(5, false);
		if (num == 0) {
			weakC = WeakCore::center_;
		}
		if (num == 1) {
			weakC = WeakCore::L1;
		}
		if (num == 2) {
			weakC = WeakCore::L2;
		}
		if (num == 3) {
			weakC = WeakCore::R1;
		}
		if (num == 4) {
			weakC = WeakCore::R2;
		}
	}
}

void BCore::Draw(const ViewProjection& view) { 
	//models_[0]->Draw(world_, view);
	models_[0]->Draw(head_, view);
	models_[1]->Draw(body_, view);
	models_[2]->Draw(leg_, view);

	models_[3]->Draw(LArm1_, view);
	models_[4]->Draw(LArm2_, view);
	models_[5]->Draw(Lhand_, view);

	models_[6]->Draw(RArm1_, view);
	models_[7]->Draw(RArm2_, view);
	models_[8]->Draw(Rhand_, view);

	
	//コア
	if (weakC == WeakCore::center_) {
		models_[10]->Draw(C_center_, view);
	} else {
		models_[9]->Draw(C_center_, view);
	}
	
	if (weakC == WeakCore::L1) {
		models_[10]->Draw(C_L1_, view);
	} else {
		models_[9]->Draw(C_L1_, view);
	}
	
	if (weakC == WeakCore::L2) {
		models_[10]->Draw(C_L2_, view);
	} else {
		models_[9]->Draw(C_L2_, view);
	}
	if (weakC == WeakCore::R1) {
		models_[10]->Draw(C_R1_, view);
	} else {
		models_[9]->Draw(C_R1_, view);
	}
	if (weakC == WeakCore::R2) {
		models_[10]->Draw(C_R2_, view);
	} else {
		models_[9]->Draw(C_R2_, view);
	}
	
}