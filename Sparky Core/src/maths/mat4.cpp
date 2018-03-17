#include "mat4.h"
#include "mathfun.h"

const float DEG2RAD = 3.141593f / 180.0f;
const float RAD2DEG = 180.0f / 3.141593f;

namespace sparky {
	namespace maths {

		/*mat4::mat4()
		{
		for (int i = 0; i < 4 * 4; i++)
		{
		elements[i] = 0.0f;
		}
		}

		mat4::mat4(float diagonal)
		{
		for (int i = 0; i < 4 * 4; i++)
		{
		elements[i] = 0.0f;
		}

		elements[0 + 0 * 4] = diagonal;
		elements[1 + 1 * 4] = diagonal;
		elements[2 + 2 * 4] = diagonal;
		elements[3 + 3 * 4] = diagonal;

		}

		mat4 mat4::identity()
		{
		return mat4(1.0f);
		}

		mat4 & mat4::multiply(const mat4 & other)
		{
		float data[16];
		for (int row = 0; row < 4; row++)
		{
		for (int col = 0; col < 4; col++)
		{
		float sum = 0.0f;
		for (int e = 0; e < 4; e++)
		{
		sum += elements[row + e * 4] * other.elements[col * 4 + e];
		}
		data[col * 4 + row] = sum;
		}
		}
		memcpy(elements, data, 4 * 4 * sizeof(float));

		return *this;
		}


		mat4 mat4::orthographic(float left, float right, float bottom, float top, float near, float far)
		{
		mat4 result(1.0f);

		result.elements[0 + 0 * 4] = 2.0f / (right - left);

		result.elements[1 + 1 * 4] = 2.0f / (top - bottom);

		result.elements[2 + 2 * 4] = 2.0f / (near - far);

		result.elements[0 + 3 * 4] = (left + right) / (left - right);
		result.elements[1 + 3 * 4] = (bottom + top) / (bottom - top);
		result.elements[2 + 3 * 4] = (far + near) / (far - near);

		return result;
		}

		mat4 mat4::perspective(float fov, float aspectRatio, float near, float far)
		{
		mat4 result(1.0f);

		float q = 1.0f / tan(toRadians(0.5f * fov));
		float a = q / aspectRatio;

		float b = (near + far) / (near - far);
		float c = (2.0f * near * far) / (near - far);

		result.elements[0 + 0 * 4] = a;
		result.elements[1 + 1 * 4] = q;
		result.elements[2 + 2 * 4] = b;
		result.elements[3 + 2 * 4] = -1.0f;
		result.elements[2 + 3 * 4] = c;

		return result;
		}

		mat4 mat4::translate(const vec3 & translation)
		{
		mat4 result(1.0f);

		result.elements[0 + 3 * 4] = translation.x;
		result.elements[1 + 3 * 4] = translation.y;
		result.elements[2 + 3 * 4] = translation.z;

		return result;
		}

		mat4 mat4::rotation(float angle, const vec3 & axis)
		{
		mat4 result(1.0f);

		float r = toRadians(angle);
		float c = cos(r);
		float s = sin(r);
		float omc = 1.0f - c;

		float x = axis.x;
		float y = axis.y;
		float z = axis.z;

		result.elements[0 + 0 * 4] = x * x * omc + c;
		result.elements[1 + 0 * 4] = y * x * omc + z * s;
		result.elements[2 + 0 * 4] = x * z * omc - y * s;

		result.elements[0 + 1 * 4] = x * y * omc - z * s;
		result.elements[1 + 1 * 4] = y * y * omc + c;
		result.elements[2 + 1 * 4] = y * z * omc + x * s;

		result.elements[0 + 2 * 4] = x * z * omc + y * s;
		result.elements[1 + 2 * 4] = y * z * omc - x * s;
		result.elements[2 + 2 * 4] = z * z * omc + c;

		return result;
		}

		mat4 mat4::scale(const vec3 & scale)
		{
		mat4 result(1.0f);

		result.elements[0 + 0 * 4] = scale.x;
		result.elements[1 + 1 * 4] = scale.y;
		result.elements[2 + 2 * 4] = scale.z;

		return result;
		}

		mat4 operator*(mat4 left, const mat4 & right)
		{
		return left.multiply(right);
		}

		vec3 operator*(const mat4 & left, const vec3 & right)
		{
		return left.multiply(right);
		}

		vec4 operator*(const mat4 & left, const vec4 & right)
		{
		return left.multiply(right);
		}

		mat4 & mat4::operator*=(const mat4 & other)
		{
		return multiply(other);
		}

		vec3 mat4::multiply(const vec3 & other) const
		{
		return vec3(
		colums[0].x * other.x + colums[1].x * other.y + colums[2].x * other.z + colums[3].x,
		colums[0].y * other.x + colums[1].y * other.y + colums[2].y * other.z + colums[3].y,
		colums[0].z * other.x + colums[1].z * other.y + colums[2].z * other.z + colums[3].z
		);
		}

		vec4 mat4::multiply(const vec4 & other) const
		{
		return vec4(
		colums[0].x * other.x + colums[1].x * other.y + colums[2].x * other.z + colums[3].x * other.w,
		colums[0].y * other.x + colums[1].y * other.y + colums[2].y * other.z + colums[3].y * other.w,
		colums[0].z * other.x + colums[1].z * other.y + colums[2].z * other.z + colums[3].z * other.w,
		colums[0].w * other.x + colums[1].w * other.y + colums[2].w * other.z + colums[3].w * other.w
		);
		}*/

		mat4& mat4::transpose()
		{
			std::swap(m[1], m[4]);
			std::swap(m[2], m[8]);
			std::swap(m[3], m[12]);
			std::swap(m[6], m[9]);
			std::swap(m[7], m[13]);
			std::swap(m[11], m[14]);

			return *this;
		}

		/*mat4 & mat4::translate(const vec3 & v)
		{
			return translate(v.x, v.y, v.z);
		}

		mat4 & mat4::translate(float x, float y, float z)
		{
			m[0] += m[3] * x; m[4] += m[7] * x; m[8] += m[11] * x; m[12] += m[15] * x;
			m[1] += m[3] * y; m[5] += m[7] * y; m[9] += m[11] * y; m[13] += m[15] * y;
			m[2] += m[3] * z; m[6] += m[7] * z; m[10] += m[11] * z; m[14] += m[15] * z;

			return *this;
		}*/

		mat4& mat4::rotate(float angle, const vec3& axis)
		{
			return rotate(angle, axis.x, axis.y, axis.z);
		}

		mat4& mat4::rotate(float angle, float x, float y, float z)
		{
			float c = cosf(angle * DEG2RAD);    // cosine
			float s = sinf(angle * DEG2RAD);    // sine
			float c1 = 1.0f - c;                // 1 - c
			float m0 = m[0], m4 = m[4], m8 = m[8], m12 = m[12],
				m1 = m[1], m5 = m[5], m9 = m[9], m13 = m[13],
				m2 = m[2], m6 = m[6], m10 = m[10], m14 = m[14];

			// build rotation matrix
			float r0 = x * x * c1 + c;
			float r1 = x * y * c1 + z * s;
			float r2 = x * z * c1 - y * s;
			float r4 = x * y * c1 - z * s;
			float r5 = y * y * c1 + c;
			float r6 = y * z * c1 + x * s;
			float r8 = x * z * c1 + y * s;
			float r9 = y * z * c1 - x * s;
			float r10 = z * z * c1 + c;

			// multiply rotation matrix
			m[0] = r0 * m0 + r4 * m1 + r8 * m2;
			m[1] = r1 * m0 + r5 * m1 + r9 * m2;
			m[2] = r2 * m0 + r6 * m1 + r10 * m2;
			m[4] = r0 * m4 + r4 * m5 + r8 * m6;
			m[5] = r1 * m4 + r5 * m5 + r9 * m6;
			m[6] = r2 * m4 + r6 * m5 + r10 * m6;
			m[8] = r0 * m8 + r4 * m9 + r8 * m10;
			m[9] = r1 * m8 + r5 * m9 + r9 * m10;
			m[10] = r2 * m8 + r6 * m9 + r10 * m10;
			m[12] = r0 * m12 + r4 * m13 + r8 * m14;
			m[13] = r1 * m12 + r5 * m13 + r9 * m14;
			m[14] = r2 * m12 + r6 * m13 + r10 * m14;

			return *this;
		}

		mat4 & mat4::rotateX(float angle)
		{
			float c = cosf(angle * DEG2RAD);
			float s = sinf(angle * DEG2RAD);
			float m1 = m[1], m2 = m[2],
				m5 = m[5], m6 = m[6],
				m9 = m[9], m10 = m[10],
				m13 = m[13], m14 = m[14];

			m[1] = m1 * c + m2 * -s;
			m[2] = m1 * s + m2 * c;
			m[5] = m5 * c + m6 * -s;
			m[6] = m5 * s + m6 * c;
			m[9] = m9 * c + m10 * -s;
			m[10] = m9 * s + m10 * c;
			m[13] = m13 * c + m14 * -s;
			m[14] = m13 * s + m14 * c;

			return *this;
		}

		mat4 & mat4::rotateY(float angle)
		{
			float c = cosf(angle * DEG2RAD);
			float s = sinf(angle * DEG2RAD);
			float m0 = m[0], m2 = m[2],
				m4 = m[4], m6 = m[6],
				m8 = m[8], m10 = m[10],
				m12 = m[12], m14 = m[14];

			m[0] = m0 * c + m2 * s;
			m[2] = m0 * -s + m2 * c;
			m[4] = m4 * c + m6 * s;
			m[6] = m4 * -s + m6 * c;
			m[8] = m8 * c + m10 * s;
			m[10] = m8 * -s + m10 * c;
			m[12] = m12 * c + m14 * s;
			m[14] = m12 * -s + m14 * c;

			return *this;
		}

		mat4 & mat4::rotateZ(float angle)
		{
			float c = cosf(angle * DEG2RAD);
			float s = sinf(angle * DEG2RAD);
			float m0 = m[0], m1 = m[1],
				m4 = m[4], m5 = m[5],
				m8 = m[8], m9 = m[9],
				m12 = m[12], m13 = m[13];

			m[0] = m0 * c + m1 * -s;
			m[1] = m0 * s + m1 * c;
			m[4] = m4 * c + m5 * -s;
			m[5] = m4 * s + m5 * c;
			m[8] = m8 * c + m9 * -s;
			m[9] = m8 * s + m9 * c;
			m[12] = m12 * c + m13 * -s;
			m[13] = m12 * s + m13 * c;

			return *this;
		}

		mat4 & mat4::lookAt(const vec3 & target)
		{
			return lookAt(target.x, target.y, target.z);
		}

		mat4 & mat4::lookAt(const vec3 & target, const vec3 & up)
		{
			return lookAt(target.x, target.y, target.z, up.x, up.y, up.z);
		}

		mat4 & mat4::lookAt(float x, float y, float z)
		{
			vec3 camPosition = vec3(m[12], m[13], m[14]);
			vec3 forward = camPosition - vec3(x, y, z);
			forward.normalize();

			vec3 up(0.0, 1.0f, 0.0f);

			vec3 right = up.cross(forward);
			up = forward.cross(right);

			this->setColumn(0, right);
			this->setColumn(1, up);
			this->setColumn(2, forward);

			return *this;
		}

		mat4 & mat4::lookAt(float x, float y, float z, float ux, float uy, float uz)
		{
			vec3 camPosition = vec3(m[12], m[13], m[14]);
			vec3 forward = camPosition - vec3(x, y, z);
			forward.normalize();

			vec3 up(ux, uy, uz);
			vec3 right = up.cross(forward);
			right.normalize();

			up = forward.cross(right);

			this->setColumn(0, right);
			this->setColumn(1, up);
			this->setColumn(2, forward);

			return *this;
		}

		mat4 & mat4::lookAt(const vec3 & eyePosition, const vec3 & targetPosition, const vec3 & up)
		{
			vec3 forward = eyePosition - targetPosition;
			forward.normalize();

			vec3 right = up.cross(forward);
			vec3 upUnit = forward.cross(right);

			this->setColumn(0, right);
			this->setColumn(1, upUnit);
			this->setColumn(2, forward);

			return *this;
		}

		mat4 mat4::orthographic(float left, float right, float bottom, float top, float near, float far)
		{
			mat4 result;

			result.m[0 + 0 * 4] = 2.0f / (right - left);

			result.m[1 + 1 * 4] = 2.0f / (top - bottom);

			result.m[2 + 2 * 4] = 2.0f / (near - far);

			result.m[0 + 3 * 4] = (left + right) / (left - right);
			result.m[1 + 3 * 4] = (bottom + top) / (bottom - top);
			result.m[2 + 3 * 4] = (far + near) / (far - near);

			return result;
		}

		mat4 mat4::perspective(float fov, float aspectRatio, float near, float far)
		{
			mat4 result;

			float q = 1.0f / tan(toRadians(0.5f * fov));
			float a = q / aspectRatio;

			float b = (near + far) / (near - far);
			float c = (2.0f * near * far) / (near - far);

			result.m[0 + 0 * 4] = a;
			result.m[1 + 1 * 4] = q;
			result.m[2 + 2 * 4] = b;
			result.m[3 + 2 * 4] = -1.0f;
			result.m[2 + 3 * 4] = c;

			return result;
		}

		mat4 mat4::translate(const vec3 & translation)
		{
			mat4 result;

			result.m[0 + 3 * 4] = translation.x;
			result.m[1 + 3 * 4] = translation.y;
			result.m[2 + 3 * 4] = translation.z;

			return result;
		}

		mat4 mat4::rotation(float angle, const vec3 & axis)
		{
			mat4 result;

			float r = toRadians(angle);
			float c = cos(r);
			float s = sin(r);
			float omc = 1.0f - c;

			float x = axis.x;
			float y = axis.y;
			float z = axis.z;

			result.m[0 + 0 * 4] = x * x * omc + c;
			result.m[1 + 0 * 4] = y * x * omc + z * s;
			result.m[2 + 0 * 4] = x * z * omc - y * s;

			result.m[0 + 1 * 4] = x * y * omc - z * s;
			result.m[1 + 1 * 4] = y * y * omc + c;
			result.m[2 + 1 * 4] = y * z * omc + x * s;

			result.m[0 + 2 * 4] = x * z * omc + y * s;
			result.m[1 + 2 * 4] = y * z * omc - x * s;
			result.m[2 + 2 * 4] = z * z * omc + c;

			return result;
		}

		mat4 mat4::scale(const vec3 & scale)
		{
			mat4 result;

			result.m[0 + 0 * 4] = scale.x;
			result.m[1 + 1 * 4] = scale.y;
			result.m[2 + 2 * 4] = scale.z;

			return result;
		}

	}
}