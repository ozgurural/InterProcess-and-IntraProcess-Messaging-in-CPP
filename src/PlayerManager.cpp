#include "PlayerManager.h"

PlayerManager::~PlayerManager() {
    // Clean up resources and stop any threads
    stopMessaging();
}

// both players should run in the same process (strong requirement)
void PlayerManager::sameProcess() {
    // Create two players.
    std::shared_ptr<Player> player1 = std::make_shared<Player>("Player 1");
    std::shared_ptr<Player> player2 = std::make_shared<Player>("Player 2");

    LOG(INFO) << "Player 1 and Player 2 are created.";

    // Have player 1 send a message to player 2.
    player1->SendMessage(player2, "Hello, world!");
}

// have every player in a separate process (different PID).
void PlayerManager::separateProcess() {
    // Create a new process.
    pid_t pid = fork();
    if (pid == 0) {
        // This is the child process.
        // Execute the player program in this process.
        execl("./player", "player", nullptr);
    } else {
        // This is the parent process.
        // Wait for the child process to finish.
        waitpid(pid, nullptr, 0);
    }
}

void PlayerManager::startMessaging(bool same_process, int message_count) {
    // Log that the task manager is starting
    LOG(INFO) << "Starting player manager";

}

void PlayerManager::stopMessaging() {


  
}