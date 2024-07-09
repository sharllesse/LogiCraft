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

void Event::Invoke()
{
	std::shared_lock<std::shared_mutex> lock(m_mutex);
	for (auto& func : m_callbacks)
	{
		func.second();
	}
}

void EventSystem::ProcessEvents()
{
	std::lock_guard<std::shared_mutex> lock(m_mutexQueuedEvents);
	while (!m_queuedEvents.empty())
	{
		int eventID;
		{
			eventID = m_queuedEvents.front();
			m_queuedEvents.erase(m_queuedEvents.begin());
		}

		std::shared_lock<std::shared_mutex> lock(m_mutexQueuedEventsCallbacks);
		m_queuedEventsCallbacks[eventID].Invoke();
	}
}

void EventSystem::QueueEvent(int eventID)
{
	std::shared_lock<std::shared_mutex> lock(m_mutexQueuedEventsCallbacks);

	auto it = m_queuedEventsCallbacks.find(eventID);
	if (it != m_queuedEventsCallbacks.end())
	{
		for (int i : m_queuedEvents)
		{
			if (i == eventID)
				return;
		}

		std::lock_guard<std::shared_mutex> lock(m_mutexQueuedEvents);
		m_queuedEvents.push_back(it->first);
	}
}