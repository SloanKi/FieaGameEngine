#pragma once
#include "Factory.h"
#include "RTTI.h"

using namespace Library;


namespace UnitTests
{
	class FooFactory final : public Factory<RTTI>
	{
	public:
		FooFactory();
		FooFactory(const FooFactory& rhs) = delete;
		FooFactory(FooFactory&& rhs) noexcept = delete;
		~FooFactory();
		FooFactory& operator=(const FooFactory& rhs) = delete;
		FooFactory& operator=(FooFactory&& rhs) noexcept = delete;

		gsl::owner<RTTI*> Create() const override;
		const std::string& ClassName() const override;
	private:
		const std::string mName;
	};

}

