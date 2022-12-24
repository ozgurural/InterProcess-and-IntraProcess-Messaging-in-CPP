#include <atomic>
#include <memory>
#include <mutex>
#include <string>

class Player : public std::enable_shared_from_this<Player> {
public:
    // Constructs a new player with the given name.
    explicit Player(std::string  name);

    // Sends a message to another player.
    void SendMessage(const std::shared_ptr<Player>& other, const std::string& message);

    // Receives a message from another player.
    void ReceiveMessage(const std::string& message);

private:
    // The name of the player.
    std::string name_;

    // The message counter for this player.
    std::atomic<int> counter_;

    // A mutex to synchronize access to shared data.
    std::mutex mutex_;
};
