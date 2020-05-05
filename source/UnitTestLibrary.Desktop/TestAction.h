#pragma once
#include "Action.h"
#include "Factory.h"

using namespace Library;

namespace UnitTests
{
	class TestAction final : public Action
	{
		RTTI_DECLARATIONS(TestAction, Action)
	public:
		TestAction() : Action(TypeIdClass()) {};
		explicit TestAction(const std::string& name, Entity* entity = nullptr); 
		explicit TestAction(std::string&& name, Entity* entity = nullptr); 
		void Update(WorldState& state) override { state; mUpdateCount++; };
		virtual gsl::owner<TestAction*> Clone() const override { return new TestAction(*this); };
		int mUpdateCount = 0;

		static Vector<Signature> Signatures();
	};
	ConcreteFactory(TestAction, Scope)
}

