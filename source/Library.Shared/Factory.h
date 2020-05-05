#pragma once
#include <Hashmap.h>
#include <memory>
#include <string>
#include <gsl/gsl>


namespace Library
{
	template <typename T>
	class Factory
	{
	public:
		Factory() = default;
		Factory(const Factory& rhs) = delete;
		Factory(Factory&& rhs) noexcept = delete;
		virtual ~Factory() = default;
		Factory& operator=(const Factory& rhs) = delete;
		Factory& operator=(Factory&& rhs) noexcept = delete;

		/// <summary>
		/// Create a given class given the factory name
		/// </summary>
		/// <param name="name">The name of the factory to use to create the object</param>
		/// <returns>A pointer to the object created by the factory with the given name</returns>
		static gsl::owner<T*> Create(const std::string& name);

		/// <summary>
		/// Find a factory with the given name
		/// </summary>
		/// <param name="name">The name of the factory to search for</param>
		/// <returns>A pointer to the factory if found, or nullptr otherwise</returns>
		static const Factory<T>* const Find(const std::string& name);

		/// <summary>
		/// Clears the hashmap of factories
		/// </summary>
		static void Clear();
		static size_t Size();

		/// <summary>
		/// Get classname of derived factory class
		/// </summary>
		/// <returns>The name of the derived factory</returns>
		virtual const std::string& ClassName() const = 0;
		
		/// <summary>
		/// Creates the concrete product 
		/// </summary>
		/// <returns>A pointer to a concrete product</returns>
		virtual gsl::owner<T*> Create() const = 0;

	protected:
		/// <summary>
		/// Adds factory to the static registry
		/// </summary>
		/// <param name="factory">the factory to add</param>
		static void Add(const Factory& factory);

		/// <summary>
		/// Removes factory from static registry
		/// </summary>
		/// <param name="factory">The factory to remove</param>
		static void Remove(const Factory& factory);


	private:
		static Hashmap<const std::string, const Factory<T>* const> mFactoryRegistry;
	};



#define ConcreteFactory(concreteProdT, AbstractProdT)														\
	class concreteProdT##Factory final : public Factory<AbstractProdT>										\
	{																										\
	public:																									\
		concreteProdT##Factory() : mName(#concreteProdT) { Factory<AbstractProdT>::Add(*this); }	\
		~concreteProdT##Factory() { Factory<AbstractProdT>::Remove(*this); }								\
		gsl::owner<AbstractProdT*> Create() const { return new concreteProdT(); }							\
		const std::string& ClassName() const { return mName; }												\
	private:																								\
		const std::string mName;																			\
	};
}

#include "Factory.inl"

