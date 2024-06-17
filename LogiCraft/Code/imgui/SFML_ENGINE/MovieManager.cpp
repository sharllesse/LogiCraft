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
#include "MovieManager.h"

#define EMPTY_MOVIE_NAME "placeholder"

#pragma region MOVIE

Movie::Movie()
{
	bool isSuccessful = m_movie.openFromFile("../Ressources/ALL/Movies/placeholder.mp4");
#ifdef _DEBUG
	if (isSuccessful)
		std::cout << "Movie " << "Placeholder" << " loaded\n";
	else
		std::cout << "Movie " << "Placeholder" << " can't be load\n";
#endif // !DEBUG
}

Movie::Movie(std::string path)
{
	bool isSuccessful = m_movie.openFromFile(path);
#ifdef _DEBUG
	if (isSuccessful)
		std::cout << "Movie " << path << " loaded\n";
	else
		std::cout << "Movie " << path << " can't be load\n";
#endif // !DEBUG

}

sfe::Movie& Movie::getMovie()
{
	return m_movie;
}
#pragma endregion

#pragma region MOVIE_MANAGER

MovieManager::MovieManager()
{
#ifdef _DEBUG
	std::cout << "------- Starting global movies load ------- \n";
#endif // !DEBUG

	for (auto dir : std::filesystem::recursive_directory_iterator("../Ressources/ALL/MOVIES"))
	{
		m_GlobalMovie.emplace(Tools::ToLower(dir.path().filename().stem().string()), std::move(std::make_unique<Movie>(dir.path().string())));
	}

#ifdef _DEBUG
	std::cout << "------- Global movies loading finish ------- \n\n\n";
#endif // !DEBUG
}

MovieManager::~MovieManager()
{
}



std::pair<MovieMap&, MovieMap&> MovieManager::getAllMovie()
{
	return std::pair<MovieMap&, MovieMap&>(std::ref(m_GlobalMovie),std::ref(m_SceneMovie));
}

void MovieManager::loadMovieScene(std::string _Scene)
{
#ifdef _DEBUG
	std::cout << "------- Starting " << _Scene << " movies load -------\n";
#endif // !DEBUG
	m_SceneMovie.clear();

	auto fileInfos = Tools::searchFilesInfo("../Ressources/" + _Scene + "/Movies", ".mp4");

	if (!fileInfos.empty())
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
						m_SceneMovie.emplace(it->first, std::make_unique<Movie>(it->second));
					}

				}));
		}

		for (auto& i : listThread)
			i.join();
	}

#ifdef _DEBUG
	std::cout << "------- " << _Scene << " movies loading finish -------\n\n\n";
#endif // !DEBUG

}

sfe::Movie& MovieManager::getMovie(std::string name)
{
	name = Tools::ToLower(name);
	if (m_GlobalMovie[name])
		return m_GlobalMovie[name].get()->getMovie();
	else if (m_SceneMovie[name])
		return m_SceneMovie[name].get()->getMovie();
	else
	{
		m_GlobalMovie.erase(name);
		m_SceneMovie.erase(name);
#ifdef _DEBUG
		std::cout << "Cannot find the movie \"" << name << "\"\n";
#endif // !DEBUG
		return m_GlobalMovie[EMPTY_MOVIE_NAME].get()->getMovie();
	}
}

#pragma endregion


