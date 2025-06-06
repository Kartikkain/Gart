#pragma once

#include<chrono>

namespace Gart 
{
	class Timer
	{
	public:
		Timer()
		{

		}

		void Timer::Reset()
		{
			m_startTime = std::chrono::high_resolution_clock::now();
		}

		float Timer::Elapsed()
		{
			return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - m_startTime).count() * 0.001f * 0.001f * 0.001f;
		}

		float Timer::ElapsedMillis()
		{
			return Elapsed() * 1000.0f;
		}
	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_startTime;
	};
}