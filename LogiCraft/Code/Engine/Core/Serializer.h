#pragma once
#include "DLLExport.h"
#include "json.hpp"

#include <fstream>
#include <string>

using namespace nlohmann;

class Serializer
{
public:
	Serializer() = default;
	Serializer(Serializer&& other);
	~Serializer() = default;

	bool Parse(const std::string& filename);

	json::iterator Begin() { return m_json.begin(); }
	json::iterator End() { return m_json.end(); }

	json::const_iterator ConstBegin() const { return m_json.cbegin(); }
	json::const_iterator ConstEnd() const { return m_json.cend(); }

	json::iterator Erase(const json::iterator& it) { return m_json.erase(it); }

	void Remove(const std::string& key) { m_json.erase(key); }

	void Swap(json& other) { m_json.swap(other); }

	json::iterator Insert(const json::const_iterator& it, const json& value) { return m_json.insert(it, value); }
	json::iterator Insert(const json::const_iterator& it, json&& value) { return m_json.insert(it, std::move(value)); }
	json::iterator Insert(const json::const_iterator& it, size_t count, const json& value) { return m_json.insert(it, count, value); }

	json::iterator       Find(const std::string& key) { return m_json.find(key); }
	json::const_iterator Find(const std::string& key) const { return m_json.find(key); }

	template<typename T>
	T At(const std::string& key)
	{
		return m_json.at(key).get<T>();
	}

	template<>
	json At<json>(const std::string& key)
	{
		return m_json.at(key);
	}

	bool Contains(const std::string& key) const { return m_json.contains(key); }

	size_t Size() const { return m_json.size(); }   // no Discard ?
	bool   Empty() const { return m_json.empty(); } // Same here

	json& GetJson() { return m_json; }

	std::vector<std::string> GetKeys() const;

	json& operator[](const std::string& key) { return m_json[key]; }

	bool        operator!=(const Serializer& other) const { return m_json != other.m_json; }
	bool        operator==(const Serializer& other) const { return m_json == other.m_json; }
	Serializer& operator=(const Serializer& other);
	Serializer& operator=(Serializer&& other);

private:
	json m_json;
};
