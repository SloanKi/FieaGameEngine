#include "Factory.h"
#include "Hashmap.h"
#include <memory>
#include <utility>

namespace Library
{
	template <typename T>
	Hashmap<const std::string, const Factory<T>* const> Factory<T>::mFactoryRegistry{ 23 };

	template<typename T>
	inline gsl::owner<T*> Factory<T>::Create(const std::string& name)
	{
		auto it = mFactoryRegistry.Find(name);
		//if found, call the Create() method of the factory, otherwise return nullptr
		return (it != mFactoryRegistry.end() ? it->second->Create() : nullptr);
	}

	template<typename T>
	inline const Factory<T>* const Factory<T>::Find(const std::string& name)
	{
		auto it = mFactoryRegistry.Find(name);
		//if found, call the Create() method of the factory, otherwise return nullptr
		return (it != mFactoryRegistry.end() ? it->second : nullptr);
	}

	template<typename T>
	inline void Factory<T>::Clear()
	{
		mFactoryRegistry.Clear();
	}

	template<typename T>
	inline size_t Factory<T>::Size()
	{
		return mFactoryRegistry.Size();
	}

	template<typename T>
	inline void Factory<T>::Add(const Factory& factory)
	{
		//if factory already exists
		if (mFactoryRegistry.ContainsKey(factory.ClassName()))
		{
			throw std::runtime_error("Factory already exists in factory registry");
		}

		auto result = mFactoryRegistry.Insert(std::make_pair(factory.ClassName(), &factory));
		assert(result.second);
	}

	template<typename T>
	inline void Factory<T>::Remove(const Factory& factory)
	{
		mFactoryRegistry.Remove(factory.ClassName());
	}
}