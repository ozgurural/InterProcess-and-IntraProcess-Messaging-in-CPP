#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

#include <atomic>
#include <condition_variable>
#include <map>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

#include "Logger.h"
#include "Packet.h"
#include "PeriodicTaskFactory.h"

struct PacketsAndTasks {
    std::vector<std::shared_ptr<Packet>> packets;
    std::vector<std::shared_ptr<PeriodicTask>> tasks;
};

class TaskManager {
public:
    static std::chrono::time_point<std::chrono::steady_clock> getTimeSource();

    void addTask(const std::shared_ptr<PeriodicTask>&);
    void removeTask(std::shared_ptr<PeriodicTask> task);
    void setPeriodicTaskInterval(std::shared_ptr<PeriodicTask> task,
                                 time_t interval_sec);

    void addPacket(const std::shared_ptr<Packet>&);

    void startAllTasks();
    void stopAllTasks();
    void onNewTime(struct timeval aCurrentTime);
    void processPackets();
    std::map<time_t, PacketsAndTasks>& getPacketsAndTasks();
    timeval getCurrentTime() { return currentTime_; }
    // Destructor
    ~TaskManager();

private:
    // current time
    timeval currentTime_ = {0, 0};

    // Mutex for thread-safety
    std::mutex mutex_;
    std::mutex packet_queue_mutex_;

    // Thread used to execute periodic tasks
    std::jthread task_thread_;
    std::jthread packet_thread_;

    // Map containing managed periodic tasks and Packages
    std::map<time_t, PacketsAndTasks> packets_and_tasks_map_;

    // Queue of packets to be processed
    std::queue<std::unique_ptr<Packet>> incoming_packet_queue_;

    // Condition variable used to signal the task thread to wake up
    std::condition_variable packet_queue_cv_;

    void taskThreadFunc();
    std::atomic<bool> stop_{false};
};

#endif  // TASK_MANAGER_H
