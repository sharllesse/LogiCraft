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
#include "WindowManager.h"

WindowManager::WindowManager() : m_size(1280, 720), m_event(), m_isDone(false), m_isFullscreen(false), m_title("SFML Window"), m_timer(0.f), m_UpdateIsStopped(false)
{
	m_window.create(sf::VideoMode(1280, 720), "SFML Window", m_isFullscreen ? sf::Style::Fullscreen : sf::Style::Default);
}

WindowManager::WindowManager(int width, int height, std::string title, bool fullscreen) : m_size(width, height), m_event(),m_isDone(false),m_isFullscreen(fullscreen),m_title(title),m_timer(0.f),m_UpdateIsStopped(false)
{
	m_window.create(sf::VideoMode(width, height), title, m_isFullscreen ? sf::Style::Fullscreen : sf::Style::Default);
}

bool WindowManager::isFullscreen() const
{
	return m_isFullscreen;
}

void WindowManager::toggleFullscreen()
{
	m_isFullscreen = !m_isFullscreen;
	m_window.close();
	m_window.create(sf::VideoMode(m_size.x, m_size.y), m_title, m_isFullscreen ? sf::Style::Fullscreen : sf::Style::Default);
}

float WindowManager::timer() const
{
	return m_timer;
}

void WindowManager::resetTimer()
{
	m_timer = 0.f;
}

void WindowManager::EventUpdate()
{
	m_timer += Tools::getDeltaTime();
	if (!m_UpdateIsStopped)
		while (m_window.pollEvent(m_event))
			if (m_event.type == m_event.Closed)
				m_isDone = 1;
	if (m_isDone)
		m_window.close();

}

sf::Event& WindowManager::getEvent()
{
	return m_event;
}

bool WindowManager::pollEvent()
{
	return m_window.pollEvent(m_event);
}

sf::RenderWindow& WindowManager::getWindow()
{
	return m_window;
}

sf::Vector2i WindowManager::getSize() const
{
	return m_size;
}

void WindowManager::clear(sf::Color clearColor)
{
	m_window.clear(clearColor);
}

void WindowManager::draw(const sf::Drawable& drawable_, const sf::RenderStates& render_)
{
	m_window.draw(drawable_, render_);
}

void WindowManager::display()
{
	m_window.display();
}

bool WindowManager::isDone()
{
	return m_isDone;
}

bool WindowManager::EventUpdateIsStopped()
{
	return m_UpdateIsStopped;
}

void WindowManager::StopEventUpdate(bool stop)
{
	m_UpdateIsStopped = stop;
}

float WindowManager::getSFX_Volume() const
{
	return m_SFX_Volume;
}

float WindowManager::getMusic_Volume() const
{
	return m_Music_Volume;
}

float WindowManager::getMovie_Volume() const
{
	return m_Movie_Volume;
}

void WindowManager::setSFX_Volume(float volume)
{
	m_SFX_Volume = volume;
}

void WindowManager::setMusic_Volume(float volume)
{
	m_Music_Volume = volume;
}

void WindowManager::setMovie_Volume(float volume)
{
	m_Movie_Volume = volume;
}
