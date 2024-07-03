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

#include "EventSystem.h"

using namespace Logicraft;

EventSystem::EventSystem() {}

EventSystem::~EventSystem() {}

int EventSystem::AddListener(int eventID, std::function<void()> _func)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	return m_events[eventID].AddListener(_func);
}

bool EventSystem::RemoveListener(int eventID, int _listenerID)
{
	std::lock_guard<std::mutex> lock(m_mutex);

	auto it = m_events.find(eventID);
	if (it != m_events.end())
	{
		return it->second.RemoveListener(_listenerID);
	}

	return false;
}

void EventSystem::ProcessEvents()
{
	while (!m_queueEvents.empty())
	{
		int eventID;
		{
			std::lock_guard<std::mutex> lock(m_mutex);

			eventID = m_queueEvents.front();
			m_queueEvents.pop();
		}
		m_events[eventID].Invoke();
	}
}

void EventSystem::QueueEvent(int eventID)
{
	std::lock_guard<std::mutex> lock(m_mutex);

	auto it = m_events.find(eventID);
	if (it != m_events.end())
	{
		m_queueEvents.emplace(it->first);
	}
}