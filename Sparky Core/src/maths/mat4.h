#pragma once


#include <math.h>
#include <iostream>
#include "maths.h"

namespace sparky { namespace maths {

	/*struct mat4
	{
		union
		{
			float elements[4 * 4];
			vec4 colums[4];
		};

		mat4();
		mat4(float diagonal);

		mat4& multiply(const mat4& other);
		friend mat4 operator*(mat4 left, const mat4& right);
		mat4& operator*=(const mat4& other);

		vec3 multiply(const vec3& other) const;
		friend vec3 operator*(const mat4& left, const vec3& right);
		
		vec4 multiply(const vec4& other) const;
		friend vec4 operator*(const mat4& left, const vec4& right);

		static mat4 identity();
		
		static mat4 orthographic(float left, float right, float bottom, float top, float near, float far);
		static mat4 perspective(float fov, float aspectRatio, float near, float far);

		static mat4 translate(const vec3& translation);
		static mat4 rotation(float angle, const vec3& axis);
		static mat4 scale(const vec3& scale);
	};*/

	struct mat4
	{
		union
		{
			float m[16];
			float tm[16];
			vec4 column[4];
		};

		mat4(); // Identity Matrix
		mat4(const float src[16]);
		mat4(float m00, float m01, float m02, float m03,  // 1st column
			 float m04, float m05, float m06, float m07,
			 float m08, float m09, float m10, float m11,
		     float m12, float m13, float m14, float m15);

		// setters
		void set(const float src[16]);
		void set(float m00, float m01, float m02, float m03,
				 float m04, float m05, float m06, float m07,
			 	 float m08, float m09, float m10, float m11,
			 	 float m12, float m13, float m14, float m15);
		void setRow(int row, const float src[4]);
		void setRow(int row, const vec4& v);
		void setColumn(int column, const float src[4]);
		void setColumn(int column, const vec4& v);

		mat4& identity();

		// getter
		const float* getElements() const;
		const float* getTransposeElements() const;
		vec3 getAngle() const; // get each angle of Rx * Ry * Rz rotation matrix
		
		// transform functions
		mat4 translate(const vec3& v); // return reference? value?
		mat4 translate(float x, float y, float z);

		mat4 rotate(float angle, const vec3& axis);
		mat4 rotate(float angle, float x, float y, float z);
		mat4 rotateX(float angle);
		mat4 rotateY(float angle);
		mat4 rotateZ(float angle);

		mat4 lookAt(const vec3& target);
		mat4 lookAt(float x, float y, float z);
		mat4 lookAt(float x, float y, float z, float ux, float uy, float uz);

		mat4 scale(float sx, float sy, float sz);
		mat4 scale(float scale);

		mat4 invert();
		mat4 invertEuclidean();
		mat4 invertAffine();
		mat4 invertProjective();
		mat4 invertGeneral();
		

		// operators
		mat4 operator+(const mat4& rhs) const;
		mat4& operator+=(const mat4& rhs);
		mat4 operator-(const math4& rhs) const;
		mat4& operator-=(const mat4& rhs);
		vec4 operator*(const mat4& rhs) const;
		vec4& operator*=(const mat4& rhs);

		bool operator==(const mat4& rhs) const;
		bool operator!=(const mat4& rhs) const;

		float operator[](int index) const;
		float& operator[](int index);


		// friend methods
		friend mat4 operator-(const mat4& m);
		friend mat4 operator*(float scalar, const mat4& m);
		friend mat4 operator*(const vec4& v, const mat4& m);
		friend std::ostream& operator<<(std::ostream& os, const mat4& rhs);

	};

	// inline functions
	inline mat4::mat4()
	{
		identity();
	}
	
	inline mat4::mat4(const float src[16])
	{
		set(src);
	}

	inline mat4::mat4(float m00, float m01, float m02, float m03,  // 1st column
				      float m04, float m05, float m06, float m07,
				      float m08, float m09, float m10, float m11,
				      float m12, float m13, float m14, float m15)
	{
		set(m00, m01, m02, m03, m04, m05, m06, m07, m08, m09, m10, m11, m12, m13, m14, m15);
	}

	inline void mat4::set(const float src[16])
	{
		m[0] = src[0]; m[1] = src[1]; m[2] = src[2]; m[3] = src[3];
		m[4] = src[4]; m[5] = src[5]; m[6] = src[6]; m[7] = src[7];
		m[8] = src[8]; m[9] = src[9]; m[10] = src[10]; m[11] = src[11];
		m[12] = src[12]; m[13] = src[13]; m[14] = src[14]; m[15] = src[15];
	}

	inline void mat4::set(float m00, float m01, float m02, float m03,
				     float m04, float m05, float m06, float m07,
				     float m08, float m09, float m10, float m11,
				     float m12, float m13, float m14, float m15)
	{
		m[0] = m00; m[1] = m01; m[2] = m02; m[3] = m03;
		m[4] = m04; m[5] = m05; m[6] = m06; m[7] = m07;
		m[8] = m08; m[9] = m09; m[10] = m10; m[11] = m11;
		m[12] = m12; m[13] = m13; m[14] = m14; m[15] = m15;
	}

	inline void mat4::setRow(int row, const float src[4])
	{
		m[row] = src[0]; 
		m[row + 4] = src[1];
		m[row + 8] = src[2];
		m[row + 12] = src[3];
	}

	inline void mat4::setRow(int row, const vec4& v)
	{
		m[row] = v.x;
		m[row + 4] = v.y;
		m[row + 8] = v.z;
		m[row + 12] = v.w;
	}

	inline void mat4::setColumn(int column, const float src[4])
	{
		m[column * 4] = src[0];
		m[column * 4 + 1] = src[1];
		m[column * 4 + 2] = src[2];
		m[column * 4 + 3] = src[3];
	}

	inline void mat4::setColumn(int column, const vec4& v)
	{
		m[column * 4] = v.x;
		m[column * 4 + 1] = v.y;
		m[column * 4 + 2] = v.z;
		m[column * 4 + 3] = v.w;
	}

	inline mat4& mat4::identity()
	{
		m[0] = m[5] = m[10] = m[15] = 1.0f;
		m[1] = m[2] = m[3] = m[4] = m[6] = m[7] = m[8] = m[9] = m[11] = m[12] = m[13] = m[14] = 0.0f;
		return *this;
	}
	
	inline const float* mat4::getElements() const
	{
		return m;
	}

	inline const float* mat4::getTransposeElements() const
	{
		tm[0] = m[0]; tm[1] = m[4]; tm[2] = m[8]; tm[3] = m[12];
		tm[4] = m[1]; tm[5] = m[5]; tm[6] = m[9]; tm[7] = m[13];
		tm[8] = m[2]; tm[9] = m[6]; tm[10] = m[10]; tm[11] = m[14];
		tm[12] = m[3]; tm[13] = m[7]; tm[14] = m[11]; tm[15] = m[15];

		return tm;
	}

	inline mat4 mat4::translate(const vec3& v)
	{
		m[12] = v.x;
		m[13] = v.y;
		m[14] = v.z;
		
		return *this;
	}

	inline mat4 mat4::translate(float x, float y, float z)
	{
		m[12] = x;
		m[13] = y;
		m[14] = z;

		return *this;
	}



} }