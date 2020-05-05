#pragma once
#include "JsonParseMaster.h"
#include <string>
#include <json/json.h>
#include "RTTI.h"


namespace Library
{
	class IJsonParseHelper : public RTTI
	{
		RTTI_DECLARATIONS(IJsonParseHelper, RTTI)
	public:
		IJsonParseHelper() = default;
		IJsonParseHelper(const IJsonParseHelper&) = delete;
		IJsonParseHelper(IJsonParseHelper&&) = default;
		~IJsonParseHelper() = default;
		IJsonParseHelper& operator=(const IJsonParseHelper&) = delete;
		IJsonParseHelper& operator=(IJsonParseHelper&&) = default;


		virtual void Initialize() {};
		virtual bool StartHandler(JsonParseMaster::SharedData& sharedData, const std::string& key, Json::Value& value, bool isArrayElement = false, size_t arrayIndex = 0) = 0;
		virtual bool EndHandler(JsonParseMaster::SharedData& sharedData, const std::string& key) = 0;
		virtual IJsonParseHelper* Create() = 0;

		bool Equals(const RTTI* rhs) const override;
	};
}

