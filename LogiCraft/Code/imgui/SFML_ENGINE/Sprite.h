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
#pragma once
#include "Tools.h"

class SFMLENGINE_API Animation
{
	sf::Sprite m_sprite;
	sf::IntRect m_animRect;
	float m_animTime;
	float m_animTimer;
	int m_frameMax;
	int m_frameX;
	sf::FloatRect m_hitbox;
public:
	Animation();
	Animation(const sf::Texture& texture, sf::IntRect animRect, float animTime, int frameMax);
	Animation(const Animation& anim);
	~Animation();


	////////////////////////////////////////////////////////////
	/// \brief make the animation on the sprite
	///
	////////////////////////////////////////////////////////////
	void Animate();
	////////////////////////////////////////////////////////////
	/// \brief make the animation on the sprite, this overload draw the sprite in the function
	/// 
	/// \param _window window were you want to draw the sprite
	/// 
	////////////////////////////////////////////////////////////
	void Animate(sf::RenderWindow& _window);
	////////////////////////////////////////////////////////////
	/// \brief make the animation on the sprite, of the sprite is on different column, tell the column width and the function will animate it automatically
	/// 
	/// \param ColumnWidth width of the column
	/// 
	////////////////////////////////////////////////////////////
	void Animate(int ColumnWidth);
	////////////////////////////////////////////////////////////
	/// \brief make the animation on the sprite, of the sprite is on different column, tell the column width and the function will animate it automatically,
	///  this overload draw the sprite in the function
	/// 
	/// \param ColumnWidth width of the column
	/// 
	////////////////////////////////////////////////////////////
	void Animate(sf::RenderWindow& _window, int ColumnWidth);

	////////////////////////////////////////////////////////////
	/// \brief return the sprite of the animation
	/// 
	/// \param ColumnWidth width of the column
	/// 
	////////////////////////////////////////////////////////////
	sf::Sprite& getSprite();

	////////////////////////////////////////////////////////////
	/// \brief return the actual frame of the animation
	/// 
	////////////////////////////////////////////////////////////
	int getFrameX() const;

	float getAnimTime() const;

	void setAnimTime(float _animTime);

	void setHitBox(sf::FloatRect hitbox);

	sf::FloatRect getHitBox();

	void setAnimRect(sf::IntRect rect);

	sf::IntRect getAnimRect() const;

};

