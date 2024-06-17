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
#include <list>
#include "SFML/Audio.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Network.hpp"
#include "sfeMovie/Movie.hpp"
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <memory>
#include <vector>
#include <assert.h>
#include <filesystem>
#include <random>
#include <thread>
#include <algorithm>
#include <stack>
#include <unordered_map>

#ifdef SFMLENGINE_EXPORTS
#define SFMLENGINE_API __declspec(dllexport)
#else
#define SFMLENGINE_API __declspec(dllimport)
#endif



#define KEY(a) sf::Keyboard::isKeyPressed(sf::Keyboard::a)
#define MOUSE(a) sf::Mouse::isButtonPressed(sf::Mouse::a)
#define RAD2DEG 57.295779f
#define DEG2RAD 0.017453f
#define PI 3.141593f
#define LERP(a,b,t) (b-a)*t+a
#define MAX(a,b) a>=b ? a:b
#define MIN(a,b) a<=b ? a:b

#define MAX_THREAD 10

typedef std::list<std::pair<std::string, std::string>> FilesInfo;

namespace Tools
{
    float SFMLENGINE_API const getDeltaTime();
    void SFMLENGINE_API restartClock();
    template <typename T>
    T Rand(T min, T max)
    {
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_real_distribution<double> distribution(min, max);
        return static_cast<T>(distribution(rd));
    }
    float SFMLENGINE_API frameRate();
    template <typename T>
    T Lerp(T a, T b, float t) { return  static_cast<T>(a + (b - a) * t); }
    std::string SFMLENGINE_API  ToLower(std::string string_);
    std::string SFMLENGINE_API  ToUpper(std::string string_);
    FilesInfo SFMLENGINE_API  searchFilesInfo(std::string path, std::string extension);


    namespace Vector
    {
        sf::Vector2f SFMLENGINE_API  getVectorFromAngle(sf::Vector2f pos, float lenght, float angle);

        template <typename T>
        float getDistance(sf::Vector2<T> pos1, sf::Vector2<T> pos2)
        {
            return sqrt(pow(pos2.x - pos1.x, 2) + pow(pos2.y - pos1.y, 2));
        }

        template <typename T>
        float dotProduct(sf::Vector2<T> pos1, sf::Vector2<T> pos2)
        {
            return pos1.x * pos2.x + pos1.y * pos2.y;
        }

        template <typename T>
        float getAngle(sf::Vector2<T> pos1, sf::Vector2<T> pos2)
        {
            return atan2(pos2.y - pos1.y, pos2.x - pos1.x);
        }

        template <typename T>
        float getNorme(sf::Vector2<T> pos)
        {
            if (pos != sf::Vector2f())
                return sqrt(pos.x * pos.x + pos.y * pos.y);

            return 1.f;
        }

        template<typename T>
        sf::Vector2<T> normalize(sf::Vector2<T> _v)
        {
            return _v / getNorme(_v);
        }
    }

    namespace Collisions
    {
        bool SFMLENGINE_API  point_circle(sf::Vector2<float> _mpos, sf::Vector2<float> _cpos, float _r);
        bool SFMLENGINE_API  circle_circle(sf::Vector2<float> _c1pos, float _c1r, sf::Vector2<float> _c2pos, float _c2r);
        bool SFMLENGINE_API  point_rect(sf::Vector2<float> _mpos, sf::FloatRect _rect);
        bool SFMLENGINE_API  rect_rect(sf::FloatRect _rect1, sf::FloatRect _rect2);
        bool SFMLENGINE_API  circle_rect(sf::Vector2<float> cpos, float radius, sf::FloatRect rect);
    }


}


