#include<WorldTransform.h>
#include"math_matrix.h"

void WorldTransform::UpdateMatrix() {
	matWorld_ = MakeAffineM(scale_, rotation_, translation_);
	if (parent_) {
		matWorld_ = Multiply(matWorld_, parent_->matWorld_);
	}
	TransferMatrix();
}
