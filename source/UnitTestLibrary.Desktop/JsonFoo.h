#pragma once
#include <string>
#include "JsonParseMaster.h"
#include "IJsonParseHelper.h"
#include "vector.h"
//Used for original testing of parsemaster and helpers
namespace UnitTests
{
	//using SharedData = Library::JsonParseMaster::SharedData;

	class JsonFoo
	{
	public:
		JsonFoo();
		JsonFoo(const JsonFoo& rhs) = delete;
		JsonFoo(JsonFoo&& rhs) noexcept = default;
		~JsonFoo() = default;
		JsonFoo& operator=(const JsonFoo& rhs) = default;
		JsonFoo& operator=(JsonFoo&& rhs) noexcept = default;
		
		int mHealth = 0;
		float mDPS = 0.0f;
		Library::Vector<std::string> mAliases;
		bool bStunned = false;
	};

	class FooJsonParseHelper : public Library::IJsonParseHelper
	{
		RTTI_DECLARATIONS(FooJsonParseHelper, Library::IJsonParseHelper)
	public:
		FooJsonParseHelper() = default;
		FooJsonParseHelper(const FooJsonParseHelper& rhs) = delete;
		FooJsonParseHelper(FooJsonParseHelper&& rhs) noexcept = default;
		~FooJsonParseHelper() = default;
		FooJsonParseHelper& operator=(const FooJsonParseHelper& rhs) = delete;
		FooJsonParseHelper& operator=(FooJsonParseHelper&& rhs) noexcept = default;

		bool StartHandler(Library::JsonParseMaster::SharedData& sharedData, const std::string& key, Json::Value& value, bool isArrayElement = false, size_t arrayIndex = 0) override;
		bool EndHandler(Library::JsonParseMaster::SharedData& sharedData, const std::string& key);
		FooJsonParseHelper* Create() override;

		bool Equals(const RTTI* rhs) const override;
	};

	class FooSharedData : public Library::JsonParseMaster::SharedData
	{
		RTTI_DECLARATIONS(FooSharedData, SharedData)
	public:
		FooSharedData(JsonFoo& data);
		FooSharedData(const FooSharedData& rhs) = default;
		FooSharedData(FooSharedData&& rhs) noexcept = default;
		~FooSharedData();
		FooSharedData& operator=(const FooSharedData& rhs) = default;
		FooSharedData& operator=(FooSharedData&& rhs) noexcept = default;

		bool operator==(const FooSharedData& rhs) const;
		bool operator!=(const FooSharedData& rhs) const;

		FooSharedData* Create() const override;

		bool Equals(const RTTI* rhs) const override;

		JsonFoo* mData;

	private:
		FooSharedData(JsonFoo* data);
		bool bOwnsData = false;
	};
}

