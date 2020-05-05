#include "pch.h"
#include "TestEntity.h"

namespace UnitTests
{
	RTTI_DEFINITIONS(TestEntity)
		Vector<Signature> TestEntity::Signatures()
	{
		return Vector<Signature>
		{
			Signature("Name", DatumType::String, 1, offsetof(TestEntity, mName)),
			Signature(ACTIONS_STRING, DatumType::Table, START_ACTIONS_CAPACITY, 0)
		};
	}
}
