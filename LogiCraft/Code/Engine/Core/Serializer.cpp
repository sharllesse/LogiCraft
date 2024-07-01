#include "Serializer.h"

#include <fstream>
#include <iostream>
#include <json/document.h>
#include <json/prettywriter.h>
#include <json/stringbuffer.h>
#include <sstream>
#include <string>
#include <vector>

#include "Action.h"
#include "Logger.h"


using namespace rapidjson;

struct Logicraft::JsonArray::Private
{
	Document::AllocatorType* pAllocator  = nullptr;
	Value*                   pValue      = nullptr;
	Value*                   pKey        = nullptr;
	bool                     isAllocated = false;
};

Logicraft::JsonArray::JsonArray()
{
	m_pPrivate = new Logicraft::JsonArray::Private();
}

Logicraft::JsonArray::JsonArray(const char* key, const Logicraft::JsonObjectPtr& root)
{
	m_pPrivate              = new Logicraft::JsonArray::Private();
	m_pPrivate->isAllocated = true;
	m_pPrivate->pAllocator  = root->m_pPrivate->pAllocator;
	m_pPrivate->pValue      = new Value(kArrayType);
	m_pPrivate->pKey        = new Value(key, *m_pPrivate->pAllocator);
}

Logicraft::JsonArray::~JsonArray()
{
	if (m_pPrivate)
	{
		if (m_pPrivate->isAllocated)
		{
			delete m_pPrivate->pKey;
			delete m_pPrivate->pValue;
		}

		delete m_pPrivate;
	}

	m_pPrivate = nullptr;
}

const char* Logicraft::JsonArray::GetKey() const
{
	if (m_pPrivate->pKey)
	{
		return m_pPrivate->pKey->GetString();
	}

	return nullptr;
}

void Logicraft::JsonArray::PushBack(const Logicraft::JsonObjectPtr& value) const
{
	if (m_pPrivate->pValue->IsArray())
		m_pPrivate->pValue->PushBack(*value->m_pPrivate->pValue, *m_pPrivate->pAllocator);
	else
		LogWarning("Cannot push back an object in a ");
}

void Logicraft::JsonArray::PushBack(const JsonArrayPtr& value) const
{
	if (m_pPrivate->pValue->IsArray())
		m_pPrivate->pValue->PushBack(*value->m_pPrivate->pValue, *m_pPrivate->pAllocator);
	else
		LogWarning("Cannot push back an array in a ");
}

void Logicraft::JsonArray::PushBack(const std::string& value) const
{
	if (m_pPrivate->pValue->IsArray())
		m_pPrivate->pValue->PushBack(Value(value.c_str(), *m_pPrivate->pAllocator), *m_pPrivate->pAllocator);
	else
		LogWarning("Cannot push back an string in a ");
}

void Logicraft::JsonArray::PushBack(const char* value) const
{
	if (m_pPrivate->pValue->IsArray())
		m_pPrivate->pValue->PushBack(Value(value, *m_pPrivate->pAllocator), *m_pPrivate->pAllocator);
	else
		LogWarning("Cannot push back a string in a ");
}

void Logicraft::JsonArray::PushBack(const int& value) const
{
	if (m_pPrivate->pValue->IsArray())
		m_pPrivate->pValue->PushBack(value, *m_pPrivate->pAllocator);
	else
		LogWarning("Cannot push back a string in a ");
}

void Logicraft::JsonArray::PushBack(const double& value) const
{
	if (m_pPrivate->pValue->IsArray())
		m_pPrivate->pValue->PushBack(value, *m_pPrivate->pAllocator);
	else
		LogWarning("Cannot push back an int in a ");
}

void Logicraft::JsonArray::PushBack(const float& value) const
{
	if (m_pPrivate->pValue->IsArray())
		m_pPrivate->pValue->PushBack(value, *m_pPrivate->pAllocator);
	else
		LogWarning("Cannot push back a double in a ");
}

void Logicraft::JsonArray::PushBack(const bool& value) const
{
	if (m_pPrivate->pValue->IsArray())
		m_pPrivate->pValue->PushBack(value, *m_pPrivate->pAllocator);
	else
		LogWarning("Cannot push back a float in a ");
}

void Logicraft::JsonArray::PopBack() const
{
	if (m_pPrivate->pValue->IsArray())
		m_pPrivate->pValue->PopBack();
	else
		LogWarning("Cannot PushBack a bool in a ");
}

void Logicraft::JsonArray::Clear() const
{
	if (m_pPrivate->pValue->IsArray())
		m_pPrivate->pValue->Clear();
	else
		LogWarning("Cannot use PopBack with a ");
}

size_t Logicraft::JsonArray::Size() const
{
	if (m_pPrivate->pValue->IsArray())
		return m_pPrivate->pValue->Size();

	LogWarning("Cannot use Size with a ");

	return 0u;
}

bool Logicraft::JsonArray::Empty() const
{
	if (m_pPrivate->pValue->IsArray())
		return m_pPrivate->pValue->Empty();

	LogWarning("Cannot use Empty with a ");

	return true;
}

void Logicraft::JsonArray::ForEach(const std::function<void(const Logicraft::JsonObjectPtr&, const bool&)>& function) const
{
	if (m_pPrivate->pValue->IsArray())
	{
		const Logicraft::JsonObjectPtr pObject = make_shared(JsonObject);
		for (auto it = m_pPrivate->pValue->Begin(); it != m_pPrivate->pValue->End();)
		{
			bool needToBeErase(false);
			pObject->m_pPrivate->pValue     = it;
			pObject->m_pPrivate->pKey       = m_pPrivate->pKey;
			pObject->m_pPrivate->pAllocator = m_pPrivate->pAllocator;
			function(pObject, needToBeErase);

			if (needToBeErase)
				it = m_pPrivate->pValue->Erase(it);
			else
				it++;
		}
	}
	else
		LogWarning("Cannot use foreach of an array with a ");
}

void Logicraft::JsonArray::LogWarning(const char* message) const
{
	if (m_pPrivate->pValue->IsString())
		Logicraft::Logger::Get().Log(Logicraft::Logger::eWarning, std::string(message) + "string");
	if (m_pPrivate->pValue->IsBool())
		Logicraft::Logger::Get().Log(Logicraft::Logger::eWarning, std::string(message) + "bool");
	if (m_pPrivate->pValue->IsDouble())
		Logicraft::Logger::Get().Log(Logicraft::Logger::eWarning, std::string(message) + "double");
	if (m_pPrivate->pValue->IsFloat())
		Logicraft::Logger::Get().Log(Logicraft::Logger::eWarning, std::string(message) + "float");
	if (m_pPrivate->pValue->IsInt())
		Logicraft::Logger::Get().Log(Logicraft::Logger::eWarning, std::string(message) + "int");
}

Logicraft::JsonObject::JsonObject()
{
	m_pPrivate = new Logicraft::JsonArray::Private();
}

Logicraft::JsonObject::JsonObject(const char* key, const Logicraft::JsonObjectPtr& root)
{
	m_pPrivate              = new Logicraft::JsonArray::Private();
	m_pPrivate->isAllocated = true;
	m_pPrivate->pAllocator  = root->m_pPrivate->pAllocator;
	m_pPrivate->pKey        = new Value(key, *m_pPrivate->pAllocator);
	m_pPrivate->pValue      = new Value(kObjectType);
}

Logicraft::JsonObject::JsonObject(const char* key, const std::string& value, const Logicraft::JsonObjectPtr& root)
{
	m_pPrivate              = new Logicraft::JsonArray::Private();
	m_pPrivate->isAllocated = true;
	m_pPrivate->pAllocator  = root->m_pPrivate->pAllocator;
	m_pPrivate->pKey        = new Value(key, *m_pPrivate->pAllocator);
	m_pPrivate->pValue      = new Value(value.c_str(), *m_pPrivate->pAllocator);
}

Logicraft::JsonObject::JsonObject(const char* key, const char* value, const Logicraft::JsonObjectPtr& root)
{
	m_pPrivate              = new Logicraft::JsonArray::Private();
	m_pPrivate->isAllocated = true;
	m_pPrivate->pAllocator  = root->m_pPrivate->pAllocator;
	m_pPrivate->pKey        = new Value(key, *m_pPrivate->pAllocator);
	m_pPrivate->pValue      = new Value(value, *m_pPrivate->pAllocator);
}

Logicraft::JsonObject::JsonObject(const char* key, const int& value, const Logicraft::JsonObjectPtr& root)
{
	m_pPrivate              = new Logicraft::JsonArray::Private();
	m_pPrivate->isAllocated = true;
	m_pPrivate->pAllocator  = root->m_pPrivate->pAllocator;
	m_pPrivate->pKey        = new Value(key, *m_pPrivate->pAllocator);
	m_pPrivate->pValue      = new Value(value);
}

Logicraft::JsonObject::JsonObject(const char* key, const bool& value, const Logicraft::JsonObjectPtr& root)
{
	m_pPrivate              = new Logicraft::JsonArray::Private();
	m_pPrivate->isAllocated = true;
	m_pPrivate->pAllocator  = root->m_pPrivate->pAllocator;
	m_pPrivate->pKey        = new Value(key, *m_pPrivate->pAllocator);
	m_pPrivate->pValue      = new Value(value);
}

Logicraft::JsonObject::JsonObject(const char* key, const float& value, const Logicraft::JsonObjectPtr& root)
{
	m_pPrivate              = new Logicraft::JsonArray::Private();
	m_pPrivate->isAllocated = true;
	m_pPrivate->pAllocator  = root->m_pPrivate->pAllocator;
	m_pPrivate->pKey        = new Value(key, *m_pPrivate->pAllocator);
	m_pPrivate->pValue      = new Value(value);
}

Logicraft::JsonObject::JsonObject(const char* key, const double& value, const Logicraft::JsonObjectPtr& root)
{
	m_pPrivate              = new Logicraft::JsonArray::Private();
	m_pPrivate->isAllocated = true;
	m_pPrivate->pAllocator  = root->m_pPrivate->pAllocator;
	m_pPrivate->pKey        = new Value(key, *m_pPrivate->pAllocator);
	m_pPrivate->pValue      = new Value(value);
}

Logicraft::JsonObject::~JsonObject()
{
	if (m_pPrivate)
	{
		if (m_pPrivate->isAllocated)
		{
			delete m_pPrivate->pKey;
			delete m_pPrivate->pValue;
		}

		delete m_pPrivate;
	}

	m_pPrivate = nullptr;
}

Logicraft::JsonObjectPtr Logicraft::JsonObject::GetObject(const char* key) const
{
	if (m_pPrivate->pValue->IsObject())
	{
		const Value::MemberIterator& memberIterator = m_pPrivate->pValue->FindMember(key);
		if (memberIterator != m_pPrivate->pValue->MemberEnd())
		{
			const JsonObjectPtr object = make_shared(JsonObject);

			object->m_pPrivate->pValue     = &memberIterator->value;
			object->m_pPrivate->pKey       = &memberIterator->name;
			object->m_pPrivate->pAllocator = m_pPrivate->pAllocator;

			return object;
		}

		return nullptr;
	}

	LogWarning("Cannot get an object from a ");

	return nullptr;
}

bool Logicraft::JsonObject::GetObject(const char* key, Logicraft::JsonObjectPtr& object) const
{
	if (m_pPrivate->pValue->IsObject())
	{
		const Value::MemberIterator& memberIterator = m_pPrivate->pValue->FindMember(key);
		if (memberIterator != m_pPrivate->pValue->MemberEnd())
		{
			if (!object)
				object = make_shared(JsonObject);

			object->m_pPrivate->pValue     = &memberIterator->value;
			object->m_pPrivate->pKey       = &memberIterator->name;
			object->m_pPrivate->pAllocator = m_pPrivate->pAllocator;

			return true;
		}

		return false;
	}

	LogWarning("Cannot get an object from a ");

	return false;
}

Logicraft::StringPtr Logicraft::JsonObject::GetString(const char* key) const
{
	if (m_pPrivate->pValue->IsObject())
	{
		const auto memberIterator = m_pPrivate->pValue->FindMember(key);
		if (memberIterator != m_pPrivate->pValue->MemberEnd())
		{
			const Value& value = memberIterator->value;

			if (value.IsString())
				return make_shared(std::string,value.GetString());
		}

		return nullptr;
	}

	LogWarning("Cannot get an string from a ");

	return nullptr;
}

bool Logicraft::JsonObject::GetString(const char* key, std::string& rString) const
{
	if (m_pPrivate->pValue->IsObject())
	{
		const auto memberIterator = m_pPrivate->pValue->FindMember(key);
		if (memberIterator != m_pPrivate->pValue->MemberEnd())
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

	LogWarning("Cannot get an string from a ");

	return false;
}

Logicraft::IntPtr Logicraft::JsonObject::GetInt(const char* key) const
{
	if (m_pPrivate->pValue->IsObject())
	{
		const auto memberIterator = m_pPrivate->pValue->FindMember(key);
		if (memberIterator != m_pPrivate->pValue->MemberEnd())
		{
			const Value& value = memberIterator->value;

			if (value.IsInt())
				return make_shared(int,value.GetInt());
		}

		return nullptr;
	}

	LogWarning("Cannot get an int from a ");

	return nullptr;
}

bool Logicraft::JsonObject::GetInt(const char* key, int& rInt) const
{
	if (m_pPrivate->pValue->IsObject())
	{
		const auto memberIterator = m_pPrivate->pValue->FindMember(key);
		if (memberIterator != m_pPrivate->pValue->MemberEnd())
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

	LogWarning("Cannot get an int from a ");

	return false;
}

Logicraft::BoolPtr Logicraft::JsonObject::GetBool(const char* key) const
{
	if (m_pPrivate->pValue->IsObject())
	{
		const auto memberIterator = m_pPrivate->pValue->FindMember(key);
		if (memberIterator != m_pPrivate->pValue->MemberEnd())
		{
			const Value& value = memberIterator->value;

			if (value.IsBool())
				return make_shared(bool,value.GetBool());
		}

		return nullptr;
	}

	LogWarning("Cannot get an bool from a ");

	return nullptr;
}

bool Logicraft::JsonObject::GetBool(const char* key, bool& rBool) const
{
	if (m_pPrivate->pValue->IsObject())
	{
		const auto memberIterator = m_pPrivate->pValue->FindMember(key);
		if (memberIterator != m_pPrivate->pValue->MemberEnd())
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

	LogWarning("Cannot get an bool from a ");

	return false;
}

Logicraft::FloatPtr Logicraft::JsonObject::GetFloat(const char* key) const
{
	if (m_pPrivate->pValue->IsObject())
	{
		const auto memberIterator = m_pPrivate->pValue->FindMember(key);
		if (memberIterator != m_pPrivate->pValue->MemberEnd())
		{
			const Value& value = memberIterator->value;

			if (value.IsFloat())
				return make_shared(float,value.GetFloat());
		}

		return nullptr;
	}

	LogWarning("Cannot get an float from a ");

	return nullptr;
}

bool Logicraft::JsonObject::GetFloat(const char* key, float& rFloat) const
{
	if (m_pPrivate->pValue->IsObject())
	{
		const auto memberIterator = m_pPrivate->pValue->FindMember(key);
		if (memberIterator != m_pPrivate->pValue->MemberEnd())
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

	LogWarning("Cannot get an float from a ");

	return false;
}

Logicraft::DoublePtr Logicraft::JsonObject::GetDouble(const char* key) const
{
	if (m_pPrivate->pValue->IsObject())
	{
		const auto memberIterator = m_pPrivate->pValue->FindMember(key);
		if (memberIterator != m_pPrivate->pValue->MemberEnd())
		{
			const Value& value = memberIterator->value;

			if (value.IsDouble())
				return make_shared(double,value.GetDouble());
		}

		return nullptr;
	}

	LogWarning("Cannot get an double from a ");

	return nullptr;
}

bool Logicraft::JsonObject::GetDouble(const char* key, double& rDouble) const
{
	if (m_pPrivate->pValue->IsObject())
	{
		const auto memberIterator = m_pPrivate->pValue->FindMember(key);
		if (memberIterator != m_pPrivate->pValue->MemberEnd())
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

	LogWarning("Cannot get an double from a ");

	return false;
}

Logicraft::JsonArrayPtr Logicraft::JsonObject::GetArray(const char* key) const
{
	if (m_pPrivate->pValue->IsObject())
	{
		const auto memberIterator = m_pPrivate->pValue->FindMember(key);
		if (memberIterator != m_pPrivate->pValue->MemberEnd())
		{
			if (memberIterator->value.IsArray())
			{
				const JsonArrayPtr array      = make_shared(JsonArray);
				array->m_pPrivate->pValue     = &memberIterator->value;
				array->m_pPrivate->pKey       = &memberIterator->name;
				array->m_pPrivate->pAllocator = m_pPrivate->pAllocator;
				return array;
			}
		}

		return nullptr;
	}

	LogWarning("Cannot get an array from a ");

	return nullptr;
}

bool Logicraft::JsonObject::GetArray(const char* key, JsonArrayPtr& rArray) const
{
	if (m_pPrivate->pValue->IsObject())
	{
		const auto memberIterator = m_pPrivate->pValue->FindMember(key);
		if (memberIterator != m_pPrivate->pValue->MemberEnd())
		{
			if (memberIterator->value.IsArray())
			{
				rArray->m_pPrivate->pValue     = &memberIterator->value;
				rArray->m_pPrivate->pKey       = &memberIterator->name;
				rArray->m_pPrivate->pAllocator = m_pPrivate->pAllocator;
				return true;
			}
		}

		return false;
	}

	LogWarning("Cannot get an array from a ");

	return false;
}

const char* Logicraft::JsonObject::GetKey() const
{
	if (m_pPrivate->pKey)
	{
		return m_pPrivate->pKey->GetString();
	}

	return nullptr;
}

Logicraft::JsonObjectPtr Logicraft::JsonObject::AddObject(const char* key) const
{
	if (m_pPrivate->pValue->IsObject())
	{
		Value newKey(key, *m_pPrivate->pAllocator);
		Value newObject(kObjectType);
		m_pPrivate->pValue->AddMember(newKey, newObject, *m_pPrivate->pAllocator);
		const auto iteratorMember =
		  m_pPrivate->pValue->FindMember(key); // Need to do this because the flag is remove when the value is added to the object ???

		const JsonObjectPtr objectPtr = make_shared(Logicraft::JsonObject);
		objectPtr->m_pPrivate->pAllocator = m_pPrivate->pAllocator;
		objectPtr->m_pPrivate->pValue     = &iteratorMember->value;
		objectPtr->m_pPrivate->pKey       = &iteratorMember->name;

		return objectPtr;
	}

	LogWarning("Cannot add an object to a ");

	return nullptr;
}

Logicraft::JsonObjectPtr Logicraft::JsonObject::AddObject(const char* key, const Logicraft::JsonObjectPtr& object) const
{
	if (m_pPrivate->pValue->IsObject())
	{
		Value newKey(key, *m_pPrivate->pAllocator);
		m_pPrivate->pValue->AddMember(newKey, *object->m_pPrivate->pValue, *m_pPrivate->pAllocator);
		const auto iteratorMember = m_pPrivate->pValue->FindMember(key);

		const JsonObjectPtr objectPtr = make_shared(JsonObject);
		objectPtr->m_pPrivate->pAllocator = m_pPrivate->pAllocator;
		objectPtr->m_pPrivate->pValue     = &iteratorMember->value;
		objectPtr->m_pPrivate->pKey       = &iteratorMember->name;

		return objectPtr;
	}

	LogWarning("Cannot add an object to a ");

	return nullptr;
}

Logicraft::JsonObjectPtr Logicraft::JsonObject::AddObject(const Logicraft::JsonObjectPtr& object) const
{
	if (m_pPrivate->pValue->IsObject())
	{
		Value newKey(object->GetKey(), *m_pPrivate->pAllocator);
		m_pPrivate->pValue->AddMember(newKey, *object->m_pPrivate->pValue, *m_pPrivate->pAllocator);
		const auto iteratorMember = m_pPrivate->pValue->FindMember(object->GetKey());

		const JsonObjectPtr objectPtr = make_shared(JsonObject);
		objectPtr->m_pPrivate->pAllocator = m_pPrivate->pAllocator;
		objectPtr->m_pPrivate->pValue     = &iteratorMember->value;
		objectPtr->m_pPrivate->pKey       = &iteratorMember->name;

		return objectPtr;
	}

	LogWarning("Cannot add an object to a ");

	return nullptr;
}

Logicraft::StringPtr Logicraft::JsonObject::AddString(const char* key, const char* value) const
{
	if (m_pPrivate->pValue->IsObject())
	{
		Value newKey(key, *m_pPrivate->pAllocator);
		Value newObject(value, *m_pPrivate->pAllocator);
		m_pPrivate->pValue->AddMember(newKey, newObject, *m_pPrivate->pAllocator);

		return make_shared(std::string,value);
	}

	LogWarning("Cannot add a string to a ");

	return nullptr;
}

Logicraft::StringPtr Logicraft::JsonObject::AddString(const char* key, const std::string& value) const
{
	if (m_pPrivate->pValue->IsObject())
	{
		Value newKey(key, *m_pPrivate->pAllocator);
		Value newObject(value.c_str(), *m_pPrivate->pAllocator);
		m_pPrivate->pValue->AddMember(newKey, newObject, *m_pPrivate->pAllocator);

		return make_shared(std::string,value);
	}

	LogWarning("Cannot add a string to a ");

	return nullptr;
}

Logicraft::IntPtr Logicraft::JsonObject::AddInt(const char* key, const int& value) const
{
	if (m_pPrivate->pValue->IsObject())
	{
		Value newKey(key, *m_pPrivate->pAllocator);
		Value newObject(value);
		m_pPrivate->pValue->AddMember(newKey, newObject, *m_pPrivate->pAllocator);

		return make_shared(int,value);
	}

	LogWarning("Cannot add an int to a ");

	return nullptr;
}

Logicraft::BoolPtr Logicraft::JsonObject::AddBool(const char* key, const bool& value) const
{
	if (m_pPrivate->pValue->IsObject())
	{
		Value newKey(key, *m_pPrivate->pAllocator);
		Value newObject(value);
		m_pPrivate->pValue->AddMember(newKey, newObject, *m_pPrivate->pAllocator);

		return make_shared(bool,value);
	}

	LogWarning("Cannot add a bool to a ");

	return nullptr;
}

Logicraft::FloatPtr Logicraft::JsonObject::AddFloat(const char* key, const float& value) const
{
	if (m_pPrivate->pValue->IsObject())
	{
		Value newKey(key, *m_pPrivate->pAllocator);
		Value newObject(value);
		m_pPrivate->pValue->AddMember(newKey, newObject, *m_pPrivate->pAllocator);

		return make_shared(float,value);
	}

	LogWarning("Cannot add a float to a ");

	return nullptr;
}

Logicraft::DoublePtr Logicraft::JsonObject::AddDouble(const char* key, const double& value) const
{
	if (m_pPrivate->pValue->IsObject())
	{
		Value newKey(key, *m_pPrivate->pAllocator);
		Value newObject(value);
		m_pPrivate->pValue->AddMember(newKey, newObject, *m_pPrivate->pAllocator);

		return make_shared(double,value);
	}

	LogWarning("Cannot add a double to a ");

	return nullptr;
}

Logicraft::JsonArrayPtr Logicraft::JsonObject::AddArray(const char* key) const
{
	if (m_pPrivate->pValue->IsObject())
	{
		Value newKey(key, *m_pPrivate->pAllocator);
		m_pPrivate->pValue->AddMember(newKey, Value(kArrayType), *m_pPrivate->pAllocator);
		auto memberIterator = m_pPrivate->pValue->FindMember(key);

		JsonArrayPtr array            = make_shared(JsonArray);
		array->m_pPrivate->pValue     = &memberIterator->value;
		array->m_pPrivate->pKey       = &memberIterator->name;
		array->m_pPrivate->pAllocator = m_pPrivate->pAllocator;

		return array;
	}

	LogWarning("Cannot add an array to a ");

	return nullptr;
}

Logicraft::JsonArrayPtr Logicraft::JsonObject::AddArray(const char* key, const JsonArrayPtr& object) const
{
	if (m_pPrivate->pValue->IsObject())
	{
		Value newKey(key, *m_pPrivate->pAllocator);
		m_pPrivate->pValue->AddMember(newKey, *object->m_pPrivate->pValue, *m_pPrivate->pAllocator);
		const auto iteratorMember =
		  m_pPrivate->pValue->FindMember(key); // Need to do this because the flag is remove when the value is added to the object ???

		const auto objectPtr              = make_shared(JsonArray);
		objectPtr->m_pPrivate->pAllocator = m_pPrivate->pAllocator;
		objectPtr->m_pPrivate->pValue     = &iteratorMember->value;
		objectPtr->m_pPrivate->pKey       = &iteratorMember->name;

		return objectPtr;
	}

	LogWarning("Cannot add an array to a ");

	return nullptr;
}

Logicraft::JsonArrayPtr Logicraft::JsonObject::AddArray(const JsonArrayPtr& object) const
{
	if (m_pPrivate->pValue->IsObject())
	{
		Value newKey(object->GetKey(), *m_pPrivate->pAllocator);
		m_pPrivate->pValue->AddMember(newKey, *object->m_pPrivate->pValue, *m_pPrivate->pAllocator);
		const auto iteratorMember = m_pPrivate->pValue->FindMember(object->GetKey());

		const auto objectPtr              = make_shared(JsonArray);
		objectPtr->m_pPrivate->pAllocator = m_pPrivate->pAllocator;
		objectPtr->m_pPrivate->pValue     = &iteratorMember->value;
		objectPtr->m_pPrivate->pKey       = &iteratorMember->name;

		return objectPtr;
	}

	LogWarning("Cannot add an array to a ");

	return nullptr;
}

bool Logicraft::JsonObject::IsString() const
{
	return m_pPrivate->pValue->IsString();
}

bool Logicraft::JsonObject::IsInt() const
{
	return m_pPrivate->pValue->IsInt();
}

bool Logicraft::JsonObject::IsBool() const
{
	return m_pPrivate->pValue->IsBool();
}

bool Logicraft::JsonObject::IsFloat() const
{
	return m_pPrivate->pValue->IsFloat();
}

bool Logicraft::JsonObject::IsDouble() const
{
	return m_pPrivate->pValue->IsDouble();
}

bool Logicraft::JsonObject::IsArray() const
{
	return m_pPrivate->pValue->IsArray();
}

Logicraft::StringPtr Logicraft::JsonObject::AsString() const
{
	if (m_pPrivate->pValue->IsString())
	{
		return make_shared(std::string,m_pPrivate->pValue->GetString());
	}

	LogWarning("Cannot convert a string to a ");

	return nullptr;
}

Logicraft::IntPtr Logicraft::JsonObject::AsInt() const
{
	if (m_pPrivate->pValue->IsInt())
	{
		return make_shared(int,m_pPrivate->pValue->GetInt());
	}

	LogWarning("Cannot convert an int to a ");

	return nullptr;
}

Logicraft::BoolPtr Logicraft::JsonObject::AsBool() const
{
	if (m_pPrivate->pValue->IsBool())
	{
		return make_shared(bool,m_pPrivate->pValue->GetBool());
	}

	LogWarning("Cannot convert a bool to a ");

	return nullptr;
}

Logicraft::FloatPtr Logicraft::JsonObject::AsFloat() const
{
	if (m_pPrivate->pValue->IsFloat())
	{
		return make_shared(float,m_pPrivate->pValue->GetFloat());
	}

	LogWarning("Cannot convert a float to a ");

	return nullptr;
}

Logicraft::DoublePtr Logicraft::JsonObject::AsDouble() const
{
	if (m_pPrivate->pValue->IsDouble())
	{
		return make_shared(double,m_pPrivate->pValue->GetDouble());
	}

	LogWarning("Cannot convert a double to a ");

	return nullptr;
}

Logicraft::JsonArrayPtr Logicraft::JsonObject::AsArray() const
{
	if (m_pPrivate->pValue->IsArray())
	{
		std::shared_ptr<JsonArray> array = make_shared(JsonArray);
		array->m_pPrivate->pValue        = m_pPrivate->pValue;
		array->m_pPrivate->pKey          = m_pPrivate->pKey;
		array->m_pPrivate->pAllocator    = m_pPrivate->pAllocator;

		return array;
	}

	LogWarning("Cannot convert an array to a ");

	return nullptr;
}

void Logicraft::JsonObject::ForEach(const std::function<void(const Logicraft::JsonObjectPtr&, const bool&)>& function) const
{
	if (m_pPrivate->pValue->IsObject())
	{
		const Logicraft::JsonObjectPtr pObject = make_shared(JsonObject);
		for (auto it = m_pPrivate->pValue->MemberBegin(); it != m_pPrivate->pValue->MemberEnd();)
		{
			bool needToBeErase(false);
			pObject->m_pPrivate->pValue     = &it->value;
			pObject->m_pPrivate->pKey       = &it->name;
			pObject->m_pPrivate->pAllocator = m_pPrivate->pAllocator;
			function(pObject, needToBeErase);

			if (needToBeErase)
				it = m_pPrivate->pValue->EraseMember(it);
			else
				++it;
		}
	}
	else
		LogWarning("Cannot use foreach of an object with a ");
}

struct Logicraft::Serializer::Private
{
	Document                 document;
	Document::AllocatorType* pAllocator = &document.GetAllocator();
};

Logicraft::Serializer::Serializer()
{
	m_pPrivate = new Serializer::Private();
}

Logicraft::Serializer::~Serializer()
{
	delete m_pPrivate;
}

Logicraft::JsonObjectPtr Logicraft::Serializer::CreateRoot()
{
	m_pPrivate->document.SetObject();
	const JsonObjectPtr root     = make_shared(JsonObject);
	root->m_pPrivate->pValue     = &m_pPrivate->document;
	root->m_pPrivate->pAllocator = m_pPrivate->pAllocator;

	return root;
}

Logicraft::JsonObjectPtr Logicraft::Serializer::GetRoot()
{
	const JsonObjectPtr root     = make_shared(JsonObject);
	root->m_pPrivate->pValue     = &m_pPrivate->document;
	root->m_pPrivate->pAllocator = m_pPrivate->pAllocator;

	return root;
}

bool Logicraft::Serializer::Parse(const std::string& path)
{
	std::ifstream file(path);
	if (file.is_open())
	{
		std::stringstream buffer;

		buffer << file.rdbuf();

		m_pPrivate->document.Parse(buffer.str().c_str());
		return true;
	}

	Logicraft::Logger::Get().Log(Logicraft::Logger::eError, "Failed to open the file to read : " + path);

	return false;
}

bool Logicraft::Serializer::Write(const std::string& path)
{
	std::ofstream file(path);
	if (file.is_open())
	{
		StringBuffer               buffer;
		PrettyWriter<StringBuffer> writer(buffer);
		m_pPrivate->document.Accept(writer);

		file << buffer.GetString();

		return true;
	}

	Logicraft::Logger::Get().Log(Logicraft::Logger::eError, "Failed to open the file to write : " + path);

	return false;
}
