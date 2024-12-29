#pragma once

#include <vector>
#include "Message.h"

class User;

class Conversation {
private:
    static inline int nextId = 0;
    int id;
    User* user1;
    User* user2;
    std::vector<Message*> messages;

public:
    Conversation(User* u1, User* u2) : id(nextId++), user1(u1), user2(u2) {}

    ~Conversation() {
        for (auto msg : messages) {
            delete msg;
        }
    }

    int getId() const { return id; }
    const std::vector<Message*>& getMessages() const { return messages; }
    
    void addMessage(Message* message) {
        messages.push_back(message);
    }

    bool hasParticipant(User* user) const {
        return user == user1 || user == user2;
    }
};
