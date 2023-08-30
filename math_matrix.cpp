#include"math_matrix.h"
#include<Vector2.h>
#include <cassert>
#include <cmath>
#include<iostream>

Vector2 CheckRotateFromVelo(const Vector3& v) {
	float length = LengV2({v.x,v.z});

	return {       
	    std::atan2(-v.y, length),
	    std::atan2(v.x, v.z),
	};
}

float CheckR_F_Y(const Vector2& v) { return std::atan2(v.x, v.y); }

Vector3 Esing(Vector3 st, Vector3 ed, float t) { 
	return {
		st.x * (1.0f - t) + ed.x * t, 
	    st.y * (1.0f - t) + ed.y * t,
	    st.z * (1.0f - t) + ed.z * t,
	}; 
}

// エリア内にもどす（円形）引数のposの数字を変えて
bool SetAreaEllipse(const Vector2& center, Vector2& pos, const float area,Vector2& velo_) {

	Vector2 velo = SubV2(pos, center);
	float length = LengV2(velo);

	// 枠外で処理開始
	if (length > area) {
		// 長さを正規化して最大値まで伸ばす
		velo = NorV2(velo);
		pos= ScaV2(area,velo);

		velo_ = velo;

		
		return true;
	}
	velo_ = {0, 0};

	return false;
}

Vector2 Esing(const Vector2 zero, const Vector2 one, float t) {
	return {
	    zero.x * (1.0f - t) + one.x * t,
	    zero.y * (1.0f - t) + one.y * t,
	};
}

Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m) {
	return {
	    v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0],
	    v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1],
	    v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2],

	};
}
Matrix4x4 ViewportAL(float VpWidth, float VpHeight, float OffsetX, float OffsetY) {
	return {VpWidth / 2, 0, 0, 0, 0, -VpHeight / 2,         0,
	        0,           0, 0, 1, 0, VpWidth / 2 + OffsetX, VpHeight / 2 + OffsetY,
	        0,           1};
}

// クロス積
Vector3 Cross(const Vector3& v1, const Vector3& v2) {
	return {v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x};
}

// 投資投影
Matrix4x4 MakePerspectiveFovM(float fovY, float aspectRatio, float nearClip, float farClip) {
	return {
	    (1.0f / aspectRatio) * (1.0f / std::tan(fovY / 2.0f)),
	    0,
	    0,
	    0,
	    0,
	    (1.0f / std::tan(fovY / 2.0f)),
	    0,
	    0,
	    0,
	    0,
	    farClip / (farClip - nearClip),
	    1,
	    0,
	    0,
	    (-nearClip * farClip) / (farClip - nearClip),
	    0};
}
// 正射影行列
Matrix4x4 MakeOrthographicM(
    float left, float top, float right, float bottom, float nearClip, float farClip) {
	return {
	    2.0f / (right - left),
	    0,
	    0,
	    0,
	    0,
	    2.0f / (top - bottom),
	    0,
	    0,
	    0,
	    0,
	    1.0f / (farClip - nearClip),
	    0,
	    (left + right) / (left - right),
	    (top + bottom) / (bottom - top),
	    nearClip / (nearClip - farClip),
	    1};
}
// ビューポート変換行列
Matrix4x4 MakeViewportM(
    float left, float top, float width, float height, float minDepth, float maxDepth) {
	return {
	    width / 2.0f,
	    0,
	    0,
	    0,
	    0,
	    -height / 2.0f,
	    0,
	    0,
	    0,
	    0,
	    maxDepth - minDepth,
	    0,
	    left + (width / 2.0f),
	    top + (height / 2.0f),
	    minDepth,
	    1};
}

Matrix4x4 MakeAffineM(const Vector3& Sca, const Vector3& Rota, const Vector3& Trans) {

	Matrix4x4 S = MakeScaleMatrix(Sca);
	Matrix4x4 R = MakeAllRotateM(Rota);
	Matrix4x4 T = MakeTranslateMatrix(Trans);

	return Multiply(S, Multiply(R, T));
}

// 平行移動
Matrix4x4 MakeTranslateMatrix(const Vector3& T) {
	return {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, T.x, T.y, T.z, 1};
}
// 拡縮
Matrix4x4 MakeScaleMatrix(const Vector3& S) {
	return {S.x, 0, 0, 0, 0, S.y, 0, 0, 0, 0, S.z, 0, 0, 0, 0, 1};
};
// 座標変換
Vector3 Transform(const Vector3& v, const Matrix4x4& m) {
	Vector3 result = {
	    v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + 1.0f * m.m[3][0],
	    v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + 1.0f * m.m[3][1],
	    v.x * m.m[0][2] + v.y * m.m[1][1] + v.z * m.m[2][2] + 1.0f * m.m[3][2],
	};
	float w = v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + 1.0f * m.m[3][3];

	assert(w != 0);
	if (w != 0) {
		return {
		    result.x / w,
		    result.y / w,
		    result.z / w,
		};
	} else {
		return result;
	}
}

// x軸回転行列
Matrix4x4 MakeRotateXM(float radian) {
	return {1,
	        0,
	        0,
	        0,
	        0,
	        std::cos(radian),
	        std::sin(radian),
	        0,
	        0,
	        -std::sin(radian),
	        std::cos(radian),
	        0,
	        0,
	        0,
	        0,
	        1};
}
// y軸回転行列
Matrix4x4 MakeRotateYM(float radian) {
	return {std::cos(radian), 0, -std::sin(radian), 0, 0, 1, 0, 0,
	        std::sin(radian), 0, std::cos(radian),  0, 0, 0, 0, 1};
}
// z軸回転行列
Matrix4x4 MakeRotateZM(float radian) {
	return {
	    std::cos(radian),
	    std::sin(radian),
	    0,
	    0,
	    -std::sin(radian),
	    std::cos(radian),
	    0,
	    0,
	    0,
	    0,
	    1,
	    0,
	    0,
	    0,
	    0,
	    1};
}

// 全軸回転
Matrix4x4 MakeAllRotateM(Vector3 rotate) {
	Matrix4x4 Rx = MakeRotateXM(rotate.x);
	Matrix4x4 Ry = MakeRotateYM(rotate.y);
	Matrix4x4 Rz = MakeRotateZM(rotate.z);

	return Multiply(Rx, Multiply(Ry, Rz));
}

#pragma region V3math

// 加算
Vector3 Add(const Vector3& v1, const Vector3& v2) {
	return {
	    v1.x + v2.x,
	    v1.y + v2.y,
	    v1.z + v2.z,
	};
}

// 減算　
Vector3 Subtract(const Vector3& v1, const Vector3& v2) {
	return {
	    v1.x - v2.x,
	    v1.y - v2.y,
	    v1.z - v2.z,
	};
}

// スカラー倍
Vector3 Multiply(float scalar, const Vector3& v) {
	return {
	    v.x * scalar,
	    v.y * scalar,
	    v.z * scalar,
	};
}

// 内積
float Dot(const Vector3& v1, const Vector3& v2) { return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z; }

// 長さ
float Length(const Vector3& v) { return sqrtf(Dot(v, v)); }

// 正規化
Vector3 Normalize(const Vector3& v) {
	float leng = Length(v);
	assert(leng != 0);
	return {
	    v.x / leng,
	    v.y / leng,
	    v.z / leng,
	};
}
#pragma endregion

#pragma region M4x4
// 加算
Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2) {
	return {
	    m1.m[0][0] + m2.m[0][0], m1.m[0][1] + m2.m[0][1],
	    m1.m[0][2] + m2.m[0][2], m1.m[0][3] + m2.m[0][3],

	    m1.m[1][0] + m2.m[1][0], m1.m[1][1] + m2.m[1][1],
	    m1.m[1][2] + m2.m[1][2], m1.m[1][3] + m2.m[1][3],

	    m1.m[2][0] + m2.m[2][0], m1.m[2][1] + m2.m[2][1],
	    m1.m[2][2] + m2.m[2][2], m1.m[2][3] + m2.m[2][3],

	    m1.m[3][0] + m2.m[3][0], m1.m[3][1] + m2.m[3][1],
	    m1.m[3][2] + m2.m[3][2], m1.m[3][3] + m2.m[3][3],
	};
}
// 減算
Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2) {
	return {
	    m1.m[0][0] - m2.m[0][0], m1.m[0][1] - m2.m[0][1],
	    m1.m[0][2] - m2.m[0][2], m1.m[0][3] - m2.m[0][3],

	    m1.m[1][0] - m2.m[1][0], m1.m[1][1] - m2.m[1][1],
	    m1.m[1][2] - m2.m[1][2], m1.m[1][3] - m2.m[1][3],

	    m1.m[2][0] - m2.m[2][0], m1.m[2][1] - m2.m[2][1],
	    m1.m[2][2] - m2.m[2][2], m1.m[2][3] - m2.m[2][3],

	    m1.m[3][0] - m2.m[3][0], m1.m[3][1] - m2.m[3][1],
	    m1.m[3][2] - m2.m[3][2], m1.m[3][3] - m2.m[3][3],
	};
}
// 積
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
	return {
	    m1.m[0][0] * m2.m[0][0] + m1.m[0][1] * m2.m[1][0] + m1.m[0][2] * m2.m[2][0] +
	        m1.m[0][3] * m2.m[3][0],
	    m1.m[0][0] * m2.m[0][1] + m1.m[0][1] * m2.m[1][1] + m1.m[0][2] * m2.m[2][1] +
	        m1.m[0][3] * m2.m[3][1],
	    m1.m[0][0] * m2.m[0][2] + m1.m[0][1] * m2.m[1][2] + m1.m[0][2] * m2.m[2][2] +
	        m1.m[0][3] * m2.m[3][2],
	    m1.m[0][0] * m2.m[0][3] + m1.m[0][1] * m2.m[1][3] + m1.m[0][2] * m2.m[2][3] +
	        m1.m[0][3] * m2.m[3][3],

	    m1.m[1][0] * m2.m[0][0] + m1.m[1][1] * m2.m[1][0] + m1.m[1][2] * m2.m[2][0] +
	        m1.m[1][3] * m2.m[3][0],
	    m1.m[1][0] * m2.m[0][1] + m1.m[1][1] * m2.m[1][1] + m1.m[1][2] * m2.m[2][1] +
	        m1.m[1][3] * m2.m[3][1],
	    m1.m[1][0] * m2.m[0][2] + m1.m[1][1] * m2.m[1][2] + m1.m[1][2] * m2.m[2][2] +
	        m1.m[1][3] * m2.m[3][2],
	    m1.m[1][0] * m2.m[0][3] + m1.m[1][1] * m2.m[1][3] + m1.m[1][2] * m2.m[2][3] +
	        m1.m[1][3] * m2.m[3][3],

	    m1.m[2][0] * m2.m[0][0] + m1.m[2][1] * m2.m[1][0] + m1.m[2][2] * m2.m[2][0] +
	        m1.m[2][3] * m2.m[3][0],
	    m1.m[2][0] * m2.m[0][1] + m1.m[2][1] * m2.m[1][1] + m1.m[2][2] * m2.m[2][1] +
	        m1.m[2][3] * m2.m[3][1],
	    m1.m[2][0] * m2.m[0][2] + m1.m[2][1] * m2.m[1][2] + m1.m[2][2] * m2.m[2][2] +
	        m1.m[2][3] * m2.m[3][2],
	    m1.m[2][0] * m2.m[0][3] + m1.m[2][1] * m2.m[1][3] + m1.m[2][2] * m2.m[2][3] +
	        m1.m[2][3] * m2.m[3][3],

	    m1.m[3][0] * m2.m[0][0] + m1.m[3][1] * m2.m[1][0] + m1.m[3][2] * m2.m[2][0] +
	        m1.m[3][3] * m2.m[3][0],
	    m1.m[3][0] * m2.m[0][1] + m1.m[3][1] * m2.m[1][1] + m1.m[3][2] * m2.m[2][1] +
	        m1.m[3][3] * m2.m[3][1],
	    m1.m[3][0] * m2.m[0][2] + m1.m[3][1] * m2.m[1][2] + m1.m[3][2] * m2.m[2][2] +
	        m1.m[3][3] * m2.m[3][2],
	    m1.m[3][0] * m2.m[0][3] + m1.m[3][1] * m2.m[1][3] + m1.m[3][2] * m2.m[2][3] +
	        m1.m[3][3] * m2.m[3][3],

	};
}
// 逆行列
Matrix4x4 Inverse(const Matrix4x4& m) {
	float A = (m.m[0][0] * m.m[1][1] * m.m[2][2] * m.m[3][3]) +
	          (m.m[0][0] * m.m[1][2] * m.m[2][3] * m.m[3][1]) +
	          (m.m[0][0] * m.m[1][3] * m.m[2][1] * m.m[3][2]) -
	          (m.m[0][0] * m.m[1][3] * m.m[2][2] * m.m[3][1]) -
	          (m.m[0][0] * m.m[1][2] * m.m[2][1] * m.m[3][3]) -
	          (m.m[0][0] * m.m[1][1] * m.m[2][3] * m.m[3][2]) -
	          (m.m[0][1] * m.m[1][0] * m.m[2][2] * m.m[3][3]) -
	          (m.m[0][2] * m.m[1][0] * m.m[2][3] * m.m[3][1]) -
	          (m.m[0][3] * m.m[1][0] * m.m[2][1] * m.m[3][2]) +
	          (m.m[0][3] * m.m[1][0] * m.m[2][2] * m.m[3][1]) +
	          (m.m[0][2] * m.m[1][0] * m.m[2][1] * m.m[3][3]) +
	          (m.m[0][1] * m.m[1][0] * m.m[2][3] * m.m[3][2]) +
	          (m.m[0][1] * m.m[1][2] * m.m[2][0] * m.m[3][3]) +
	          (m.m[0][2] * m.m[1][3] * m.m[2][0] * m.m[3][1]) +
	          (m.m[0][3] * m.m[1][1] * m.m[2][0] * m.m[3][2]) -
	          (m.m[0][3] * m.m[1][2] * m.m[2][0] * m.m[3][1]) -
	          (m.m[0][2] * m.m[1][1] * m.m[2][0] * m.m[3][3]) -
	          (m.m[0][1] * m.m[1][3] * m.m[2][0] * m.m[3][2]) -
	          (m.m[0][1] * m.m[1][2] * m.m[2][3] * m.m[3][0]) -
	          (m.m[0][2] * m.m[1][3] * m.m[2][1] * m.m[3][0]) -
	          (m.m[0][3] * m.m[1][1] * m.m[2][2] * m.m[3][0]) +
	          (m.m[0][3] * m.m[1][2] * m.m[2][1] * m.m[3][0]) +
	          (m.m[0][2] * m.m[1][1] * m.m[2][3] * m.m[3][0]) +
	          (m.m[0][1] * m.m[1][3] * m.m[2][2] * m.m[3][0]);

	A = 1.0f / A;
	return {
	    A * ((m.m[1][1] * m.m[2][2] * m.m[3][3]) + (m.m[1][2] * m.m[2][3] * m.m[3][1]) +
	         (m.m[1][3] * m.m[2][1] * m.m[3][2]) - (m.m[1][3] * m.m[2][2] * m.m[3][1]) -
	         (m.m[1][2] * m.m[2][1] * m.m[3][3]) - (m.m[1][1] * m.m[2][3] * m.m[3][2])),
	    A * (-(m.m[0][1] * m.m[2][2] * m.m[3][3]) - (m.m[0][2] * m.m[2][3] * m.m[3][1]) -
	         (m.m[0][3] * m.m[2][1] * m.m[3][2]) + (m.m[0][3] * m.m[2][2] * m.m[3][1]) +
	         (m.m[0][2] * m.m[2][1] * m.m[3][3]) + (m.m[0][1] * m.m[2][3] * m.m[3][2])),
	    A * ((m.m[0][1] * m.m[1][2] * m.m[3][3]) + (m.m[0][2] * m.m[1][3] * m.m[3][1]) +
	         (m.m[0][3] * m.m[1][1] * m.m[3][2]) - (m.m[0][3] * m.m[1][2] * m.m[3][1]) -
	         (m.m[0][2] * m.m[1][1] * m.m[3][3]) - (m.m[0][1] * m.m[1][3] * m.m[3][2])),
	    A * (-(m.m[0][1] * m.m[1][2] * m.m[2][3]) - (m.m[0][2] * m.m[1][3] * m.m[2][1]) -
	         (m.m[0][3] * m.m[1][1] * m.m[2][2]) + (m.m[0][3] * m.m[1][2] * m.m[2][1]) +
	         (m.m[0][2] * m.m[1][1] * m.m[2][3]) + (m.m[0][1] * m.m[1][3] * m.m[2][2])),

	    A * (-(m.m[1][0] * m.m[2][2] * m.m[3][3]) - (m.m[1][2] * m.m[2][3] * m.m[3][0]) -
	         (m.m[1][3] * m.m[2][0] * m.m[3][2]) + (m.m[1][3] * m.m[2][2] * m.m[3][0]) +
	         (m.m[1][2] * m.m[2][0] * m.m[3][3]) + (m.m[1][0] * m.m[2][3] * m.m[3][2])),
	    A * (+(m.m[0][0] * m.m[2][2] * m.m[3][3]) + (m.m[0][2] * m.m[2][3] * m.m[3][0]) +
	         (m.m[0][3] * m.m[2][0] * m.m[3][2]) - (m.m[0][3] * m.m[2][2] * m.m[3][0]) -
	         (m.m[0][2] * m.m[2][0] * m.m[3][3]) - (m.m[0][0] * m.m[2][3] * m.m[3][2])),
	    A * (-(m.m[0][0] * m.m[1][2] * m.m[3][3]) - (m.m[0][2] * m.m[1][3] * m.m[3][0]) -
	         (m.m[0][3] * m.m[1][0] * m.m[3][2]) + (m.m[0][3] * m.m[1][2] * m.m[3][0]) +
	         (m.m[0][2] * m.m[1][0] * m.m[3][3]) + (m.m[0][0] * m.m[1][3] * m.m[3][2])),
	    A * (+(m.m[0][0] * m.m[1][2] * m.m[2][3]) + (m.m[0][2] * m.m[1][3] * m.m[2][0]) +
	         (m.m[0][3] * m.m[1][0] * m.m[2][2]) - (m.m[0][3] * m.m[1][2] * m.m[2][0]) -
	         (m.m[0][2] * m.m[1][0] * m.m[2][3]) - (m.m[0][0] * m.m[1][3] * m.m[2][2])),

	    A * (+(m.m[1][0] * m.m[2][1] * m.m[3][3]) + (m.m[1][1] * m.m[2][3] * m.m[3][0]) +
	         (m.m[1][3] * m.m[2][0] * m.m[3][1]) - (m.m[1][3] * m.m[2][1] * m.m[3][0]) -
	         (m.m[1][1] * m.m[2][0] * m.m[3][3]) - (m.m[1][0] * m.m[2][3] * m.m[3][1])),
	    A * (-(m.m[0][0] * m.m[2][1] * m.m[3][3]) - (m.m[0][1] * m.m[2][3] * m.m[3][0]) -
	         (m.m[0][3] * m.m[2][0] * m.m[3][1]) + (m.m[0][3] * m.m[2][1] * m.m[3][0]) +
	         (m.m[0][1] * m.m[2][0] * m.m[3][3]) + (m.m[0][0] * m.m[2][3] * m.m[3][1])),
	    A * (+(m.m[0][0] * m.m[1][1] * m.m[3][3]) + (m.m[0][1] * m.m[1][3] * m.m[3][0]) +
	         (m.m[0][3] * m.m[1][0] * m.m[3][1]) - (m.m[0][3] * m.m[1][1] * m.m[3][0]) -
	         (m.m[0][1] * m.m[1][0] * m.m[3][3]) - (m.m[0][0] * m.m[1][3] * m.m[3][1])),
	    A * (-(m.m[0][0] * m.m[1][1] * m.m[2][3]) - (m.m[0][1] * m.m[1][3] * m.m[2][0]) -
	         (m.m[0][3] * m.m[1][0] * m.m[2][1]) + (m.m[0][3] * m.m[1][1] * m.m[2][0]) +
	         (m.m[0][1] * m.m[1][0] * m.m[2][3]) + (m.m[0][0] * m.m[1][3] * m.m[2][1])),

	    A * (-(m.m[1][0] * m.m[2][1] * m.m[3][2]) - (m.m[1][1] * m.m[2][2] * m.m[3][0]) -
	         (m.m[1][2] * m.m[2][0] * m.m[3][1]) + (m.m[1][2] * m.m[2][1] * m.m[3][0]) +
	         (m.m[1][1] * m.m[2][0] * m.m[3][2]) + (m.m[1][0] * m.m[2][2] * m.m[3][1])),
	    A * (+(m.m[0][0] * m.m[2][1] * m.m[3][2]) + (m.m[0][1] * m.m[2][2] * m.m[3][0]) +
	         (m.m[0][2] * m.m[2][0] * m.m[3][1]) - (m.m[0][2] * m.m[2][1] * m.m[3][0]) -
	         (m.m[0][1] * m.m[2][0] * m.m[3][2]) - (m.m[0][0] * m.m[2][2] * m.m[3][1])),
	    A * (-(m.m[0][0] * m.m[1][1] * m.m[3][2]) - (m.m[0][1] * m.m[1][2] * m.m[3][0]) -
	         (m.m[0][2] * m.m[1][0] * m.m[3][1]) + (m.m[0][2] * m.m[1][1] * m.m[3][0]) +
	         (m.m[0][1] * m.m[1][0] * m.m[3][2]) + (m.m[0][0] * m.m[1][2] * m.m[3][1])),
	    A * (+(m.m[0][0] * m.m[1][1] * m.m[2][2]) + (m.m[0][1] * m.m[1][2] * m.m[2][0]) +
	         (m.m[0][2] * m.m[1][0] * m.m[2][1]) - (m.m[0][2] * m.m[1][1] * m.m[2][0]) -
	         (m.m[0][1] * m.m[1][0] * m.m[2][2]) - (m.m[0][0] * m.m[1][2] * m.m[2][1])),

	};
}

// 転置行列
Matrix4x4 Transpose(const Matrix4x4& m) {
	return {
	    m.m[0][0], m.m[1][0], m.m[2][0], m.m[3][0], m.m[0][1], m.m[1][1], m.m[2][1], m.m[3][1],
	    m.m[0][2], m.m[1][2], m.m[2][2], m.m[3][2], m.m[0][3], m.m[1][3], m.m[2][3], m.m[3][3],
	};
}
// 単位行列の作成
Matrix4x4 MakeIdentity4x4() { return {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1}; }

#pragma endregion

#pragma region V2
Vector2 AddV2(Vector2 v1, Vector2 v2) { return {v1.x + v2.x, v1.y + v2.y};}

Vector2 SubV2(Vector2 v1, Vector2 v2) { return {v1.x - v2.x, v1.y - v2.y}; }

Vector2 ScaV2(float A, Vector2 v) { return {v.x * A, v.y * A}; }

float LengV2(Vector2 v) { return sqrtf(v.x * v.x + v.y * v.y); }

Vector2 NorV2(Vector2 v) { 
	float L = LengV2(v);
	return {
	    v.x / L,
	    v.y / L,
	};
}
#pragma endregion
