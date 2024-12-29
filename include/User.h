#ifndef USER_H
#define USER_H

#include <string>
#include <vector>
#include <algorithm>
#include <memory>
#include "Post.h"
#include "Exceptions.h"

class User {
private:
    std::string username;
    std::string email;
    std::string password;
    std::string gender;
    bool isUserBot;
    bool isPublicProfile;
    std::vector<std::string> friends;
    std::vector<std::string> friendRequests;
    std::vector<std::string> restrictedFriends;
    std::vector<std::string> blockedUsers;
    std::vector<Post*> posts;

public:
    User(const std::string& username, const std::string& email,
         const std::string& password, const std::string& gender = "");
      
    ~User() {
        for (Post* post : posts) {
            delete post;
        }
    }

    // Getters
    const std::string& getUsername() const { return username; }
    const std::string& getEmail() const { return email; }
    const std::string& getPassword() const { return password; }
    const std::string& getGender() const { return gender; }
    bool isBot() const { return isUserBot; }
    bool isPublic() const { return isPublicProfile; }
    const std::vector<std::string>& getFriends() const { return friends; }
    const std::vector<std::string>& getFriendRequests() const { return friendRequests; }
    const std::vector<std::string>& getRestrictedFriends() const { return restrictedFriends; }
    const std::vector<std::string>& getBlockedUsers() const { return blockedUsers; }
    const std::vector<Post*>& getPosts() const { return posts; }

    // Setters
    void setUsername(const std::string& username) { this->username = username; }
    void setEmail(const std::string& email) { this->email = email; }
    void setPassword(const std::string& password) { this->password = password; }
    void setGender(const std::string& gender) { this->gender = gender; }
    void setBot(bool bot) { isUserBot = bot; }
    void setPublic(bool pub) { isPublicProfile = pub; }
    
    // Friend management
    void addFriend(const std::string& friendUsername);
    void removeFriend(const std::string& friendUsername);
    void addFriendRequest(const std::string& fromUsername);
    void removeFriendRequest(const std::string& fromUsername);
    bool hasFriend(const std::string& username) const;
    bool hasFriendRequest(const std::string& username) const;
    bool isFriend(const std::string& username) const;
    
    // Password management
    bool checkPassword(const std::string& password) const;
    bool changePassword(const std::string& oldPassword, const std::string& newPassword);
    
    // Friend restrictions
    void restrictFriend(const std::string& friendUsername);
    void unrestrictFriend(const std::string& friendUsername);
    bool isRestrictedFriend(const std::string& username) const;
    
    // User blocking
    void blockUser(const std::string& username);
    void unblockUser(const std::string& username);
    bool isBlocked(const std::string& username) const;
    
    // Post management
    void addPost(Post* post) {
        if (post) {
            posts.push_back(post);
        }
    }

    bool removePost(Post* post) {
        auto it = std::find(posts.begin(), posts.end(), post);
        if (it == posts.end()) return false;
        
        posts.erase(it);
        delete post;
        return true;
    }
    
    // Serialization
    std::string serialize() const;
    static User* deserialize(const std::string& data);
    
    // Operators
    std::vector<Post*> operator+(const User& other) const; // Common posts
    std::vector<User*> operator&(const User& other) const; // Mutual friends
};

#endif
