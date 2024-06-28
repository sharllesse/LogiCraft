#include "Serializer.h"

#include <json/document.h>
#include <json/prettywriter.h>
#include <json/stringbuffer.h>

using namespace rapidjson;

// #include <json.hpp>
//
//
// using namespace nlohmann;
//
// class Serializer
//{
// public:
//	Serializer()  = default;
//	~Serializer() = default;
//
//	Serializer(const Serializer& other)            = delete;
//	Serializer(Serializer&& other)                 = delete;
//	Serializer& operator=(const Serializer& other) = delete;
//	Serializer& operator=(Serializer&& other)      = delete;
//
//	json& operator[](const std::string& key) { return m_json[key]; }
//
//	bool operator!=(const Serializer& other) const { return m_json != other.m_json; }
//	bool operator==(const Serializer& other) const { return m_json == other.m_json; }
//
//	bool Parse(const std::string& path);
//	void Save(const std::string& path);
//
//	/*
//	 * @brief Return the iterator to the beginning of the json objects
//	 */
//	json::iterator Begin() { return m_json.begin(); }
//
//	/*
//	 * @brief Return the iterator to the end of the json objects
//	 */
//	json::iterator End() { return m_json.end(); }
//
//	/*
//	 * @brief Return the const iterator to the beginning of the json objects
//	 */
//	json::const_iterator ConstBegin() const { return m_json.cbegin(); }
//	/*
//	 * @brief Return the const iterator to the end of the json objects
//	 */
//	json::const_iterator ConstEnd() const { return m_json.cend(); }
//
//	/*
//	 * @brief Erase the iterator from the json objects
//	 */
//	json::iterator Erase(const json::iterator& it) { return m_json.erase(it); }
//
//	/*
//	 * @brief Remove the iterator from the json objects
//	 */
//	void Remove(const std::string& key) { m_json.erase(key); }
//
//	/*
//	 * @brief Swap the json objects with another json object
//	 */
//	void Swap(json& other) { m_json.swap(other); }
//
//	json::iterator Insert(const json::const_iterator& it, const json& value) { return m_json.insert(it, value); }
//	json::iterator Insert(const json::const_iterator& it, json&& value) { return m_json.insert(it, std::move(value)); }
//	json::iterator Insert(const json::const_iterator& it, size_t count, const json& value) { return m_json.insert(it, count, value); }
//
//	// void Emplace(const std::string& key, const json& value) { m_json.emplace(key, value); }
//	// void EmplaceBack(const std::string& key, const json& value) { m_json.emplace_back(key, value); }
//
//	json::iterator       Find(const std::string& key) { return m_json.find(key); }
//	json::const_iterator Find(const std::string& key) const { return m_json.find(key); }
//
//	template<typename T>
//	T At(const std::string& key)
//	{
//		return m_json.at(key).get<T>();
//	}
//
//	template<>
//	json At<json>(const std::string& key)
//	{
//		return m_json.at(key);
//	}
//
//	bool Contains(const std::string& key) const { return m_json.contains(key); }
//
//	size_t Size() const { return m_json.size(); }
//	bool   Empty() const { return m_json.empty(); }
//
//	json& GetJson() { return m_json; }
//
//	std::vector<std::string> GetKeys() const;
//
// private:
//	json m_json;
// };
//
// bool Serializer::Parse(const std::string& path)
//{
//	std::ifstream file(path);
//	if (file.is_open())
//	{
//		m_json = json::parse(file);
//
//		return true;
//	}
//
//	return false; // Could not open file
// }
//
// void Serializer::Save(const std::string& path)
//{
//	std::ofstream file(path);
//	if (file.is_open())
//	{
//		file << m_json.dump(4);
//	}
// }
//
// std::vector<std::string> Serializer::GetKeys() const
//{
//	std::vector<std::string> keys;
//
//	for (auto it = m_json.cbegin(); it != m_json.cend(); ++it)
//		keys.emplace_back(it.key());
//
//	return keys;
// }

// struct JsonObject_Private
//{
//	Document::AllocatorType* pAllocator = nullptr;
//	Value* pValue = nullptr;
//	Value* pKey   = nullptr;
//	bool isAllocated = false;
// };
//
// struct JsonArray_Private
//{
//	Document::AllocatorType* pAllocator = nullptr;
//	Value* pValue = nullptr;
//	Value* pKey = nullptr;
//	bool isAllocated = false;
// };

struct Json_Private
{
	Document::AllocatorType* pAllocator  = nullptr;
	Value*                   pValue      = nullptr;
	Value*                   pKey        = nullptr;
	bool                     isAllocated = false;
};

JsonArray::JsonArray()
{
	m_jsonPrivate = new Json_Private();
}

JsonArray::JsonArray(const char* key, const JsonObjectPtr& root)
{
	m_jsonPrivate              = new Json_Private();
	m_jsonPrivate->isAllocated = true;
	m_jsonPrivate->pAllocator  = root->GetPrivate()->pAllocator;
	m_jsonPrivate->pValue      = new Value(kArrayType);
	m_jsonPrivate->pKey        = new Value(key, *m_jsonPrivate->pAllocator);
}

JsonArray::~JsonArray()
{
	if (m_jsonPrivate)
	{
		if (m_jsonPrivate->isAllocated)
		{
			delete m_jsonPrivate->pKey;
			delete m_jsonPrivate->pValue;
		}

		delete m_jsonPrivate;
	}

	m_jsonPrivate = nullptr;
}

const char* JsonArray::GetKey() const
{
	if (m_jsonPrivate->pKey)
	{
		return m_jsonPrivate->pKey->GetString();
	}

	return nullptr;
}

void JsonArray::PushBack(const JsonObjectPtr& value) const
{
	if (m_jsonPrivate->pValue->IsArray())
		m_jsonPrivate->pValue->PushBack(*value->GetPrivate()->pValue, *m_jsonPrivate->pAllocator);
}

void JsonArray::PushBack(const JsonArrayPtr& value) const
{
	if (m_jsonPrivate->pValue->IsArray())
		m_jsonPrivate->pValue->PushBack(*value->GetPrivate()->pValue, *m_jsonPrivate->pAllocator);
}

void JsonArray::PushBack(const std::string& value) const
{
	if (m_jsonPrivate->pValue->IsArray())
		m_jsonPrivate->pValue->PushBack(value, *m_jsonPrivate->pAllocator);
}

void JsonArray::PushBack(const char* value) const
{
	if (m_jsonPrivate->pValue->IsArray())
		m_jsonPrivate->pValue->PushBack(Value(value, *m_jsonPrivate->pAllocator), *m_jsonPrivate->pAllocator);
}

void JsonArray::PushBack(const int& value) const
{
	if (m_jsonPrivate->pValue->IsArray())
		m_jsonPrivate->pValue->PushBack(value, *m_jsonPrivate->pAllocator);
}

void JsonArray::PushBack(const double& value) const
{
	if (m_jsonPrivate->pValue->IsArray())
		m_jsonPrivate->pValue->PushBack(value, *m_jsonPrivate->pAllocator);
}

void JsonArray::PushBack(const float& value) const
{
	if (m_jsonPrivate->pValue->IsArray())
		m_jsonPrivate->pValue->PushBack(value, *m_jsonPrivate->pAllocator);
}

void JsonArray::PushBack(const bool& value) const
{
	if (m_jsonPrivate->pValue->IsArray())
		m_jsonPrivate->pValue->PushBack(value, *m_jsonPrivate->pAllocator);
}

void JsonArray::PopBack() const
{
	if (m_jsonPrivate->pValue->IsArray())
		m_jsonPrivate->pValue->PopBack();
}

void JsonArray::Clear() const
{
	if (m_jsonPrivate->pValue->IsArray())
		m_jsonPrivate->pValue->Clear();
}

size_t JsonArray::Size() const
{
	if (m_jsonPrivate->pValue->IsArray())
		return m_jsonPrivate->pValue->Size();

	return 0u;
}

bool JsonArray::Empty() const
{
	if (m_jsonPrivate->pValue->IsArray())
		return m_jsonPrivate->pValue->Empty();

	return true;
}

void JsonArray::ForEach(const std::function<void(const JsonObjectPtr&, const bool&)>& function) const
{
	if (m_jsonPrivate->pValue->IsArray())
	{
		const JsonObjectPtr pObject = std::make_shared<JsonObject>();
		for (auto it = m_jsonPrivate->pValue->Begin(); it != m_jsonPrivate->pValue->End();)
		{
			bool needToBeErase(false);
			pObject->GetPrivate()->pValue     = it;
			pObject->GetPrivate()->pKey       = m_jsonPrivate->pKey;
			pObject->GetPrivate()->pAllocator = m_jsonPrivate->pAllocator;
			function(pObject, needToBeErase);

			if (needToBeErase)
				it = m_jsonPrivate->pValue->Erase(it);
			else
				it++;
		}
	}
}

Json_Private* JsonArray::GetPrivate() const
{
	return m_jsonPrivate;
}

JsonObject::JsonObject()
{
	m_jsonPrivate = new Json_Private();
}

JsonObject::JsonObject(const char* key, const JsonObjectPtr& root)
{
	m_jsonPrivate              = new Json_Private();
	m_jsonPrivate->isAllocated = true;
	m_jsonPrivate->pAllocator  = root->m_jsonPrivate->pAllocator;
	m_jsonPrivate->pKey        = new Value(key, *m_jsonPrivate->pAllocator);
	m_jsonPrivate->pValue      = new Value(kObjectType);
}

JsonObject::JsonObject(const char* key, const std::string& value, const JsonObjectPtr& root)
{
	m_jsonPrivate              = new Json_Private();
	m_jsonPrivate->isAllocated = true;
	m_jsonPrivate->pAllocator  = root->m_jsonPrivate->pAllocator;
	m_jsonPrivate->pKey        = new Value(key, *m_jsonPrivate->pAllocator);
	m_jsonPrivate->pValue      = new Value(value.c_str(), *m_jsonPrivate->pAllocator);
}

JsonObject::JsonObject(const char* key, const char* value, const JsonObjectPtr& root)
{
	m_jsonPrivate              = new Json_Private();
	m_jsonPrivate->isAllocated = true;
	m_jsonPrivate->pAllocator  = root->m_jsonPrivate->pAllocator;
	m_jsonPrivate->pKey        = new Value(key, *m_jsonPrivate->pAllocator);
	m_jsonPrivate->pValue      = new Value(value, *m_jsonPrivate->pAllocator);
}

JsonObject::JsonObject(const char* key, const int& value, const JsonObjectPtr& root)
{
	m_jsonPrivate              = new Json_Private();
	m_jsonPrivate->isAllocated = true;
	m_jsonPrivate->pAllocator  = root->m_jsonPrivate->pAllocator;
	m_jsonPrivate->pKey        = new Value(key, *m_jsonPrivate->pAllocator);
	m_jsonPrivate->pValue      = new Value(value);
}

JsonObject::JsonObject(const char* key, const bool& value, const JsonObjectPtr& root)
{
	m_jsonPrivate              = new Json_Private();
	m_jsonPrivate->isAllocated = true;
	m_jsonPrivate->pAllocator  = root->m_jsonPrivate->pAllocator;
	m_jsonPrivate->pKey        = new Value(key, *m_jsonPrivate->pAllocator);
	m_jsonPrivate->pValue      = new Value(value);
}

JsonObject::JsonObject(const char* key, const float& value, const JsonObjectPtr& root)
{
	m_jsonPrivate              = new Json_Private();
	m_jsonPrivate->isAllocated = true;
	m_jsonPrivate->pAllocator  = root->m_jsonPrivate->pAllocator;
	m_jsonPrivate->pKey        = new Value(key, *m_jsonPrivate->pAllocator);
	m_jsonPrivate->pValue      = new Value(value);
}

JsonObject::JsonObject(const char* key, const double& value, const JsonObjectPtr& root)
{
	m_jsonPrivate              = new Json_Private();
	m_jsonPrivate->isAllocated = true;
	m_jsonPrivate->pAllocator  = root->m_jsonPrivate->pAllocator;
	m_jsonPrivate->pKey        = new Value(key, *m_jsonPrivate->pAllocator);
	m_jsonPrivate->pValue      = new Value(value);
}

JsonObject::~JsonObject()
{
	if (m_jsonPrivate)
	{
		if (m_jsonPrivate->isAllocated)
		{
			delete m_jsonPrivate->pKey;
			delete m_jsonPrivate->pValue;
		}

		delete m_jsonPrivate;
	}

	m_jsonPrivate = nullptr;
}

JsonObjectPtr JsonObject::GetObject(const char* key) const
{
	if (m_jsonPrivate->pValue->IsObject())
	{
		const Value::MemberIterator& memberIterator = m_jsonPrivate->pValue->FindMember(key);
		if (memberIterator != m_jsonPrivate->pValue->MemberEnd())
		{
			const auto object = std::make_shared<JsonObject>();

			object->GetPrivate()->pValue     = &memberIterator->value;
			object->GetPrivate()->pKey       = &memberIterator->name;
			object->GetPrivate()->pAllocator = m_jsonPrivate->pAllocator;

			return object;
		}

		return nullptr;
	}

	return nullptr;
}

bool JsonObject::GetObject(const char* key, JsonObjectPtr& object) const
{
	if (m_jsonPrivate->pValue->IsObject())
	{
		const Value::MemberIterator& memberIterator = m_jsonPrivate->pValue->FindMember(key);
		if (memberIterator != m_jsonPrivate->pValue->MemberEnd())
		{
			if (!object)
				object = std::make_shared<JsonObject>();

			object->GetPrivate()->pValue     = &memberIterator->value;
			object->GetPrivate()->pKey       = &memberIterator->name;
			object->GetPrivate()->pAllocator = m_jsonPrivate->pAllocator;

			return true;
		}

		return false;
	}

	return false;
}

StrPtr JsonObject::GetString(const char* key) const
{
	if (m_jsonPrivate->pValue->IsObject())
	{
		const auto memberIterator = m_jsonPrivate->pValue->FindMember(key);
		if (memberIterator != m_jsonPrivate->pValue->MemberEnd())
		{
			const Value& value = memberIterator->value;

			if (value.IsString())
				return std::make_shared<std::string>(value.GetString());
		}

		return nullptr;
	}

	return nullptr;
}

bool JsonObject::GetString(const char* key, std::string& rString) const
{
	if (m_jsonPrivate->pValue->IsObject())
	{
		const auto memberIterator = m_jsonPrivate->pValue->FindMember(key);
		if (memberIterator != m_jsonPrivate->pValue->MemberEnd())
		{
			const Value& value = memberIterator->value;

			if (value.IsString())
			{
				rString = value.GetString();
				return true;
			}
		}

		return false;
	}

	return false;
}

IntPtr JsonObject::GetInt(const char* key) const
{
	if (m_jsonPrivate->pValue->IsObject())
	{
		const auto memberIterator = m_jsonPrivate->pValue->FindMember(key);
		if (memberIterator != m_jsonPrivate->pValue->MemberEnd())
		{
			const Value& value = memberIterator->value;

			if (value.IsInt())
				return std::make_shared<int>(value.GetInt());
		}

		return nullptr;
	}

	return nullptr;
}

bool JsonObject::GetInt(const char* key, int& rInt) const
{
	if (m_jsonPrivate->pValue->IsObject())
	{
		const auto memberIterator = m_jsonPrivate->pValue->FindMember(key);
		if (memberIterator != m_jsonPrivate->pValue->MemberEnd())
		{
			const Value& value = memberIterator->value;

			if (value.IsInt())
			{
				rInt = value.GetInt();
				return true;
			}
		}

		return false;
	}

	return false;
}

BoolPtr JsonObject::GetBool(const char* key) const
{
	if (m_jsonPrivate->pValue->IsObject())
	{
		const auto memberIterator = m_jsonPrivate->pValue->FindMember(key);
		if (memberIterator != m_jsonPrivate->pValue->MemberEnd())
		{
			const Value& value = memberIterator->value;

			if (value.IsBool())
				return std::make_shared<bool>(value.GetBool());
		}

		return nullptr;
	}

	return nullptr;
}

bool JsonObject::GetBool(const char* key, bool& rBool) const
{
	if (m_jsonPrivate->pValue->IsObject())
	{
		const auto memberIterator = m_jsonPrivate->pValue->FindMember(key);
		if (memberIterator != m_jsonPrivate->pValue->MemberEnd())
		{
			const Value& value = memberIterator->value;

			if (value.IsBool())
			{
				rBool = value.GetBool();
				return true;
			}
		}

		return false;
	}

	return false;
}

FloatPtr JsonObject::GetFloat(const char* key) const
{
	if (m_jsonPrivate->pValue->IsObject())
	{
		const auto memberIterator = m_jsonPrivate->pValue->FindMember(key);
		if (memberIterator != m_jsonPrivate->pValue->MemberEnd())
		{
			const Value& value = memberIterator->value;

			if (value.IsFloat())
				return std::make_shared<float>(value.GetFloat());
		}

		return nullptr;
	}

	return nullptr;
}

bool JsonObject::GetFloat(const char* key, float& rFloat) const
{
	if (m_jsonPrivate->pValue->IsObject())
	{
		const auto memberIterator = m_jsonPrivate->pValue->FindMember(key);
		if (memberIterator != m_jsonPrivate->pValue->MemberEnd())
		{
			const Value& value = memberIterator->value;

			if (value.IsFloat())
			{
				rFloat = value.GetFloat();
				return true;
			}
		}

		return false;
	}

	return false;
}

DoublePtr JsonObject::GetDouble(const char* key) const
{
	if (m_jsonPrivate->pValue->IsObject())
	{
		const auto memberIterator = m_jsonPrivate->pValue->FindMember(key);
		if (memberIterator != m_jsonPrivate->pValue->MemberEnd())
		{
			const Value& value = memberIterator->value;

			if (value.IsDouble())
				return std::make_shared<double>(value.GetDouble());
		}

		return nullptr;
	}

	return nullptr;
}

bool JsonObject::GetDouble(const char* key, double& rDouble) const
{
	if (m_jsonPrivate->pValue->IsObject())
	{
		const auto memberIterator = m_jsonPrivate->pValue->FindMember(key);
		if (memberIterator != m_jsonPrivate->pValue->MemberEnd())
		{
			const Value& value = memberIterator->value;

			if (value.IsDouble())
			{
				rDouble = value.GetDouble();
				return true;
			}
		}

		return false;
	}

	return false;
}

JsonArrayPtr JsonObject::GetArray(const char* key) const
{
	if (m_jsonPrivate->pValue->IsObject())
	{
		const auto memberIterator = m_jsonPrivate->pValue->FindMember(key);
		if (memberIterator != m_jsonPrivate->pValue->MemberEnd())
		{
			if (memberIterator->value.IsArray())
			{
				const JsonArrayPtr array        = std::make_shared<JsonArray>();
				array->GetPrivate()->pValue     = &memberIterator->value;
				array->GetPrivate()->pKey       = &memberIterator->name;
				array->GetPrivate()->pAllocator = m_jsonPrivate->pAllocator;
				return array;
			}
		}

		return nullptr;
	}

	return nullptr;
}

bool JsonObject::GetArray(const char* key, JsonArrayPtr& rArray) const
{
	if (m_jsonPrivate->pValue->IsObject())
	{
		const auto memberIterator = m_jsonPrivate->pValue->FindMember(key);
		if (memberIterator != m_jsonPrivate->pValue->MemberEnd())
		{
			if (memberIterator->value.IsArray())
			{
				rArray->GetPrivate()->pValue     = &memberIterator->value;
				rArray->GetPrivate()->pKey       = &memberIterator->name;
				rArray->GetPrivate()->pAllocator = m_jsonPrivate->pAllocator;
				return true;
			}
		}

		return false;
	}

	return false;
}

const char* JsonObject::GetKey() const
{
	if (m_jsonPrivate->pKey)
	{
		return m_jsonPrivate->pKey->GetString();
	}

	return nullptr;
}

JsonObjectPtr JsonObject::AddObject(const char* key) const
{
	if (m_jsonPrivate->pValue->IsObject())
	{
		Value newKey(key, *m_jsonPrivate->pAllocator);
		Value newObject(kObjectType);
		m_jsonPrivate->pValue->AddMember(newKey, newObject, *m_jsonPrivate->pAllocator);
		const auto iteratorMember =
		  m_jsonPrivate->pValue->FindMember(key); // Need to do this because the flag is remove when the value is added to the object ???

		const auto objectPtr                = std::make_shared<JsonObject>();
		objectPtr->GetPrivate()->pAllocator = m_jsonPrivate->pAllocator;
		objectPtr->GetPrivate()->pValue     = &iteratorMember->value;
		objectPtr->GetPrivate()->pKey       = &iteratorMember->name;

		return objectPtr;
	}

	return nullptr;
}

JsonObjectPtr JsonObject::AddObject(const char* key, const JsonObjectPtr& object) const
{
	if (m_jsonPrivate->pValue->IsObject())
	{
		Value newKey(key, *m_jsonPrivate->pAllocator);
		m_jsonPrivate->pValue->AddMember(newKey, *object->GetPrivate()->pValue, *m_jsonPrivate->pAllocator);
		const auto iteratorMember = m_jsonPrivate->pValue->FindMember(key);

		const auto objectPtr                = std::make_shared<JsonObject>();
		objectPtr->GetPrivate()->pAllocator = m_jsonPrivate->pAllocator;
		objectPtr->GetPrivate()->pValue     = &iteratorMember->value;
		objectPtr->GetPrivate()->pKey       = &iteratorMember->name;

		return objectPtr;
	}

	return nullptr;
}

JsonObjectPtr JsonObject::AddObject(const JsonObjectPtr& object) const
{
	if (m_jsonPrivate->pValue->IsObject())
	{
		Value newKey(object->GetKey(), *m_jsonPrivate->pAllocator);
		m_jsonPrivate->pValue->AddMember(newKey, *object->GetPrivate()->pValue, *m_jsonPrivate->pAllocator);
		const auto iteratorMember = m_jsonPrivate->pValue->FindMember(object->GetKey());

		const auto objectPtr                = std::make_shared<JsonObject>();
		objectPtr->GetPrivate()->pAllocator = m_jsonPrivate->pAllocator;
		objectPtr->GetPrivate()->pValue     = &iteratorMember->value;
		objectPtr->GetPrivate()->pKey       = &iteratorMember->name;

		return objectPtr;
	}

	return nullptr;
}

StrPtr JsonObject::AddString(const char* key, const char* value) const
{
	if (m_jsonPrivate->pValue->IsObject())
	{
		Value newKey(key, *m_jsonPrivate->pAllocator);
		Value newObject(value, *m_jsonPrivate->pAllocator);
		m_jsonPrivate->pValue->AddMember(newKey, newObject, *m_jsonPrivate->pAllocator);

		return std::make_shared<std::string>(value);
	}

	return nullptr;
}

StrPtr JsonObject::AddString(const char* key, const std::string& value) const
{
	if (m_jsonPrivate->pValue->IsObject())
	{
		Value newKey(key, *m_jsonPrivate->pAllocator);
		Value newObject(value.c_str(), *m_jsonPrivate->pAllocator);
		m_jsonPrivate->pValue->AddMember(newKey, newObject, *m_jsonPrivate->pAllocator);

		return std::make_shared<std::string>(value);
	}

	return nullptr;
}

IntPtr JsonObject::AddInt(const char* key, const int& value) const
{
	if (m_jsonPrivate->pValue->IsObject())
	{
		Value newKey(key, *m_jsonPrivate->pAllocator);
		Value newObject(value);
		m_jsonPrivate->pValue->AddMember(newKey, newObject, *m_jsonPrivate->pAllocator);

		return std::make_shared<int>(value);
	}

	return nullptr;
}

BoolPtr JsonObject::AddBool(const char* key, const bool& value) const
{
	if (m_jsonPrivate->pValue->IsObject())
	{
		Value newKey(key, *m_jsonPrivate->pAllocator);
		Value newObject(value);
		m_jsonPrivate->pValue->AddMember(newKey, newObject, *m_jsonPrivate->pAllocator);

		return std::make_shared<bool>(value);
	}

	return nullptr;
}

FloatPtr JsonObject::AddFloat(const char* key, const float& value) const
{
	if (m_jsonPrivate->pValue->IsObject())
	{
		Value newKey(key, *m_jsonPrivate->pAllocator);
		Value newObject(value);
		m_jsonPrivate->pValue->AddMember(newKey, newObject, *m_jsonPrivate->pAllocator);

		return std::make_shared<float>(value);
	}

	return nullptr;
}

DoublePtr JsonObject::AddDouble(const char* key, const double& value) const
{
	if (m_jsonPrivate->pValue->IsObject())
	{
		Value newKey(key, *m_jsonPrivate->pAllocator);
		Value newObject(value);
		m_jsonPrivate->pValue->AddMember(newKey, newObject, *m_jsonPrivate->pAllocator);

		return std::make_shared<double>(value);
	}

	return nullptr;
}

JsonArrayPtr JsonObject::AddArray(const char* key) const
{
	if (m_jsonPrivate->pValue->IsObject())
	{
		Value newKey(key, *m_jsonPrivate->pAllocator);
		m_jsonPrivate->pValue->AddMember(newKey, Value(kArrayType), *m_jsonPrivate->pAllocator);
		auto memberIterator = m_jsonPrivate->pValue->FindMember(key);

		JsonArrayPtr array              = std::make_shared<JsonArray>();
		array->GetPrivate()->pValue     = &memberIterator->value;
		array->GetPrivate()->pKey       = &memberIterator->name;
		array->GetPrivate()->pAllocator = m_jsonPrivate->pAllocator;

		return array;
	}

	return nullptr;
}

JsonArrayPtr JsonObject::AddArray(const char* key, const JsonArrayPtr& object) const
{
	if (m_jsonPrivate->pValue->IsObject())
	{
		Value newKey(key, *m_jsonPrivate->pAllocator);
		m_jsonPrivate->pValue->AddMember(newKey, *object->GetPrivate()->pValue, *m_jsonPrivate->pAllocator);
		const auto iteratorMember =
		  m_jsonPrivate->pValue->FindMember(key); // Need to do this because the flag is remove when the value is added to the object ???

		const auto objectPtr                = std::make_shared<JsonArray>();
		objectPtr->GetPrivate()->pAllocator = m_jsonPrivate->pAllocator;
		objectPtr->GetPrivate()->pValue     = &iteratorMember->value;
		objectPtr->GetPrivate()->pKey       = &iteratorMember->name;

		return objectPtr;
	}

	return nullptr;
}

JsonArrayPtr JsonObject::AddArray(const JsonArrayPtr& object) const
{
	if (m_jsonPrivate->pValue->IsObject())
	{
		Value newKey(object->GetKey(), *m_jsonPrivate->pAllocator);
		m_jsonPrivate->pValue->AddMember(newKey, *object->GetPrivate()->pValue, *m_jsonPrivate->pAllocator);
		const auto iteratorMember = m_jsonPrivate->pValue->FindMember(object->GetKey());

		const auto objectPtr                = std::make_shared<JsonArray>();
		objectPtr->GetPrivate()->pAllocator = m_jsonPrivate->pAllocator;
		objectPtr->GetPrivate()->pValue     = &iteratorMember->value;
		objectPtr->GetPrivate()->pKey       = &iteratorMember->name;

		return objectPtr;
	}

	return nullptr;
}

bool JsonObject::IsString() const
{
	return m_jsonPrivate->pValue->IsString();
}

bool JsonObject::IsInt() const
{
	return m_jsonPrivate->pValue->IsInt();
}

bool JsonObject::IsBool() const
{
	return m_jsonPrivate->pValue->IsBool();
}

bool JsonObject::IsFloat() const
{
	return m_jsonPrivate->pValue->IsFloat();
}

bool JsonObject::IsDouble() const
{
	return m_jsonPrivate->pValue->IsDouble();
}

bool JsonObject::IsArray() const
{
	return m_jsonPrivate->pValue->IsArray();
}

StrPtr JsonObject::AsString() const
{
	if (m_jsonPrivate->pValue->IsString())
	{
		return std::make_shared<std::string>(m_jsonPrivate->pValue->GetString());
	}

	return nullptr;
}

IntPtr JsonObject::AsInt() const
{
	if (m_jsonPrivate->pValue->IsInt())
	{
		return std::make_shared<int>(m_jsonPrivate->pValue->GetInt());
	}

	return nullptr;
}

BoolPtr JsonObject::AsBool() const
{
	if (m_jsonPrivate->pValue->IsBool())
	{
		return std::make_shared<bool>(m_jsonPrivate->pValue->GetBool());
	}

	return nullptr;
}

FloatPtr JsonObject::AsFloat() const
{
	if (m_jsonPrivate->pValue->IsFloat())
	{
		return std::make_shared<float>(m_jsonPrivate->pValue->GetFloat());
	}

	return nullptr;
}

DoublePtr JsonObject::AsDouble() const
{
	if (m_jsonPrivate->pValue->IsDouble())
	{
		return std::make_shared<double>(m_jsonPrivate->pValue->GetDouble());
	}

	return nullptr;
}

JsonArrayPtr JsonObject::AsArray() const
{
	if (m_jsonPrivate->pValue->IsArray())
	{
		std::shared_ptr<JsonArray> array = std::make_shared<JsonArray>();
		array->GetPrivate()->pValue      = m_jsonPrivate->pValue;
		array->GetPrivate()->pKey        = m_jsonPrivate->pKey;
		array->GetPrivate()->pAllocator  = m_jsonPrivate->pAllocator;

		return array;
	}

	return nullptr;
}

void JsonObject::ForEach(const std::function<void(const JsonObjectPtr&, const bool&)>& function) const
{
	if (m_jsonPrivate->pValue->IsObject())
	{
		const JsonObjectPtr pObject = std::make_shared<JsonObject>();
		for (auto it = m_jsonPrivate->pValue->MemberBegin(); it != m_jsonPrivate->pValue->MemberEnd();)
		{
			bool needToBeErase(false);
			pObject->GetPrivate()->pValue     = &it->value;
			pObject->GetPrivate()->pKey       = &it->name;
			pObject->GetPrivate()->pAllocator = m_jsonPrivate->pAllocator;
			function(pObject, needToBeErase);

			if (needToBeErase)
				it = m_jsonPrivate->pValue->EraseMember(it);
			else
				++it;
		}
	}
}

struct Serializer_Private
{
	Document                 document;
	Document::AllocatorType* pAllocator = &document.GetAllocator();
};

Serializer::Serializer()
{
	m_serializerPrivate = new Serializer_Private();
}

Serializer::~Serializer()
{
	delete m_serializerPrivate;
}

JsonObjectPtr Serializer::CreateRoot()
{
	m_serializerPrivate->document.SetObject();
	const auto root                = std::make_shared<JsonObject>();
	root->GetPrivate()->pValue     = &m_serializerPrivate->document;
	root->GetPrivate()->pAllocator = m_serializerPrivate->pAllocator;

	return root;
}

JsonObjectPtr Serializer::GetRoot()
{
	const auto root                = std::make_shared<JsonObject>();
	root->GetPrivate()->pValue     = &m_serializerPrivate->document;
	root->GetPrivate()->pAllocator = m_serializerPrivate->pAllocator;
	return root;
}

bool Serializer::Parse(const std::string& path)
{
	std::ifstream file(path);
	if (file.is_open())
	{
		std::stringstream buffer;

		buffer << file.rdbuf();

		m_serializerPrivate->document.Parse(buffer.str().c_str());
		return true;
	}

	// Logicraft::Logger::Get().Log(Logicraft::Logger::eError, "Failed to open the file : " + path);
	return false;
}

bool Serializer::Write(const std::string& path)
{
	std::ofstream file(path);
	if (file.is_open())
	{
		StringBuffer               buffer;
		PrettyWriter<StringBuffer> writer(buffer);
		m_serializerPrivate->document.Accept(writer);

		file << buffer.GetString();

		return true;
	}

	// Logicraft::Logger::Get().Log(Logicraft::Logger::eError, "Failed to open the file : " + path);
	return false;
}
