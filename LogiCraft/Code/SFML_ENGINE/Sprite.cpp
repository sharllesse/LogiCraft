/*
    Based on Coffee Engine

    Author: GRALLAN Yann

    Description: SFML-Engine

    Date: 2024/01/31

    MIT License

    Copyright (c) 2024 GRALLAN Yann


    Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "Sprite.h"

Animation::Animation() : m_sprite(), m_animRect(), m_animTime(0.f), m_animTimer(0.f), m_frameMax(0), m_frameX(0)
{
	m_sprite.setTextureRect(m_animRect);
	m_hitbox = { m_sprite.getPosition() - m_sprite.getOrigin(), m_sprite.getGlobalBounds().getSize() };
}

Animation::Animation(const sf::Texture& texture, sf::IntRect animRect, float animTime, int frameMax) : m_sprite(texture), m_animRect(animRect), m_animTime(animTime), m_frameMax(frameMax), m_animTimer(0.f), m_frameX(0)
{
	m_sprite.setTextureRect(m_animRect);
	m_hitbox = { m_sprite.getPosition() - m_sprite.getOrigin(), m_sprite.getGlobalBounds().getSize() };
}

Animation::Animation(const Animation& anim)
{
	m_sprite = anim.m_sprite;
	m_animRect = anim.m_animRect;
	m_animTime = anim.m_animTime;
	m_animTimer = anim.m_animTimer;
	m_frameMax = anim.m_frameMax;
	m_frameX = anim.m_frameX;
	m_sprite.setTextureRect(m_animRect);
	m_hitbox = { m_sprite.getPosition() - m_sprite.getOrigin(), m_sprite.getGlobalBounds().getSize() };
}

Animation::~Animation()
{
}

void Animation::Animate()
{
	m_animTimer += Tools::getDeltaTime();
	if (m_animTimer > m_animTime)
	{
		m_frameX++;
		if (m_frameX > m_frameMax)
		{
			m_frameX = 0;
		}
		m_animTimer = 0.f;
	}
	m_animRect.left = m_animRect.width * m_frameX;
	m_sprite.setTextureRect(m_animRect);
	m_hitbox = { m_sprite.getPosition() - m_sprite.getOrigin(), m_sprite.getGlobalBounds().getSize() };
}

void Animation::Animate(sf::RenderWindow& _window)
{
	m_animTimer += Tools::getDeltaTime();
	if (m_animTimer > m_animTime)
	{
		m_frameX++;
		if (m_frameX > m_frameMax)
		{
			m_frameX = 0;
		}
		m_animTimer = 0.f;
	}
	m_animRect.left = m_animRect.width * m_frameX;
	m_sprite.setTextureRect(m_animRect);
	_window.draw(m_sprite);
	m_hitbox = { m_sprite.getPosition() - m_sprite.getOrigin(), m_sprite.getGlobalBounds().getSize() };
}

void Animation::Animate(int ColumnWidth)
{
	m_animTimer += Tools::getDeltaTime();
	if (m_animTimer > m_animTime)
	{
		m_frameX++;
		if (m_frameX > m_frameMax)
		{
			m_frameX = 0;
		}
		m_animTimer = 0.f;
	}
	m_animRect.left = m_animRect.width * (m_frameX % ColumnWidth);
	m_animRect.top = m_animRect.height * (m_frameX / ColumnWidth);
	m_sprite.setTextureRect(m_animRect);
	m_hitbox = { m_sprite.getPosition() - m_sprite.getOrigin(), m_sprite.getGlobalBounds().getSize() };
}

void Animation::Animate(sf::RenderWindow& _window, int ColumnWidth)
{
	m_animTimer += Tools::getDeltaTime();
	if (m_animTimer > m_animTime)
	{
		m_frameX++;
		if (m_frameX > m_frameMax)
		{
			m_frameX = 0;
		}
		m_animTimer = 0.f;
	}
	m_animRect.left = m_animRect.width * (m_frameX % ColumnWidth);
	m_animRect.top = m_animRect.height * (m_frameX / ColumnWidth);
	m_sprite.setTextureRect(m_animRect);
	_window.draw(m_sprite);
	m_hitbox = { m_sprite.getPosition() - m_sprite.getOrigin(), m_sprite.getGlobalBounds().getSize() };
}

sf::Sprite& Animation::getSprite()
{
	return m_sprite;
}


int Animation::getFrameX() const
{
	return m_frameX;
}	

sf::FloatRect Animation::getHitBox()
{
	return m_hitbox;
}

void Animation::setHitBox(sf::FloatRect hitBox)
{
	m_hitbox = hitBox;
}

void Animation::setAnimRect(sf::IntRect rect)
{
	m_animRect = rect;
	m_sprite.setTextureRect(m_animRect);
}

sf::IntRect Animation::getAnimRect() const
{
	return m_animRect;
}

