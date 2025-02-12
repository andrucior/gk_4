#include <cmath>

struct Vector3 {
    float x, y, z;

    Vector3() : x(0), y(0), z(0) {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

    // Vector addition
    Vector3 operator+(const Vector3& other) {
        return Vector3(x + other.x, y + other.y, z + other.z);
    }

    // Scalar multiplication
    Vector3 operator*(float scalar) {
        return Vector3(x * scalar, y * scalar, z * scalar);
    }

    // Dot product
    float dot(const Vector3& other) {
        return x * other.x + y * other.y + z * other.z;
    }

    // Cross product
    Vector3 cross(const Vector3& other) {
        return Vector3(y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x);
    }

    // Normalize the vector
    Vector3 normalize() {
        float length = std::sqrt(x * x + y * y + z * z);
        return *this * (1.0f / length);
    }
};
