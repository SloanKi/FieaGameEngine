#include "TypeRegistry.h"

namespace Library
{
	//TypeRegistry::MapType* TypeRegistry::mRegistry = nullptr;

	inline void TypeRegistry::Create()
	{
		if (mRegistry != nullptr) 
		{ 
			//return; 
			throw std::runtime_error("can't create more than one type registry");
		}
		mRegistry = new MapType(23);
	}

	inline void TypeRegistry::Shutdown()
	{
		delete(mRegistry);
		mRegistry = nullptr;
	}

	inline void TypeRegistry::RegisterType(RTTI::IdType type, const Vector<Signature>& signatures)
	{
		assert(mRegistry != nullptr);
		mRegistry->Insert({ type, signatures });
	}

	inline size_t TypeRegistry::Size()
	{
		assert(mRegistry != nullptr);
		return mRegistry->Size();
	}

	inline const Vector<Signature>& TypeRegistry::GetSignatures(RTTI::IdType type)
	{
		assert(mRegistry != nullptr);
		return mRegistry->At(type);
	}
	inline void TypeRegistry::DeregisterType(RTTI::IdType type)
	{
		assert(mRegistry != nullptr);
		mRegistry->Remove(type);
	}
}