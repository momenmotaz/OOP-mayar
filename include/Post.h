#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include "IReactable.h"
#include "Comment.h"
#include "Exceptions.h"

class User;  // Forward declaration

enum class PostPrivacy {
    PUBLIC,
    FRIENDS_ONLY,
    PRIVATE
};

class Post {
private:
    static inline int nextId = 0;
    int id;
    User* user;
    std::string content;
    std::string timestamp;
    std::vector<std::string> likes;
    std::vector<Comment*> comments;
    std::vector<User*> taggedUsers;
    PostPrivacy privacy;

public:
    Post(User* user, const std::string& content, const std::string& timestamp, PostPrivacy privacy = PostPrivacy::PUBLIC);
    ~Post();

    int getId() const { return id; }
    User* getUser() const { return user; }
    const std::string& getContent() const { return content; }
    const std::string& getTimestamp() const { return timestamp; }
    PostPrivacy getPrivacy() const { return privacy; }
    std::string getAuthorUsername() const;
    
    const std::vector<std::string>& getLikes() const { return likes; }
    const std::vector<Comment*>& getComments() const { return comments; }
    const std::vector<User*>& getTaggedUsers() const { return taggedUsers; }
    
    void addLike(const std::string& username) {
        if (std::find(likes.begin(), likes.end(), username) == likes.end()) {
            likes.push_back(username);
        }
    }
    
    void removeLike(const std::string& username) {
        auto it = std::find(likes.begin(), likes.end(), username);
        if (it != likes.end()) {
            likes.erase(it);
        }
    }
    
    bool hasLiked(const std::string& username) const {
        return std::find(likes.begin(), likes.end(), username) != likes.end();
    }
    
    Comment* addComment(User* author, const std::string& content);
    void tagUser(User* user);
    void setPrivacy(PostPrivacy newPrivacy) { privacy = newPrivacy; }
    bool isUserTagged(const User* user) const {
        return std::find(taggedUsers.begin(), taggedUsers.end(), user) != taggedUsers.end();
    }
    bool canUserView(const User* viewer) const;
};
