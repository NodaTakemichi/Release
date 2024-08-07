/*!
 * @brief	行列。
 */

#include "stdafx.h"
#include "Math/Matrix.h"

namespace RasterizerBasedSoftwareRendering {
	const Matrix Matrix::Identity(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}
