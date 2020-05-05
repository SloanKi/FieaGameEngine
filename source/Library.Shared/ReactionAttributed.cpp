#include "pch.h"
#include "ReactionAttributed.h"
#include "Event.h"
#include "EventMessageAttributed.h"
#include "World.h"

namespace Library
{
	RTTI_DEFINITIONS(ReactionAttributed)

	ReactionAttributed::ReactionAttributed() :
		Reaction(TypeIdClass())
	{
		Event<EventMessageAttributed>::Subscribe(*this);
	}

	ReactionAttributed::ReactionAttributed(std::string subtype) :
		Reaction(TypeIdClass()), mSubtype(std::move(subtype))
	{
		Event<EventMessageAttributed>::Subscribe(*this);
	}

	ReactionAttributed::ReactionAttributed(std::string subtype, std::string name, Entity* entity) :
		Reaction(TypeIdClass(), std::move(name), entity), mSubtype(subtype)
	{
		Event<EventMessageAttributed>::Subscribe(*this);
	}

	ReactionAttributed::ReactionAttributed(const ReactionAttributed& rhs) :
		Reaction(rhs), mSubtype(rhs.mSubtype)
	{
		Event<EventMessageAttributed>::Subscribe(*this);
	}

	ReactionAttributed::ReactionAttributed(ReactionAttributed&& rhs) noexcept :
		Reaction(std::move(rhs)), mSubtype(std::move(rhs.mSubtype))
	{
		Event<EventMessageAttributed>::Unsubscribe(rhs);
		Event<EventMessageAttributed>::Subscribe(*this);
	}

	ReactionAttributed::~ReactionAttributed()
	{
		Event<EventMessageAttributed>::Unsubscribe(*this);
	}

	void ReactionAttributed::Notify(const EventPublisher& event)
	{
		//accept only eventmessageattributed events
		assert(event.Is(Event<EventMessageAttributed>::TypeIdClass())); //only subscribing to <EventMessageAttributed>
		auto& eventEMA = static_cast<const Event<EventMessageAttributed>&>(event);
		auto& eventMsgAttr = eventEMA.GetMessage();
		
		if (eventMsgAttr.GetSubtype() == mSubtype)
		{
			//copy attribute arguments to this instance
			auto arguments = eventMsgAttr.AuxiliaryAttributes();
			for (auto& argument : arguments)
			{
				Append(argument->first) = argument->second;
			}

			//call actionListUpdate
			assert(eventMsgAttr.GetWorld() != nullptr); //ActionEvent should always give EMA a world
			ActionList::Update(const_cast<WorldState&>(eventMsgAttr.GetWorld()->GetWorldState()));
		}
		
	}

	const std::string& ReactionAttributed::GetSubtype() const
	{
		return mSubtype;
	}

	void ReactionAttributed::SetSubtype(std::string subtype)
	{
		mSubtype = std::move(subtype);
	}

	gsl::owner<ReactionAttributed*> ReactionAttributed::Clone() const
	{
		return new ReactionAttributed(*this);
	}

	Vector<Signature> ReactionAttributed::Signatures()
	{
		return Vector<Signature>
		{
			Signature("Name", DatumType::String, 1, offsetof(ReactionAttributed, mName)),
			Signature(ACTIONS_STRING, DatumType::Table, START_ACTIONS_CAPACITY, 0),
			Signature("Subtype", DatumType::String, 1, offsetof(ReactionAttributed, mSubtype))
		};
	}

}