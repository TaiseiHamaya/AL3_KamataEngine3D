#include "WorldTransform.h"

#include <Transform3D.h>

void WorldTransform::UpdateMatrix() {
	matWorld_ = Transform3D::MakeAffineMatrix(scale_, Quaternion{ rotation_.x,rotation_.y,rotation_.z }, translation_);

	if (parent_) {
		matWorld_ *= parent_->matWorld_;
	}
	
	TransferMatrix();
}
