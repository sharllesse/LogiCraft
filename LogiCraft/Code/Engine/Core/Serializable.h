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

#pragma once
#include "DLLExport.h"
#include "Serializer.h"

#include <atomic>
#include <mutex>

namespace Logicraft
{
class LOGI_ENGINE_API Serializable
{
public:
	Serializable()  = default;
	~Serializable() = default;

	Serializable(Serializable&&)      = delete;
	Serializable(const Serializable&) = delete;

	Serializable& operator=(Serializable&&)      = delete;
	Serializable& operator=(const Serializable&) = delete;

	void StartLoading();
	bool IsLoaded() const { return m_loaded; }

	void StartSaving();
	bool IsSaving() const { return m_isSaving; }

	void Unload() {}
	void Reload();

protected:
	// Serialize is the internal function called by Load() and Save(), it uses the already created Serializer
	virtual void Serialize(bool load, JsonObjectPtr pJsonObject) = 0;

	// Load and Save are the functions called from the top level, they create the Serializer and call Serialize
	// If a Serializable object has children to Load/Save,
	// it should call Serialize() if they are in the same file
	// and Load/Save if they have their own file
	virtual void Load() {}
	virtual void Save() {}

private:
	std::mutex        m_loadingMutex;
	std::atomic<bool> m_loaded{false};

	std::mutex        m_savingMutex;
	std::atomic<bool> m_isSaving{false};
};
} // namespace Logicraft