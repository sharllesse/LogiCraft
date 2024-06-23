/*------------------------------------LICENSE------------------------------------
MIT License

Copyright (c) 2024 CIRON Robin
Copyright (c) 2024 GRALLAN Yann
Copyright (c) 2024 LESAGE Charles
Copyright (c) 2024 MENA-BOUR Samy

This software utilizes code from the following GitHub repositories, which are also licensed under the MIT License:

- [SFML](https://github.com/SFML)
- [ImGUI](https://github.com/ocornut/imgui)
- [ImNodes](https://github.com/Nelarius/imnodes)
- [SFML-Manager](https://github.com/Xanhos/Cpp-Manager-for-SFML)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
---------------------------------------------------------------------------------*/

#include "Logger.h"

#include <assert.h>
#include <iostream>

using namespace Logicraft;

Logger* s_pLogger = nullptr;

Logger& Logger::Get()
{
	assert(s_pLogger);
	return *s_pLogger;
}
Logger::Logger()
{
	assert(!s_pLogger);
	s_pLogger = this;
}

Logger::~Logger()
{
	s_pLogger = nullptr;
}

void Logger::Log(ELogLevel level, const std::string& message)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	switch (level)
	{
	case ELogLevel::eInfo:
		{
			std::cout << "INFO: " << message << std::endl;
		}
		break;
	case ELogLevel::eWarning:
		{
			std::cout << "WARNING: " << message << std::endl;
		}
		break;
	case ELogLevel::eError:
		{
			std::cout << "ERROR: " << message << std::endl;
		}
		break;
	}
}
