#include "Player.h"

#include <iostream>
#include <memory>
#include <string>
#include <utility>

Player::Player(std::string  name) : name_(std::move(name)), counter_(0) {}

void Player::SendMessage(const std::shared_ptr<Player>& other,
                         const std::string& message) {
    // Lock the recursive mutex.
    std::lock_guard<std::recursive_mutex> lock(mutex_);

    // Increment the message counter.
    ++counter_;

    // Check if the termination condition has been reached.
    if (counter_ == 11) {
        terminate_ = true;
        exit(0);
    }

    // Print the message being sent.
    std::cout << name_ << ": Sending message " << counter_ << ": " << message
              << std::endl;

    // Receive the message and send a reply.
    auto self = shared_from_this();
    other->ReceiveMessage(message, self);
}

bool Player::ShouldTerminate() const {
    return terminate_;
}

void Player::ReceiveMessage(const std::string& message,
                            const std::shared_ptr<Player>& sender) {
    // Lock the recursive mutex.
    std::lock_guard<std::recursive_mutex> lock(mutex_);

    // Increment the message counter.
    ++counter_;

    // Print the message being received.
    std::cout << name_ << ": Received message " << counter_ << ": " << message
              << std::endl;

    auto self = shared_from_this();
    // Create a shared_ptr to this Player object and pass it to the sender.
    sender->SendMessage(self, message + " " + std::to_string(counter_));

    if (counter_ == 11) {
        terminate_ = true;
        exit(0);
    }
}
