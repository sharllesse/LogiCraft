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
#include "Objects/GameComponent.h"
#include "Utils/TypeDefinition.h"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>

namespace Logicraft
{
enum ECollider
{
	eBoxCollider = 0,
	eSphereCollider,
	eSpriteCollider
};
class LOGI_ENGINE_API ComponentCollider : public GameComponent
{
	LOGI_TYPEDEF_DERIVED_TYPE(GameComponent, ComponentCollider, "Collider")

public:
	ComponentCollider();

	void Render(sf::RenderWindow& target) override;

	void SetBoxCollider(sf::Vector2f size, sf::Vector2f offset);
	void SetSphereCollider(float radius, sf::Vector2f offset);
	void SetTypeOfCollider(int value);
	void SetEnable(bool enable);

private:
	ECollider m_colliderType;

	sf::Vector2f m_offset;
	sf::Vector2f m_size;
	float        m_radius;
	sf::Color    m_color;
	bool         m_enable;

	sf::RectangleShape m_rectangleShape;
	sf::CircleShape    m_circleShape;
};
using ComponentColliderPtr = std::shared_ptr<ComponentCollider>;
} // namespace Logicraft