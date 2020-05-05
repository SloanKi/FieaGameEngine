#include "pch.h"
#include "CppUnitTest.h"
#include "TypeRegistry.h"
#include "ReactionAttributed.h"
#include "EventMessageAttributed.h"
#include "World.h"
#include "Event.h"
#include "Entity.h"
#include "TestAction.h"
#include "ActionIncrement.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;
using namespace UnitTests;
using namespace std;
using namespace std::string_literals;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	inline std::wstring ToString<World>(World* t)
	{
		RETURN_WIDE_STRING(t);
	}
	template<>
	inline std::wstring ToString<World>(const World* t)
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
	}
}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(ReactionAttributedTests)
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
			TypeRegistry::RegisterType(ReactionAttributed::TypeIdClass(), ReactionAttributed::Signatures());
			TypeRegistry::RegisterType(EventMessageAttributed::TypeIdClass(), EventMessageAttributed::Signatures());
			TypeRegistry::RegisterType(World::TypeIdClass(), World::Signatures());
			TypeRegistry::RegisterType(Entity::TypeIdClass(), Entity::Signatures());
			TypeRegistry::RegisterType(TestAction::TypeIdClass(), TestAction::Signatures());
			TypeRegistry::RegisterType(ActionIncrement::TypeIdClass(), ActionIncrement::Signatures());
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
			ReactionAttributed reaction;
			Assert::IsNotNull(reaction.As<ReactionAttributed>());
			Assert::IsNotNull(reaction.As<Reaction>());
			Assert::IsNotNull(reaction.As<ActionList>());
			Assert::IsNotNull(reaction.As<Action>());
			Assert::IsNotNull(reaction.As<Attributed>());
			Assert::IsNotNull(reaction.As<Scope>());
			Assert::IsNull(reaction.As<EventMessageAttributed>());
			Assert::IsNull(reaction.As<World>());
		}

		TEST_METHOD(Ctors)
		{
			//default ctor
			{
				Assert::AreEqual(0_z, Event<EventMessageAttributed>::GetSubscribers().Size());
				ReactionAttributed reaction;
				Assert::AreEqual(1_z, Event<EventMessageAttributed>::GetSubscribers().Size());
				
				//actionlist properties
				Assert::IsTrue(reaction["this"] == &reaction);
				Assert::IsTrue(reaction["Name"] == "");
				Assert::IsNull(reaction.GetEntity());
				Assert::IsNull(reaction.GetActionList());
				Assert::AreEqual(DatumType::Table, reaction["Actions"].Type());
				Assert::AreEqual(23_z, reaction["Actions"].Capacity());
				
				//subtype
				Assert::IsTrue(reaction["Subtype"] == "");

				Event<EventMessageAttributed>::Unsubscribe(reaction);
				Assert::AreEqual(0_z, Event<EventMessageAttributed>::GetSubscribers().Size());
			}


			//subtype ctor
			{
				Assert::AreEqual(0_z, Event<EventMessageAttributed>::GetSubscribers().Size());
				ReactionAttributed reaction("A type");
				Assert::AreEqual(1_z, Event<EventMessageAttributed>::GetSubscribers().Size());

				//actionlist properties
				Assert::IsTrue(reaction["this"] == &reaction);
				Assert::IsTrue(reaction["Name"] == "");
				Assert::IsNull(reaction.GetEntity());
				Assert::IsNull(reaction.GetActionList());
				Assert::AreEqual(DatumType::Table, reaction["Actions"].Type());
				Assert::AreEqual(23_z, reaction["Actions"].Capacity());

				//subtype
				Assert::IsTrue(reaction["Subtype"] == "A type");

				Event<EventMessageAttributed>::Unsubscribe(reaction);
				Assert::AreEqual(0_z, Event<EventMessageAttributed>::GetSubscribers().Size());
			}

			//everything ctor
			{
				Entity entity;
				Assert::AreEqual(0_z, Event<EventMessageAttributed>::GetSubscribers().Size());
				ReactionAttributed reaction("A type", "A Name", &entity);
				Assert::AreEqual(1_z, Event<EventMessageAttributed>::GetSubscribers().Size());

				//actionlist properties
				Assert::IsTrue(reaction["this"] == &reaction);
				Assert::IsTrue(reaction["Name"] == "A Name");
				Assert::IsNotNull(reaction.GetEntity());
				Assert::AreEqual(&entity, reaction.GetEntity());
				Assert::IsNull(reaction.GetActionList());
				Assert::AreEqual(DatumType::Table, reaction["Actions"].Type());
				Assert::AreEqual(23_z, reaction["Actions"].Capacity());

				//subtype
				Assert::IsTrue(reaction["Subtype"] == "A type");

				Event<EventMessageAttributed>::Unsubscribe(reaction);
				Assert::AreEqual(0_z, Event<EventMessageAttributed>::GetSubscribers().Size());
			}
		}

		TEST_METHOD(GetSetSubtype)
		{
			ReactionAttributed reaction;
			Assert::AreEqual(""s, reaction.GetSubtype());
			reaction.SetSubtype("A subtype");
			Assert::AreEqual("A subtype"s, reaction.GetSubtype());
		}

		TEST_METHOD(Clone)
		{
			Assert::AreEqual(0_z, Event<EventMessageAttributed>::GetSubscribers().Size());
			ReactionAttributed reaction("A type", "A Name");
			Assert::AreEqual(1_z, Event<EventMessageAttributed>::GetSubscribers().Size());


			auto reactionClone = reaction.Clone();
			Assert::AreEqual(2_z, Event<EventMessageAttributed>::GetSubscribers().Size());
			Assert::AreEqual(reaction.Name(), reactionClone->Name());
			Assert::AreEqual(reaction.GetSubtype(), reactionClone->GetSubtype());

			delete(reactionClone);


			Event<EventMessageAttributed>::UnsubscribeAll();
		}

		TEST_METHOD(Notify)
		{
			//prep work 
			World world;
			EventMessageAttributed eventMsg("test type", &world);
			ReactionAttributed reaction("test type");
			TestAction* testAction = new TestAction();
			ActionIncrement* actionIncrement = new ActionIncrement();
			actionIncrement->SetOperandString("Health");
			eventMsg.AppendAuxiliaryAttribute("Health") = 20;
			reaction.AdoptAction(*testAction);
			reaction.AdoptAction(*actionIncrement);

			//test arguments are copied and actions are completed
			Event<EventMessageAttributed> event(eventMsg);
			Assert::AreEqual(0, testAction->mUpdateCount);
			reaction.Notify(event);
			Assert::AreEqual(1, testAction->mUpdateCount);

			//really bad example, just don't want to rewrite increment right now
			//(increment should search for attribute called "Operand" but doesn't)
			Assert::IsNotNull(reaction.Find("Health"));
			Assert::AreEqual(21, reaction["Health"].Get<int>());

			//ema is const, so should still be 20
			Assert::AreEqual(20, event.GetMessage().Find("Health")->Get<int>());


			//test that sending an event with wrong subtype won't do anything.
			EventMessageAttributed eventMsg2("wrong", &world);
			Event<EventMessageAttributed> event2(eventMsg2);
			reaction.Notify(event2);
			Assert::AreEqual(1, testAction->mUpdateCount);

			Event<EventMessageAttributed>::UnsubscribeAll();
		}

	private:
		static _CrtMemState sStartMemState;	//for memory leak detection
	};
	_CrtMemState ReactionAttributedTests::sStartMemState;
}