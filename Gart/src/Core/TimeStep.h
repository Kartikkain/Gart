#pragma once
namespace Gart
{
	class TimeStep
	{
	public:

		TimeStep(float p_time = 0.0f)
			: m_Time(p_time) { }

		operator float() { return m_Time; }

		float GetSeconds() const { return m_Time; }

		float GetMilliSeconds() const { return m_Time * 1000.0f; }

	private:
		float m_Time;
	};
}
