#include "pch.h"
#include "ActionIncrement.h"

namespace Library
{
	RTTI_DEFINITIONS(ActionIncrement)

	ActionIncrement::ActionIncrement() : Action(TypeIdClass(), "Increment") {}


	void ActionIncrement::Update([[maybe_unused]] WorldState& state)
	{
		//if operand not yet set, search for it.
		if (mOperandDatum == nullptr)
		{
			mOperandDatum = Search(mOperandString);
			if (mOperandDatum == nullptr)
			{
				throw std::runtime_error("Operand not found");
			}
		}

		//todo: maybe cache another int reference so don't have to do the checks every time?
		mOperandDatum->Get<int>() += mStep;
	}

	void ActionIncrement::SetOperandString(const std::string& operand)
	{
		mOperandString = operand;
	}

	const std::string& ActionIncrement::GetOperandString() const
	{
		return mOperandString;
	}

	void ActionIncrement::SetStep(int step)
	{
		mStep = step;
	}

	int ActionIncrement::GetStep() const
	{
		return mStep;
	}

	gsl::owner<ActionIncrement*> ActionIncrement::Clone() const
	{
		return new ActionIncrement(*this);
	}

	Vector<Signature> ActionIncrement::Signatures()
	{
		return Vector<Signature>
		{
			Signature("Name", DatumType::String, 1, offsetof(ActionIncrement, mName)),
			Signature("Operand", DatumType::String, 1, offsetof(ActionIncrement, mOperandString)),
			Signature("Step", DatumType::Integer, 1, offsetof(ActionIncrement, mStep))
		};
	}
}