#include "pch.h"
#include "EventMessageAttributed.h"

namespace Library
{
	RTTI_DEFINITIONS(EventMessageAttributed)

	EventMessageAttributed::EventMessageAttributed() :
		Attributed(TypeIdClass())
	{}

	EventMessageAttributed::EventMessageAttributed(std::string subtype, World* world) :
		Attributed(TypeIdClass()), mSubtype(std::move(subtype)), mWorldPtr(world)
	{}

	World* EventMessageAttributed::GetWorld()
	{
		return mWorldPtr;
	}

	const World* EventMessageAttributed::GetWorld() const
	{
		return mWorldPtr;
	}

	void EventMessageAttributed::SetWorld(World& world)
	{
		mWorldPtr = &world;
	}

	const std::string& EventMessageAttributed::GetSubtype() const
	{
		return mSubtype;
	}

	void EventMessageAttributed::SetSubtype(std::string subtype)
	{
		mSubtype = std::move(subtype);
	}

	gsl::owner<EventMessageAttributed*> EventMessageAttributed::Clone() const
	{
		return new EventMessageAttributed(*this);
	}

	Vector<Signature> EventMessageAttributed::Signatures()
	{
		return Vector<Signature>
		{
			Signature("Subtype", DatumType::String, 1, offsetof(EventMessageAttributed, mSubtype))
		};
	}
}