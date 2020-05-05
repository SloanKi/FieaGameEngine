#include "pch.h"
#include "JsonTableParseHelper.h"
#include "Scope.h"
#include "Factory.h"

namespace Library
{
	/************************************************************************/
	/************************SharedData Functions****************************/
	/************************************************************************/
	RTTI_DEFINITIONS(TableSharedData)

	TableSharedData::TableSharedData(Scope& data) :
		mData(&data)
	{ }

	//private
	TableSharedData::TableSharedData(Scope* data) :
		mData(data)
	{
		assert(mData != nullptr);
		bOwnsData = true;
	}

	TableSharedData::~TableSharedData()
	{
		if (bOwnsData)
		{
			delete(mData);
		}
	}

	gsl::owner<TableSharedData*> TableSharedData::Create() const
	{
		return new TableSharedData(mData->Clone());
	}

	bool TableSharedData::Equals(const RTTI* rhs) const
	{
		return (rhs != nullptr ? rhs->Is(TypeIdClass()) : false);
	}

	Scope& TableSharedData::Data()
	{
		return *mData;
	}

	void TableSharedData::SetData(Scope& data)
	{
		mData = &data;
	}



	/************************************************************************/
	/**************************Helper Functions******************************/
	/************************************************************************/
	RTTI_DEFINITIONS(JsonTableParseHelper)
	const Hashmap<std::string, DatumType> JsonTableParseHelper::mTypes = 
	{ 
		{"integer", DatumType::Integer},
		{"float", DatumType::Float},
		{"vector", DatumType::Vector},
		{"matrix", DatumType::Matrix},
		{"string", DatumType::String},
		{"table", DatumType::Table},
	};

	void JsonTableParseHelper::Initialize()
	{
		IJsonParseHelper::Initialize();
		mContextStack.Clear();
	}

	bool JsonTableParseHelper::StartHandler(JsonParseMaster::SharedData& sharedData, const std::string& key, Json::Value& value, bool isArrayElement, size_t arrayIndex)
	{
		bool parsed = false;
		TableSharedData* tableData = sharedData.As<TableSharedData>();
		if (tableData == nullptr) { return false; }

		if (key == "type" && value.isString())
		{
			auto str = value.asString();
			auto it = mTypes.Find(str);
			if (it != mTypes.end())
			{
				mContextStack.Top().dat->SetType(it->second);
				parsed = true;
			}
			//if type was not valid (not found in hashmap) will return false
		}
		else if (key == "class" && value.isString())
		{
			mContextStack.Top().classKey = value.asString();
		}
		else if (key == "value")
		{
			StackFrame& frame = mContextStack.Top();
			if (frame.dat->Type() == DatumType::Table)
			{
				auto nestedScopeDat = frame.dat;
				Scope* nestedScope;
				if (nestedScopeDat != nullptr && nestedScopeDat->Size() > arrayIndex)
				{
					nestedScope = &nestedScopeDat->Get<Scope>(arrayIndex);
				}
				else 
				{
					//if no class, append a normal scope
					if (frame.classKey.empty())
					{
						nestedScope = &frame.context->AppendScope(frame.key);
					}
					//if class, use factory and adopt
					else
					{
						nestedScope = Factory<Scope>::Create(frame.classKey);
						assert(nestedScope != nullptr);
						frame.context->Adopt(*nestedScope, frame.key);
					}
				}
				if (isArrayElement)
				{
					mContextStack.Push(StackFrame(key, nestedScopeDat, *nestedScope));
				}
				else
				{
					frame.context = nestedScope;
				}
				parsed = true;
			} 
			else if (value.isString())
			{
				//Change grammer so everything isn't string?
				if (frame.dat->Size() <= arrayIndex) 
				{
					frame.dat->Resize(arrayIndex+1, true);
				}
				frame.dat->SetFromStringDanger(value.asString(), arrayIndex);
				parsed = true;
			}
		}
		else //not parsing "type" or "value"
		{
			//if not object, grammar not accepted, will return false
			if (value.isObject())
			{
				Scope* scope = (mContextStack.IsEmpty() ? &tableData->Data() : mContextStack.Top().context);
				Datum& dat = scope->Append(key);
				mContextStack.Push(StackFrame(key, &dat, *scope));
				parsed = true;
			}
		}

		return parsed;
	}

	bool JsonTableParseHelper::EndHandler(JsonParseMaster::SharedData& sharedData, const std::string& key)
	{
		if (!sharedData.Is(TableSharedData::TypeIdClass())) { return false; }
		auto& frameKey = mContextStack.Top().key;
		if (&frameKey == &key)
		{
			mContextStack.Pop();
		}
		return true;
	}

	JsonTableParseHelper* JsonTableParseHelper::Create()
	{
		return new JsonTableParseHelper();
	}

}