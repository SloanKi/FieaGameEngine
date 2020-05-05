#include "pch.h"
#include "TestAction.h"

namespace UnitTests
{
	RTTI_DEFINITIONS(TestAction)


	TestAction::TestAction(const std::string& name, Entity* entity) :
		Action(TypeIdClass(), name, entity) {}

	TestAction::TestAction(std::string&& name, Entity* entity) :
		Action(TypeIdClass(), std::move(name), entity) {}

	Vector<Signature> TestAction::Signatures()
	{
		return Vector<Signature>
		{
			Signature("Name", DatumType::String, 1, offsetof(TestAction, mName))
		};
	}
}