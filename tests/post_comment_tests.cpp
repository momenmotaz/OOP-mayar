#include <gtest/gtest.h>
#include "../include/Post.h"
#include "../include/Comment.h"
#include "../include/User.h"

class PostCommentTest : public ::testing::Test {
protected:
    void SetUp() override {
        user1 = new User("ahmed", "ahmed@test.com", "pass123");
        user2 = new User("mohamed", "mohamed@test.com", "pass456");
        post = new Post(user1, "Test post content", "2024-01-01 12:00:00", PostPrivacy::PUBLIC);
    }

    void TearDown() override {
        delete post;
        delete user1;
        delete user2;
    }

    User* user1;
    User* user2;
    Post* post;
};

// Post Tests
TEST_F(PostCommentTest, PostCreation) {
    EXPECT_EQ(post->getUser(), user1);
    EXPECT_EQ(post->getContent(), "Test post content");
    EXPECT_EQ(post->getTimestamp(), "2024-01-01 12:00:00");
    EXPECT_EQ(post->getPrivacy(), PostPrivacy::PUBLIC);
}

TEST_F(PostCommentTest, PostLikes) {
    post->addLike("ahmed");
    post->addLike("mohamed");
    // Try adding duplicate like
    post->addLike("ahmed");
    
    auto likes = post->getLikes();
    EXPECT_EQ(likes.size(), 2);
    EXPECT_TRUE(std::find(likes.begin(), likes.end(), "ahmed") != likes.end());
    EXPECT_TRUE(std::find(likes.begin(), likes.end(), "mohamed") != likes.end());
}

TEST_F(PostCommentTest, PostComments) {
    Comment* comment = post->addComment(user2, "Great post!");
    EXPECT_EQ(post->getComments().size(), 1);
    EXPECT_EQ(comment->getAuthor(), user2);
    EXPECT_EQ(comment->getContent(), "Great post!");
}

TEST_F(PostCommentTest, PostTagging) {
    post->tagUser(user2);
    auto tagged = post->getTaggedUsers();
    EXPECT_EQ(tagged.size(), 1);
    EXPECT_EQ(tagged[0], user2);
}

// Comment Tests
TEST_F(PostCommentTest, CommentCreation) {
    Comment* comment = new Comment(user2, "Test comment");
    comment->setTimestamp("2024-01-01 12:00:00");
    EXPECT_EQ(comment->getAuthor(), user2);
    EXPECT_EQ(comment->getContent(), "Test comment");
    EXPECT_EQ(comment->getTimestamp(), "2024-01-01 12:00:00");
    delete comment;
}

TEST_F(PostCommentTest, CommentReplies) {
    Comment* parentComment = new Comment(user1, "Parent comment");
    parentComment->setTimestamp("2024-01-01 12:00:00");
    parentComment->addReply(user2, "Reply 1");
    parentComment->addReply(user1, "Reply 2");
    
    auto replies = parentComment->getReplies();
    EXPECT_EQ(replies.size(), 2);
    EXPECT_EQ(replies[0]->getContent(), "Reply 1");
    EXPECT_EQ(replies[1]->getContent(), "Reply 2");
    
    delete parentComment;
}

TEST_F(PostCommentTest, CommentLikes) {
    Comment* comment = new Comment(user1, "Test comment");
    
    comment->addLike(user2);
    EXPECT_EQ(comment->getLikes().size(), 1);
    EXPECT_EQ(comment->getLikes()[0], user2);
    
    comment->removeLike(user2);
    EXPECT_EQ(comment->getLikes().size(), 0);
    
    delete comment;
}

// Test Privacy Settings
TEST_F(PostCommentTest, PostPrivacySettings) {
    Post* privatePost = new Post(user1, "Private post", "2024-01-01 12:00:00", PostPrivacy::PRIVATE);
    EXPECT_EQ(privatePost->getPrivacy(), PostPrivacy::PRIVATE);
    
    Post* friendsOnlyPost = new Post(user1, "Friends only", "2024-01-01 12:00:00", PostPrivacy::FRIENDS_ONLY);
    EXPECT_EQ(friendsOnlyPost->getPrivacy(), PostPrivacy::FRIENDS_ONLY);
    
    delete privatePost;
    delete friendsOnlyPost;
}

// Test Post Privacy Change
TEST_F(PostCommentTest, ChangePostPrivacy) {
    EXPECT_EQ(post->getPrivacy(), PostPrivacy::PUBLIC);
    post->setPrivacy(PostPrivacy::PRIVATE);
    EXPECT_EQ(post->getPrivacy(), PostPrivacy::PRIVATE);
}

// Test Comment Modification
TEST_F(PostCommentTest, CommentModification) {
    Comment* comment = new Comment(user2, "Test comment");
    comment->setTimestamp("2024-01-01 12:00:00");
    EXPECT_EQ(comment->getContent(), "Test comment");
    comment->setContent("Modified comment");
    EXPECT_EQ(comment->getContent(), "Modified comment");
    delete comment;
}

// Test Nested Comments
TEST_F(PostCommentTest, NestedComments) {
    Comment* parentComment = new Comment(user1, "Parent comment");
    parentComment->setTimestamp("2024-01-01 12:00:00");
    
    parentComment->addReply(user2, "Reply 1");
    auto replies = parentComment->getReplies();
    EXPECT_EQ(replies.size(), 1);
    
    replies[0]->addReply(user1, "Reply to reply");
    auto nestedReplies = replies[0]->getReplies();
    EXPECT_EQ(nestedReplies.size(), 1);
    
    delete parentComment;
}
