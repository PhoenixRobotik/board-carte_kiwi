#pragma once

#include <memory>
#include <vector>

class InterruptProvider;

class InterruptListener
: std::enable_shared_from_this<InterruptListener>
{
public:
    virtual bool callback() = 0;

protected:
    void subscribe(InterruptProvider& provider);
};



class InterruptProvider {
public:
    void subscribe(std::shared_ptr<InterruptListener> subscriber) {

        subscribers.push_back(subscriber);
    }

protected:
    // Cleanup subscribers on the fly
    void interrupt() {
        for (auto subs_it = subscribers.begin(); subs_it != subscribers.end(); ++subs_it)
            if (auto subscriber = subs_it->lock())
                subscriber->callback();
            else
                subs_it = subscribers.erase(subs_it);
    }

private:
    std::vector<std::weak_ptr<InterruptListener>> subscribers;
};




void InterruptListener::subscribe(InterruptProvider& provider) {
    provider.subscribe(shared_from_this());
}
