#include "pch.h"
#include "CppUnitTest.h"
#include "TypeRegistry.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;
//using namespace UnitTests;
using namespace std;
using namespace std::string_literals;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	inline std::wstring ToString<Library::Vector<Signature>>(const Library::Vector<Signature>& t)
	{
		RETURN_WIDE_STRING(&t);
	}

	template<>
	inline std::wstring ToString<Library::Signature>(const Library::Signature& t)
	{
		RETURN_WIDE_STRING(&t);
	}
}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(TypeRegistryTests)
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
	
		TEST_METHOD(SignatureEquality)
		{
			//for my sanity
			Signature sig1 = Signature("Health"s, DatumType::Integer, 1, 0);
			Signature sig2 = sig1;
			Assert::AreEqual(sig1, sig2);

			sig2 = Signature("Name"s, DatumType::String, 1, 8);
			Assert::IsTrue(sig1 != sig2);
		}

		TEST_METHOD(CreateShutdown)
		{
			TypeRegistry::Create();
			Assert::AreEqual(TypeRegistry::Size(), 0_z);
			Assert::ExpectException<std::runtime_error>([] { TypeRegistry::Create(); });
			TypeRegistry::Shutdown();
			TypeRegistry::Create();
			TypeRegistry::Shutdown();
		}

		TEST_METHOD(RegisterGetDeregister)
		{
			TypeRegistry::Create();
			//register
			Vector<Signature> signatures;
			signatures.PushBack(Signature("Health"s, DatumType::Integer, 1, 0));
			signatures.PushBack(Signature("Name"s, DatumType::String, 1, 8));
			TypeRegistry::RegisterType(0, signatures);
			Assert::AreEqual(TypeRegistry::Size(), 1_z);
			//no duplicate register
			TypeRegistry::RegisterType(0, signatures);
			Assert::AreEqual(TypeRegistry::Size(), 1_z);

			//get (exists)
			Assert::AreEqual(signatures, TypeRegistry::GetSignatures(0));

			//get (does not exist)
			Assert::ExpectException<std::runtime_error>([] { TypeRegistry::GetSignatures(1); });

			//deregister (exist)
			TypeRegistry::DeregisterType(0);
			Assert::AreEqual(TypeRegistry::Size(), 0_z);
			Assert::ExpectException<std::runtime_error>([] { TypeRegistry::GetSignatures(0); });

			//deregister (does not exist) (nothing happens)
			TypeRegistry::DeregisterType(0);
			Assert::AreEqual(TypeRegistry::Size(), 0_z);

			TypeRegistry::Shutdown();
		}

	private:
		static _CrtMemState sStartMemState;	//for memory leak detection
	};
	_CrtMemState TypeRegistryTests::sStartMemState;
}