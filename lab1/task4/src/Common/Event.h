#pragma once
#include <functional>
#include <memory>
#include <algorithm>
#include <iostream>
#include <list>

template <typename Signature>
class Event;

template <typename... Args>
class Event<void(Args...)> {
public:
    using Handler = std::function<void(Args...)>;

    struct Connection {
        Handler handler;
        bool active = true;
    };

    class Subscription {
    public:
        Subscription() = default;

        ~Subscription() {
            Disconnect();
        }

        explicit Subscription(std::weak_ptr<Connection> state)
        : m_conn(std::move(state)) {
            std::cout << "Subscription constructor";
        }

        Subscription(const Subscription&) = delete;
        Subscription& operator=(const Subscription&) = delete;

        Subscription(Subscription&& other) noexcept = default;
        Subscription& operator=(Subscription&& other) noexcept {
            if (this != &other) {
                Disconnect();
                m_conn = std::move(other.m_conn);
            }
            return *this;
        }

        void Disconnect() {
            if (auto conn = m_conn.lock()) {
                conn->active = false;
            }
        }

        bool IsActive() {
            if (auto conn = m_conn.lock()) {
                return conn->active;
            }
            return false;
        }

    private:
        std::weak_ptr<Connection> m_conn;
    };

    ~Event() {
        for (std::shared_ptr<Connection>& item : m_subscriptions) {
            if (item) {
                item->active = false;
            }
        }
    }

    [[nodiscard]] Subscription Subscribe(Handler handler) {
        auto connection = std::make_shared<Connection>(std::move(handler), true);
        m_subscriptions.push_back(connection);

        return Subscription(connection);
    }

    void Notify(const Args&... args) {
        for (std::shared_ptr<Connection>& subscriber : m_subscriptions) {
            if (subscriber && subscriber->active) {
                subscriber->handler(args...);
            }
        }

        CleanUp();
    }

private:
    std::list<std::shared_ptr<Connection>> m_subscriptions;

    void CleanUp() {
        m_subscriptions.erase(
            std::remove_if(
                m_subscriptions.begin(),
                m_subscriptions.end(),
                [](const std::shared_ptr<Connection>& subscriber) {
                    return !subscriber || !subscriber->active;
                }
            ),
            m_subscriptions.end()
        );
    }
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

