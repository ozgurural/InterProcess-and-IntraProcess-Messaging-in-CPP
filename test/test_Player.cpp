#include <gtest/gtest.h>
#include "TaskManager.h"

// Define a test for the addPacket() method
TEST(TaskManagerTest, addPacket) {
    // Create an object of type TaskManager
    TaskManager task_manager;

    // Create a packet with some data
    std::vector<uint8_t> packet_data = {1, 2, 3, 4};
    struct timeval packet_time = {5000, 0};
    std::shared_ptr<Packet> packet =
        std::make_shared<Packet>(packet_time, packet_data);

    // Add the packet to the queue
    task_manager.addPacket(packet);

    // Get the map of packets and tasks from the TaskManager object
    std::map<time_t, PacketsAndTasks> packets_and_tasks_map =
        task_manager.getPacketsAndTasks();

    // Check that the packet was added to the map
    EXPECT_EQ(packets_and_tasks_map[5000].packets.size(), 1);
    EXPECT_EQ(packets_and_tasks_map[5000].packets[0], packet);
}

// Define a test for the addTask() method
TEST(TaskManagerTest, addTask) {
    // Create an object of type TaskManager
    TaskManager task_manager;

    // Create a function that prints the packet data
    auto print_packet_data = [](std::shared_ptr<Packet>& packet) {
        std::cout << "Packet data: ";
        for (auto& byte : packet->data) {
            std::cout << std::hex << (int)byte << " ";
        }
        std::cout << std::endl;
    };

    // Create a periodic task that takes an interval and packet as arguments
    // and prints the packet data
    std::shared_ptr<PeriodicTask> task =
        std::make_shared<PeriodicTask>(5000, print_packet_data);

    // Add the task to the TaskManager object
    task_manager.addTask(task);

    // Get the map of packets and tasks from the TaskManager object
    std::map<time_t, PacketsAndTasks> packets_and_tasks_map =
        task_manager.getPacketsAndTasks();

    // Check that the task was added to the map
    EXPECT_EQ(packets_and_tasks_map[5000].tasks.size(), 1);
    EXPECT_EQ(packets_and_tasks_map[5000].tasks[0], task);
}

TEST(TaskManagerTest, TestAddTask) {
    // Create a TaskManager instance
    TaskManager taskManager;

    // Create a periodic task with an interval of 1 second
    auto task = PeriodicTaskFactory::createPeriodicTask(1, [](auto&) {});

    // Add the task to the TaskManager
    taskManager.addTask(task);

    // Check that the task was added to the map
    EXPECT_EQ(taskManager.getPacketsAndTasks()[1].tasks.size(), 1);
    EXPECT_EQ(taskManager.getPacketsAndTasks()[1].tasks[0], task);
}

// Test to add and remove a task
TEST(TaskManagerTest, TestAddAndRemoveTask) {
    // Create a TaskManager instance
    TaskManager taskManager;
    // Create a periodic task with an interval of 1 second and a lambda function
    // that increments a counter
    int counter = 0;
    auto task =
        std::make_shared<PeriodicTask>(1, [&counter](auto) { ++counter; });

    // Add the task to the TaskManager
    taskManager.addTask(task);
    // Check that the task has been added
    std::map<time_t, PacketsAndTasks>& packetsAndTasks =
        taskManager.getPacketsAndTasks();
    bool taskFound = false;
    for (auto& [interval, tasksAndPackets] : packetsAndTasks) {
        for (auto& t : tasksAndPackets.tasks) {
            if (t->getId() == task->getId()) {
                taskFound = true;
                break;
            }
        }
    }
    EXPECT_TRUE(taskFound);

    // Remove the task from the TaskManager
    taskManager.removeTask(task);
    // Check that the task has been removed
    taskFound = false;
    for (auto& [interval, tasksAndPackets] : packetsAndTasks) {
        for (auto& t : tasksAndPackets.tasks) {
            if (t->getId() == task->getId()) {
                taskFound = true;
                break;
            }
        }
    }
    EXPECT_FALSE(taskFound);
}

TEST(TaskManagerTest, TestChangeTaskInterval) {
    // Create a TaskManager instance
    TaskManager taskManager;

    // Create a periodic task with an interval of 1 second and a lambda function
    // that increments a counter
    int counter = 0;
    auto task =
        std::make_shared<PeriodicTask>(1, [&counter](auto) { ++counter; });

    // Add the task to the TaskManager
    taskManager.addTask(task);

    // Change the interval of the task to 2 seconds
    taskManager.setPeriodicTaskInterval(task, 2);

    // Check that the task has been moved to the 2-second interval
    std::map<time_t, PacketsAndTasks>& packetsAndTasks =
        taskManager.getPacketsAndTasks();
    bool taskFound = false;
    for (auto& [interval, tasksAndPackets] : packetsAndTasks) {
        if (interval == 2) {
            for (auto& t : tasksAndPackets.tasks) {
                if (t->getId() == task->getId()) {
                    taskFound = true;
                    break;
                }
            }
        }
    }
    EXPECT_TRUE(taskFound);
}
