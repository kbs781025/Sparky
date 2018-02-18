#pragma once

#include <iostream>

namespace sparky {
	namespace maths {

		struct vec4
		{
			float x, y, z, w;

			vec4() = default;
			vec4(const float x, const float y, const float z, const float w);
			vec4(const vec4& rhs) = default;

			vec4& operator+=(const vec4& other);
			vec4& operator-=(const vec4& other);
			vec4& operator*=(const vec4& other);
			vec4& operator/=(const vec4& other);

			friend std::ostream& operator<<(std::ostream& os, const vec4& vec);
		};

		vec4 operator+(const vec4& lhs, const vec4& rhs);
		vec4 operator-(const vec4& lhs, const vec4& rhs);
		vec4 operator*(const vec4& lhs, const vec4& rhs);
		vec4 operator/(const vec4& lhs, const vec4& rhs);
	}
}