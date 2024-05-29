#include "WorldTransform.h"

#include <Transform3D.h>

void WorldTransform::UpdateMatrix() {
	matWorld_ = Transform3D::MakeAffineMatrix(scale_, rotation_, translation_);
	TransferMatrix();
}