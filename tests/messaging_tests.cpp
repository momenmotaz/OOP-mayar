#include <gtest/gtest.h>
#include "../include/User.h"
#include "../include/Message.h"
#include "../include/Conversation.h"

class MessagingTest : public ::testing::Test {
protected:
    void SetUp() override {
        user1 = new User("user1", "user1@test.com", "pass123");
        user2 = new User("user2", "user2@test.com", "pass123");
        conversation = new Conversation(user1, user2);
    }

    void TearDown() override {
        delete user1;
        delete user2;
        delete conversation;
    }

    User* user1;
    User* user2;
    Conversation* conversation;
};

// Message Tests
TEST_F(MessagingTest, MessageCreation) {
    Message* msg = new Message(user1, "Hello!", "2024-01-01 10:00");
    EXPECT_EQ(msg->getSender(), user1);
    EXPECT_EQ(msg->getContent(), "Hello!");
    EXPECT_EQ(msg->getTimestamp(), "2024-01-01 10:00");
    delete msg;
}

// Conversation Tests
TEST_F(MessagingTest, ConversationCreation) {
    EXPECT_GE(conversation->getId(), 0);
    EXPECT_EQ(conversation->getMessages().size(), 0);
    EXPECT_TRUE(conversation->hasParticipant(user1));
    EXPECT_TRUE(conversation->hasParticipant(user2));
}

TEST_F(MessagingTest, AddingMessages) {
    Message* msg1 = new Message(user1, "Hi Mohamed!", "2024-01-01 10:00");
    Message* msg2 = new Message(user2, "Hi Ahmed!", "2024-01-01 10:01");
    
    conversation->addMessage(msg1);
    conversation->addMessage(msg2);
    
    auto messages = conversation->getMessages();
    EXPECT_EQ(messages.size(), 2);
    EXPECT_EQ(messages[0]->getSender(), user1);
    EXPECT_EQ(messages[0]->getContent(), "Hi Mohamed!");
    EXPECT_EQ(messages[1]->getSender(), user2);
    EXPECT_EQ(messages[1]->getContent(), "Hi Ahmed!");
}

TEST_F(MessagingTest, ConversationMessageOrder) {
    Message* msg1 = new Message(user1, "First message", "2024-01-01 10:00");
    Message* msg2 = new Message(user2, "Second message", "2024-01-01 10:01");
    Message* msg3 = new Message(user1, "Third message", "2024-01-01 10:02");
    
    conversation->addMessage(msg1);
    conversation->addMessage(msg2);
    conversation->addMessage(msg3);
    
    auto messages = conversation->getMessages();
    EXPECT_EQ(messages.size(), 3);
    EXPECT_EQ(messages[0]->getTimestamp(), "2024-01-01 10:00");
    EXPECT_EQ(messages[1]->getTimestamp(), "2024-01-01 10:01");
    EXPECT_EQ(messages[2]->getTimestamp(), "2024-01-01 10:02");
}

// Test message content validation
TEST_F(MessagingTest, MessageContentValidation) {
    // Test empty message
    Message* emptyMsg = new Message(user1, "", "2024-01-01 10:00");
    EXPECT_EQ(emptyMsg->getContent(), "");
    delete emptyMsg;
    
    // Test long message
    std::string longContent(1000, 'a'); // 1000 character message
    Message* longMsg = new Message(user1, longContent, "2024-01-01 10:00");
    EXPECT_EQ(longMsg->getContent().length(), 1000);
    delete longMsg;
}

// Test Message Access
TEST_F(MessagingTest, MessageAccess) {
    Message* msg = new Message(user1, "Hello", "2024-01-01 10:00");
    conversation->addMessage(msg);
    EXPECT_TRUE(conversation->hasParticipant(user1));
    EXPECT_TRUE(conversation->hasParticipant(user2));
    
    User* user3 = new User("user3", "user3@test.com", "pass123");
    EXPECT_FALSE(conversation->hasParticipant(user3));
    delete user3;
}
