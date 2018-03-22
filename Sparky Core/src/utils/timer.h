#pragma once

#include <Windows.h>

namespace sparky {

	class Timer
	{
	private:
		LARGE_INTEGER m_Last;
		double m_Frequency;
	public:
		Timer()
		{
			LARGE_INTEGER frequency;
			QueryPerformanceFrequency(&frequency);
			m_Frequency = 1.0 / frequency.QuadPart;
		}

		float elapsed()
		{
			LARGE_INTEGER current;
			QueryPerformanceCounter(&current);
			unsigned __int64 cycles = current.QuadPart - m_Last.QuadPart;
			float elapsedTime = cycles * (float)m_Frequency;
			m_Last = current;

			return (float)(cycles * m_Frequency);
		}

		bool cyclicTimer(float threshold)
		{
			LARGE_INTEGER current;
			QueryPerformanceCounter(&current);
			unsigned __int64 cycles = current.QuadPart - m_Last.QuadPart;
			float elapsedTime = cycles * (float)m_Frequency;
			if (elapsedTime > threshold)
			{
				m_Last = current;
				return true;
			}

			return false;
		}
	};

}
