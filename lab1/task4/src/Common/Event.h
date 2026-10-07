#pragma once
#include <functional>
#include <memory>
#include <algorithm>
#include <iostream>
#include <list>

class Connection {
public:
    virtual ~Connection() = default;
    virtual void Disconnect() = 0;
    [[nodiscard]] virtual bool IsActive() const = 0;
};

class Subscription {
public:
    Subscription() = default;

    ~Subscription() {
        Disconnect();
    }

    explicit Subscription(std::shared_ptr<Connection> conn)
    : m_conn(std::move(conn)) {}

    Subscription(const Subscription&) = delete;
    Subscription& operator=(const Subscription&) = delete;

    Subscription(Subscription&& other) noexcept
    : m_conn(std::move(other.m_conn)) {};
    Subscription& operator=(Subscription&& other) noexcept {
        if (this != &other) {
            Disconnect();
            m_conn = std::move(other.m_conn);
        }
        return *this;
    }

    void Disconnect() {
        if (m_conn && m_conn->IsActive()) {
            m_conn->Disconnect();
            m_conn.reset();
        }
    }

    [[nodiscard]] bool IsConnected() const {
        return m_conn && m_conn->IsActive();
    }

private:
    std::shared_ptr<Connection> m_conn;
};

template <typename Signature>
class Event;

template <typename... Args>
class Event<void(Args...)> {
public:
    using Handler = std::function<void(Args...)>;

    ~Event() {
        for (std::shared_ptr<SlotInfo>& item : m_slots) {
            if (item) {
                item->active = false;
            }
        }
    }

    [[nodiscard]] Subscription Subscribe(Handler handler) {
        auto slotInfo = std::make_shared<SlotInfo>(std::move(handler), true);
        m_slots.push_back(slotInfo);

        auto conn = std::make_shared<EventConnection>(slotInfo);
        return Subscription(conn);
    }

    void Notify(const Args&... args) {
        auto copiedSlots = m_slots;
        for (std::shared_ptr<SlotInfo>& subscriber : copiedSlots) {
            if (subscriber && subscriber->active) {
                subscriber->handler(args...);
            }
        }

        CleanUp();
    }

private:
    struct SlotInfo {
        Handler handler;
        bool active = true;
    };

    std::list<std::shared_ptr<SlotInfo>> m_slots;

    void CleanUp() {
        m_slots.erase(
            std::remove_if(
                m_slots.begin(),
                m_slots.end(),
                [](const std::shared_ptr<SlotInfo>& subscriber) {
                    return !subscriber || !subscriber->active;
                }
            ),
            m_slots.end()
        );
    }

    class EventConnection : public Connection {
    public:
        explicit EventConnection(std::weak_ptr<SlotInfo> slot)
        : m_slot(slot) {}

        [[nodiscard]] bool IsActive() const override {
            if (std::shared_ptr<SlotInfo> exist = m_slot.lock()) {
                return exist->active;
            }
            return false;
        };
        void Disconnect() override {
            if (std::shared_ptr<SlotInfo> exist = m_slot.lock()) {
                exist->active = false;
            }
        };

    private:
        std::weak_ptr<SlotInfo> m_slot;
    };
};

#define DECLARE_EVENT(Name, ...) \
private: \
    Event<void(__VA_ARGS__)> m_on##Name; \
public: \
    [[nodiscard]] auto SubscribeOn##Name(typename Event<void(__VA_ARGS__)>::Handler handler) \
    { \
        return m_on##Name.Subscribe(std::move(handler)); \
    } \
private:

