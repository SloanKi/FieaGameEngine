#include "pch.h"
#include "CppUnitTest.h"
#include "DefaultHash.h"
#include "Foo.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;
using namespace UnitTests;
using namespace std;
using namespace std::string_literals;

namespace Library
{
	template<>
	struct DefaultHash<Foo>
	{
		//couldn't do with a const Foo because then key.Data is an r value and can't reinterpret cast it
		size_t operator()(Foo& key)
		{
			const uint8_t* data = reinterpret_cast<const uint8_t*>(&key.Data());
			return AdditiveHash(data, sizeof(key.Data()));
		}
	};
}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(DefaultHashTest)
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

		TEST_METHOD(IntegerHash)
		{
			int a = 10;
			int b = 10;
			int c = 20;
			DefaultHash<int> hashFunc;

			Assert::AreEqual(hashFunc(a), hashFunc(b));
			Assert::AreNotEqual(hashFunc(a), hashFunc(c));
			Assert::AreNotEqual(hashFunc(b), hashFunc(c));
		}

		TEST_METHOD(StringHash)
		{
			string a = "hello"s;
			string b = "goodbye"s;
			string c(a);
			DefaultHash<string> hashFunc;

			Assert::AreEqual(hashFunc(a), hashFunc(c));
			Assert::AreNotEqual(hashFunc(a), hashFunc(b));
			Assert::AreNotEqual(hashFunc(b), hashFunc(c));
		}

		TEST_METHOD(CharStarHash)
		{
			char* a = "hello";
			char* b = "goodbye";
			char c[6];
			strcpy_s(c, a);
			DefaultHash<char*> hashFunc;

			Assert::AreEqual(hashFunc(a), hashFunc(c));
			Assert::AreNotEqual(hashFunc(a), hashFunc(b));
			Assert::AreNotEqual(hashFunc(b), hashFunc(c));
		}

		TEST_METHOD(FooHash)
		{
			Foo a(5), b(10), c(5);
			DefaultHash<Foo> hashFunc;

			Assert::AreEqual(hashFunc(a), hashFunc(c));
			Assert::AreNotEqual(hashFunc(a), hashFunc(b));
			Assert::AreNotEqual(hashFunc(b), hashFunc(c));
		}


	private:
		static _CrtMemState sStartMemState;	//for memory leak detection
	};
	_CrtMemState DefaultHashTest::sStartMemState;
}