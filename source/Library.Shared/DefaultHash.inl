#include "DefaultHash.h"

namespace Library
{
	template <typename T>
	inline size_t DefaultHash<T>::operator()(const T& key) const
	{
		const uint8_t* data = reinterpret_cast<const uint8_t*>(&key);
		return AdditiveHash(data, sizeof(T));
	}
	
	inline size_t DefaultHash<std::string>::operator()(const std::string& key) const
	{
		const uint8_t* data = reinterpret_cast<const uint8_t*>(key.c_str());
		return AdditiveHash(data, key.length());
	}
	
	inline size_t DefaultHash<const std::string>::operator()(const std::string& key) const
	{
		const uint8_t* data = reinterpret_cast<const uint8_t*>(key.c_str());
		return AdditiveHash(data, key.length());
	}

	inline size_t DefaultHash<char*>::operator()(const char* key) const
	{
		const uint8_t* data = reinterpret_cast<const uint8_t*>(key);
		return AdditiveHash(data, strlen(key));
	}

	inline size_t DefaultHash<const char*>::operator()(const char* key) const
	{
		const uint8_t* data = reinterpret_cast<const uint8_t*>(key);
		return AdditiveHash(data, strlen(key));
	}
}