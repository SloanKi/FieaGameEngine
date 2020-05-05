#pragma once
#include "Action.h"
#include "Factory.h"

namespace Library
{
	class ActionEvent final : public Action
	{
		RTTI_DECLARATIONS(ActionEvent, Action)
	public:
		/// <summary>
		/// Default ctor: sets name to "ActionEvent" and passes RTTI info to action
		/// </summary>
		ActionEvent();
		ActionEvent(const ActionEvent& rhs) = default;
		ActionEvent(ActionEvent&& rhs) noexcept = default;
		~ActionEvent() = default;
		ActionEvent& operator=(const ActionEvent& rhs) = default;
		ActionEvent& operator=(ActionEvent&& rhs) noexcept = default;

		/// <summary>
		/// Get the subtype of event to create
		/// </summary>
		/// <returns>The subtype of event to create</returns>
		const std::string& GetSubtype() const;

		/// <summary>
		/// Set the subtype of event to create
		/// </summary>
		/// <param name="subtype">the subtype of event to create</param>
		void SetSubtype(std::string subtype);

		/// <summary>
		/// Get the delay in milliseconds
		/// </summary>
		/// <returns>the delay in milliseconds</returns>
		int GetDelay() const;

		/// <summary>
		/// Set the delay
		/// </summary>
		/// <param name="delay">the number of milliseconds to delay the event</param>
		void SetDelay(int delay);

		/// <summary>
		/// Creates an EventMessageAttributed and copies into it the auxiliary attributes from this.
		/// Wraps the EMA in an Event, and queues it with the given delay time.
		/// If run once is true, also adds self to destroy queue
		/// </summary>
		/// <param name="state">the world state</param>
		/// <exception cref="std::runtime_error">Throws exception if subtype empty or delay < 0</exception>
		void Update(WorldState& state) override;

		/// <summary>
		/// Virtual copy constructor
		/// </summary>
		/// <returns>Create a heap allocated copy of this</returns>
		gsl::owner<ActionEvent*> Clone() const override;

		/// <summary>
		/// Get the signatures to populate this
		/// </summary>
		/// <returns>the signatures to populate this class</returns>
		static Vector<Signature> Signatures();
	private:
		std::string mSubtype;
		int mDelay;
	};
	ConcreteFactory(ActionEvent, Scope)
}

