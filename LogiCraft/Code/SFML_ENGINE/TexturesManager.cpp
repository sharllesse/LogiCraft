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
#include "TexturesManager.h"

#define EMPTY_TEXTURE_NAME "placeholder"

#pragma region TEXTURES

Textures::Textures()
{
	bool isSuccessful = m_texture.loadFromFile("../Ressources/ALL/Textures/Placeholder.png");
#ifdef _DEBUG
	if (isSuccessful)
		std::cout << "Textures Placeholder loaded\n";
	else
		std::cout << "Textures Placeholder can't be load\n";
#endif // !DEBUG
}

Textures::Textures(std::string path)
{
	bool isSuccessful = m_texture.loadFromFile(path);
#ifdef _DEBUG
	if (isSuccessful)
		std::cout << "Textures " << path << " loaded\n";
	else
		std::cout << "Textures " << path << " can't be load\n";
#endif // !DEBUG
}

Textures::~Textures()
{
}

sf::Texture& Textures::getTexture()
{
	return m_texture;
}
#pragma endregion


#pragma region TEXTURES_MANAGER

TexturesManager::TexturesManager()
{
#ifdef _DEBUG
	std::cout << "------- Starting global textures load -------\n";
#endif // !DEBUG
	for (auto dir : std::filesystem::recursive_directory_iterator("../Ressources/ALL/Textures"))
	{
		m_GlobalTextures.emplace(Tools::ToLower(dir.path().filename().stem().string()), std::make_unique<Textures>(dir.path().string()));
	}
#ifdef _DEBUG
	std::cout << "------- Global textures loading finish -------\n\n\n";
#endif // !DEBUG
}



TexturesManager::~TexturesManager()
{
}

void TexturesManager::loadSceneResources(std::string _Scene)
{
#ifdef _DEBUG
	std::cout << "------- Starting " << _Scene << " textures load -------\n";
#endif // !DEBUG
	m_SceneTextures.clear();

	auto fileInfos = Tools::searchFilesInfo("../Ressources/" + _Scene + "/Textures",".png");
	if(!fileInfos.empty())
	{

		int nbrThread = (static_cast<int>(fileInfos.size()) < MAX_THREAD ? static_cast<int>(fileInfos.size()) : MAX_THREAD);
		int block_size = static_cast<int>(fileInfos.size()) / nbrThread;

		std::list<std::thread> listThread;


		for (int i = 0; i < nbrThread; i++)
		{
			int start = i * block_size;
			int end = (i == nbrThread - 1) ? static_cast<int>(fileInfos.size()) : (i + 1) * block_size;

			listThread.push_back(std::thread([start, end, &fileInfos, this]
				{
					auto itBegin = std::next(fileInfos.begin(), start);
					auto itEnd = std::next(fileInfos.begin(), end);

					for (auto it = itBegin; it != itEnd; it++)
					{
						m_SceneTextures.emplace(it->first, std::make_unique<Textures>(it->second));
					}

				}));
		}

		for (auto& i : listThread)
			i.join();
	}

#ifdef _DEBUG
	std::cout << "------- " << _Scene << " textures loading finish -------\n\n\n";
#endif // !DEBUG
}

const sf::Texture& TexturesManager::getTexture(std::string name)
{
	name = Tools::ToLower(name);
	if (m_GlobalTextures[name])
		return m_GlobalTextures[name].get()->getTexture();
	else if (m_SceneTextures[name])
		return m_SceneTextures[name].get()->getTexture();
	else
	{
		m_GlobalTextures.erase(name);
		m_SceneTextures.erase(name);
#ifdef _DEBUG
		std::cout << "Cannot find the textures \"" << name << "\"\n";
#endif // !DEBUG
		return m_GlobalTextures[EMPTY_TEXTURE_NAME].get()->getTexture();
	}
}



#pragma endregion

