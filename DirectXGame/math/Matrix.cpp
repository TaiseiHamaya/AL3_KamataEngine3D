#include "Matrix.h"

#include <vector>

const Matrix4x4 Matrix4x4::identity = {
	{{1,0,0,0},
	{0,1,0,0},
	{0,0,1,0},
	{0,0,0,1}}
};
Matrix3x3::Matrix3x3(const Matrix<3, 3>&& rhs) noexcept {
	__matrix = rhs;
}

Matrix3x3::Matrix3x3(const std::initializer_list<std::initializer_list<float>>& init) {
	assert(init.size() == 3 && init.begin()->size() == 3);
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			__matrix[i][j] = *((init.begin() + i)->begin() + j);
		}
	}
}

Matrix3x3 Matrix3x3::Multiply(const Matrix3x3& matrix, const float& times) {
	return Matrix<3, 3>::Multiply(matrix.__matrix, times);
}

Matrix3x3 Matrix3x3::Inverse(const Matrix3x3& matrix) {
	return matrix.inverse();
}

Matrix3x3 Matrix3x3::Transpose(const Matrix3x3& matrix) {
	return matrix.transpose();
}

const Matrix3x3 Matrix3x3::inverse() const {
	float length =
		__matrix[0][0] * __matrix[1][1] * __matrix[2][2] +
		__matrix[0][1] * __matrix[1][2] * __matrix[2][0] +
		__matrix[0][2] * __matrix[1][0] * __matrix[2][1] -

		__matrix[0][2] * __matrix[1][1] * __matrix[2][0] -
		__matrix[0][1] * __matrix[1][0] * __matrix[2][2] -
		__matrix[0][0] * __matrix[1][2] * __matrix[2][1];
	assert(length != 0);
	Matrix3x3 result{
		{ (__matrix[1][1] * __matrix[2][2] - __matrix[1][2] * __matrix[2][1]) / length, -(__matrix[0][1] * __matrix[2][2] - __matrix[0][2] * __matrix[2][1]) / length, (__matrix[0][1] * __matrix[1][2] - __matrix[0][2] * __matrix[1][1]) / length },
		{ -(__matrix[1][0] * __matrix[2][2] - __matrix[1][2] * __matrix[2][0]) / length, (__matrix[0][0] * __matrix[2][2] - __matrix[0][2] * __matrix[2][0]) / length, -(__matrix[0][0] * __matrix[1][2] - __matrix[0][2] * __matrix[1][0]) / length },
		{ (__matrix[1][0] * __matrix[2][1] - __matrix[1][1] * __matrix[2][0]) / length, -(__matrix[0][0] * __matrix[2][1] - __matrix[0][1] * __matrix[2][0]) / length, (__matrix[0][0] * __matrix[1][1] - __matrix[0][1] * __matrix[1][0]) / length },
	};
	return result;
}

const Matrix3x3 Matrix3x3::transpose() const {
	return __matrix.transpose();
}

Matrix4x4::Matrix4x4(const Matrix<4, 4>& rhs) {
	__matrix = rhs;
}

Matrix4x4::Matrix4x4(std::initializer_list<std::initializer_list<float>> init) {
	assert(init.size() == 4 && init.begin()->size() == 4);
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			__matrix[i][j] = *((init.begin() + i)->begin() + j);
		}
	}
}

Matrix4x4 Matrix4x4::Multiply(const Matrix4x4& matrix, const float& times) {
	return Matrix<4, 4>::Multiply(matrix.__matrix, times);
}

Matrix4x4 Matrix4x4::Inverse(const Matrix4x4& matrix) {
	return matrix.inverse();
}

Matrix4x4 Matrix4x4::Transpose(const Matrix4x4& matrix) {
	return matrix.transpose();
}

const Matrix4x4 Matrix4x4::inverse() const {
	constexpr size_t AugmentedColumn = COLUMN * 2;
	Matrix<ROW, AugmentedColumn> augmented;
	for (size_t i = 0; i < ROW; ++i) {
		for (size_t j = 0; j < COLUMN; ++j) {
			augmented[i][j] = __matrix[i][j];
		}
		augmented[i][i + COLUMN] = 1;
	}

	for (size_t i = 0; i < ROW; ++i) {
		// [i][i] = 1だと左側が単位行列にならないので、別の行と交換して解決
		if (augmented[i][i] == 0) {
			bool found = false;
			for (size_t k = i + 1; k < ROW; ++k) {
				if (augmented[k][i] != 0) {
					std::swap(augmented[i], augmented[k]);
					found = true;
					break;
				}
			}
			if (!found) {
				// 交換できない場合は何かがおかしいので止める
				assert(false);
			}
		}

		// 単位行列にするため[i][i]を取得
		float pivot = augmented[i][i];
		for (size_t j = 0; j < AugmentedColumn; ++j) {
			augmented[i][j] /= pivot;
		}
		// すべての列について、i != kの場合は0にするため別の行から持ってきてどうにかする
		for (size_t k = 0; k < ROW; ++k) {
			if (k != i) {
				// その他の行のi要素を0にする
				float factor = augmented[k][i];
				if (factor != 0) {
					// 定義より、k行のすべての列をi行目の定数倍する
					for (size_t j = 0; j < AugmentedColumn; ++j) {
						augmented[k][j] -= factor * augmented[i][j];
					}
				}
			}
		}
	}

	// 逆行列を抽出
	Matrix4x4 inversed{};
	for (size_t i = 0; i < 4; ++i) {
		for (size_t j = 0; j < 4; ++j) {
			inversed[i][j] = augmented[i][j + 4];
		}
	}

	// 正しくinverseできたかチェック(デバッグのみ)
#ifdef _DEBUG
	bool isInversed = true;
	for (size_t i = 0; i < 4; ++i) {
		for (size_t j = 0; j < 4; ++j) {
			if (!(i == j ? augmented[i][j] == 1 : augmented[i][j] == 0)) {
				isInversed = false;
			}
		}
	}
	assert(isInversed);
#endif // _DEBUG

	return inversed;
}

const Matrix4x4 Matrix4x4::transpose() const {
	return __matrix.transpose();
}