#include "pch.h"
#include "TestEventSubscribers.h"
#include "Bar.h"

using namespace Library;

namespace UnitTests
{
	void FooSubscriber::Notify(const EventPublisher& event)
	{
		assert(event.Is(Event<Foo>::TypeIdClass()));
		const Event<Foo>& eventPtr = static_cast<const Event<Foo>&> (event);
		const Foo& foo = eventPtr.GetMessage();
		Data = foo.Data();
		notifyCount++;
	}

	//unsubscribe self after being notified
	void SingleUseSubscriber::Notify([[maybe_unused]] const EventPublisher& event)
	{
		Event<Foo>::Unsubscribe(*this);
		notifyCount++;
	}

	//alternate subscriptions between Event<Foo> and Event<Bar>
	void AlternateEventsSubscriber::Notify(const EventPublisher& event)
	{
		if (event.Is(Event<Foo>::TypeIdClass()))
		{
			fooCount++;
			Event<Foo>::Unsubscribe(*this);
			Event<Bar>::Subscribe(*this);
		}
		else if (event.Is(Event<Bar>::TypeIdClass()))
		{
			barCount++;
			Event<Bar>::Unsubscribe(*this);
			Event<Foo>::Subscribe(*this);
		}
	}

	//adds a new event to the queue
	void EventEnqueueSubscriber::Notify([[maybe_unused]]const EventPublisher& event)
	{
		if (queuePtr != nullptr)
		{
			std::shared_ptr<Event<Foo>> newEvent = std::make_shared<Event<Foo>>(Foo(2));
			queuePtr->Enqueue(newEvent, std::chrono::milliseconds(5));
		}
		notifyCount++;
	}
	
	
	//writes to unit test log the value of the data
	void AsyncFooSubscriber::Notify(const EventPublisher& event)
	{
		assert(event.Is(Event<Foo>::TypeIdClass()));
		const Event<Foo>& eventPtr = static_cast<const Event<Foo>&> (event);
		std::string var = "Foo " + std::to_string(eventPtr.GetMessage().Data()) + "\n";
		Microsoft::VisualStudio::CppUnitTestFramework::Logger::WriteMessage(var.c_str());
		notifyCount++;
	}

	void AsyncFooSubscriberNumbered::Notify([[maybe_unused]] const EventPublisher& event)
	{
		assert(event.Is(Event<Foo>::TypeIdClass()));
		bWasNotified = true;
		std::string var = "Subscriber " + std::to_string(id) + "\n";
		Microsoft::VisualStudio::CppUnitTestFramework::Logger::WriteMessage(var.c_str());
	}


	void ExceptionSubscriber::Notify([[maybe_unused]] const EventPublisher& event)
	{
		bWasNotified = true;
		throw std::exception("this is an error");
	}

}