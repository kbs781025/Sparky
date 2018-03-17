#pragma once

#include <iostream>

namespace sparky {
	namespace maths {

		struct vec3
		{
			float x, y, z;

			vec3();
			vec3(const float x, const float y, const float z);
			vec3(const vec3& rhs);

			vec3& operator+=(const vec3& other);
			vec3& operator-=(const vec3& other);
			vec3& operator*=(const vec3& other);
			vec3& operator/=(const vec3& other);
			vec3& operator=(const vec3& other);

			vec3& normalize();
			vec3 cross(const vec3& other) const;

			friend std::ostream& operator<<(std::ostream& os, const vec3& vec);
		};

		vec3 operator+(const vec3& lhs, const vec3& rhs);
		vec3 operator-(const vec3& lhs, const vec3& rhs);
		vec3 operator*(const vec3& lhs, const vec3& rhs);
		vec3 operator/(const vec3& lhs, const vec3& rhs);
	}
}