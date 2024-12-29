#include <gtest/gtest.h>
#include "../include/FacebookSystem.h"
#include <filesystem>
#include <iostream>

class FacebookSystemTest : public ::testing::Test {
protected:
    void SetUp() override {
        try {
            // Remove existing data files before each test
            std::filesystem::path dataDir = std::filesystem::current_path().parent_path() / "data";
            std::cout << "Data directory path: " << dataDir << std::endl;

            if (std::filesystem::exists(dataDir)) {
                std::filesystem::remove(dataDir / "users.txt");
                std::filesystem::remove(dataDir / "friends.txt");
                std::filesystem::remove(dataDir / "posts.txt");
                std::filesystem::remove(dataDir / "messages.txt");
                std::cout << "Removed existing data files" << std::endl;
            }

            std::filesystem::create_directories(dataDir);
            std::cout << "Created data directory" << std::endl;
            
            system = new FacebookSystem();
            EXPECT_TRUE(system->registerUser("ahmed", "ahmed@test.com", "pass123", "male"));
            EXPECT_TRUE(system->registerUser("mohamed", "mohamed@test.com", "pass456", "male"));
            EXPECT_TRUE(system->registerUser("sara", "sara@test.com", "pass789", "female"));
        } catch (const std::exception& e) {
            std::cout << "Error in SetUp: " << e.what() << std::endl;
            throw;
        }
    }

    void TearDown() override {
        delete system;
    }

    FacebookSystem* system;
};

// User Management Tests
TEST_F(FacebookSystemTest, UserRegistration) {
    // Test registering a new user
    EXPECT_TRUE(system->registerUser("testuser", "test@test.com", "testpass", "male"));
    
    // Test registering with existing email
    EXPECT_FALSE(system->registerUser("newuser", "ahmed@test.com", "pass123", "male"));
}

TEST_F(FacebookSystemTest, UserLogin) {
    // Test successful login
    EXPECT_TRUE(system->login("ahmed@test.com", "pass123"));
    
    // Test failed login
    EXPECT_FALSE(system->login("ahmed@test.com", "wrongpass"));
    EXPECT_FALSE(system->login("nonexistent@test.com", "pass123"));
}

// Friend Management Tests
TEST_F(FacebookSystemTest, FriendRequests) {
    // Login as ahmed
    EXPECT_TRUE(system->login("ahmed@test.com", "pass123"));
    
    // Send friend request to mohamed
    EXPECT_TRUE(system->sendFriendRequest("mohamed"));
    
    // Login as mohamed and accept the request
    system->logout();
    EXPECT_TRUE(system->login("mohamed@test.com", "pass456"));
    system->acceptFriendRequest("ahmed");
    
    // Verify they are friends
    EXPECT_TRUE(system->areFriends(system->getCurrentUser(), system->findUserByEmail("ahmed@test.com")));
}

TEST_F(FacebookSystemTest, RejectFriendRequest) {
    // Login as ahmed
    EXPECT_TRUE(system->login("ahmed@test.com", "pass123"));
    
    // Send friend request to sara
    EXPECT_TRUE(system->sendFriendRequest("sara"));
    
    // Login as sara and reject the request
    system->logout();
    EXPECT_TRUE(system->login("sara@test.com", "pass789"));
    system->rejectFriendRequest("ahmed");
    
    // Verify they are not friends
    EXPECT_FALSE(system->areFriends(system->getCurrentUser(), system->findUserByEmail("ahmed@test.com")));
}

// System State Tests
TEST_F(FacebookSystemTest, Logout) {
    system->login("ahmed@test.com", "pass123");
    system->logout();
    EXPECT_EQ(system->getCurrentUser(), nullptr);
}

TEST_F(FacebookSystemTest, NonFriendUsers) {
    // Login as ahmed
    EXPECT_TRUE(system->login("ahmed@test.com", "pass123"));
    
    // Search for users
    auto users = system->searchUsers("");
    EXPECT_GT(users.size(), 0);
}

// Post Management Tests
TEST_F(FacebookSystemTest, CreateAndInteractWithPost) {
    system->login("ahmed@test.com", "pass123");
    
    // Create post
    system->createPost("Test post content");
    auto posts = system->getPosts();
    EXPECT_EQ(posts.size(), 1);
    EXPECT_EQ(posts[0]->getContent(), "Test post content");
    
    // Like post
    int postId = posts[0]->getId();
    system->likePost(postId);
    EXPECT_EQ(posts[0]->getLikes().size(), 1);
    
    // Comment on post
    system->commentOnPost(postId, "Test comment");
    EXPECT_EQ(posts[0]->getComments().size(), 1);
    EXPECT_EQ(posts[0]->getComments()[0]->getContent(), "Test comment");
}

TEST_F(FacebookSystemTest, PostVisibility) {
    system->login("ahmed@test.com", "pass123");
    system->createPost("Public post");
    
    // Login as another user
    system->logout();
    system->login("mohamed@test.com", "pass456");
    auto visiblePosts = system->getPosts();
    EXPECT_GE(visiblePosts.size(), 1);
    
    // Check if the post is visible
    bool foundPost = false;
    for (const auto& post : visiblePosts) {
        if (post->getContent() == "Public post") {
            foundPost = true;
            break;
        }
    }
    EXPECT_TRUE(foundPost);
}
