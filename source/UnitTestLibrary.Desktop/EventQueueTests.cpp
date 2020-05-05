#include "pch.h"
#include "CppUnitTest.h"
#include "TestEventSubscribers.h"
#include "EventQueue.h"
#include "Event.h"
#include "GameTime.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;
using namespace UnitTests;
using namespace std;
using namespace std::string_literals;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	inline std::wstring ToString<GameClock>(const Library::GameClock& t)
	{
		RETURN_WIDE_STRING(&t);
	}

	template<>
	inline std::wstring ToString<Event<Foo>>(Event<Foo>* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<Event<Foo>>(const Event<Foo>& t)
	{
		RETURN_WIDE_STRING(&t);
	}

	template<>
	inline std::wstring ToString<std::chrono::high_resolution_clock::time_point>(const std::chrono::high_resolution_clock::time_point& t)
	{
		RETURN_WIDE_STRING(&t);
	}
}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(EventQueueTests)
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

		TEST_METHOD(Constructor)
		{
			GameClock clock;
			EventQueue eventQueue = EventQueue(clock); //only wrote it this way so intellisense would stop being dumb

			Assert::AreSame(clock, eventQueue.Clock());
			Assert::AreEqual(0_z, eventQueue.Size());
			Assert::IsTrue(eventQueue.isEmpty());
		}

		TEST_METHOD(SizeIsEmptyClear)
		{
			Foo foo;
			GameClock clock;
			std::shared_ptr<Event<Foo>> event = std::make_shared<Event<Foo>>(foo);
			std::shared_ptr<Event<Foo>> event2 = std::make_shared<Event<Foo>>(foo);
			EventQueue eventQueue = EventQueue(clock); //only wrote it this way so intellisense would stop being dumb
			Assert::AreEqual(0_z, eventQueue.Size());
			Assert::IsTrue(eventQueue.isEmpty());

			eventQueue.Enqueue(event, std::chrono::milliseconds(5));
			Assert::AreEqual(1_z, eventQueue.Size());
			eventQueue.Enqueue(event2, std::chrono::milliseconds(10));
			//eventQueue.Enqueue(*event, std::chrono::milliseconds(10)); PAUL WHY LEAK?
			Assert::AreEqual(2_z, eventQueue.Size());
			Assert::IsFalse(eventQueue.isEmpty());

			eventQueue.Clear();
			Assert::AreEqual(0_z, eventQueue.Size());
		}

		TEST_METHOD(Send)
		{
			Foo foo(23);
			FooSubscriber fooSubscriber1, fooSubscriber2, fooSubscriber3;
			Event<Foo> event(foo);
			GameClock clock;
			EventQueue eventQueue = EventQueue(clock); //only wrote it this way so intellisense would stop being dumb
			Event<Foo>::Subscribe(fooSubscriber1);
			Event<Foo>::Subscribe(fooSubscriber2);
			Event<Foo>::Subscribe(fooSubscriber3);
			
			Assert::AreEqual(0, fooSubscriber1.notifyCount);
			Assert::AreEqual(0, fooSubscriber2.notifyCount);
			Assert::AreEqual(0, fooSubscriber3.notifyCount);
			
			eventQueue.Send(event);

			Assert::AreEqual(1, fooSubscriber1.notifyCount);
			Assert::AreEqual(1, fooSubscriber2.notifyCount);
			Assert::AreEqual(1, fooSubscriber3.notifyCount);

			Event<Foo>::UnsubscribeAll();
		}

		TEST_METHOD(Enqueue)
		{
			Foo foo(23);
			FooSubscriber fooSubscriber;
			std::shared_ptr<Event<Foo>> event1 = std::make_shared<Event<Foo>>(foo);
			GameClock clock;
			GameTime gameTime;
			clock.UpdateGameTime(gameTime);
			EventQueue eventQueue = EventQueue(clock); //only wrote it this way so intellisense would stop being dumb
			Event<Foo>::Subscribe(fooSubscriber);

			//test 1: no delay (send on next update)
			eventQueue.Enqueue(event1);
			Assert::AreEqual(1_z, eventQueue.Size());
			Assert::AreEqual(0, fooSubscriber.notifyCount);
			eventQueue.Update(gameTime);
			Assert::AreEqual(0_z, eventQueue.Size());
			Assert::AreEqual(1, fooSubscriber.notifyCount);

			//test 2: with delay (should queue)
			eventQueue.Enqueue(event1, std::chrono::milliseconds(5));
			Assert::AreEqual(1_z, eventQueue.Size());
			Assert::AreEqual(*event1, *eventQueue.GetEvents()[0].first->As<Event<Foo>>());
			Assert::AreSame(*event1, *eventQueue.GetEvents()[0].first->As<Event<Foo>>()); //sanity
			Assert::AreEqual(event1->GetMessage(), eventQueue.GetEvents()[0].first->As<Event<Foo>>()->GetMessage()); //sanity, is what above does
			Assert::AreEqual(clock.CurrentTime() + std::chrono::milliseconds(5), eventQueue.GetEvents()[0].second);
			Assert::AreEqual(1, fooSubscriber.notifyCount); //should not increment from previous test


			//test 3: duplicate (throw exception)
			Assert::ExpectException<std::runtime_error>([&eventQueue, &event1] { eventQueue.Enqueue(event1, std::chrono::milliseconds(5)); });

			Event<Foo>::UnsubscribeAll();
		}

		TEST_METHOD(Update)
		{
			Foo foo(23);
			FooSubscriber fooSubscriber;
			GameClock clock;
			GameTime gameTime;
			
			Event<Foo>::Subscribe(fooSubscriber);
			clock.UpdateGameTime(gameTime);
			std::shared_ptr<Event<Foo>> event1 = std::make_shared<Event<Foo>>(foo);
			EventQueue eventQueue = EventQueue(clock); //only wrote it this way so intellisense would stop being dumb
			eventQueue.Enqueue(event1, std::chrono::milliseconds(5));
			

			//pre testing
			Assert::AreEqual(1_z, eventQueue.Size());
			Assert::AreEqual(0, fooSubscriber.notifyCount);
			Assert::AreNotEqual(gameTime.CurrentTime(), eventQueue.GetEvents()[0].second);
			Assert::IsTrue(gameTime.CurrentTime() < eventQueue.GetEvents()[0].second);

			//test 1: not time yet
			eventQueue.Update(gameTime);
			Assert::AreEqual(0, fooSubscriber.notifyCount);
			Assert::AreEqual(1_z, eventQueue.Size());


			gameTime.SetCurrentTime(eventQueue.GetEvents()[0].second);
			Assert::AreEqual(gameTime.CurrentTime(), eventQueue.GetEvents()[0].second);
			eventQueue.Update(gameTime);
			Assert::AreEqual(1, fooSubscriber.notifyCount);
			Assert::AreEqual(0_z, eventQueue.Size());


			//test 2: more events
			fooSubscriber.notifyCount = 0;
			std::shared_ptr<Event<Foo>> event2 = std::make_shared<Event<Foo>>(foo);
			std::shared_ptr<Event<Foo>> event3 = std::make_shared<Event<Foo>>(foo);
			std::shared_ptr<Event<Foo>> event4 = std::make_shared<Event<Foo>>(foo);
			std::shared_ptr<Event<Foo>> event5 = std::make_shared<Event<Foo>>(foo);
			eventQueue.Enqueue(event2, std::chrono::milliseconds(10));
			eventQueue.Enqueue(event3, std::chrono::milliseconds(15));
			eventQueue.Enqueue(event4, std::chrono::milliseconds(15));
			eventQueue.Enqueue(event5, std::chrono::milliseconds(20));
			Assert::AreEqual(0, fooSubscriber.notifyCount);
			Assert::AreEqual(4_z, eventQueue.Size());


			eventQueue.Update(gameTime);
			Assert::AreEqual(0, fooSubscriber.notifyCount);
			Assert::AreEqual(4_z, eventQueue.Size());

			//event 2 should fire
			gameTime.SetCurrentTime(eventQueue.GetEvents()[0].second);
			eventQueue.Update(gameTime);
			Assert::AreEqual(1, fooSubscriber.notifyCount);
			Assert::AreEqual(3_z, eventQueue.Size());

			//event 3 and 4 should fire
			gameTime.SetCurrentTime(eventQueue.GetEvents()[0].second);
			eventQueue.Update(gameTime);
			Assert::AreEqual(3, fooSubscriber.notifyCount);
			Assert::AreEqual(1_z, eventQueue.Size());

			//event 5 should fire
			gameTime.SetCurrentTime(eventQueue.GetEvents()[0].second);
			eventQueue.Update(gameTime);
			Assert::AreEqual(4, fooSubscriber.notifyCount);
			Assert::AreEqual(0_z, eventQueue.Size());

			Event<Foo>::UnsubscribeAll();
		}

		TEST_METHOD(EnqueueDuringUpdate)
		{
			Foo foo(23);
			EventEnqueueSubscriber subscriber;
			GameClock clock;
			GameTime gameTime;

			Event<Foo>::Subscribe(subscriber);
			clock.UpdateGameTime(gameTime);
			EventQueue eventQueue = EventQueue(clock);
			Event<Foo> event1 = Event<Foo>(foo);

			//test 1: make sure subscriber works as expected
			subscriber.queuePtr = &eventQueue;
			Assert::AreEqual(0_z, eventQueue.Size());
			event1.Deliver(); //should make subscriber add an event to queue
			Assert::AreEqual(1, subscriber.notifyCount);
			Assert::AreEqual(1_z, eventQueue.Size());

			//test 2: add during update
			subscriber.notifyCount = 0;
			Assert::AreEqual(1_z, eventQueue.Size());
			gameTime.SetCurrentTime(eventQueue.GetEvents()[0].second);
			eventQueue.Update(gameTime);
			Assert::AreEqual(1_z, eventQueue.Size()); //1 because new one queued and old one destroyed
			clock.UpdateGameTime(gameTime);
			Assert::IsTrue(gameTime.CurrentTime() < eventQueue.GetEvents()[0].second);

			Event<Foo>::UnsubscribeAll();
		}

		TEST_METHOD(AsyncTest)
		{
			Foo foo(23);
			AsyncFooSubscriber fooSubscriber;
			GameClock clock;
			GameTime gameTime;

			Event<Foo>::Subscribe(fooSubscriber);
			clock.UpdateGameTime(gameTime);
			EventQueue eventQueue = EventQueue(clock); //only wrote it this way so intellisense would stop being dumb

			//enqueue 1000 events to send on next update
			for (int i = 0; i < 500; i++)
			{
				eventQueue.Enqueue(std::make_shared<Event<Foo>>(Foo(i)));
			}

			Assert::AreEqual(0, fooSubscriber.notifyCount);
			Assert::AreEqual(500_z, eventQueue.Size());
			eventQueue.Update(gameTime);
			Assert::AreEqual(500, fooSubscriber.notifyCount);
			Assert::AreEqual(0_z, eventQueue.Size());
			

			Event<Foo>::UnsubscribeAll();
		}

		TEST_METHOD(AsyncExceptionTest)
		{
			Foo foo(23);
			ExceptionSubscriber subscriber;
			GameClock clock;
			GameTime gameTime;

			Event<Foo>::Subscribe(subscriber);
			clock.UpdateGameTime(gameTime);
			EventQueue eventQueue = EventQueue(clock); //only wrote it this way so intellisense would stop being dumb

			//enqueue 1000 events to send on next update
			for (int i = 0; i < 100; i++)
			{
				eventQueue.Enqueue(std::make_shared<Event<Foo>>(Foo(i)));
			}

			Assert::AreEqual(100_z, eventQueue.Size());
			Assert::ExpectException<std::exception>([&eventQueue, &gameTime]() {eventQueue.Update(gameTime); });
			Assert::AreEqual(0_z, eventQueue.Size());


			Event<Foo>::UnsubscribeAll();
		}

	private:
		static _CrtMemState sStartMemState;	//for memory leak detection
	};
	_CrtMemState EventQueueTests::sStartMemState;
}