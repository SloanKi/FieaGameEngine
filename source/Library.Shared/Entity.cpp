#include "pch.h"
#include "Entity.h"
#include "Sector.h"
#include "Action.h"
#include "WorldState.h"

namespace Library
{
	RTTI_DEFINITIONS(Entity)

	Entity::Entity() : Attributed(Entity::TypeIdClass()) 
	{
		mActionsDatum = Find(ACTIONS_STRING);
		assert(mActionsDatum != nullptr);
	}
	
	Entity::Entity(const std::string& name, Sector* sector) :
		Attributed(Entity::TypeIdClass()), mName(name) 
	{
		if (sector != nullptr) { SetSector(*sector); }
		mActionsDatum = Find(ACTIONS_STRING);
		assert(mActionsDatum != nullptr);
	}

	Entity::Entity(std::string&& name, Sector* sector) :
		Attributed(Entity::TypeIdClass()), mName(name) 
	{
		if (sector != nullptr) { SetSector(*sector); }
		mActionsDatum = Find(ACTIONS_STRING);
		assert(mActionsDatum != nullptr);
	}

	void Entity::Update(WorldState& state)
	{
		for (size_t i = 0; i < mActionsDatum->Size(); i++)
		{
			Scope& action = mActionsDatum->Get<Scope>(i);
			assert(action.Is(Action::TypeIdClass()));
			state.mAction = &static_cast<Action&>(action);
			static_cast<Action&>(action).Update(state);
		}
		state.mAction = nullptr;
	}

	const std::string& Entity::Name() const noexcept
	{
		return mName;
	}

	void Entity::SetName(const std::string& name) noexcept
	{
		assert(!name.empty());
		mName = name;
	}

	void Entity::SetName(std::string&& name) noexcept
	{
		assert(!name.empty());
		mName = std::move(name); //is std::move needed here?
	}

	Sector* Entity::GetSector() const 
	{
		Scope* parent = GetParent();
		//sanity check: if parent isn't nullptr, make sure it is a sector
		assert(parent != nullptr ? parent->Is(Sector::TypeIdClass()) : true);
		return static_cast<Sector*>(parent);
	}

	void Entity::SetSector(Sector& sector)
	{
		sector.Adopt(*this, Sector::ENTITIES_STRING);
	}

	Datum& Entity::Actions()
	{
		assert(mActionsDatum != nullptr); //sanity check
		return *mActionsDatum;
	}

	const Datum& Entity::Actions() const
	{
		assert(mActionsDatum != nullptr); //sanity check
		return *mActionsDatum;
	}

	Action* Entity::CreateAction(const std::string& className, const std::string& instanceName)
	{
		auto newAction = Factory<Scope>::Create(className)->As<Action>();
		assert(newAction != nullptr);
		newAction->SetName(instanceName);
		AdoptAction(*newAction);
		return newAction;
	}

	void Entity::AdoptAction(Action& action)
	{
		action.SetEntity(*this);
	}

	gsl::owner<Entity*> Entity::Clone() const
	{
		return new Entity(*this);
	}

	Vector<Signature> Entity::Signatures()
	{
		return Vector<Signature>
		{
			Signature("Name", DatumType::String, 1, offsetof(Entity, mName)),
			Signature(ACTIONS_STRING, DatumType::Table, START_ACTIONS_CAPACITY, 0)
		};
	}

	Entity::Entity(RTTI::IdType typeID) :
		Attributed(typeID)
	{
		mActionsDatum = Find(ACTIONS_STRING);
		assert(mActionsDatum != nullptr);
	}

	Entity::Entity(RTTI::IdType typeID, const std::string& name, Sector* sector) :
		Attributed(typeID), mName(name)
	{
		if (sector != nullptr) { SetSector(*sector); }
		mActionsDatum = Find(ACTIONS_STRING);
		assert(mActionsDatum != nullptr);
	}

	Entity::Entity(RTTI::IdType typeID, std::string&& name, Sector* sector) :
		Attributed(typeID), mName(name)
	{
		if (sector != nullptr) { SetSector(*sector); }
		mActionsDatum = Find(ACTIONS_STRING);
		assert(mActionsDatum != nullptr);
	}
}