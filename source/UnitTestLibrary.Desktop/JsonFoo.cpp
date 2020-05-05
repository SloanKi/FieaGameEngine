#include "pch.h"
#include "JsonFoo.h"
#include "vector.h"

namespace UnitTests
{
	/************************************************************************/
	/************************SharedData Functions****************************/
	/************************************************************************/
	RTTI_DEFINITIONS(FooSharedData)

	FooSharedData::FooSharedData(JsonFoo& data) :
		mData(&data)
	{ }

	FooSharedData::FooSharedData(JsonFoo* data) :
		mData(data)
	{ 
		assert(mData != nullptr);
		bOwnsData = true;
	}

	FooSharedData::~FooSharedData()
	{
		if (bOwnsData)
		{
			delete(mData);
		}
	}

	bool FooSharedData::operator==(const FooSharedData& rhs) const
	{
		return mData == rhs.mData;
	}

	bool FooSharedData::operator!=(const FooSharedData& rhs) const
	{
		return !operator==(rhs);
	}

	FooSharedData* FooSharedData::Create() const
	{
		return new FooSharedData(new JsonFoo());
	}

	bool FooSharedData::Equals(const RTTI* rhs) const
	{
		if (rhs == nullptr) { return false; }
		const FooSharedData* other = rhs->As<FooSharedData>();
		//if other is a scope, compare, else return false
		return (other != nullptr ? *this == *other : false);
	}

	/************************************************************************/
	/**************************Helper Functions******************************/
	/************************************************************************/
	RTTI_DEFINITIONS(FooJsonParseHelper)

	bool FooJsonParseHelper::StartHandler(Library::JsonParseMaster::SharedData& sharedData, const std::string& key, Json::Value& value, bool isArrayElement, size_t arrayIndex)
	{
		arrayIndex; //get rid of errors
		isArrayElement; //get rid of errors
		bool parsed = false;
		if (!sharedData.Is(FooSharedData::TypeIdClass())) { return false; }
		//sharedData.IncrementDepth();

		//Note this sucks lol. In real helper for scope use lookup tables for functions and stuff probably
		if (key == "Health" && value.isInt())
		{
			sharedData.As<FooSharedData>()->mData->mHealth = value.asInt();
			parsed = true;
		}
		else if (key == "DPS" && value.isDouble())
		{
			sharedData.As<FooSharedData>()->mData->mDPS = value.asFloat();
			parsed = true;
		}
		else if (key == "Aliases" && value.isString() && isArrayElement && arrayIndex < 3)
		{
			sharedData.As<FooSharedData>()->mData->mAliases[arrayIndex] = value.asString();
		}
		else if (key == "Stunned" && value.isBool())
		{
			sharedData.As<FooSharedData>()->mData->bStunned = value.asBool();
			parsed = true;
		}

		//EndHandler(sharedData, key);
		return parsed;
	}

	bool FooJsonParseHelper::EndHandler(Library::JsonParseMaster::SharedData& sharedData, const std::string& key)
	{
		key; //used to get rid of warnings
		sharedData; //get rid of warnings
		//for scope: use key to compare and pop off stack
		//sharedData.DecrementDepth();
		return true;
	}

	FooJsonParseHelper* FooJsonParseHelper::Create()
	{
		return new FooJsonParseHelper();
	}

	bool FooJsonParseHelper::Equals(const RTTI* rhs) const
	{
		if (rhs == nullptr) { return false; }
		auto helper = rhs->As<FooJsonParseHelper>();
		return helper != nullptr;
	}

	JsonFoo::JsonFoo()
	{
		mAliases.Resize(3);
	}

}