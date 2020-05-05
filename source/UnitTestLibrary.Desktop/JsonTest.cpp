#include "pch.h"
#include "CppUnitTest.h"
#include <json/json.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace std::string_literals;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(JsonTests)
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

		TEST_METHOD(ReadWriteString)
		{
			//reading from string
			{
				const std::string testStr = 
					R"({
						"Name": "Bob", 
						"Health": 100.0, "NumLimbs": 4
					})";
				JSONCPP_STRING err;
				Json::Value root;
				Json::CharReaderBuilder builder;
				std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
				bool parsed = reader->parse(testStr.c_str(), testStr.c_str() + testStr.length(), &root, &err);
				Assert::IsTrue(parsed);
				Assert::AreEqual(root["Name"].asString().c_str(), "Bob");
				Assert::AreEqual(root["Health"].asFloat(), 100.0f);
				Assert::AreEqual(root["NumLimbs"].asInt(), 4);

				//print from string for funsies
				Json::StreamWriterBuilder builder2;
				const std::string json = Json::writeString(builder2, root);
				Logger::WriteMessage("Test 1: \n");
				Logger::WriteMessage(json.c_str());
				Logger::WriteMessage("\n\n");
				Logger::WriteMessage("\n\n");


				//getting names (unrelated to this test)
				auto memberNames = root.getMemberNames();
				for (auto& name : memberNames)
				{
					Logger::WriteMessage(name.c_str());
					Logger::WriteMessage("\n");
				}
				Logger::WriteMessage("\n\n");
				for (auto& member : root)
				{
					Logger::WriteMessage(member.asString().c_str());
					Logger::WriteMessage("\n");
				}
				Logger::WriteMessage("\n\n");
			}

			//writing to string
			{
				Json::Value root;
				Json::Value data;
				root["Name"] = "Bob";
				root["Health"] = 100.0;
				root["NumLimbs"] = 4;
				data["Alias"] = "Robert";
				root["Alias"] = data;

				Json::StreamWriterBuilder builder;
				const std::string json = Json::writeString(builder, root);
				Logger::WriteMessage("Test 2: \n");
				Logger::WriteMessage(json.c_str());
				Logger::WriteMessage("\n\n");
			}
		}

		TEST_METHOD(ReadWriteStream)
		{
			//reading from stream
			{
				const std::string testStr = R"({"Name": "Bob", "Health": 100.0, "NumLimbs": 4})";
				std::istringstream testStream(testStr);
				JSONCPP_STRING err;
				Json::Value root;
				Json::CharReaderBuilder builder;
				bool parsed = parseFromStream(builder, testStream, &root, &err);
				Assert::IsTrue(parsed);
				Assert::AreEqual(root["Name"].asString().c_str(), "Bob");
				Assert::AreEqual(root["Health"].asFloat(), 100.0f);
				Assert::AreEqual(root["NumLimbs"].asInt(), 4);

				//print from stream for funsies
				Json::StreamWriterBuilder builder2;
				std::ostringstream outStream;
				const std::unique_ptr<Json::StreamWriter> writer(builder2.newStreamWriter());
				writer->write(root, &outStream);
				Logger::WriteMessage("Test 1: \n");
				Logger::WriteMessage(outStream.str().c_str());
				Logger::WriteMessage("\n\n");
			}

			//writing from stream
			{
				std::ostringstream testStream;
				Json::Value root;
				Json::Value data;
				root["Name"] = "Bob";
				root["Health"] = 100.0;
				root["NumLimbs"] = 4;
				data["Alias"] = "Robert";
				root["Alias"] = data;

				Json::StreamWriterBuilder builder;
				const std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
				writer->write(root, &testStream);
				Logger::WriteMessage("Test 2: \n");
				Logger::WriteMessage(testStream.str().c_str());
				Logger::WriteMessage("\n\n");
			}
		}


		TEST_METHOD(TypeChecking)
		{
			const std::string jsonStr =
				R"({
					"Integer": 100,
					"Float": 2.321,
					"Boolean": true,
					"String": "This is a string",
					"IntArr": [1, 2, 3, 4],
					"StrArr": ["A", "B", "C"]
				})";

			//parse from string
			JSONCPP_STRING err;
			Json::Value root;
			Json::CharReaderBuilder builder;
			std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
			bool parsed = reader->parse(jsonStr.c_str(), jsonStr.c_str() + jsonStr.length(), &root, &err);
			Logger::WriteMessage(err.c_str());
			Assert::IsTrue(parsed);


			//type checking
			Assert::AreEqual(root["Integer"].asInt(), 100);
			Assert::AreEqual(root["Float"].asFloat(), 2.321f);
			Assert::AreEqual(root["Boolean"].asBool(), true);
			Assert::AreEqual(root["String"].asCString(), "This is a string");
			Assert::IsTrue(root["IntArr"].isArray());

			//reading from array
			Assert::AreEqual((int)root["IntArr"].size(), 4);
			Assert::AreEqual(root["IntArr"][0].asInt(), 1);
			Assert::AreEqual(root["IntArr"][1].asInt(), 2);
			Assert::AreEqual(root["IntArr"][2].asInt(), 3);
			Assert::AreEqual(root["IntArr"][3].asInt(), 4);

			//reading from strArr
			Assert::IsTrue(root["StrArr"].isArray());
			Assert::AreEqual(root["StrArr"][0].asString(), "A"s);
			auto& strArr = root["StrArr"];
			Assert::AreEqual(strArr[0].asString(), "A"s);


			//printing for funsies
			Json::StreamWriterBuilder builder2;
			const std::string json = Json::writeString(builder2, root);
			Logger::WriteMessage("Test 1: \n");
			Logger::WriteMessage(json.c_str());
			Logger::WriteMessage("\n\n");
		}

	private:
		static _CrtMemState sStartMemState;	//for memory leak detection
	};
	_CrtMemState JsonTests::sStartMemState;
}