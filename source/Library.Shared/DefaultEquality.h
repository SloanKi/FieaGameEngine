#pragma once

namespace Library
{
	template <typename T>
	struct DefaultEquality final
	{
		bool operator()(const T& lhs, const T& rhs) const;
	};

	template <>
	struct DefaultEquality<char*> final
	{
		bool operator()(const char* lhs, const char* rhs) const;
	};

	template <>
	struct DefaultEquality<const char*> final
	{
		bool operator()(const char* lhs, const char* rhs) const;
	};

	template <>
	struct DefaultEquality<const char* const> final
	{
		bool operator()(const char* lhs, const char* rhs) const;
	};

	template <>
	struct DefaultEquality<char* const> final
	{
		bool operator()(const char* lhs, const char* rhs) const;
	};
}

#include "DefaultEquality.inl"