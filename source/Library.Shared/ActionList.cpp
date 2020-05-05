#include "pch.h"
#include "ActionList.h"
#include "WorldState.h"


namespace Library
{
	RTTI_DEFINITIONS(ActionList)

	ActionList::ActionList() : Action(TypeIdClass())
	{
		mActionsDatum = Find(ACTIONS_STRING);
		assert(mActionsDatum != nullptr);
	}

	ActionList::ActionList(const std::string& name, Entity* entity) :
		Action(TypeIdClass(), name, entity)
	{
		mActionsDatum = Find(ACTIONS_STRING);
		assert(mActionsDatum != nullptr);
	}

	ActionList::ActionList(std::string&& name, Entity* entity) :
		Action(TypeIdClass(), std::move(name), entity)
	{
		mActionsDatum = Find(ACTIONS_STRING);
		assert(mActionsDatum != nullptr);
	}

	void ActionList::Update(WorldState& state)
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

	Datum& ActionList::Actions()
	{
		assert(mActionsDatum != nullptr); //sanity check
		return *mActionsDatum;
	}

	const Datum& ActionList::Actions() const
	{
		assert(mActionsDatum != nullptr); //sanity check
		return *mActionsDatum;
	}

	Action* ActionList::CreateAction(const std::string& className, const std::string& instanceName)
	{
		auto newAction = Factory<Scope>::Create(className)->As<Action>();
		assert(newAction != nullptr);
		newAction->SetName(instanceName);
		AdoptAction(*newAction);
		return newAction;
	}

	void ActionList::AdoptAction(Action& action)
	{
		Adopt(action, ACTIONS_STRING);
	}

	gsl::owner<ActionList*> ActionList::Clone() const
	{
		return new ActionList(*this);
	}

	Vector<Signature> ActionList::Signatures()
	{
		return Vector<Signature>
		{
			Signature("Name", DatumType::String, 1, offsetof(ActionList, mName)),
			Signature(ACTIONS_STRING, DatumType::Table, START_ACTIONS_CAPACITY, 0)
		};
	}

	//protected
	ActionList::ActionList(RTTI::IdType typeID) : Action(typeID)
	{
		mActionsDatum = Find(ACTIONS_STRING);
		assert(mActionsDatum != nullptr);
	}

	//protected
	ActionList::ActionList(RTTI::IdType typeID, const std::string& name, Entity* entity) :
		Action(typeID, name, entity)
	{
		mActionsDatum = Find(ACTIONS_STRING);
		assert(mActionsDatum != nullptr);
	}

	//protected
	ActionList::ActionList(RTTI::IdType typeID, std::string&& name, Entity* entity) :
		Action(typeID, std::move(name), entity)
	{
		mActionsDatum = Find(ACTIONS_STRING);
		assert(mActionsDatum != nullptr);
	}
}