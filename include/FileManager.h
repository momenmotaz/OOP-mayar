#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include "User.h"
#include "Post.h"
#include "Comment.h"
#include "Conversation.h"
#include "Exceptions.h"

class FileManager {
public:
    static const std::string USERS_FILE;
    static const std::string POSTS_FILE;
    static const std::string FRIENDS_FILE;

    static void loadData(std::vector<User*>& users, 
                        std::vector<Post*>& posts,
                        std::map<std::string, std::vector<std::pair<std::string, std::string>>>& conversations);

    static void saveData(const std::vector<User*>& users,
                        const std::vector<Post*>& posts,
                        const std::map<std::string, std::vector<std::pair<std::string, std::string>>>& conversations);

    static void loadUsers(std::vector<User*>& users);
    static void loadPosts(std::vector<User*>& users, std::vector<Post*>& posts);
    static void loadConversations(std::map<std::string, std::vector<std::pair<std::string, std::string>>>& conversations);
    static void loadFriendships(std::vector<User*>& users);

    static void saveUsers(const std::vector<User*>& users);
    static void savePosts(const std::vector<Post*>& posts);
    static void saveConversations(const std::map<std::string, std::vector<std::pair<std::string, std::string>>>& conversations);
    static void saveFriendships(const std::vector<User*>& users);

private:
    static std::vector<std::string> readLines(const std::string& filename);
    static void writeLines(const std::string& filename, const std::vector<std::string>& lines);
};

#endif
