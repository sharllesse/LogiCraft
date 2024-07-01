#pragma once
#include <chrono>

#include "DLLExport.h"

#include "TaskManager.h"

#define PROFILE_SCOPE(name) Logicraft::Profiler __profiler(name)
#define PROFILE_FUNCTION    Logicraft::Profiler __functionProfiler(__FUNCTION__);

using TimePoint = std::chrono::time_point<std::chrono::high_resolution_clock>;

namespace Logicraft
{
class LOGI_ENGINE_API Timer
{
public:
	Timer() = default;

	void Start();
	void Pause();

	long long GetElapsedTime() const;

private:
	TimePoint m_start;
	long long m_duration{0};
	bool      m_running{false};
};

class LOGI_ENGINE_API Profiler
{
public:
	Profiler() = default;
	explicit Profiler(const char* name);
	~Profiler();

	void Pause();
	void Start();

private:
	const char* m_name{"No Name"};

	Timer m_timer;

	Profiler* m_parent{nullptr};

	inline static Profiler* s_pRunningProfiler{nullptr};
};
} // namespace Logicraft
