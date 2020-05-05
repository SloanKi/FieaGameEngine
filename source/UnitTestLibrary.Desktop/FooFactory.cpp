#include "pch.h"
#include "FooFactory.h"
#include "Foo.h"

using namespace Library;

namespace UnitTests
{
	FooFactory::FooFactory() :
		mName("Foo")
	{
		Factory<RTTI>::Add(*this);
	}

	FooFactory::~FooFactory()
	{
		Factory<RTTI>::Remove(*this);
	}

	gsl::owner<RTTI*> FooFactory::Create() const
	{
		return new Foo();
	}

	const std::string& FooFactory::ClassName() const
	{
		return mName;
	}
}

