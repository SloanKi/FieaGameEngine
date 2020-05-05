#pragma once
#include "Stack.h"

namespace Library
{
	template<typename T>
	inline void Stack<T>::Push(const T& data)
	{
		mList.PushBack(data);
	}

	template<typename T>
	inline void Stack<T>::Pop()
	{
		mList.PopBack();
	}

	template<typename T>
	inline T& Stack<T>::Top()
	{
		return mList.Back();
	}

	template<typename T>
	inline const T& Stack<T>::Top() const
	{
		return mList.Back();
	}

	template<typename T>
	inline size_t Stack<T>::Size() const noexcept
	{
		return mList.Size();
	}

	template<typename T>
	inline bool Stack<T>::IsEmpty() const noexcept
	{
		return mList.IsEmpty();
	}

	template<typename T>
	inline void Stack<T>::Clear()
	{
		mList.Clear();
	}
}