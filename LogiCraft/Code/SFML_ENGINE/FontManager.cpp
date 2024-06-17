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
#include "FontManager.h"

#define EMPTY_FONT_NAME "placeholder"

#pragma region FONT
Font::Font()
{
	bool isSuccessful = m_font.loadFromFile("../Ressources/ALL/Fonts/arial.ttf");
#ifdef _DEBUG
	if (isSuccessful)
		std::cout << "Font Placeholder loaded\n";
	else
		std::cout << "Font Placeholder can't be load\n";
#endif // !DEBUG
}

Font::Font(std::string path)
{
	bool isSuccessful = m_font.loadFromFile(path);
#ifdef _DEBUG
	if (isSuccessful)
		std::cout << "Font " << path << " loaded\n";
	else
		std::cout << "Font " << path << " can't be load\n";
#endif // !DEBUG
}


#pragma region FONT_MANAGER


Font::~Font()
{
}

sf::Font& Font::getFont()
{
	return m_font;
}

#pragma endregion

FontManager::FontManager()
{
#ifdef _DEBUG
	std::cout << "------- Starting global fonts load -------\n";
#endif // !DEBUG
	for (auto dir : std::filesystem::recursive_directory_iterator("../Ressources/ALL/Fonts"))
	{
		m_GlobalFont.emplace(Tools::ToLower(dir.path().filename().stem().string()), std::make_unique<Font>(dir.path().string()));
	}
#ifdef _DEBUG
	std::cout << "------- Global fonts loading finish -------\n\n\n";
#endif // !DEBUG
}

FontManager::~FontManager()
{
}

void FontManager::loadFontScene(std::string _Scene)
{
#ifdef _DEBUG
	std::cout << "------- Starting " << _Scene << " fonts load -------\n";
#endif // !DEBUG
	m_SceneFont.clear();

	auto fileInfos = Tools::searchFilesInfo("../Ressources/" + _Scene + "/Fonts", ".ttf");

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
						m_SceneFont.emplace(it->first, std::make_unique<Font>(it->second));
					}

				}));
		}

		for (auto& i : listThread)
			i.join();
	}

#ifdef _DEBUG
	std::cout << "------- " << _Scene << " fonts loading finish -------\n\n\n";
#endif // !DEBUG

}

sf::Font& FontManager::getFont(std::string name)
{
	name = Tools::ToLower(name);
	if (m_GlobalFont[name])
		return m_GlobalFont[name].get()->getFont();
	else if (m_SceneFont[name])
		return m_SceneFont[name].get()->getFont();
	else
	{
		m_GlobalFont.erase(name);
		m_SceneFont.erase(name);
#ifdef _DEBUG
		std::cout << "Cannot find the font \"" << name << "\"\n";
#endif // !DEBUG
		return m_GlobalFont[EMPTY_FONT_NAME].get()->getFont();
	}
}

#pragma endregion

