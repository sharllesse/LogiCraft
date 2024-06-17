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

class Movie
{
	sfe::Movie m_movie;
public:
	Movie();
	Movie(std::string path);

	sfe::Movie& getMovie();
};

typedef std::unordered_map<std::string, std::unique_ptr<Movie>> MovieMap;

class MovieManager
{
	MovieMap m_GlobalMovie;
	MovieMap m_SceneMovie;
public:
	MovieManager();
	~MovieManager();

	MovieManager(const MovieManager&) = delete;
	MovieManager& operator=(const MovieManager&) = delete;

	std::pair<MovieMap&,MovieMap&> getAllMovie();

	void loadMovieScene(std::string _Scene);
	sfe::Movie& getMovie(std::string name);
};

