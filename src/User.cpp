#include "../include/User.h"
#include <sstream>

User::User(const std::string& username, const std::string& email,
           const std::string& password, const std::string& gender)
    : username(username), email(email), password(password),
      gender(gender), isUserBot(false), isPublicProfile(true) {
}

void User::addFriend(const std::string& friendUsername) {
    if (std::find(friends.begin(), friends.end(), friendUsername) == friends.end()) {
        friends.push_back(friendUsername);
    }
}

void User::removeFriend(const std::string& friendUsername) {
    auto it = std::find(friends.begin(), friends.end(), friendUsername);
    if (it != friends.end()) {
        friends.erase(it);
    }
}

bool User::isFriend(const std::string& username) const {
    return std::find(friends.begin(), friends.end(), username) != friends.end();
}

bool User::hasFriend(const std::string& username) const {
    return std::find(friends.begin(), friends.end(), username) != friends.end();
}

void User::addFriendRequest(const std::string& fromUsername) {
    if (!hasFriendRequest(fromUsername)) {
        friendRequests.push_back(fromUsername);
    }
}

void User::removeFriendRequest(const std::string& fromUsername) {
    auto it = std::find(friendRequests.begin(), friendRequests.end(), fromUsername);
    if (it != friendRequests.end()) {
        friendRequests.erase(it);
    }
}

bool User::hasFriendRequest(const std::string& fromUsername) const {
    return std::find(friendRequests.begin(), friendRequests.end(), fromUsername) != friendRequests.end();
}

bool User::checkPassword(const std::string& password) const {
    return this->password == password;
}

bool User::changePassword(const std::string& oldPassword, const std::string& newPassword) {
    if (checkPassword(oldPassword)) {
        this->password = newPassword;
        return true;
    }
    return false;
}

void User::restrictFriend(const std::string& friendUsername) {
    if (isFriend(friendUsername) && 
        std::find(restrictedFriends.begin(), restrictedFriends.end(), friendUsername) == restrictedFriends.end()) {
        restrictedFriends.push_back(friendUsername);
    }
}

void User::unrestrictFriend(const std::string& friendUsername) {
    auto it = std::find(restrictedFriends.begin(), restrictedFriends.end(), friendUsername);
    if (it != restrictedFriends.end()) {
        restrictedFriends.erase(it);
    }
}

bool User::isRestrictedFriend(const std::string& username) const {
    return std::find(restrictedFriends.begin(), restrictedFriends.end(), username) != restrictedFriends.end();
}

void User::blockUser(const std::string& username) {
    // Remove from friends if they are a friend
    removeFriend(username);
    
    // Add to blocked users if not already blocked
    if (std::find(blockedUsers.begin(), blockedUsers.end(), username) == blockedUsers.end()) {
        blockedUsers.push_back(username);
    }
}

void User::unblockUser(const std::string& username) {
    auto it = std::find(blockedUsers.begin(), blockedUsers.end(), username);
    if (it != blockedUsers.end()) {
        blockedUsers.erase(it);
    }
}

bool User::isBlocked(const std::string& username) const {
    return std::find(blockedUsers.begin(), blockedUsers.end(), username) != blockedUsers.end();
}

std::string User::serialize() const {
    std::stringstream ss;
    ss << username << "|" 
       << email << "|"
       << password << "|"
       << gender << "|"
       << (isPublicProfile ? "1" : "0") << "|"
       << (isUserBot ? "1" : "0");
    
    // Serialize friends
    ss << "|";
    for (const auto& friend_ : friends) {
        ss << friend_ << ",";
    }
    
    // Serialize friend requests
    ss << "|";
    for (const auto& request : friendRequests) {
        ss << request << ",";
    }
    
    return ss.str();
}

User* User::deserialize(const std::string& data) {
    std::stringstream ss(data);
    std::string username, email, password, gender;
    std::string isPublicStr, botStatusStr;
    std::string friendsStr, requestsStr;
    
    std::getline(ss, username, '|');
    std::getline(ss, email, '|');
    std::getline(ss, password, '|');
    std::getline(ss, gender, '|');
    std::getline(ss, isPublicStr, '|');
    std::getline(ss, botStatusStr, '|');
    std::getline(ss, friendsStr, '|');
    std::getline(ss, requestsStr);
    
    User* user = new User(username, email, password, gender);
    user->setPublic(isPublicStr == "1");
    user->setBot(botStatusStr == "1");
    
    // Deserialize friends
    std::stringstream friendsSS(friendsStr);
    std::string friend_;
    while (std::getline(friendsSS, friend_, ',')) {
        if (!friend_.empty()) {
            user->addFriend(friend_);
        }
    }
    
    // Deserialize friend requests
    std::stringstream requestsSS(requestsStr);
    std::string request;
    while (std::getline(requestsSS, request, ',')) {
        if (!request.empty()) {
            user->addFriendRequest(request);
        }
    }
    
    return user;
}

std::vector<Post*> User::operator+(const User& other) const {
    std::vector<Post*> commonPosts;
    
    // If users are not friends or one has restricted/blocked the other, return empty vector
    if (!isFriend(other.getUsername()) || isRestrictedFriend(other.getUsername()) || 
        isBlocked(other.getUsername()) || other.isBlocked(getUsername())) {
        return commonPosts;
    }
    
    // Get posts that are visible to both users
    for (Post* post : posts) {
        if (post->getPrivacy() == PostPrivacy::PUBLIC || 
            post->getUser() == this || post->getUser() == &other) {
            commonPosts.push_back(post);
        }
    }
    
    for (Post* post : other.getPosts()) {
        if (post->getPrivacy() == PostPrivacy::PUBLIC || 
            post->getUser() == this || post->getUser() == &other) {
            commonPosts.push_back(post);
        }
    }
    
    return commonPosts;
}

std::vector<User*> User::operator&(const User& other) const {
    std::vector<User*> mutualFriends;
    
    // If users are not friends or one has restricted/blocked the other, return empty vector
    if (!isFriend(other.getUsername()) || isRestrictedFriend(other.getUsername()) || 
        isBlocked(other.getUsername()) || other.isBlocked(getUsername())) {
        return mutualFriends;
    }
    
    // Find common friends
    for (const auto& friendUsername : friends) {
        if (other.isFriend(friendUsername)) {
            // Note: This requires access to the FacebookSystem to get User* from username
            // For now, we'll return an empty vector
            // In a real implementation, we'd need to pass the FacebookSystem instance
            // or have some way to look up User* from username
        }
    }
    
    return mutualFriends;
}
