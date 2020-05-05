#pragma once
#include "RTTI.h"
#include "vector.h"
#include "Signature.h"
#include "Hashmap.h"

namespace Library
{
	class TypeRegistry final
	{
	public: 
		static void Create();
		static void Shutdown();
		static void RegisterType(RTTI::IdType type, const Vector<Signature>& signatures);
		static size_t Size();
		static const Vector<Signature>& GetSignatures(RTTI::IdType type);
		static void DeregisterType(RTTI::IdType type);
	private:
		TypeRegistry() = delete;
		using MapType = Hashmap<RTTI::IdType, const Vector<Signature>>;
		using PairType = MapType::PairType;
		static MapType* mRegistry;
	};
	//TypeRegistry::MapType* TypeRegistry::mRegistry = nullptr;
}

#include "TypeRegistry.inl"