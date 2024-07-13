
#include "Engine/Core/EventSystem.h"
#include "Engine/Core/TaskManager.h"
#include "Objects/GameObjectManager.h"

#include <SFML/System/Clock.hpp>
#include <assert.h>
#include <cstdlib>
#include <ctime>
#include <shared_mutex>
#include <vector>

namespace Logicraft
{
class ObjectsManagerTests
{
public:
	ObjectsManagerTests(sf::Int32 timeOut)
	  : timeOut(timeOut)
	{
	}

	void Run()
	{
		Logger::Get().Log(Logger::eInfo, "Begin ObjectsManager test");

		ActionManager::Get().ExecuteAction("GameObjectManager_disable_log");
		Logicraft::Memory::Report();

		const char* cpntType = GameComponent::GetRegisteredTypes()[0]->GetName().c_str();

		// Create objects
		for (int i = 0; i < 100; i++)
		{
			++m_runningTasks;
			TaskManager::Get().AddTask([this]() {
				for (int j = 0; j < 100; j++)
				{
					GameObjectPtr pObject = GameObjectManager::Get().CreateObject();

					std::lock_guard<std::shared_mutex> lock(m_objectsMutex);
					m_objects.push_back(pObject);
				}
				--m_runningTasks;
			});
		}

		// Create components
		for (int i = 0; i < 100; i++)
		{
			++m_runningTasks;
			TaskManager::Get().AddTask([this, cpntType]() {
				for (int j = 0; j < 100; j++)
				{
					GameComponentPtr pCpnt   = GameObjectManager::Get().CreateComponent(cpntType);
					GameObjectPtr    pObject = GetRandomObject();
					pObject->AddComponent(pCpnt);

					std::lock_guard<std::shared_mutex> lock(m_componentsMutex);
					m_components.push_back(pCpnt);
				}
				--m_runningTasks;
			});
		}

		// Destroy components
		for (int i = 0; i < 100; i++)
		{
			++m_runningTasks;
			TaskManager::Get().AddTask([this]() {
				for (int j = 0; j < 100; j++)
				{
					GameComponentPtr pCpnt = GetRandomComponent();
					{
						std::lock_guard<std::shared_mutex> lock(m_componentsMutex);
						std::erase(m_components, pCpnt);
					}
					if (pCpnt->GetObject())
					{
						GameObjectManager::Get().RemoveComponent(pCpnt->GetGUID());
					}
					else
					{
						// if the component has no game object, it should have been released by its object and not be in the list anymore
						assert(GameObjectManager::Get().GetComponent(pCpnt->GetGUID()) == nullptr);
					}
				}
				--m_runningTasks;
			});
		}

		// Destroy objects
		for (int i = 0; i < 100; i++)
		{
			++m_runningTasks;
			TaskManager::Get().AddTask([this]() {
				for (int j = 0; j < 100; j++)
				{
					GameObjectPtr pObject = GetRandomObject();
					{
						std::lock_guard<std::shared_mutex> lock(m_objectsMutex);
						std::erase(m_objects, pObject);
					}
					GameObjectManager::Get().RemoveObject(pObject->GetGUID());
				}
				--m_runningTasks;
			});
		}

		Stop();
		Logicraft::Memory::Report();
		Logger::Get().Log(Logger::eInfo, "End ObjectsManager test");
	}

	GameObjectPtr GetRandomObject()
	{
		std::shared_lock<std::shared_mutex> lock(m_objectsMutex);
		while (m_objects.empty() && !m_stop) {}
		int objectToRemove = std::rand() % m_objects.size();
		return m_objects[objectToRemove];
	}

	GameComponentPtr GetRandomComponent()
	{
		std::shared_lock<std::shared_mutex> lock(m_componentsMutex);
		while (m_components.empty() && !m_stop) {}
		int componentToRemove = std::rand() % m_components.size();
		return m_components[componentToRemove];
	}

	void Stop()
	{
		while (m_runningTasks > 0 && m_clock.getElapsedTime().asMilliseconds() < timeOut) {}
		m_stop = true; // avoid infinite loops but if they need to be stopped there is a problem
		assert(m_runningTasks == 0 && m_objects.empty() && m_components.empty());
	}

private:
	std::atomic<int>              m_runningTasks{0};
	std::shared_mutex             m_objectsMutex;
	std::vector<GameObjectPtr>    m_objects;
	std::shared_mutex             m_componentsMutex;
	std::vector<GameComponentPtr> m_components;

	sf::Int32 timeOut;
	sf::Clock m_clock;
	bool      m_stop = false;
};

class UnitTest
{
public:
	UnitTest()  = default;
	~UnitTest() = default;

	void Run()
	{
		ObjectsManagerTests(9999).Run();

		// EventSystem
		{
			std::srand((unsigned int)std::time(NULL));

			for (int i = 0; i < 100; i++)
			{
				TaskManager::Get().AddTask([this]() {
					for (int j = 0; j < 100; j++)
					{
						int a = std::rand() % 4;
						switch (a)
						{
						case 0:
							{
								m_eventSystem.AddQueuedEventCallback(this, std::rand() % 4, []() {});
							}
							break;
						case 1:
							{
								m_eventSystem.RemoveQueuedEventCallback(this, std::rand() % 4);
							}
							break;
						case 2:
							{
								m_eventSystem.QueueEvent(std::rand() % 4);
							}
							break;
						default:
							{
								m_eventSystem.ProcessEvents();
							}
							break;
						}
					}
				});
			}
		}
	}

private:
	EventSystem m_eventSystem;
};
} // namespace Logicraft