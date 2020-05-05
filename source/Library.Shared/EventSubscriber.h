#pragma once

namespace Library
{
	class EventPublisher; //forward declaration

	/// <summary>
	/// Interface for an EventSubscriber
	/// </summary>
	class EventSubscriber
	{
	public:
		/// <summary>
		/// Notifies this subscriber that an event occurred (called by the event publisher)
		/// </summary>
		/// <param name="event">The event that occured</param>
		virtual void Notify(const EventPublisher& event) = 0;

		/// <summary>
		/// Default destructor (virtual for children)
		/// </summary>
		virtual ~EventSubscriber() = default;
	};
}