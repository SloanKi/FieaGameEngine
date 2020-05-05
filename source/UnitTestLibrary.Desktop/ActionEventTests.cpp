#include "pch.h"
#include "CppUnitTest.h"
#include "TypeRegistry.h"
#include "ActionEvent.h"
#include "EventMessageAttributed.h"
#include "World.h"
#include "TestAction.h"
#include "EventPublisher.h"
#include "Event.h"
#include "ReactionAttributed.h"
//#include "Event.h"
//#include "Entity.h"
//#include "ActionIncrement.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;
using namespace UnitTests;
using namespace std;
using namespace std::string_literals;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	inline std::wstring ToString<std::chrono::high_resolution_clock::time_point>(const std::chrono::high_resolution_clock::time_point& t)
	{
		RETURN_WIDE_STRING(&t);
	}

	template<>
	inline std::wstring ToString<World>(World* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<World>(const World& t)
	{
		RETURN_WIDE_STRING(&t);
	}
	/*template<>
	inline std::wstring ToString<World>(World* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<Library::DatumType>(const Library::DatumType& t)
	{
		RETURN_WIDE_STRING(&t);
	}

	template<>
	inline std::wstring ToString<Entity>(const Entity& t)
	{
		RETURN_WIDE_STRING(&t);
	}

	template<>
	inline std::wstring ToString<Entity>(Entity* t)
	{
		RETURN_WIDE_STRING(t);
	}*/
}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(ActionEventTests)
	{
	public:
		//check for memory leaks
		TEST_METHOD_INITIALIZE(Initialize)
		{

#if defined(DEBUG) || defined(_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&sStartMemState);
#endif
			TypeRegistry::Create();
			TypeRegistry::RegisterType(EventMessageAttributed::TypeIdClass(), EventMessageAttributed::Signatures());
			TypeRegistry::RegisterType(ActionEvent::TypeIdClass(), ActionEvent::Signatures());
			TypeRegistry::RegisterType(World::TypeIdClass(), World::Signatures());
			TypeRegistry::RegisterType(TestAction::TypeIdClass(), TestAction::Signatures());
			TypeRegistry::RegisterType(ReactionAttributed::TypeIdClass(), ReactionAttributed::Signatures());
		}

		//check for memory leaks
		TEST_METHOD_CLEANUP(Cleanup)
		{
			TypeRegistry::Shutdown();
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

		TEST_METHOD(RTTI_Tests)
		{
			ActionEvent actionEvent;

			Assert::IsNotNull(actionEvent.As<ActionEvent>());
			Assert::IsNotNull(actionEvent.As<Action>());
			Assert::IsNotNull(actionEvent.As<Attributed>());
			Assert::IsNotNull(actionEvent.As<Scope>());
			Assert::IsNull(actionEvent.As<World>());
			Assert::IsNull(actionEvent.As<TestAction>());
		}

		TEST_METHOD(Ctors)
		{
			ActionEvent actionEvent;
			Assert::IsTrue(actionEvent["this"] == &actionEvent);
			Assert::IsTrue(actionEvent["Name"] == "ActionEvent");
			Assert::IsTrue(actionEvent["Subtype"] == "");
			Assert::IsTrue(actionEvent["Delay"] == 0);
		}

		TEST_METHOD(GetSetSubtype)
		{
			ActionEvent actionEvent;
			Assert::AreEqual(""s, actionEvent.GetSubtype());
			actionEvent.SetSubtype("A subtype");
			Assert::AreEqual("A subtype"s, actionEvent.GetSubtype());
		}

		TEST_METHOD(GetSetDelay)
		{
			ActionEvent actionEvent;
			Assert::AreEqual(0, actionEvent.GetDelay());
			actionEvent.SetDelay(5);
			Assert::AreEqual(5, actionEvent.GetDelay());
		}

		TEST_METHOD(Clone)
		{
			ActionEvent actionEvent;
			actionEvent.SetDelay(5);
			actionEvent.SetSubtype("subtype");

			ActionEvent* actionEventClone = actionEvent.Clone();
			Assert::AreEqual(5, actionEventClone->GetDelay());
			Assert::AreEqual("subtype"s, actionEventClone->GetSubtype());

			delete(actionEventClone);
		}

		TEST_METHOD(Update)
		{
			//no subtype/delay + test without aux attributes
			{
				World world;
				ActionEvent actionEvent;
				WorldState& state = const_cast<WorldState&>(world.GetWorldState());

				//test 1: update without subtype/delay (throw exception)
				Assert::ExpectException<std::runtime_error>([&actionEvent, &state] { actionEvent.Update(state); });

				//test 2: Enqueue without auxiliary attributes (kinda useless, but doable)
				Assert::AreEqual(0_z, world.GetEventQueue().Size());
				actionEvent.SetSubtype("A subtype");
				actionEvent.SetDelay(5);
				actionEvent.Update(state);

				Assert::AreEqual(1_z, world.GetEventQueue().Size());
				auto eventInfo = world.GetEventQueue().GetEvents()[0];
				Assert::AreEqual(world.GetGameClock().CurrentTime() + std::chrono::milliseconds(5), eventInfo.second);
				auto event = eventInfo.first->As<Event<EventMessageAttributed>>();
				Assert::IsNotNull(event);

				//check ema info
				auto& eventMsg = event->GetMessage();
				Assert::AreEqual(0_z, eventMsg.AuxiliaryAttributes().Size());
				Assert::AreEqual("A subtype"s, eventMsg.GetSubtype());
			}
			
			//test with aux attributes
			{
				World world;
				ActionEvent actionEvent;
				WorldState& state = const_cast<WorldState&>(world.GetWorldState());

				actionEvent.SetSubtype("A subtype");
				actionEvent.SetDelay(5);
				actionEvent.AppendAuxiliaryAttribute("Argument 1") = 5;
				actionEvent.AppendAuxiliaryAttribute("Argument 2") = "an argument";

				Assert::AreEqual(0_z, world.GetEventQueue().Size());
				actionEvent.Update(state);

				Assert::AreEqual(1_z, world.GetEventQueue().Size());
				auto eventInfo = world.GetEventQueue().GetEvents()[0];
				Assert::AreEqual(world.GetGameClock().CurrentTime() + std::chrono::milliseconds(5), eventInfo.second);
				auto event = eventInfo.first->As<Event<EventMessageAttributed>>();
				Assert::IsNotNull(event);

				//check ema info
				auto& eventMsg = event->GetMessage();
				Assert::AreEqual("A subtype"s, eventMsg.GetSubtype());
				Assert::AreEqual(2_z, eventMsg.AuxiliaryAttributes().Size());
				Assert::IsTrue(*eventMsg.Find("Argument 1") == 5);
				Assert::IsTrue(*eventMsg.Find("Argument 2") == "an argument");
			}
		}

		TEST_METHOD(TestWithReactionAttributed)
		{
			World world;
			ActionEvent actionEvent;
			//WorldState& state = const_cast<WorldState&>(world.GetWorldState());
			WorldState& state = (world.GetWorldState());
			Assert::AreSame(world, *state.mWorld);
			ReactionAttributed reaction("A subtype");
			TestAction* testAction = new TestAction();
			reaction.AdoptAction(*testAction);

			actionEvent.SetSubtype("A subtype");
			actionEvent.SetDelay(0);
			actionEvent.AppendAuxiliaryAttribute("Argument 1") = 5;
			actionEvent.AppendAuxiliaryAttribute("Argument 2") = "an argument";

			//update to cause the action to queue the event
			Assert::AreEqual(0_z, world.GetEventQueue().Size());
			actionEvent.Update(state);
			Assert::AreEqual(1_z, world.GetEventQueue().Size());

			//world update so eventqueue that will send the event and reactionAttributed will react
			Assert::AreEqual(0, testAction->mUpdateCount);
			world.Update();
			Assert::AreEqual(0_z, world.GetEventQueue().Size());
			Assert::AreEqual(1, testAction->mUpdateCount);

			//make sure reactionAttributed copied attributes
			Assert::IsTrue(reaction["Argument 1"] == 5);
			Assert::IsTrue(reaction["Argument 2"] == "an argument");
			
			Event<EventMessageAttributed>::UnsubscribeAll();
		}

	private:
		static _CrtMemState sStartMemState;	//for memory leak detection
	};
	_CrtMemState ActionEventTests::sStartMemState;
}