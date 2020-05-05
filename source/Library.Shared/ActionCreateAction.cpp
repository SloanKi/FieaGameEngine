#include "pch.h"
#include "ActionCreateAction.h"
#include "WorldState.h"
#include "World.h"

namespace Library
{
	RTTI_DEFINITIONS(ActionCreateAction)

	ActionCreateAction::ActionCreateAction() : Action(TypeIdClass(), "CreateAction") { bRunOnce = true; }

	ActionCreateAction::ActionCreateAction(std::string prototype, std::string actionName) :
		Action(TypeIdClass(), "CreateAction"), mPrototype(std::move(prototype)), mActionName(std::move(actionName))
	{
		bRunOnce = true;
	}

	void ActionCreateAction::Update(WorldState & state)
	{
		if (mParent == nullptr)
		{
			throw std::runtime_error("No parent to add action to");
		}
		if (mPrototype.empty() || mActionName.empty())
		{
			throw std::runtime_error("Prototype and ActionName cannot be empty");
		}

		state.mWorld->AddActionToCreateQueue(mPrototype, mActionName, mParent, "Actions");
		QueueDeleteIfRunOnce(state);
	}

	const std::string& ActionCreateAction::GetPrototype() const
	{
		return mPrototype;
	}

	void ActionCreateAction::SetPrototype(std::string prototype)
	{
		mPrototype = std::move(prototype);
	}

	const std::string& ActionCreateAction::GetActionName() const
	{
		return mActionName;
	}

	void ActionCreateAction::SetActionName(std::string actionName)
	{
		mActionName = std::move(actionName);
	}

	gsl::owner<ActionCreateAction*> ActionCreateAction::Clone() const
	{
		return new ActionCreateAction(*this);
	}

	Vector<Signature> ActionCreateAction::Signatures()
	{
		return Vector<Signature>
		{
			Signature("Name", DatumType::String, 1, offsetof(ActionCreateAction, mName)),
			Signature("Prototype", DatumType::String, 1, offsetof(ActionCreateAction, mPrototype)),
			Signature("ActionName", DatumType::String, 1, offsetof(ActionCreateAction, mActionName))
		};
	}
}

