// main.cpp

#include "Logger.h"
#include "PlayerManager.h"

int main() {
    // Create an object of type TaskManager
    PlayerManager player_manager;
    int message_count = 10;
    player_manager.startSeparateProcessMessaging(message_count);
    // player_manager.startSameProcessMessaging(message_count);
    std::cin.get();
    return 0;
}
