#include "Profiler.h"
#include "Logger.h"

#include <string>

namespace Logicraft
{
Profiler::Profiler(const char* name)
  : m_name(name)
  , m_pParent(s_pLastProfiler)
{
	s_pLastProfiler = this;
	Start();
}

Profiler::~Profiler()
{
	if (m_pParent)
		m_pParent->Pause();

	m_timer += m_clock.getElapsedTime();

	Logger::Get().Log(Logger::eInfo, std::string(m_name) + " took : " + std::to_string(m_timer.asMicroseconds()) + "us");

	if (m_pParent)
		m_pParent->Resume();

	s_pLastProfiler = m_pParent;
}

void Profiler::Pause()
{
	m_timer += m_clock.getElapsedTime();
	if (m_pParent)
		m_pParent->Pause();
}

void Profiler::Resume()
{
	m_clock.restart();
	if (m_pParent)
		m_pParent->Resume();
}

void Profiler::Start()
{
	m_clock.restart();
	m_timer = sf::Time::Zero;
}
} // namespace Logicraft
