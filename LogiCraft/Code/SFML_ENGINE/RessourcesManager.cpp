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
#include "RessourcesManager.h"

static RessourcesManager* m_instance = nullptr;

RessourcesManager::RessourcesManager()
{
	m_isReady = true;
	m_loadingScreen = Animation(m_textures.getTexture("loading"), { 0,0,128,128, }, 0.1f, 8);
}

RessourcesManager::RessourcesManager(Animation loadScreen)
{
	m_isReady = true;
	m_loadingScreen = loadScreen;
}

RessourcesManager::~RessourcesManager()
{
}


RessourcesManager* RessourcesManager::getInstance()
{
	if (!m_instance)
		m_instance = new RessourcesManager();
	return m_instance;
}

void RessourcesManager::loadScene(std::string _Scene)
{
	m_isReady = false;

	std::thread t([this,_Scene] {
		m_audio.loadSceneSound(_Scene);
		m_font.loadFontScene(_Scene);
		m_textures.loadSceneResources(_Scene);
		m_movie.loadMovieScene(_Scene);
		
		
		m_isReady = true;
	});

	t.detach();

}

bool RessourcesManager::IsReady()
{
	return m_isReady;
}

void RessourcesManager::showLoadingScreen(sf::RenderWindow& window)
{
	m_loadingScreen.Animate(window);
}

void RessourcesManager::showLoadingScreen(sf::RenderWindow& window, int ColumnWidth)
{
	m_loadingScreen.Animate(window, ColumnWidth);
}

Animation& RessourcesManager::getLoadingScreen()
{
	return m_loadingScreen;
}

const sf::Texture& RessourcesManager::getTexture(std::string name)
{
	return m_textures.getTexture(name);
}

std::list<std::pair<unsigned, sf::Sound>>& RessourcesManager::getSound(std::string name)
{
	return m_audio.getSoundPlayer(name);
}

sf::Music& RessourcesManager::getMusic(std::string name)
{
	return m_audio.getMusic(name);
}

const sf::Font& RessourcesManager::getFont(std::string name)
{
	return m_font.getFont(name);
}

sf::SoundBuffer& RessourcesManager::getSoundBuffer(std::string name)
{
	return m_audio.getSound(name);
}

void RessourcesManager::playSound(std::string name, unsigned id, bool startNewSound, bool waitToFinish)
{
	m_audio.getSoundPlayer(name).remove_if([](auto& sound) {return sound.second.getStatus() == sf::Sound::Stopped; });

	if (startNewSound)
	{
		m_audio.getSoundPlayer(name).push_back(std::make_pair(id, sf::Sound(m_audio.getSound(name))));
		m_audio.getSoundPlayer(name).back().second.play();
	}
	else
	{
		auto tmp_sound = std::find_if(m_audio.getSoundPlayer(name).begin(), m_audio.getSoundPlayer(name).end(), [&id](auto& _sound) { return _sound.first == id; });

		if (tmp_sound != m_audio.getSoundPlayer(name).end() && !waitToFinish)
		{
			tmp_sound->second.play();
		}
		else if (tmp_sound == m_audio.getSoundPlayer(name).end())
		{
			m_audio.getSoundPlayer(name).push_back(std::make_pair(id, sf::Sound(m_audio.getSound(name))));
			m_audio.getSoundPlayer(name).back().second.play();
		}
	}
}

void RessourcesManager::playSound(std::string name, unsigned id, bool startNewSound, bool waitToFinish, float attenuation, float minDistance)
{
	m_audio.getSoundPlayer(name).remove_if([](auto& sound) {return sound.second.getStatus() == sf::Sound::Stopped; });

	if (startNewSound)
	{
		m_audio.getSoundPlayer(name).push_back(std::make_pair(id, sf::Sound(m_audio.getSound(name))));
		m_audio.getSoundPlayer(name).back().second.setAttenuation(attenuation);
		m_audio.getSoundPlayer(name).back().second.setMinDistance(minDistance);
		m_audio.getSoundPlayer(name).back().second.play();
	}
	else
	{
		auto tmp_sound = std::find_if(m_audio.getSoundPlayer(name).begin(), m_audio.getSoundPlayer(name).end(), [&id](auto& _sound) { return _sound.first == id; });

		if (tmp_sound != m_audio.getSoundPlayer(name).end() && !waitToFinish)
		{
			tmp_sound->second.setAttenuation(attenuation);
			tmp_sound->second.setMinDistance(minDistance);
			tmp_sound->second.play();
		}
		else if (tmp_sound == m_audio.getSoundPlayer(name).end())
		{
			m_audio.getSoundPlayer(name).push_back(std::make_pair(id, sf::Sound(m_audio.getSound(name))));
			m_audio.getSoundPlayer(name).back().second.setAttenuation(attenuation);
			m_audio.getSoundPlayer(name).back().second.setMinDistance(minDistance);
			m_audio.getSoundPlayer(name).back().second.play();
		}
	}
}

void RessourcesManager::updateSoundPosition(std::string name, unsigned id, sf::Vector3f position)
{
	for (auto sound = m_audio.getSoundPlayer(name).begin(); sound != m_audio.getSoundPlayer(name).end(); sound++)
	{
		if (sound->first == id)
			if (sound->second.getStatus() != sf::Sound::Stopped)
				sound->second.setPosition(position.x, 0.f, position.y);
	}
}

void RessourcesManager::playMusic(std::string name)
{
	if (m_audio.getMusic(name).getStatus() != sf::Music::Playing)
		m_audio.getMusic(name).play();
}

void RessourcesManager::stopMusic(std::string name)
{
	if (m_audio.getMusic(name).getStatus() == sf::Music::Playing)
		m_audio.getMusic(name).stop();
}

void RessourcesManager::stopSound(std::string name)
{
	for(auto& i : m_audio.getSoundPlayer(name))
		i.second.stop();
	m_audio.getSoundPlayer(name).remove_if([](auto& sound) {return sound.second.getStatus() == sf::Sound::Stopped; });
}

void RessourcesManager::stopAllMusics()
{
	for (auto it = m_audio.getAllMusic().first.begin(); it != m_audio.getAllMusic().first.end(); it++)
	{
		it->second.get()->getMusic().stop();
	}
	for (auto it = m_audio.getAllMusic().second.begin(); it != m_audio.getAllMusic().second.end(); it++)
	{
		it->second.get()->getMusic().stop();
	}
}

void RessourcesManager::stopAllSounds()
{
	for (auto it = m_audio.getAllSound().first.begin(); it != m_audio.getAllSound().first.end(); it++)
	{
		for (auto& i : it->second.get()->getSoundPlayer())
			i.second.stop();
		it->second.get()->getSoundPlayer().remove_if([](auto& sound) {return sound.second.getStatus() == sf::Sound::Stopped; });
	}
	for (auto it = m_audio.getAllSound().second.begin(); it != m_audio.getAllSound().second.end(); it++)
	{
		for (auto& i : it->second.get()->getSoundPlayer())
			i.second.stop();
		it->second.get()->getSoundPlayer().remove_if([](auto& sound) {return sound.second.getStatus() == sf::Sound::Stopped; });
	}
}

void RessourcesManager::setVolumeForAllSound(float volume)
{
	for (auto it = m_audio.getAllSound().first.begin(); it != m_audio.getAllSound().first.end(); it++)
	{
		for (auto& i : it->second.get()->getSoundPlayer())
			i.second.setVolume(volume);
	}
	for (auto it = m_audio.getAllSound().second.begin(); it != m_audio.getAllSound().second.end(); it++)
	{
		for (auto& i : it->second.get()->getSoundPlayer())
			i.second.setVolume(volume);
	}
}

void RessourcesManager::setVolumeForAllMusic(float volume)
{
	for (auto it = m_audio.getAllMusic().first.begin(); it != m_audio.getAllMusic().first.end(); it++)
		it->second.get()->getMusic().setVolume(volume);
	for (auto it = m_audio.getAllMusic().second.begin(); it != m_audio.getAllMusic().second.end(); it++)
		it->second.get()->getMusic().setVolume(volume);
}

void RessourcesManager::setVolumeForSpecificSound(std::string name, float volume)
{
	for (auto& i : m_audio.getSoundPlayer(name))
		i.second.setVolume(volume);
}

void RessourcesManager::setVolumeForSpecificMusic(std::string name, float volume)
{
	m_audio.getMusic(name).setVolume(volume);
}

void RessourcesManager::setSoundPosition(std::string name, sf::Vector3f position)
{
	for (auto& i : m_audio.getSoundPlayer(name))
		i.second.setPosition(position);
}

void RessourcesManager::setSoundAttenuation(std::string name, float attenuation)
{
	for (auto& i : m_audio.getSoundPlayer(name))
		i.second.setAttenuation(attenuation);
}

void RessourcesManager::setSoundMinDistance(std::string name, float distance)
{
	for (auto& i : m_audio.getSoundPlayer(name))
		i.second.setMinDistance(distance);
}


sfe::Movie& RessourcesManager::getMovie(std::string name)
{
	return m_movie.getMovie(name);
}

void RessourcesManager::playMovie(std::string name, sf::RenderWindow& _window)
{
	auto& movie = m_movie.getMovie(name);
	if (movie.getStatus() != sfe::Playing)
		movie.play();
	movie.update();
	movie.fit(0.f, 0.f, static_cast<float>(_window.getSize().x), static_cast<float>(_window.getSize().y));
	_window.draw(movie);
}

void RessourcesManager::setVolumeForAllMovie(float volume)
{
	for (auto it = m_movie.getAllMovie().first.begin(); it != m_movie.getAllMovie().first.end(); it++)
		it->second.get()->getMovie().setVolume(volume);
	for (auto it = m_movie.getAllMovie().second.begin(); it != m_movie.getAllMovie().second.end(); it++)
		it->second.get()->getMovie().setVolume(volume);
}

void RessourcesManager::setVolumeForSpecificMovie(std::string name, float volume)
{
	m_movie.getMovie(name).setVolume(volume);
}
