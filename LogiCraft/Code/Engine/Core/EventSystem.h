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
#include <unordered_map>
#include <vector>

namespace Logicraft
{
class LOGI_ENGINE_API Event
{
	using CallBackPair = std::pair<void*, std::function<void()>>;

public:
	Event()  = default;
	~Event() = default;

	Event(Event&&)                 = delete;
	Event(const Event&)            = delete;
	Event& operator=(Event&&)      = delete;
	Event& operator=(const Event&) = delete;

	// To add a call back in the event using the adress of the call back owner and the void() function
	template<typename TCallBackOwner>
	void AddCallBack(TCallBackOwner* pCallBackOwner, std::function<void()> func)
	{
		CallBackPair cb = {(void*)pCallBackOwner, func};
		m_callBacks.push_back(std::move(cb));
	}

	// To remove a call back in the event using the adress of the call back owner
	template<typename TCallBackOwner>
	void RemoveCallBack(TCallBackOwner* pCallBackOwner)
	{
		m_callBacks.erase(
		  std::remove_if(m_callBacks.begin(), m_callBacks.end(), [pCallBackOwner](CallBackPair& pair) { return (pair.first == pCallBackOwner); }),
		  m_callBacks.end());
	}
	// To invoke all call backs in this event
	void Invoke();

private:
	std::vector<CallBackPair> m_callBacks;
	std::mutex                m_mutex;
};

////////////////////////////////////////////////////////////////////////////////
// To use to create an unique id for call back in class
#define GET_UNIQUE_CALL_BACK_ID inline static int ID = EventSystem::s_callBackID++;

class LOGI_ENGINE_API EventSystem
{
	struct CallBack
	{
		void* callBackOwnerAdress;
		int   eventID; // Unique ID for each Event
	};
	using CallBackPair = std::pair<CallBack, std::function<void(void*)>>;

public:
	inline static int s_callBackID = 0;

	EventSystem()  = default;
	~EventSystem() = default;

	EventSystem(EventSystem&&)                 = delete;
	EventSystem(const EventSystem&)            = delete;
	EventSystem& operator=(EventSystem&&)      = delete;
	EventSystem& operator=(const EventSystem&) = delete;

	// To add a call back in the event using the adress of the call back owner, the enum of the event and a void() function
	template<typename TCallBackOwner>
	void AddQueuedEventCallback(TCallBackOwner* pCallBackOwner, int eventID, std::function<void()> func)
	{
		m_queuedCallBacks[eventID].AddCallBack(pCallBackOwner, func);
	}

	// To remove a call back in the event using the adress of the call back owner and the enum of the event
	template<typename TCallBackOwner>
	void RemoveQueuedEventCallBack(TCallBackOwner* pCallBackOwner, int eventID)
	{
		auto it = m_queuedCallBacks.find(eventID);
		if (it != m_queuedCallBacks.end())
		{
			m_queuedCallBacks[eventID].RemoveCallBack(pCallBackOwner);
		}
	}

	// To put an event in the queue to be proccess
	void QueueEvent(int eventID);

	// Proccess events wich there are in queue
	void ProcessEvents();

	// To add a call back in the event system using the adress of the call back owner and a function with args
	template<typename TEvent, typename TCallBackOwner>
	void AddCallBack(TCallBackOwner* pCallBackOwner, std::function<void(const TEvent&)>&& func)
	{
		CallBack cb = {(void*)pCallBackOwner, TEvent::ID};

		CallBackPair pair(std::move(cb), [func](void* pEventObject) { func(*(const TEvent*)pEventObject); });

		m_callBacks.push_back(std::move(pair));
	}

	// To remove a call back in the event system using the adress of the call back owner
	template<typename TEvent, typename TCallBackOwner>
	void RemoveCallBack(TCallBackOwner* pCallBackOwner)
	{
		m_callBacks.erase(
		  std::remove_if(m_callBacks.begin(),
		    m_callBacks.end(),
		    [pCallBackOwner](CallBackPair& pair) { return (pair.first.eventID == TEvent::ID && pair.first.callBackOwnerAdress == pCallBackOwner); }),
		  m_callBacks.end());
	}

	// To call an event
	template<typename TEvent>
	void SendEvent(const TEvent& typeEvent)
	{
		for (auto& cb : m_callBacks)
		{
			if (cb.first.eventID == TEvent::ID)
			{
				cb.second((void*)&typeEvent);
			}
		}
	}

private:
	std::vector<CallBackPair> m_callBacks;

	std::unordered_map<int, Event> m_queuedCallBacks;
	std::vector<int>               m_queuedEvents;
	std::mutex                     m_mutex;
};
} // namespace Logicraft