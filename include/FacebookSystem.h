#ifndef FACEBOOKSYSTEM_H
#define FACEBOOKSYSTEM_H

#include "User.h"
#include "Post.h"
#include "Conversation.h"
#include "Message.h"
#include <vector>
#include <string>
#include <map>
#include <chrono>
#include <ctime>

class FacebookSystem {
private:
    std::vector<User*> users;
    std::vector<Post*> posts;
    std::map<std::string, std::vector<std::string>> notifications;
    std::map<std::string, std::vector<std::pair<std::string, std::string>>> conversations;
    User* currentUser;

    void AddDefaultBots();
    void CreateDefaultBots();
    void CreateBotPosts(User* bot);
    std::string createChatKey(const std::string& user1, const std::string& user2) const;
    void createDefaultUsers();
    void SendBotFriendRequests();
    std::string getCurrentTimestamp() const;

public:
    FacebookSystem();
    ~FacebookSystem();

    void loadUsers();
    void loadFriends();
    void loadPosts();
    void loadMessages();
    void saveMessages();
    void saveUsersToFile();
    void saveFriends();
    void savePosts();

    bool login(const std::string& email, const std::string& password);
    void logout();
    bool registerUser(const std::string& username, const std::string& email,
                     const std::string& password, const std::string& gender);
    bool resetPassword(const std::string& email, const std::string& securityAnswer,
                      const std::string& newPassword);

    void createPost(const std::string& content);
    Post* createPost(const std::string& content, PostPrivacy privacy);
    Post* createPost(const std::string& content, User* author);
    void likePost(Post* post);
    void likePost(int postId);
    void commentOnPost(int postId, const std::string& comment);
    void sharePost(int postId);

    std::vector<Post*> searchPosts(const std::string& query) const;
    std::vector<User*> searchUsers(const std::string& query) const;
    
    bool sendFriendRequest(const std::string& username);
    void acceptFriendRequest(const std::string& username);
    void rejectFriendRequest(const std::string& username);
    void removeFriend(const std::string& username);
    bool areFriends(const User* user1, const User* user2) const;
    bool hasPendingFriendRequest(const User* fromUser, const User* toUser) const;
    User* findUserByUsername(const std::string& username) const;
    User* findUserByEmail(const std::string& email) const;
    bool isValidEmail(const std::string& email) const;
    
    void sendMessage(const std::string& to, const std::string& message);
    std::vector<std::pair<std::string, std::string>> getMessages(const std::string& withUsername) const;
    
    void addNotification(User* user, const std::string& message);
    std::vector<std::string> getNotifications() const;
    void clearNotifications();

    User* getCurrentUser() const { return currentUser; }
    const std::vector<User*>& getUsers() const { return users; }
    const std::vector<Post*>& getPosts() const { return posts; }
    const std::map<std::string, std::vector<std::pair<std::string, std::string>>>& getConversations() const { return conversations; }
};

#endif
