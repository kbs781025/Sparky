#include "vec4.h"

namespace sparky {
	namespace maths {
		vec4::vec4(const float x, const float y, const float z, const float w)
		{
			this->x = x;
			this->y = y;
			this->z = z;
			this->w = w;
		}

		vec4 & vec4::operator+=(const vec4 & other)
		{
			x += other.x;
			y += other.y;
			z += other.z;
			w += other.w;
			return *this;
		}

		vec4 & vec4::operator-=(const vec4 & other)
		{
			x -= other.x;
			y -= other.y;
			z -= other.z;
			w -= other.w;
			return *this;
		}

		vec4 & vec4::operator*=(const vec4 & other)
		{
			x *= other.x;
			y *= other.y;
			z *= other.z;
			w *= other.w;
			return *this;
		}

		vec4 & vec4::operator/=(const vec4 & other)
		{
			x /= other.x;
			y /= other.y;
			z /= other.z;
			w /= other.w;
			return *this;
		}

		std::ostream & operator<<(std::ostream & os, const vec4 & vec)
		{
			os << "vec4 : <" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ">";
			return os;
		}

		vec4 operator+(const vec4 & lhs, const vec4 & rhs)
		{
			return vec4(lhs) += rhs;
		}

		vec4 operator-(const vec4 & lhs, const vec4 & rhs)
		{
			return vec4(lhs) -= rhs;
		}

		vec4 operator*(const vec4 & lhs, const vec4 & rhs)
		{
			return vec4(lhs) *= rhs;
		}

		vec4 operator/(const vec4 & lhs, const vec4 & rhs)
		{
			return vec4(lhs) /= rhs;
		}

	}
}