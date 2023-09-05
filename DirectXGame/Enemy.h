#pragma once
#include"BaseCharacter.h"
#include<Model.h>
#include<WorldTransform.h>


enum class Phase {
	WAIT,//待ち
	SHOT,//射撃
};


class GameScene;

class Enemy : public BaseCharacter {
public:
	void Initialize(const std::vector<Model*>& models, const uint32_t HP,Vector3 pos);
	
	void Update() override;

	void Draw(const ViewProjection& view) override;
	
	void OnCollision();


	private:
	GameScene* gameScene_=nullptr;

};