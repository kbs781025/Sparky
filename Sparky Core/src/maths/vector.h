#pragma once

#include <array>

namespace sparky { namespace maths {

	template<typename T, unsigned int size> 
	struct Vector
	{
		std::array<T, size> data;

		T()
	};

} }