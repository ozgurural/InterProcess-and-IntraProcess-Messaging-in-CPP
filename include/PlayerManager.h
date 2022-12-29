#ifndef PLAYER_MANAGER_H
#define PLAYER_MANAGER_H

#include <sys/wait.h>
#include <unistd.h>
#include <atomic>

#include "Logger.h"
#include "Player.h"

class PlayerManager {
public:
    void stopMessaging();
    static void startSameProcessMessaging(int message_count);
    static void startSeparateProcessMessaging(int message_count);

private:
};

#endif  // PLAYER_MANAGER_H
