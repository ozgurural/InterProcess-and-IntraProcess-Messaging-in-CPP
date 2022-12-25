#ifndef PLAYER_H
#define PLAYER_H

#include <atomic>
#include <memory>
#include <mutex>
#include <string>
#include <condition_variable>
#include <atomic>

#include "Logger.h"

class Player : public std::enable_shared_from_this<Player> {
public:
    // Constructs a new player with the given name.
    explicit Player(std::string  name);

    // Sends a message to another player.
    void SendMessage(const std::shared_ptr<Player>& other, const std::string& message);

    // Receives a message from another player.
    void ReceiveMessage(const std::string& message,
                        const std::shared_ptr<Player>& sender);

    // Returns true if the program should terminate.
    bool ShouldTerminate() const;

private:
    // The name of the player.
    std::string name_;

    // The message counter for this player.
    std::atomic<int> counter_;

    // A mutex to synchronize access to shared data.
    std::recursive_mutex mutex_;

    // A flag to signal that the program should terminate.
    std::atomic<bool> terminate_ = false;

    std::condition_variable cv_;
};

#endif  // PLAYER_H