#include "PlayerManager.h"

PlayerManager::~PlayerManager() {
    // Clean up resources and stop any threads
    LOG(INFO) << "Player manager is destroyed.";
}

// both players should run in the same process (same PID).
void PlayerManager::startSameProcessMessaging(int message_count) {
    LOG(INFO) << "Both players are running in the same process.";
    // Create two players.
    std::shared_ptr<Player> player1 =
        std::make_shared<Player>("Player 1", message_count, -1);
    std::shared_ptr<Player> player2 =
        std::make_shared<Player>("Player 2", message_count, -1);

    LOG(INFO) << "Player 1 and Player 2 are created.";

    // Have player 1 send a message to player 2.
    player1->SameProcessSendMessage(player2, "Hello, world!");
}

// have every player in a separate process (different PID).
void PlayerManager::startSeparateProcessMessaging(int message_count) {
    LOG(INFO) << "Each player is running in a separate process.";

    // Create a pipe for communication between the two processes.
    int pipe_fd[2];
    if (pipe(pipe_fd) == -1) {
        perror("Error creating pipe");
        return;
    }

    // Fork the first process.
    pid_t pid = fork();
    if (pid == -1) {
        perror("Error forking process");
        return;
    }

    if (pid == 0) {
        // This is the child process.
        // Close the write end of the pipe.
        close(pipe_fd[1]);

        // Create a Player object with the read end of the pipe as the pipe file
        // descriptor.
        std::shared_ptr<Player> player =
            std::make_shared<Player>("Player 1", message_count, pipe_fd[0]);

        // Wait until the other player has sent back message_count messages.
        while (!player->ShouldTerminate()) {
            player->SeparateProcessReceiveMessage();
        }
    } else {
        // This is the parent process.
        // Close the read end of the pipe.
        close(pipe_fd[0]);

        // Create a Player object with the write end of the pipe as the pipe
        // file descriptor.
        std::shared_ptr<Player> player =
            std::make_shared<Player>("Player 2", message_count, pipe_fd[1]);

        // Wait until the other player has sent back message_count messages.
        while (!player->ShouldTerminate()) {
            player->SeparateProcessSendMessage("Hello, world!");
        }
    }
}

void PlayerManager::stopMessaging() {
    // Log that the task manager is stopping
    LOG(INFO) << "Stopping player manager";
}