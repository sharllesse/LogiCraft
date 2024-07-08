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
#include "DLLExport.h"

#include <functional>
#include <iostream>
#include <mutex>
#include <queue>
#include <shared_mutex>
#include <unordered_map>
#include <utility>
#include <vector>
namespace Logicraft
{
class Event
{
	using TCallBackPair = std::pair<void*, std::function<void()>>;

public:
	Event()  = default;
	~Event() = default;

	Event(Event&&)                 = delete;
	Event(const Event&)            = delete;
	Event& operator=(Event&&)      = delete;
	Event& operator=(const Event&) = delete;

	template<typename TCallBackOwner>
	void AddCallback(TCallBackOwner* pCallBackOwner, std::function<void()> func)
	{
		std::lock_guard<std::shared_mutex> lock(m_mutex);

		TCallBackPair cb = {(void*)pCallBackOwner, func};
		m_callbacks.push_back(std::move(cb));
	}

	template<typename TCallBackOwner>
	void RemoveCallback(TCallBackOwner* pCallBackOwner)
	{
		std::lock_guard<std::shared_mutex> lock(m_mutex);

		m_callbacks.erase(
		  std::remove_if(m_callbacks.begin(), m_callbacks.end(), [pCallBackOwner](TCallBackPair& pair) { return (pair.first == pCallBackOwner); }),
		  m_callbacks.end());
	}
	void Invoke();

private:
	std::vector<TCallBackPair> m_callbacks;
	std::shared_mutex          m_mutex;
};

#define UNIQUE_EVENT_ID inline static int ID = EventSystem::s_callBackID++;
// How to declare an unique ID for each callbacks with args:
//	class NewEvent
//	{
// 	public:
//		UNIQUE_EVENT_ID
// 		...
//	};

class LOGI_ENGINE_API EventSystem
{
	struct Callback
	{
		void* pOwnerAdress;
		int   eventID; // Unique ID for each Event
	};
	using TCallBackPair = std::pair<Callback, std::function<void(void*)>>;

public:
	inline static int s_callBackID = 0;

	EventSystem()  = default;
	~EventSystem() = default;

	EventSystem(EventSystem&&)                 = delete;
	EventSystem(const EventSystem&)            = delete;
	EventSystem& operator=(EventSystem&&)      = delete;
	EventSystem& operator=(const EventSystem&) = delete;

	template<typename TCallBackOwner>
	void AddQueuedEventCallback(TCallBackOwner* pCallBackOwner, int eventID, std::function<void()> func)
	{
		std::lock_guard<std::shared_mutex> lock(m_mutexQueuedEventsCallbacks);

		m_queuedEventsCallbacks[eventID].AddCallback(pCallBackOwner, func);
	}

	template<typename TCallBackOwner>
	void RemoveQueuedEventCallback(TCallBackOwner* pCallBackOwner, int eventID)
	{
		std::lock_guard<std::shared_mutex> lock(m_mutexQueuedEventsCallbacks);

		auto it = m_queuedEventsCallbacks.find(eventID);
		if (it != m_queuedEventsCallbacks.end())
		{
			m_queuedEventsCallbacks[eventID].RemoveCallback(pCallBackOwner);
		}
	}

	// Push an event to the queue that will be processed later
	void QueueEvent(int eventID);

	// Process events in queue
	void ProcessEvents();

	// To add a call back in the event system using the adress of the call back owner and a function with args
	template<typename TEvent, typename TCallBackOwner>
	void AddCallback(TCallBackOwner* pCallBackOwner, std::function<void(const TEvent&)>&& func)
	{
		std::lock_guard<std::shared_mutex> lock(m_mutexEventsCallback);

		Callback cb = {(void*)pCallBackOwner, TEvent::ID};

		TCallBackPair pair(std::move(cb), [func](void* pEventObject) { func(*(const TEvent*)pEventObject); });

		m_eventsCallback.push_back(std::move(pair));
	}

	// To remove a call back in the event system using the adress of the call back owner
	template<typename TEvent, typename TCallBackOwner>
	void RemoveCallback(TCallBackOwner* pCallBackOwner)
	{
		std::lock_guard<std::shared_mutex> lock(m_mutexEventsCallback);

		m_eventsCallback.erase(
		  std::remove_if(m_eventsCallback.begin(),
		    m_eventsCallback.end(),
		    [pCallBackOwner](TCallBackPair& pair) { return (pair.first.eventID == TEvent::ID && pair.first.pOwnerAdress == pCallBackOwner); }),
		  m_eventsCallback.end());
	}

	// To call an event
	template<typename TEvent>
	void SendEvent(const TEvent& typeEvent)
	{
		std::lock_guard<std::shared_mutex> lock(m_mutexEventsCallback);

		for (auto& cb : m_eventsCallback)
		{
			if (cb.first.eventID == TEvent::ID)
			{
				cb.second((void*)&typeEvent);
			}
		}
	}

private:
	std::vector<TCallBackPair>     m_eventsCallback;
	std::unordered_map<int, Event> m_queuedEventsCallbacks;
	std::queue<int>                m_queuedEvents;

	std::shared_mutex m_mutexEventsCallback;
	std::shared_mutex m_mutexQueuedEventsCallbacks;
};
} // namespace Logicraft