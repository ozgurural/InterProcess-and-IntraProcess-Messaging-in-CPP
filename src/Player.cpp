#include "Player.h"

Player::Player(std::string name,
               int message_count,
               int pipe_fd1,
               int pipe_fd2,
               bool send_state)
    : name_(std::move(name)),
      counter_(0),
      message_count_(message_count),
      pipe_fd1_(pipe_fd1),
      pipe_fd2_(pipe_fd2),
      write_state_(send_state) {
    // Initialize the semaphore with a value of 1.
    sem_init(&semaphore_, 0, 1);
    message_ = "Hello World";
}

/*
Player::~Player() {
  // Release any resources held by the object
  if (pipe_fd1_ != -1) close(pipe_fd1_);
  if (pipe_fd2_ != -1) close(pipe_fd2_);
  sem_destroy(&semaphore_);
}
*/

void Player::SameProcessSendMessage(const std::shared_ptr<Player>& other,
                                    const std::string& message) {
    // Lock the recursive mutex.
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    LOG(INFO) << "Lock acquired by " << name_ << " (Process Id: " << getpid()
              << ")";

    // Increment the message counter.
    ++counter_;

    // Check if the termination condition has been reached.
    if (counter_ == message_count_ + 1) {
        LOG(INFO) << "Termination condition reached by " << name_
                  << " (Process Id: " << getpid() << ")";
        terminate_ = true;
        return;
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

    if (counter_ == message_count_) {
        terminate_ = true;
        return;
    }
}

void Player::WritePipeThread() {
    // Repeat this process until the stop condition is reached
    while (counter_ < message_count_ - 1) {
        // wait for the write thread to be ready
        std::unique_lock<std::mutex> lk(m_);
        cv.wait(lk, [this] { return write_state_; });

        // Increment the message counter.
        ++counter_;

        message_ += " " + std::to_string(counter_);

        LOG(INFO) << name_ << "(Process Id: " << getpid()
                  << "): Sending message " << counter_ << ": " << message_
                  << " ";

        long bytes_written =
            write(pipe_fd2_, message_.c_str(), message_.size() + 1);
        if (bytes_written < 0) {
            // An error occurred while writing to the pipe
            LOG(ERROR) << "Error occurred while writing to the pipe: "
                       << strerror(errno);
            return;
        }

        // update the write state
        write_state_ = false;
    }
    LOG(INFO) << name_ << "(Process Id: " << getpid()
              << "): WritePipeThread is terminating";
}

void Player::ReadPipeThread() {
    while (counter_ < message_count_ - 1) {
        std::string received_message;
        char c;
        while (read(pipe_fd1_, &c, 1) > 0 && c != '\0') {
            received_message += c;
            // LOG(INFO) << "received_message: " << received_message;
        }
        if (received_message.empty()) {
            // Handle end of file
            LOG(INFO) << "received_message is empty";
        } else {
            // Increment the message counter.
            ++counter_;

            message_ = received_message;

            LOG(INFO) << name_ << "(Process Id: " << getpid()
                      << "): Received message " << counter_ << ": " << message_;
        }

        std::lock_guard<std::mutex> lock(m_);
        write_state_ = true;
        cv.notify_one();  // Notify the WritePipeThread that it can write to the
                          // pipe now
    }
    LOG(INFO) << name_ << "(Process Id: " << getpid()
              << "): ReadPipeThread is terminating";
}

void Player::PipeCommunication() {
    if (pipe_fd1_ == -1 || pipe_fd2_ == -1) {
        LOG(INFO) << name_ << "(Process Id: " << getpid()
                  << "): Pipe is not initialized";
        return;
    }
    LOG(INFO) << name_ << "(Process Id: " << getpid()
              << "): Pipes are initialized";

    read_pipe_thread_ = std::jthread([this] { ReadPipeThread(); });
    write_pipe_thread_ = std::jthread([this] { WritePipeThread(); });
}

void Player::joinThreads() {
    std::lock_guard<std::mutex> lock(m_);

    if (read_pipe_thread_.joinable()) {
        read_pipe_thread_.join();
        LOG(INFO) << "Read pipe thread stopped";
    } else {
        LOG(WARNING) << "Read pipe thread is not joinable";
    }

    LOG(INFO) << "Write pipe thread is joining";
    if (write_pipe_thread_.joinable()) {
        write_pipe_thread_.join();
        LOG(INFO) << "Write pipe thread stopped";
    } else {
        LOG(WARNING) << "Write pipe thread is not joinable";
    }
}
