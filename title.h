#pragma once
#include<Model.h>

enum class Scene {
	start,
	startAnimation,
	normal,
	endAnimation,
};

enum class NormalAnime {
	Normal,
	HitBoundIni,
	HitBound,
};

class TitleS {
public:
	void Initialize(Model*model,Model*tile);

	void Update();

	void Draw(ViewProjection view);

	void SetBeginTitle() { scene_ = Scene::start; }

	void OnCollision();

	const WorldTransform& GetRW() { return world_; }

	Scene GetScene() { return scene_; }

	bool IsPlay() { return isPlay_; }

	private:
#pragma region まとめ
	    void SetStart();
	    void StartAnimation();
	    void Normal();
	    void EndAnimation();
#pragma endregion




	Model* model_;
	Model* tileM_;

	WorldTransform world_;
	WorldTransform tile_;

	bool isPlay_ = false;

	const Vector3 zero = {0, 0, 0};

	Vector3 velo_ = {0, 0, 0};
	//Vector3 acce_ = {0, 0, 0};

	const float addAcce_ = 1.0f / 120.0f;

	Scene scene_ = Scene::start;

	NormalAnime anime_ = NormalAnime::Normal;

	const Vector3 startpos = {0, 20, -300};

	const float NormalY = 3.0f;

	uint32_t hitCount_ = 0;

	const uint32_t maxHitNum = 10;

	const float endFrame = 180;

	float floatT=0;
	const float floatheight_ = 1;
};