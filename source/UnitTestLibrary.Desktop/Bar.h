#pragma once
#include "RTTI.h"

namespace UnitTests
{
	class Bar final : public Library::RTTI
	{
		RTTI_DECLARATIONS(Bar, Library::RTTI)

	public:
		explicit Bar(int data = 0);
		Bar(const Bar& rhs);
		Bar(Bar&& rhs);
		Bar& operator=(const Bar& rhs);
		Bar& operator=(Bar&& rhs);
		~Bar();

		bool operator==(const Bar& rhs) const noexcept;
		bool operator!=(const Bar& rhs) const noexcept;

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
	inline std::wstring ToString<UnitTests::Bar>(const UnitTests::Bar& t)
	{
		RETURN_WIDE_STRING(t.Data());
	}

	template<>
	inline std::wstring ToString<UnitTests::Bar>(const UnitTests::Bar* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<UnitTests::Bar>(UnitTests::Bar* t)
	{
		RETURN_WIDE_STRING(t);
	}
}
