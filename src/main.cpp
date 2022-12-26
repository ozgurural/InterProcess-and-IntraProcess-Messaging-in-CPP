// main.cpp

#include "Logger.h"
#include "Player.h"
#include "PlayerManager.h"

int main() {
    // Create an object of type TaskManager
    PlayerManager player_manager;
    bool same_process = true;
    int message_count = 10;
    player_manager.startMessaging(same_process, message_count);

    // wait to keyboard input
    std::cin.get();
    player_manager.stopMessaging();

    same_process = false;
    player_manager.startMessaging(same_process, message_count);
    player_manager.stopMessaging();

    std::cin.get();
    return 0;
}
