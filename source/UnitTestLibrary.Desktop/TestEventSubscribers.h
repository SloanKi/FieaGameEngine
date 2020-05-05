#pragma once
#include "EventSubscriber.h"
#include "Event.h"
#include "Foo.h"
#include "EventQueue.h"

using namespace Library;

namespace UnitTests
{
	struct FooSubscriber final : public EventSubscriber
	{
		void Notify(const EventPublisher& event) override;

		int notifyCount = 0;
		int Data = 0;
	};

	//unsubscribe self after being notified
	struct SingleUseSubscriber final : public EventSubscriber
	{
		void Notify(const EventPublisher& event) override;
		int notifyCount = 0;
	};

	//alternate subscriptions between Event<Foo> and Event<Bar>
	struct AlternateEventsSubscriber final : public EventSubscriber
	{
		void Notify(const EventPublisher& event) override;
		int fooCount = 0;
		int barCount = 0;
	};

	//adds a new event to the queue in notify
	struct EventEnqueueSubscriber final : public EventSubscriber
	{
		void Notify(const EventPublisher& event) override;
		int notifyCount = 0;
		EventQueue* queuePtr = nullptr;
	};

	struct AsyncFooSubscriber final : public EventSubscriber
	{
		void Notify(const EventPublisher& event) override;
		//EventQueue* queuePtr = nullptr;
		int notifyCount = 0;
	};

	struct AsyncFooSubscriberNumbered final : public EventSubscriber
	{
		AsyncFooSubscriberNumbered(int num) : id(num) {};
		void Notify(const EventPublisher& event) override;
		int id;
		bool bWasNotified = false;
	};

	struct ExceptionSubscriber final : public EventSubscriber
	{
		void Notify(const EventPublisher& event) override;
		bool bWasNotified = false;
	};
}


