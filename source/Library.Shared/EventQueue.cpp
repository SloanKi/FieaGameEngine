#include "pch.h"
#include "EventQueue.h"
#include "EventPublisher.h"
#include "GameTime.h"

namespace Library
{
	EventQueue::EventQueue(const GameClock& clock) :
		mClock(&clock)
	{}

	void EventQueue::Send(const EventPublisher& publisher) const
	{
		publisher.Deliver();
	}

	const GameClock& EventQueue::Clock() const
	{
		return *mClock;
	}

	
	size_t EventQueue::Size() const
	{
		return mEventList.Size();
	}

	bool EventQueue::isEmpty() const
	{
		return mEventList.IsEmpty();
	}

	void EventQueue::Clear()
	{
		mEventList.Clear();
	}

	void EventQueue::Enqueue(std::shared_ptr<EventPublisher> publisher, std::chrono::milliseconds delay)
	{
		VerifyNotDuplicate(*publisher);

		//get delay
		std::chrono::high_resolution_clock::time_point expireTime = mClock->CurrentTime() + delay;

		//store in vector
		if (bIsUpdating)
		{
			mPendingAddList.PushBack({ std::move(publisher), expireTime });
		}
		else //if not updating, safe to push straight to event list
		{
			mEventList.PushBack({ std::move(publisher), expireTime });
		}
	}

	void EventQueue::Update(const GameTime& gameTime)
	{
		bIsUpdating = true;

		Vector<std::future<void>> futures;
		Vector<std::exception> exceptionList;

		//Partition: will put all expired events at the END of the vector.
		auto currentTime = gameTime.CurrentTime();
		auto it = std::partition(mEventList.begin(), mEventList.end(), [&currentTime](const EventQueueInfo& info) { return info.second > currentTime; });
		size_t itIndex = it.Index(); //hold index to use to remove later

		//iterate through expiring
		for (it;  it != mEventList.end(); it++)
		{
			assert(it->second <= currentTime); //sanity check
			futures.PushBack(std::async(std::launch::async, [it]() {it->first->Deliver(); }));
		}


		//blocking: will not continue until all events have finished their deliver methods
		for (auto& future : futures)
		{
			try
			{
				future.get();
			}
			catch (std::exception e)
			{
				exceptionList.PushBack(e);
			}
		}
		
		mEventList.Resize(itIndex); //delete expiring
		bIsUpdating = false;

		//Add at end: guarantees that any events added during update are added into the list for the next update.
		if (!mPendingAddList.IsEmpty())
		{
			AddPendingEvents();
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

	const Vector<EventQueue::EventQueueInfo>& EventQueue::GetEvents() const
	{
		return mEventList;
	}

	void EventQueue::AddPendingEvents()
	{
		for (auto& eventInfo : mPendingAddList)
		{
			mEventList.PushBack(std::move(eventInfo));
		}
		mPendingAddList.Clear();
	}


	void EventQueue::VerifyNotDuplicate(const EventPublisher& publisher)
	{
		//check pending list
		for (const auto& eventInfo : mPendingAddList)
		{
			//TODO: Ask Paul if there's a less disgusting way get raw ptr from shared ptr
			if (&(*eventInfo.first) == &publisher)
			{
				throw std::runtime_error("Can't enqueue the same event twice");
			}
		}

		//check event list
		for (const auto& eventInfo : mEventList)
		{
			//TODO: Ask Paul if there's a less disgusting way get raw ptr from shared ptr
			if (&(*eventInfo.first) == &publisher)
			{
				throw std::runtime_error("Can't enqueue the same event twice");
			}
		}
	}
}

