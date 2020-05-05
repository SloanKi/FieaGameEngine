#include "pch.h"
#include "ActionListSwitch.h"

namespace Library {
	RTTI_DEFINITIONS(ActionListSwitch)

	ActionListSwitch::ActionListSwitch() :
		ActionList(TypeIdClass(), "Switch"), mCaseCache(DEFAULT_CACHE_CAPACITY)
	{
		mActionsDatum = Find(ACTIONS_STRING);
		assert(mActionsDatum != nullptr);
	}

	void ActionListSwitch::Update([[maybe_unused]] WorldState& state)
	{
		//Find the action we want and cache it
		if (bValueChanged)
		{
			//Got cases from parser and needs to be initialized
			//Paul: I was thinking that maybe all actions (and maybe entities?) should have an initialize method
			//that world would call before the first update (like Start in unity or BeginPlay in UE4)
			//Is there a reason we don't have this?
			if (mCaseCache.Size() == 0 && !mActionsDatum->IsEmpty())
			{
				for (size_t i = 0; i < mActionsDatum->Size(); i++)
				{
					auto& caseScope = mActionsDatum->Get<Scope>(i);
					assert(caseScope.Is(Action::TypeIdClass()));
					Action* casePtr = static_cast<Action*>(&caseScope);
					assert(casePtr != nullptr);
					mCaseCache.Insert({ casePtr->Name(), casePtr });
				}
			}

			mCurrentAction = mCaseCache[mValue];
			//Todo: maybe allow it to be nullptr, and have a default case?
			if (mCurrentAction == nullptr)
			{
				throw std::runtime_error("Case does not exist");
			}
			bValueChanged = false;
		}
		assert(mCurrentAction != nullptr);

		//call the action
		mCurrentAction->Update(state);
	}

	void ActionListSwitch::SetValue(std::string value)
	{
		mValue = std::move(value);
		bValueChanged = true;
	}

	const std::string& ActionListSwitch::GetValue() const
	{
		return mValue;
	}

	void ActionListSwitch::AddCase(Action& action, std::string caseName)
	{
		AdoptAction(action);
		mCaseCache.Insert({ std::move(caseName), &action });
	}

	void ActionListSwitch::RemoveCase(const std::string& caseName)
	{
		Action* actionPtr = mCaseCache[caseName];
		//todo: change if/when add garbage collection crap
		if (actionPtr != nullptr)
		{
			actionPtr->Orphan();
			mCaseCache.Remove(caseName);
			delete(actionPtr);
		}
	}

	typename ActionListSwitch::CacheType& ActionListSwitch::GetCases()
	{
		return mCaseCache;
	}

	const typename ActionListSwitch::CacheType& ActionListSwitch::GetCases() const
	{
		return mCaseCache;
	}

	gsl::owner<ActionListSwitch*> ActionListSwitch::Clone() const
	{
		return new ActionListSwitch(*this);
	}

	Vector<Signature> ActionListSwitch::Signatures()
	{
		return Vector<Signature>
		{
			Signature("Name", DatumType::String, 1, offsetof(ActionListSwitch, mName)),
			Signature(ACTIONS_STRING, DatumType::Table, START_ACTIONS_CAPACITY, 0),
			Signature("CaseValue", DatumType::String, 1, offsetof(ActionListSwitch, mValue))
		};
	}
}