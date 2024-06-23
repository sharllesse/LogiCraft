/*------------------------------------LICENSE------------------------------------
MIT License

Copyright (c) 2024 CIRON Robin
Copyright (c) 2024 GRALLAN Yann
Copyright (c) 2024 LESAGE Charles
Copyright (c) 2024 MENA-BOUR Samy

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

#include "TaskManager.h"

#include <assert.h>

using namespace Logicraft;

TaskManager* s_pTaskManager = nullptr;

TaskManager& TaskManager::Get()
{
	assert(s_pTaskManager);
	return *s_pTaskManager;
}

TaskManager::TaskManager()
{
	for (int i = 0; i < NumThreads; ++i)
	{
		m_workers.emplace_back([this] {
			while (true)
			{
				std::function<void()> task;

				{
					std::unique_lock<std::mutex> lock(m_queueMutex);

					// Wait for a task if the queue is empty and the stop flag is not set
					m_condition.wait(lock, [this] { return m_stop || !m_taskQueue.empty(); });

					if (m_stop)
						return;

					// Get the task from the front of the queue
					task = std::move(m_taskQueue.front());
					m_taskQueue.pop();
				}

				// Execute the task
				task();
			}
		});
	}

	assert(!s_pTaskManager);
	s_pTaskManager = this;
}

TaskManager::~TaskManager()
{
	s_pTaskManager = nullptr;
	EndAllTasks();
}

void TaskManager::EndAllTasks()
{
	{
		std::unique_lock<std::mutex> lock(m_queueMutex);
		m_stop = true;
	}

	// Notify all threads to wake up and exit
	m_condition.notify_all();

	// Join all threads
	for (std::thread& worker : m_workers)
	{
		worker.join();
	}
}
