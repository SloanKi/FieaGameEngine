#include "pch.h"
#include "CppUnitTest.h"
#include "TypeRegistry.h"
#include "Attributed.h"
#include "Entity.h"
#include "Action.h"
#include "TestAction.h"
#include "WorldState.h"
#include "ActionList.h"
#include "ActionIncrement.h"
#include "ActionCreateAction.h"
#include "World.h"
#include "Sector.h"
#include "ActionDestroyAction.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;
using namespace UnitTests;
using namespace std;
using namespace std::string_literals;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
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

	template<>
	inline std::wstring ToString<TestAction>(const TestAction& t)
	{
		RETURN_WIDE_STRING(&t);
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
	inline std::wstring ToString<TestAction>(TestAction* t)
	{
		RETURN_WIDE_STRING(t);
	}
}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(ActionTests)
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
			TypeRegistry::RegisterType(Entity::TypeIdClass(), Entity::Signatures());
			TypeRegistry::RegisterType(ActionList::TypeIdClass(), ActionList::Signatures());
			TypeRegistry::RegisterType(TestAction::TypeIdClass(), TestAction::Signatures());
			TypeRegistry::RegisterType(ActionIncrement::TypeIdClass(), ActionIncrement::Signatures());
			TypeRegistry::RegisterType(ActionCreateAction::TypeIdClass(), ActionCreateAction::Signatures());
			TypeRegistry::RegisterType(ActionDestroyAction::TypeIdClass(), ActionDestroyAction::Signatures());
			TypeRegistry::RegisterType(Sector::TypeIdClass(), Sector::Signatures());
			TypeRegistry::RegisterType(World::TypeIdClass(), World::Signatures());
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
				TestAction action;
				Assert::IsTrue(action["this"] == &action);
				Assert::IsTrue(action["Name"] == "");
				Assert::IsNull(action.GetEntity());
				Assert::IsNull(action.GetActionList());
				Assert::AreEqual(TestAction::TypeIdClass(), action.TypeIdInstance());
				Assert::AreNotEqual(Action::TypeIdClass(), action.TypeIdInstance());
			}

			//ctor with name (r value)
			{
				TestAction action("Hello"s);
				Assert::IsTrue(action["this"] == &action);
				Assert::IsTrue(action["Name"] == "Hello");
				Assert::IsNull(action.GetEntity());
				Assert::IsNull(action.GetActionList());
				Assert::AreEqual(TestAction::TypeIdClass(), action.TypeIdInstance());
				Assert::AreNotEqual(Action::TypeIdClass(), action.TypeIdInstance());
			}

			//ctor with name (l value)
			{
				std::string name = "Hello";
				TestAction action(name);
				Assert::IsTrue(action["this"] == &action);
				Assert::IsTrue(action["Name"] == "Hello");
				Assert::IsNull(action.GetEntity());
				Assert::IsNull(action.GetActionList());
				Assert::AreEqual(TestAction::TypeIdClass(), action.TypeIdInstance());
				Assert::AreNotEqual(Action::TypeIdClass(), action.TypeIdInstance());
			}

			//ctor with name and entity
			{
				Entity entity;
				TestAction action("Hello"s, &entity);
				Assert::IsTrue(action["this"] == &action);
				Assert::IsTrue(action["Name"] == "Hello");
				Assert::AreEqual(&entity, action.GetEntity());
				Assert::IsNull(action.GetActionList());
				Assert::AreEqual(TestAction::TypeIdClass(), action.TypeIdInstance());
				Assert::AreNotEqual(Action::TypeIdClass(), action.TypeIdInstance());
			}
		}

		TEST_METHOD(GetSetName)
		{
			TestAction action;
			Assert::AreEqual(""s, action.Name());
			Assert::IsTrue(action["Name"] == "");

			//r value
			action.SetName("Bob");
			Assert::AreEqual("Bob"s, action.Name());
			Assert::IsTrue(action["Name"] == "Bob");

			//l value
			std::string name = "not bob";
			action.SetName(name);
			Assert::AreEqual(name, action.Name());
			Assert::IsTrue(action["Name"] == name);
		}

		TEST_METHOD(GetSetEntity)
		{
			//tests from before ActionList Change
			{
				Entity entity;
				Action* action = new TestAction();
	
				Assert::IsNull(action->GetEntity());
				Assert::AreEqual(0_z, entity.Actions().Size());
				action->SetEntity(entity);
				Assert::AreEqual(&entity, action->GetEntity());
				Assert::AreEqual(1_z, entity.Actions().Size());
	
				//check that switching to another entity will remove from old one
				Entity entity2;
				Assert::AreEqual(0_z, entity2.Actions().Size());
				action->SetEntity(entity2);
				Assert::AreEqual(&entity2, action->GetEntity());
				Assert::AreEqual(0_z, entity.Actions().Size());
				Assert::AreEqual(1_z, entity2.Actions().Size());
			}

			//ActionList test
			Entity entity;
			ActionList* actionList = new ActionList();
			TestAction* action = new TestAction();

			Assert::IsNull(action->GetEntity());
			Assert::IsNull(actionList->GetEntity());

			actionList->SetEntity(entity);
			Assert::AreEqual(&entity, actionList->GetEntity());
			Assert::IsNull(action->GetEntity());

			action->SetActionList(*actionList);
			Assert::AreEqual(actionList, action->GetActionList());
			Assert::AreEqual(&entity, action->GetEntity());

			//double check it keeps going down the chain
			Entity entity2;
			ActionList* actionList2 = new ActionList();
			actionList2->SetEntity(entity2);
			actionList->SetActionList(*actionList2);
			Assert::AreEqual(actionList2, actionList->GetActionList());
			Assert::AreEqual(actionList, action->GetActionList());
			Assert::AreEqual(&entity2, actionList2->GetEntity());
			Assert::AreEqual(&entity2, actionList->GetEntity());
			Assert::AreEqual(&entity2, action->GetEntity());
		}

		TEST_METHOD(GetSetActionList)
		{
			ActionList actionList;
			TestAction* action = new TestAction();


			Assert::IsNull(action->GetActionList());
			Assert::AreEqual(0_z, actionList.Actions().Size());
			action->SetActionList(actionList);
			Assert::AreEqual(&actionList, action->GetActionList());
			Assert::AreEqual(1_z, actionList.Actions().Size());

			//check that switching to another actionlist will remove from old one
			ActionList actionList2;
			Assert::AreEqual(0_z, actionList2.Actions().Size());
			action->SetActionList(actionList2);
			Assert::AreEqual(&actionList2, action->GetActionList());
			Assert::AreEqual(1_z, actionList2.Actions().Size());
			Assert::AreEqual(0_z, actionList.Actions().Size());
		}

		TEST_METHOD(Clone)
		{
			TestAction action("Test");
			TestAction* clone = action.Clone();
			Assert::AreNotSame(action, *clone);
			Assert::AreEqual(action, *clone);
			delete(clone);
		}

		TEST_METHOD(Update)
		{
			WorldState state;
			TestAction action;
			Assert::AreEqual(0, action.mUpdateCount);
			action.Update(state);
			Assert::AreEqual(1, action.mUpdateCount);
			action.Update(state);
			action.Update(state);
			Assert::AreEqual(3, action.mUpdateCount);
		}

		TEST_METHOD(ActionIncrementTest)
		{
			WorldState state;

			//test 1: Action has no parent, so no valid operand (throw exception) 
			ActionIncrement* actionIncrement = new ActionIncrement();
			Assert::AreEqual("Increment"s, actionIncrement->Name());
			Assert::AreEqual(""s, actionIncrement->GetOperandString());
			Assert::AreEqual(1, actionIncrement->GetStep());
			//sanity checks first
			Assert::IsTrue(actionIncrement->Append("Name") == "Increment");
			Assert::IsTrue(actionIncrement->Append("Operand") == "");
			Assert::IsTrue(actionIncrement->Append("Step") == 1);
			Assert::ExpectException<std::runtime_error>([&actionIncrement, &state] { actionIncrement->Update(state); });

			//Test 2: valid operand
			Entity entity;
			entity.AppendAuxiliaryAttribute("Health") = 20;
			entity.AdoptAction(*actionIncrement);
			actionIncrement->SetOperandString("Health");
			Assert::IsTrue(actionIncrement->Append("Operand") == "Health"); //sanity check

			//int& health = entity["Health"].Get<int>();
			//Assert::AreEqual(20, health);
			Assert::IsTrue(entity["Health"] == 20);
			entity.Update(state); //will also call update on actionincrement
			Assert::IsTrue(entity["Health"] == 21);

			//test 3: change step amount
			actionIncrement->SetStep(5);
			Assert::IsTrue(actionIncrement->Append("Step") == 5); //sanity check
			entity.Update(state);
			Assert::IsTrue(entity["Health"] == 26);
		}

		TEST_METHOD(ActionCreateActionTest)
		{
			TestActionFactory actionFactory;

			//test on Entity
			{
				World world;
				Sector* sector = new Sector("Sector", &world);
				Entity* entity = new Entity("Entity", sector);

				//validate constructor
				ActionCreateAction* createAction = new ActionCreateAction();
				Assert::IsNull(createAction->GetEntity());
				Assert::IsNull(createAction->GetActionList());
				Assert::AreEqual("CreateAction"s, createAction->Name());
				Assert::AreEqual(""s, createAction->GetPrototype());
				Assert::AreEqual(""s, createAction->GetActionName());

				//exception if no parent
				WorldState state;
				Assert::ExpectException<std::runtime_error>([&createAction, &state] { createAction->Update(state); });

				entity->AdoptAction(*createAction);
				Assert::AreEqual(entity, createAction->GetEntity());
				//exception because prototype and name not set
				Assert::ExpectException<std::runtime_error>([&world] { world.Update(); });

				//get/set prototype
				createAction->SetPrototype("TestAction");
				Assert::AreEqual("TestAction"s, createAction->GetPrototype());
				//exception because name not set
				Assert::ExpectException<std::runtime_error>([&world] { world.Update(); });

				//get/set action name
				createAction->SetActionName("Action1");
				Assert::AreEqual("Action1"s, createAction->GetActionName());

				//Update
				Assert::AreEqual(1_z, entity->Actions().Size());
				world.Update(); //adds to queue to be created next update
				Assert::AreEqual(0_z, entity->Actions().Size()); //0 because create destroys itself
				world.Update(); //new action should be created now
				Assert::AreEqual(1_z, entity->Actions().Size());

				//verify action is correctly made
				Scope& actionScope = entity->Actions().Get<Scope>();
				TestAction* action = actionScope.As<TestAction>();
				Assert::IsNotNull(action);
				Assert::AreEqual(1, action->mUpdateCount);

				//verify that it hasn't created anymore secretly
				world.Update();
				Assert::AreEqual(1_z, entity->Actions().Size());
			}

			//also verify on actionlist
			{
				World world;
				Sector* sector = new Sector("Sector", &world);
				Entity* entity = new Entity("Entity", sector);
				ActionList* actionList = new ActionList("ActionList", entity);

				//validate constructor
				ActionCreateAction* createAction = new ActionCreateAction("TestAction", "action1");
				Assert::IsNull(createAction->GetEntity());
				Assert::IsNull(createAction->GetActionList());
				Assert::AreEqual("CreateAction"s, createAction->Name());
				Assert::AreEqual("TestAction"s, createAction->GetPrototype());
				Assert::AreEqual("action1"s, createAction->GetActionName());

				//exception if no parent
				WorldState state;
				Assert::ExpectException<std::runtime_error>([&createAction, &state] { createAction->Update(state); });

				actionList->AdoptAction(*createAction);
				Assert::AreEqual(entity, createAction->GetEntity());
				Assert::AreEqual(actionList, createAction->GetActionList());

				//Update
				Assert::AreEqual(1_z, actionList->Actions().Size());
				world.Update(); //adds to queue to be created next update
				Assert::AreEqual(0_z, actionList->Actions().Size());
				world.Update(); //new action should be created now
				//TODO: CHANGE THIS SLIGHTLY BECAUSE CREATE SHOULD REMOVE SELF
				//(need to change size values)
				Assert::AreEqual(1_z, actionList->Actions().Size());

				//verify action is correctly made
				Scope& actionScope = actionList->Actions().Get<Scope>();
				TestAction* action = actionScope.As<TestAction>();
				Assert::IsNotNull(action);
				Assert::AreEqual(1, action->mUpdateCount);

				//verify that it hasn't created anymore secretly
				world.Update();
				Assert::AreEqual(1_z, actionList->Actions().Size());
			}
		}

		TEST_METHOD(ActionDestroyActionTest)
		{
			World world;
			Sector* sector = new Sector("Sector", &world);
			Entity* entity = new Entity("Entity", sector);

			TestAction* action1 = new TestAction("action1", entity);
			TestAction* action2 = new TestAction("action2", entity);
			TestAction* action3 = new TestAction("action3", entity);
			action1; action2; action3; //STOP WARNING ME
			Assert::AreEqual(3_z, entity->Actions().Size());

			//validate constructor
			ActionDestroyAction* destroyAction = new ActionDestroyAction();
			Assert::IsNull(destroyAction->GetEntity());
			Assert::IsNull(destroyAction->GetActionList());
			Assert::AreEqual("DestroyAction"s, destroyAction->Name());
			Assert::AreEqual(""s, destroyAction->GetActionName());

			//exception if no parent
			WorldState state;
			Assert::ExpectException<std::runtime_error>([&destroyAction, &state] { destroyAction->Update(state); });

			entity->AdoptAction(*destroyAction);
			Assert::AreEqual(entity, destroyAction->GetEntity());
			Assert::AreEqual(4_z, entity->Actions().Size());
			//exception because prototype and name not set
			Assert::ExpectException<std::runtime_error>([&world] { world.Update(); });

			//get/set action name
			destroyAction->SetActionName("action2");
			Assert::AreEqual("action2"s, destroyAction->GetActionName());

			//update
			Assert::AreEqual(4_z, entity->Actions().Size());
			world.Update(); //Destroy happens at end of update, so should be done right now
			Assert::AreEqual(2_z, entity->Actions().Size()); //2 because destroys self too

			Assert::AreEqual(action1, entity->Actions().Get<Scope>().As<TestAction>());
			Assert::AreEqual(action3, entity->Actions().Get<Scope>(1).As<TestAction>());
		}

	private:
		static _CrtMemState sStartMemState;	//for memory leak detection
	};
	_CrtMemState ActionTests::sStartMemState;
}