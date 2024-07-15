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

#include "ComponentCollider.h"
#include "Core/Engine.h"

using namespace Logicraft;

ComponentCollider::ComponentCollider()
{
	m_colliderType = eBoxCollider;
	m_enable       = true;

	m_size   = {200.f, 200.f};
	m_offset = {0.f, 0.f};
	m_color  = sf::Color::Magenta;

	m_radius = 50.f;

	sf::Color tmp{0, 0, 0, 0};

	m_rectangleShape.setSize(m_size);
	m_rectangleShape.setOutlineColor(m_color);
	m_rectangleShape.setFillColor(tmp);
	m_rectangleShape.setOutlineThickness(-2.5f);

	m_circleShape.setRadius(m_radius);
	m_circleShape.setOutlineColor(m_color);
	m_circleShape.setFillColor(tmp);
	m_circleShape.setOutlineThickness(-2.5f);
}

void ComponentCollider::Render(sf::RenderWindow& target)
{
	if (!m_enable)
		return;

	switch (m_colliderType)
	{
	case eBoxCollider: target.draw(m_rectangleShape); break;
	case eSphereCollider: target.draw(m_circleShape); break;
	case eSpriteCollider: break;
	default: break;
	}
}

void ComponentCollider::SetBoxCollider(sf::Vector2f size, sf::Vector2f offset)
{
	m_size   = size;
	m_offset = offset;
	m_rectangleShape.setSize(m_size);
	m_rectangleShape.setPosition(m_offset);
}

void ComponentCollider::SetSphereCollider(float radius, sf::Vector2f offset)
{
	m_radius = radius;
	m_offset = offset;

	m_circleShape.setRadius(m_radius);
	m_circleShape.setPosition(m_offset);
}

void ComponentCollider::SetTypeOfCollider(int value)
{
	m_colliderType = static_cast<ECollider>(value);
}

void ComponentCollider::SetEnable(bool enable)
{
	m_enable = enable;
}
