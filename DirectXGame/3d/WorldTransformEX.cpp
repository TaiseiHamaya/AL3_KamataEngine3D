#include "WorldTransform.h"

#include "Transform3D.h"

void WorldTransform::UpdateMatrix(const Transform3D& transform) {
	matWorld_ = transform.get_matrix();

	if (parent_) {
		matWorld_ *= parent_->matWorld_;
	}

	TransferMatrix();
}
