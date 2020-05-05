#pragma once
#include "ActionList.h"
#include "EventSubscriber.h"

namespace Library
{
	class Reaction : public ActionList, public EventSubscriber
	{
		RTTI_DECLARATIONS(Reaction, ActionList)
	public:
		Reaction(const Reaction& rhs) = default;
		Reaction(Reaction&& rhs) noexcept = default;
		~Reaction() = default;
		Reaction& operator=(const Reaction& rhs) = default;
		Reaction& operator=(Reaction&& rhs) noexcept = default;

		/// <summary>
		/// Derived classes will call ActionList::Update if the event is what they are looking for.
		/// </summary>
		/// <param name="event">The event that occured</param>
		virtual void Notify(const EventPublisher& event) = 0;

		/// <summary>
		/// Does nothing in base implementation as we only want actions called as a reaction, not every update
		/// </summary>
		/// <param name="state">the world state</param>
		virtual void Update([[maybe_unused]]WorldState& state) {}; //does nothing in base implementation

	protected:
		/// <summary>
		/// Virtual Constructor to pass RTTI type ID down to get correct attributes
		/// </summary>
		/// <param name="typeID">the type of this class</param>
		explicit Reaction(RTTI::IdType typeID);

		/// <summary>
		/// Virtual constructor to pass RTTI type id down to get correct attributes. 
		/// Also sets name, and if provided, adopts into entity
		/// </summary>
		/// <param name="typeID">the type id of this class</param>
		/// <param name="name">the name given to this reaction</param>
		/// <param name="entity">the entity to adopt this reaction into</param>
		Reaction(RTTI::IdType typeID, std::string name, Entity* entity = nullptr);
	};
}

