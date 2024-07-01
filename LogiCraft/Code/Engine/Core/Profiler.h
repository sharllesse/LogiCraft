#pragma once
#include <chrono>

#include "DLLExport.h"

#define PROFILE_SCOPE(name) Profiler name(#name)
#define PROFILE_FUNCTION()  PROFILE_SCOPE(__FUNCTION__)

using TimePoint = std::chrono::time_point<std::chrono::high_resolution_clock>;

class LOGI_ENGINE_API Profiler
{
public:
	Profiler(const Profiler&) = delete;
	Profiler(Profiler&&)      = delete;

	Profiler() = default;
	explicit Profiler(const char* name);
	~Profiler();

	Profiler& operator=(const Profiler&) = delete;
	Profiler& operator=(Profiler&&)      = delete;

private:
	const char* m_name{"No Name"};

	TimePoint m_start{std::chrono::high_resolution_clock::now()};
	TimePoint m_end;
};
