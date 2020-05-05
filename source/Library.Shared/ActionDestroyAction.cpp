#include "pch.h"
#include "ActionDestroyAction.h"
#include "World.h"
#include "WorldState.h"

namespace Library
{
	RTTI_DEFINITIONS(ActionDestroyAction)

		ActionDestroyAction::ActionDestroyAction() : Action(TypeIdClass(), "DestroyAction") {}

	ActionDestroyAction::ActionDestroyAction(std::string actionName) :
		Action(TypeIdClass(), "DestroyAction"), mActionName(std::move(actionName))
	{}

	void ActionDestroyAction::Update(WorldState& state)
	{
		if (!bHasUpdated)
		{
			if (mParent == nullptr)
			{
				throw std::runtime_error("No parent to remove action from");
			}
			if (mActionName.empty())
			{
				throw std::runtime_error("Action name cannot be empty");
			}

			//find action
			auto actionDatum = Search("Actions");
			for (size_t i = 0; i < actionDatum->Size(); i++)
			{
				Scope& actionScope = actionDatum->Get<Scope>(i);
				assert(actionScope.Is(Action::TypeIdClass()));
				Action* action = static_cast<Action*>(&actionScope);
				//remove action
				if (action->Name() == mActionName)
				{
					state.mWorld->AddActionToDestroyQueue(*action);
					break;
				}
			}
		}
		bHasUpdated = true;
		state.mWorld->AddActionToDestroyQueue(*this);
	}

	const std::string& ActionDestroyAction::GetActionName() const
	{
		return mActionName;
	}

	void ActionDestroyAction::SetActionName(std::string actionName)
	{
		mActionName = std::move(actionName);
	}

	gsl::owner<ActionDestroyAction*> ActionDestroyAction::Clone() const
	{
		return new ActionDestroyAction(*this);
	}

	Vector<Signature> ActionDestroyAction::Signatures()
	{
		return Vector<Signature>
		{
			Signature("Name", DatumType::String, 1, offsetof(ActionDestroyAction, mName)),
			Signature("ActionName", DatumType::String, 1, offsetof(ActionDestroyAction, mActionName))
		};
	}
}