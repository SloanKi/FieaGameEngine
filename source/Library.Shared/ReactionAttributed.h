#pragma once
#include "Reaction.h"

namespace Library
{
	class ReactionAttributed final : public Reaction
	{
		RTTI_DECLARATIONS(ReactionAttributed, Reaction)
	public:
		/// <summary>
		/// Default constructor: subscribes to Event<EventMessageAttributed> and passes RTTI to reaction
		/// </summary>
		ReactionAttributed();

		/// <summary>
		/// subscribes to Event<EventMessageAttributed>, passes RTTI to reaction, and sets subtype
		/// </summary>
		/// <param name="subtype">The subtype of event this reaction subscribes to</param>
		explicit ReactionAttributed(std::string subtype);

		/// <summary>
		/// subscribes to Event<EventMessageAttributed>, passes RTTI to reaction, sets subtype, name, and entity
		/// </summary>
		/// <param name="subtype">The subtype of event this reaction subscribes to</param>
		/// <param name="name">The name of this reaction</param>
		/// <param name="entity">The entity this reaction belongs to (nullptr if not specified)</param>
		ReactionAttributed(std::string subtype, std::string name, Entity* entity = nullptr);

		/// <summary>
		/// Copy Ctor: Copies rhs and subscribes to Event<EventMessageAttributed>
		/// </summary>
		/// <param name="rhs">the ReactionAttributed to copy</param>
		ReactionAttributed(const ReactionAttributed& rhs);

		/// <summary>
		/// Move ctor: Moves the rhs into this ReactionAttributed. Unsubscribes rhs and subscribes self to Event<EventMessageAttributed>
		/// </summary>
		/// <param name="rhs">the reaction attributed to move into this</param>
		ReactionAttributed(ReactionAttributed&& rhs) noexcept;

		/// <summary>
		/// Destructor: uses destructor from Reaction, but also unsubscribe from Event<EventMessageAttributed>
		/// </summary>
		~ReactionAttributed();


		ReactionAttributed& operator=(const ReactionAttributed & rhs) = default;
		ReactionAttributed& operator=(ReactionAttributed&& rhs) = default;

		/// <summary>
		/// Notify this subscriber that an event happened.
		/// If the event subtype matches this subtype, then copy the arguments from the event and 
		/// call the update of every action in the list.
		/// </summary>
		/// <param name="event">the event that occured</param>
		/// <remarks>Asserts that the event is of type Event<EventMessageAttributed>. </remarks>
		void Notify(const EventPublisher& event) override;

		/// <summary>
		/// Get the subtype
		/// </summary>
		/// <returns>The subtype (what type of event this subscribes to)</returns>
		const std::string& GetSubtype() const;

		/// <summary>
		/// Set the subtype
		/// </summary>
		/// <param name="subtype">The subtype (what type of event this subscribes to)</param>
		void SetSubtype(std::string subtype);

		/// <summary>
		/// Virtual copy constructor
		/// </summary>
		/// <returns>a pointer to a heap allocated copy of this</returns>
		virtual gsl::owner<ReactionAttributed*> Clone() const override;

		/// <summary>
		/// Gets the prescribed attributes for a reactionAttributed
		/// </summary>
		/// <returns>The prescribed attributes for a ReactionAttributed</returns>
		static Vector<Signature> Signatures();

	private:
		std::string mSubtype;
	};
	ConcreteFactory(ReactionAttributed, Scope)
}

