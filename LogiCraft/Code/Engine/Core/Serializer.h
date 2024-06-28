#pragma once
#include "Logger.h"

#include <functional>
#include <fstream>
#include <sstream>

// struct JsonArray_Private;
// struct JsonObject_Private;
struct Json_Private;

class JsonArray;
using JsonArrayPtr = std::shared_ptr<JsonArray>;

class JsonObject;
using JsonObjectPtr = std::shared_ptr<JsonObject>;

using IntPtr    = std::shared_ptr<int>;
using BoolPtr   = std::shared_ptr<bool>;
using FloatPtr  = std::shared_ptr<float>;
using DoublePtr = std::shared_ptr<double>;
using StrPtr    = std::shared_ptr<std::string>;

class JsonArray
{
public:
	JsonArray();
	JsonArray(const char* key, const JsonObjectPtr& root);
	virtual ~JsonArray();

	virtual const char* GetKey() const;

	void PushBack(const JsonObjectPtr& value) const;
	void PushBack(const JsonArrayPtr& value) const;
	void PushBack(const std::string& value) const;
	void PushBack(const char* value) const;
	void PushBack(const int& value) const;
	void PushBack(const double& value) const;
	void PushBack(const float& value) const;
	void PushBack(const bool& value) const;
	void PopBack() const;

	void   Clear() const;
	size_t Size() const;
	bool   Empty() const;

	virtual void ForEach(const std::function<void(const JsonObjectPtr&, const bool&)>& function) const;

	Json_Private* GetPrivate() const;

protected:
	struct Json_Private* m_jsonPrivate;
};

class JsonObject : public JsonArray
{
public:
	JsonObject();
	// This constructor is used if the object as no allocator
	//@param furtherParent: The further parent of the object.
	JsonObject(const char* key, const JsonObjectPtr& root);
	JsonObject(const char* key, const std::string& value, const JsonObjectPtr& root);
	JsonObject(const char* key, const char* value, const JsonObjectPtr& root);
	JsonObject(const char* key, const int& value, const JsonObjectPtr& root);
	JsonObject(const char* key, const bool& value, const JsonObjectPtr& root);
	JsonObject(const char* key, const float& value, const JsonObjectPtr& root);
	JsonObject(const char* key, const double& value, const JsonObjectPtr& root);
	virtual ~JsonObject() override;

	JsonObjectPtr GetObject(const char* key) const;
	bool          GetObject(const char* key, JsonObjectPtr& object) const;

	StrPtr GetString(const char* key) const;
	bool   GetString(const char* key, std::string& rString) const;

	IntPtr GetInt(const char* key) const;
	bool   GetInt(const char* key, int& rInt) const;

	BoolPtr GetBool(const char* key) const;
	bool    GetBool(const char* key, bool& rBool) const;

	FloatPtr GetFloat(const char* key) const;
	bool     GetFloat(const char* key, float& rFloat) const;

	DoublePtr GetDouble(const char* key) const;
	bool      GetDouble(const char* key, double& rDouble) const;

	JsonArrayPtr GetArray(const char* key) const;
	bool         GetArray(const char* key, JsonArrayPtr& rArray) const;

	virtual const char* GetKey() const override;

	JsonObjectPtr AddObject(const char* key) const;
	JsonObjectPtr AddObject(const char* key, const JsonObjectPtr& object) const;
	JsonObjectPtr AddObject(const JsonObjectPtr& object) const;

	StrPtr AddString(const char* key, const char* value) const; // No ptr but normal value at the return ? maybe ?
	StrPtr AddString(const char* key, const std::string& value) const;

	IntPtr    AddInt(const char* key, const int& value) const;
	BoolPtr   AddBool(const char* key, const bool& value) const;
	FloatPtr  AddFloat(const char* key, const float& value) const;
	DoublePtr AddDouble(const char* key, const double& value) const;

	JsonArrayPtr AddArray(const char* key) const;
	JsonArrayPtr AddArray(const char* key, const JsonArrayPtr& object) const;
	JsonArrayPtr AddArray(const JsonArrayPtr& object) const;

	bool IsString() const;
	bool IsInt() const;
	bool IsBool() const;
	bool IsFloat() const;
	bool IsDouble() const;
	bool IsArray() const;

	StrPtr       AsString() const;
	IntPtr       AsInt() const;
	BoolPtr      AsBool() const;
	FloatPtr     AsFloat() const;
	DoublePtr    AsDouble() const;
	JsonArrayPtr AsArray() const;

	virtual void ForEach(const std::function<void(const JsonObjectPtr&, const bool&)>& function) const override;

	// Json_ObjectPrivate* GetPrivateObject() const { return m_jsonObjectPrivate; }

private:
	// struct Json_ObjectPrivate* m_jsonObjectPrivate;
};

class Serializer
{
public:
	Serializer();
	~Serializer();

	JsonObjectPtr CreateRoot();
	JsonObjectPtr GetRoot();

	bool Parse(const std::string& path);
	bool Write(const std::string& path);

private:
	struct Serializer_Private* m_serializerPrivate;
};
