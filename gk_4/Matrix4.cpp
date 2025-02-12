#include <algorithm>
#include "Vector3.cpp"

struct Matrix4 {
    float m[16];

    Matrix4() {
        std::fill(std::begin(m), std::end(m), 0.0f);
    }

    // Identity matrix
    static Matrix4 identity() {
        Matrix4 mat;
        mat.m[0] = mat.m[5] = mat.m[10] = mat.m[15] = 1.0f;
        return mat;
    }

    // Matrix multiplication
    Matrix4 operator*(const Matrix4& other) {
        Matrix4 result;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                result.m[i * 4 + j] = 0;
                for (int k = 0; k < 4; ++k) {
                    result.m[i * 4 + j] += m[i * 4 + k] * other.m[k * 4 + j];
                }
            }
        }
        return result;
    }

    // Translation matrix
    static Matrix4 translate(const Vector3& vec) {
        Matrix4 mat = Matrix4::identity();
        mat.m[12] = vec.x;
        mat.m[13] = vec.y;
        mat.m[14] = vec.z;
        return mat;
    }

    // Perspective projection matrix
    static Matrix4 perspective(float fov, float aspect, float near, float far) {
        Matrix4 mat;
        float tanHalfFov = std::tan(fov / 2.0f);

        mat.m[0] = 1.0f / (aspect * tanHalfFov);
        mat.m[5] = 1.0f / tanHalfFov;
        mat.m[10] = -(far + near) / (far - near);
        mat.m[11] = -1.0f;
        mat.m[14] = -(2.0f * far * near) / (far - near);
        mat.m[15] = 0.0f;

        return mat;
    }
};
