#pragma once
#include <glm/glm.hpp>
#include <string>
#include "RTTI.h"

namespace Library
{
	enum class DatumType
	{
		Unknown,
		Integer,
		Float,
		Vector,
		Matrix,
		Table, 
		String,
		Pointer,

		Begin = Integer,
		End = Pointer
	};

	class Scope; //forward declaration
	class Attributed; //forward declaration

	class Datum final
	{
		friend Scope;
		friend Attributed;
	public:
		/// <summary>
		/// Default constructor: Initialized to be empty, type unknown
		/// </summary>
		Datum();

		/// <summary>
		/// Copy Constructor
		/// If rhs has external storage, this does a shallow copy.
		/// If rhs has internal storage, does a deep copy
		/// </summary>
		/// <param name="rhs">the datum to copy</param>
		Datum(const Datum& rhs);

		/// <summary>
		/// Move copy constructor
		/// </summary>
		/// <param name="rhs">the datum to move from</param>
		Datum(Datum&& rhs) noexcept;

		/// <summary>
		/// Constructor: set the type and reserve memory
		/// </summary>
		/// <param name="type">The type to set it to</param>
		/// <param name="capacity">How many elements to reserve memory for (default = 0)</param>
		Datum(DatumType type, size_t capacity = 0);

		/// <summary>
		/// Destructor
		/// </summary>
		~Datum();

		/// <summary>
		/// Copy Assignment Operator
		/// If rhs has external storage, this does a shallow copy.
		/// If rhs has internal storage, does a deep copy
		/// </summary>
		/// <param name="rhs">the datum to copy</param>
		/// <returns>A copy of rhs</returns>
		Datum& operator=(const Datum& rhs);

		/// <summary>
		/// Move Assignment operator
		/// </summary>
		/// <param name="rhs">the datum to move data from</param>
		/// <returns>a reference to this datum</returns>
		Datum& operator=(Datum&& rhs) noexcept;

		/// <summary>
		/// Scalar Assignment Operator:
		/// Sets the type to Integer, capacity and size to 1, allocates memory, and sets the first element equal to value
		/// </summary>
		/// <param name="value">the value to store in this datum</param>
		/// <returns>A datum with type Integer, capacity and size of 1, and contains value</returns>
		/// <exception cref="std::runtime_error">Throws exception is type is not Unknown or Integer</exception>
		Datum& operator=(int value);

		/// <summary>
		/// Scalar Assignment Operator:
		/// Sets the type to Float, capacity and size to 1, allocates memory, and sets the first element equal to value
		/// </summary>
		/// <param name="value">the value to store in this datum</param>
		/// <returns>A datum with type float, capacity and size of 1, and contains value</returns>
		/// <exception cref="std::runtime_error">Throws exception is type is not Unknown or Float</exception>
		Datum& operator=(float value);

		/// <summary>
		/// Scalar Assignment Operator:
		/// Sets the type to Vector, capacity and size to 1, allocates memory, and sets the first element equal to value
		/// </summary>
		/// <param name="value">the value to store in this datum</param>
		/// <returns>A datum with type Vector, capacity and size of 1, and contains value</returns>
		/// <exception cref="std::runtime_error">Throws exception is type is not Unknown or Vector</exception>
		Datum& operator=(const glm::vec4& value);

		/// <summary>
		/// Scalar Assignment Operator:
		/// Sets the type to Matrix, capacity and size to 1, allocates memory, and sets the first element equal to value
		/// </summary>
		/// <param name="value">the value to store in this datum</param>
		/// <returns>A datum with type Matrix, capacity and size of 1, and contains value</returns>
		/// <exception cref="std::runtime_error">Throws exception is type is not Unknown or Matrix</exception>
		Datum& operator=(const glm::mat4& value);

		/// <summary>
		/// Scalar Assignment Operator:
		/// Sets the type to String, capacity and size to 1, allocates memory, and sets the first element equal to value
		/// </summary>
		/// <param name="value">the value to store in this datum</param>
		/// <returns>A datum with type String, capacity and size of 1, and contains value</returns>
		/// <exception cref="std::runtime_error">Throws exception is type is not Unknown or String</exception>
		Datum& operator=(const std::string& value);

		/// <summary>
		/// Scalar Assignment Operator:
		/// Sets the type to Pointer, capacity and size to 1, allocates memory, and sets the first element equal to value
		/// </summary>
		/// <param name="value">the value to store in this datum</param>
		/// <returns>A datum with type Pointer, capacity and size of 1, and contains value</returns>
		/// <exception cref="std::runtime_error">Throws exception is type is not Unknown or Pointer</exception>
		Datum& operator=(RTTI* value);

		/// <summary>
		/// Comparison, returns true if the type, capacity, size, and contents are equal, false otherwise
		/// </summary>
		/// <param name="rhs">the datum to compare to</param>
		/// <returns>true if the type, capacity, size, and contents are equal, false otherwise</returns>
		bool operator==(const Datum& rhs) const noexcept;

		/// <summary>
		/// Treats the datum as a scalar integer and compares it to the given value
		/// </summary>
		/// <param name="value">the value to compare to</param>
		/// <returns>
		/// True if Datum is scalar and equals value.
		/// False if Datum is not scalar (has more than one element), if Datum is wrong type, or if it does not equal value.
		/// </returns>
		bool operator==(int value) const noexcept;

		/// <summary>
		/// Treats the datum as a scalar float and compares it to the given value
		/// </summary>
		/// <param name="value">the value to compare to</param>
		/// <returns>
		/// True if Datum is scalar and equals value.
		/// False if Datum is not scalar (has more than one element), if Datum is wrong type, or if it does not equal value.
		/// </returns>
		bool operator==(float value) const noexcept;

		/// <summary>
		/// Treats the datum as a scalar vector and compares it to the given value
		/// </summary>
		/// <param name="value">the value to compare to</param>
		/// <returns>
		/// True if Datum is scalar and equals value.
		/// False if Datum is not scalar (has more than one element), if Datum is wrong type, or if it does not equal value.
		/// </returns>
		bool operator==(const glm::vec4& value) const noexcept;

		/// <summary>
		/// Treats the datum as a scalar matrix and compares it to the given value
		/// </summary>
		/// <param name="value">the value to compare to</param>
		/// <returns>
		/// True if Datum is scalar and equals value.
		/// False if Datum is not scalar (has more than one element), if Datum is wrong type, or if it does not equal value.
		/// </returns>
		bool operator==(const glm::mat4& value) const noexcept;

		/// <summary>
		/// Treats the datum as a scalar string and compares it to the given value
		/// </summary>
		/// <param name="value">the value to compare to</param>
		/// <returns>
		/// True if Datum is scalar and equals value.
		/// False if Datum is not scalar (has more than one element), if Datum is wrong type, or if it does not equal value.
		/// </returns>
		bool operator==(const std::string& value) const noexcept;

		/// <summary>
		/// Treats the datum as a scalar RTTI* and compares it to the given value
		/// </summary>
		/// <param name="value">the value to compare to</param>
		/// <returns>
		/// True if Datum is scalar and equals value.
		/// False if Datum is not scalar (has more than one element), if Datum is wrong type, or if it does not equal value.
		/// </returns>
		bool operator==(RTTI* value) const noexcept;

		/// <summary>
		/// Comparison, returns false if the type, capacity, size, and contents are equal, true otherwise
		/// </summary>
		/// <param name="rhs">the datum to compare to</param>
		/// <returns>false if the type, capacity, size, and contents are equal, true otherwise</returns>
		bool operator!=(const Datum& rhs) const noexcept;

		/// <summary>
		/// Treats the datum as a scalar integer and compares it to the given value
		/// </summary>
		/// <param name="value">the value to compare to</param>
		/// <returns>
		/// False if Datum is scalar and equals value.
		/// True if Datum is not scalar (has more than one element), if Datum is wrong type, or if it does not equal value.
		/// </returns>
		bool operator!=(int value) const noexcept;

		/// <summary>
		/// Treats the datum as a scalar float and compares it to the given value
		/// </summary>
		/// <param name="value">the value to compare to</param>
		/// <returns>
		/// False if Datum is scalar and equals value.
		/// True if Datum is not scalar (has more than one element), if Datum is wrong type, or if it does not equal value.
		/// </returns>
		bool operator!=(float value) const noexcept;

		/// <summary>
		/// Treats the datum as a scalar vector and compares it to the given value
		/// </summary>
		/// <param name="value">the value to compare to</param>
		/// <returns>
		/// False if Datum is scalar and equals value.
		/// True if Datum is not scalar (has more than one element), if Datum is wrong type, or if it does not equal value.
		/// </returns>
		bool operator!=(const glm::vec4& value) const noexcept;

		/// <summary>
		/// Treats the datum as a scalar matrix and compares it to the given value
		/// </summary>
		/// <param name="value">the value to compare to</param>
		/// <returns>
		/// False if Datum is scalar and equals value.
		/// True if Datum is not scalar (has more than one element), if Datum is wrong type, or if it does not equal value.
		/// </returns>
		bool operator!=(const glm::mat4& value) const noexcept;

		/// <summary>
		/// Treats the datum as a scalar string and compares it to the given value
		/// </summary>
		/// <param name="value">the value to compare to</param>
		/// <returns>
		/// False if Datum is scalar and equals value.
		/// True if Datum is not scalar (has more than one element), if Datum is wrong type, or if it does not equal value.
		/// </returns>
		bool operator!=(const std::string& value) const noexcept;

		/// <summary>
		/// Treats the datum as a scalar RTTI* and compares it to the given value
		/// </summary>
		/// <param name="value">the value to compare to</param>
		/// <returns>
		/// False if Datum is scalar and equals value.
		/// True if Datum is not scalar (has more than one element), if Datum is wrong type, or if it does not equal value.
		/// </returns>
		bool operator!=(RTTI* value) const noexcept;

		/// <summary>
		/// Get the type associated with this Datum
		/// </summary>
		/// <returns>The type associated with this Datum</returns>
		DatumType Type() const noexcept;

		/// <summary>
		/// Get the size of the Datum (the number of elements currently held)
		/// </summary>
		/// <returns>The size of the Datum (the number of elements currently held)</returns>
		size_t Size() const noexcept;

		/// <summary>
		/// Get the capacity of the Datum (how many elements that memory is allocated for)
		/// </summary>
		/// <returns>the capacity of the Datum (how many elements that memory is allocated for)</returns>
		size_t Capacity() const noexcept;

		bool IsEmpty() const noexcept;

		/// <summary>
		/// Set the type of this Datum
		/// </summary>
		/// <param name="type">The type of data this Datum will hold</param>
		/// <exception cref="std::runtime_error">Throws exception if type is already set or if type is invalid</exception>
		void SetType(const DatumType type);

		/// <summary>
		/// Allocates memory for capacity elements
		/// Will not shrink the array. If called with a lower capacity, nothing happens.
		/// </summary>
		/// <param name="capacity">the number of elements to allocate memory for</param>
		/// <exception cref="std::runtime_error">Throws exception if type is not set or if storage is external</exception>
		void Reserve(size_t capacity);

		/// <summary>
		/// Resizes the capacity to the given size. 
		/// If defaultConstruct is true, will default construct items so that Size = Capacity
		/// </summary>
		/// <param name="capacity">the number of elements to allocate memory for</param>
		/// <param name="defaultConstruct">Whether or not to default construct items</param>
		/// <exception cref="std::runtime_error">Throws exception if type is not set or if storage is external</exception>
		void Resize(size_t capacity, bool defaultConstruct = false);

		/// <summary>
		/// Clear the array without shrinking the capacity
		/// </summary>
		void Clear();

		/// <summary>
		/// Sets the internal values array to the given array, and the size and capacity to the given number of elements.
		/// Marks the storage as external
		/// </summary>
		/// <param name="array">the external storage array</param>
		/// <param name="size">the number of elements in the array</param>
		/// <remarks>Once storage is marked as external, any functions that change size or capacity cannot be called.</remarks>
		/// <exception cref="std::runtime_error">Throws exception if type does not match, if storage is external, or if memory is already allocated</exception>
		void SetStorage(int* array, size_t size);

		/// <summary>
		/// Sets the internal values array to the given array, and the size and capacity to the given number of elements.
		/// Marks the storage as external
		/// </summary>
		/// <param name="array">the external storage array</param>
		/// <param name="size">the number of elements in the array</param>
		/// <remarks>Once storage is marked as external, any functions that change size or capacity cannot be called.</remarks>
		/// <exception cref="std::runtime_error">Throws exception if type does not match, if storage is external, or if memory is already allocated</exception>
		void SetStorage(float* array, size_t size);

		/// <summary>
		/// Sets the internal values array to the given array, and the size and capacity to the given number of elements.
		/// Marks the storage as external
		/// </summary>
		/// <param name="array">the external storage array</param>
		/// <param name="size">the number of elements in the array</param>
		/// <remarks>Once storage is marked as external, any functions that change size or capacity cannot be called.</remarks>
		/// <exception cref="std::runtime_error">Throws exception if type does not match, if storage is external, or if memory is already allocated</exception>
		void SetStorage(glm::vec4* array, size_t size);

		/// <summary>
		/// Sets the internal values array to the given array, and the size and capacity to the given number of elements.
		/// Marks the storage as external
		/// </summary>
		/// <param name="array">the external storage array</param>
		/// <param name="size">the number of elements in the array</param>
		/// <remarks>Once storage is marked as external, any functions that change size or capacity cannot be called.</remarks>
		/// <exception cref="std::runtime_error">Throws exception if type does not match, if storage is external, or if memory is already allocated</exception>
		void SetStorage(glm::mat4* array, size_t size);

		/// <summary>
		/// Sets the internal values array to the given array, and the size and capacity to the given number of elements.
		/// Marks the storage as external
		/// </summary>
		/// <param name="array">the external storage array</param>
		/// <param name="size">the number of elements in the array</param>
		/// <remarks>Once storage is marked as external, any functions that change size or capacity cannot be called.</remarks>
		/// <exception cref="std::runtime_error">Throws exception if type does not match, if storage is external, or if memory is already allocated</exception>
		void SetStorage(std::string* array, size_t size);

		/// <summary>
		/// Sets the internal values array to the given array, and the size and capacity to the given number of elements.
		/// Marks the storage as external
		/// </summary>
		/// <param name="array">the external storage array</param>
		/// <param name="size">the number of elements in the array</param>
		/// <remarks>Once storage is marked as external, any functions that change size or capacity cannot be called.</remarks>
		/// <exception cref="std::runtime_error">Throws exception if type does not match, if storage is external, or if memory is already allocated</exception>
		void SetStorage(RTTI** array, size_t size);

		/// <summary>
		/// Template for Get
		/// </summary>
		template<typename T>
		T& Get(size_t index = 0);

		/// <summary>
		/// Template for get (const)
		/// </summary>
		template<typename T>
		const T& Get(size_t index = 0) const;

		/// <summary>
		/// Gets integer at index
		/// </summary>
		/// <returns>The integer at the given index</returns>
		/// <exception cref="std::runtime_error">Throws exception if type is not Integer or if index is out of bounds</exception>
		template<>
		int& Get<int>(size_t index);

		/// <summary>
		/// Gets integer at index (const)
		/// </summary>
		/// <returns>The integer at the given index (const)</returns>
		/// <exception cref="std::runtime_error">Throws exception if type is not Integer or if index is out of bounds</exception>
		template<>
		const int& Get<int>(size_t index) const;

		/// <summary>
		/// Gets float at index
		/// </summary>
		/// <returns>The float at the given index</returns>
		/// <exception cref="std::runtime_error">Throws exception if type is not float or if index is out of bounds</exception>
		template<>
		float& Get<float>(size_t index);

		/// <summary>
		/// Gets float at index (const)
		/// </summary>
		/// <returns>The float at the given index (const)</returns>
		/// <exception cref="std::runtime_error">Throws exception if type is not float or if index is out of bounds</exception>
		template<>
		const float& Get<float>(size_t index) const;

		/// <summary>
		/// Gets vector at index
		/// </summary>
		/// <returns>The vector at the given index</returns>
		/// <exception cref="std::runtime_error">Throws exception if type is not vector or if index is out of bounds</exception>
		template<>
		glm::vec4& Get<glm::vec4>(size_t index);

		/// <summary>
		/// Gets vector at index (const)
		/// </summary>
		/// <returns>The vector at the given index (const)</returns>
		/// <exception cref="std::runtime_error">Throws exception if type is not vector or if index is out of bounds</exception>
		template<>
		const glm::vec4& Get<glm::vec4>(size_t index) const;

		/// <summary>
		/// Gets matrix at index
		/// </summary>
		/// <returns>The matrix at the given index</returns>
		/// <exception cref="std::runtime_error">Throws exception if type is not matrix or if index is out of bounds</exception>
		template<>
		glm::mat4& Get<glm::mat4>(size_t index);

		/// <summary>
		/// Gets matrix at index (const)
		/// </summary>
		/// <returns>The matrix at the given index (const)</returns>
		/// <exception cref="std::runtime_error">Throws exception if type is not matrix or if index is out of bounds</exception>
		template<>
		const glm::mat4& Get<glm::mat4>(size_t index) const;

		/// <summary>
		/// Gets table at index
		/// </summary>
		/// <returns>The table at the given index</returns>
		/// <exception cref="std::runtime_error">Throws exception if type is not table or if index is out of bounds</exception>
		template<>
		Scope& Get<Scope>(size_t index);

		/// <summary>
		/// Gets table at index (const)
		/// </summary>
		/// <returns>The table at the given index (const)</returns>
		/// <exception cref="std::runtime_error">Throws exception if type is not table or if index is out of bounds</exception>
		template<>
		const Scope& Get<Scope>(size_t index) const;

		/// <summary>
		/// Gets string at index
		/// </summary>
		/// <returns>The string at the given index</returns>
		/// <exception cref="std::runtime_error">Throws exception if type is not string or if index is out of bounds</exception>
		template<>
		std::string& Get<std::string>(size_t index);

		/// <summary>
		/// Gets string at index (const)
		/// </summary>
		/// <returns>The string at the given index (const)</returns>
		/// <exception cref="std::runtime_error">Throws exception if type is not string or if index is out of bounds</exception>
		template<>
		const std::string& Get<std::string>(size_t index) const;

		/// <summary>
		/// Gets RTTI* at index
		/// </summary>
		/// <returns>The RTTI* at the given index</returns>
		/// <exception cref="std::runtime_error">Throws exception if type is not pointer or if index is out of bounds</exception>
		template<>
		RTTI*& Get<RTTI*>(size_t index);

		/// <summary>
		/// Gets RTTI* at index (const)
		/// </summary>
		/// <returns>The RTTI* at the given index</returns>
		/// <exception cref="std::runtime_error">Throws exception if type is not pointer or if index is out of bounds</exception>
		template<>
		RTTI* const& Get<RTTI*>(size_t index) const;

		/// <summary>
		/// Sets the element at the given index to the given value
		/// </summary>
		/// <param name="value">the value to put in the datum</param>
		/// <param name="index">the location to put the value</param>
		/// <exception cref="std::runtime_error">Throws exception if type is not integer or if index is out of bounds</exception>
		void Set(int value, size_t index = 0);

		/// <summary>
		/// Sets the element at the given index to the given value
		/// </summary>
		/// <param name="value">the value to put in the datum</param>
		/// <param name="index">the location to put the value</param>
		/// <exception cref="std::runtime_error">Throws exception if type is not float or if index is out of bounds</exception>
		void Set(float value, size_t index = 0);

		/// <summary>
		/// Sets the element at the given index to the given value
		/// </summary>
		/// <param name="value">the value to put in the datum</param>
		/// <param name="index">the location to put the value</param>
		/// <exception cref="std::runtime_error">Throws exception if type is not vector or if index is out of bounds</exception>
		void Set(const glm::vec4& value, size_t index = 0);

		/// <summary>
		/// Sets the element at the given index to the given value
		/// </summary>
		/// <param name="value">the value to put in the datum</param>
		/// <param name="index">the location to put the value</param>
		/// <exception cref="std::runtime_error">Throws exception if type is not matrix or if index is out of bounds</exception>
		void Set(const glm::mat4& value, size_t index = 0);

		/// <summary>
		/// Sets the element at the given index to the given value
		/// </summary>
		/// <param name="value">the value to put in the datum</param>
		/// <param name="index">the location to put the value</param>
		/// <exception cref="std::runtime_error">Throws exception if type is not string or if index is out of bounds</exception>
		void Set(const std::string& value, size_t index = 0);

		/// <summary>
		/// Sets the element at the given index to the given value
		/// </summary>
		/// <param name="value">the value to put in the datum</param>
		/// <param name="index">the location to put the value</param>
		/// <exception cref="std::runtime_error">Throws exception if type is not pointer or if index is out of bounds</exception>
		void Set(RTTI* value, size_t index = 0);

		/// <summary>
		/// Push the given data into the array.
		/// If datum is full, increases the capacity
		/// Increment function: Add half the current capacity + 1 each capacity increment
		/// </summary>
		/// <param name="data">the data to push into the datum</param>
		/// <exception cref="std::runtime_error">Throws exception if type is not int or unknown</exception>
		/// <remarks>If datum's type is not set, it will set the datums type to int</remarks>
		void PushBack(int data);

		/// <summary>
		/// Push the given data into the array.
		/// If datum is full, increases the capacity
		/// Increment function: Add half the current capacity + 1 each capacity increment
		/// </summary>
		/// <param name="data">the data to push into the datum</param>
		/// <exception cref="std::runtime_error">Throws exception if type is not float or unknown</exception>
		/// <remarks>If datum's type is not set, it will set the datums type to float</remarks>
		void PushBack(float data);

		/// <summary>
		/// Push the given data into the array.
		/// If datum is full, increases the capacity
		/// Increment function: Add half the current capacity + 1 each capacity increment
		/// </summary>
		/// <param name="data">the data to push into the datum</param>
		/// <exception cref="std::runtime_error">Throws exception if type is not vector or unknown</exception>
		/// <remarks>If datum's type is not set, it will set the datums type to vector</remarks>
		void PushBack(const glm::vec4& data);

		/// <summary>
		/// Push the given data into the array.
		/// If datum is full, increases the capacity
		/// Increment function: Add half the current capacity + 1 each capacity increment
		/// </summary>
		/// <param name="data">the data to push into the datum</param>
		/// <exception cref="std::runtime_error">Throws exception if type is not matrix or unknown</exception>
		/// <remarks>If datum's type is not set, it will set the datums type to matrix</remarks>
		void PushBack(const glm::mat4& data);

		/// <summary>
		/// Push the given data into the array.
		/// If datum is full, increases the capacity
		/// Increment function: Add half the current capacity + 1 each capacity increment
		/// </summary>
		/// <param name="data">the data to push into the datum</param>
		/// <exception cref="std::runtime_error">Throws exception if type is not string or unknown</exception>
		/// <remarks>If datum's type is not set, it will set the datums type to string</remarks>
		void PushBack(const std::string& data);

		/// <summary>
		/// Push the given data into the array.
		/// If datum is full, increases the capacity
		/// Increment function: Add half the current capacity + 1 each capacity increment
		/// </summary>
		/// <param name="data">the data to push into the datum</param>
		/// <exception cref="std::runtime_error">Throws exception if type is not pointer or unknown</exception>
		/// <remarks>If datum's type is not set, it will set the datums type to pointer</remarks>
		void PushBack(RTTI* data);

		/// <summary>
		/// Removes the last element in the datum without shrinking capacity
		/// If datum is empty, does nothing
		/// </summary>
		/// <remarks>
		/// ***IMPORTANT NOTE ABOUT POPBACK WITH TABLE/SCOPE***
		/// Popback does not call orphan on a scope, so if you popback a child scope, this may cause major memory leaks.
		/// </remarks>
		void PopBack();

		/// <summary>
		/// Searches for the given value. If found, returns the index. 
		/// If not found, returns size (an out of bounds index)
		/// </summary>
		/// <param name="value">the value to search for</param>
		/// <returns>The index pointing to value, or Size if value not found</returns>
		///  <exception cref="std::runtime_error">Throws exception if type does not match</exception>
		size_t Find(int value) const;

		/// <summary>
		/// Searches for the given value. If found, returns the index. 
		/// If not found, returns size (an out of bounds index)
		/// </summary>
		/// <param name="value">the value to search for</param>
		/// <returns>The index pointing to value, or Size if value not found</returns>
		///  <exception cref="std::runtime_error">Throws exception if type does not match</exception>
		size_t Find(float value) const;

		/// <summary>
		/// Searches for the given value. If found, returns the index. 
		/// If not found, returns size (an out of bounds index)
		/// </summary>
		/// <param name="value">the value to search for</param>
		/// <returns>The index pointing to value, or Size if value not found</returns>
		///  <exception cref="std::runtime_error">Throws exception if type does not match</exception>
		size_t Find(const glm::vec4& value) const;

		/// <summary>
		/// Searches for the given value. If found, returns the index. 
		/// If not found, returns size (an out of bounds index)
		/// </summary>
		/// <param name="value">the value to search for</param>
		/// <returns>The index pointing to value, or Size if value not found</returns>
		///  <exception cref="std::runtime_error">Throws exception if type does not match</exception>
		size_t Find(const glm::mat4& value) const;

		/// <summary>
		/// Searches for the given value. If found, returns the index. 
		/// If not found, returns size (an out of bounds index)
		/// </summary>
		/// <param name="value">the value to search for</param>
		/// <returns>The index pointing to value, or Size if value not found</returns>
		///  <exception cref="std::runtime_error">Throws exception if type does not match</exception>
		size_t Find(const Scope& value) const;

		/// <summary>
		/// Searches for the given value. If found, returns the index. 
		/// If not found, returns size (an out of bounds index)
		/// </summary>
		/// <param name="value">the value to search for</param>
		/// <returns>The index pointing to value, or Size if value not found</returns>
		///  <exception cref="std::runtime_error">Throws exception if type does not match</exception>
		size_t Find(const std::string& value) const;

		/// <summary>
		/// Searches for the given value. If found, returns the index. 
		/// If not found, returns size (an out of bounds index)
		/// </summary>
		/// <param name="value">the value to search for</param>
		/// <returns>The index pointing to value, or Size if value not found</returns>
		///  <exception cref="std::runtime_error">Throws exception if type does not match</exception>
		size_t Find(RTTI* value) const;

		/// <summary>
		/// Removes the element at the given index
		/// </summary>
		/// <param name="index"></param>
		/// <returns>
		/// True if element at index was removed
		/// False if:
		///		-Index is out of bounds
		///		-Type is not set
		/// </returns>
		/// /// <remarks>
		/// ***IMPORTANT NOTE ABOUT REMOVEAT WITH TABLE/SCOPE***
		/// Popback does not call orphan on a scope, so if you popback a child scope, this may cause major memory leaks.
		/// </remarks>
		bool RemoveAt(size_t index);

		/// <summary>
		/// Searches for and removes given value and returns a bool indicating whether the value was removed.
		/// </summary>
		/// <param name="value">the value to remove from datum</param>
		/// <returns> True if the element at index was removed, False if item does not exist </returns>
		/// <exception cref="std::runtime_error">Throws exception if type does not match</exception>
		bool Remove(int value);

		/// <summary>
		/// Searches for and removes given value and returns a bool indicating whether the value was removed.
		/// </summary>
		/// <param name="value">the value to remove from datum</param>
		/// <returns> True if the element at index was removed, False if item does not exist </returns>
		/// <exception cref="std::runtime_error">Throws exception if type does not match</exception>
		bool Remove(float value);

		/// <summary>
		/// Searches for and removes given value and returns a bool indicating whether the value was removed.
		/// </summary>
		/// <param name="value">the value to remove from datum</param>
		/// <returns> True if the element at index was removed, False if item does not exist </returns>
		/// <exception cref="std::runtime_error">Throws exception if type does not match</exception>
		bool Remove(const glm::vec4& value);

		/// <summary>
		/// Searches for and removes given value and returns a bool indicating whether the value was removed.
		/// </summary>
		/// <param name="value">the value to remove from datum</param>
		/// <returns> True if the element at index was removed, False if item does not exist </returns>
		/// <exception cref="std::runtime_error">Throws exception if type does not match</exception>
		bool Remove(const glm::mat4& value);

		/// <summary>
		/// Searches for and removes given value and returns a bool indicating whether the value was removed.
		/// </summary>
		/// <param name="value">the value to remove from datum</param>
		/// <returns> True if the element at index was removed, False if item does not exist </returns>
		/// <exception cref="std::runtime_error">Throws exception if type does not match</exception>
		bool Remove(const std::string& value);

		/// <summary>
		/// Searches for and removes given value and returns a bool indicating whether the value was removed.
		/// </summary>
		/// <param name="value">the value to remove from datum</param>
		/// <returns> True if the element at index was removed, False if item does not exist </returns>
		/// <exception cref="std::runtime_error">Throws exception if type does not match</exception>
		bool Remove(RTTI* value);

		/// <summary>
		/// Returns a string representing the data at the given index (default index = 0)
		/// </summary>
		/// <param name="index">the index to get the value from</param>
		/// <returns>A string representing the data at the given index</returns>
		/// <exception cref="std::runtime_error">Throws exception if type is not set or index is out of bounds</exception>
		std::string ToString(size_t index = 0) const;

		void SetFromStringDanger(const std::string& str, size_t index = 0);

		/// <summary>
		/// Template for set from string
		/// </summary>
		template<typename T>
		void SetFromString(const std::string& str, size_t index = 0);

		/// <summary>
		/// Sets the value at the given index to the value parsed from str 
		/// </summary>
		/// <param name="str">the string representing the value to add</param>
		/// <param name="index">the index to get the value from</param>
		/// <exception cref="std::runtime_error">Throws exception if type is not set or index is out of bounds, string can't be parsed, or type is wrong</exception>
		template<>
		void SetFromString<int>(const std::string& str, size_t index);

		/// <summary>
		/// Sets the value at the given index to the value parsed from str 
		/// </summary>
		/// <param name="str">the string representing the value to add</param>
		/// <param name="index">the index to get the value from</param>
		/// <exception cref="std::runtime_error">Throws exception if type is not set or index is out of bounds, string can't be parsed, or type is wrong</exception>
		template<>
		void SetFromString<float>(const std::string& str, size_t index);

		/// <summary>
		/// Sets the value at the given index to the value parsed from str 
		/// </summary>
		/// <param name="str">the string representing the value to add</param>
		/// <param name="index">the index to get the value from</param>
		/// <exception cref="std::runtime_error">Throws exception if type is not set or index is out of bounds, string can't be parsed, or type is wrong</exception>
		template<>
		void SetFromString<glm::vec4>(const std::string& str, size_t index);

		/// <summary>
		/// Sets the value at the given index to the value parsed from str 
		/// </summary>
		/// <param name="str">the string representing the value to add</param>
		/// <param name="index">the index to get the value from</param>
		/// <exception cref="std::runtime_error">Throws exception if type is not set or index is out of bounds, string can't be parsed, or type is wrong</exception>
		template<>
		void SetFromString<glm::mat4>(const std::string& str, size_t index);

		/// <summary>
		/// Sets the value at the given index to the value parsed from str 
		/// </summary>
		/// <param name="str">the string representing the value to add</param>
		/// <param name="index">the index to get the value from</param>
		/// <exception cref="std::runtime_error">Throws exception if type is not set or index is out of bounds, string can't be parsed, or type is wrong</exception>
		template<>
		void SetFromString<std::string>(const std::string& str, size_t index);

		/// <summary>
		/// Gets a reference to the scope at the given index
		/// </summary>
		/// <param name="index">the index the scope is at</param>
		/// <returns></returns>
		/// <exception cref="std::runtime_error">Throws exception if index is out of bounds or if type is not table</exception>
		Scope& operator[](size_t index);
	
	private:
		//The kinds of data Datum can hold
		union DatumValues
		{
			int* i;
			float* f;
			glm::vec4* v;
			glm::mat4* m;
			Scope** t; 
			std::string* s;
			RTTI** p;
			void* vo;
			uint8_t* byte;
		};

		//The sizes of each kind of data
		inline static const size_t DatumSizes[] =
		{
			0,
			sizeof(int),
			sizeof(float),
			sizeof(glm::vec4),
			sizeof(glm::mat4),
			sizeof(Scope*),
			sizeof(std::string),
			sizeof(RTTI*)
		};

		DatumValues mData;
		size_t mSize = 0;
		size_t mCapacity = 0;
		bool mIsExternal = false;
		DatumType mType = DatumType::Unknown;

		/// <summary>
		/// Helper for AttributedScope. Type must be set before calling this. 
		/// </summary>
		/// <param name="array"></param>
		/// <param name="size"></param>
		void SetStorage(void* array, size_t size);

		/// <summary>
		/// Scalar Assignment Operator:
		/// Sets the type to Table, capacity and size to 1, allocates memory, and sets the first element equal to value
		/// </summary>
		/// <param name="value">the value to store in this datum</param>
		/// <returns>A datum with type Pointer, capacity and size of 1, and contains value</returns>
		/// <exception cref="std::runtime_error">Throws exception is type is not Unknown or Table</exception>
		Datum& operator=(Scope& value);

		/// <summary>
		/// Sets the element at the given index to the given value
		/// </summary>
		/// <param name="value">the value to put in the datum</param>
		/// <param name="index">the location to put the value</param>
		/// <exception cref="std::runtime_error">Throws exception if type is not table or if index is out of bounds</exception>
		void Set(Scope& value, size_t index = 0);

		/// <summary>
		/// Push the given data into the array.
		/// If datum is full, increases the capacity
		/// Increment function: Add half the current capacity + 1 each capacity increment
		/// </summary>
		/// <param name="data">the data to push into the datum</param>
		/// <exception cref="std::runtime_error">Throws exception if type is not table or unknown</exception>
		/// <remarks>If datum's type is not set, it will set the datums type to table</remarks>
		void PushBack(Scope& data);


		/************************************************************************/
		/**************************Helper Functions******************************/
		/************************************************************************/
		/// <summary>
		/// Reallocs data (uses mType to decide size of elements) and sets Capacity
		/// </summary>
		/// <param name="capacity">The capacity to realloc data for</param>
		void ReallocData(size_t capacity);

		/// <summary>
		/// Throws exception if datum is not type specified
		/// </summary>
		/// <param name="type">The type to compare to</param>
		void TypeCheck(DatumType type) const;

		/// <summary>
		/// Checks that datum matches specified type or that type hasn't been set yet
		/// (throws exception if mType != type or mType != unknown)
		/// </summary>
		/// <param name="type">the type to compare to</param>
		void TypeCheckOrUnknown(DatumType type) const;

		/// <summary>
		/// Throws exception if datum is not type specified
		/// </summary>
		/// <param name="type">The type to compare to</param>
		void CheckTypeHasBeenSet() const;

		/// <summary>
		/// throws exception if index is >= mSize
		/// </summary>
		/// <param name="index">the index to check</param>
		void VerifyIndexInBounds(size_t index) const;

		/// <summary>
		/// Helper function for Pushback()
		/// Increment capacity if size = capacity
		/// Increment function: Add half the current capacity + 1 each capacity increment
		/// </summary>
		void IncrementCapacityIfFull();

		/// <summary>
		/// helper function for set storage
		/// Checks for exceptions (wrong type, memory already allocated, array nullptr)
		/// Sets isExternal to true, and size and capacity to given size
		/// </summary>
		/// <param name="array">the array to check if nullptr or not</param>
		/// <param name="type">the type of datum storage is being set for</param>
		/// <param name="size">the size of the array</param>
		void SetStorageHelper(void* array, DatumType type, size_t size);

		void ExternalException();
	};
}

#include "Datum.inl"