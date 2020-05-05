#pragma once
#include "JsonParseMaster.h"
#include "IJsonParseHelper.h"

namespace UnitTests
{
	class TestParseHelper : public Library::IJsonParseHelper
	{
		RTTI_DECLARATIONS(TestParseHelper, Library::IJsonParseHelper)
	public:
		TestParseHelper() = default;
		TestParseHelper(const TestParseHelper & rhs) = delete;
		TestParseHelper(TestParseHelper && rhs) noexcept = default;
		~TestParseHelper() = default;
		TestParseHelper& operator=(const TestParseHelper & rhs) = delete;
		TestParseHelper& operator=(TestParseHelper && rhs) noexcept = default;

		bool StartHandler(Library::JsonParseMaster::SharedData & sharedData, const std::string & key, Json::Value & value, bool isArrayElement = false, size_t arrayIndex = 0) override;
		bool EndHandler(Library::JsonParseMaster::SharedData & sharedData, const std::string & key) override;
		TestParseHelper* Create() override;
		void Initialize() override;
		bool Equals(const RTTI* rhs) const override;

		int startCalls = 0;
		int endCalls = 0;
		bool bInitialized = false;
	};

	class TestSharedData : public Library::JsonParseMaster::SharedData
	{
		RTTI_DECLARATIONS(TestSharedData, SharedData)
	public:
		TestSharedData() = default;
		TestSharedData(const TestSharedData& rhs) = default;
		TestSharedData(TestSharedData&& rhs) noexcept = default;
		~TestSharedData() = default;
		TestSharedData& operator=(const TestSharedData& rhs) = default;
		TestSharedData& operator=(TestSharedData&& rhs) noexcept = default;

		bool operator==(const TestSharedData& rhs) const;
		bool operator!=(const TestSharedData& rhs) const;

		TestSharedData* Create() const override;

		bool Equals(const RTTI* rhs) const override;

		void Initialize() override;
		
		bool bInitialized = false;
		size_t maxDepth = 0;
	};
}

