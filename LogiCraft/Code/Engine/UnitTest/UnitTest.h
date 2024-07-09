
#include "Engine/Core/EventSystem.h"
#include "Engine/Core/TaskManager.h"

#include <cstdlib>

using namespace Logicraft;
class UnitTest
{
public:
	UnitTest()  = default;
	~UnitTest() = default;

	void Start()
	{
		if (!m_enableUnit)
			return;

		// EventSystem
		{
			std::srand(4);

			for (int i = 0; i < 100; i++)
			{
				TaskManager::Get().AddTask([this]() {
					for (int i = 0; i < 100; i++)
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
	bool        m_enableUnit{false};
};
