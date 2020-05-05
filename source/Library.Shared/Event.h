#pragma once
#include "EventPublisher.h"

namespace Library
{
	template <typename T>
	class Event final : public EventPublisher
	{
		RTTI_DECLARATIONS(Event, EventPublisher)
	public:
		/// <summary>
		/// Creates an event that contains a copy of message.
		/// </summary>
		/// <param name="message">the "payload" of the event</param>
		explicit Event(const T& message);

		/// <summary>
		/// Creates an event that contains a copy of message.
		/// </summary>
		/// <param name="message">the "payload" of the event</param>
		explicit Event(T&& message);
		Event(const Event& rhs) = default;
		Event(Event&& rhs) noexcept = default;
		Event& operator=(const Event& rhs) = default;
		Event& operator=(Event&& rhs) noexcept = default;

		/// <summary>
		/// Add a subscriber to the list of subscribers for this type of event
		/// </summary>
		/// <param name="subscriber">the event subscriber to add to the list</param>
		static void Subscribe(EventSubscriber& subscriber);

		/// <summary>
		/// Add a subscriber to the list of subscribers for this type of event
		/// </summary>
		/// <param name="subscriber">the event subscriber to remove from the list</param>
		static void Unsubscribe(EventSubscriber& subscriber);

		/// <summary>
		/// Remove all subscribers for this type of event
		/// </summary>
		static void UnsubscribeAll();

		/// <summary>
		/// Get the list of subscribers
		/// </summary>
		/// <returns>a const reference to the list of subscribers</returns>
		static const SubscriberListType& GetSubscribers();

		/// <summary>
		/// Get the message this event holds
		/// </summary>
		/// <returns>a constant reference to the message this event holds</returns>
		const T& GetMessage() const;

		/// <summary>
		/// Equality: Compares the messages of these events
		/// </summary>
		/// <param name="rhs">the event to compare to</param>
		/// <returns>true of equal, false if not</returns>
		bool operator==(const Event& rhs) const noexcept;

		/// <summary>
		/// Inequality: Compares the messages of these events
		/// </summary>
		/// <param name="rhs">the event to compare to</param>
		/// <returns>true if not equal, false if equal</returns>
		bool operator!=(const Event& rhs) const noexcept;

	private:
		static SubscriberListType mSubscriberList;
		static inline std::mutex mSubscriberMutex;
		T mMessage;
	};
}

#include "Event.inl"