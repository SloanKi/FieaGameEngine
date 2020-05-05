#pragma once
#include "Entity.h"
#include "Factory.h"

using namespace Library;

namespace UnitTests
{
	class TestEntity final : public Entity
	{
		RTTI_DECLARATIONS(TestEntity, Entity)
	public:
		TestEntity() : Entity(TypeIdClass()) {};
		explicit TestEntity(const std::string& name, Sector* sector = nullptr) : Entity(TypeIdClass(), name, sector) {};
		explicit TestEntity(std::string&& name, Sector* sector = nullptr) : Entity(TypeIdClass(), std::move(name), sector) {};
		void Update(WorldState& state) override { state;  mUpdateCount++; }; //note does not update actions here
		int mUpdateCount = 0;
		virtual gsl::owner<TestEntity*> Clone() const override { return new TestEntity(*this); };
		static Vector<Signature> Signatures();
	};
	ConcreteFactory(TestEntity, Scope)
}

