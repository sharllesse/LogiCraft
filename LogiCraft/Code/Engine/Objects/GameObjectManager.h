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
#include "GameObject.h"

#include <guiddef.h>
#include <memory>
#include <shared_mutex>
#include <vector>

namespace Logicraft
{
class LOGI_ENGINE_API GameObjectManager
{
public:
	static GameObjectManager& Get();

	GameObjectManager();
	~GameObjectManager();

	void Init();

	GameObjectPtr CreateObject();
	void          RemoveObject(REFGUID objectGUID);
	GameObjectPtr GetObject(REFGUID objectGUID) const;

	GameComponentPtr CreateComponent(const char* componentType);
	void             RemoveComponent(REFGUID componentGUID);
	GameComponentPtr GetComponent(REFGUID componentGUID) const;

	const std::vector<GameObjectPtr>&    GetObjects() const { return m_objects; }
	const std::vector<GameComponentPtr>& GetComponents() const { return m_components; }

protected:
	bool                          m_infoLogEnabled = false;
	mutable std::shared_mutex     m_objectsMutex;
	std::vector<GameObjectPtr>    m_objects;
	mutable std::shared_mutex     m_componentsMutex;
	std::vector<GameComponentPtr> m_components;
};
} // namespace Logicraft