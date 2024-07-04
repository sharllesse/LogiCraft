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

// namespace Logicraft
//{
// class LOGI_ENGINE_API EventSystem
//{
// public:
//	EventSystem();
//	~EventSystem();
//
//	EventSystem(EventSystem&&)      = delete;
//	EventSystem(const EventSystem&) = delete;
//
//	EventSystem& operator=(EventSystem&&)      = delete;
//	EventSystem& operator=(const EventSystem&) = delete;
//
//	struct CallBack
//	{
//		void* listenerAdress;
//		int   eventID;
//	};
//
//	int  AddListener(int eventID, std::function<void()> _func);
//	bool RemoveListener(int eventID, int _listenerID);
//	void QueueEvent(int eventID);
//	void ProcessEvents();
//
// private:
//	std::unordered_map<int, Event> m_events;
//
//	std::queue<int> m_queueEvents;
//	std::mutex      m_mutex;
// };
// } // namespace Logicraft

#pragma once
#include "DLLExport.h"

#include <functional>
#include <iostream>
#include <mutex>
#include <queue>
#include <unordered_map>
#include <vector>

namespace Logicraft
{
class LOGI_ENGINE_API Event
{
public:
	Event();
	~Event();

	int  AddListener(std::function<void()> _func);
	bool RemoveListener(int _id);

	void Invoke();

private:
	std::unordered_map<int, std::function<void()>> m_listeners;

	std::mutex m_mutex;
	int        m_listenerID;
};
} // namespace Logicraft

class LOGI_ENGINE_API EventSystem
{
	struct CallBack
	{
		void* listenerAdress;
		int   eventID;
	};
	using CallBackPair = std::pair<CallBack, std::function<void(void*)>>;

public:
	EventSystem()  = default;
	~EventSystem() = default;

	EventSystem(EventSystem&&)                 = delete;
	EventSystem(const EventSystem&)            = delete;
	EventSystem& operator=(EventSystem&&)      = delete;
	EventSystem& operator=(const EventSystem&) = delete;

	int  AddAsyncListener(int eventID, std::function<void()> _func);
	bool RemoveAsyncListener(int eventID, int _listenerID);
	void QueueEvent(int eventID);
	void ProcessEvents();

	template<typename TEvent, typename TListener>
	void AddListener(TListener* listener, std::function<void(const TEvent&)>&& func)
	{
		CallBack cb = {(void*)listener, TEvent::ID};

		CallBackPair pair(std::move(cb), [func](void* pEventObject) { func(*(const TEvent*)pEventObject); });

		m_listeners.push_back(std::move(pair));
	}

	template<typename TEvent, typename TListener>
	void RemoveListener(TListener* pListener)
	{
		m_listeners.erase(std::remove_if(m_listeners.begin(),
		                    m_listeners.end(),
		                    [pListener](CallBackPair& pair) {
			                    bool tmp = (pair.first.eventID == TEvent::ID && pair.first.listenerAdress == pListener);
			                    std::cout << tmp;
			                    return tmp;
		                    }),
		  m_listeners.end());
	}

	template<typename TEvent>
	void SendEvent(const TEvent& typeEvent)
	{
		for (auto& cb : m_listeners)
		{
			if (cb.first.eventID == TEvent::ID)
			{
				cb.second((void*)&typeEvent);
			}
		}
	}

private:
	std::vector<CallBackPair> m_listeners;

	std::unordered_map<int, Event> m_asyncListeners; // TODO avec l'adresse du listener
	std::queue<int>                m_queuedEvents;
	std::mutex                     m_mutex;
};
} // namespace Logicraft
