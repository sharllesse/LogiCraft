/*------------------------------------LICENSE------------------------------------
MIT License

Copyright (c) [2024] [CIRON Robin]

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
#include <define.h>

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <utility>

class LOGI_ENGINE_API TaskManager
{
public:
	static const int NumThreads = 32;

	static TaskManager& Get();

	TaskManager();
	~TaskManager();

	void EndAllTasks();

	template<typename Function>
	void AddTask(Function&& func)
	{
		std::function<void()> task = std::bind(std::forward<Function>(func));

		{
			std::unique_lock<std::mutex> lock(m_queueMutex);
			m_taskQueue.emplace(std::move(task));
		}

		// Notify one waiting thread to wake up and process the task
		m_condition.notify_one();
	}

private:
	std::vector<std::thread>          m_workers;
	std::queue<std::function<void()>> m_taskQueue;
	std::mutex                        m_queueMutex;
	std::condition_variable           m_condition;

	bool m_stop;
};
