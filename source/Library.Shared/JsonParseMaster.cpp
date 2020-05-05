#include "pch.h"
#include "IJsonParseHelper.h"
#include "JsonParseMaster.h"

namespace Library
{
	/************************************************************************/
	/************************SharedData Functions****************************/
	/************************************************************************/
	RTTI_DEFINITIONS(JsonParseMaster::SharedData)

	void JsonParseMaster::SharedData::Initialize()
	{
		mDepth = 0;
	}

	JsonParseMaster* JsonParseMaster::SharedData::GetJsonParseMaster() const
	{
		return mParseMaster;
	}

	bool JsonParseMaster::SharedData::operator==(const SharedData& rhs) const
	{
		return (mParseMaster == rhs.mParseMaster && mDepth == rhs.mDepth);
	}

	bool JsonParseMaster::SharedData::operator!=(const SharedData& rhs) const
	{
		return !operator==(rhs);
	}

	void JsonParseMaster::SharedData::SetJsonParseMaster(JsonParseMaster& master)
	{
		mParseMaster = &master;
	}

	void JsonParseMaster::SharedData::IncrementDepth()
	{
		mDepth++;
	}

	void JsonParseMaster::SharedData::DecrementDepth()
	{
		assert(mDepth > 0);
		mDepth--;
	}

	size_t JsonParseMaster::SharedData::Depth() const
	{
		return mDepth;
	}

	bool JsonParseMaster::SharedData::Equals(const RTTI* rhs) const
	{
		if (rhs == nullptr) { return false; }
		const JsonParseMaster::SharedData* other = rhs->As<JsonParseMaster::SharedData>();
		//if other is a shared data, compare, else return false
		return (other != nullptr ? *this == *other : false);
	}

	/************************************************************************/
	/************************ParseMaster Functions***************************/
	/************************************************************************/
	JsonParseMaster::JsonParseMaster(SharedData& data) :
		mData(&data)
	{
		mData->SetJsonParseMaster(*this);
	}

	JsonParseMaster::JsonParseMaster(JsonParseMaster&& rhs) noexcept :
		mData(std::move(rhs.mData)), mHelpers(std::move(rhs.mHelpers)), bIsClone(rhs.bIsClone)
	{
		mData->SetJsonParseMaster(*this); //reparent the shared data
		rhs.mData = nullptr; //invalidate rhs
		rhs.bIsClone = false;
	}

	JsonParseMaster::~JsonParseMaster()
	{
		//clone owns its shared data and helpers
		if (bIsClone)
		{
			delete(mData);
			for (auto helper : mHelpers)
			{
				delete(helper);
			}
		}
	}

	JsonParseMaster& JsonParseMaster::operator=(JsonParseMaster&& rhs) noexcept
	{
		if (this != &rhs)
		{
			//if a clone, you own data. Properly dispose before moving in new data
			if (bIsClone)
			{
				delete(mData);
				for (auto helper : mHelpers)
				{
					delete(helper);
				}
			}

			mData = std::move(rhs.mData);
			mHelpers = std::move(rhs.mHelpers);
			bIsClone = rhs.bIsClone;
			mData->SetJsonParseMaster(*this); //reparent the shared data

			rhs.mData = nullptr; //invalidate rhs
		}
		return *this;
	}

	void JsonParseMaster::Initialize()
	{
		mData->Initialize();
		for (auto& helper : mHelpers)
		{
			helper->Initialize();
		}
		mFileName = "";
	}

	JsonParseMaster* JsonParseMaster::Clone() const
	{
		auto newData = mData->Create();
		auto clone = new JsonParseMaster(*newData);
		for (auto helper : mHelpers)
		{
			auto helperClone = helper->Create();
			clone->AddHelper(*helperClone);
		}
		clone->bIsClone = true;

		return clone;
	}

	void JsonParseMaster::AddHelper(IJsonParseHelper& helper)
	{
		mHelpers.PushBack(&helper);
	}

	void JsonParseMaster::RemoveHelper(IJsonParseHelper& helper)
	{
		mHelpers.Remove(mHelpers.Find(&helper));
	}

	bool JsonParseMaster::Parse(const std::string& data)
	{
		bool parsed;
		//parse json from string
		JSONCPP_STRING err;
		Json::Value root;
		Json::CharReaderBuilder builder;
		std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
		parsed = reader->parse(data.c_str(), data.c_str() + data.length(), &root, &err);

		//parse members from json
		if (parsed)
		{
			mData->IncrementDepth();
			parsed = ParseMembers(root);
			mData->DecrementDepth();
		}
		return parsed;
	}

	
	bool JsonParseMaster::Parse(std::istream& data)
	{
		bool parsed;
		//parse json from string
		JSONCPP_STRING err;
		Json::Value root;
		Json::CharReaderBuilder builder;
		parsed = parseFromStream(builder, data, &root, &err);

		//parse members from json
		if (parsed)
		{
			mData->IncrementDepth();
			parsed = ParseMembers(root);
			mData->DecrementDepth();
		}

		return parsed;
	}

	bool JsonParseMaster::ParseFromFile(const std::string& filename)
	{
		//Initialize();
		bool parsed = false;
		mFileName = filename;

		std::ifstream file(filename);
		if (file.good())
		{
			parsed = Parse(file);
		}

		return parsed;
	}

	const std::string& JsonParseMaster::GetFileName() const
	{
		return mFileName;
	}

	JsonParseMaster::SharedData* JsonParseMaster::GetSharedData()
	{
		return mData;
	}

	const JsonParseMaster::SharedData* JsonParseMaster::GetSharedData() const
	{
		return mData;
	}

	void JsonParseMaster::SetSharedData(SharedData& data)
	{
		//todo: ask paul what should happen with a clone.
		//Should the clone delete it's data and accept this sharedData?
		//If so should there be a second bool indicating that it owns sharedData?
		mData = &data;
		data.SetJsonParseMaster(*this);
	}

	bool JsonParseMaster::ParseMembers(Json::Value& root, bool isArr, size_t index)
	{
		bool parsed = false;
		auto memberNames = root.getMemberNames();
		for (auto& member : memberNames)
		{
			parsed = Parse(member, root[member], isArr, index);
		}

		return parsed;
	}

	bool JsonParseMaster::Parse(const std::string& key, Json::Value& value, bool isArr, size_t index)
	{
		assert(mData != nullptr); //shouldn't even be possible but sanity check
		bool parsed = false;

		if (value.isObject())
		{
			mData->IncrementDepth();
			for (auto& helper : mHelpers)
			{
				if (helper->StartHandler(*mData, key, value, isArr, index))
				{
					parsed = ParseMembers(value);
					if (parsed) { parsed = helper->EndHandler(*mData, key); }
					break;
				}
			}
			mData->DecrementDepth();
		}
		else if (value.isArray())
		{
			//for (auto& element : value)
			for (unsigned int i = 0; i < value.size(); i++)
			{
				if (value[i].isObject())
				{
					mData->IncrementDepth();
					parsed = ParseMembers(value[i], true, i);
					mData->DecrementDepth();
				}
				else
				{
					parsed = Parse(key, value[i], true, i);
				}
			}
		}
		else //is primitive
		{
			for (auto& helper : mHelpers)
			{
				if (helper->StartHandler(*mData, key, value, isArr, index))
				{
					parsed = helper->EndHandler(*mData, key);
					break;
				}
			}
		}

		return parsed;
	}





}
