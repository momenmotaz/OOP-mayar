#pragma once

#include <string>
#include <vector>

class User;

class Message {
private:
    User* sender;
    std::string content;
    std::string timestamp;

public:
    Message(User* sender, const std::string& content, const std::string& timestamp)
        : sender(sender), content(content), timestamp(timestamp) {}

    User* getSender() const { return sender; }
    const std::string& getContent() const { return content; }
    const std::string& getTimestamp() const { return timestamp; }
};
