#include "pch.h"
#include "CppUnitTest.h"
#include "DefaultEquality.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;
using namespace std;
using namespace std::string_literals;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(DefaultEqualityTest)
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

		TEST_METHOD(CharStarEquality)
		{
			char* a = "dog";
			char* b = "cat";
			char c[4];
			strcpy_s(c, a);

			DefaultEquality<char*> equalFunc;
			Assert::IsTrue(equalFunc(a, a));
			Assert::IsFalse(equalFunc(a, b));
			Assert::IsFalse(equalFunc(c, b));
			Assert::IsTrue(equalFunc(a, c));
		}

	private:
		static _CrtMemState sStartMemState;	//for memory leak detection
	};
	_CrtMemState DefaultEqualityTest::sStartMemState;
}