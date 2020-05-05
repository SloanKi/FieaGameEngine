#include "pch.h"
#include "AttributedFoo.h"

namespace UnitTests
{
	RTTI_DEFINITIONS(AttributedFoo)

	AttributedFoo::AttributedFoo() : Attributed(AttributedFoo::TypeIdClass())
	{}

	bool AttributedFoo::Equals(const RTTI* rhs) const
	{
		if (rhs == nullptr) { return false; }
		const AttributedFoo* other = rhs->As<AttributedFoo>();
		//if other is a scope, compare, else return false
		return (other != nullptr ? *this == *other : false);
	}

	gsl::owner<AttributedFoo*> AttributedFoo::Clone() const
	{
		return new AttributedFoo(*this);
	}

	Vector<Signature> AttributedFoo::Signatures()
	{
		return Vector<Signature>
		{
			Signature("ExternalInteger", DatumType::Integer, 1, offsetof(AttributedFoo, ExternalInteger)),
			Signature("ExternalFloat", DatumType::Float, 1, offsetof(AttributedFoo, ExternalFloat)),
			Signature("ExternalVector", DatumType::Vector, 1, offsetof(AttributedFoo, ExternalVector)),
			Signature("ExternalMatrix", DatumType::Matrix, 1, offsetof(AttributedFoo, ExternalMatrix)),
			Signature("ExternalString", DatumType::String, 1, offsetof(AttributedFoo, ExternalString)),
			Signature("ExternalRTTI", DatumType::Pointer, 1, offsetof(AttributedFoo, ExternalRTTI)),
			Signature("InternalScope", DatumType::Table, 1, 0),
			Signature("ExternalIntArr", DatumType::Integer, 5, offsetof(AttributedFoo, ExternalIntArr)),
			Signature("ExternalFloatArr", DatumType::Float, 5, offsetof(AttributedFoo, ExternalFloatArr)),
			Signature("ExternalVecArr", DatumType::Vector, 5, offsetof(AttributedFoo, ExternalVecArr)),
			Signature("ExternalMatArr", DatumType::Matrix, 5, offsetof(AttributedFoo, ExternalMatArr)),
			Signature("ExternalStringArr", DatumType::String, 5, offsetof(AttributedFoo, ExternalStringArr)),
			Signature("InternalScopeArr", DatumType::Table, 5, 0),
		};
	}
}
