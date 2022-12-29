// main.cpp

#include "Logger.h"
#include "PlayerManager.h"

int main() {
    // Create an object of type TaskManager
    PlayerManager player_manager;
    int message_count = 10;
    LOG(INFO) << "Starting same process messaging";
    player_manager.startSameProcessMessaging(message_count);

    LOG(INFO) << "---------------------------------";
    LOG(INFO) << "Starting separate process messaging";
    PlayerManager ipc_player_manager;
    ipc_player_manager.startSeparateProcessMessaging(message_count);
    return 0;
}
