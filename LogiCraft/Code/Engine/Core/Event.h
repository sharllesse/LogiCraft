/*------------------------------------LICENSE------------------------------------
MIT License

Copyright (c) 2024 CIRON Robin
Copyright (c) 2024 GRALLAN Yann
Copyright (c) 2024 LESAGE Charles
Copyright (c) 2024 MENA-BOUR Samy
Copyright (c) 2024 TORRES Theo

This software utilizes code from the following GitHub repositories, which are also licensed under the MIT License:

- [SFML](https://github.com/SFML)
- [ImGUI](https://github.com/ocornut/imgui)
- [ImNodes](https://github.com/Nelarius/imnodes)
- [SFML-Manager](https://github.com/Xanhos/Cpp-Manager-for-SFML)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
---------------------------------------------------------------------------------*/

#pragma once
#include "define.h"
#include <functional>
#include <iostream>
#include <vector>

template<typename T>
class Event;

template<typename R, typename... Args>
class Event<R(Args...)>
{
public:
	using FunctionType = R (*)(Args...);

	Event() = default;

	void AddEvent(FunctionType event) { m_events.push_back(event); }

	auto Invoke(Args... args)
	{
		if constexpr (std::is_void_v<R>)
		{
			for (auto& func : m_events)
			{
				func(args...);
			}
		}
		else
		{
			std::vector<R> results;
			for (auto& func : m_events)
			{
				results.push_back(func(args...));
			}
			return results;
		}
	}

	void RemoveEvent(FunctionType event)
	{
		auto it = std::remove(m_events.begin(), m_events.end(), event);
		m_events.erase(it, m_events.end());
	}

private:
	std::vector<FunctionType> m_events;
};
