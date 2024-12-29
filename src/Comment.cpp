#include "../include/Comment.h"
#include "../include/User.h"
#include <algorithm>
#include <ctime>

Comment::~Comment() {
    for (auto reply : replies) {
        delete reply;
    }
}

void Comment::addLike(User* user) {
    if (std::find(likes.begin(), likes.end(), user) == likes.end()) {
        likes.push_back(user);
    }
}

void Comment::removeLike(User* user) {
    auto it = std::find(likes.begin(), likes.end(), user);
    if (it != likes.end()) {
        likes.erase(it);
    }
}

void Comment::addReply(User* user, const std::string& content) {
    Comment* reply = new Comment(user, content);
    replies.push_back(reply);
}

void Comment::removeReply(Comment* reply) {
    auto it = std::find(replies.begin(), replies.end(), reply);
    if (it != replies.end()) {
        replies.erase(it);
        delete reply;
    }
}
