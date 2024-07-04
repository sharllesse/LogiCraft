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

Event::Event()
{
	m_listenerID = 0;
}

Event::~Event() {}

int Event::AddListener(std::function<void()> _func)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	m_listeners[m_listenerID] = _func;
	return m_listenerID++;
}

bool Event::RemoveListener(int _id)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	return m_listeners.erase(_id);
}

void Event::Invoke()
{
	std::lock_guard<std::mutex> lock(m_mutex);
	for (auto& func : m_listeners)
	{
		func.second();
	}
}

int EventSystem::AddAsyncListener(int eventID, std::function<void()> _func)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	return m_asyncListeners[eventID].AddListener(_func);
}

bool EventSystem::RemoveAsyncListener(int eventID, int _listenerID)
{
	std::lock_guard<std::mutex> lock(m_mutex);

	auto it = m_asyncListeners.find(eventID);
	if (it != m_asyncListeners.end())
	{
		return it->second.RemoveListener(_listenerID);
	}

	return false;
}

void EventSystem::ProcessEvents()
{
	while (!m_queuedEvents.empty())
	{
		int eventID;
		{
			std::lock_guard<std::mutex> lock(m_mutex);

			eventID = m_queuedEvents.front();
			m_queuedEvents.pop();
		}
		m_asyncListeners[eventID].Invoke();
	}
}

void EventSystem::QueueEvent(int eventID)
{
	std::lock_guard<std::mutex> lock(m_mutex);

	auto it = m_asyncListeners.find(eventID);
	if (it != m_asyncListeners.end())
	{
		m_queuedEvents.emplace(it->first);
	}
}