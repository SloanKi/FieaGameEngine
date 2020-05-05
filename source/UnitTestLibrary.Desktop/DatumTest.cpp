#include "pch.h"
#include "CppUnitTest.h"
#include "Datum.h"
#include "Foo.h"
#include "Scope.h"

#pragma warning(push)
#pragma warning(disable:4201)
#include <glm/gtx/string_cast.hpp>
#pragma warning(pop)

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
	inline std::wstring ToString<glm::vec4>(const glm::vec4& t)
	{
		RETURN_WIDE_STRING(&t);
	}

	template<>
	inline std::wstring ToString<glm::mat4>(const glm::mat4& t)
	{
		RETURN_WIDE_STRING(&t);
	}

	template<>
	inline std::wstring ToString<Library::RTTI>(Library::RTTI* t)
	{
		RETURN_WIDE_STRING(t);
	}
	
	template<>
	inline std::wstring ToString<Library::Datum>(const Library::Datum& t)
	{
		RETURN_WIDE_STRING(&t);
	}

	template<>
	inline std::wstring ToString<Library::Scope>(const Library::Scope& t)
	{
		RETURN_WIDE_STRING(&t);
	}

	template<>
	inline std::wstring ToString<Library::Scope>(const Library::Scope* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<Library::Scope>(Library::Scope* t)
	{
		RETURN_WIDE_STRING(t);
	}
}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(DatumTests)
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

		TEST_METHOD(Constructor)
		{
			Datum datum(DatumType::Matrix);
			Assert::AreEqual(datum.Size(), 0_z);
			Assert::AreEqual(datum.Capacity(), 0_z);
			Assert::AreEqual(datum.Type(), DatumType::Matrix);

			Datum datum2(DatumType::Integer, 5);
			Assert::AreEqual(datum2.Size(), 0_z);
			Assert::AreEqual(datum2.Capacity(), 5_z);
			Assert::AreEqual(datum2.Type(), DatumType::Integer);
		}

		TEST_METHOD(TypeSizeCapacity)
		{
			Datum datum;
			Assert::AreEqual(datum.Size(), 0_z);
			Assert::AreEqual(datum.Capacity(), 0_z);
			Assert::AreEqual(datum.Type(), DatumType::Unknown);
			Assert::IsTrue(datum.IsEmpty());
		}

		TEST_METHOD(SetType)
		{
			Datum datum;
			Assert::AreEqual(datum.Type(), DatumType::Unknown);

			//test 1: setting to unknown (valid but means this datum still isn't assigned a type)
			datum.SetType(DatumType::Unknown);
			Assert::AreEqual(datum.Type(), DatumType::Unknown);

			//test 2: set type on datum without type
			datum.SetType(DatumType::Float);
			Assert::AreEqual(datum.Type(), DatumType::Float);

			//test 3: set type on datum already assigned a type (throw exception)
			Assert::ExpectException<std::runtime_error>([&datum] { datum.SetType(DatumType::Integer); });
		}

		TEST_METHOD(Resize)
		{
			//test 1: datum without type (throw exception)
			Datum invDatum;
			Assert::ExpectException<std::runtime_error>([&invDatum] { invDatum.Resize(5); });

			//Integer Datum Tests
			{
				//test 2: grow without default construct
				Datum datum(DatumType::Integer);
				datum.Resize(2);
				Assert::AreEqual(datum.Capacity(), 2_z);
				Assert::AreEqual(datum.Size(), 0_z);

				//test 3: grow with default construct
				datum.Resize(3, true);
				Assert::AreEqual(datum.Capacity(), 3_z);
				Assert::AreEqual(datum.Size(), 3_z);
				Assert::AreEqual(datum.Get<int>(0), 0);
				Assert::AreEqual(datum.Get<int>(1), 0);
				Assert::AreEqual(datum.Get<int>(2), 0);

				//test 4: shrink
				datum.Resize(2);
				Assert::AreEqual(datum.Capacity(), 2_z);
				Assert::AreEqual(datum.Size(), 2_z);
				Assert::AreEqual(datum.Get<int>(0), 0);
				Assert::AreEqual(datum.Get<int>(1), 0);

				//test 5: capacity = 0
				datum.Resize(0);
				Assert::AreEqual(datum.Capacity(), 0_z);
				Assert::AreEqual(datum.Size(), 0_z);
			}


			//Float Datum Tests
			{
				//test 2: grow without default construct
				Datum datum(DatumType::Float);
				datum.Resize(2);
				Assert::AreEqual(datum.Capacity(), 2_z);
				Assert::AreEqual(datum.Size(), 0_z);

				//test 3: grow with default construct
				datum.Resize(3, true);
				Assert::AreEqual(datum.Capacity(), 3_z);
				Assert::AreEqual(datum.Size(), 3_z);
				Assert::AreEqual(datum.Get<float>(0), 0.0f);
				Assert::AreEqual(datum.Get<float>(1), 0.0f);
				Assert::AreEqual(datum.Get<float>(2), 0.0f);

				//test 4: shrink
				datum.Resize(2);
				Assert::AreEqual(datum.Capacity(), 2_z);
				Assert::AreEqual(datum.Size(), 2_z);
				Assert::AreEqual(datum.Get<float>(0), 0.0f);
				Assert::AreEqual(datum.Get<float>(1), 0.0f);

				//test 5: capacity = 0
				datum.Resize(0);
				Assert::AreEqual(datum.Capacity(), 0_z);
				Assert::AreEqual(datum.Size(), 0_z);
			}

			//Vector Datum Tests
			{
				//test 2: grow without default construct
				Datum datum(DatumType::Vector);
				datum.Resize(2);
				Assert::AreEqual(datum.Capacity(), 2_z);
				Assert::AreEqual(datum.Size(), 0_z);

				//test 3: grow with default construct
				datum.Resize(3, true);
				Assert::AreEqual(datum.Capacity(), 3_z);
				Assert::AreEqual(datum.Size(), 3_z);
				Assert::AreEqual(datum.Get<glm::vec4>(0), glm::vec4(0.0f));
				Assert::AreEqual(datum.Get<glm::vec4>(1), glm::vec4(0.0f));
				Assert::AreEqual(datum.Get<glm::vec4>(2), glm::vec4(0.0f));

				//test 4: shrink
				datum.Resize(2);
				Assert::AreEqual(datum.Capacity(), 2_z);
				Assert::AreEqual(datum.Size(), 2_z);
				Assert::AreEqual(datum.Get<glm::vec4>(0), glm::vec4(0.0f));
				Assert::AreEqual(datum.Get<glm::vec4>(1), glm::vec4(0.0f));

				//test 5: capacity = 0
				datum.Resize(0);
				Assert::AreEqual(datum.Capacity(), 0_z);
				Assert::AreEqual(datum.Size(), 0_z);
			}
			
			//Matrix Datum Tests
			{
				//test 2: grow without default construct
				Datum datum(DatumType::Matrix);
				datum.Resize(2);
				Assert::AreEqual(datum.Capacity(), 2_z);
				Assert::AreEqual(datum.Size(), 0_z);

				//test 3: grow with default construct
				datum.Resize(3, true);
				Assert::AreEqual(datum.Capacity(), 3_z);
				Assert::AreEqual(datum.Size(), 3_z);
				Assert::AreEqual(datum.Get<glm::mat4>(0), glm::mat4(0.0f));
				Assert::AreEqual(datum.Get<glm::mat4>(1), glm::mat4(0.0f));
				Assert::AreEqual(datum.Get<glm::mat4>(2), glm::mat4(0.0f));

				//test 4: shrink
				datum.Resize(2);
				Assert::AreEqual(datum.Capacity(), 2_z);
				Assert::AreEqual(datum.Size(), 2_z);
				Assert::AreEqual(datum.Get<glm::mat4>(0), glm::mat4(0.0f));
				Assert::AreEqual(datum.Get<glm::mat4>(1), glm::mat4(0.0f));

				//test 5: capacity = 0
				datum.Resize(0);
				Assert::AreEqual(datum.Capacity(), 0_z);
				Assert::AreEqual(datum.Size(), 0_z);
			}

			//String Datum Tests
			{
				//test 2: grow without default construct
				Datum datum(DatumType::String);
				datum.Resize(2);
				Assert::AreEqual(datum.Capacity(), 2_z);
				Assert::AreEqual(datum.Size(), 0_z);

				//test 3: grow with default construct
				datum.Resize(3, true);
				Assert::AreEqual(datum.Capacity(), 3_z);
				Assert::AreEqual(datum.Size(), 3_z);
				Assert::AreEqual(datum.Get<std::string>(0), std::string());
				Assert::AreEqual(datum.Get<std::string>(1), std::string());
				Assert::AreEqual(datum.Get<std::string>(2), std::string());

				//test 4: shrink
				datum.Resize(2);
				Assert::AreEqual(datum.Capacity(), 2_z);
				Assert::AreEqual(datum.Size(), 2_z);
				Assert::AreEqual(datum.Get<std::string>(0), std::string());
				Assert::AreEqual(datum.Get<std::string>(1), std::string());

				//test 5: capacity = 0
				datum.Resize(0);
				Assert::AreEqual(datum.Capacity(), 0_z);
				Assert::AreEqual(datum.Size(), 0_z);
			}

			//RTTI* Datum Tests
			{
				//test 2: grow without default construct
				Datum datum(DatumType::Pointer);
				datum.Resize(2);
				Assert::AreEqual(datum.Capacity(), 2_z);
				Assert::AreEqual(datum.Size(), 0_z);

				//test 3: grow with default construct
				datum.Resize(3, true);
				Assert::AreEqual(datum.Capacity(), 3_z);
				Assert::AreEqual(datum.Size(), 3_z);
				//RTTI* test = datum.Get<RTTI*>();
				//test;
				Assert::AreEqual(datum.Get<RTTI*>(0), reinterpret_cast<RTTI*>(NULL));
				Assert::AreEqual(datum.Get<RTTI*>(1), reinterpret_cast<RTTI*>(NULL));
				Assert::AreEqual(datum.Get<RTTI*>(2), reinterpret_cast<RTTI*>(NULL));

				//test 4: shrink
				datum.Resize(2);
				Assert::AreEqual(datum.Capacity(), 2_z);
				Assert::AreEqual(datum.Size(), 2_z);
				Assert::AreEqual(datum.Get<RTTI*>(0), reinterpret_cast<RTTI*>(NULL));
				Assert::AreEqual(datum.Get<RTTI*>(1), reinterpret_cast<RTTI*>(NULL));

				//test 5: capacity = 0
				datum.Resize(0);
				Assert::AreEqual(datum.Capacity(), 0_z);
				Assert::AreEqual(datum.Size(), 0_z);
			}

			//test 6: external (throw exception)
			Datum extDatum;
			int test1(5), test2(10), test3(15);
			int arr[3] = { test1, test2, test3 };
			extDatum.SetStorage(arr, 3);
			Assert::ExpectException<std::runtime_error>([&extDatum, &arr] {extDatum.Resize(5); });

			//test 7: resize on scope* with default construct (throw exception
			{
				Datum datum(DatumType::Table);
				Assert::ExpectException<std::runtime_error>([&datum] {datum.Resize(5, true); });
			}
		}

		TEST_METHOD(Reserve)
		{
			//test 1: datum without type (throw exception)
			Datum datum;
			Assert::ExpectException<std::runtime_error>([&datum] { datum.Reserve(5); });

			//test 2: valid reserve
			datum.SetType(DatumType::Float);
			datum.Reserve(5);
			Assert::AreEqual(datum.Capacity(), 5_z);
			Assert::AreEqual(datum.Size(), 0_z);

			//test 3: try to shrink (nothing happens)
			datum.Reserve(1);
			Assert::AreEqual(datum.Capacity(), 5_z);
			Assert::AreEqual(datum.Size(), 0_z);

			//test 4: external (throw exception)
			Datum extDatum;
			int test1(5), test2(10), test3(15);
			int arr[3] = { test1, test2, test3 };
			extDatum.SetStorage(arr, 3);
			Assert::ExpectException<std::runtime_error>([&extDatum, &arr] {extDatum.Resize(5); });
		}

		TEST_METHOD(Clear)
		{
			//int datum
			{
				Datum datum(DatumType::Integer);
				datum.Resize(5, true);	//default construct items
				Assert::AreEqual(datum.Size(), 5_z);
				Assert::AreEqual(datum.Size(), 5_z);
				datum.Clear();
				Assert::AreEqual(datum.Size(), 0_z);
				Assert::AreEqual(datum.Size(), 0_z);
			}

			//float datum
			{
				Datum datum(DatumType::Float);
				datum.Resize(5, true);	//default construct items
				Assert::AreEqual(datum.Size(), 5_z);
				Assert::AreEqual(datum.Size(), 5_z);
				datum.Clear();
				Assert::AreEqual(datum.Size(), 0_z);
				Assert::AreEqual(datum.Size(), 0_z);
			}

			//vector datum
			{
				Datum datum(DatumType::Vector);
				datum.Resize(5, true);	//default construct items
				Assert::AreEqual(datum.Size(), 5_z);
				Assert::AreEqual(datum.Size(), 5_z);
				datum.Clear();
				Assert::AreEqual(datum.Size(), 0_z);
				Assert::AreEqual(datum.Size(), 0_z);
			}

			//matrix datum
			{
				Datum datum(DatumType::Matrix);
				datum.Resize(5, true);	//default construct items
				Assert::AreEqual(datum.Size(), 5_z);
				Assert::AreEqual(datum.Size(), 5_z);
				datum.Clear();
				Assert::AreEqual(datum.Size(), 0_z);
				Assert::AreEqual(datum.Size(), 0_z);
			}

			//string datum
			{
				Datum datum(DatumType::String);
				datum.Resize(5, true);	//default construct items
				Assert::AreEqual(datum.Size(), 5_z);
				Assert::AreEqual(datum.Size(), 5_z);
				datum.Clear();
				Assert::AreEqual(datum.Size(), 0_z);
				Assert::AreEqual(datum.Size(), 0_z);
			}

			// pointer datum
			{
				Datum datum(DatumType::Pointer);
				datum.Resize(5, true);	//default construct items
				Assert::AreEqual(datum.Size(), 5_z);
				Assert::AreEqual(datum.Size(), 5_z);
				datum.Clear();
				Assert::AreEqual(datum.Size(), 0_z);
				Assert::AreEqual(datum.Size(), 0_z);
			}

			//test external (throw exception)
			Datum extDatum;
			int test1(5), test2(10), test3(15);
			int arr[3] = { test1, test2, test3 };
			extDatum.SetStorage(arr, 3);
			Assert::ExpectException<std::runtime_error>([&extDatum, &arr] {extDatum.Resize(5); });
		}

		TEST_METHOD(Copy)
		{
			//int datum tests
			{
				int test1(5), test2(10), test3(20);
				int arr[3] = { test1, test2, test3 };

				//test 1: copy datum with internal memory
				Datum datum1;
				datum1.PushBack(test1);
				datum1.PushBack(test2);
				datum1.PushBack(test3);
				Datum datum2(datum1);
				Assert::AreEqual(datum2, datum1);

				//test 2: datum with external memory
				Datum datum3;
				datum3.SetStorage(arr, 3);
				Datum datum4(datum3);
				Assert::AreEqual(datum3, datum4);
			}

			//float datum tests
			{
				float test1(5), test2(10), test3(20);
				float arr[3] = { test1, test2, test3 };

				//test 1: copy datum with internal memory
				Datum datum1;
				datum1.PushBack(test1);
				datum1.PushBack(test2);
				datum1.PushBack(test3);
				Datum datum2(datum1);
				Assert::AreEqual(datum2, datum1);

				//test 2: datum with external memory
				Datum datum3;
				datum3.SetStorage(arr, 3);
				Datum datum4(datum3);
				Assert::AreEqual(datum3, datum4);
			}

			//vector datum tests
			{
				glm::vec4 test1(5), test2(10), test3(20);
				glm::vec4 arr[3] = { test1, test2, test3 };

				//test 1: copy datum with internal memory
				Datum datum1;
				datum1.PushBack(test1);
				datum1.PushBack(test2);
				datum1.PushBack(test3);
				Datum datum2(datum1);
				Assert::AreEqual(datum2, datum1);

				//test 2: datum with external memory
				Datum datum3;
				datum3.SetStorage(arr, 3);
				Datum datum4(datum3);
				Assert::AreEqual(datum3, datum4);
			}

			//matrix datum tests
			{
				glm::mat4 test1(5), test2(10), test3(20);
				glm::mat4 arr[3] = { test1, test2, test3 };

				//test 1: copy datum with internal memory
				Datum datum1;
				datum1.PushBack(test1);
				datum1.PushBack(test2);
				datum1.PushBack(test3);
				Datum datum2(datum1);
				Assert::AreEqual(datum2, datum1);

				//test 2: datum with external memory
				Datum datum3;
				datum3.SetStorage(arr, 3);
				Datum datum4(datum3);
				Assert::AreEqual(datum3, datum4);
			}

			//string datum tests
			{
				std::string test1("hello"), test2("Goodbye"), test3("ugh");
				std::string arr[3] = { test1, test2, test3 };

				//test 1: copy datum with internal memory
				Datum datum1;
				datum1.PushBack(test1);
				datum1.PushBack(test2);
				datum1.PushBack(test3);
				Datum datum2(datum1);
				Assert::AreEqual(datum2, datum1);

				//test 2: datum with external memory
				Datum datum3;
				datum3.SetStorage(arr, 3);
				Datum datum4(datum3);
				Assert::AreEqual(datum3, datum4);
			}

			//RTTI datum tests
			{
				Foo test1(5), test2(10), test3(20);
				RTTI* arr[3] = { &test1, &test2, &test3 };

				//test 1: copy datum with internal memory
				Datum datum1;
				datum1.PushBack(&test1);
				datum1.PushBack(&test2);
				datum1.PushBack(&test3);
				Datum datum2(datum1);
				Assert::AreEqual(datum2, datum1);

				//test 2: datum with external memory
				Datum datum3;
				datum3.SetStorage(arr, 3);
				Datum datum4(datum3);
				Assert::AreEqual(datum3, datum4);
			}

			//test 3: copy a default constructed datum (sanity check)
			Datum datum5;
			Datum datum6(datum5);
			Assert::AreEqual(datum5, datum6);
		}

		TEST_METHOD(MoveSemantics)
		{
			std::string test1("Hello"), test2("Goodbye"), test3("ugh");
			std::string arr[3] = { test1, test2, test3 };
			std::string arr2[4] = { test3, test2, test1, test1 };

			//test 1: rhs has internal memory (constructor)
			Datum datum1;
			datum1.PushBack(test1);
			datum1.PushBack(test2);
			datum1.PushBack(test3);
			Datum datum2(std::move(datum1));
			Assert::AreEqual(datum2.Size(), 3_z);
			Assert::AreEqual(datum2.Get<std::string>(), test1);
			Assert::AreEqual(datum1.Size(), 0_z); //make sure datum1 was invalidated

			//test 2: rhs has external memory (constructor)
			Datum datum3;
			datum3.SetStorage(arr, 3);
			Datum datum4(std::move(datum3));
			Assert::AreEqual(datum4.Size(), 3_z);
			Assert::AreEqual(datum4.Get<std::string>(), test1);
			Assert::AreEqual(datum3.Size(), 0_z); //make sure datum1 was invalidated

			//test 3: lhs has internal (assign)
			Datum datum2Cpy = datum2; //internal
			Datum datum4Cpy = datum4; //external
			datum2Cpy = std::move(datum4);
			Assert::AreEqual(datum2Cpy.Size(), 3_z);
			Assert::AreEqual(datum2Cpy.Get<std::string>(), test1);
			Assert::AreEqual(datum4.Size(), 0_z); //make sure datum1 was invalidated

			//test 4: lhs has external (assign)
			datum4Cpy = std::move(datum2);
			Assert::AreEqual(datum4Cpy.Size(), 3_z);
			Assert::AreEqual(datum4Cpy.Get<std::string>(), test1);
			Assert::AreEqual(datum2.Size(), 0_z); //make sure datum1 was invalidated

		}

		TEST_METHOD(AssignmentDatum)
		{
			//int datum tests
			{
				int test1(5), test2(10), test3(20);
				int arr[3] = { test1, test2, test3 };
				int arr2[4] = { test3, test2, test1, test1 };

				//test 1: default constructed datum = datum with internal memory
				Datum datum1, datum2;
				datum1.PushBack(test1);
				datum1.PushBack(test2);
				datum1.PushBack(test3);
				datum2 = datum1;
				Assert::AreEqual(datum2, datum1);

				//test 2: lhs internal rhs internal
				Datum emptyDatum;
				datum1 = emptyDatum;
				Assert::AreEqual(datum1, emptyDatum);

				//test 3: lhs internal rhs external
				Datum datum3;
				datum3.SetStorage(arr, 3);
				datum1 = datum2;	//give datum1 internal memory to make sure it doesn't leak 
				datum1 = datum3;
				Assert::AreEqual(datum1, datum3);

				//test 4: lhs external rhs internal
				datum1 = datum2;
				Assert::AreEqual(datum1, datum2);
				
				//test 5: lhs external rhs external
				Datum datum4;
				datum4.SetStorage(arr, 4);
				datum3 = datum4;
				Assert::AreEqual(datum3, datum4);

				//test 6: for sanity, lhs external rhs default constructed
				datum4 = emptyDatum;
				Assert::AreEqual(datum4, emptyDatum);
			}

			//float datum tests
			{
				float test1(5), test2(10), test3(20);
				float arr[3] = { test1, test2, test3 };
				float arr2[4] = { test3, test2, test1, test1 };

				//test 1: default constructed datum = datum with internal memory
				Datum datum1, datum2;
				datum1.PushBack(test1);
				datum1.PushBack(test2);
				datum1.PushBack(test3);
				datum2 = datum1;
				Assert::AreEqual(datum2, datum1);

				//test 2: lhs internal rhs internal
				Datum emptyDatum;
				datum1 = emptyDatum;
				Assert::AreEqual(datum1, emptyDatum);

				//test 3: lhs internal rhs external
				Datum datum3;
				datum3.SetStorage(arr, 3);
				datum1 = datum2;	//give datum1 internal memory to make sure it doesn't leak 
				datum1 = datum3;
				Assert::AreEqual(datum1, datum3);

				//test 4: lhs external rhs internal
				datum1 = datum2;
				Assert::AreEqual(datum1, datum2);

				//test 5: lhs external rhs external
				Datum datum4;
				datum4.SetStorage(arr, 4);
				datum3 = datum4;
				Assert::AreEqual(datum3, datum4);

				//test 6: for sanity, lhs external rhs default constructed
				datum4 = emptyDatum;
				Assert::AreEqual(datum4, emptyDatum);
			}

			//vector datum tests
			{
				glm::vec4 test1(5), test2(10), test3(20);
				glm::vec4 arr[3] = { test1, test2, test3 };
				glm::vec4 arr2[4] = { test3, test2, test1, test1 };

				//test 1: default constructed datum = datum with internal memory
				Datum datum1, datum2;
				datum1.PushBack(test1);
				datum1.PushBack(test2);
				datum1.PushBack(test3);
				datum2 = datum1;
				Assert::AreEqual(datum2, datum1);

				//test 2: lhs internal rhs internal
				Datum emptyDatum;
				datum1 = emptyDatum;
				Assert::AreEqual(datum1, emptyDatum);

				//test 3: lhs internal rhs external
				Datum datum3;
				datum3.SetStorage(arr, 3);
				datum1 = datum2;	//give datum1 internal memory to make sure it doesn't leak 
				datum1 = datum3;
				Assert::AreEqual(datum1, datum3);

				//test 4: lhs external rhs internal
				datum1 = datum2;
				Assert::AreEqual(datum1, datum2);

				//test 5: lhs external rhs external
				Datum datum4;
				datum4.SetStorage(arr, 4);
				datum3 = datum4;
				Assert::AreEqual(datum3, datum4);

				//test 6: for sanity, lhs external rhs default constructed
				datum4 = emptyDatum;
				Assert::AreEqual(datum4, emptyDatum);
			}

			//Matrix datum tests
			{
				glm::mat4 test1(5), test2(10), test3(20);
				glm::mat4 arr[3] = { test1, test2, test3 };
				glm::mat4 arr2[4] = { test3, test2, test1, test1 };

				//test 1: default constructed datum = datum with internal memory
				Datum datum1, datum2;
				datum1.PushBack(test1);
				datum1.PushBack(test2);
				datum1.PushBack(test3);
				datum2 = datum1;
				Assert::AreEqual(datum2, datum1);

				//test 2: lhs internal rhs internal
				Datum emptyDatum;
				datum1 = emptyDatum;
				Assert::AreEqual(datum1, emptyDatum);

				//test 3: lhs internal rhs external
				Datum datum3;
				datum3.SetStorage(arr, 3);
				datum1 = datum2;	//give datum1 internal memory to make sure it doesn't leak 
				datum1 = datum3;
				Assert::AreEqual(datum1, datum3);

				//test 4: lhs external rhs internal
				datum1 = datum2;
				Assert::AreEqual(datum1, datum2);

				//test 5: lhs external rhs external
				Datum datum4;
				datum4.SetStorage(arr, 4);
				datum3 = datum4;
				Assert::AreEqual(datum3, datum4);

				//test 6: for sanity, lhs external rhs default constructed
				datum4 = emptyDatum;
				Assert::AreEqual(datum4, emptyDatum);
			}

			//string datum tests
			{
				std::string test1("Hello"), test2("Goodbye"), test3("ugh");
				std::string arr[3] = { test1, test2, test3 };
				std::string arr2[4] = { test3, test2, test1, test1 };

				//test 1: default constructed datum = datum with internal memory
				Datum datum1, datum2;
				datum1.PushBack(test1);
				datum1.PushBack(test2);
				datum1.PushBack(test3);
				datum2 = datum1;
				Assert::AreEqual(datum2, datum1);

				//test 2: lhs internal rhs internal
				Datum emptyDatum;
				datum1 = emptyDatum;
				Assert::AreEqual(datum1, emptyDatum);

				//test 3: lhs internal rhs external
				Datum datum3;
				datum3.SetStorage(arr, 3);
				datum1 = datum2;	//give datum1 internal memory to make sure it doesn't leak 
				datum1 = datum3;
				Assert::AreEqual(datum1, datum3);

				//test 4: lhs external rhs internal
				datum1 = datum2;
				Assert::AreEqual(datum1, datum2);

				//test 5: lhs external rhs external
				Datum datum4;
				datum4.SetStorage(arr, 4);
				datum3 = datum4;
				Assert::AreEqual(datum3, datum4);

				//test 6: for sanity, lhs external rhs default constructed
				datum4 = emptyDatum;
				Assert::AreEqual(datum4, emptyDatum);
			}

			//pointer datum tests
			{
				Foo test1(5), test2(10), test3(20);
				RTTI* arr[3] = { &test1, &test2, &test3 };
				RTTI* arr2[4] = { &test3, &test2, &test1, &test1 };

				//test 1: default constructed datum = datum with internal memory
				Datum datum1, datum2;
				datum1.PushBack(&test1);
				datum1.PushBack(&test2);
				datum1.PushBack(&test3);
				datum2 = datum1;
				Assert::AreEqual(datum2, datum1);

				//test 2: lhs internal rhs internal
				Datum emptyDatum;
				datum1 = emptyDatum;
				Assert::AreEqual(datum1, emptyDatum);

				//test 3: lhs internal rhs external
				Datum datum3;
				datum3.SetStorage(arr, 3);
				datum1 = datum2;	//give datum1 internal memory to make sure it doesn't leak 
				datum1 = datum3;
				Assert::AreEqual(datum1, datum3);

				//test 4: lhs external rhs internal
				datum1 = datum2;
				Assert::AreEqual(datum1, datum2);

				//test 5: lhs external rhs external
				Datum datum4;
				datum4.SetStorage(arr, 4);
				datum3 = datum4;
				Assert::AreEqual(datum3, datum4);

				//test 6: for sanity, lhs external rhs default constructed
				datum4 = emptyDatum;
				Assert::AreEqual(datum4, emptyDatum);
			}
		}

		TEST_METHOD(AssignmentInt)
		{
			//test 1: undefined datum
			Datum datum;
			datum = 5;
			Assert::AreEqual(datum.Size(), 1_z);
			Assert::AreEqual(datum.Capacity(), 1_z);
			Assert::AreEqual(datum.Get<int>(), 5);
			Assert::AreEqual(datum.Type(), DatumType::Integer);

			//test 2: overwrite datum with 1 element
			datum = 22;
			Assert::AreEqual(datum.Size(), 1_z);
			Assert::AreEqual(datum.Capacity(), 1_z);
			Assert::AreEqual(datum.Get<int>(), 22);

			/*//test 3: overwrite datum with many elements
			datum.Resize(5, true);
			datum = 1;
			Assert::AreEqual(datum.Size(), 1_z);
			Assert::AreEqual(datum.Capacity(), 1_z);
			Assert::AreEqual(datum.Get<int>(), 1);*/

			//test 4: datum with wrong type (throw exception)
			Datum floatDatum(DatumType::Float);
			Assert::ExpectException<std::runtime_error>([&floatDatum] { floatDatum = 5; });
		}

		TEST_METHOD(AssignmentFloat)
		{
			//test 1: undefined datum
			Datum datum;
			datum = 5.0f;
			Assert::AreEqual(datum.Size(), 1_z);
			Assert::AreEqual(datum.Capacity(), 1_z);
			Assert::AreEqual(datum.Get<float>(), 5.0f);
			Assert::AreEqual(datum.Type(), DatumType::Float);

			//test 2: overwrite datum with 1 element
			datum = 22.0f;
			Assert::AreEqual(datum.Size(), 1_z);
			Assert::AreEqual(datum.Capacity(), 1_z);
			Assert::AreEqual(datum.Get<float>(), 22.0f);

			//test 3: overwrite datum with many elements
			/*datum.Resize(5, true);
			datum = 1.0f;
			Assert::AreEqual(datum.Size(), 1_z);
			Assert::AreEqual(datum.Capacity(), 1_z);
			Assert::AreEqual(datum.Get<float>(), 1.0f);*/

			//test 4: datum with wrong type (throw exception)
			Datum intDatum(DatumType::Integer);
			Assert::ExpectException<std::runtime_error>([&intDatum] { intDatum = 5.0f; });
		}

		TEST_METHOD(AssignmentVector)
		{
			//test 1: undefined datum
			Datum datum;
			glm::vec4 testVector(1.0);
			datum = testVector;
			Assert::AreEqual(datum.Size(), 1_z);
			Assert::AreEqual(datum.Capacity(), 1_z);
			Assert::AreEqual(datum.Get<glm::vec4>(), testVector);
			Assert::AreEqual(datum.Type(), DatumType::Vector);

			//test 2: overwrite datum with 1 element
			glm::vec4 testVector2(3.0);
			datum = testVector2;
			Assert::AreEqual(datum.Size(), 1_z);
			Assert::AreEqual(datum.Capacity(), 1_z);
			Assert::AreEqual(datum.Get<glm::vec4>(), testVector2);

			//test 3: overwrite datum with many elements
			/*datum.Resize(5, true);
			datum = testVector;
			Assert::AreEqual(datum.Size(), 1_z);
			Assert::AreEqual(datum.Capacity(), 1_z);
			Assert::AreEqual(datum.Get<glm::vec4>(), testVector);*/

			//test 4: datum with wrong type (throw exception)
			Datum intDatum(DatumType::Integer);
			Assert::ExpectException<std::runtime_error>([&intDatum, &testVector] { intDatum = testVector; });
		}

		TEST_METHOD(AssignmentMatrix)
		{
			//test 1: undefined datum
			Datum datum;
			glm::mat4 testMatrix(1.0);
			datum = testMatrix;
			Assert::AreEqual(datum.Size(), 1_z);
			Assert::AreEqual(datum.Capacity(), 1_z);
			Assert::AreEqual(datum.Get<glm::mat4>(), testMatrix);
			Assert::AreEqual(datum.Type(), DatumType::Matrix);

			//test 2: overwrite datum with 1 element
			glm::mat4 testMatrix2(3.0);
			datum = testMatrix2;
			Assert::AreEqual(datum.Size(), 1_z);
			Assert::AreEqual(datum.Capacity(), 1_z);
			Assert::AreEqual(datum.Get<glm::mat4>(), testMatrix2);

			//test 3: overwrite datum with many elements
			/*datum.Resize(5, true);
			datum = testMatrix;
			Assert::AreEqual(datum.Size(), 1_z);
			Assert::AreEqual(datum.Capacity(), 1_z);
			Assert::AreEqual(datum.Get<glm::mat4>(), testMatrix);*/

			//test 4: datum with wrong type (throw exception)
			Datum intDatum(DatumType::Integer);
			Assert::ExpectException<std::runtime_error>([&intDatum, &testMatrix] { intDatum = testMatrix; });
		}

		TEST_METHOD(AssignmentString)
		{
			//test 1: undefined datum
			Datum datum;
			std::string testString = "Hello"s;
			datum = testString;
			Assert::AreEqual(datum.Size(), 1_z);
			Assert::AreEqual(datum.Capacity(), 1_z);
			Assert::AreEqual(datum.Get<std::string>(), testString);
			Assert::AreEqual(datum.Type(), DatumType::String);

			//test 2: overwrite datum with 1 element
			std::string testString2 = "Goodbye"s;
			datum = testString2;
			Assert::AreEqual(datum.Size(), 1_z);
			Assert::AreEqual(datum.Capacity(), 1_z);
			Assert::AreEqual(datum.Get<std::string>(), testString2);

			//test 3: overwrite datum with many elements
			/*datum.Resize(5, true);
			datum = testString;
			Assert::AreEqual(datum.Size(), 1_z);
			Assert::AreEqual(datum.Capacity(), 1_z);
			Assert::AreEqual(datum.Get<std::string>(), testString);*/

			//test 4: datum with wrong type (throw exception)
			Datum intDatum(DatumType::Integer);
			Assert::ExpectException<std::runtime_error>([&intDatum, &testString] { intDatum = testString; });
		}

		TEST_METHOD(AssignmentPointer)
		{
			//test 1: undefined datum
			Datum datum;
			Foo foo(5);
			RTTI* testPointer = &foo;
			datum = &foo;
			Assert::AreEqual(datum.Size(), 1_z);
			Assert::AreEqual(datum.Capacity(), 1_z);
			Assert::AreEqual(datum.Get<RTTI*>(), testPointer);
			Assert::AreEqual(datum.Type(), DatumType::Pointer);

			//test 2: overwrite datum with 1 element
			Foo foo2(10);
			RTTI* testPointer2 = &foo2;
			datum = testPointer2;
			Assert::AreEqual(datum.Size(), 1_z);
			Assert::AreEqual(datum.Capacity(), 1_z);
			Assert::AreEqual(datum.Get<RTTI*>(), testPointer2);

			//test 3: overwrite datum with many elements
			/*datum.Resize(5, true);
			datum = testPointer;
			Assert::AreEqual(datum.Size(), 1_z);
			Assert::AreEqual(datum.Capacity(), 1_z);
			Assert::AreEqual(datum.Get<RTTI*>(), testPointer);*/

			//test 4: datum with wrong type (throw exception)
			Datum intDatum(DatumType::Integer);
			Assert::ExpectException<std::runtime_error>([&intDatum, &testPointer] { intDatum = testPointer; });
		}

		TEST_METHOD(Equality) 
		{
			//test 1: two default constructed datums (equal)
			{
				Datum datum1, datum2;
				Assert::AreEqual(datum1, datum2);
			}

			//test 2: mData's are equal (aliases)

			//test 3: different types (not equal)
			{
				Datum datum1(DatumType::Float), datum2(DatumType::Integer);
				Assert::IsTrue(datum1 != datum2);
			}

			//test 4: same type, different capacity (not equal)
			{
				Datum datum1(DatumType::Integer), datum2(DatumType::Integer, 5);
				Assert::IsTrue(datum1 != datum2);
			}

			//test 5: same type, same capacity, different size (not equal)
			{
				Datum datum1(DatumType::Integer, 5), datum2(DatumType::Integer, 5);
				datum1.PushBack(5);
				Assert::IsTrue(datum1 != datum2);
			}

			//test 6: datums with same type, capacity, size, contents (equal)
			//6.1: int datums
			{
				int test1(5), test2(10), test3(20), test4(15);
				Datum datum1(DatumType::Integer, 5), datum2(DatumType::Integer, 5);
				datum1.PushBack(test1);
				datum1.PushBack(test2);
				datum1.PushBack(test3);
				datum2.PushBack(test1);
				datum2.PushBack(test2);
				datum2.PushBack(test3);
				Assert::AreEqual(datum1, datum2);

				//7.1 same type, capacity, size but different contents
				datum2.PopBack();
				datum2.PushBack(test4);
				Assert::IsTrue(datum1 != datum2);
			}

			//6.2: float datums
			{
				float test1(5), test2(10), test3(20), test4(15);
				Datum datum1(DatumType::Float, 5), datum2(DatumType::Float, 5);
				datum1.PushBack(test1);
				datum1.PushBack(test2);
				datum1.PushBack(test3);
				datum2.PushBack(test1);
				datum2.PushBack(test2);
				datum2.PushBack(test3);
				Assert::AreEqual(datum1, datum2);

				//7.2 same type, capacity, size but different contents
				datum2.PopBack();
				datum2.PushBack(test4);
				Assert::IsTrue(datum1 != datum2);
			}

			//6.3 vector datums
			{
				glm::vec4 test1(5), test2(10), test3(20), test4(15);
				Datum datum1(DatumType::Vector, 5), datum2(DatumType::Vector, 5);
				datum1.PushBack(test1);
				datum1.PushBack(test2);
				datum1.PushBack(test3);
				datum2.PushBack(test1);
				datum2.PushBack(test2);
				datum2.PushBack(test3);
				Assert::AreEqual(datum1, datum2);

				//7.3 same type, capacity, size but different contents
				datum2.PopBack();
				datum2.PushBack(test4);
				Assert::IsTrue(datum1 != datum2);
			}

			//6.4: matrix datums
			{
				glm::mat4 test1(5), test2(10), test3(20), test4(15);
				Datum datum1(DatumType::Matrix, 5), datum2(DatumType::Matrix, 5);
				datum1.PushBack(test1);
				datum1.PushBack(test2);
				datum1.PushBack(test3);
				datum2.PushBack(test1);
				datum2.PushBack(test2);
				datum2.PushBack(test3);
				Assert::AreEqual(datum1, datum2);

				//7.4 same type, capacity, size but different contents
				datum2.PopBack();
				datum2.PushBack(test4);
				Assert::IsTrue(datum1 != datum2);
			}

			//6.5: string datums
			{
				std::string test1("Hello"), test2("Goodbye"), test3("ugh"), test4("blegh");
				Datum datum1(DatumType::String, 5), datum2(DatumType::String, 5);
				datum1.PushBack(test1);
				datum1.PushBack(test2);
				datum1.PushBack(test3);
				datum2.PushBack(test1);
				datum2.PushBack(test2);
				datum2.PushBack(test3);
				Assert::AreEqual(datum1, datum2);

				//7.5 same type, capacity, size but different contents
				datum2.PopBack();
				datum2.PushBack(test4);
				Assert::IsTrue(datum1 != datum2);
			}

			//6.6: pointer datums
			{
				Foo test1(5), test2(10), test3(20), test4(15);
				Datum datum1(DatumType::Pointer, 5), datum2(DatumType::Pointer, 5);
				datum1.PushBack(&test1);
				datum1.PushBack(&test2);
				datum1.PushBack(&test3);
				datum2.PushBack(&test1);
				datum2.PushBack(&test2);
				datum2.PushBack(&test3);
				Assert::AreEqual(datum1, datum2);

				//7.6 same type, capacity, size but different contents
				datum2.PopBack();
				datum2.PushBack(&test4);
				Assert::IsTrue(datum1 != datum2);
			}
		}

		TEST_METHOD(EqualityInt)
		{
			//test 1: are equal
			Datum datum;
			datum.PushBack(5);
			Assert::IsTrue(datum == 5);

			//test 2: datum not scalar (not equal)
			datum.PushBack(10);
			Assert::IsTrue(datum != 5);

			//test 3: datum with wrong type (not equal)
			Datum datum2(DatumType::Float);
			Assert::IsTrue(datum != 5);
		}

		TEST_METHOD(EqualityFloat)
		{
			//test 1: are equal
			Datum datum;
			datum.PushBack(5.0f);
			Assert::IsTrue(datum == 5.0f);

			//test 2: datum not scalar (not equal)
			datum.PushBack(10.0f);
			Assert::IsTrue(datum != 5.0f);

			//test 3: datum with wrong type (not equal)
			Datum datum2(DatumType::Integer);
			Assert::IsTrue(datum != 5.0f);
		}

		TEST_METHOD(EqualityVector)
		{
			glm::vec4 test1(1), test2(2);
			//test 1: are equal
			Datum datum;
			datum.PushBack(test1);
			Assert::IsTrue(datum == test1);

			//test 2: datum not scalar (not equal)
			datum.PushBack(test2);
			Assert::IsTrue(datum != test1);

			//test 3: datum with wrong type (not equal)
			Datum datum2(DatumType::Integer);
			Assert::IsTrue(datum != test1);
		}

		TEST_METHOD(EqualityMatrix)
		{
			glm::mat4 test1(1), test2(2);
			//test 1: are equal
			Datum datum;
			datum.PushBack(test1);
			Assert::IsTrue(datum == test1);

			//test 2: datum not scalar (not equal)
			datum.PushBack(test2);
			Assert::IsTrue(datum != test1);

			//test 3: datum with wrong type (not equal)
			Datum datum2(DatumType::Integer);
			Assert::IsTrue(datum != test1);
		}

		TEST_METHOD(EqualityString)
		{
			std::string test1("Hello"), test2("Goodbye");
			//test 1: are equal
			Datum datum;
			datum.PushBack(test1);
			Assert::IsTrue(datum == test1);

			//test 2: datum not scalar (not equal)
			datum.PushBack(test2);
			Assert::IsTrue(datum != test1);

			//test 3: datum with wrong type (not equal)
			Datum datum2(DatumType::Integer);
			Assert::IsTrue(datum != test1);
		}

		TEST_METHOD(EqualityPointer)
		{
			Foo test1(1), test2(2);
			//test 1: are equal
			Datum datum;
			datum.PushBack(&test1);
			Assert::IsTrue(datum == &test1);

			//test 2: datum not scalar (not equal)
			datum.PushBack(&test2);
			Assert::IsTrue(datum != &test1);

			//test 3: datum with wrong type (not equal)
			Datum datum2(DatumType::Integer);
			Assert::IsTrue(datum != &test1);
		}

		TEST_METHOD(GetInt)
		{
			//test 1: valid get
			Datum datum;
			datum = 5;
			Assert::AreEqual(datum.Get<int>(), 5);

			//test 2: out of bounds (throw exception)
			Assert::ExpectException<std::runtime_error>([&datum] { datum.Get<int>(1); });
			
			//test 3: invalid type (throw exception)
			Datum floatDatum;
			floatDatum.SetType(DatumType::Float);
			Assert::ExpectException<std::runtime_error>([&floatDatum] { floatDatum.Get<int>(); });

			//test 4: const
			const Datum* constDat = &datum;
			Assert::AreEqual(constDat->Get<int>(), 5);
		}

		TEST_METHOD(GetFloat)
		{
			//test 1: valid get
			Datum datum;
			datum = 5.0f;
			Assert::AreEqual(datum.Get<float>(), 5.0f);

			//test 2: out of bounds (throw exception)
			Assert::ExpectException<std::runtime_error>([&datum] { datum.Get<float>(1); });

			//test 3: invalid type (throw exception)
			Datum intDatum(DatumType::Integer);
			Assert::ExpectException<std::runtime_error>([&intDatum] { intDatum.Get<float>(); });

			//test 4: const
			const Datum* constDat = &datum;
			Assert::AreEqual(constDat->Get<float>(), 5.0f);
		}

		TEST_METHOD(GetVector)
		{
			//test 1: valid get
			Datum datum;
			glm::vec4 testVector(1.0);
			datum = testVector;
			Assert::AreEqual(datum.Get<glm::vec4>(), testVector);

			//test 2: out of bounds (throw exception)
			Assert::ExpectException<std::runtime_error>([&datum] { datum.Get<glm::vec4>(1); });

			//test 3: invalid type (throw exception)
			Datum intDatum(DatumType::Integer);
			Assert::ExpectException<std::runtime_error>([&intDatum] { intDatum.Get<glm::vec4>(); });

			//test 4: const
			const Datum* constDat = &datum;
			Assert::AreEqual(constDat->Get<glm::vec4>(), testVector);
		}

		TEST_METHOD(GetMatrix)
		{
			//test 1: valid get
			Datum datum;
			glm::mat4 testMatrix(1.0);
			datum = testMatrix;
			Assert::AreEqual(datum.Get<glm::mat4>(), testMatrix);

			//test 2: out of bounds (throw exception)
			Assert::ExpectException<std::runtime_error>([&datum] { datum.Get<glm::mat4>(1); });

			//test 3: invalid type (throw exception)
			Datum intDatum(DatumType::Integer);
			Assert::ExpectException<std::runtime_error>([&intDatum] { intDatum.Get<glm::mat4>(); });

			//test 4: const
			const Datum* constDat = &datum;
			Assert::AreEqual(constDat->Get<glm::mat4>(), testMatrix);
		}

		TEST_METHOD(GetString)
		{
			//test 1: valid get
			Datum datum;
			std::string testString = "Hello"s;
			datum = testString;
			Assert::AreEqual(datum.Get<std::string>(), testString);

			//test 2: out of bounds (throw exception)
			Assert::ExpectException<std::runtime_error>([&datum] { datum.Get<std::string>(1); });

			//test 3: invalid type (throw exception)
			Datum intDatum(DatumType::Integer);
			Assert::ExpectException<std::runtime_error>([&intDatum] { intDatum.Get<std::string>(); });

			//test 4: const
			const Datum* constDat = &datum;
			Assert::AreEqual(constDat->Get<std::string>(), testString);
		}

		TEST_METHOD(GetPointer)
		{
			//test 1: valid get
			Datum datum;
			Foo foo(5);
			RTTI* rttiPtr = &foo;
			datum = &foo;
			Assert::AreEqual(datum.Get<RTTI*>(), rttiPtr);

			//test 2: out of bounds (throw exception)
			Assert::ExpectException<std::runtime_error>([&datum] { datum.Get<RTTI*>(1); });

			//test 3: invalid type (throw exception)
			Datum intDatum(DatumType::Integer);
			Assert::ExpectException<std::runtime_error>([&intDatum] { intDatum.Get<RTTI*>(); });

			//test 4: const
			const Datum* constDat = &datum;
			Assert::AreEqual(constDat->Get<RTTI*>(), rttiPtr);
		}

		TEST_METHOD(SetInt)
		{
			//test 1: type not set (throw exception)
			Datum datum;
			Assert::ExpectException<std::runtime_error>([&datum] { datum.Set(5); });

			//test 2: out of bounds (throw exception)
			datum.SetType(DatumType::Integer);
			Assert::ExpectException<std::runtime_error>([&datum] { datum.Set(5); });
			datum.Resize(3, true);
			Assert::ExpectException<std::runtime_error>([&datum] { datum.Set(5, 3); });
			
			//test 3: valid sets
			datum.Set(1);
			datum.Set(2, 1);
			datum.Set(3, 2);
			Assert::AreEqual(datum.Get<int>(0), 1);
			Assert::AreEqual(datum.Get<int>(1), 2);
			Assert::AreEqual(datum.Get<int>(2), 3);
			datum.Set(5);
			Assert::AreEqual(datum.Get<int>(0), 5);

			//test 4: datum with invalid type
			Datum floatDatum;
			floatDatum = 5.0f;
			Assert::ExpectException<std::runtime_error>([&floatDatum] { floatDatum.Set(5); });
		}

		TEST_METHOD(SetFloat)
		{
			//test 1: type not set (throw exception)
			Datum datum;
			Assert::ExpectException<std::runtime_error>([&datum] { datum.Set(5.0f); });

			//test 2: out of bounds (throw exception)
			datum.SetType(DatumType::Float);
			Assert::ExpectException<std::runtime_error>([&datum] { datum.Set(5.0f); });
			datum.Resize(3, true);
			Assert::ExpectException<std::runtime_error>([&datum] { datum.Set(5.0f, 3); });
			
			//test 3: valid sets
			datum.Set(1.0f);
			datum.Set(2.0f, 1);
			datum.Set(3.0f, 2);
			Assert::AreEqual(datum.Get<float>(0), 1.0f);
			Assert::AreEqual(datum.Get<float>(1), 2.0f);
			Assert::AreEqual(datum.Get<float>(2), 3.0f);
			datum.Set(5.0f);		   
			Assert::AreEqual(datum.Get<float>(0), 5.0f);

			//test 4: datum with invalid type
			Datum intDatum;
			intDatum = 5;
			Assert::ExpectException<std::runtime_error>([&intDatum] { intDatum.Set(5.0f); });
		}

		TEST_METHOD(SetVector)
		{
			glm::vec4 test1(1), test2(2), test3(3);

			//test 1: type not set (throw exception)
			Datum datum;
			Assert::ExpectException<std::runtime_error>([&datum, &test1] { datum.Set(test1); });

			//test 2: out of bounds (throw exception)
			datum.SetType(DatumType::Vector);
			Assert::ExpectException<std::runtime_error>([&datum, &test1] { datum.Set(test1); });
			datum.Resize(3, true);
			Assert::ExpectException<std::runtime_error>([&datum, &test1] { datum.Set(test1, 3); });

			//test 3: valid sets
			datum.Set(test1);
			datum.Set(test2, 1);
			datum.Set(test3, 2);
			Assert::AreEqual(datum.Get<glm::vec4>(0), test1);
			Assert::AreEqual(datum.Get<glm::vec4>(1), test2);
			Assert::AreEqual(datum.Get<glm::vec4>(2), test3);
			datum.Set(test3);							  
			Assert::AreEqual(datum.Get<glm::vec4>(0), test3);

			//test 4: datum with invalid type
			Datum floatDatum;
			floatDatum = 5.0f;
			Assert::ExpectException<std::runtime_error>([&floatDatum, &test1] { floatDatum.Set(test1); });
		}

		TEST_METHOD(SetMatrix)
		{
			glm::mat4 test1(1), test2(2), test3(3);

			//test 1: type not set (throw exception)
			Datum datum;
			Assert::ExpectException<std::runtime_error>([&datum, &test1] { datum.Set(test1); });

			//test 2: out of bounds (throw exception)
			datum.SetType(DatumType::Matrix);
			Assert::ExpectException<std::runtime_error>([&datum, &test1] { datum.Set(test1); });
			datum.Resize(3, true);
			Assert::ExpectException<std::runtime_error>([&datum, &test1] { datum.Set(test1, 3); });

			//test 3: valid sets
			datum.Set(test1);
			datum.Set(test2, 1);
			datum.Set(test3, 2);
			Assert::AreEqual(datum.Get<glm::mat4>(0), test1);
			Assert::AreEqual(datum.Get<glm::mat4>(1), test2);
			Assert::AreEqual(datum.Get<glm::mat4>(2), test3);
			datum.Set(test3);
			Assert::AreEqual(datum.Get<glm::mat4>(0), test3);

			//test 4: datum with invalid type
			Datum floatDatum;
			floatDatum = 5.0f;
			Assert::ExpectException<std::runtime_error>([&floatDatum, &test1] { floatDatum.Set(test1); });
		}

		TEST_METHOD(SetString)
		{
			std::string test1("Hello"), test2("Goodbye"), test3("Ugh");

			//test 1: type not set (throw exception)
			Datum datum;
			Assert::ExpectException<std::runtime_error>([&datum, &test1] { datum.Set(test1); });

			//test 2: out of bounds (throw exception)
			datum.SetType(DatumType::String);
			Assert::ExpectException<std::runtime_error>([&datum, &test1] { datum.Set(test1); });
			datum.Resize(3, true);
			Assert::ExpectException<std::runtime_error>([&datum, &test1] { datum.Set(test1, 3); });

			//test 3: valid sets
			datum.Set(test1);
			datum.Set(test2, 1);
			datum.Set(test3, 2);
			Assert::AreEqual(datum.Get<std::string>(0), test1);
			Assert::AreEqual(datum.Get<std::string>(1), test2);
			Assert::AreEqual(datum.Get<std::string>(2), test3);
			datum.Set(test3);
			Assert::AreEqual(datum.Get<std::string>(0), test3);

			//test 4: datum with invalid type
			Datum floatDatum;
			floatDatum = 5.0f;
			Assert::ExpectException<std::runtime_error>([&floatDatum, &test1] { floatDatum.Set(test1); });
		}

		TEST_METHOD(SetPointer)
		{
			Foo test1(1), test2(2), test3(3);
			RTTI* rttiPtr1 = &test1;
			RTTI* rttiPtr2 = &test2;
			RTTI* rttiPtr3 = &test3;

			//test 1: type not set (throw exception)
			Datum datum;
			Assert::ExpectException<std::runtime_error>([&datum, &test1] { datum.Set(&test1); });

			//test 2: out of bounds (throw exception)
			datum.SetType(DatumType::Pointer);
			Assert::ExpectException<std::runtime_error>([&datum, &test1] { datum.Set(&test1); });
			datum.Resize(3, true);
			Assert::ExpectException<std::runtime_error>([&datum, &test1] { datum.Set(&test1, 3); });

			//test 3: valid sets
			datum.Set(&test1);
			datum.Set(&test2, 1);
			datum.Set(&test3, 2);
			Assert::AreEqual(datum.Get<RTTI*>(0), rttiPtr1);
			Assert::AreEqual(datum.Get<RTTI*>(1), rttiPtr2);
			Assert::AreEqual(datum.Get<RTTI*>(2), rttiPtr3);
			datum.Set(&test3);					  
			Assert::AreEqual(datum.Get<RTTI*>(0), rttiPtr3);

			//test 4: datum with invalid type
			Datum floatDatum;
			floatDatum = 5.0f;
			Assert::ExpectException<std::runtime_error>([&floatDatum, &test1] { floatDatum.Set(&test1); });
		}

		TEST_METHOD(PushBackInt)
		{
			//test 1: push to empty list 
			Datum datum;
			datum.PushBack(5);
			Assert::AreEqual(datum.Type(), DatumType::Integer);
			Assert::AreEqual(datum.Size(), 1_z);
			Assert::AreEqual(datum.Capacity(), 1_z);
			Assert::AreEqual(datum.Get<int>(), 5);

			//test 2: more valid pushes
			datum.PushBack(10);
			Assert::AreEqual(datum.Size(), 2_z);
			Assert::AreEqual(datum.Capacity(), 2_z);
			datum.PushBack(15);
			Assert::AreEqual(datum.Size(), 3_z);
			Assert::AreEqual(datum.Capacity(), 4_z);
			Assert::AreEqual(datum.Get<int>(), 5);
			Assert::AreEqual(datum.Get<int>(1), 10);
			Assert::AreEqual(datum.Get<int>(2), 15);
			

			//test 3: invalid type (throw exception)
			Datum floatDatum(DatumType::Float);
			Assert::ExpectException<std::runtime_error>([&floatDatum] { floatDatum.PushBack(5); });
		}

		TEST_METHOD(PushBackFloat)
		{
			//test 1: push to empty list 
			Datum datum;
			datum.PushBack(5.0f);
			Assert::AreEqual(datum.Type(), DatumType::Float);
			Assert::AreEqual(datum.Size(), 1_z);
			Assert::AreEqual(datum.Capacity(), 1_z);
			Assert::AreEqual(datum.Get<float>(), 5.0f);

			//test 2: more valid pushes
			datum.PushBack(10.0f);
			Assert::AreEqual(datum.Size(), 2_z);
			Assert::AreEqual(datum.Capacity(), 2_z);
			datum.PushBack(15.0f);
			Assert::AreEqual(datum.Size(), 3_z);
			Assert::AreEqual(datum.Capacity(), 4_z);
			Assert::AreEqual(datum.Get<float>(), 5.0f);
			Assert::AreEqual(datum.Get<float>(1), 10.0f);
			Assert::AreEqual(datum.Get<float>(2), 15.0f);


			//test 3: invalid type (throw exception)
			Datum intDatum(DatumType::Integer);
			Assert::ExpectException<std::runtime_error>([&intDatum] { intDatum.PushBack(5.0f); });
		}

		TEST_METHOD(PushBackVector)
		{
			glm::vec4 test1(1), test2(2), test3(3);
			//test 1: push to empty list 
			Datum datum;
			datum.PushBack(test1);
			Assert::AreEqual(datum.Type(), DatumType::Vector);
			Assert::AreEqual(datum.Size(), 1_z);
			Assert::AreEqual(datum.Capacity(), 1_z);
			Assert::AreEqual(datum.Get<glm::vec4>(), test1);

			//test 2: more valid pushes
			datum.PushBack(test2);
			Assert::AreEqual(datum.Size(), 2_z);
			Assert::AreEqual(datum.Capacity(), 2_z);
			datum.PushBack(test3);
			Assert::AreEqual(datum.Size(), 3_z);
			Assert::AreEqual(datum.Capacity(), 4_z);
			Assert::AreEqual(datum.Get<glm::vec4>(), test1);
			Assert::AreEqual(datum.Get<glm::vec4>(1), test2);
			Assert::AreEqual(datum.Get<glm::vec4>(2), test3);


			//test 3: invalid type (throw exception)
			Datum floatDatum(DatumType::Float);
			Assert::ExpectException<std::runtime_error>([&floatDatum, &test1] { floatDatum.PushBack(test1); });
		}

		TEST_METHOD(PushBackMatrix)
		{
			glm::mat4 test1(1), test2(2), test3(3);
			//test 1: push to empty list 
			Datum datum;
			datum.PushBack(test1);
			Assert::AreEqual(datum.Type(), DatumType::Matrix);
			Assert::AreEqual(datum.Size(), 1_z);
			Assert::AreEqual(datum.Capacity(), 1_z);
			Assert::AreEqual(datum.Get<glm::mat4>(), test1);

			//test 2: more valid pushes
			datum.PushBack(test2);
			Assert::AreEqual(datum.Size(), 2_z);
			Assert::AreEqual(datum.Capacity(), 2_z);
			datum.PushBack(test3);
			Assert::AreEqual(datum.Size(), 3_z);
			Assert::AreEqual(datum.Capacity(), 4_z);
			Assert::AreEqual(datum.Get<glm::mat4>(), test1);
			Assert::AreEqual(datum.Get<glm::mat4>(1), test2);
			Assert::AreEqual(datum.Get<glm::mat4>(2), test3);


			//test 3: invalid type (throw exception)
			Datum floatDatum(DatumType::Float);
			Assert::ExpectException<std::runtime_error>([&floatDatum, &test1] { floatDatum.PushBack(test1); });
		}

		TEST_METHOD(PushBackString)
		{
			std::string test1("Hello"), test2("Goodbye"), test3("UGH");
			//test 1: push to empty list 
			Datum datum;
			datum.PushBack(test1);
			Assert::AreEqual(datum.Type(), DatumType::String);
			Assert::AreEqual(datum.Size(), 1_z);
			Assert::AreEqual(datum.Capacity(), 1_z);
			Assert::AreEqual(datum.Get<std::string>(), test1);

			//test 2: more valid pushes
			datum.PushBack(test2);
			Assert::AreEqual(datum.Size(), 2_z);
			Assert::AreEqual(datum.Capacity(), 2_z);
			datum.PushBack(test3);
			Assert::AreEqual(datum.Size(), 3_z);
			Assert::AreEqual(datum.Capacity(), 4_z);
			Assert::AreEqual(datum.Get<std::string>(), test1);
			Assert::AreEqual(datum.Get<std::string>(1), test2);
			Assert::AreEqual(datum.Get<std::string>(2), test3);


			//test 3: invalid type (throw exception)
			Datum floatDatum(DatumType::Float);
			Assert::ExpectException<std::runtime_error>([&floatDatum, &test1] { floatDatum.PushBack(test1); });
		}

		TEST_METHOD(PushBackPointer)
		{
			Foo test1(1), test2(2), test3(3);
			//test 1: push to empty list 
			Datum datum;
			datum.PushBack(&test1);
			Assert::AreEqual(datum.Type(), DatumType::Pointer);
			Assert::AreEqual(datum.Size(), 1_z);
			Assert::AreEqual(datum.Capacity(), 1_z);
			Assert::AreEqual(datum.Get<RTTI*>(), reinterpret_cast<RTTI*>(&test1));

			//test 2: more valid pushes
			datum.PushBack(&test2);
			Assert::AreEqual(datum.Size(), 2_z);
			Assert::AreEqual(datum.Capacity(), 2_z);
			datum.PushBack(&test3);
			Assert::AreEqual(datum.Size(), 3_z);
			Assert::AreEqual(datum.Capacity(), 4_z);
			Assert::AreEqual(datum.Get<RTTI*>(), reinterpret_cast<RTTI*>(&test1));
			Assert::AreEqual(datum.Get<RTTI*>(1), reinterpret_cast<RTTI*>(&test2));
			Assert::AreEqual(datum.Get<RTTI*>(2), reinterpret_cast<RTTI*>(&test3));


			//test 3: invalid type (throw exception)
			Datum floatDatum(DatumType::Float);
			Assert::ExpectException<std::runtime_error>([&floatDatum, &test1] { floatDatum.PushBack(&test1); });
		}

		TEST_METHOD(PopBack)
		{
			//test 1: empty datum (nothing happens)
			Datum datum;
			datum.PopBack();

			//test 2: valid pop backs
			//2.1 int datum
			Datum intDatum;
			intDatum.PushBack(5);
			intDatum.PushBack(10);
			Assert::AreEqual(intDatum.Size(), 2_z);
			Assert::AreEqual(intDatum.Capacity(), 2_z);
			intDatum.PopBack();
			Assert::AreEqual(intDatum.Get<int>(), 5);
			Assert::AreEqual(intDatum.Size(), 1_z);
			intDatum.PopBack();
			Assert::AreEqual(intDatum.Size(), 0_z);
			Assert::AreEqual(intDatum.Capacity(), 2_z);

			//2.2 float datum
			Datum floatDatum;
			floatDatum.PushBack(5.0f);
			floatDatum.PushBack(10.0f);
			Assert::AreEqual(floatDatum.Size(), 2_z);
			Assert::AreEqual(floatDatum.Capacity(), 2_z);
			floatDatum.PopBack();
			Assert::AreEqual(floatDatum.Get<float>(), 5.0f);
			Assert::AreEqual(floatDatum.Size(), 1_z);
			floatDatum.PopBack();
			Assert::AreEqual(floatDatum.Size(), 0_z);
			Assert::AreEqual(floatDatum.Capacity(), 2_z);

			//2.3 vector datum
			glm::vec4 testVector1(5), testVector2(10);
			Datum vectorDatum;
			vectorDatum.PushBack(testVector1);
			vectorDatum.PushBack(testVector2);
			Assert::AreEqual(vectorDatum.Size(), 2_z);
			Assert::AreEqual(vectorDatum.Capacity(), 2_z);
			vectorDatum.PopBack();
			Assert::AreEqual(vectorDatum.Get<glm::vec4>(), testVector1);
			Assert::AreEqual(vectorDatum.Size(), 1_z);
			vectorDatum.PopBack();
			Assert::AreEqual(vectorDatum.Size(), 0_z);
			Assert::AreEqual(vectorDatum.Capacity(), 2_z);

			//2.4 matrix datum
			glm::mat4 testMatrix1(5), testMatrix2(10);
			Datum matrixDatum;
			matrixDatum.PushBack(testMatrix1);
			matrixDatum.PushBack(testMatrix2);
			Assert::AreEqual(matrixDatum.Size(), 2_z);
			Assert::AreEqual(matrixDatum.Capacity(), 2_z);
			matrixDatum.PopBack();
			Assert::AreEqual(matrixDatum.Get<glm::mat4>(), testMatrix1);
			Assert::AreEqual(matrixDatum.Size(), 1_z);
			matrixDatum.PopBack();
			Assert::AreEqual(matrixDatum.Size(), 0_z);
			Assert::AreEqual(matrixDatum.Capacity(), 2_z);

			//2.5 string datum
			std::string testString1("Hello"), testString2("Goodbye");
			Datum StringDatum;
			StringDatum.PushBack(testString1);
			StringDatum.PushBack(testString2);
			Assert::AreEqual(StringDatum.Size(), 2_z);
			Assert::AreEqual(StringDatum.Capacity(), 2_z);
			StringDatum.PopBack();
			Assert::AreEqual(StringDatum.Get<std::string>(), testString1);
			Assert::AreEqual(StringDatum.Size(), 1_z);
			StringDatum.PopBack();
			Assert::AreEqual(StringDatum.Size(), 0_z);
			Assert::AreEqual(StringDatum.Capacity(), 2_z);

			//2.6 RTTI* datum
			Foo test1(1), test2(2);
			Datum PointerDatum;
			PointerDatum.PushBack(&test1);
			PointerDatum.PushBack(&test2);
			Assert::AreEqual(PointerDatum.Size(), 2_z);
			Assert::AreEqual(PointerDatum.Capacity(), 2_z);
			PointerDatum.PopBack();
			Assert::AreEqual(PointerDatum.Get<RTTI*>(), reinterpret_cast<RTTI*>(&test1));
			Assert::AreEqual(PointerDatum.Size(), 1_z);
			PointerDatum.PopBack();
			Assert::AreEqual(PointerDatum.Size(), 0_z);
			Assert::AreEqual(PointerDatum.Capacity(), 2_z);
		}

		TEST_METHOD(FindInt)
		{
			//test 1: type not set (throw exception)
			Datum datum;
			Assert::ExpectException<std::runtime_error>([&datum] { datum.Find(5); });

			//test 2: type set but list empty
			datum.SetType(DatumType::Integer);
			Assert::AreEqual(datum.Find(5), 0_z);

			//test 2: value exists
			datum.PushBack(5);
			datum.PushBack(10);
			datum.PushBack(15);
			Assert::AreEqual(datum.Find(5), 0_z);
			Assert::AreEqual(datum.Find(10), 1_z);
			Assert::AreEqual(datum.Find(15), 2_z);

			//test 3: value does not exist
			Assert::AreEqual(datum.Find(20), 3_z);

			//test 4: invalid type (throw exception)
			Datum floatDatum(DatumType::Float);
			Assert::ExpectException<std::runtime_error>([&floatDatum] { floatDatum.Find(5); });
		}

		TEST_METHOD(FindFloat)
		{
			//test 1: type not set (throw exception)
			Datum datum;
			Assert::ExpectException<std::runtime_error>([&datum] { datum.Find(5.0f); });

			//test 2: type set but list empty
			datum.SetType(DatumType::Float);
			Assert::AreEqual(datum.Find(5.0f), 0_z);

			//test 2: value exists
			datum.PushBack(5.0f);
			datum.PushBack(10.0f);
			datum.PushBack(15.0f);
			Assert::AreEqual(datum.Find(5.0f), 0_z);
			Assert::AreEqual(datum.Find(10.0f), 1_z);
			Assert::AreEqual(datum.Find(15.0f), 2_z);

			//test 3: value does not exist
			Assert::AreEqual(datum.Find(20.0f), 3_z);

			//test 4: invalid type (throw exception)
			Datum intDatum(DatumType::Integer);
			Assert::ExpectException<std::runtime_error>([&intDatum] { intDatum.Find(5.0f); });
		}

		TEST_METHOD(FindVector)
		{
			glm::vec4 test1(1), test2(2), test3(3), test4(4);
			//test 1: type not set (throw exception)
			Datum datum;
			Assert::ExpectException<std::runtime_error>([&datum, &test1] { datum.Find(test1); });

			//test 2: type set but list empty
			datum.SetType(DatumType::Vector);
			Assert::AreEqual(datum.Find(test1), 0_z);

			//test 2: value exists
			datum.PushBack(test1);
			datum.PushBack(test2);
			datum.PushBack(test3);
			Assert::AreEqual(datum.Find(test1), 0_z);
			Assert::AreEqual(datum.Find(test2), 1_z);
			Assert::AreEqual(datum.Find(test3), 2_z);

			//test 3: value does not exist
			Assert::AreEqual(datum.Find(test4), 3_z);

			//test 4: invalid type (throw exception)
			Datum intDatum(DatumType::Integer);
			Assert::ExpectException<std::runtime_error>([&intDatum, &test1] { intDatum.Find(test1); });
		}

		TEST_METHOD(FindMatrix)
		{
			glm::mat4 test1(1), test2(2), test3(3), test4(4);
			//test 1: type not set (throw exception)
			Datum datum;
			Assert::ExpectException<std::runtime_error>([&datum, &test1] { datum.Find(test1); });

			//test 2: type set but list empty
			datum.SetType(DatumType::Matrix);
			Assert::AreEqual(datum.Find(test1), 0_z);

			//test 2: value exists
			datum.PushBack(test1);
			datum.PushBack(test2);
			datum.PushBack(test3);
			Assert::AreEqual(datum.Find(test1), 0_z);
			Assert::AreEqual(datum.Find(test2), 1_z);
			Assert::AreEqual(datum.Find(test3), 2_z);

			//test 3: value does not exist
			Assert::AreEqual(datum.Find(test4), 3_z);

			//test 4: invalid type (throw exception)
			Datum intDatum(DatumType::Integer);
			Assert::ExpectException<std::runtime_error>([&intDatum, &test1] { intDatum.Find(test1); });
		}

		TEST_METHOD(FindString)
		{
			std::string test1("Hello"), test2("Goodbye"), test3("Ugh"), test4("Blegh");
			//test 1: type not set (throw exception)
			Datum datum;
			Assert::ExpectException<std::runtime_error>([&datum, &test1] { datum.Find(test1); });

			//test 2: type set but list empty
			datum.SetType(DatumType::String);
			Assert::AreEqual(datum.Find(test1), 0_z);

			//test 2: value exists
			datum.PushBack(test1);
			datum.PushBack(test2);
			datum.PushBack(test3);
			Assert::AreEqual(datum.Find(test1), 0_z);
			Assert::AreEqual(datum.Find(test2), 1_z);
			Assert::AreEqual(datum.Find(test3), 2_z);

			//test 3: value does not exist
			Assert::AreEqual(datum.Find(test4), 3_z);

			//test 4: invalid type (throw exception)
			Datum intDatum(DatumType::Integer);
			Assert::ExpectException<std::runtime_error>([&intDatum, &test1] { intDatum.Find(test1); });
		}

		TEST_METHOD(FindPointer)
		{
			Foo test1(1), test2(2), test3(3), test4(4);
			//test 1: type not set (throw exception)
			Datum datum;
			Assert::ExpectException<std::runtime_error>([&datum, &test1] { datum.Find(&test1); });

			//test 2: type set but list empty
			datum.SetType(DatumType::Pointer);
			Assert::AreEqual(datum.Find(&test1), 0_z);

			//test 2: value exists
			datum.PushBack(&test1);
			datum.PushBack(&test2);
			datum.PushBack(&test3);
			Assert::AreEqual(datum.Find(&test1), 0_z);
			Assert::AreEqual(datum.Find(&test2), 1_z);
			Assert::AreEqual(datum.Find(&test3), 2_z);
										
			//test 3: value does not exist
			Assert::AreEqual(datum.Find(&test4), 3_z);

			//test 4: invalid type (throw exception)
			Datum intDatum(DatumType::Integer);
			Assert::ExpectException<std::runtime_error>([&intDatum, &test1] { intDatum.Find(&test1); });
		}

		TEST_METHOD(RemoveAt)
		{
			//test 1: type not set/out of bounds
			Datum unknownDatum;
			Assert::IsFalse(unknownDatum.RemoveAt(0));
			
			//int datum tests
			{
				int test1(5), test2(10), test3(20);
				Datum datum;
				datum.PushBack(test1);
				datum.PushBack(test2);
				datum.PushBack(test3);

				Assert::IsTrue(datum.RemoveAt(0));
				Assert::AreEqual(datum.Size(), 2_z);
				Assert::AreEqual(datum.Capacity(), 4_z);
				Assert::AreEqual(datum.Find(test1), datum.Size());

				Assert::IsTrue(datum.RemoveAt(1));
				Assert::AreEqual(datum.Find(test3), datum.Size());
				Assert::IsTrue(datum.RemoveAt(0));
				Assert::AreEqual(datum.Find(test2), datum.Size());

				Assert::AreEqual(datum.Size(), 0_z);
				Assert::AreEqual(datum.Capacity(), 4_z);
			}

			//float datum tests
			{
				float test1(5), test2(10), test3(20);
				Datum datum;
				datum.PushBack(test1);
				datum.PushBack(test2);
				datum.PushBack(test3);

				Assert::IsTrue(datum.RemoveAt(0));
				Assert::AreEqual(datum.Size(), 2_z);
				Assert::AreEqual(datum.Capacity(), 4_z);
				Assert::AreEqual(datum.Find(test1), datum.Size());

				Assert::IsTrue(datum.RemoveAt(1));
				Assert::AreEqual(datum.Find(test3), datum.Size());
				Assert::IsTrue(datum.RemoveAt(0));
				Assert::AreEqual(datum.Find(test2), datum.Size());

				Assert::AreEqual(datum.Size(), 0_z);
				Assert::AreEqual(datum.Capacity(), 4_z);
			}

			//vector datum tests
			{
				glm::vec4 test1(5), test2(10), test3(20);
				Datum datum;
				datum.PushBack(test1);
				datum.PushBack(test2);
				datum.PushBack(test3);

				Assert::IsTrue(datum.RemoveAt(0));
				Assert::AreEqual(datum.Size(), 2_z);
				Assert::AreEqual(datum.Capacity(), 4_z);
				Assert::AreEqual(datum.Find(test1), datum.Size());

				Assert::IsTrue(datum.RemoveAt(1));
				Assert::AreEqual(datum.Find(test3), datum.Size());
				Assert::IsTrue(datum.RemoveAt(0));
				Assert::AreEqual(datum.Find(test2), datum.Size());

				Assert::AreEqual(datum.Size(), 0_z);
				Assert::AreEqual(datum.Capacity(), 4_z);
			}

			//matrix datum tests
			{
				glm::mat4 test1(5), test2(10), test3(20);
				Datum datum;
				datum.PushBack(test1);
				datum.PushBack(test2);
				datum.PushBack(test3);

				Assert::IsTrue(datum.RemoveAt(0));
				Assert::AreEqual(datum.Size(), 2_z);
				Assert::AreEqual(datum.Capacity(), 4_z);
				Assert::AreEqual(datum.Find(test1), datum.Size());

				Assert::IsTrue(datum.RemoveAt(1));
				Assert::AreEqual(datum.Find(test3), datum.Size());
				Assert::IsTrue(datum.RemoveAt(0));
				Assert::AreEqual(datum.Find(test2), datum.Size());

				Assert::AreEqual(datum.Size(), 0_z);
				Assert::AreEqual(datum.Capacity(), 4_z);
			}

			//String datum tests
			{
				std::string test1("Hello"), test2("Goodbye"), test3("Ugh");
				Datum datum;
				datum.PushBack(test1);
				datum.PushBack(test2);
				datum.PushBack(test3);

				Assert::IsTrue(datum.RemoveAt(0));
				Assert::AreEqual(datum.Size(), 2_z);
				Assert::AreEqual(datum.Capacity(), 4_z);
				Assert::AreEqual(datum.Find(test1), datum.Size());

				Assert::IsTrue(datum.RemoveAt(1));
				Assert::AreEqual(datum.Find(test3), datum.Size());
				Assert::IsTrue(datum.RemoveAt(0));
				Assert::AreEqual(datum.Find(test2), datum.Size());

				Assert::AreEqual(datum.Size(), 0_z);
				Assert::AreEqual(datum.Capacity(), 4_z);
			}

			//pointer datum tests
			{
				Foo test1(5), test2(10), test3(20);
				Datum datum;
				datum.PushBack(&test1);
				datum.PushBack(&test2);
				datum.PushBack(&test3);

				Assert::IsTrue(datum.RemoveAt(0));
				Assert::AreEqual(datum.Size(), 2_z);
				Assert::AreEqual(datum.Capacity(), 4_z);
				Assert::AreEqual(datum.Find(&test1), datum.Size());

				Assert::IsTrue(datum.RemoveAt(1));
				Assert::AreEqual(datum.Find(&test3), datum.Size());
				Assert::IsTrue(datum.RemoveAt(0));
				Assert::AreEqual(datum.Find(&test2), datum.Size());

				Assert::AreEqual(datum.Size(), 0_z);
				Assert::AreEqual(datum.Capacity(), 4_z);
			}

		}

		TEST_METHOD(RemoveInt)
		{
			int test1(5), test2(10), test3(20), test4(50);

			//test 1: type not int
			Datum datum;
			Assert::ExpectException<std::runtime_error>([&datum, &test1] { datum.Remove(test1); });

			//test 2: does not exist
			datum.PushBack(test1);
			datum.PushBack(test2);
			datum.PushBack(test3);
			Assert::IsFalse(datum.Remove(test4));

			//test 3: valid removes
			Assert::IsTrue(datum.Remove(test1));
			Assert::AreEqual(datum.Size(), 2_z);
			Assert::AreEqual(datum.Capacity(), 4_z);
			Assert::AreEqual(datum.Find(test1), datum.Size());

			Assert::IsTrue(datum.Remove(test3));
			Assert::AreEqual(datum.Find(test3), datum.Size());
			Assert::IsTrue(datum.Remove(test2));
			Assert::AreEqual(datum.Find(test2), datum.Size());

			Assert::AreEqual(datum.Size(), 0_z);
			Assert::AreEqual(datum.Capacity(), 4_z);
		}

		TEST_METHOD(RemoveFloat)
		{
			float test1(5), test2(10), test3(20), test4(50);

			//test 1: type not int
			Datum datum;
			Assert::ExpectException<std::runtime_error>([&datum, &test1] { datum.Remove(test1); });

			//test 2: does not exist
			datum.PushBack(test1);
			datum.PushBack(test2);
			datum.PushBack(test3);
			Assert::IsFalse(datum.Remove(test4));

			//test 3: valid removes
			Assert::IsTrue(datum.Remove(test1));
			Assert::AreEqual(datum.Size(), 2_z);
			Assert::AreEqual(datum.Capacity(), 4_z);
			Assert::AreEqual(datum.Find(test1), datum.Size());

			Assert::IsTrue(datum.Remove(test3));
			Assert::AreEqual(datum.Find(test3), datum.Size());
			Assert::IsTrue(datum.Remove(test2));
			Assert::AreEqual(datum.Find(test2), datum.Size());

			Assert::AreEqual(datum.Size(), 0_z);
			Assert::AreEqual(datum.Capacity(), 4_z);
		}

		TEST_METHOD(RemoveVector)
		{
			glm::vec4 test1(5), test2(10), test3(20), test4(50);

			//test 1: type not int
			Datum datum;
			Assert::ExpectException<std::runtime_error>([&datum, &test1] { datum.Remove(test1); });

			//test 2: does not exist
			datum.PushBack(test1);
			datum.PushBack(test2);
			datum.PushBack(test3);
			Assert::IsFalse(datum.Remove(test4));

			//test 3: valid removes
			Assert::IsTrue(datum.Remove(test1));
			Assert::AreEqual(datum.Size(), 2_z);
			Assert::AreEqual(datum.Capacity(), 4_z);
			Assert::AreEqual(datum.Find(test1), datum.Size());

			Assert::IsTrue(datum.Remove(test3));
			Assert::AreEqual(datum.Find(test3), datum.Size());
			Assert::IsTrue(datum.Remove(test2));
			Assert::AreEqual(datum.Find(test2), datum.Size());

			Assert::AreEqual(datum.Size(), 0_z);
			Assert::AreEqual(datum.Capacity(), 4_z);
		}

		TEST_METHOD(RemoveMatrix)
		{
			glm::mat4 test1(5), test2(10), test3(20), test4(50);

			//test 1: type not int
			Datum datum;
			Assert::ExpectException<std::runtime_error>([&datum, &test1] { datum.Remove(test1); });

			//test 2: does not exist
			datum.PushBack(test1);
			datum.PushBack(test2);
			datum.PushBack(test3);
			Assert::IsFalse(datum.Remove(test4));

			//test 3: valid removes
			Assert::IsTrue(datum.Remove(test1));
			Assert::AreEqual(datum.Size(), 2_z);
			Assert::AreEqual(datum.Capacity(), 4_z);
			Assert::AreEqual(datum.Find(test1), datum.Size());

			Assert::IsTrue(datum.Remove(test3));
			Assert::AreEqual(datum.Find(test3), datum.Size());
			Assert::IsTrue(datum.Remove(test2));
			Assert::AreEqual(datum.Find(test2), datum.Size());

			Assert::AreEqual(datum.Size(), 0_z);
			Assert::AreEqual(datum.Capacity(), 4_z);
		}

		TEST_METHOD(RemoveString)
		{
			std::string test1("Hello"), test2("Goodbye"), test3("ugh"), test4("blegh");

			//test 1: type not int
			Datum datum;
			Assert::ExpectException<std::runtime_error>([&datum, &test1] { datum.Remove(test1); });

			//test 2: does not exist
			datum.PushBack(test1);
			datum.PushBack(test2);
			datum.PushBack(test3);
			Assert::IsFalse(datum.Remove(test4));

			//test 3: valid removes
			Assert::IsTrue(datum.Remove(test1));
			Assert::AreEqual(datum.Size(), 2_z);
			Assert::AreEqual(datum.Capacity(), 4_z);
			Assert::AreEqual(datum.Find(test1), datum.Size());

			Assert::IsTrue(datum.Remove(test3));
			Assert::AreEqual(datum.Find(test3), datum.Size());
			Assert::IsTrue(datum.Remove(test2));
			Assert::AreEqual(datum.Find(test2), datum.Size());

			Assert::AreEqual(datum.Size(), 0_z);
			Assert::AreEqual(datum.Capacity(), 4_z);
		}

		TEST_METHOD(RemovePointer)
		{
			Foo test1(5), test2(10), test3(20), test4(50);

			//test 1: type not int
			Datum datum;
			Assert::ExpectException<std::runtime_error>([&datum, &test1] { datum.Remove(&test1); });

			//test 2: does not exist
			datum.PushBack(&test1);
			datum.PushBack(&test2);
			datum.PushBack(&test3);
			Assert::IsFalse(datum.Remove(&test4));

			//test 3: valid removes
			Assert::IsTrue(datum.Remove(&test1));
			Assert::AreEqual(datum.Size(), 2_z);
			Assert::AreEqual(datum.Capacity(), 4_z);
			Assert::AreEqual(datum.Find(&test1), datum.Size());

			Assert::IsTrue(datum.Remove(&test3));
			Assert::AreEqual(datum.Find(&test3), datum.Size());
			Assert::IsTrue(datum.Remove(&test2));
			Assert::AreEqual(datum.Find(&test2), datum.Size());

			Assert::AreEqual(datum.Size(), 0_z);
			Assert::AreEqual(datum.Capacity(), 4_z);
		}

		TEST_METHOD(SetStorageInt)
		{
			int test1(5), test2(10), test3(15);
			int arr[3] = { test1, test2, test3 };
			int* badptr = nullptr;

			//test 1: nullptr
			Datum datum;
			Assert::ExpectException<std::runtime_error>([&datum, &badptr] { datum.SetStorage(badptr, 0); });

			//test2: valid
			datum.SetStorage(arr, 3);
			Assert::AreEqual(datum.Capacity(), 3_z);
			Assert::AreEqual(datum.Size(), 3_z);
			Assert::AreEqual(datum.Get<int>(), test1);
			Assert::AreEqual(datum.Get<int>(1), test2);
			Assert::AreEqual(datum.Get<int>(2), test3);

			//test 3: bad type
			Datum floatDatum(DatumType::Float);
			Assert::ExpectException<std::runtime_error>([&floatDatum, &arr] { floatDatum.SetStorage(arr, 3); });

			//test 4: allocated memory already
			Assert::ExpectException<std::runtime_error>([&datum, &arr] { datum.SetStorage(arr, 3); });
		}

		TEST_METHOD(SetStorageFloat)
		{
			float test1(5), test2(10), test3(15);
			float arr[3] = { test1, test2, test3 };
			float* badptr = nullptr;

			//test 1: nullptr
			Datum datum;
			Assert::ExpectException<std::runtime_error>([&datum, &badptr] { datum.SetStorage(badptr, 0); });

			//test2: valid
			datum.SetStorage(arr, 3);
			Assert::AreEqual(datum.Capacity(), 3_z);
			Assert::AreEqual(datum.Size(), 3_z);
			Assert::AreEqual(datum.Get<float>(), test1);
			Assert::AreEqual(datum.Get<float>(1), test2);
			Assert::AreEqual(datum.Get<float>(2), test3);

			//test 3: bad type
			Datum intDatum(DatumType::Integer);
			Assert::ExpectException<std::runtime_error>([&intDatum, &arr] { intDatum.SetStorage(arr, 3); });

			//test 4: allocated memory already
			Assert::ExpectException<std::runtime_error>([&datum, &arr] { datum.SetStorage(arr, 3); });
		}

		TEST_METHOD(SetStorageVector)
		{
			glm::vec4 test1(5), test2(10), test3(15);
			glm::vec4 arr[3] = { test1, test2, test3 };
			glm::vec4* badptr = nullptr;

			//test 1: nullptr
			Datum datum;
			Assert::ExpectException<std::runtime_error>([&datum, &badptr] { datum.SetStorage(badptr, 0); });

			//test2: valid
			datum.SetStorage(arr, 3);
			Assert::AreEqual(datum.Capacity(), 3_z);
			Assert::AreEqual(datum.Size(), 3_z);
			Assert::AreEqual(datum.Get<glm::vec4>(), test1);
			Assert::AreEqual(datum.Get<glm::vec4>(1), test2);
			Assert::AreEqual(datum.Get<glm::vec4>(2), test3);

			//test 3: bad type
			Datum intDatum(DatumType::Integer);
			Assert::ExpectException<std::runtime_error>([&intDatum, &arr] { intDatum.SetStorage(arr, 3); });

			//test 4: allocated memory already
			Assert::ExpectException<std::runtime_error>([&datum, &arr] { datum.SetStorage(arr, 3); });
		}

		TEST_METHOD(SetStorageMatrix)
		{
			glm::mat4 test1(5), test2(10), test3(15);
			glm::mat4 arr[3] = { test1, test2, test3 };
			glm::mat4* badptr = nullptr;

			//test 1: nullptr
			Datum datum;
			Assert::ExpectException<std::runtime_error>([&datum, &badptr] { datum.SetStorage(badptr, 0); });

			//test2: valid
			datum.SetStorage(arr, 3);
			Assert::AreEqual(datum.Capacity(), 3_z);
			Assert::AreEqual(datum.Size(), 3_z);
			Assert::AreEqual(datum.Get<glm::mat4>(), test1);
			Assert::AreEqual(datum.Get<glm::mat4>(1), test2);
			Assert::AreEqual(datum.Get<glm::mat4>(2), test3);

			//test 3: bad type
			Datum intDatum(DatumType::Integer);
			Assert::ExpectException<std::runtime_error>([&intDatum, &arr] { intDatum.SetStorage(arr, 3); });

			//test 4: allocated memory already
			Assert::ExpectException<std::runtime_error>([&datum, &arr] { datum.SetStorage(arr, 3); });
		}

		TEST_METHOD(SetStorageString)
		{
			std::string test1("Hello"), test2("Goodbye"), test3("ugh");
			std::string arr[3] = { test1, test2, test3 };
			std::string* badptr = nullptr;

			//test 1: nullptr
			Datum datum;
			Assert::ExpectException<std::runtime_error>([&datum, &badptr] { datum.SetStorage(badptr, 0); });

			//test2: valid
			datum.SetStorage(arr, 3);
			Assert::AreEqual(datum.Capacity(), 3_z);
			Assert::AreEqual(datum.Size(), 3_z);
			Assert::AreEqual(datum.Get<std::string>(), test1);
			Assert::AreEqual(datum.Get<std::string>(1), test2);
			Assert::AreEqual(datum.Get<std::string>(2), test3);

			//test 3: bad type
			Datum intDatum(DatumType::Integer);
			Assert::ExpectException<std::runtime_error>([&intDatum, &arr] { intDatum.SetStorage(arr, 3); });

			//test 4: allocated memory already
			Assert::ExpectException<std::runtime_error>([&datum, &arr] { datum.SetStorage(arr, 3); });
		}

		TEST_METHOD(SetStoragePointer)
		{
			Foo test1(10), test2(15), test3(25);
			RTTI* arr[3] = { &test1, &test2, &test3 };
			std::string* badptr = nullptr;

			//test 1: nullptr
			Datum datum;
			Assert::ExpectException<std::runtime_error>([&datum, &badptr] { datum.SetStorage(badptr, 0); });

			//test2: valid
			datum.SetStorage(arr, 3);
			Assert::AreEqual(datum.Capacity(), 3_z);
			Assert::AreEqual(datum.Size(), 3_z);
			Assert::AreEqual(datum.Get<RTTI*>(), reinterpret_cast<RTTI*>(&test1));
			Assert::AreEqual(datum.Get<RTTI*>(1), reinterpret_cast<RTTI*>(&test2));
			Assert::AreEqual(datum.Get<RTTI*>(2), reinterpret_cast<RTTI*>(&test3));

			//test 3: bad type
			Datum intDatum(DatumType::Integer);
			Assert::ExpectException<std::runtime_error>([&intDatum, &arr] { intDatum.SetStorage(arr, 3); });

			//test 4: allocated memory already
			Assert::ExpectException<std::runtime_error>([&datum, &arr] { datum.SetStorage(arr, 3); });
		}

		TEST_METHOD(ToString)
		{
			//test 1: default constructed datum (throw exception)
			Datum defaultDatum;
			Assert::ExpectException<std::runtime_error>([&defaultDatum] { defaultDatum.ToString(); });

			//test 2: index out of bounds (throw exception)
			defaultDatum.SetType(DatumType::String);
			Assert::ExpectException<std::runtime_error>([&defaultDatum] { defaultDatum.ToString(); });

			//test 3: valid to strings
			Datum intDatum;
			intDatum = 5;
			Assert::AreEqual(intDatum.ToString(), "5"s);

			Datum floatDatum;
			floatDatum = 5.0f;
			Assert::AreEqual(floatDatum.ToString(), "5.000000"s);

			Datum vectorDatum;
			vectorDatum = glm::vec4(5);
			Assert::AreEqual(vectorDatum.ToString(), "vec4(5.000000, 5.000000, 5.000000, 5.000000)"s);

			Datum matrixDatum;
			matrixDatum = glm::mat4(5);
			Assert::AreEqual(matrixDatum.ToString(), "mat4x4((5.000000, 0.000000, 0.000000, 0.000000), (0.000000, 5.000000, 0.000000, 0.000000), (0.000000, 0.000000, 5.000000, 0.000000), (0.000000, 0.000000, 0.000000, 5.000000))"s);

			Datum stringDatum;
			stringDatum = "Hello"s;
			Assert::AreEqual(stringDatum.ToString(), "Hello"s);

			Datum RTTIDatum;
			Foo foo(5);
			RTTIDatum = &foo;
			Assert::AreEqual(RTTIDatum.ToString(), "RTTI"s);
		}

		TEST_METHOD(SetFromString)
		{
			//int tests
			{
				std::string str1 = "5";
				std::string str2 = "10";
				Datum datum(DatumType::Integer);
				datum.Resize(5, true);
				datum.SetFromString<int>(str1);
				datum.SetFromString<int>(str2, 1);
				Assert::AreEqual(datum.Get<int>(), 5);
				Assert::AreEqual(datum.Get<int>(1), 10);

				//invalid string
				Assert::ExpectException<invalid_argument>([&datum] { datum.SetFromString<int>("bleh"s); });
				Assert::ExpectException<out_of_range>([&datum] { datum.SetFromString<int>("99999999999999"s); });
			}

			//float tests
			{
				std::string str1 = "5.123";
				std::string str2 = "10.2901";
				Datum datum(DatumType::Float);
				datum.Resize(5, true);
				datum.SetFromString<float>(str1);
				datum.SetFromString<float>(str2, 1);
				Assert::AreEqual(datum.Get<float>(), 5.123f);
				Assert::AreEqual(datum.Get<float>(1), 10.2901f);

				//invalid string
				Assert::ExpectException<invalid_argument>([&datum] { datum.SetFromString<float>("bleh"s); });
				//Assert::ExpectException<out_of_range>([&datum] { datum.SetFromString<float>("99999999999999.5"s); });
			}

			//vector tests
			{
				glm::vec4 vec1(5.123f, 10.2f, 3.1f, 2.3f);
				glm::vec4 vec2(2.23f);
				Datum datum(DatumType::Vector);
				datum.Resize(5, true);
				datum.SetFromString<glm::vec4>(glm::to_string(vec1));
				datum.SetFromString<glm::vec4>(glm::to_string(vec2), 1);
				Assert::AreEqual(datum.Get<glm::vec4>(), vec1);
				Assert::AreEqual(datum.Get<glm::vec4>(1), vec2);

				Assert::ExpectException<std::runtime_error>([&datum] { datum.SetFromString<glm::vec4>("bleh"s); });
			}

			//matrix tests
			{
				glm::mat4 mat1(5.00f);
				glm::mat4 mat2(2.23f);
				Datum datum(DatumType::Matrix);
				datum.Resize(5, true);
				datum.SetFromString<glm::mat4>(glm::to_string(mat1));
				datum.SetFromString<glm::mat4>(glm::to_string(mat2), 1);
				Assert::AreEqual(datum.Get<glm::mat4>(), mat1);
				Assert::AreEqual(datum.Get<glm::mat4>(1), mat2);

				Assert::ExpectException<std::runtime_error>([&datum] { datum.SetFromString<glm::mat4>("bleh"s); });
			}

			//string test
			{
				Datum datum(DatumType::String);
				datum.Resize(5, true);
				datum.SetFromString<string>("I'm tired"s);
				Assert::AreEqual(datum.Get<string>(), "I'm tired"s);
			}
		}

	private:
		static _CrtMemState sStartMemState;	//for memory leak detection
	};
	_CrtMemState DatumTests::sStartMemState;
}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       