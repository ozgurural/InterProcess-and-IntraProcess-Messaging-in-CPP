#include <gtest/gtest.h>
#include "Player.h"

TEST(PlayerTest, TestSendMessage) {
    // Create two players.
    std::shared_ptr<Player> player1 = std::make_shared<Player>("Player 1");
    std::shared_ptr<Player> player2 = std::make_shared<Player>("Player 2");

    // Send a message from player 1 to player 2.
    player1->SendMessage(player2, "Hello, World!");
}

TEST(PlayerTest, TestReceiveMessage) {
    // Create two players.
    std::shared_ptr<Player> player1 = std::make_shared<Player>("Player 1");
    std::shared_ptr<Player> player2 = std::make_shared<Player>("Player 2");

    // Send a message from player 1 to player 2.
    player1->SendMessage(player2, "Hello, World!");
}

TEST(PlayerTest, TestConcurrentSendAndReceive) {
    // Create two players.
    std::shared_ptr<Player> player1 = std::make_shared<Player>("Player 1");
    std::shared_ptr<Player> player2 = std::make_shared<Player>("Player 2");

    // Create a thread to send a message from player 1 to player 2.
    std::thread t1([&]() { player1->SendMessage(player2, "Hello, World!"); });

    // Create a thread to receive the message from player 2.
    std::thread t2([&]() { player2->ReceiveMessage("Hello, World!", player1); });

    // Wait for the threads to finish.
    t1.join();
    t2.join();
}