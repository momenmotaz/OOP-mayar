#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>
#include "../include/User.h"
#include "../include/Post.h"
#include "../include/Comment.h"
#include "../include/Conversation.h"
#include "../include/FileManager.h"
#include "../include/Exceptions.h"

class UserTest_AddFriend_Test : public ::testing::Test {
protected:
    void SetUp() override {
        user1 = new User("user1", "user1@test.com", "pass123");
        user2 = new User("user2", "user2@test.com", "pass123");
    }

    void TearDown() override {
        delete user1;
        delete user2;
    }

    User* user1;
    User* user2;
};

TEST_F(UserTest_AddFriend_Test, TestBody) {
    user1->addFriend(user2->getUsername());
    EXPECT_TRUE(user1->isFriend(user2->getUsername()));
    
    // Adding the same friend again should not cause issues
    user1->addFriend(user2->getUsername());
    EXPECT_TRUE(user1->isFriend(user2->getUsername()));
}

class UserTest_RemoveFriend_Test : public ::testing::Test {
protected:
    void SetUp() override {
        user1 = new User("user1", "user1@test.com", "pass123");
        user2 = new User("user2", "user2@test.com", "pass123");
    }

    void TearDown() override {
        delete user1;
        delete user2;
    }

    User* user1;
    User* user2;
};

TEST_F(UserTest_RemoveFriend_Test, TestBody) {
    user1->addFriend(user2->getUsername());
    EXPECT_TRUE(user1->isFriend(user2->getUsername()));
    user1->removeFriend(user2->getUsername());
    EXPECT_FALSE(user1->isFriend(user2->getUsername()));
}

class UserTest_RestrictFriend_Test : public ::testing::Test {
protected:
    void SetUp() override {
        user1 = new User("user1", "user1@test.com", "pass123");
        user2 = new User("user2", "user2@test.com", "pass123");
    }

    void TearDown() override {
        delete user1;
        delete user2;
    }

    User* user1;
    User* user2;
};

TEST_F(UserTest_RestrictFriend_Test, TestBody) {
    user1->addFriend(user2->getUsername());
    user1->restrictFriend(user2->getUsername());
    EXPECT_TRUE(user1->isRestrictedFriend(user2->getUsername()));
    EXPECT_TRUE(user1->isFriend(user2->getUsername())); // Should still be a friend
}

class UserTest_UnrestrictFriend_Test : public ::testing::Test {
protected:
    void SetUp() override {
        user1 = new User("user1", "user1@test.com", "pass123");
        user2 = new User("user2", "user2@test.com", "pass123");
    }

    void TearDown() override {
        delete user1;
        delete user2;
    }

    User* user1;
    User* user2;
};

TEST_F(UserTest_UnrestrictFriend_Test, TestBody) {
    user1->addFriend(user2->getUsername());
    user1->restrictFriend(user2->getUsername());
    user1->unrestrictFriend(user2->getUsername());
    EXPECT_FALSE(user1->isRestrictedFriend(user2->getUsername()));
    EXPECT_TRUE(user1->isFriend(user2->getUsername())); // Should still be a friend
}

class UserTest_BlockUser_Test : public ::testing::Test {
protected:
    void SetUp() override {
        user1 = new User("user1", "user1@test.com", "pass123");
        user2 = new User("user2", "user2@test.com", "pass123");
    }

    void TearDown() override {
        delete user1;
        delete user2;
    }

    User* user1;
    User* user2;
};

TEST_F(UserTest_BlockUser_Test, TestBody) {
    user1->addFriend(user2->getUsername());
    user1->blockUser(user2->getUsername());
    EXPECT_TRUE(user1->isBlocked(user2->getUsername()));
    EXPECT_FALSE(user1->isFriend(user2->getUsername())); // Should remove from friends
}

class UserTest_UnblockUser_Test : public ::testing::Test {
protected:
    void SetUp() override {
        user1 = new User("user1", "user1@test.com", "pass123");
        user2 = new User("user2", "user2@test.com", "pass123");
    }

    void TearDown() override {
        delete user1;
        delete user2;
    }

    User* user1;
    User* user2;
};

TEST_F(UserTest_UnblockUser_Test, TestBody) {
    user1->blockUser(user2->getUsername());
    user1->unblockUser(user2->getUsername());
    EXPECT_FALSE(user1->isBlocked(user2->getUsername()));
}

class UserTest_RestrictedFriendsList_Test : public ::testing::Test {
protected:
    void SetUp() override {
        user1 = new User("user1", "user1@test.com", "pass123");
    }

    void TearDown() override {
        delete user1;
    }

    User* user1;
};

TEST_F(UserTest_RestrictedFriendsList_Test, TestBody) {
    User* friend1 = new User("friend1", "friend1@test.com", "pass123", "male");
    User* friend2 = new User("friend2", "friend2@test.com", "pass123", "female");
    
    user1->addFriend(friend1->getUsername());
    user1->addFriend(friend2->getUsername());
    user1->restrictFriend(friend1->getUsername());
    
    EXPECT_EQ(user1->getRestrictedFriends().size(), 1);
    EXPECT_TRUE(user1->isRestrictedFriend(friend1->getUsername()));
    EXPECT_FALSE(user1->isRestrictedFriend(friend2->getUsername()));
    
    delete friend1;
    delete friend2;
}

class UserTest_CommonPosts_Test : public ::testing::Test {
protected:
    void SetUp() override {
        user1 = new User("user1", "user1@test.com", "pass123");
        user2 = new User("user2", "user2@test.com", "pass123");
    }

    void TearDown() override {
        delete user1;
        delete user2;
    }

    User* user1;
    User* user2;
};

TEST_F(UserTest_CommonPosts_Test, TestBody) {
    // Create posts for both users
    Post* post1 = new Post(user1, "Post 1", "2023-12-25", PostPrivacy::PUBLIC);
    Post* post2 = new Post(user2, "Post 2", "2023-12-25", PostPrivacy::PUBLIC);
    
    user1->addPost(post1);
    user2->addPost(post2);
    
    // Make users friends
    user1->addFriend(user2->getUsername());
    user2->addFriend(user1->getUsername());
    
    // Test operator+
    std::vector<Post*> commonPosts = *user1 + *user2;
    EXPECT_EQ(commonPosts.size(), 2);
    
    delete post1;
    delete post2;
}

class UserTest_MutualFriends_Test : public ::testing::Test {
protected:
    void SetUp() override {
        user1 = new User("user1", "user1@test.com", "pass123");
        user2 = new User("user2", "user2@test.com", "pass123");
    }

    void TearDown() override {
        delete user1;
        delete user2;
    }

    User* user1;
    User* user2;
};

TEST_F(UserTest_MutualFriends_Test, TestBody) {
    User* user3 = new User("user3", "user3@test.com", "pass123", "male");
    
    user1->addFriend(user2->getUsername());
    user2->addFriend(user1->getUsername());
    user1->addFriend(user3->getUsername());
    user2->addFriend(user3->getUsername());
    
    // Test operator&
    std::vector<User*> mutualFriends = *user1 & *user2;
    EXPECT_EQ(mutualFriends.size(), 1); // user3 should be a mutual friend
    
    delete user3;
}

class UserTest_OperatorPlus_Test : public ::testing::Test {
protected:
    void SetUp() override {
        user1 = new User("user1", "user1@test.com", "pass123");
        user2 = new User("user2", "user2@test.com", "pass123");
    }

    void TearDown() override {
        delete user1;
        delete user2;
    }

    User* user1;
    User* user2;
};

TEST_F(UserTest_OperatorPlus_Test, TestBody) {
    // Create posts for both users
    Post* post1 = new Post(user1, "Post 1", "2023-12-25", PostPrivacy::PUBLIC);
    Post* post2 = new Post(user2, "Post 2", "2023-12-25", PostPrivacy::PUBLIC);
    Post* post3 = new Post(user1, "Post 3", "2023-12-25", PostPrivacy::PRIVATE);
    
    user1->addPost(post1);
    user2->addPost(post2);
    user1->addPost(post3);
    
    // Test when users are not friends
    std::vector<Post*> commonPosts = *user1 + *user2;
    EXPECT_EQ(commonPosts.size(), 2); // Only public posts
    
    // Test when users are friends
    user1->addFriend(user2->getUsername());
    user2->addFriend(user1->getUsername());
    
    commonPosts = *user1 + *user2;
    EXPECT_EQ(commonPosts.size(), 3); // All posts
    
    // Test when one user restricts the other
    user1->restrictFriend(user2->getUsername());
    commonPosts = *user1 + *user2;
    EXPECT_EQ(commonPosts.size(), 0); // No posts
    
    delete post1;
    delete post2;
    delete post3;
}

class UserTest_OperatorAnd_Test : public ::testing::Test {
protected:
    void SetUp() override {
        user1 = new User("user1", "user1@test.com", "pass123");
        user2 = new User("user2", "user2@test.com", "pass123");
    }

    void TearDown() override {
        delete user1;
        delete user2;
    }

    User* user1;
    User* user2;
};

TEST_F(UserTest_OperatorAnd_Test, TestBody) {
    User* user3 = new User("user3", "user3@test.com", "pass123", "male");
    User* user4 = new User("user4", "user4@test.com", "pass123", "female");
    
    // Test when users are not friends
    std::vector<User*> mutualFriends = *user1 & *user2;
    EXPECT_EQ(mutualFriends.size(), 0);
    
    // Test when users are friends with common friend
    user1->addFriend(user2->getUsername());
    user2->addFriend(user1->getUsername());
    user1->addFriend(user3->getUsername());
    user2->addFriend(user3->getUsername());
    
    mutualFriends = *user1 & *user2;
    EXPECT_EQ(mutualFriends.size(), 1); // user3 should be a mutual friend
    
    // Test when one user has additional friends
    user1->addFriend(user4->getUsername());
    mutualFriends = *user1 & *user3;
    EXPECT_EQ(mutualFriends.size(), 0); // No mutual friends
    
    // Test when one user restricts the other
    user1->restrictFriend(user3->getUsername());
    mutualFriends = *user1 & *user2;
    EXPECT_EQ(mutualFriends.size(), 0); // No mutual friends due to restriction
    
    delete user3;
    delete user4;
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
