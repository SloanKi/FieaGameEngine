#pragma once
#include "IJsonParseHelper.h"
#include "Stack.h"
#include <string>
#include "Hashmap.h"
#include "Datum.h"
#include <gsl/gsl>

namespace Library
{
	class Scope;
	class Datum;
	class TableSharedData final : public Library::JsonParseMaster::SharedData
	{
		RTTI_DECLARATIONS(TableSharedData, SharedData)
	public:
		TableSharedData(Scope& data);
		TableSharedData(const TableSharedData& rhs) = default;
		TableSharedData(TableSharedData&& rhs) noexcept = default;
		~TableSharedData();
		TableSharedData& operator=(const TableSharedData& rhs) = default;
		TableSharedData& operator=(TableSharedData&& rhs) noexcept = default;

		gsl::owner<TableSharedData*> Create() const override;

		bool Equals(const RTTI* rhs) const override;

		Scope& Data();

		void SetData(Scope& data);

	private:
		TableSharedData(Scope* data);
		Scope* mData;
		bool bOwnsData = false;
	};


	class JsonTableParseHelper final : public IJsonParseHelper
	{
		RTTI_DECLARATIONS(JsonTableParseHelper, IJsonParseHelper)
	public:
		JsonTableParseHelper() = default;
		JsonTableParseHelper(const JsonTableParseHelper&) = delete;
		JsonTableParseHelper(JsonTableParseHelper&&) = default;
		~JsonTableParseHelper() = default;
		JsonTableParseHelper& operator=(const JsonTableParseHelper&) = delete;
		JsonTableParseHelper& operator=(JsonTableParseHelper&&) = default;


		void Initialize() override;
		bool StartHandler(JsonParseMaster::SharedData & sharedData, const std::string & key, Json::Value & value, bool isArrayElement = false, size_t arrayIndex = 0) override;
		bool EndHandler(JsonParseMaster::SharedData & sharedData, const std::string & key) override;
		JsonTableParseHelper* Create();

	private:
		struct StackFrame
		{
			Scope* context = nullptr;
			const std::string& key;
			Datum* dat = nullptr;
			std::string classKey;

			StackFrame(const std::string& keyIn, Datum* datIn, Scope& scope) : key(keyIn), dat(datIn), context(&scope) {};
		};
		Stack<StackFrame> mContextStack;
		static const Hashmap<std::string, DatumType> mTypes;
	};
}

