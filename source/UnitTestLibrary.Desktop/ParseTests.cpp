#include "pch.h"
#include "CppUnitTest.h"
#include "JsonParseMaster.h"
#include "IJsonParseHelper.h"
#include "JsonFoo.h"
#include "Foo.h"
#include "vector.h"
#include "TestParseHelper.h"
#include "JsonTableParseHelper.h"
#include "Scope.h"

#pragma warning(push)
#pragma warning(disable:4201)
#include <glm/gtx/string_cast.hpp>
#pragma warning(pop)

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace std::string_literals;
using namespace Library;
using namespace UnitTests;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	inline std::wstring ToString<Library::JsonParseMaster>(Library::JsonParseMaster* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<FooSharedData>(const FooSharedData& t)
	{
		RETURN_WIDE_STRING(&t);
	}

	template<>
	inline std::wstring ToString<JsonParseMaster::SharedData>(JsonParseMaster::SharedData* t)
	{
		RETURN_WIDE_STRING(&t);
	}

	template<>
	inline std::wstring ToString<JsonParseMaster::SharedData>(const JsonParseMaster::SharedData* t)
	{
		RETURN_WIDE_STRING(&t);
	}


	template<>
	inline std::wstring ToString<glm::vec4>(const glm::vec4& t)
	{
		RETURN_WIDE_STRING(glm::to_string(t).c_str());
	}

	template<>
	inline std::wstring ToString<glm::mat4>(const glm::mat4& t)
	{
		RETURN_WIDE_STRING(glm::to_string(t).c_str());
	}
}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(ParsingTests)
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

		TEST_METHOD(FooSharedDataTest)
		{
			JsonFoo foo;
			FooSharedData fooData(foo);

			//Set/get parse master 
			Assert::IsNull(fooData.GetJsonParseMaster());
			JsonParseMaster parseMaster(fooData);
			Assert::AreEqual(fooData.GetJsonParseMaster(), &parseMaster);

			//increment/decrement/get depth
			Assert::AreEqual(fooData.Depth(), 0_z);
			fooData.IncrementDepth();
			Assert::AreEqual(fooData.Depth(), 1_z);
			fooData.DecrementDepth();
			Assert::AreEqual(fooData.Depth(), 0_z);

			//Initialize check
			fooData.IncrementDepth();
			fooData.IncrementDepth();
			Assert::AreEqual(fooData.Depth(), 2_z);
			fooData.Initialize();
			Assert::AreEqual(fooData.Depth(), 0_z);

			//Equality
			FooSharedData fooDat2(foo);
			Assert::AreEqual(fooDat2, fooData);
			fooDat2.mData = nullptr;
			Assert::AreNotEqual(fooDat2, fooData);

			//create
			JsonParseMaster::SharedData* newData = fooData.Create();
			Assert::IsNotNull(newData->As<FooSharedData>());

			delete(newData);
		}

		TEST_METHOD(SharedDataRTTI)
		{
			JsonFoo jsonFoo;
			Foo foo;
			FooSharedData fooDat(jsonFoo), fooDat2(jsonFoo);

			//equals
			Assert::IsTrue(fooDat.Equals(&fooDat));
			Assert::IsTrue(fooDat.Equals(&fooDat2));
			Assert::IsFalse(fooDat.Equals(nullptr));
			Assert::IsFalse(fooDat.Equals(&foo));

			//Is
			Assert::IsTrue(fooDat.Is(fooDat.TypeIdClass()));
			Assert::IsTrue(fooDat.Is("FooSharedData"));
			Assert::IsTrue(fooDat.Is(JsonParseMaster::SharedData::TypeIdClass()));
			Assert::IsTrue(fooDat.Is("SharedData"));
			Assert::IsFalse(fooDat.Is(foo.TypeIdClass()));
			Assert::IsFalse(fooDat.Is("Foo"));

			//As
			Assert::IsNotNull(fooDat.As<FooSharedData>());
			Assert::IsNotNull(fooDat.As<JsonParseMaster::SharedData>());
			Assert::IsNull(fooDat.As<Foo>());
		}

		TEST_METHOD(HelperRTTI)
		{
			Foo foo;
			FooJsonParseHelper fooHelper;
			TestParseHelper testHelper, testHelper2;

			//equals
			Assert::IsTrue(testHelper.Equals(&testHelper));
			Assert::IsTrue(testHelper.Equals(&testHelper2));
			Assert::IsFalse(testHelper.Equals(nullptr));
			Assert::IsFalse(testHelper.Equals(&foo));
			Assert::IsFalse(testHelper.Equals(&fooHelper));

			//is
			Assert::IsTrue(testHelper.Is(testHelper.TypeIdClass()));
			Assert::IsTrue(testHelper.Is("TestParseHelper"));
			Assert::IsTrue(testHelper.Is(IJsonParseHelper::TypeIdClass()));
			Assert::IsTrue(testHelper.Is("IJsonParseHelper"));
			Assert::IsFalse(testHelper.Is(foo.TypeIdClass()));
			Assert::IsFalse(testHelper.Is("Foo"));

			//as
			Assert::IsNotNull(testHelper.As<TestParseHelper>());
			Assert::IsNotNull(testHelper.As<IJsonParseHelper>());
			Assert::IsNull(testHelper.As<Foo>());
		}

		TEST_METHOD(GeneralTestHelper)
		{
			TestSharedData sharedDat;
			JsonParseMaster master(sharedDat);
			TestParseHelper helper;
			master.AddHelper(helper);

			const std::string jsonStr =
				R"({
					"Health": 100,
					"DPS": 1.5,
					"Aliases": ["A", "B", "C"],
					"Stunned": false
				})";

			master.Parse(jsonStr);
			Assert::AreEqual(helper.startCalls, 6);
			Assert::AreEqual(helper.endCalls, 6);
			Assert::AreEqual(sharedDat.maxDepth, 1_z);

			const std::string jsonStr2 =
				R"({
					 "Address": {
                          "type": "table",
                          "value": {
                               "Street": {
                                    "type": "string",
                                    "value": "123 anywhere st"
                                }
                           }
                      }
				   })";

			master.Initialize();
			Assert::AreEqual(helper.startCalls, 0);
			Assert::AreEqual(helper.endCalls, 0);
			Assert::AreEqual(sharedDat.maxDepth, 0_z);


			master.Parse(jsonStr2);
			Assert::AreEqual(helper.startCalls, 6);
			Assert::AreEqual(helper.endCalls, 6);
			Assert::AreEqual(sharedDat.maxDepth, 4_z);
		}

		TEST_METHOD(MasterInitialize)
		{
			TestSharedData data;
			JsonParseMaster master(data);
			TestParseHelper helper;
			master.AddHelper(helper);

			master.Initialize();
			Assert::IsTrue(data.bInitialized);
			Assert::IsTrue(helper.bInitialized);
		}

		TEST_METHOD(StartEndHandlerJsonFoo)
		{
			const std::string jsonStr =
				R"({
					"Health": 100,
					"DPS": 1.5,
					"Aliases": ["A", "B", "C"],
					"Stunned": false
				})";

			//parse from string
			JSONCPP_STRING err;
			Json::Value root;
			Json::CharReaderBuilder builder;
			std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
			bool parsed = reader->parse(jsonStr.c_str(), jsonStr.c_str() + jsonStr.length(), &root, &err);
			Logger::WriteMessage(err.c_str());
			Assert::IsTrue(parsed);

			//valid handles
			FooJsonParseHelper helper;
			JsonFoo foo;
			FooSharedData sharedData(foo);
			Assert::IsTrue(helper.StartHandler(sharedData, "Health", root["Health"]));
			Assert::AreEqual(sharedData.mData->mHealth, 100);
			Assert::IsTrue(helper.StartHandler(sharedData, "DPS", root["DPS"]));
			Assert::AreEqual(sharedData.mData->mDPS, 1.5f);
			Assert::IsTrue(helper.StartHandler(sharedData, "Stunned", root["Stunned"]));
			Assert::AreEqual(sharedData.mData->bStunned, false);
			/*Assert::IsTrue(helper.StartHandler(sharedData, "Aliases", root["Aliases"][0], true, 0));
			Assert::IsTrue(helper.StartHandler(sharedData, "Aliases", root["Aliases"][1], true, 1));
			Assert::IsTrue(helper.StartHandler(sharedData, "Aliases", root["Aliases"][2], true, 2));
			Assert::AreEqual(sharedData.mData->mAliases[0], "A"s);
			Assert::AreEqual(sharedData.mData->mAliases[1], "B"s);
			Assert::AreEqual(sharedData.mData->mAliases[2], "C"s);*/

			//invalid handles
			Assert::IsFalse(helper.StartHandler(sharedData, "Bleh", root));
			Assert::IsFalse(helper.StartHandler(sharedData, "Health", root));
			Assert::IsFalse(helper.StartHandler(sharedData, "DPS", root));
			Assert::IsFalse(helper.StartHandler(sharedData, "Aliases", root));

			//Create test
			auto newHelper = helper.Create();
			Assert::IsNotNull(newHelper);
			delete(newHelper);
		}

		TEST_METHOD(AddRemoveHelpers)
		{
			const std::string jsonStr =
				R"({
					"Health": 100,
					"DPS": 1.5,
					"Aliases": ["A", "B", "C"],
					"Stunned": false
				})";

			JsonFoo foo;
			FooSharedData sharedDat(foo);
			JsonParseMaster master(sharedDat);
			FooJsonParseHelper helper;
			master.AddHelper(helper);
			Assert::IsTrue(master.Parse(jsonStr));

			master.RemoveHelper(helper);
			Assert::IsFalse(master.Parse(jsonStr));
		}

		TEST_METHOD(ParseString)
		{
			const std::string jsonStr =
				R"({
					"Health": 100,
					"DPS": 1.5,
					"Aliases": ["A", "B", "C"],
					"Stunned": false
				})";

			JsonFoo foo;
			FooSharedData sharedDat(foo);
			JsonParseMaster master(sharedDat);
			FooJsonParseHelper helper;
			master.AddHelper(helper);
			Assert::IsTrue(master.Parse(jsonStr));

			Assert::AreEqual(foo.mHealth, 100);
			Assert::AreEqual(foo.mDPS, 1.5f);
			Assert::AreEqual(foo.bStunned, false);
			Assert::AreEqual(foo.mAliases[0], "A"s);
			Assert::AreEqual(foo.mAliases[1], "B"s);
			Assert::AreEqual(foo.mAliases[2], "C"s);

			//invalid string test
			const std::string invalid = "";
			Assert::IsFalse(master.Parse(invalid));
		}

		TEST_METHOD(ParseStream)
		{
			const std::string jsonStr =
				R"({
					"Health": 100,
					"DPS": 1.5,
					"Aliases": ["A", "B", "C"],
					"Stunned": false
				})";

			std::istringstream jsonStream(jsonStr);
			JsonFoo foo;
			FooSharedData sharedDat(foo);
			JsonParseMaster master(sharedDat);
			FooJsonParseHelper helper;
			master.AddHelper(helper);
			Assert::IsTrue(master.Parse(jsonStream));

			Assert::AreEqual(foo.mHealth, 100);
			Assert::AreEqual(foo.mDPS, 1.5f);
			Assert::AreEqual(foo.bStunned, false);
			Assert::AreEqual(foo.mAliases[0], "A"s);
			Assert::AreEqual(foo.mAliases[1], "B"s);
			Assert::AreEqual(foo.mAliases[2], "C"s);

			//invalid string test
			const std::string invalid = "";
			std::istringstream invStream(invalid);
			Assert::IsFalse(master.Parse(invStream));
		}

		TEST_METHOD(ParseFile)
		{
			const std::string jsonStr =
				R"({
					"Health": 100,
					"DPS": 1.5,
					"Aliases": ["A", "B", "C"],
					"Stunned": false
				})";

			//create/write to file
			ofstream file;
			file.open("temp.txt");
			file << jsonStr;
			file.close();

			//attempt to parse from file
			//ifstream jsonStream("temp.txt");
			JsonFoo foo;
			FooSharedData sharedDat(foo);
			JsonParseMaster master(sharedDat);
			FooJsonParseHelper helper;
			master.AddHelper(helper);
			Assert::IsTrue(master.ParseFromFile("temp.txt"s));

			Assert::AreEqual(foo.mHealth, 100);
			Assert::AreEqual(foo.mDPS, 1.5f);
			Assert::AreEqual(foo.bStunned, false);
			Assert::AreEqual(foo.mAliases[0], "A"s);
			Assert::AreEqual(foo.mAliases[1], "B"s);
			Assert::AreEqual(foo.mAliases[2], "C"s);

			Assert::AreEqual(master.GetFileName(), "temp.txt"s);

			//delete file
			remove("temp.txt");
		}

		TEST_METHOD(GetSetSharedData)
		{
			//get
			JsonFoo foo;
			FooSharedData sharedDat(foo);
			JsonParseMaster master(sharedDat);
			Assert::AreEqual(master.GetSharedData(), sharedDat.As<JsonParseMaster::SharedData>());

			//set
			JsonFoo foo2;
			FooSharedData sharedDat2(foo2);
			master.SetSharedData(sharedDat2);
			Assert::AreEqual(master.GetSharedData(), sharedDat2.As<JsonParseMaster::SharedData>());
			Assert::AreNotEqual(master.GetSharedData(), sharedDat.As<JsonParseMaster::SharedData>());

			//const get
			const JsonParseMaster constMaster(sharedDat);
			Assert::AreEqual(constMaster.GetSharedData(), sharedDat.As<const JsonParseMaster::SharedData>());
		}

		TEST_METHOD(Clone)
		{
			const std::string jsonStr =
				R"({
					"Health": 100,
					"DPS": 1.5,
					"Aliases": ["A", "B", "C"],
					"Stunned": false
				})";

			JsonFoo foo;
			FooSharedData sharedDat(foo);
			JsonParseMaster master(sharedDat);
			FooJsonParseHelper helper;
			master.AddHelper(helper);
			Assert::IsTrue(master.Parse(jsonStr));

			auto masterClone = master.Clone();
			Assert::AreNotEqual(master.GetSharedData(), masterClone->GetSharedData());
			Assert::IsTrue(masterClone->Parse(jsonStr));

			delete(masterClone);
		}

		TEST_METHOD(MoveSemantics)
		{
			const std::string jsonStr =
				R"({
					"Health": 100,
					"DPS": 1.5,
					"Aliases": ["A", "B", "C"],
					"Stunned": false
				})";

			JsonFoo foo;
			FooSharedData sharedDat(foo);
			JsonParseMaster master(sharedDat);
			FooJsonParseHelper helper;
			master.AddHelper(helper);
			Assert::IsTrue(master.Parse(jsonStr));

			//move constructor
			JsonParseMaster movedMaster(std::move(master));
			Assert::AreEqual(movedMaster.GetSharedData(), sharedDat.As<JsonParseMaster::SharedData>());
			Assert::AreEqual(sharedDat.GetJsonParseMaster(), &movedMaster);
			Assert::IsTrue(movedMaster.Parse(jsonStr));

			//move into a clone (move operator)
			auto masterClone = movedMaster.Clone();
			JsonParseMaster newMaster(sharedDat); //technically bad because shared data shouldn't be shared between masters but ok for this
			masterClone;
			newMaster;
			*masterClone = std::move(newMaster);
			Assert::AreEqual(masterClone->GetSharedData(), sharedDat.As<JsonParseMaster::SharedData>());

			delete(masterClone);
		}

		/************************************************************************/
		/**************************Table Parse Tests*****************************/
		/************************************************************************/

		TEST_METHOD(RTTI)
		{

		}

		TEST_METHOD(NoArrayOrNestedScopes)
		{
			Scope scope;
			TableSharedData sharedData(scope);
			JsonParseMaster master(sharedData);
			JsonTableParseHelper helper;
			master.AddHelper(helper);
			master.Initialize();
			Assert::IsTrue(master.ParseFromFile("Content\\NoArrayOrNestedScope.json"));
			Assert::IsTrue(scope["ExternalInteger"] == 23);
			Assert::IsTrue(scope["ExternalFloat"] == 5.0f);
			Assert::IsTrue(scope["ExternalVector"] == glm::vec4(5.0f));
			Assert::IsTrue(scope["ExternalMatrix"] == glm::mat4(5.0f));
			Assert::IsTrue(scope["ExternalString"] == "Meow");
		}

		TEST_METHOD(ArraysNoNestedScopes)
		{
			Scope scope;
			TableSharedData sharedData(scope);
			JsonParseMaster master(sharedData);
			JsonTableParseHelper helper;
			master.AddHelper(helper);
			master.Initialize();
			//primitives
			Assert::IsTrue(master.ParseFromFile("Content\\ArraysIncluded.json"));
			Assert::IsTrue(scope["ExternalInteger"] == 23);
			Assert::IsTrue(scope["ExternalFloat"] == 5.0f);
			Assert::IsTrue(scope["ExternalVector"] == glm::vec4(5.0f));
			Assert::IsTrue(scope["ExternalMatrix"] == glm::mat4(5.0f));
			Assert::IsTrue(scope["ExternalString"] == "Meow");

			//Arrays
			Assert::AreEqual(scope["ExternalIntArr"].Get<int>(0), 0);
			Assert::AreEqual(scope["ExternalIntArr"].Get<int>(1), 1);
			Assert::AreEqual(scope["ExternalIntArr"].Get<int>(2), 2);
			Assert::AreEqual(scope["ExternalFloatArr"].Get<float>(0), 0.0f);
			Assert::AreEqual(scope["ExternalFloatArr"].Get<float>(1), 1.1f);
			Assert::AreEqual(scope["ExternalFloatArr"].Get<float>(2), 2.2f);
			Assert::AreEqual(scope["ExternalVecArr"].Get<glm::vec4>(0), glm::vec4(0));
			Assert::AreEqual(scope["ExternalVecArr"].Get<glm::vec4>(1), glm::vec4(1));
			Assert::AreEqual(scope["ExternalVecArr"].Get<glm::vec4>(2), glm::vec4(2));
			Assert::AreEqual(scope["ExternalMatArr"].Get<glm::mat4>(0), glm::mat4(0));
			Assert::AreEqual(scope["ExternalMatArr"].Get<glm::mat4>(1), glm::mat4(1));
			Assert::AreEqual(scope["ExternalMatArr"].Get<glm::mat4>(2), glm::mat4(2));
			Assert::AreEqual(scope["ExternalStringArr"].Get<std::string>(0), "String0"s);
			Assert::AreEqual(scope["ExternalStringArr"].Get<std::string>(1), "String1"s);
			Assert::AreEqual(scope["ExternalStringArr"].Get<std::string>(2), "String2"s);
		}

		TEST_METHOD(NestedScopes)
		{
			Scope scope;
			TableSharedData sharedData(scope);
			JsonParseMaster master(sharedData);
			JsonTableParseHelper helper;
			master.AddHelper(helper);
			master.Initialize();

			Assert::IsTrue(master.ParseFromFile("Content\\NestedScope.json"));

			//sanity check that it still parses arrays and primitives fine
			Assert::IsTrue(scope["ExternalInteger"] == 23);
			Assert::AreEqual(scope["ExternalIntArr"].Get<int>(0), 0);
			Assert::AreEqual(scope["ExternalIntArr"].Get<int>(1), 1);
			Assert::AreEqual(scope["ExternalIntArr"].Get<int>(2), 2);

			//checking nested scopes
			auto nestedScopeDat = scope.Find("NestedScope");
			Assert::IsNotNull(nestedScopeDat);
			Scope& nestedScope = nestedScopeDat->Get<Scope>();
			Assert::IsTrue(nestedScope["Name"] == "Bob");
			Assert::IsTrue(nestedScope["Health"] == 100);

			//nested scope inside nested scope
			auto nestedScopeDat2 = nestedScope.Find("NestedScope2");
			Assert::IsNotNull(nestedScopeDat2);
			Scope& nestedScope2 = nestedScopeDat2->Get<Scope>();
			Assert::IsTrue(nestedScope2["DPS"] == 22.3f);
		}

		TEST_METHOD(NestedScopeArray)
		{
			Scope scope;
			TableSharedData sharedData(scope);
			JsonParseMaster master(sharedData);
			JsonTableParseHelper helper;
			master.AddHelper(helper);
			master.Initialize();

			Assert::IsTrue(master.ParseFromFile("Content\\NestedScopeArray.json"));
			auto nestedScopeDat = scope.Find("NestedScopeArr");
			Assert::IsNotNull(nestedScopeDat);
			Assert::AreEqual(nestedScopeDat->Size(), 3_z);
			Scope& nestedScope = nestedScopeDat->Get<Scope>(0);
			Assert::IsTrue(nestedScope["Name"] == "Table1");
			Assert::AreEqual(nestedScope.Size(), 1_z);

			auto nestedScopeDat2 = nestedScope.Find("NestedScopeArr");
			Assert::IsNull(nestedScopeDat2);
			
			Scope& nestedScope2 = nestedScopeDat->Get<Scope>(1);
			Assert::IsTrue(nestedScope2["Name"] == "Table2");
			Scope& nestedScope3 = nestedScopeDat->Get<Scope>(2);
			Assert::IsTrue(nestedScope3["Name"] == "Bob");
			Assert::IsTrue(nestedScope3["Health"] == 100);

			//nested scope inside nested scope
			auto nestedScopeDat3 = nestedScope3.Find("NestedScope2");
			Assert::IsNotNull(nestedScopeDat3);
			Scope& nestedScope4 = nestedScopeDat3->Get<Scope>();
			Assert::IsTrue(nestedScope4["DPS"] == 22.3f);
		}

	private:
		static _CrtMemState sStartMemState;	//for memory leak detection
	};
	_CrtMemState ParsingTests::sStartMemState;
}