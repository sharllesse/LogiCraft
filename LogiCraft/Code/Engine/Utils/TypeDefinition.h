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

#include <SFML/Window/Event.hpp>
#include <string>

namespace Logicraft
{
#define LOGI_TYPEDEF_BASE_TYPE(TYPE)                    \
                                                        \
public:                                                 \
	using TDef = TypeDefinition<TYPE>;                    \
                                                        \
	static const std::vector<TDef*>& GetRegisteredTypes() \
	{                                                     \
		return TDef::s_registeredTypes;                     \
	}                                                     \
                                                        \
	virtual const TDef& GetType() const = 0;              \
                                                        \
private:

#define LOGI_TYPEDEF_DERIVED_TYPE(BASETYPE, DERIVEDTYPE, NAME) \
                                                               \
public:                                                        \
	using TDef = DerivedTypeDefinition<BASETYPE, DERIVEDTYPE>;   \
                                                               \
	const TDef::TDefParent& GetType() const override             \
	{                                                            \
		return s_typeDef;                                          \
	}                                                            \
	static TDef& GetTypeStatic()                                 \
	{                                                            \
		return s_typeDef;                                          \
	}                                                            \
                                                               \
private:                                                       \
	inline static TDef s_typeDef{NAME};

#define LOGI_TYPEDEF_LINKED_BASE_TYPE(TYPE, LINKEDTYPE) \
                                                        \
public:                                                 \
	using TDef = LinkedTypeDefinition<TYPE, LINKEDTYPE>;  \
                                                        \
	static const std::vector<TDef*>& GetRegisteredTypes() \
	{                                                     \
		return TDef::s_registeredTypes;                     \
	}                                                     \
                                                        \
	virtual const TDef& GetType() const = 0;              \
                                                        \
private:

#define LOGI_TYPEDEF_LINKED_DERIVED_TYPE(BASETYPE, DERIVEDTYPE, LINKEDTYPE, NAME)                                    \
                                                                                                                     \
public:                                                                                                              \
	using TDef = DerivedLinkedTypeDefinition<BASETYPE, DERIVEDTYPE, LINKEDTYPE::TDef::TDefParent::TClass, LINKEDTYPE>; \
                                                                                                                     \
	const TDef::TDefParent& GetType() const override                                                                   \
	{                                                                                                                  \
		return s_typeDef;                                                                                                \
	}                                                                                                                  \
	static TDef& GetTypeStatic()                                                                                       \
	{                                                                                                                  \
		return s_typeDef;                                                                                                \
	}                                                                                                                  \
                                                                                                                     \
private:                                                                                                             \
	inline static TDef s_typeDef{NAME};

template<typename BaseType>
class TypeDefinition
{
public:
	using TClass = BaseType;

	explicit TypeDefinition(const char* typeName)
	  : m_typeName(typeName)
	{
		s_registeredTypes.emplace_back(this);
	}

	const std::string&                GetName() const { return m_typeName; }
	virtual std::shared_ptr<BaseType> Create() const = 0;

private:
	std::string m_typeName;

	friend typename BaseType;
	inline static std::vector<TypeDefinition<BaseType>*> s_registeredTypes;
};

template<typename BaseType, typename DerivedType>
class DerivedTypeDefinition : public TypeDefinition<BaseType>
{
public:
	using TDefParent = TypeDefinition<BaseType>;

	explicit DerivedTypeDefinition(const char* typeName)
	  : TypeDefinition<BaseType>(typeName)
	{
	}

	std::shared_ptr<BaseType> Create() const override { return make_shared(DerivedType); }
};

template<typename BaseType, typename LinkedType>
class LinkedTypeDefinition
{
public:
	using TClass = BaseType;

	inline static std::vector<LinkedTypeDefinition*> s_registeredTypes;

	explicit LinkedTypeDefinition(const char* typeName)
	  : m_typeName(typeName)
	{
		s_registeredTypes.emplace_back(this);
	}

	const std::string&                GetName() const { return m_typeName; }
	virtual std::shared_ptr<BaseType> Create() const = 0;

	virtual const TypeDefinition<LinkedType>* GetLinkedType() const = 0;

private:
	std::string m_typeName;
};

template<typename BaseType, typename DerivedType, typename BaseLinkedType, typename DerivedLinkedType>
class DerivedLinkedTypeDefinition : public LinkedTypeDefinition<BaseType, BaseLinkedType>
{
public:
	using TClass     = DerivedType;
	using TDefParent = LinkedTypeDefinition<BaseType, BaseLinkedType>;
	using TLinkedDef = DerivedTypeDefinition<BaseLinkedType, DerivedLinkedType>;

	explicit DerivedLinkedTypeDefinition(const char* typeName)
	  : TDefParent(typeName)
	{
		m_linkedType = &DerivedLinkedType::GetTypeStatic();
	}
	std::shared_ptr<BaseType> Create() const override { return make_shared(DerivedType); }
	virtual const TLinkedDef* GetLinkedType() const { return m_linkedType; }

private:
	TLinkedDef* m_linkedType{nullptr};
};
} // namespace Logicraft
