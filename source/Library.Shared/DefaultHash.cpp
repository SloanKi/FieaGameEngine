#include "pch.h"
#include "DefaultHash.h"
#include <cstddef>
#include <cstdint>

namespace Library
{
	/// <summary>
	/// Additive  Hash function. Creates a hash value for data of a given size.
	/// </summary>
	/// <param name="data">A pointer to a byte array of data</param>
	/// <param name="size">How many bytes the data contains</param>
	/// <returns>A hash value</returns>
	std::size_t AdditiveHash(const uint8_t* data, std::size_t size)
	{
		std::size_t hashValue = 0;
		std::size_t hashPrime = 29;

		for (std::size_t i = 0; i < size; i++)
		{
			hashValue += hashPrime * data[i];
		}

		return hashValue;
	}

}