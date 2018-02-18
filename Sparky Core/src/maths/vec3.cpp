#include "vec3.h"

namespace sparky {
	namespace maths {
		vec3::vec3()
		{
			x = 0.0f;
			y = 0.0f;
			z = 0.0f;
		}

		vec3::vec3(const float x, const float y, const float z)
		{
			this->x = x;
			this->y = y;
			this->z = z;
		}

		vec3::vec3(const vec3 & rhs)
		{
			x = rhs.x;
			y = rhs.y;
			z = rhs.z;
		}

		vec3 & vec3::operator+=(const vec3 & other)
		{
			x += other.x;
			y += other.y;
			z += other.z;
			return *this;
		}

		vec3 & vec3::operator-=(const vec3 & other)
		{
			x -= other.x;
			y -= other.y;
			z -= other.z;
			return *this;
		}

		vec3 & vec3::operator*=(const vec3 & other)
		{
			x *= other.x;
			y *= other.y;
			z *= other.z;
			return *this;
		}

		vec3 & vec3::operator/=(const vec3 & other)
		{
			x /= other.x;
			y /= other.y;
			z /= other.z;
			return *this;
		}

		std::ostream & operator<<(std::ostream & os, const vec3 & vec)
		{
			os << "vec3 : <" << vec.x << ", " << vec.y << ", " << vec.z << ">";
			return os;
		}

		vec3 operator+(const vec3 & lhs, const vec3 & rhs)
		{
			return vec3(lhs) += rhs;
		}

		vec3 operator-(const vec3 & lhs, const vec3 & rhs)
		{
			return vec3(lhs) -= rhs;
		}

		vec3 operator*(const vec3 & lhs, const vec3 & rhs)
		{
			return vec3(lhs) *= rhs;
		}

		vec3 operator/(const vec3 & lhs, const vec3 & rhs)
		{
			return vec3(lhs) /= rhs;
		}

	}
}