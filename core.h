#pragma once
#include<Model.h>
#include<WorldTransform.h>
#include"BaseCharacter.h"

//敵の行動状態
enum class StateCore {
	Start,
	StartAnimation,

	Normal,

	breakAnimetion,

};

//弱点コアの場所
enum class WeakCore { 
	center_,
	L1,
	L2,
	R1,
	R2,
	None,
};


enum class MoveE {
	attackWait,
	attack,
	back,
};

struct esing {
	Vector3 st;
	Vector3 ed;

};


enum class ATKtype {
	Eye_Layzer,
	Punch_missiles,
	Beam,
	Armswinging,
	ChaseBullet,
	None,
};

struct N_R {
	Vector3 H;
	Vector3 B;
	Vector3 leg;
	Vector3 LA1;
	Vector3 LA2;
	Vector3 Lha;
	Vector3 RA1;
	Vector3 RA2;
	Vector3 Rha;

	Vector3 CC;
	Vector3 CL1;
	Vector3 CL2;
	Vector3 CR1;
	Vector3 CR2;
};

class GameScene;

class BCore :public BaseCharacter{
public:
	

	void Initialize(const std::vector<Model*>& models);

	
	void Update()override;

	void Draw(const ViewProjection& view)override;

	void InCollision();

	Vector3 GetmatW() {
		return {
		    worldtransform_.matWorld_.m[3][0], worldtransform_.matWorld_.m[3][1],
		    worldtransform_.matWorld_.m[3][2]};
	}

	Vector3 GetmatCore() {
		Vector3 pos={0,-1000,0};
		switch (weakC) {
		case WeakCore::center_:
			pos = {
			    C_center_.matWorld_.m[3][0], 
				C_center_.matWorld_.m[3][1],
			    C_center_.matWorld_.m[3][2]};
			break;
		case WeakCore::L1:
			pos = {
			    C_L1_.matWorld_.m[3][0], 
				C_L1_.matWorld_.m[3][1],
			    C_L1_.matWorld_.m[3][2]};
			break;
		case WeakCore::L2:
			pos = {
			    C_L2_.matWorld_.m[3][0],
				C_L2_.matWorld_.m[3][1],
			    C_L2_.matWorld_.m[3][2]};
			break;
		case WeakCore::R1:
			pos = {
			    C_R1_.matWorld_.m[3][0], 
				C_R1_.matWorld_.m[3][1],
			    C_R1_.matWorld_.m[3][2]};
			break;
		case WeakCore::R2:
			pos = {
			    C_R2_.matWorld_.m[3][0], 
				C_R2_.matWorld_.m[3][1],
			    C_R2_.matWorld_.m[3][2]};
			break;
		case WeakCore::None:
			break;
		
		default:
			break;
		}
	
	
		return pos;
	}
	
	void UpdateAllMatrix();

	void SetPlayer(const WorldTransform* world) { 
		player_ = world;
		
	}

	void SeeTarget();

	const StateCore GetState() { return state_; }

	void SetStart();

private:

	

	void Start();

	void StartAnime();

	void Normal();

	void BreakAnime();

	void Move();

	void EyeLayzer();

	void Punch_M();

	void Beam();

	void ArmSwing();

	void ChaseBullet();
#pragma region Animation

	//攻撃予備動作
	float T = 0;
	
	//頭イージング
	esing H_;
	//身体イージング
	esing B_;
	//
	esing Leg_;

	esing LA1_;
	esing LA2_;
	esing RA1_;
	esing RA2_;

	esing LH_;
	esing RH_;

	esing CC;
	esing CL1_;
	esing CL2_;
	esing CR1_;
	esing CR2_;

	esing Stock1_;
	esing Stock2_;
	esing Stock3_;
#pragma endregion

	uint32_t WaitShotBeam = 0;

	//初期化
	bool firstAction_ = false;

	//攻撃のモード
	MoveE MoveWave_ = MoveE::attackWait;

	//攻撃状態
	ATKtype ATKType = ATKtype::None;

	uint32_t nextATKCount = 60;

	const uint32_t maxATKWait = 180;

	const N_R NorRotate_ = {
	    .H{0, 0,     0    },
	    .B{0, 0,     0    },
	    .leg{0, 0,     0    },

	    .LA1{0, 0,     0    },
	    .LA2{0, 0,     0    },
	    .Lha{0, 0,     0    },
	    .RA1{0, 0,     0    },
	    .RA2{0, 0,     0    },
	    .Rha{0, 0,     0    },

	    .CC{0, 0,     0    },
	    .CL1{0, 0,     0.5f },
	    .CL2{0, 2.5f,  0.5  },
	    .CR1{0, 0.5f,  -0.6f},
	    .CR2{0, -2.4f, 0    },
	};

	const float addWaitT = 1.0f / 60.0f;

	const float addAttackT = 1.0f / 30.0f;


	const uint32_t maxHP = 1000;

	//コアの基本状態
	StateCore state_ = StateCore::Start;

	// 弱点コア位置
	WeakCore weakC = WeakCore::L1;

	const uint32_t maxCoreSwitchTime = 60 * 20;

	uint32_t coreSwitchTime_ = 0;


	const WorldTransform* player_;

	Vector3 GetTargetW() { 
		return {player_->matWorld_.m[3][0], player_->matWorld_.m[3][1], player_->matWorld_.m[3][2]
		};
	}

	
	
	WorldTransform head_;
	WorldTransform body_;
	WorldTransform leg_;
	WorldTransform LArm1_;
	WorldTransform LArm2_;
	WorldTransform RArm1_;
	WorldTransform RArm2_;

	WorldTransform Lhand_;
	WorldTransform Rhand_;

	//コア
	WorldTransform C_center_;
	WorldTransform C_L1_;
	WorldTransform C_L2_;
	WorldTransform C_R1_;
	WorldTransform C_R2_;

};