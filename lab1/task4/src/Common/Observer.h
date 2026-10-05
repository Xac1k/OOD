#pragma once
#include <algorithm>
#include <emmintrin.h>
#include <map>
#include <unordered_map>
#include <utility>
#include <vector>
#include "Deletable.h"

template<typename T>
concept HasEnumEventType = requires {
    typename T::EventType;
} && std::is_enum_v<typename T::EventType>;

template<typename Subject>
class Observer : public Deletable {
public:
    virtual void Update(const Subject& data) = 0;
    virtual ~Observer() = default;
};

template<typename Derived, typename EventType>
class Observable;

template<typename Derived, typename EventType>
class AutoObserver : public Observer<Derived> {
public:
    using Subscription = Observable<Derived, EventType>::Subscription;

    AutoObserver(Observable<Derived, EventType>& observable, EventType event)
    : m_subscription(observable.Subscribe(event, *this)) {}
private:
    Subscription m_subscription;
};

template<typename Derived, typename EventType>
class Observable {
public:
    Observable() {
        static_assert(HasEnumEventType<Derived>, "The Derived class must have EventType. Please put 'using EventType = EventType;' in your class.");
        static_assert(std::is_same_v<EventType, typename Derived::EventType>, "Event type of Derived class need to be the same os EventType of template.");
    }

    struct Connection {
        Observer<Derived>* observer = nullptr;
        Observable* observable = nullptr;
        EventType event{};
        bool active = true;
    };

    class Subscription {
    public:
        explicit Subscription(std::shared_ptr<Connection> connection)
        : m_connection(std::move(connection)) {}

        Subscription(Subscription&& other) noexcept
        : m_connection(std::move(other.m_connection)) {}

        Subscription& operator=(const Subscription& other) = delete;
        Subscription& operator=(Subscription& other) = delete;
        Subscription& operator=(Subscription&& other) noexcept {
            if (this != &other) {
                Disconnect();
                m_connection = std::move(other.m_connection);
            }
            return *this;
        }

        void Disconnect() {
            if (!m_connection) return;

            if (m_connection->active) {
                m_connection->active = false;
                if (m_connection->observable) {
                    m_connection->observable->Unsubscribe(m_connection->event, *m_connection->observer);
                }
            }

            m_connection.reset();
        }

        [[nodiscard]] bool IsActive() const {
            return m_connection && m_connection->active;
        }

        ~Subscription() {
            Disconnect();
        }
    private:
        std::shared_ptr<Connection> m_connection;
    };

    using ObserverPointer = Observer<Derived>*;

    Subscription Subscribe(const EventType& event, Observer<Derived>& observer) {
        auto key = std::make_pair(&observer, event);

        auto it = activeConnections.find(key);
        if (it != activeConnections.end()) {
            if (auto existing = it->second.lock()) {
                return Subscription(existing);
            }
            activeConnections.erase(it);
        }

        AddListener(event, observer);
        return Subscription(CreateConnection(event, observer));
    }

    void Unsubscribe(const EventType& event, Observer<Derived>& observer) {
        BreakConnection(event, observer);
        DeleteListener(event, observer);
    }

    void Notify(const EventType& event) {
        auto it = listeners.find(event);
        if (it == listeners.end()) return;

        auto copiedListeners = it->second;
        for (auto& listener : copiedListeners) {
            if (!listener->IsDeleted()) {
                listener->Update(static_cast<const Derived&>(*this));
            }
        }
    }

    ~Observable() {
        BreakAllConnections();
        activeConnections.clear();
        listeners.clear();
    }
private:
    std::map<EventType, std::vector<ObserverPointer>> listeners;
    std::map<std::pair<ObserverPointer, EventType>, std::weak_ptr<Connection>> activeConnections;

    std::shared_ptr<Connection> CreateConnection(const EventType& event, Observer<Derived>& observer) {
        auto key = std::make_pair(&observer, event);
        auto conn = std::make_shared<Connection>(&observer, this, event, true);
        activeConnections[key] = conn;

        return conn;
    }
    void AddListener(const EventType& event, Observer<Derived>& observer) {
        observer.Remedy();
        listeners[event].push_back(&observer);
    }
    void BreakAllConnections() {
        for (auto& [key, connectionObserver] : activeConnections) {
            if (auto conn = connectionObserver.lock()) {
                conn->active = false;
                conn->observable = nullptr;
            }
        }
    }
    void BreakConnection(const EventType& event, Observer<Derived>& observer) {
        auto key = std::make_pair(&observer, event);

        auto it = activeConnections.find(key);
        if (it != activeConnections.end()) {
            if (auto conn = it->second.lock()) {
                conn->active = false;
                conn->observable = nullptr;
            }
            activeConnections.erase(it);
        };
    }
    void DeleteListener(const EventType& event, Observer<Derived>& observer) {
        auto& eventListeners = listeners[event];
        auto observerIt = std::ranges::find(eventListeners, &observer);
        if (observerIt == eventListeners.end())
            return;

        (*observerIt)->Delete();
        eventListeners.erase(
            std::remove(eventListeners.begin(), eventListeners.end(), &observer),
            eventListeners.end()
        );
    }
};
