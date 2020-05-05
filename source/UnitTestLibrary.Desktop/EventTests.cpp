#include "pch.h"
#include "CppUnitTest.h"
#include "TestEventSubscribers.h"
#include "Bar.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;
using namespace UnitTests;
using namespace std;
using namespace std::string_literals;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	inline std::wstring ToString<Event<Foo>>(const Event<Foo>& t)
	{
		RETURN_WIDE_STRING(&t);
	}
}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(EventTests)
	{
	public:
		//check for memory leaks
		TEST_METHOD_INITIALIZE(Initialize)
		{

#if defined(DEBUG) || defined(_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&sStartMemState);
#endif
		}

		//check for memory leaks
		TEST_METHOD_CLEANUP(Cleanup)
		{
#if defined(DEBUG) || defined(_DEBUG)
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &sStartMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
#endif
		}

		TEST_METHOD(RTTITests)
		{
			Foo foo;
			Bar bar;

			Event<Foo> fooEvent(foo);
			Event<Bar> barEvent(bar);

			//is
			Assert::IsTrue(fooEvent.Is(Event<Foo>::TypeIdClass()));
			Assert::IsTrue(fooEvent.Is(EventPublisher::TypeIdClass()));
			Assert::IsFalse(fooEvent.Is(Event<Bar>::TypeIdClass()));

			Assert::IsTrue(barEvent.Is(Event<Bar>::TypeIdClass()));
			Assert::IsTrue(barEvent.Is(EventPublisher::TypeIdClass()));
			Assert::IsFalse(barEvent.Is(Event<Foo>::TypeIdClass()));

			//As
			Assert::IsNotNull(fooEvent.As<Event<Foo>>());
			Assert::IsNotNull(fooEvent.As<EventPublisher>());
			Assert::IsNull(fooEvent.As<Event<Bar>>());

			Assert::IsNotNull(barEvent.As<Event<Bar>>());
			Assert::IsNotNull(barEvent.As<EventPublisher>());
			Assert::IsNull(barEvent.As<Event<Foo>>());
		}

		TEST_METHOD(Equality)
		{
			Foo foo(1), foo2(2);
			Assert::AreNotEqual(foo, foo2);
			Assert::AreEqual(foo, foo);

			Event<Foo> event(foo);
			Event<Foo> event2(foo2);
			Event<Foo> eventCopy(foo);

			Assert::AreEqual(event, event);
			Assert::IsTrue(event != event2);
			Assert::AreEqual(event, eventCopy);
			Assert::IsTrue(event2 != eventCopy);
		}

		TEST_METHOD(SubscribeUnsubscribe)
		{
			FooSubscriber subscriber1, subscriber2, subscriber3;

			//subscribe
			Assert::AreEqual(0_z, Event<Foo>::GetSubscribers().Size());
			Event<Foo>::Subscribe(subscriber1);
			Assert::AreEqual(1_z, Event<Foo>::GetSubscribers().Size());

			//verify unsubscribing a non-existent thing doesn't break it
			Event<Foo>::Unsubscribe(subscriber2);
			Assert::AreEqual(1_z, Event<Foo>::GetSubscribers().Size());

			//normal unsubscribe
			Event<Foo>::Unsubscribe(subscriber1);
			Assert::AreEqual(0_z, Event<Foo>::GetSubscribers().Size());

			//unsubscribe all
			Event<Foo>::Subscribe(subscriber1);
			Event<Foo>::Subscribe(subscriber2);
			Event<Foo>::Subscribe(subscriber3);
			Assert::AreEqual(3_z, Event<Foo>::GetSubscribers().Size());
			Event<Foo>::UnsubscribeAll();
			Assert::AreEqual(0_z, Event<Foo>::GetSubscribers().Size());
		}

		TEST_METHOD(Deliver)
		{
			Foo foo(23);
			FooSubscriber fooSubscriber;
			Event<Foo> event(foo);
			Assert::AreEqual(foo, event.GetMessage());

			//not yet subscribed
			Assert::AreEqual(0, fooSubscriber.notifyCount);
			event.Deliver();
			Assert::AreEqual(0, fooSubscriber.notifyCount);

			//subscribe and deliver
			Event<Foo>::Subscribe(fooSubscriber);
			Assert::AreNotEqual(event.GetMessage().Data(), fooSubscriber.Data);
			event.Deliver();
			Assert::AreEqual(1, fooSubscriber.notifyCount);
			Assert::AreEqual(event.GetMessage().Data(), fooSubscriber.Data);

			//unsubscribe and deliver (shouldn't increment)
			Event<Foo>::Unsubscribe(fooSubscriber);
			//event.Deliver();
			Assert::AreEqual(1, fooSubscriber.notifyCount);

			//verify separate subscriber lists
			FooSubscriber barSubscriber;
			Event<Bar>::Subscribe(barSubscriber);
			Event<Foo>::Subscribe(fooSubscriber);
			event.Deliver();
			Assert::AreEqual(2, fooSubscriber.notifyCount);
			Assert::AreEqual(0, barSubscriber.notifyCount);
			
			Event<Bar>::UnsubscribeAll();
			Event<Foo>::UnsubscribeAll();
		}

		TEST_METHOD(GetMessage)
		{
			Foo foo(23);
			Event<Foo> event(foo);
			Assert::AreEqual(foo, event.GetMessage());
			Assert::AreNotSame(foo, event.GetMessage());
		}

		TEST_METHOD(CopySemantics)
		{
			Foo foo(23);
			Event<Foo> event(foo);

			//copy constructor
			Event<Foo> eventCopy = event;
			Assert::AreEqual(event.GetMessage(), eventCopy.GetMessage());
			Assert::AreNotSame(event.GetMessage(), eventCopy.GetMessage());

			//copy assignment
			Event<Foo> event2(Foo(1));
			Assert::AreNotEqual(event.GetMessage(), event2.GetMessage());
			event2 = event;
			Assert::AreEqual(event.GetMessage(), event2.GetMessage());
		}

		TEST_METHOD(MoveSemantics)
		{
			Foo foo(23);
			Event<Foo> event(foo);

			//move constructor
			Assert::AreEqual(foo, event.GetMessage());
			Event<Foo> eventMoved = std::move(event);
			Assert::AreEqual(foo, eventMoved.GetMessage());

			//move assignment
			Foo foo2(12);
			Event<Foo> event2(foo2);
			Assert::AreNotEqual(foo, event2.GetMessage());
			Assert::AreEqual(foo2, event2.GetMessage());
			event2 = event;
			Assert::AreEqual(foo, event2.GetMessage());
			Assert::AreNotEqual(foo2, event2.GetMessage());
		}

		TEST_METHOD(UnsubscribeDuringDeliver)
		{
			SingleUseSubscriber subscriber;
			Event<Foo>::Subscribe(subscriber);
			Event<Foo> event(Foo(2));
			
			Assert::AreEqual(1_z, Event<Foo>::GetSubscribers().Size());
			Assert::AreEqual(0, subscriber.notifyCount);
			event.Deliver();
			Assert::AreEqual(0_z, Event<Foo>::GetSubscribers().Size());
			Assert::AreEqual(1, subscriber.notifyCount);

			//make sure gone for realsies
			subscriber.notifyCount = 0;
			event.Deliver();
			Assert::AreEqual(0_z, Event<Foo>::GetSubscribers().Size());
			Assert::AreEqual(0, subscriber.notifyCount);

			Event<Foo>::UnsubscribeAll();
		}

		TEST_METHOD(SubscribeDuringDeliver)
		{
			AlternateEventsSubscriber subscriber;
			Event<Foo>::Subscribe(subscriber);
			Event<Foo> eventFoo(Foo(2));
			Event<Bar> eventBar(Bar(2));

			//pre test
			Assert::AreEqual(1_z, Event<Foo>::GetSubscribers().Size());
			Assert::AreEqual(0_z, Event<Bar>::GetSubscribers().Size());
			Assert::AreEqual(0, subscriber.fooCount);
			Assert::AreEqual(0, subscriber.barCount);

			//alternate
			eventFoo.Deliver();
			Assert::AreEqual(0_z, Event<Foo>::GetSubscribers().Size());
			Assert::AreEqual(1_z, Event<Bar>::GetSubscribers().Size());
			Assert::AreEqual(1, subscriber.fooCount);
			Assert::AreEqual(0, subscriber.barCount);

			//alternate
			eventBar.Deliver();
			Assert::AreEqual(1_z, Event<Foo>::GetSubscribers().Size());
			Assert::AreEqual(0_z, Event<Bar>::GetSubscribers().Size());
			Assert::AreEqual(1, subscriber.fooCount);
			Assert::AreEqual(1, subscriber.barCount);

			//alternate
			eventFoo.Deliver();
			Assert::AreEqual(0_z, Event<Foo>::GetSubscribers().Size());
			Assert::AreEqual(1_z, Event<Bar>::GetSubscribers().Size());
			Assert::AreEqual(2, subscriber.fooCount);
			Assert::AreEqual(1, subscriber.barCount);

			Event<Foo>::UnsubscribeAll();
			Event<Bar>::UnsubscribeAll();
		}

		TEST_METHOD(AsyncTest)
		{
			Vector<AsyncFooSubscriberNumbered> subscribeList(1000);
			for (int i = 0; i < 1000; i++)
			{
				subscribeList.PushBack(AsyncFooSubscriberNumbered(i));
				Event<Foo>::Subscribe(subscribeList[i]);
			}

			Assert::AreEqual(1000_z, Event<Foo>::GetSubscribers().Size());
			Event<Foo> event(Foo(0));
			event.Deliver();
			Assert::AreEqual(1000_z, Event<Foo>::GetSubscribers().Size());

			for (auto& subscriber : subscribeList)
			{
				Assert::IsTrue(subscriber.bWasNotified);
			}

			Event<Foo>::UnsubscribeAll();
		}

		TEST_METHOD(AsyncExceptionTest)
		{
			Vector<ExceptionSubscriber> subscribeList(100);
			for (int i = 0; i < 100; i++)
			{
				subscribeList.PushBack(ExceptionSubscriber());
				Event<Foo>::Subscribe(subscribeList[i]);
			}

			Assert::AreEqual(100_z, Event<Foo>::GetSubscribers().Size());
			Event<Foo> event(Foo(0));
			//event.Deliver();
			Assert::ExpectException<std::exception>([&event] { event.Deliver(); });
			Assert::AreEqual(100_z, Event<Foo>::GetSubscribers().Size());

			for (int i = 0; i < 100; i++)
			{
				Assert::IsTrue(subscribeList[i].bWasNotified);
			}

			Event<Foo>::UnsubscribeAll();
		}

	private:
		static _CrtMemState sStartMemState;	//for memory leak detection
	};
	_CrtMemState EventTests::sStartMemState;
}