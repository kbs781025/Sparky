#pragma once

#include <iostream>

namespace sparky { namespace maths {

	struct vec2
	{
		float x, y;

		vec2();
		vec2(const float x, const float y);
		vec2(const vec2& rhs);

		vec2& operator+=(const vec2& other);
		vec2& operator-=(const vec2& other);
		vec2& operator*=(const vec2& other);
		vec2& operator/=(const vec2& other);

		friend std::ostream& operator<<(std::ostream& os, const vec2& vec);
	};

	vec2 operator+(const vec2& lhs, const vec2& rhs);
	vec2 operator-(const vec2& lhs, const vec2& rhs);
	vec2 operator*(const vec2& lhs, const vec2& rhs);
	vec2 operator/(const vec2& lhs, const vec2& rhs);
} }