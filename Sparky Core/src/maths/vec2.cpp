#include "vec2.h"

namespace sparky { namespace maths {
	vec2::vec2()
	{
		x = 0.0f;
		y = 0.0f;
	}

	vec2::vec2(const float x, const float y)
	{
		this->x = x;
		this->y = y;
	}

	vec2::vec2(const vec2 & rhs)
	{
		x = rhs.x;
		y = rhs.y;
	}

	vec2 & vec2::operator+=(const vec2 & other)
	{
		/**this = *this + other;
		return *this;*/
		x += other.x;
		y += other.y;
		return *this;
	}

	vec2 & vec2::operator-=(const vec2 & other)
	{
		x -= other.x;
		y -= other.y;
		return *this;
	}

	vec2 & vec2::operator*=(const vec2 & other)
	{
		x *= other.x;
		y *= other.y;
		return *this;
	}

	vec2 & vec2::operator/=(const vec2 & other)
	{
		x /= other.x;
		y /= other.y;
		return *this;
	}

	std::ostream & operator<<(std::ostream & os, const vec2 & vec)
	{
		os << "vec2 : <" << vec.x << ", " << vec.y << ">";
		return os;
	}

	vec2 operator+(const vec2 & lhs, const vec2 & rhs)
	{
		return vec2(lhs) += rhs;
	}

	vec2 operator-(const vec2 & lhs, const vec2 & rhs)
	{
		return vec2(lhs) -= rhs;
	}

	vec2 operator*(const vec2 & lhs, const vec2 & rhs)
	{
		return vec2(lhs) *= rhs;
	}

	vec2 operator/(const vec2 & lhs, const vec2 & rhs)
	{
		return vec2(lhs) /= rhs;
	}

	}
}