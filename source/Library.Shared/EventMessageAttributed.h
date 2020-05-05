#pragma once
#include "Attributed.h"
#include "vector.h"
#include "Signature.h"

namespace Library
{
	class World; //Forward Declaration

	class EventMessageAttributed final : public Attributed
	{
		RTTI_DECLARATIONS(EventMessageAttributed, Attributed)
	public:
		/// <summary>
		/// Default Constructor: Passes RTTI to Attributed, subtype is empty, world is nullptr
		/// </summary>
		EventMessageAttributed();

		/// <summary>
		/// Constructor: Pass RTTI to attributed, set subtype and world (if provided)
		/// </summary>
		/// <param name="subtype">string indicating event subtype</param>
		/// <param name="world">(optional) A pointer to the world. Nullptr if not specified </param>
		EventMessageAttributed(std::string subtype, World* world = nullptr);


		EventMessageAttributed(const EventMessageAttributed& rhs) = default;
		EventMessageAttributed(EventMessageAttributed&& rhs) noexcept = default;
		~EventMessageAttributed() = default;
		EventMessageAttributed& operator=(const EventMessageAttributed& rhs) = default;
		EventMessageAttributed& operator=(EventMessageAttributed&& rhs) noexcept = default;

		/// <summary>
		/// Get the world ptr.
		/// </summary>
		/// <returns>A pointer to the world, or nullptr if world not yet set</returns>
		World* GetWorld();

		/// <summary>
		/// Get the world ptr.
		/// </summary>
		/// <returns>A pointer to the world, or nullptr if world not yet set</returns>
		const World* GetWorld() const;

		/// <summary>
		/// Set the world 
		/// </summary>
		/// <param name="world">The world</param>
		void SetWorld(World& world);

		/// <summary>
		/// Get the subtype (string indicating event subtype)
		/// </summary>
		/// <returns>String indicating event subtype</returns>
		const std::string& GetSubtype() const;

		/// <summary>
		/// Set the subtype
		/// </summary>
		/// <param name="subtype">a string indicating the subtype of the event</param>
		void SetSubtype(std::string subtype);

		/// <summary>
		/// Virtual constructor: creates a heap allocated copy of this
		/// </summary>
		/// <returns>A heap allocated copy of this</returns>
		virtual gsl::owner<EventMessageAttributed*> Clone() const override;

		/// <summary>
		/// Get the signatures for this class (used to populate attributes)
		/// </summary>
		/// <returns>The signatures for prescribed attributes for this class</returns>
		static Vector<Signature> Signatures();

	private:
		World* mWorldPtr = nullptr;
		std::string mSubtype; //prescribed attribute
	};
};

