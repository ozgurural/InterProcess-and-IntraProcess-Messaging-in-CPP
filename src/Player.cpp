#include "Player.h"

Player::Player(std::string name, int message_count, int pipe_fd1, int pipe_fd2)
    : name_(std::move(name)),
      counter_(0),
      message_count_(message_count),
      pipe_fd1_(pipe_fd1),
      pipe_fd2_(pipe_fd2) {
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
    if (pipe_fd1_ == -1 || pipe_fd2_ == -1) {
        LOG(INFO) << "Pipe is not initialized";
        return;
    }

    // Wait on the semaphore to ensure only one process can access the pipe at a
    // time.
    sem_wait(&semaphore_);

    // Check if the termination condition has been reached.
    if (counter_ == message_count_ + 1) {
        terminate_ = true;
        exit(0);
    }

    // Read the message from the pipe one character at a time.
    std::string received_message;
    char c;
    while (read(pipe_fd2_, &c, 1) > 0 && c != '\0') {
        received_message += c;
    }

    if (received_message.empty() && counter_ != 0) {
        // Handle end of file
        // LOG(INFO) << "received_message is empty " << counter_;
    } else {
        // Increment the message counter.
        ++counter_;

        // Write the message to the pipe.
        std::string message_to_send = message + received_message;

        message_to_send.append(" " + std::to_string(counter_));

        // Print the message being sent with logger
        LOG(INFO) << name_ << "(Process Id: " << getpid()
                  << "): Sending message " << counter_ << ": "
                  << message_to_send;

        write(pipe_fd1_, message_to_send.c_str(), message_to_send.size() + 1);

        // clear the received message
        received_message.clear();

        // Clear the message to send
        message_to_send.clear();
    }

    // Release the semaphore.
    sem_post(&semaphore_);
}

void Player::SeparateProcessReceiveMessage() {
    if (pipe_fd1_ == -1 || pipe_fd2_ == -1) {
        LOG(INFO) << "Pipe is not initialized";
        return;
    }
    LOG(INFO) << "SeparateProcessReceiveMessage";
    sem_wait(&semaphore_);
    // Read the message from the pipe one character at a time.
    std::string received_message;
    char c;
    while (read(pipe_fd1_, &c, 1) > 0 && c != '\0') {
        received_message += c;
        // LOG(INFO) << "received_message: " << received_message;
    }
    LOG(INFO) << "the received_message: " << received_message;
    if (received_message.empty()) {
        // Handle end of file
        LOG(INFO) << "received_message is empty";
    } else {
        // Increment the message counter.
        ++counter_;

        // Print the message being received
        LOG(INFO) << name_ << "(Process Id: " << getpid()
                  << "): Received message " << counter_ << ": "
                  << received_message + '\0';

        // Check if the termination condition has been reached.
        if (counter_ == message_count_ + 1) {
            LOG(INFO) << "Terminating" << counter_ << " " << message_count_;
            terminate_ = true;
            exit(0);
        }

        received_message.append(" " + std::to_string(counter_));

        LOG(INFO) << received_message;

        // Write the received message back to the pipe.
        write(pipe_fd2_, received_message.c_str(), received_message.size() + 1);

        // Clear the received message
        received_message.clear();

        // Clear the buffer
        memset(&c, 0, sizeof(c));
    }

    // Release the semaphore.
    sem_post(&semaphore_);
}
