#pragma once
#include <SFML/System/Time.hpp>
#include <SFML/System/Clock.hpp>

#include "DLLExport.h"

#define PROFILE_SCOPE(name) Logicraft::Profiler __profiler(name)
#define PROFILE_FUNCTION    Logicraft::Profiler __functionProfiler(__FUNCTION__);

namespace Logicraft
{
class LOGI_ENGINE_API Profiler
{
public:
	explicit Profiler(const char* name);
	~Profiler();

	void Start();
	void Pause();
	void Resume();

private:
	const char* m_name;
	sf::Time    m_timer;
	sf::Clock   m_clock;
	Profiler*   m_pParent{nullptr};

	inline static Profiler* s_pLastProfiler{nullptr};
};
} // namespace Logicraft
