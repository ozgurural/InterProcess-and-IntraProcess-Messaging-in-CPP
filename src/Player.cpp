#include "TaskManager.h"

TaskManager::~TaskManager() {
    // Clean up resources and stop any threads
    stopAllTasks();
}

// Define the time_source_() function.
std::chrono::time_point<std::chrono::steady_clock>
TaskManager::getTimeSource() {
    // Return the current time according to a monotonic clock.
    return std::chrono::steady_clock::now();
}

// Method to process packets from the current source until there are no more
// packets.
void TaskManager::processPackets() {
    while (!stop_) {
        // Log ProcessPackets thread
        LOG(INFO) << "Process Packets thread running";

        try {
            // Wait for 500 milliseconds
            std::this_thread::sleep_for(std::chrono::milliseconds(500));

            // If the stop flag is set or the queue is empty, continue to the
            // next iteration of the loop
            if (stop_ || incoming_packet_queue_.empty()) {
                continue;
            }

            // Pop the next packet from the queue
            std::unique_lock<std::mutex> lock(packet_queue_mutex_);
            auto packet = std::move(incoming_packet_queue_.front());
            incoming_packet_queue_.pop();
            lock.unlock();

            // If the packet has a new timestamp, call onNewTime()
            if (currentTime_.tv_sec != packet->time.tv_sec) {
                onNewTime(packet->time);
            }

            // Add packet to the packets_and_tasks_map_
            addPacket(std::move(packet));
            LOG(INFO) << "Packet processed";
        } catch (const std::exception& e) {
            // Log the exception that was thrown
            LOG(ERROR) << "Exception thrown in processPackets(): " << e.what();
        }
    }
}

void TaskManager::onNewTime(struct timeval aCurrentTime) {
    std::lock_guard<std::mutex> lock(mutex_);
    currentTime_ = aCurrentTime;
    LOG(TRACE) << "New time: " << currentTime_.tv_sec << " seconds";
}

void TaskManager::addPacket(const std::shared_ptr<Packet>& packet) {
    std::lock_guard<std::mutex> lock(packet_queue_mutex_);
    // Log that a packet is being added
    LOG(INFO) << "Adding packet " << packet->time.tv_sec << " seconds";
    packets_and_tasks_map_[packet->time.tv_sec].packets.emplace_back(packet);

    LOG(INFO) << "Number of packets added: "
              << packets_and_tasks_map_[packet->time.tv_sec].packets.size();
}

void TaskManager::addTask(const std::shared_ptr<PeriodicTask>& task) {
    std::lock_guard<std::mutex> lock(mutex_);

    auto interval = task->getInterval();

    // Log that a task is being added
    LOG(INFO) << "Adding task " << task->getId() << " with interval "
              << interval << " seconds";

    LOG(INFO) << "Number of packets before adding task: "
              << packets_and_tasks_map_[interval].packets.size();

    packets_and_tasks_map_[interval].tasks.emplace_back(task);

    // After adding the task check packet size still same
    LOG(INFO) << "Number of packets after adding task: "
              << packets_and_tasks_map_[interval].packets.size();
}

void TaskManager::removeTask(std::shared_ptr<PeriodicTask> task) {
    std::lock_guard<std::mutex> lock(mutex_);

    // Log that a task is being removed
    LOG(INFO) << "Removing task " << task->getId() << " with interval "
              << task->getInterval() << " seconds";

    // tasks_ is a map from interval to vector of tasks
    // get the vector of tasks for the given interval
    auto& task_vec = packets_and_tasks_map_[task->getInterval()].tasks;

    // find the task with the matching id and remove it
    task_vec.erase(
        std::remove_if(task_vec.begin(), task_vec.end(),
                       [&task](const std::shared_ptr<PeriodicTask>& t) {
                           return t->getId() == task->getId();
                       }),
        task_vec.end());
}

void TaskManager::setPeriodicTaskInterval(std::shared_ptr<PeriodicTask> task,
                                          const time_t interval) {
    std::lock_guard<std::mutex> lock(mutex_);

    // Log that the interval of a task is being changed
    LOG(INFO) << "Changing interval of task " << task->getId() << " from "
              << task->getInterval() << " seconds to " << interval
              << " seconds";

    // tasks_ is a map from interval to vector of tasks
    // get the vector of tasks for the old interval
    auto& task_vec = packets_and_tasks_map_[task->getInterval()].tasks;

    // find the task with the matching id and remove it from the vector
    task_vec.erase(
        std::remove_if(task_vec.begin(), task_vec.end(),
                       [&task](const std::shared_ptr<PeriodicTask>& t) {
                           return t->getId() == task->getId();
                       }),
        task_vec.end());

    // Set the new interval for the task
    task->setInterval(interval);

    // Add the task to the vector of tasks for the new interval
    packets_and_tasks_map_[interval].tasks.emplace_back(task);
}

void TaskManager::startAllTasks() {
    // Log that the task manager is starting
    LOG(INFO) << "Starting task manager";

    task_thread_ = std::jthread(&TaskManager::taskThreadFunc, this);
    packet_thread_ = std::jthread(&TaskManager::processPackets, this);
}

void TaskManager::stopAllTasks() {
    // Log that the task manager is stopping
    std::lock_guard<std::mutex> lock(packet_queue_mutex_);
    stop_ = true;

    // Log that the task thread is stopping
    LOG(INFO) << "Stopping task thread";
    if (task_thread_.joinable()) {
        task_thread_.join();
        // Log that task thread has stopped
        LOG(INFO) << "Task thread stopped";
    } else {
        LOG(WARNING) << "Task thread is not joinable";
    }

    // Log that the packet thread is stopping
    LOG(INFO) << "Stopping packet thread";
    if (packet_thread_.joinable()) {
        packet_thread_.join();
        // Log that packet thread has stopped
        LOG(INFO) << "Packet thread stopped";
    } else {
        LOG(WARNING) << "Packet thread is not joinable";
    }
}

std::map<time_t, PacketsAndTasks>& TaskManager::getPacketsAndTasks() {
    return packets_and_tasks_map_;
}

void TaskManager::taskThreadFunc() {
    while (!stop_) {
        std::lock_guard<std::mutex> lock(mutex_);

        // Log that the task thread is running
        LOG(INFO) << "Task thread running";

        // Get the PacketsAndTasks struct for the current interval
        auto& packets_and_tasks = packets_and_tasks_map_[currentTime_.tv_sec];

        // Iterate over the vector of tasks
        for (auto& task : packets_and_tasks.tasks) {
            // Check if the task needs to be executed
            if (task->isTimeToExecute()) {
                // Iterate over the packets and execute the task with each
                // packet
                for (auto& packet : packets_and_tasks.packets) {
                    // Log that the task manager is executing tasks
                    LOG(INFO) << "Executing tasks";
                    task->execute(packet);
                }
                // Update the last_executed_time
                task->setLastExecutedTime(getTimeSource());
            }
        }

        // Sleep for a short time to allow other threads to run
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}
