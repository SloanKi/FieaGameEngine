#include "pch.h"
#include "Foo.h"

namespace UnitTests
{
	RTTI_DEFINITIONS(Foo)

	//constructor
	Foo::Foo(int data) :
		mData(new int(data))
	{
	}

	//copy constructor
	Foo::Foo(const Foo& rhs) :
		mData(new int(*rhs.mData))
	{
	}

	//move constructor 
	Foo::Foo(Foo&& rhs) :
		mData(rhs.mData)
	{
		rhs.mData = nullptr;
	}


	//assignment operator
	Foo& Foo::operator=(const Foo& rhs)
	{
		if (this != &rhs)				//self assignment test
		{
			*mData = *rhs.mData;		//mData was already assigned, so instead of deleting it, just overwrite the memory with the new data
		}

		return *this;
	}

	Foo& Foo::operator=(Foo&& rhs)
	{
		if (this != &rhs)
		{
			delete mData;
			mData = rhs.mData;
			rhs.mData = nullptr;
		}

		return *this;
	}

	Foo::~Foo()
	{
		delete mData;
	}

	bool Foo::operator==(const Foo& rhs) const noexcept
	{
		return *mData == *rhs.mData;
	}

	bool Foo::operator!=(const Foo& rhs) const noexcept
	{
		return !operator==(rhs);
	}

	int& Foo::Data()
	{
		return *mData;
	}

	int Foo::Data() const
	{
		return *mData;
	}

	void Foo::SetData(int data)
	{
		*mData = data;
	}

	bool Foo::Equals(const RTTI* rhs) const
	{
		if (rhs == nullptr) { return false; }
		const Foo* other = rhs->As<Foo>();
		//if other is a foo, compare, else return false
		return (other != nullptr ? *this == *other : false);
	}
}