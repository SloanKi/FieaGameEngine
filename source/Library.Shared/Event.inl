#pragma once
#include "Event.h"

namespace Library
{
	template<typename T>
	EventPublisher::SubscriberListType Event<T>::mSubscriberList{ 5 };

	template<typename T>
	RTTI_DEFINITIONS(Event<T>)

	template<typename T>
	inline Event<T>::Event(const T& message) :
		EventPublisher(mSubscriberList, mSubscriberMutex), mMessage(message)
	{}

	template<typename T>
	inline Event<T>::Event(T&& message) :
		EventPublisher(mSubscriberList, mSubscriberMutex), mMessage(std::move(message))
	{}

	template<typename T>
	inline void Event<T>::Subscribe(EventSubscriber& subscriber)
	{
		std::lock_guard<std::mutex> lockSubscribers(mSubscriberMutex);
		mSubscriberList.PushBack(&subscriber);
	}

	template<typename T>
	inline void Event<T>::Unsubscribe(EventSubscriber& subscriber)
	{
		std::lock_guard<std::mutex> lockSubscribers(mSubscriberMutex);
		mSubscriberList.Remove(mSubscriberList.Find(&subscriber));
	}

	template<typename T>
	inline void Event<T>::UnsubscribeAll()
	{
		std::lock_guard<std::mutex> lockSubscribers(mSubscriberMutex);
		mSubscriberList.Clear();
		mSubscriberList.ReCapacity(5);
	}

	template<typename T>
	inline const typename EventPublisher::SubscriberListType& Event<T>::GetSubscribers()
	{
		return mSubscriberList;
	}

	template<typename T>
	inline const T& Event<T>::GetMessage() const
	{
		return mMessage;
	}

	template<typename T>
	inline bool Event<T>::operator==(const Event<T>& rhs) const noexcept
	{
		return mMessage == rhs.mMessage;
	}

	template<typename T>
	inline bool Event<T>::operator!=(const Event& rhs) const noexcept
	{
		return !operator==(rhs);
	}


}