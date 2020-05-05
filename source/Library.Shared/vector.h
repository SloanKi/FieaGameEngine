#pragma once
#include <initializer_list>

namespace Library
{
	template <typename T>
	class Vector
	{
	public:
		using value_type = T;

		class Iterator;			//forward declaration because I don't want the class all the way up here.
		class ConstIterator;	//forward declaration because I don't want the class all the way up here.
		static const size_t DEFAULT_CAPACITY = 5;	//default capacity, this is set after the first PushBack

		/// <summary>
		/// Default Constructor
		/// </summary>
		Vector() = default;

		/// <summary>
		/// Initializer list Constructor
		/// </summary>
		/// <param name="list">the initializer list to use to construct this vector</param>
		Vector(std::initializer_list<T> list);

		/// <summary>
		/// Copy Constructor: Deep-copy the list given as argument into this list
		/// </summary>
		/// <param name="rhs"> The list to deep-copy into this list </param>
		Vector(const Vector& rhs);

		/// <summary>
		/// Move constructor. Shallow copies the Vector and invalidates the RHS
		/// </summary>
		/// <param name="rhs">R-Value reference of the vector to move the memory from</param>
		Vector(Vector&& rhs) noexcept;

		/// <summary>
		/// Constructor: Initialize the list with given capacity
		/// </summary>
		/// <param name="capacity">the number of elements to allocate memory for</param>
		Vector(size_t capacity);

		/// <summary>
		/// Destructor: destructs every element in the list, then frees the memory.
		/// </summary>
		virtual ~Vector();

		/// <summary>
		/// Assignment Operator: Deep-copy the list given as argument into this list
		/// </summary>
		/// <param name="rhs"> The list to deep-copy into this list </param>
		/// <returns>A reference to the deep-copy of rhs</returns>
		Vector<T>& operator=(const Vector& rhs);

		/// <summary>
		/// Move assignment operator. Shallow copies the Vector and invalidates the rhs.
		/// </summary>
		/// <param name="rhs">R-Value reference of the Vector to move the memory from</param>
		/// <returns>Reference to the new Vector created by moving rhs mem</returns>
		/// <exception cref="std::runtime_error">Throws exception if rhs is the same as this list</exception>
		Vector<T>& operator=(Vector&& rhs);

		bool operator==(const Vector & rhs) const noexcept;
		bool operator!=(const Vector & rhs) const noexcept;

		/// <summary>
		/// Array index operator: returns a reference to the item located at the given index
		/// </summary>
		/// <param name="index">the index to get the item from</param>
		/// <exception cref="std::runtime_error">Throws exception if the given index is invalid (out of bounds)</exception>
		/// <returns>a reference to the item located at the given index</returns>
		T& operator[](size_t index);

		/// <summary>
		/// Array index operator (const): returns a const reference to the item located at the given index
		/// </summary>
		/// <param name="index">the index to get the item from</param>
		/// <exception cref="std::runtime_error">Throws exception if the given index is invalid (out of bounds)</exception>
		/// <returns>a const reference to the item located at the given index</returns>
		const T& operator[](size_t index) const;

		/// <summary>
		/// Returns the number of items in the vector
		/// </summary>
		/// <returns> mSize (the number of items in the vector) </returns>
		size_t Size() const noexcept;

		/// <summary>
		/// Function to check if list is empty or not
		/// </summary>
		/// <returns>True if list is empty, false if it is not empty</returns>
		bool IsEmpty() const noexcept;

		/// <summary>
		/// Returns the number of items allocated within the vector
		/// </summary>
		/// <returns> mCapacity (the number of items allocated within the vector) </returns>
		size_t Capacity() const noexcept;

		/// <summary>
		/// Reserves memory given with given capacity while preserving any memory already allocated.
		/// </summary>
		/// <param name="capacity">the new desired capacity</param>
		/// <exception cref="std::runtime_error">Throws exception if realloc fails</exception>
		/// <remarks>
		/// Attempting to reserve a smaller capacity results in an early return.
		/// This will never shrink the array. 
		/// </remarks>
		void Reserve(size_t capacity);

		/// <summary>
		/// Resizes the capacity of the list to the given capacity.
		/// Preserves the memory that will fit into the new capacity.
		/// The rest of the data (past capacity) will be deleted.
		/// </summary>
		/// <param name="capacity">the new desired capacity</param>
		/// <exception cref="std::runtime_error">Throws exception if realloc fails</exception>
		void ReCapacity(size_t capacity);

		/// <summary>
		/// Resizes the list to the given size
		/// Preserves the memory that will fit into the new size.
		/// The rest of the data (past size) will be deleted.
		/// If grows, default construct new items to fill to capacity.
		/// </summary>
		/// <param name="capacity">the new desired size</param>
		/// <exception cref="std::runtime_error">Throws exception if realloc fails</exception>
		/// <remarks>At the end of this, mSize = mCapacity </remarks>
		void Resize(size_t size);

		/// <summary>
		/// Pushes the given data to the back of the array. Increases capacity if needed.
		/// </summary>
		/// <param name="data">the data to add to the list</param>
		/// <remarks>
		/// If there is currently no memory allocated, it will set the capacity to DEFAULT_CAPACITY.
		/// Default Capacity Increment: Add mCapacity/2 + 1 to the current capacity.
		/// </remarks>
		void PushBack(const T& data);

		void PushBack(T&& data);

		/// <summary>
		/// Removes the last element of the list, but keeps the capacity the same.
		/// </summary>
		void PopBack();

		/// <summary>
		/// Clears the list of all elements, but keeps the capacity the same.
		/// </summary>
		void Clear();

		/// <summary>
		/// Gets an Iterator pointing to the first element in the list
		/// </summary>
		/// <returns>an Iterator pointing at the first element in the list</returns>
		Iterator begin();

		/// <summary>
		/// Gets a ConstIterator pointing to the first element in the list
		/// </summary>
		/// <returns>an ConstIterator pointing at the first element in the list</returns>
		/// <remarks>calls cbegin()</remarks>
		ConstIterator begin() const;

		/// <summary>
		/// Gets a ConstIterator pointing to the first element in the list
		/// </summary>
		/// <returns>a ConstIterator pointing at the first element in the list</returns>
		ConstIterator cbegin() const;

		/// <summary>
		/// Gets an Iterator pointing to one past the end of the list
		/// </summary>
		/// <returns>an Iterator pointing past the end of the list</returns>
		/// <remarks>past the end of the list means that mIndex = mSize</remarks>
		Iterator end(); 

		/// <summary>
		/// Gets a ConstIterator pointing to one past the end of the list
		/// </summary>
		/// <returns>a ConstIterator pointing past the end of the list</returns>
		/// <remarks>Calls cend()</remarks>
		ConstIterator end() const;

		/// <summary>
		/// Gets a ConstIterator pointing to one past the end of the list
		/// </summary>
		/// <returns>a ConstIterator pointing past the end of the list</returns>
		ConstIterator cend() const;

		/// <summary>
		/// Searches for the value in the list and returns an Iterator pointing to the first location containing value
		/// </summary>
		/// <param name="value">the value to search for</param>
		/// <returns>
		/// If found, returns an Iterator pointing to the location of the data.
		/// If not found, returns end()
		/// </returns>
		Iterator Find(const T& value);

		/// <summary>
		/// Searches for the value in the list and returns a ConstIterator pointing to the first location containing value
		/// </summary>
		/// <param name="value">the value to search for</param>
		/// <returns>
		/// If found, returns a ConstIterator pointing to the data.
		/// If not found, returns cend()
		/// </returns>
		ConstIterator Find(const T& value) const;

		/// <summary>
		/// Removes the item associated with the given data and maintains list integrity.
		/// </summary>
		/// <param name="it">the iterator pointing at the data to remove</param>
		/// <exception cref="std::runtime_error">Throws exception if owner is not this.</exception>
		/// <remarks>Calling remove on a non-existent item immediately returns.</remarks>
		void Remove(const Iterator& it);

	private:
		size_t mSize = 0;		//elements currently in vector
		size_t mCapacity = 0;	//how many elements vector has memory to hold
		T* mData = nullptr;			//array of data


	public:
		class Iterator
		{
			friend Vector;
			friend ConstIterator;

		public:
			using size_type = std::size_t;
			using difference_type = std::ptrdiff_t;
			using value_type = T;
			using reference = T&;
			using pointer = T*;
			//using iterator_category = std::random_access_iterator_tag;
			using iterator_category = std::forward_iterator_tag;

			/// <summary>
			/// Default constructor, sets owner to nullptr and index to 0
			/// </summary>
			Iterator() = default;

			/// <summary>
			/// Constructor that sets mOwner to owner and mIndex to index
			/// </summary>
			/// <param name="owner">The owner of this iterator</param>
			/// <param name="index">The index of the element this iterator points to</param>
			/// <exception cref="std::runtime_error">
			/// Throws exception if owner is nullptr or index is greater than size
			/// </exception>
			Iterator(Vector& owner, size_t index);

			/// <summary>
			/// Copy constructor: copies mOwner and mIndex from rhs
			/// </summary>
			/// <param name="rhs"></param>
			Iterator(const Iterator& rhs) = default;

			/// <summary>
			/// Assignment operator: copies mOwner and mIndex from rhs
			/// </summary>
			/// <param name="rhs">the iterator to copy</param>
			/// <returns>A reference of a copy of rhs</returns>
			Iterator& operator=(const Iterator& rhs) = default;

			/// <summary>
			/// Dereference operator
			/// </summary>
			/// <returns>The data held where the iterator points to</returns>
			/// <exception cref="std::runtime_error">Throws exception if index >= size</exception>
			T& operator*() const;

			T* operator->() const;

			/// <summary>
			/// Comparison Operator
			/// </summary>
			/// <returns>True if both mIndex and mOwner are equal, false otherwise.</returns>
			bool operator==(const Iterator& rhs) const noexcept;

			/// <summary>
			/// Comparison Operator (not equal)
			/// </summary>
			/// <returns>True if not equal, false if equal</returns>
			bool operator!=(const Iterator& rhs) const noexcept;

			/// <summary>
			/// Prefix Increment operator-increments to the next element
			/// </summary>
			/// <returns>This Iterator incremented to the next element</returns>
			/// <exception cref="std::runtime_error">Throws exception if incrementing out of list bounds</exception>
			Iterator& operator++();

			/// <summary>
			/// Postfix Increment operator-increments to the next element
			/// </summary>
			/// <returns>This Iterator incremented to the next element</returns>
			/// <remarks>For performance reasons, actually calls the prefix increment operator.</remarks>
			/// <exception cref="std::runtime_error">Throws exception if incrementing out of list bounds</exception>
			Iterator operator++(int);

			size_t Index() const;

		private:

			Vector<T>* mOwner = nullptr;
			size_t mIndex = 0;
		};


		class ConstIterator
		{
			friend Vector;

		public:
			using size_type = std::size_t;
			using difference_type = std::ptrdiff_t;
			using value_type = const T;
			using reference = const T&;
			using pointer = const T*;
			//using iterator_category = std::random_access_iterator_tag;
			using iterator_category = std::forward_iterator_tag; //must be std::fwd for partition, or need to make bidirectional

			/// <summary>
			/// Default constructor, sets owner to nullptr and index to 0
			/// </summary>
			ConstIterator() = default;

			/// <summary>
			/// Constructor that sets mOwner to owner and mIndex to index
			/// </summary>
			/// <param name="owner">The owner of this ConstIterator</param>
			/// <param name="index">The index of the element this ConstIterator points to</param>
			/// <exception cref="std::runtime_error">
			/// Throws exception if owner is nullptr or index is greater than size
			/// </exception>
			ConstIterator(const Vector& owner, size_t index);

			/// <summary>
			/// Copy constructor: copies mOwner and mIndex from rhs
			/// </summary>
			/// <param name="rhs"></param>
			ConstIterator(const ConstIterator& rhs) = default;

			/// <summary>
			/// Constructor that creates a ConstIterator from an Iterator
			/// </summary>
			/// <param name="rhs"> The Iterator to copy into a ConstIterator </param>
			/// <exception cref="std::runtime_error">Throws exception if  rhs's owner is nullptr</exception>
			ConstIterator(const Iterator& rhs);

			/// <summary>
			/// Assignment operator: copies mOwner and mIndex from rhs
			/// </summary>
			/// <param name="rhs">the ConstIterator to copy</param>
			/// <returns>A reference of a copy of rhs</returns>
			ConstIterator& operator=(const ConstIterator& rhs) = default;

			/// <summary>
			/// Dereference operator
			/// </summary>
			/// <returns>The data held where the ConstIterator points to</returns>
			/// <exception cref="std::runtime_error">Throws exception if index >= size</exception>
			const T& operator*() const;

			const T* operator->() const;

			/// <summary>
			/// Comparison Operator
			/// </summary>
			/// <returns>True if both mIndex and mOwner are equal, false otherwise.</returns>
			bool operator==(const ConstIterator& rhs) const noexcept;

			/// <summary>
			/// Comparison Operator (not equal)
			/// </summary>
			/// <returns>True if not equal, false if equal</returns>
			bool operator!=(const ConstIterator& rhs) const noexcept;

			/// <summary>
			/// Prefix Increment operator-increments to the next element
			/// </summary>
			/// <returns>This ConstIterator incremented to the next element</returns>
			/// <exception cref="std::runtime_error">Throws exception if incrementing out of list bounds</exception>
			ConstIterator& operator++();

			/// <summary>
			/// Postfix Increment operator-increments to the next element
			/// </summary>
			/// <returns>This ConstIterator incremented to the next element</returns>
			/// <remarks>For performance reasons, actually calls the prefix increment operator.</remarks>
			/// <exception cref="std::runtime_error">Throws exception if incrementing out of list bounds</exception>
			ConstIterator operator++(int);

			size_t Index() const;

		private:

			const Vector<T>* mOwner = nullptr;
			size_t mIndex = 0;
		};
	};
}



#include "vector.inl"