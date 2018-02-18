#pragma once

#include <Windows.h>

namespace sparky {

	class Timer
	{
	private:
		LARGE_INTEGER  m_Start;
		double m_Frequency;
	public:
		Timer()
		{
			LARGE_INTEGER frequency;
			QueryPerformanceFrequency(&frequency);
			m_Frequency = 1.0 / frequency.QuadPart;

			reset();
		}

		void reset() // reset m_Start
		{
			QueryPerformanceCounter(&m_Start);
		}

		float elapsed()
		{
			LARGE_INTEGER current;
			QueryPerformanceCounter(&current);
			unsigned __int64 cycles = current.QuadPart - m_Start.QuadPart;

			return (float)(cycles * m_Frequency);
		}
	};

}
