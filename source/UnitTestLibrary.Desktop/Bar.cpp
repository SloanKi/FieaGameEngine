#include "pch.h"
#include "Bar.h"

namespace UnitTests
{
	RTTI_DEFINITIONS(Bar)

		//constructor
		Bar::Bar(int data) :
		mData(new int(data))
	{
	}

	//copy constructor
	Bar::Bar(const Bar& rhs) :
		mData(new int(*rhs.mData))
	{
	}

	//move constructor 
	Bar::Bar(Bar&& rhs) :
		mData(rhs.mData)
	{
		rhs.mData = nullptr;
	}


	//assignment operator
	Bar& Bar::operator=(const Bar& rhs)
	{
		if (this != &rhs)				//self assignment test
		{
			*mData = *rhs.mData;		//mData was already assigned, so instead of deleting it, just overwrite the memory with the new data
		}

		return *this;
	}

	Bar& Bar::operator=(Bar&& rhs)
	{
		if (this != &rhs)
		{
			delete mData;
			mData = rhs.mData;
			rhs.mData = nullptr;
		}

		return *this;
	}

	Bar::~Bar()
	{
		delete mData;
	}

	bool Bar::operator==(const Bar& rhs) const noexcept
	{
		return *mData == *rhs.mData;
	}

	bool Bar::operator!=(const Bar& rhs) const noexcept
	{
		return !operator==(rhs);
	}

	int& Bar::Data()
	{
		return *mData;
	}

	int Bar::Data() const
	{
		return *mData;
	}

	void Bar::SetData(int data)
	{
		*mData = data;
	}

	bool Bar::Equals(const RTTI* rhs) const
	{
		if (rhs == nullptr) { return false; }
		const Bar* other = rhs->As<Bar>();
		//if other is a foo, compare, else return false
		return (other != nullptr ? *this == *other : false);
	}
}
