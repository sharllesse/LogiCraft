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

	Serializer& operator=(const Serializer& other);
	Serializer& operator=(Serializer&& other);

	json& operator[](const std::string& key) { return m_json[key]; }

	bool operator!=(const Serializer& other) const { return m_json != other.m_json; }
	bool operator==(const Serializer& other) const { return m_json == other.m_json; }
	
	bool Parse(const std::string& path);
	void Save(const std::string& path);

	/*
	 * @brief Return the iterator to the beginning of the json objects
	 */
	json::iterator Begin() { return m_json.begin(); }

	/*
	 * @brief Return the iterator to the end of the json objects
	 */
	json::iterator End() { return m_json.end(); }

	/*
	 * @brief Return the const iterator to the beginning of the json objects
	 */
	json::const_iterator ConstBegin() const { return m_json.cbegin(); }
	/*
	 * @brief Return the const iterator to the end of the json objects
	 */
	json::const_iterator ConstEnd() const { return m_json.cend(); }

	/*
	 * @brief Erase the iterator from the json objects
	 */
	json::iterator Erase(const json::iterator& it) { return m_json.erase(it); }

	/*
	 * @brief Remove the iterator from the json objects
	 */
	void Remove(const std::string& key) { m_json.erase(key); }

	/*
	 * @brief Swap the json objects with another json object
	 */
	void Swap(json& other) { m_json.swap(other); }

	json::iterator Insert(const json::const_iterator& it, const json& value) { return m_json.insert(it, value); }
	json::iterator Insert(const json::const_iterator& it, json&& value) { return m_json.insert(it, std::move(value)); }
	json::iterator Insert(const json::const_iterator& it, size_t count, const json& value) { return m_json.insert(it, count, value); }

	//void Emplace(const std::string& key, const json& value) { m_json.emplace(key, value); }
	//void EmplaceBack(const std::string& key, const json& value) { m_json.emplace_back(key, value); }

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

private:
	json m_json;
};
