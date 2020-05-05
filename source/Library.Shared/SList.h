#pragma once

#include <stdexcept>
#include <functional>
#include "DefaultEquality.h"

namespace Library
{

	/// <summary>
	/// SList is a template for a singly linked list.
	/// </summary>
	template <typename T>
	class SList
	{
	private:
		/// <summary>
		/// The SList is comprised of Nodes which hold data of type T and a pointer to the next node in the list.
		/// </summary>
		struct Node final
		{
			T Data;
			Node* Next;

			Node(const T& data, Node* next = nullptr);
			Node(T&& data, Node* next = nullptr);
		};

	public:
		using EqualityFunctor = std::function<bool(const T& lhs, const T& rhs)>;
		class Iterator;			//forward declaration because I don't want the class all the way up here.
		class ConstIterator;	//forward declaration because I don't want the class all the way up here.

		/// <summary>
		/// Default constructor: SList is initialized to empty
		/// </summary>
		/// <remarks> Runs in constant time </remarks>
		SList() = default;		//user declared compiler provided default constructor

		/// <summary>
		/// Destructor: deletes all nodes in the list using Clear()
		/// </summary>
		/// <remarks> Runs in linear time </remarks>
		~SList();

		/// <summary>
		/// Copy Constructor: Deep-copy the origList passed as an argument.
		/// </summary>
		/// <param name="origList"> The list to deep-copy into this list </param>
		/// <remarks> Runs in linear time </remarks>
		SList(const SList<T>& origList);

		/// <summary>
		/// Move constructor. Shallow copies the SList and invalidates the RHS
		/// </summary>
		/// <param name="rhs">R-Value reference of the SList to move the memory from</param>
		SList(SList&& rhs);

		/// <summary>
		/// Assignment Operator: Deep-copy the origList passed as an argument.
		/// </summary>
		/// <param name="origList"> The list to deep-copy into this list </param>
		/// <returns>A reference to the deep-copy of origList</returns>
		/// <remarks> Runs in linear time </remarks>
		SList<T>& operator= (const SList<T>& origList);

		/// <summary>
		/// Move assignment operator. Shallow copies the SList and invalidates the rhs.
		/// </summary>
		/// <param name="rhs">R-Value reference of the SList to move the memory from</param>
		/// <returns>Reference to the new SList created by moving rhs mem</returns>
		/// <exception cref="std::runtime_error">Throws exception if rhs is the same as this list</exception>
		SList<T>& operator=(SList&& rhs);

		/// <summary>
		/// Accessor function for member variable mSize
		/// </summary>
		/// <returns> mSize (the size of the list) </returns>
		/// <remarks> Runs in constant time </remarks>
		size_t Size() const;

		/// <summary>
		/// Function to check if list is empty or not
		/// </summary>
		/// <returns>True if list is empty, false if it is not empty</returns>
		/// <remarks>Runs in constant time</remarks>
		bool IsEmpty() const;

		/// <summary>
		/// Accessor for the data contained in the node pointed to by mFront
		/// </summary>
		/// <returns>The data contained in the node pointed to by mFront</returns>
		/// <exception cref="std::runtime_error">Throws exception if list is empty</exception>
		/// <remarks>Runs in constant time</remarks>
		T& Front();

		/// <summary>
		/// Const Accessor for the data contained in the node pointed to by mFront
		/// </summary>
		/// <returns>The data contained in the node pointed to by mFront</returns>
		/// <exception cref="std::runtime_error">Throws exception if list is empty</exception>
		/// <remarks>Runs in constant time</remarks>
		const T& Front() const;

		/// <summary>
		/// Accessor for the data contained in the node pointed to by mBack
		/// </summary>
		/// <returns>The data contained in the node pointed to by mBack</returns>
		/// <exception cref="std::runtime_error">Throws exception if list is empty</exception>
		/// <remarks>Runs in constant time</remarks>
		T& Back();

		/// <summary>
		/// Const Accessor for the data contained in the node pointed to by mBack
		/// </summary>
		/// <returns>The data contained in the node pointed to by mBack</returns>
		/// <exception cref="std::runtime_error">Throws exception if list is empty</exception>
		/// <remarks>Runs in constant time</remarks>
		const T& Back() const;

		/// <summary>
		/// Pushes the data onto the front of the list
		/// </summary>
		/// <param name="data">The data to add to the list</param>
		/// <remarks>Runs in constant time</remarks>
		void PushFront(const T& data);

		/// <summary>
		/// Pushes the data onto the back of the list
		/// </summary>
		/// <param name="data">The data to add to the list</param>
		/// <returns>An iterator pointing to the new mBack</returns>
		/// <remarks>Runs in constant time</remarks>
		Iterator PushBack(const T& data);

		/// <summary>
		/// Pushes the data onto the back of the list
		/// </summary>
		/// <param name="data">The data to add to the list</param>
		/// <returns>An iterator pointing to the new mBack</returns>
		/// <remarks>Runs in constant time</remarks>
		Iterator Pushback(T&& data);

		/// <summary>
		/// Removes the first node of the list. If list is empty, nothing is done.
		/// </summary>
		/// <remarks>Runs in constant time</remarks>
		void PopFront();

		/// <summary>
		/// Removes the last node of the list. If list is empty, nothing is done.
		/// </summary>
		/// <remarks>Runs in linear time</remarks>
		void PopBack();

		/// <summary>
		/// Removes all nodes from the list.
		/// </summary>
		/// <remarks>Runs in linear time</remarks>
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
		/// <remarks>past the end of the list means that mNode is nullptr</remarks>
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
		/// Inserts the given data in front of the given iterator. 
		/// </summary>
		/// <param name="it">the iterator to insert after</param>
		/// <param name="data">the data to insert into the list</param>
		/// <returns>An iterator pointing to the new container of the new data in the list</returns>
		Iterator InsertAfter(Iterator it, const T& data);

		/// <summary>
		/// Searches for the value in the list and returns an Iterator pointing to the first location containing value
		/// </summary>
		/// <param name="value">the value to search for</param>
		/// <returns>
		/// If found, returns an Iterator pointing to the location of the data.
		/// If not found, returns end()
		/// </returns>
		Iterator Find(const T& value, EqualityFunctor equalFunc = DefaultEquality<T>{});

		/// <summary>
		/// Searches for the value in the list and returns a ConstIterator pointing to the first location containing value
		/// </summary>
		/// <param name="value">the value to search for</param>
		/// <returns>
		/// If found, returns a ConstIterator pointing to the location of the data.
		/// If not found, returns cend()
		/// </returns>
		ConstIterator Find(const T& value, EqualityFunctor equalFunc = DefaultEquality<T>{}) const;

		/// <summary>
		/// Removes the item associated with the given data and maintains list integrity.
		/// </summary>
		/// <param name="it"></param>
		/// <exception cref="std::runtime_error">Throws exception if owner is not this.</exception>
		/// <remarks>Calling remove on a non-existent item immediately returns.</remarks>
		void Remove(Iterator it);


	private:
		size_t mSize{ 0 };			
		Node* mFront{ nullptr };
		Node* mBack{ nullptr };
		
	public:
		/// <summary>
		/// Iterator class for SList. Used to traverse and access elements in the list.
		/// </summary>
		class Iterator
		{
			friend SList;
			friend ConstIterator;

		public:
			/// <summary>
			/// Default constructor, sets owner and node to nullptr
			/// </summary>
			Iterator() = default;		//user declared compiler provided default constructor

			/// <summary>
			/// Constructor that sets mOwner to owner and mNode to the first item in the list.
			/// </summary>
			/// <param name="owner">A const pointer to the SList this iterator belongs to</param>
			/// <exception cref="std::runtime_error">Throws exception if owner is nullptr</exception>
			Iterator(const SList<T>* owner);

			/// <summary>
			/// Constructor that sets mOwner to owner and mNode to node.
			/// </summary>
			/// <param name="owner">A const pointer to the SList this iterator belongs to</param>
			/// <param name="node">Node pointer for the node that the Iterator contains</param>
			/// <exception cref="std::runtime_error">Throws exception if owner is nullptr</exception>
			Iterator(const SList<T>* owner, Node* node);

			/// <summary>
			/// Copy Constructor: copies the owner and node from rhs
			/// </summary>
			/// <param name="rhs"> The iterator to copy </param>
			/// <exception cref="std::runtime_error">Throws exception if owner is nullptr</exception>
			Iterator(const Iterator& rhs);

			/// <summary>
			/// Dereference operator
			/// </summary>
			/// <returns>The data held within the node that the iterator points to</returns>
			/// <exception cref="std::runtime_error">Throws exception if mNode is nullptr</exception>
			T& operator*() const;

			/// <summary>
			/// Comparison Operator
			/// </summary>
			/// <returns>True if both mNode and mOwner are equal, false otherwise.</returns>
			bool operator==(const Iterator& rhs) const noexcept;

			/// <summary>
			/// Comparison Operator (not equal)
			/// </summary>
			/// <returns>True if not equal, false if equal</returns>
			bool operator!=(const Iterator& rhs) const noexcept;

			/// <summary>
			/// Prefix Increment operator-increments to the next node
			/// </summary>
			/// <returns>This Iterator incremented to the next node</returns>
			/// <exception cref="std::runtime_error">Throws exception if incrementing out of list bounds</exception>
			Iterator& operator++();

			/// <summary>
			/// Postfix Increment operator-increments to the next node
			/// </summary>
			/// <returns>This Iterator incremented to the next node</returns>
			/// <remarks>For performance reasons, actually calls the prefix increment operator.</remarks>
			/// <exception cref="std::runtime_error">Throws exception if incrementing out of list bounds</exception>
			Iterator operator++(int);

		private:
			const SList<T>* mOwner = nullptr;
			Node* mNode = nullptr;
		};


		class ConstIterator
		{
			friend SList;

		public:
			/// <summary>
			/// Default constructor, sets owner and node to nullptr
			/// </summary>
			ConstIterator() = default;

			/// <summary>
			/// Constructor that sets mOwner to owner and mNode to the first item in the list.
			/// </summary>
			/// <param name="owner">A const pointer to the SList this ConstIterator belongs to</param>
			/// <exception cref="std::runtime_error">Throws exception if owner is nullptr</exception>
			ConstIterator(const SList<T>* owner);

			/// <summary>
			/// Constructor that sets mOwner to owner and mNode to node.
			/// </summary>
			/// <param name="owner">A const pointer to the SList this ConstIterator belongs to</param>
			/// <param name="node">Node pointer for the node that the ConstIterator contains</param>
			/// <exception cref="std::runtime_error">Throws exception if owner is nullptr</exception>
			ConstIterator(const SList<T>* owner, Node* node);

			/// <summary>
			/// Copy Constructor: copies the owner and node from rhs
			/// </summary>
			/// <param name="rhs"> The ConstIterator to copy </param>
			/// <exception cref="std::runtime_error">Throws exception if rhs's owner is nullptr</exception>
			ConstIterator(const ConstIterator& rhs);

			/// <summary>
			/// Constructor that creates a ConstIterator from an Iterator
			/// </summary>
			/// <param name="rhs"> The Iterator to copy into a ConstIterator </param>
			/// <exception cref="std::runtime_error">Throws exception if  rhs's owner is nullptr</exception>
			ConstIterator(const Iterator& rhs);

			/// <summary>
			/// Dereference operator
			/// </summary>
			/// <returns>The data held within the node that the ConstIterator points to</returns>
			/// <exception cref="std::runtime_error">Throws exception if mNode is nullptr</exception>
			const T& operator*() const;

			/// <summary>
			/// Comparison Operator
			/// </summary>
			/// <returns>True if both mNode and mOwner are equal, false otherwise.</returns>
			bool operator==(const ConstIterator & rhs) const noexcept;

			/// <summary>
			/// Comparison Operator (not equal)
			/// </summary>
			/// <returns>True if not equal, false if equal</returns>
			bool operator!=(const ConstIterator & rhs) const noexcept;

			/// <summary>
			/// Prefix Increment operator-increments to the next node
			/// </summary>
			/// <returns>This ConstIterator incremented to the next node</returns>
			/// <exception cref="std::runtime_error">Throws exception if incrementing out of list bounds</exception>
			ConstIterator& operator++();

			/// <summary>
			/// Postfix Increment operator-increments to the next node
			/// </summary>
			/// <returns>This ConstIterator incremented to the next node</returns>
			/// <exception cref="std::runtime_error">Throws exception if incrementing out of list bounds</exception>
			/// <remarks>For performance reasons, actually calls the prefix increment operator.</remarks>
			ConstIterator operator++(int);

		private:
			const SList<T>* mOwner = nullptr;
			Node* mNode = nullptr;
		};
	};

}


#include "SList.inl"