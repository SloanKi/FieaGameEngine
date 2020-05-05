#pragma once
#include "RTTI.h"
#include "vector.h"


namespace Library
{
	class EventSubscriber; // forward declaration

	class EventPublisher : public RTTI
	{
		RTTI_DECLARATIONS(EventPublisher, RTTI)
	public:
		using SubscriberListType = Vector<EventSubscriber*>;
		EventPublisher(const EventPublisher& rhs) = default;
		EventPublisher(EventPublisher&& rhs) noexcept = default;
		EventPublisher& operator=(const EventPublisher& rhs) = default;
		EventPublisher& operator=(EventPublisher&& rhs) noexcept = default;

		/// <summary>
		/// Iterates through all subscribers and notifies them of this event happening.
		/// </summary>
		void Deliver() const;
	
	protected:
		EventPublisher(SubscriberListType& subscriberList, std::mutex& subscriberMutex);
		

	private:
		SubscriberListType* mSubscriberListPtr; 
		std::mutex* mSubscriberMutexPtr;
	};
}

