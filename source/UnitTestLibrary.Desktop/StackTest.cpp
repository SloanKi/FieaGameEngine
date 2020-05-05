#include "pch.h"
#include "CppUnitTest.h"
#include "Stack.h"
#include "Foo.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;
using namespace UnitTests;
using namespace std;
using namespace std::string_literals;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(StackTests)
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

		TEST_METHOD(QuickTest)
		{
			Stack<Foo> stack;
			Assert::IsTrue(stack.IsEmpty());

			stack.Push(Foo(1));
			stack.Push(Foo(2));
			stack.Push(Foo(3));

			Assert::AreEqual(stack.Size(), 3_z);
			Assert::AreEqual(stack.Top(), Foo(3));
			stack.Pop();

			Assert::AreEqual(stack.Size(), 2_z);
			Assert::AreEqual(stack.Top(), Foo(2));
			stack.Pop();

			Assert::AreEqual(stack.Size(), 1_z);
			Assert::AreEqual(stack.Top(), Foo(1));
			stack.Pop();
			
			Assert::IsTrue(stack.IsEmpty());
		}

	private:
		static _CrtMemState sStartMemState;	//for memory leak detection
	};
	_CrtMemState StackTests::sStartMemState;
}