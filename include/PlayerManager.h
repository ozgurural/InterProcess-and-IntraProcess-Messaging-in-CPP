#ifndef PLAYER_MANAGER_H
#define PLAYER_MANAGER_H

#include <sys/wait.h>
#include <unistd.h>

#include "Logger.h"
#include "Player.h"


class PlayerManager {
public:

    void startMessaging(bool same_process, int message_count);
    void stopMessaging();

    // Destructor
    ~PlayerManager();

private:
    void sameProcess();
    void separateProcess();

};

#endif  // PLAYER_MANAGER_H
