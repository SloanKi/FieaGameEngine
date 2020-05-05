#pragma once
#include "SList.h"

namespace Library
{
	/// <summary>
	/// Last-in First out container
	/// </summary>
	template <typename T>
	class Stack final
	{
	public:
		Stack() = default;
		Stack(const Stack& rhs) = default;
		Stack(Stack&& rhs) noexcept = default;
		~Stack() = default;
		Stack& operator=(const Stack& rhs) = default;
		Stack& operator=(Stack&& rhs) noexcept = default;

		/// <summary>
		/// Push the data to the stack
		/// </summary>
		/// <param name="data">The data to push</param>
		void Push(const T& data);

		/// <summary>
		/// Pop the last element off the stack
		/// </summary>
		void Pop();

		/// <summary>
		/// Gets the element on top of the stack (the last element put in)
		/// </summary>
		/// <returns>The element on top of the stack</returns>
		T& Top();

		/// <summary>
		/// Gets the element on top of the stack (the last element put in) (Const)
		/// </summary>
		/// <returns>The element on top of the stack (const)</returns>
		const T& Top() const;

		/// <summary>
		/// Gets the size of the stack
		/// </summary>
		/// <returns>the number of elements in the stack</returns>
		size_t Size() const noexcept;

		/// <summary>
		/// Ask if stack is empty or not
		/// </summary>
		/// <returns>True if empty, false if not</returns>
		bool IsEmpty() const noexcept;

		/// <summary>
		/// Clear the stack
		/// </summary>
		void Clear();
	private:
		SList<T> mList;
	};
}

#include "Stack.inl"