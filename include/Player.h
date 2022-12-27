#ifndef PLAYER_H
#define PLAYER_H

#include <atomic>
#include <memory>
#include <mutex>
#include <string>

#include "Logger.h"

class Player : public std::enable_shared_from_this<Player> {
public:
    // Constructs a new player with the given name.
    explicit Player(std::string name, int message_count_, int pipe_fd);

    // Sends a message to another player.
    void SameProcessSendMessage(const std::shared_ptr<Player>& other,
                     const std::string& message);

    // Receives a message from another player.
    void SameProcessReceiveMessage(const std::string& message,
                        const std::shared_ptr<Player>& sender);

    // Sends a message to another player in a separate process.
    void SeparateProcessSendMessage(const std::string& message);
    
    // Receives a message from another player in a separate process.
    void SeparateProcessReceiveMessage();

    // Returns true if the program should terminate.
    bool ShouldTerminate() const;

private:
    // The name of the player.
    std::string name_;

    // The message counter for this player.
    std::atomic<int> counter_;

    std::atomic<int> message_count_;

    // A mutex to synchronize access to shared data.
    std::recursive_mutex mutex_;

    // The file descriptor for the pipe.
    int pipe_fd_;

    // A flag to signal that the program should terminate.
    std::atomic<bool> terminate_;
};

#endif  // PLAYER_H