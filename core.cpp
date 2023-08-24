#include<core.h>

void Core::Initialize(Model* model, uint32_t textu) {

}

void Core::Update() {


}

void Core::InCollision() { 
	hp_--;
	if (hp_ <= 0) {
		isdead_ = true;
	}
}

void Core::Draw(ViewProjection view) {

}