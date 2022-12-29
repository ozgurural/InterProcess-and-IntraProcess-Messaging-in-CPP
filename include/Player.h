#ifndef PLAYER_H
#define PLAYER_H

#include <atomic>
#include <cstring>
#include <memory>
#include <mutex>
#include <string>

#include <semaphore.h>  // for sem_t
#include <unistd.h>     // for pipe, fork
#include <condition_variable>
#include <memory>
#include <thread>
#include <utility>

#include "Logger.h"

class Player : public std::enable_shared_from_this<Player> {
public:
    // Constructs a new player with the given name.
    explicit Player(std::string name,
                    int message_count,
                    int pipe_fd1,
                    int pipe_fd2,
                    bool write_state);

    // Sends a message to another player.
    void SameProcessSendMessage(const std::shared_ptr<Player>& other,
                                const std::string& message);

    // Receives a message from another player.
    void SameProcessReceiveMessage(const std::string& message,
                                   const std::shared_ptr<Player>& sender);

    // Sends a message to another player in a separate process.
    void PipeCommunication();

    // Returns true if the program should terminate.
    bool ShouldTerminate() const;

    // read pipe thread
    void ReadPipeThread();

    // write pipe thread
    void WritePipeThread();

    // join threads
    void joinThreads();

private:
    // The name of the player.
    std::string name_;

    // The message counter for this player.
    std::atomic<int> counter_;

    std::atomic<int> message_count_;

    // write state condition variable
    std::condition_variable cv;

    // write state
    bool write_state_;

    // A mutex to synchronize access to shared data.
    std::recursive_mutex mutex_;

    std::mutex m_;

    // The file descriptor for the pipe.
    int pipe_fd1_;
    int pipe_fd2_;

    // A flag to signal that the program should terminate.
    bool terminate_;

    // A semaphore to ensure only one process can access the pipe at a time.
    sem_t semaphore_{};

    // init message
    std::string message_;

    // Thread used to execute periodic tasks
    std::jthread read_pipe_thread_;
    std::jthread write_pipe_thread_;
};

#endif  // PLAYER_H