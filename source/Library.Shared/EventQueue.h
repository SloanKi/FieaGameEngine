#pragma once
#include "vector.h"
#include <chrono>
#include <memory>

namespace Library
{
	class GameClock; //forward declaration
	class GameTime; //forward Declaration
	class EventPublisher; //forward declaration

	class EventQueue final
	{
	public:
		using EventQueueInfo = std::pair<std::shared_ptr<EventPublisher>, std::chrono::high_resolution_clock::time_point>;

		/// <summary>
		/// Creates an EventQueue
		/// </summary>
		/// <param name="clock">the clock for this queue to use (from World)</param>
		explicit EventQueue(const GameClock& clock);
		EventQueue(const EventQueue& rhs) = default;
		EventQueue(EventQueue&& rhs) noexcept = default;
		~EventQueue() = default;
		EventQueue& operator=(const EventQueue& rhs) = default;
		EventQueue& operator=(EventQueue&& rhs) noexcept = default;

		/// <summary>
		/// Get the game clock this queue is referencing
		/// </summary>
		/// <returns>the clock the queue uses</returns>
		const GameClock& Clock() const;

		/// <summary>
		/// Get the number of elements in this event queue
		/// </summary>
		/// <returns>the number of elements in the event queue</returns>
		size_t Size() const;

		/// <summary>
		/// Query if this event list is empty or not
		/// </summary>
		/// <returns>true if empty, false if not</returns>
		bool isEmpty() const;

		/// <summary>
		/// Clears all events from this list
		/// </summary>
		void Clear();

		/// <summary>
		/// 
		/// </summary>
		/// <param name="publisher"></param>
		void Send(const EventPublisher& publisher) const;

		/// <summary>
		/// Add a publisher to this event list with an optional delay. If no delay, event delivered immediately
		/// </summary>
		/// <param name="publisher">the publisher to add to this event list</param>
		/// <param name="delay">the time (in milliseconds) to wait before delivering this event</param>
		/// <exception cref="std::runtime_error">Throws exception if publisher already queued</exception>
		void Enqueue(std::shared_ptr<EventPublisher> publisher, std::chrono::milliseconds delay = std::chrono::milliseconds(0));

		/// <summary>
		/// Deliver any expired events and remove them from the queue
		/// </summary>
		/// <param name="gameTime">the current gametime</param>
		void Update(const GameTime& gameTime);

		/// <summary>
		/// Get the events
		/// </summary>
		/// <returns>a const reference to the events</returns>
		const Vector<EventQueueInfo>& GetEvents() const;

	private:
		void AddPendingEvents();
		void VerifyNotDuplicate(const EventPublisher& publisher);
		const GameClock* mClock;
		Vector<EventQueueInfo> mEventList;		
		Vector<EventQueueInfo> mPendingAddList;		
		bool bIsUpdating = false;
	};
}

