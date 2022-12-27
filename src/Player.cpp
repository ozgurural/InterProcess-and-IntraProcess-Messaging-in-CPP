#include "Player.h"

Player::Player(std::string name, int message_count, int pipe_fd)
    : name_(std::move(name)),
      counter_(0),
      message_count_(message_count),
      pipe_fd_(pipe_fd) {
    // Initialize the semaphore with a value of 1.
    sem_init(&semaphore_, 0, 1);
}

void Player::SameProcessSendMessage(const std::shared_ptr<Player>& other,
                                    const std::string& message) {
    // Lock the recursive mutex.
    std::lock_guard<std::recursive_mutex> lock(mutex_);

    // Increment the message counter.
    ++counter_;

    // Check if the termination condition has been reached.
    if (counter_ == message_count_ + 1) {
        terminate_ = true;
        exit(0);
    }

    // Print the message being sent with logger
    LOG(INFO) << name_ << "(Process Id: " << getpid() << "): Sending message "
              << counter_ << ": " << message;

    // Receive the message and send a reply.
    auto self = shared_from_this();
    other->SameProcessReceiveMessage(message, self);

    if (pipe_fd_ == -1) {
        LOG(INFO) << "Pipe is not initialized";
        return;
    }
    // Write the message to the pipe.
    write(pipe_fd_, message.c_str(), message.size());
}

bool Player::ShouldTerminate() const {
    return terminate_;
}

void Player::SameProcessReceiveMessage(const std::string& message,
                                       const std::shared_ptr<Player>& sender) {
    // Lock the recursive mutex.
    std::lock_guard<std::recursive_mutex> lock(mutex_);

    // Increment the message counter.
    ++counter_;

    // Print the message being received
    LOG(INFO) << name_ << "(Process Id: " << getpid() << "): Received message "
              << counter_ << ": " << message;

    auto self = shared_from_this();
    // Create a shared_ptr to this Player object and pass it to the sender.
    sender->SameProcessSendMessage(self,
                                   message + " " + std::to_string(counter_));

    if (counter_ == message_count_ + 1) {
        terminate_ = true;
        exit(0);
    }
}

void Player::SeparateProcessSendMessage(const std::string& message) {
    if (pipe_fd_ == -1) {
        LOG(INFO) << "Pipe is not initialized";
        return;
    }

    // Wait on the semaphore to ensure only one process can access the pipe at a
    // time.
    sem_wait(&semaphore_);

    // Increment the message counter.
    ++counter_;

    // Check if the termination condition has been reached.
    if (counter_ == message_count_ + 1) {
        terminate_ = true;
        exit(0);
    }

    // Read the message from the pipe.
    char buffer[1024];
    int bytes_read = read(pipe_fd_, buffer, sizeof(buffer) - 1);
    buffer[bytes_read] = '\0';

    std::string received_message;
    if (bytes_read == 0) {
        // Handle end of file
    } else {
        received_message = buffer;
    }

    // Print the message being sent with logger
    LOG(INFO) << name_ << "(Process Id: " << getpid() << "): Sending message "
              << counter_ << ": " << message;

    // Write the message to the pipe.
    std::string message_to_send = message + received_message;
    message_to_send += '\0';  // null-terminate the string
    write(pipe_fd_, message_to_send.c_str(), message_to_send.size());

    // Release the semaphore.
    sem_post(&semaphore_);
}

void Player::SeparateProcessReceiveMessage() {
    if (pipe_fd_ == -1) {
        LOG(INFO) << "Pipe is not initialized";
        return;
    }

    sem_wait(&semaphore_);

    // Read the message from the pipe one character at a time.
    std::string received_message;
    char c;
    while (read(pipe_fd_, &c, 1) > 0 && c != '\0') {
        received_message += c;
    }

    if (received_message.empty()) {
        // Handle end of file
    } else {

        // Increment the message counter.
        ++counter_;
        
        // Print the message being received
        LOG(INFO) << name_ << "(Process Id: " << getpid()
                  << "): Received message " << counter_ << ": "
                  << received_message;



        // Check if the termination condition has been reached.
        if (counter_ == message_count_ + 1) {
            terminate_ = true;
            exit(0);
        }

        received_message.append(" " + std::to_string(counter_));

        // Write the received message back to the pipe.
        write(pipe_fd_, received_message.c_str(), received_message.size());
    }

    sem_post(&semaphore_);
}
