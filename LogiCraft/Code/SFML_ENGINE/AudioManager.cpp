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
#include "AudioManager.h"

#define EMPTY_SOUND_NAME "placeholder"
#define EMPTY_MUSIC_NAME "musicplaceholder"

#pragma region SOUND

Sound::Sound()
{
	bool isSuccessful = m_sound.loadFromFile("../Ressources/ALL/SOUND/Placeholder.ogg");
#ifdef _DEBUG
	if (isSuccessful)
		std::cout << "Sound Placeholder loaded\n";
	else
		std::cout << "Sound Placeholder can't be load\n";
#endif // !DEBUG
}

Sound::Sound(std::string path)
{
	bool isSuccessful = m_sound.loadFromFile(path);
#ifdef _DEBUG
	if (isSuccessful)
		std::cout << "Sound " << path << " loaded\n";
	else
		std::cout << "Sound " << path << " can't be load\n";
#endif // !DEBUG
}

Sound::~Sound()
{
}

sf::SoundBuffer& Sound::getSound()
{
	return m_sound;
}

std::list<std::pair<unsigned, sf::Sound>>& Sound::getSoundPlayer()
{
	return m_soundPlayer;
}
#pragma endregion

#pragma region MUSIC

Music::Music()
{
	m_music = std::move(std::make_unique<sf::Music>());
	bool isSuccessful = m_music.get()->openFromFile("../Ressources/ALL/SOUND/MusicPlaceholder.ogg");
#ifdef _DEBUG
	if (isSuccessful)
		std::cout << "Sound Placeholder loaded\n";
	else
		std::cout << "Sound Placeholder can't be load\n";
#endif // !DEBUG
}

Music::Music(std::string path)
{
	m_music = std::make_unique<sf::Music>();
	bool isSuccessful = m_music.get()->openFromFile(path);
#ifdef _DEBUG
	if (isSuccessful)
		std::cout << "Music " << path << " loaded\n";
	else
		std::cout << "Music " << path << " can't be load\n";
#endif // !DEBUG
}

Music::~Music()
{
}

sf::Music& Music::getMusic()
{
	return *m_music.get();
}
#pragma endregion

#pragma region SOUND_MANAGER

AudioManager::AudioManager()
{
#ifdef _DEBUG
	std::cout << "------- Starting global music load ------- \n";
#endif // !DEBUG

	for (auto dir : std::filesystem::recursive_directory_iterator("../Ressources/ALL/MUSICS"))
	{
		m_GlobalMusic.emplace(Tools::ToLower(dir.path().filename().stem().string()), std::move(std::make_unique<Music>(dir.path().string())));
	}

#ifdef _DEBUG
	std::cout << "------- Global music loading finish ------- \n\n\n";
#endif // !DEBUG

#ifdef _DEBUG
	std::cout << "------- Starting global sound load -------\n";
#endif // !DEBUG
	for (auto dir : std::filesystem::recursive_directory_iterator("../Ressources/ALL/SOUNDS"))
	{
		m_GlobalSound.emplace(Tools::ToLower(dir.path().filename().stem().string()), std::move(std::make_unique<Sound>(dir.path().string())));
	}

#ifdef _DEBUG
	std::cout << "------- Global sound loading finish -------\n\n\n";
#endif // !DEBUG
}

AudioManager::~AudioManager()
{
}


void AudioManager::loadSceneSound(std::string _Scene)
{
#ifdef _DEBUG
	std::cout << "------- Starting " << _Scene << " music/sound load -------\n";
#endif // !DEBUG
	m_SceneMusic.clear();
	m_SceneSound.clear();

	auto fileInfosSound = Tools::searchFilesInfo("../Ressources/" + _Scene + "/Sounds", ".ogg");
	auto fileInfosSoundWav = Tools::searchFilesInfo("../Ressources/" + _Scene + "/Sounds", ".wav");
	fileInfosSound.insert(fileInfosSound.end(), fileInfosSoundWav.begin(), fileInfosSoundWav.end());

	if(!fileInfosSound.empty())
	{

		int nbrThread = (static_cast<int>(fileInfosSound.size()) < MAX_THREAD ? static_cast<int>(fileInfosSound.size()) : MAX_THREAD);
		int block_size = static_cast<int>(fileInfosSound.size()) / nbrThread;

		std::list<std::thread> listThread;

		for (int i = 0; i < nbrThread; i++)
		{
			int start = i * block_size;
			int end = (i == nbrThread - 1) ? static_cast<int>(fileInfosSound.size()) : (i + 1) * block_size;

			listThread.push_back(std::thread([start, end, &fileInfosSound, this]
				{
					auto itBegin = std::next(fileInfosSound.begin(), start);
					auto itEnd = std::next(fileInfosSound.begin(), end);

					for (auto it = itBegin; it != itEnd; it++)
					{
						m_SceneSound.emplace(it->first, std::move(std::make_unique<Sound>(it->second)));
					}

				}));
		}

		for (auto& i : listThread)
			i.join();

		listThread.clear();
	}


	
	auto fileInfosMusic = Tools::searchFilesInfo("../Ressources/" + _Scene + "/Musics", ".ogg");
	auto fileInfosMusicWav = Tools::searchFilesInfo("../Ressources/" + _Scene + "/Musics", ".wav");

	fileInfosMusic.insert(fileInfosMusic.end(), fileInfosMusicWav.begin(), fileInfosMusicWav.end());

	if(!fileInfosMusic.empty())
	{
		int nbrThread = (static_cast<int>(fileInfosMusic.size()) < MAX_THREAD ? static_cast<int>(fileInfosMusic.size()) : MAX_THREAD);
		int block_size = static_cast<int>(fileInfosMusic.size()) / nbrThread;

		std::list<std::thread> listThread;

		for (int i = 0; i < nbrThread; i++)
		{
			int start = i * block_size;
			int end = (i == nbrThread - 1) ? static_cast<int>(fileInfosMusic.size()) : (i + 1) * block_size;

			listThread.push_back(std::thread([start, end, &fileInfosMusic, this]
				{
					auto itBegin = std::next(fileInfosMusic.begin(), start);
					auto itEnd = std::next(fileInfosMusic.begin(), end);

					for (auto it = itBegin; it != itEnd; it++)
					{
						m_SceneMusic.emplace(it->first, std::move(std::make_unique<Music>(it->second)));
					}

				}));
		}

		for (auto& i : listThread)
			i.join();
	}

#ifdef _DEBUG
	std::cout  << "------- " << _Scene << " music/sound loading finish -------\n\n\n";
#endif // !DEBUG

}

sf::Music& AudioManager::getMusic(std::string name)
{
	name = Tools::ToLower(name);
	if (m_GlobalMusic[name])
		return m_GlobalMusic[name].get()->getMusic();
	else if (m_SceneMusic[name])
		return m_SceneMusic[name].get()->getMusic();
	else
	{
		m_GlobalMusic.erase(name);
		m_SceneMusic.erase(name);
#ifdef _DEBUG
		std::cout << "Cannot find the music \"" << name << "\"\n";
#endif // !DEBUG
		return m_GlobalMusic[EMPTY_MUSIC_NAME].get()->getMusic();
	}
}

sf::SoundBuffer& AudioManager::getSound(std::string name)
{
	name = Tools::ToLower(name);
	if (m_GlobalSound[name])
		return m_GlobalSound[name].get()->getSound();
	else if (m_SceneSound[name])
		return m_SceneSound[name].get()->getSound();
	else
	{
		m_GlobalSound.erase(name);
		m_SceneSound.erase(name);
#ifdef _DEBUG
		std::cout << "Cannot find the sound \"" << name << "\"\n";
#endif // !DEBUG
		return m_GlobalSound[EMPTY_SOUND_NAME].get()->getSound();
	}
}

std::list<std::pair<unsigned, sf::Sound>>& AudioManager::getSoundPlayer(std::string name)
{
	name = Tools::ToLower(name);
	if (m_GlobalSound[name])
		return m_GlobalSound[name].get()->getSoundPlayer();
	else if (m_SceneSound[name])
		return m_SceneSound[name].get()->getSoundPlayer();
	else
	{
		m_GlobalSound.erase(name);
		m_SceneSound.erase(name);
#ifdef _DEBUG
		std::cout << "Cannot find the sound \"" << name << "\"\n";
#endif // !DEBUG
		return m_GlobalSound[EMPTY_SOUND_NAME].get()->getSoundPlayer();
	}
}

std::pair<MusicMap&, MusicMap&> AudioManager::getAllMusic()
{
	return std::pair<MusicMap&, MusicMap&>(std::make_pair(std::ref(m_GlobalMusic), std::ref(m_SceneMusic)));
}

std::pair<SoundMap&, SoundMap&> AudioManager::getAllSound()
{
	return std::pair<SoundMap&, SoundMap&>(std::make_pair(std::ref(m_GlobalSound), std::ref(m_SceneSound)));
}

#pragma endregion


