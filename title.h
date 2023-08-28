#pragma once
#include<Model.h>

enum class Scene {
	start,
	startAnimation,
	normal,
	endAnimation,
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

	Scene scene_ = Scene::start;
};