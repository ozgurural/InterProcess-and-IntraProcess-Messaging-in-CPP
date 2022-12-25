#include "Player.h"

#include <iostream>
#include <memory>
#include <string>
#include <utility>

Player::Player(std::string name) : name_(std::move(name)), counter_(0) {}

void Player::SendMessage(const std::shared_ptr<Player>& other,
                         const std::string& message) {
    // Lock the mutex.
    std::lock_guard<std::mutex> lock(mutex_);

    // Increment the message counter.
    ++counter_;

    // Check if the termination condition has been reached.
    if (counter_ == 10) {
        return;
    }

    // Print the message being sent with logger
    LOG(INFO) << name_ << ": Sending message " << counter_ << ": " << message;

    // Receive the message and send a reply.
    other->ReceiveMessage(message);
}

void Player::ReceiveMessage(const std::string& message) {
    // Lock the mutex.
    std::lock_guard<std::mutex> lock(mutex_);

    // Increment the message counter.
    ++counter_;

    // Print the message being received with logger
    LOG(INFO) << name_ << ": Received message " << counter_ << ": " << message;

    auto self = shared_from_this();
    // Create a shared_ptr to this Player object and pass it to the sender.
    self->SendMessage(shared_from_this(),
                      message + " " + std::to_string(counter_));
}