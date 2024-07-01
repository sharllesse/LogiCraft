#include "Profiler.h"

#include <iostream>

using namespace std::chrono;

Profiler::Profiler(const char* name)
	: m_name(name)
{
	m_start = high_resolution_clock::now();
}

Profiler::~Profiler()
{
	m_end = high_resolution_clock::now();
	long long duration = duration_cast<microseconds>(m_end - m_start).count();

	std::cout << m_name << " took : " << duration << "ms" << '\n';
}
