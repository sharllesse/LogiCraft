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
#include "Tools.h"


static sf::Time m_time = m_time.Zero;
static sf::Clock m_clock = sf::Clock::Clock();


float Tools::frameRate()
{
	return 1.0f / getDeltaTime();
}

float const Tools::getDeltaTime()
{
	return m_time.asSeconds();
}

void Tools::restartClock()
{
	m_time = m_clock.restart();
}

std::string Tools::ToLower(std::string string_)
{
	std::transform(string_.begin(), string_.end(), string_.begin(), [](unsigned char c) {return std::tolower(c); });
	return string_;
}

std::string Tools::ToUpper(std::string string_)
{
	std::transform(string_.begin(), string_.end(), string_.begin(), [](unsigned char c) {return std::toupper(c); });
	return string_;
}

FilesInfo Tools::searchFilesInfo(std::string path, std::string extension)
{
	FilesInfo fileInfos;
	if(std::filesystem::exists(path))
		for (auto dir : std::filesystem::recursive_directory_iterator(path))
		{
			if (dir.path().filename().extension().string() == extension)
				fileInfos.push_back(std::make_pair(Tools::ToLower(dir.path().filename().stem().string()), dir.path().string()));
		}
	return fileInfos;
}

sf::Vector2f Tools::Vector::getVectorFromAngle(sf::Vector2f pos, float lenght, float angle)
{
    angle *= DEG2RAD;

    float x = pos.x + lenght * cos(angle);
    float y = pos.y + lenght * sin(angle);

    return sf::Vector2f(x, y);
}

bool Tools::Collisions::point_circle(sf::Vector2<float> _mpos, sf::Vector2<float> _cpos, float _r)
{
    // get distance between the point and circle's center
    float distX = _mpos.x - _cpos.x;
    float distY = _mpos.y - _cpos.y;
    float distance = sqrt((distX * distX) + (distY * distY));

    // if the distance is less than the circle's
    // radius the point is inside!
    if (distance <= _r) {
        return true;
    }

    return false;
}

bool Tools::Collisions::circle_circle(sf::Vector2<float> _c1pos, float _c1r, sf::Vector2<float> _c2pos, float _c2r)
{
    float distX = _c1pos.x - _c2pos.x;
    float distY = _c1pos.y - _c2pos.y;
    float distance = sqrt((distX * distX) + (distY * distY));

    // if the distance is less than the sum of the circle's
    // radii, the circles are touching!
    if (distance <= _c1r + _c2r) {
        return true;
    }
    return false;
}

bool Tools::Collisions::point_rect(sf::Vector2<float> _mpos, sf::FloatRect _rect)
{
    // is the point inside the rectangle's bounds?
    if (_mpos.x >= _rect.left &&        // right of the left edge AND
        _mpos.x <= _rect.left + _rect.width &&   // left of the right edge AND
        _mpos.y >= _rect.top &&        // below the top AND
        _mpos.y <= _rect.top + _rect.height) {   // above the bottom
        return true;
    }
    return false;
}

bool Tools::Collisions::rect_rect(sf::FloatRect _rect1, sf::FloatRect _rect2)
{
    if (_rect1.left + _rect1.width >= _rect2.left &&    // r1 right edge past r2 left
        _rect1.left <= _rect2.left + _rect2.width &&    // r1 left edge past r2 right
        _rect1.top + _rect1.height >= _rect2.top &&    // r1 top edge past r2 bottom
        _rect1.top <= _rect2.top + _rect2.height) {    // r1 bottom edge past r2 top
        return true;
    }

    return false;
}

bool Tools::Collisions::circle_rect(sf::Vector2<float> cpos, float radius, sf::FloatRect rect)
{
    // temporary variables to set edges for testing
    float testX = cpos.x;
    float testY = cpos.y;

    // which edge is closest?
    if (cpos.x < rect.left)         testX = rect.left;      // test left edge
    else if (cpos.x > rect.left + rect.width) testX = rect.left + rect.width;   // right edge
    if (cpos.y < rect.top)         testY = rect.top;      // top edge
    else if (cpos.y > rect.top + rect.height) testY = rect.top + rect.height;   // bottom edge

    // get distance from closest edges
    float distX = cpos.x - testX;
    float distY = cpos.y - testY;
    float distance = sqrt((distX * distX) + (distY * distY));

    // Si la distance est inférieure ou égale au rayon, il y a collision
    if (distance <= radius) {
        return true;
    }
    return false;
}
