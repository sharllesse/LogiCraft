#include "Serializer.h"
#include <json.hpp>

using namespace nlohmann;

bool Serializer::Parse(const std::string& path)
{
	std::ifstream file(path);
	if (file.is_open())
	{
		m_json = json::parse(file);

		return true;
	}

	return false; // Could not open file
}

void Serializer::Save(const std::string& path)
{
	std::ofstream file(path);
	if (file.is_open())
	{
		file << m_json.dump(4);
	}
}

std::vector<std::string> Serializer::GetKeys() const
{
	std::vector<std::string> keys;

	for (auto it = m_json.cbegin(); it != m_json.cend(); ++it)
		keys.emplace_back(it.key());

	return keys;
}
