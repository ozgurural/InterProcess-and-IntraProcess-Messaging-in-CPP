#include "Player.h"

#include <iostream>
#include <memory>
#include <string>

Player::Player(const std::string& name) : name_(name), counter_(0) {}

void Player::SendMessage(std::shared_ptr<Player> other,
                         const std::string& message) {
    // Lock the mutex.
    std::lock_guard<std::mutex> lock(mutex_);

    // Increment the message counter.
    ++counter_;

    // Print the message being sent.
    std::cout << name_ << ": Sending message " << counter_ << ": " << message
              << std::endl;

    // Receive the message and send a reply.
    other->ReceiveMessage(message);
}

void Player::ReceiveMessage(const std::string& message) {
    // Lock the mutex.
    std::lock_guard<std::mutex> lock(mutex_);

    // Increment the message counter.
    ++counter_;

    // Print the message being received.
    std::cout << name_ << ": Received message " << counter_ << ": " << message
              << std::endl;

    // Create a shared_ptr to this Player object and pass it to the sender.
    sender_->SendMessage(std::shared_ptr<Player>(this, [](Player*) {}),
                         message + " " + std::to_string(counter_));
}
