#include "pch.h"
#include "Action.h"
#include "Entity.h"
#include "ActionList.h"
#include "World.h"

namespace Library
{
	RTTI_DEFINITIONS(Action)

	//No default because base is abstract, and children MUST pass their RTTI type id
	/*Action::Action() : Attributed(Action::TypeIdClass()) {}*/

	Action::Action(RTTI::IdType typeID) : Attributed(typeID) {}

	Action::Action(RTTI::IdType typeID, const std::string& name, Entity* entity) :
		Attributed(typeID), mName(name)
	{
		if (entity != nullptr) { SetEntity(*entity); }
	}

	Action::Action(RTTI::IdType typeID, std::string&& name, Entity* entity) :
		Attributed(typeID), mName(name)
	{
		if (entity != nullptr) { SetEntity(*entity); }
	}

	void Action::QueueDeleteIfRunOnce(WorldState& state)
	{
		if (bRunOnce)
		{
			state.mWorld->AddActionToDestroyQueue(*this);
		}
	}

	const std::string& Action::Name() const noexcept
	{
		return mName;
	}

	void Action::SetName(const std::string& name) noexcept
	{
		assert(!name.empty());
		mName = name;
	}

	void Action::SetName(std::string&& name) noexcept
	{
		assert(!name.empty());
		mName = std::move(name); //is std::move needed here?
	}

	//todo: maybe change this. Parent doesn't always have to be entity. So do an As, which will return nullptr if it isn't.
	Entity* Action::GetEntity() const
	{
		Scope* parent = GetParent();
		if (parent == nullptr) { return nullptr; } //if no parent, there is no entity. Return nullptr.

		//if parent is an ActionList, go up the hierarchy and see if you can find an entity
		if (parent->Is(ActionList::TypeIdClass())) 
		{ 
			return static_cast<ActionList*>(parent)->GetEntity(); 
		}

		//if parent is not an actionList, then return it as an entity (will return nullptr if not entity)
		return parent->As<Entity>();
	}

	void Action::SetEntity(Entity& entity)
	{
		entity.Adopt(*this, Entity::ACTIONS_STRING);
	}

	ActionList* Action::GetActionList() const
	{
		Scope* parent = GetParent();
		return (parent != nullptr ? parent->As<ActionList>() : nullptr);
	}

	void Action::SetActionList(ActionList& actionList)
	{
		actionList.AdoptAction(*this);
	}

	/*gsl::owner<Action*> Action::Clone() const
	{
		return new Action(*this);
	}*/

	Vector<Signature> Action::Signatures()
	{
		return Vector<Signature>
		{
			Signature("Name", DatumType::String, 1, offsetof(Action, mName))
		};
	}


}
