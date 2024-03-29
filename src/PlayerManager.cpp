#include "PlayerManager.h"

// both players should run in the same process (same PID).
void PlayerManager::startSameProcessMessaging(int message_count) {
    LOG(INFO) << "Both players are running in the same process.";
    // Create two players.
    std::shared_ptr<Player> player1 =
        std::make_shared<Player>("Player 1", message_count, -1, -1, false);
    std::shared_ptr<Player> player2 =
        std::make_shared<Player>("Player 2", message_count, -1, -1, false);

    LOG(INFO) << "Player 1 and Player 2 are created.";

    // Have player 1 send a message to player 2.
    player1->SameProcessSendMessage(player2, "Hello, world!");
}

void PlayerManager::startSeparateProcessMessaging(int message_count) {
    LOG(INFO) << "Each player is running in a separate process.";

    // Create two pipes for communication between the two processes.
    int pipe_fd_1[2], pipe_fd_2[2];
    if (pipe(pipe_fd_1) == -1 || pipe(pipe_fd_2) == -1) {
        perror("Error creating pipes");
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
        // Close the unwanted ends of the pipes.
        close(pipe_fd_1[0]);  // close the read end of the pipe
        close(pipe_fd_2[1]);  // close the write end of the pipe

        // Create a Player object with the read end of the first pipe and the
        // write end of the second pipe as the pipe file descriptors.
        std::atomic_bool send_state = {true};
        std::unique_ptr<Player> player1 = std::make_unique<Player>(
            "Player 1", message_count, pipe_fd_2[0], pipe_fd_1[1], send_state);

        // Wait until the other player has sent back message_count messages.
        player1->PipeCommunication();
    } else {
        // This is the parent process.
        // Close the unwanted ends of the pipes.
        close(pipe_fd_1[1]);  // close the write end of the pipe
        close(pipe_fd_2[0]);  // close the read end of the pipe

        // Create a Player object with the read end of the first pipe and the
        // write end of the second pipe as the pipe file descriptors.
        std::atomic_bool send_state = {false};
        std::unique_ptr<Player> player2 = std::make_unique<Player>(
            "Player 2", message_count, pipe_fd_1[0], pipe_fd_2[1], send_state);

        // Wait until the other player has sent back message_count messages.
        player2->PipeCommunication();
    }
}

void PlayerManager::stopMessaging() {
    // Log that the task manager is stopping
    LOG(INFO) << "Stopping player manager";
}
