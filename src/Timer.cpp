#include "Timer.h"

bool Timer::Initialize()
{
	bool result;

	result = QueryPerformanceFrequency(&m_frequency);
	if (!result)
	{
		return false;
	}

	m_ticksPerMs = (m_frequency.QuadPart) / 1000.0f;

	QueryPerformanceCounter(&m_counter);
	m_counterStart = m_counter.QuadPart;

	return true;
}

void Timer::Frame()
{
	float frameDiff;

	QueryPerformanceCounter(&m_counter);

	double newElapsed = (double(m_counter.QuadPart) - m_counterStart) / m_ticksPerMs;

	frameDiff = (float)(newElapsed - m_currentTime);
	m_frameTime = frameDiff;

	m_currentTime = newElapsed;
	
	return;
}

