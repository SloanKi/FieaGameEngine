#include "pch.h"
#include "ActionEvent.h"
#include "Event.h"
#include "EventMessageAttributed.h"
#include "WorldState.h"
#include "World.h"

namespace Library
{
	RTTI_DEFINITIONS(ActionEvent)

	ActionEvent::ActionEvent() : Action(TypeIdClass(), "ActionEvent") {}


	const std::string& ActionEvent::GetSubtype() const
	{
		return mSubtype;
	}

	void ActionEvent::SetSubtype(std::string subtype)
	{
		mSubtype = std::move(subtype);
	}

	int ActionEvent::GetDelay() const
	{
		return mDelay;
	}

	void ActionEvent::SetDelay(int delay)
	{
		mDelay = delay;
	}

	void ActionEvent::Update(WorldState& state)
	{
		assert(state.mWorld != nullptr); //sanity/protect ptrs

		if (mSubtype == "" || mDelay < 0)
		{
			throw std::runtime_error("Subtype or Delay in ActionEvent is invalid");
		}

		//create ema and assign world, subtype
		EventMessageAttributed eventMsg(mSubtype, state.mWorld);

		//copy aux attributes into EMA
		//Start at 4 because indexes 0-3 are prescribed by this, name, subtype, and delay
		for (size_t i = 4; i < mOrderVector.Size(); i++)
		{
			auto& argument = mOrderVector[i]; 
			eventMsg.Append(argument->first) = argument->second;
		}

		//create event sharedptr
		auto event = std::make_shared<Event<EventMessageAttributed>>(std::move(eventMsg));

		//queue the event with given delay
		state.mWorld->GetEventQueue().Enqueue(event, std::chrono::milliseconds(mDelay));

		QueueDeleteIfRunOnce(state);
	}

	gsl::owner<ActionEvent*> ActionEvent::Clone() const
	{
		return new ActionEvent(*this);
	}

	Vector<Signature> ActionEvent::Signatures()
	{
		return Vector<Signature>
		{
			Signature("Name", DatumType::String, 1, offsetof(ActionEvent, mName)),
			Signature("Subtype", DatumType::String, 1, offsetof(ActionEvent, mSubtype)),
			Signature("Delay", DatumType::Integer, 1, offsetof(ActionEvent, mDelay))
		};
	}

}