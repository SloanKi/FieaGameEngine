#include "pch.h"
#include "CppUnitTest.h"
#include "TypeRegistry.h"
#include "ActionList.h"
#include "TestAction.h"
#include "Entity.h"
#include "WorldState.h"
#include "ActionListSwitch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;
using namespace UnitTests;
using namespace std;
using namespace std::string_literals;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	inline std::wstring ToString<Library::DatumType>(const Library::DatumType& t)
	{
		RETURN_WIDE_STRING(&t);
	}

	template<>
	inline std::wstring ToString<Entity>(Entity* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<ActionList>(const ActionList& t)
	{
		RETURN_WIDE_STRING(&t);
	}

	template<>
	inline std::wstring ToString<ActionList>(ActionList* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<Hashmap<const std::string, Action*>>(const Hashmap<const std::string, Action*>& t)
	{
		RETURN_WIDE_STRING(&t);
	}
}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(ActionListTests)
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
			//TypeRegistry::RegisterType(Action::TypeIdClass(), Action::Signatures());
			TypeRegistry::RegisterType(Entity::TypeIdClass(), Entity::Signatures());
			TypeRegistry::RegisterType(ActionList::TypeIdClass(), ActionList::Signatures());
			TypeRegistry::RegisterType(TestAction::TypeIdClass(), TestAction::Signatures());
			TypeRegistry::RegisterType(ActionListSwitch::TypeIdClass(), ActionListSwitch::Signatures());
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

		TEST_METHOD(Constructor)
		{
			//RTTI/Default Ctor
			{
				ActionList actionList;
				Assert::IsTrue(actionList["this"] == &actionList);
				Assert::IsTrue(actionList["Name"] == "");
				Assert::IsNull(actionList.GetEntity());
				Assert::IsNull(actionList.GetActionList());
				Assert::AreEqual(DatumType::Table, actionList["Actions"].Type());
				Assert::AreEqual(23_z, actionList["Actions"].Capacity());
			}

			//ctor with name (r value)
			{
				ActionList actionList("Hello"s);
				Assert::IsTrue(actionList["this"] == &actionList);
				Assert::IsTrue(actionList["Name"] == "Hello");
				Assert::IsNull(actionList.GetEntity());
				Assert::IsNull(actionList.GetActionList());
				Assert::AreEqual(DatumType::Table, actionList["Actions"].Type());
				Assert::AreEqual(23_z, actionList["Actions"].Capacity());
			}

			//ctor with name (l value)
			{
				std::string name = "Hello";
				ActionList actionList(name);
				Assert::IsTrue(actionList["this"] == &actionList);
				Assert::IsTrue(actionList["Name"] == "Hello");
				Assert::IsNull(actionList.GetEntity());
				Assert::IsNull(actionList.GetActionList());
				Assert::AreEqual(DatumType::Table, actionList["Actions"].Type());
				Assert::AreEqual(23_z, actionList["Actions"].Capacity());
			}

			//ctor with name and entity
			{
				Entity entity;
				ActionList actionList("Hello"s, &entity);
				Assert::IsTrue(actionList["this"] == &actionList);
				Assert::IsTrue(actionList["Name"] == "Hello");
				Assert::AreEqual(&entity, actionList.GetEntity());
				Assert::IsNull(actionList.GetActionList());
				Assert::AreEqual(DatumType::Table, actionList["Actions"].Type());
				Assert::AreEqual(23_z, actionList["Actions"].Capacity());
			}
		}

		TEST_METHOD(GetAdoptCreateActions)
		{
			TestActionFactory actionFact;
			ActionList actionList;
			Datum& actionDat = actionList.Actions();
			Assert::AreEqual(DatumType::Table, actionDat.Type());
			Assert::AreEqual(23_z, actionDat.Capacity());
			Assert::AreEqual(0_z, actionDat.Size());

			//create test
			auto action = actionList.CreateAction("TestAction", "Instance");
			Assert::AreEqual(1_z, actionDat.Size());
			Assert::IsNotNull(action);
			Assert::AreEqual("Instance"s, action->Name());
			Assert::AreEqual(&actionList, action->GetActionList());
			Assert::IsTrue(action->Is(TestAction::TypeIdClass()));

			//adopt action test
			TestAction* action2 = new TestAction();
			actionList.AdoptAction(*action2);
			Assert::AreEqual(2_z, actionDat.Size());
			Assert::AreEqual(&actionList, action2->GetActionList());

			//const actions()
			const ActionList& constActionList = actionList;
			Assert::AreEqual(2_z, constActionList.Actions().Size());
		}

		TEST_METHOD(Clone)
		{
			ActionList actionList("Test");
			ActionList* clone = actionList.Clone();
			Assert::AreNotSame(actionList, *clone);
			Assert::AreEqual(actionList, *clone);
			delete(clone);
		}

		TEST_METHOD(Update)
		{
			WorldState state;
			ActionList actionList;
			TestAction act1, act2, act3;
			actionList.AdoptAction(act1);
			actionList.AdoptAction(act2);
			actionList.AdoptAction(act3);

			//make sure they are in there
			Assert::AreEqual(actionList.Actions().Size(), 3_z);

			//make sure update is being called on all actions
			actionList.Update(state);
			Assert::AreEqual(act1.mUpdateCount, 1);
			Assert::AreEqual(act2.mUpdateCount, 1);
			Assert::AreEqual(act3.mUpdateCount, 1);

			//2x more for sanity
			actionList.Update(state);
			actionList.Update(state);
			Assert::AreEqual(act1.mUpdateCount, 3);
			Assert::AreEqual(act2.mUpdateCount, 3);
			Assert::AreEqual(act3.mUpdateCount, 3);
		}

		TEST_METHOD(ActionListSwitchTests)
		{
			ActionListSwitch actionSwitch;
			WorldState state;

			//validate constructor
			Assert::IsTrue(actionSwitch["this"] == &actionSwitch);
			Assert::IsTrue(actionSwitch["Name"] == "Switch");
			Assert::IsNull(actionSwitch.GetEntity());
			Assert::IsNull(actionSwitch.GetActionList());
			Assert::AreEqual(DatumType::Table, actionSwitch["Actions"].Type());
			Assert::AreEqual(23_z, actionSwitch["Actions"].Capacity());

			TestAction* action1 = new TestAction();
			TestAction* action2 = new TestAction();
			TestAction* action3 = new TestAction();

			//add/get cases test
			auto& caseRef = actionSwitch.GetCases();
			Assert::AreEqual(0_z, caseRef.Size());
			Assert::AreEqual(0_z, actionSwitch["Actions"].Size());
			actionSwitch.AddCase(*action1, "Case1");
			Assert::AreEqual(1_z, caseRef.Size());
			Assert::AreEqual(1_z, actionSwitch["Actions"].Size());

			actionSwitch.AddCase(*action2, "Case2");
			actionSwitch.AddCase(*action3, "Case3");
			Assert::AreEqual(3_z, caseRef.Size());
			Assert::AreEqual(3_z, actionSwitch["Actions"].Size());

			//const get cases
			const ActionListSwitch& constSwitch = actionSwitch;
			Assert::AreSame(caseRef, constSwitch.GetCases());

			//set/get value
			actionSwitch.SetValue("Not a case");
			Assert::AreEqual("Not a case"s, actionSwitch.GetValue());

			//update with invalid case (throw exception)
			Assert::ExpectException<std::runtime_error>([&actionSwitch, &state] { actionSwitch.Update(state); });

			//valid updates
			actionSwitch.SetValue("Case2");
			actionSwitch.Update(state);
			Assert::AreEqual(0, action1->mUpdateCount);
			Assert::AreEqual(1, action2->mUpdateCount);
			Assert::AreEqual(0, action3->mUpdateCount);
			actionSwitch.Update(state);
			actionSwitch.Update(state);
			Assert::AreEqual(0, action1->mUpdateCount);
			Assert::AreEqual(3, action2->mUpdateCount);
			Assert::AreEqual(0, action3->mUpdateCount);

			//switch value and update
			actionSwitch.SetValue("Case1");
			actionSwitch.Update(state);
			Assert::AreEqual(1, action1->mUpdateCount);
			Assert::AreEqual(3, action2->mUpdateCount);
			Assert::AreEqual(0, action3->mUpdateCount);

			//remove cases test
			actionSwitch.RemoveCase("Case1");
			actionSwitch.RemoveCase("Case2");
			actionSwitch.RemoveCase("Case3");
		}

	private:
		static _CrtMemState sStartMemState;	//for memory leak detection
	};
	_CrtMemState ActionListTests::sStartMemState;
}