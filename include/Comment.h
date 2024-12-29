#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include "IReactable.h"

class User;

class Comment : public IReactable {
private:
    static inline int commentCounter = 0;
    int commentId;
    User* author;
    std::string content;
    std::vector<Comment*> replies;
    std::string timestamp;
    std::vector<User*> likes;

public:
    Comment(User* author, const std::string& content)
        : author(author), content(content) {}

    ~Comment();

    // Getters
    int getId() const { return commentId; }
    User* getAuthor() const { return author; }
    const std::string& getContent() const { return content; }
    const std::string& getTimestamp() const { return timestamp; }
    const std::vector<Comment*>& getReplies() const { return replies; }
    const std::vector<User*>& getLikes() const { return likes; }

    // Reply management
    void addReply(User* user, const std::string& content);
    void removeReply(Comment* reply);

    // IReactable interface implementation
    void addLike(User* user) override;
    void removeLike(User* user) override;

    void setContent(const std::string& newContent) { content = newContent; }
    void setTimestamp(const std::string& newTimestamp) { timestamp = newTimestamp; }
};
