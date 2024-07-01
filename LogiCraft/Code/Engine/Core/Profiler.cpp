#include "Profiler.h"
#include "Logger.h"

#include <iostream>

using namespace std::chrono;

namespace Logicraft
{
void Timer::Start()
{
	m_running = true;
	m_start   = high_resolution_clock::now();
}

void Timer::Pause()
{
	m_running = false;
	m_duration += duration_cast<microseconds>(high_resolution_clock::now() - m_start).count();
}

long long Timer::GetElapsedTime() const
{
	if (m_running)
		return m_duration + duration_cast<microseconds>(high_resolution_clock::now() - m_start).count();

	return m_duration;
}

Profiler::Profiler(const char* name)
  : m_name(name)
  , m_parent(s_pRunningProfiler)
{
	s_pRunningProfiler = this;
	m_timer.Start();
}

Profiler::~Profiler()
{
	Pause();
	long long duration = m_timer.GetElapsedTime();

	Logger::Get().Log(Logger::eInfo, std::string(m_name) + " took : " + std::to_string(duration) + "us");

	Start();
	s_pRunningProfiler = m_parent;
}

void Profiler::Pause()
{
	m_timer.Pause();
	if (m_parent)
		m_parent->Pause();
}

void Profiler::Start()
{
	m_timer.Start();
	if (m_parent)
		m_parent->Start();
}
} // namespace Logicraft
