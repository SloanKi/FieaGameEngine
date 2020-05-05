#include "pch.h"
#include "TestParseHelper.h"

namespace UnitTests
{
	/************************************************************************/
	/************************SharedData Functions****************************/
	/************************************************************************/
	RTTI_DEFINITIONS(TestSharedData)


	bool TestSharedData::operator==(const TestSharedData& rhs) const
	{
		rhs;
		return true;
	}

	bool TestSharedData::operator!=(const TestSharedData& rhs) const
	{
		return !operator==(rhs);
	}

	TestSharedData* TestSharedData::Create() const
	{
		return new TestSharedData();
	}

	bool TestSharedData::Equals(const RTTI* rhs) const
	{
		const TestSharedData* other = rhs->As<TestSharedData>();
		//if other is a right type, compare, else return false
		return (other != nullptr ? *this == *other : false);
	}

	void TestSharedData::Initialize()
	{
		SharedData::Initialize();
		bInitialized = true;
		maxDepth = 0;
	}

	/************************************************************************/
	/**************************Helper Functions******************************/
	/************************************************************************/

	RTTI_DEFINITIONS(TestParseHelper)
	bool TestParseHelper::StartHandler(Library::JsonParseMaster::SharedData& sharedData, const std::string& key, Json::Value& value, bool isArrayElement, size_t arrayIndex)
	{
		key;				//get rid of warnings
		value;				//get rid of warnings
		isArrayElement;		//get rid of warnings
		arrayIndex;			//get rid of warnings

		auto data = sharedData.As<TestSharedData>();
		if (data == nullptr) { return false; }

		++startCalls;

		if (data->Depth() > data->maxDepth)
		{
			data->maxDepth = data->Depth();
		}

		return true;
	}

	bool TestParseHelper::EndHandler(Library::JsonParseMaster::SharedData& sharedData, const std::string& key)
	{
		key; //get rid of warnings
		if (!sharedData.Is(TestSharedData::TypeIdClass())) { return false; }
		endCalls++;
		return true;
	}

	TestParseHelper* TestParseHelper::Create()
	{
		return new TestParseHelper();
	}

	void TestParseHelper::Initialize()
	{
		IJsonParseHelper::Initialize();
		bInitialized = true;
		startCalls = 0;
		endCalls = 0;
	}

	bool TestParseHelper::Equals(const RTTI* rhs) const
	{
		if (rhs == nullptr) { return false; }
		auto helper = rhs->As<TestParseHelper>();
		return helper != nullptr;
	}

}