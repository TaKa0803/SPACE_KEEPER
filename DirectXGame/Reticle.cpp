#include"Reticle.h"
#include<ImGuiManager.h>
#include"math_matrix.h"
#include<GameScene.h>

void Reticle::Initialize(Model* model, const WorldTransform& parent) {
	input_ = Input::GetInstance();
	
	tex_ = TextureManager::Load("reticle.png");

	R2D_ = Sprite::Create(tex_, {0, 0}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f,0.5f});

	reticleWorld_.Initialize();
	reticleWorld_.translation_ = NormalPos;
	//reticleWorld_.scale_ = {1, 1, 10};
	reticleWorld_.parent_ = &parent;

	model_ = model;


	

}

void Reticle::DrawSPrite() { R2D_->Draw(); }

void Reticle::Move(float length) {
	float maxfar = 220;
	//+ni
	length = sqrtf(length * length);
	//gap
	float gap = length /maxfar;


	Vector2 controll = {0, 0};

	XINPUT_STATE joyState;
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		controll = {
			(float)joyState.Gamepad.sThumbRX/SHRT_MAX, 
			(float)joyState.Gamepad.sThumbRY / SHRT_MAX
		};
		reticleWorld_.translation_.x += controll.x * moveNum;
		reticleWorld_.translation_.y += controll.y * moveNum;

	}
	
	//max値
	Vector3 Max = {
		90.0f*gap,
		70.0f*gap
	};

	reticleWorld_.translation_.x = min(reticleWorld_.translation_.x, Max.x);
	reticleWorld_.translation_.x = max(reticleWorld_.translation_.x, -Max.x);
	reticleWorld_.translation_.y = min(reticleWorld_.translation_.y, Max.y);
	reticleWorld_.translation_.y = max(reticleWorld_.translation_.y, -Max.y);
	
}


void Reticle::Update(float depth) { 
	#ifdef _DEBUG
	ImGui::Begin("Reticle");
	ImGui::Text("pos :%4.1f/%4.1f/%4.1f", reticleWorld_.translation_.x, reticleWorld_.translation_.y,reticleWorld_.translation_.z);
	ImGui::Text("rotate :%4.1f/%4.1f/%4.1f", reticleWorld_.rotation_.x, reticleWorld_.rotation_.y,reticleWorld_.rotation_.z);
	ImGui::Text("scale :%4.1f/%4.1f/%4.1f", reticleWorld_.scale_.x, reticleWorld_.scale_.y,reticleWorld_.scale_.z);
	size_t a = input_->GetNumberOfJoysticks();
	ImGui::Text("a %d", a);	
	XINPUT_STATE joyState;
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		ImGui::Text("%f/%f", (float)joyState.Gamepad.sThumbLX, (float)joyState.Gamepad.sThumbLY);
		ImGui::Text("%f/%f", (float)joyState.Gamepad.sThumbRX, (float)joyState.Gamepad.sThumbRY);	
	}
	ImGui::End();
#endif // _DEBUG
	
	//移動処理
	Move(depth);
	
	//行列更新
	reticleWorld_.UpdateMatrix();

	Vector3 posRE = GetmatW();

	//2Dレティクル移動
	Matrix4x4 matvp = MakeViewportM(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
	Matrix4x4 matvpv = Multiply(Multiply(view_->matView, view_->matProjection), matvp);
	Vector3 pos = Transform(posRE, matvpv);
	R2D_->SetPosition({pos.x,pos.y});
}

void Reticle::Draw(ViewProjection view) { 

	//model_->Draw(reticleWorld_, view);

	
}