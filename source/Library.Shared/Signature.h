#pragma once
#include <string>
#include "Datum.h"

namespace Library
{
	/// <summary>
	/// Signature is used to describe an attribute (datum) of an attributed scope.
	/// Holds the name, type, size, and offset of an attribute
	/// </summary>
	struct Signature final
	{
		Signature() = default;
		Signature(const Signature& rhs) = default;
		Signature(Signature&& rhs) noexcept = default;
		Signature& operator=(const Signature& rhs) = default;
		Signature& operator=(Signature&& rhs) noexcept = default;
		~Signature() = default;

		Signature(std::string name, DatumType type, size_t size, size_t offset)
			: mName(name), mType(type), mSize(size), mOffset(offset) {}

		bool operator==(const Signature& rhs) const noexcept = default;
		bool operator!=(const Signature& rhs) const noexcept = default;
		
		/// <summary>
		/// The string indicating key to use for Datum in its Scope
		/// </summary>
		std::string mName;

		/// <summary>
		/// The datum type
		/// </summary>
		DatumType mType;

		/// <summary>
		/// The number of elements in this datum
		/// </summary>
		size_t mSize;

		/// <summary>
		/// The offset to the class data member (for mapping to external storage)
		/// </summary>
		size_t mOffset;
	};
}