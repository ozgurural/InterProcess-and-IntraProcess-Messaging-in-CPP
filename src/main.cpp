#include <sys/wait.h>
#include <unistd.h>
#include <cstdlib>
#include <iostream>

#include "Player.h"

// both players should run in the same process (strong requirement)
void same_process() {
    // Create two players.
    auto player1 = std::make_shared<Player>("Player 1");
    auto player2 = std::make_shared<Player>("Player 2");

    // Have player 1 send a message to player 2.
    player1->SendMessage(player2, "Hello, world!");
}

// have every player in a separate process (different PID).
void seperate_process() {
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

int main() {
    same_process();

    // wait to keyboad input
    std::cin.get();

    seperate_process();

    return 0;
}