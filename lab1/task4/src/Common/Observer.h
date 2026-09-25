#pragma once
#include <algorithm>
#include <map>
#include <vector>

template<typename Subject>
class Observer {
public:
    virtual void Update(const Subject& data) = 0;
    virtual ~Observer() = default;
};

template<typename T>
concept HasEnumEventType = requires {
    typename T::EventType;
} && std::is_enum_v<typename T::EventType>;

template<typename Derived, typename EventType>
class Observable {
public:
    Observable() {
        static_assert(HasEnumEventType<Derived>, "The Derived class must have EventType. Please put 'using EventType = EventType;' in your class.");
        static_assert(std::is_same_v<EventType, typename Derived::EventType>, "Event type of Derived class need to be the same os EventType of template.");
    }

    void Subscribe(const EventType& event, Observer<Derived>& observer) {
        for (auto listener : listeners[event]) {
            if (listener == &observer) return;
        }

        listeners[event].push_back(&observer);
    }

    void Unsubscribe(const EventType& event, Observer<Derived>& observer) {
        auto it = listeners.find(event);
        if (it == listeners.end()) return;
        auto& vec = it->second;
        vec.erase(std::remove(vec.begin(), vec.end(), &observer), vec.end());
    }

    void Notify(const EventType& event) const {
        auto it = listeners.find(event);
        if (it == listeners.end()) return;
        auto copiedListeners = it->second;
        for (auto& listener : copiedListeners) {
            listener->Update(static_cast<const Derived&>(*this));
        }
    }

private:
    std::map<EventType, std::vector<Observer<Derived>*>> listeners;
};
