#pragma once
#include "RTTI.h"

namespace UnitTests
{
	class Foo final : public Library::RTTI
	{
		RTTI_DECLARATIONS(Foo, Library::RTTI)

	public:
		explicit Foo(int data = 0);
		Foo(const Foo& rhs);
		Foo(Foo&& rhs);
		Foo& operator=(const Foo& rhs);
		Foo& operator=(Foo&& rhs);
		~Foo();

		bool operator==(const Foo& rhs) const noexcept;
		bool operator!=(const Foo& rhs) const noexcept;

		int& Data();
		int Data() const;
		void SetData(int data);

		//RTTI
		bool Equals(const RTTI* rhs) const override;

	private:
		int* mData;
	};
}


namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	inline std::wstring ToString<UnitTests::Foo>(const UnitTests::Foo& t)
	{
		RETURN_WIDE_STRING(t.Data());
	}

	template<>
	inline std::wstring ToString<UnitTests::Foo>(const UnitTests::Foo* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<UnitTests::Foo>(UnitTests::Foo* t)
	{
		RETURN_WIDE_STRING(t);
	}
}