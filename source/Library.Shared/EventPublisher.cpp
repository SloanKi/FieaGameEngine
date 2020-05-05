#include "pch.h"
#include "EventPublisher.h"
#include "EventSubscriber.h"

namespace Library
{
	RTTI_DEFINITIONS(EventPublisher)

	EventPublisher::EventPublisher(SubscriberListType& subscriberList, std::mutex& subscriberMutex)
		: mSubscriberListPtr(&subscriberList), mSubscriberMutexPtr(&subscriberMutex)
	{
	}

	void EventPublisher::Deliver() const
	{
		Vector<std::future<void>> futures;

		//critical section
		{
			std::lock_guard<std::mutex> lockSubscribers(*mSubscriberMutexPtr);

			for (EventSubscriber* subscriber : *mSubscriberListPtr)
			{
				assert(subscriber != nullptr);
				futures.PushBack(std::async(std::launch::async, [this, subscriber]() {subscriber->Notify(*this); }));
			}
		}
		
		Vector<std::exception> exceptionList;
		
		//blocking: will not continue until all subscribers finish their notify
		for (auto& future : futures)
		{
			try
			{
				future.get(); //if use wait, don't get exception
			}
			catch (std::exception e)
			{
				exceptionList.PushBack(e);
			}
		}
		
		//create aggregate exception
		if (!exceptionList.IsEmpty())
		{
			std::string exceptionMsg;
			for (auto& except : exceptionList)
			{
				exceptionMsg += except.what();
			}
			throw std::exception(exceptionMsg.c_str());
		}
	}

}
