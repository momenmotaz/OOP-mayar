#include "../include/Post.h"
#include "../include/User.h"

Post::Post(User* user, const std::string& content, const std::string& timestamp, PostPrivacy privacy)
    : user(user), content(content), timestamp(timestamp), privacy(privacy) {
    id = nextId++;
}

Post::~Post() {
    for (auto comment : comments) {
        delete comment;
    }
}

void Post::tagUser(User* user) {
    if (std::find(taggedUsers.begin(), taggedUsers.end(), user) == taggedUsers.end()) {
        taggedUsers.push_back(user);
    }
}

bool Post::canUserView(const User* viewer) const {
    if (!viewer) return false;
    
    if (privacy == PostPrivacy::PUBLIC) {
        return true;
    }
    
    if (viewer == user) {
        return true;
    }
    
    if (privacy == PostPrivacy::FRIENDS_ONLY) {
        return user->isFriend(viewer->getUsername());
    }
    
    return false;
}

Comment* Post::addComment(User* author, const std::string& content) {
    Comment* comment = new Comment(author, content);
    comments.push_back(comment);
    return comment;
}

std::string Post::getAuthorUsername() const {
    return user ? user->getUsername() : "";
}
