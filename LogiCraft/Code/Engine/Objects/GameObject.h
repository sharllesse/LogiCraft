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

#pragma once
#include "Core/Serializable.h"
#include "GameComponent.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <memory>
#include <shared_mutex>
#include <vector>

namespace Logicraft
{
class GameObject;
using GameObjectPtr = std::shared_ptr<GameObject>;

class LOGI_ENGINE_API GameObject : public Serializable
{
public:
	struct GUIDCompare
	{
		GUIDCompare(REFGUID guid)
		  : m_guid(guid)
		{
		}

		bool operator()(const GameObjectPtr& object) const { return object->GetGUID() == m_guid; }

		REFGUID m_guid;
	};

	GameObject();
	void Release();

	void Update();
	void Render(sf::RenderWindow& target);

	void                                 AddComponent(GameComponentPtr component);
	void                                 RemoveComponent(GameComponentPtr component);
	const std::vector<GameComponentPtr>& GetComponents() { return m_components; }

	GUID GetGUID() const { return m_GUID; }

	void Serialize(bool load, JsonObjectPtr pJsonObject) override;

protected:
	void Load() override;

private:
	GUID                          m_GUID{0};
	sf::Vector2f                  m_position;
	std::shared_mutex             m_componentsMutex;
	std::vector<GameComponentPtr> m_components;
};
} // namespace Logicraft