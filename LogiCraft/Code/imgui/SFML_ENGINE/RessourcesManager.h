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
#include "FontManager.h"
#include "TexturesManager.h"
#include "AudioManager.h"
#include "Sprite.h"
#include "MovieManager.h"

#define GET_MANAGER RessourcesManager::getInstance()


class SFMLENGINE_API RessourcesManager
{
	RessourcesManager();
	RessourcesManager(Animation loadScreen);
	~RessourcesManager();
	AudioManager m_audio;
	FontManager m_font;
	TexturesManager m_textures;
	MovieManager m_movie;

	Animation m_loadingScreen;
	bool m_isReady;
public:
	static RessourcesManager* getInstance();

#pragma region RESSOURCES_MANAGER


	////////////////////////////////////////////////////////////
	/// \brief load all the ressources associates to the repository
	///
	/// \param _Scene repository you want to load. Exemple : if you want to load all the ressources in this repository "..//Ressources//HUB", just type HUB
	/// 
	////////////////////////////////////////////////////////////
	void loadScene(std::string  _Scene);

	////////////////////////////////////////////////////////////
	/// \brief return a bool to tell if the loading is finish or not
	/// 
	////////////////////////////////////////////////////////////
	bool IsReady();

	////////////////////////////////////////////////////////////
	/// \brief display the loading screen loaded in the RessourcesManager
	///
	/// \param window window were the loading screen is supposed to be displayed
	/// 
	////////////////////////////////////////////////////////////
	void showLoadingScreen(sf::RenderWindow& window);

	////////////////////////////////////////////////////////////
	/// \brief display the loading screen loaded in the RessourcesManager, use this function if your animation is on a array
	///
	/// \param window window were the loading screen is supposed to be displayed
	/// \param ColumnWidth width of the array
	/// 
	////////////////////////////////////////////////////////////
	void showLoadingScreen(sf::RenderWindow& window, int ColumnWidth);

	////////////////////////////////////////////////////////////
	/// \brief return a ref of the loading screen
	/// 
	////////////////////////////////////////////////////////////
	Animation& getLoadingScreen();

#pragma endregion



#pragma region TEXTURE

	////////////////////////////////////////////////////////////
	/// \brief return a ref of the texture associate to the name
	///
	/// \param name name of the texture
	/// 
	////////////////////////////////////////////////////////////
	const sf::Texture& getTexture(std::string name);

#pragma endregion


#pragma region FONT

	////////////////////////////////////////////////////////////
	/// \brief return a ref of the font associate to the name
	///
	/// \param name name of the font
	/// 
	////////////////////////////////////////////////////////////
	const sf::Font& getFont(std::string name);

#pragma endregion


#pragma region MUSIC_AND_SOUND

	sf::SoundBuffer& getSoundBuffer(std::string name);

	////////////////////////////////////////////////////////////
	/// \brief return a ref of the sound associate to the name
	///
	/// \param name name of the sound
	/// 
	////////////////////////////////////////////////////////////
	std::list<std::pair<unsigned, sf::Sound>>& getSound(std::string name);

	////////////////////////////////////////////////////////////
	/// \brief return a ref of the music associate to the name
	///
	/// \param name name of the music
	/// 
	////////////////////////////////////////////////////////////
	sf::Music& getMusic(std::string name);

	
	////////////////////////////////////////////////////////////
	/// \brief play the sound associates to the name.
	///
	/// \param name : Name of the sound/music
	/// \param
	/// \param id : With the id you can create multiple group of sound of the same sound.
	/// \param So if i have 2 open_door sound but with a different id they wont be count as the same sound.
	/// \param
	/// \param startNewSound : This parameter let you start the same sound multiple time at the same time.
	/// \param So you can combine the id with this and have forth same sound 2 of them have the same id.
	/// \param So they will be count in the same group and play at the same time.
	/// \param
	/// \param waitToFinish : This parameter will just force the sound to wait for the same sound witht the same id
	/// \param to finish. And them it will play again.
	/// \param Important to know that if startNewSound is true there is no point to use waitToFinish.
	/// \param But you can always use a different id to play the same sound but he will be block by waitToFinish.
	/// \param
	/// \param attenuation : The attenuation is how fast the sound volume 
	/// \param will go down or go up with the distance of the listener position.
	/// \param
	/// \param minDistance : The minDistance is how far we can hear the sound from the listener position.
	///
	////////////////////////////////////////////////////////////
	void playSound(std::string name, unsigned id, bool startNewSound = false, bool waitToFinish = false, float attenuation = 0.f, float minDistance = 0.f);

	////////////////////////////////////////////////////////////
	/// \brief Set the sound to a position.
	///
	/// \param name : Name of the sound/music.
	/// \param
	/// \param id : Id of the sound.
	/// \param
	/// \param position : The position where the sound is going to be set.
	/// 
	////////////////////////////////////////////////////////////
	void updateSoundPosition(std::string name, unsigned id, sf::Vector3f position);

	////////////////////////////////////////////////////////////
	/// \brief play the music associates to the name. This function check is the music is already playing to not play it twice.
	///
	/// \param name name of the sound/music
	///
	////////////////////////////////////////////////////////////
	void playMusic(std::string name);

	////////////////////////////////////////////////////////////
	/// \brief play the music associates to the name. This function check is the music is already playing to not play it twice.
	///
	/// \param name name of the sound/music
	///
	////////////////////////////////////////////////////////////
	void stopMusic(std::string name);

	////////////////////////////////////////////////////////////
	/// \brief play the music associates to the name. This function check is the music is already playing to not play it twice.
	///
	/// \param name name of the sound/music
	///
	////////////////////////////////////////////////////////////
	void stopSound(std::string name);

	////////////////////////////////////////////////////////////
	/// \brief play the music associates to the name. This function check is the music is already playing to not play it twice.
	///
	/// \param name name of the sound/music
	///
	////////////////////////////////////////////////////////////
	void stopAllMusics();

	////////////////////////////////////////////////////////////
	/// \brief play the music associates to the name. This function check is the music is already playing to not play it twice.
	///
	/// \param name name of the sound/music
	///
	////////////////////////////////////////////////////////////
	void stopAllSounds();

	////////////////////////////////////////////////////////////
	/// \brief set the volum for all the sounds already loaded
	///
	/// \param volume new volume (value between 0 - 100, default value : 50) 
	///
	////////////////////////////////////////////////////////////
	void setVolumeForAllSound(float volume);

	////////////////////////////////////////////////////////////
	/// \brief set the volum for all the musics already loaded
	///
	/// \param volume new volume (value between 0 - 100, default value : 50) 
	///
	////////////////////////////////////////////////////////////
	void setVolumeForAllMusic(float volume);

	////////////////////////////////////////////////////////////
	/// \brief set the volum of the sound associate to the name
	///
	/// \param volume new volume (value between 0 - 100, default value : 50) 
	/// \param name name of the sound
	///
	////////////////////////////////////////////////////////////
	void setVolumeForSpecificSound(std::string name, float volume);

	////////////////////////////////////////////////////////////
	/// \brief set the volum of the music associate to the name
	///
	/// \param volume new volume (value between 0 - 100, default value : 50) 
	/// \param name name of the music
	///
	////////////////////////////////////////////////////////////
	void setVolumeForSpecificMusic(std::string name, float volume);

	void setSoundPosition(std::string name, sf::Vector3f position);

	void setSoundAttenuation(std::string name, float attenuation);

	void setSoundMinDistance(std::string name, float distance);
#pragma endregion

#pragma region MOVIE

	////////////////////////////////////////////////////////////
	/// \brief return a ref of the movie associate to the name
	///
	/// \param name name of the movie
	///
	////////////////////////////////////////////////////////////
	sfe::Movie& getMovie(std::string name);

	////////////////////////////////////////////////////////////
	/// \brief play the movie associate to the name
	///
	/// \param name name of the movie
	/// \param _window window were the movie is supposed to play
	/// 
	////////////////////////////////////////////////////////////
	void playMovie(std::string name, sf::RenderWindow& _window);

	////////////////////////////////////////////////////////////
	/// \brief set the volum for all the movies already loaded
	///
	/// \param volume new volume (value between 0 - 100, default value : 50) 
	///
	////////////////////////////////////////////////////////////
	void setVolumeForAllMovie(float volume);

	////////////////////////////////////////////////////////////
	/// \brief set the volum of the movie associate to the name
	///
	/// \param volume new volume (value between 0 - 100, default value : 50) 
	/// \param name name of the movie
	///
	////////////////////////////////////////////////////////////
	void setVolumeForSpecificMovie(std::string name, float volume);

#pragma endregion



};

