#include "../include/FileManager.h"
#include <filesystem>
#include <iostream>
#include <iomanip>
#include <set>

const std::string FileManager::USERS_FILE = "../data/users.txt";
const std::string FileManager::POSTS_FILE = "../data/posts.txt";
const std::string FileManager::FRIENDS_FILE = "../data/friends.txt";

std::vector<std::string> FileManager::readLines(const std::string& filename) {
    std::vector<std::string> lines;
    try {
        std::filesystem::path filePath(filename);
        std::filesystem::create_directories(filePath.parent_path());

        std::cout << std::setw(15) << std::left << "[File]" << "Reading: " << filename << std::endl;
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cout << std::setw(15) << std::left << "[Error]" << "Failed to open: " << filename << std::endl;
            throw FileOperationException();
        }
        std::string line;
        while (std::getline(file, line)) {
            if (!line.empty()) {
                lines.push_back(line);
            }
        }
        file.close();
        std::cout << std::setw(15) << std::left << "[Success]" << "Read " << lines.size() << " lines from " << filename << std::endl;
    } catch (const std::exception& e) {
        std::cout << std::setw(15) << std::left << "[Error]" << "Exception: " << e.what() << std::endl;
        throw FileOperationException();
    }
    return lines;
}

void FileManager::writeLines(const std::string& filename, const std::vector<std::string>& lines) {
    try {
        std::filesystem::path filePath(filename);
        std::filesystem::create_directories(filePath.parent_path());

        std::cout << std::setw(15) << std::left << "[File]" << "Writing to: " << filename << std::endl;
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cout << std::setw(15) << std::left << "[Error]" << "Failed to open for writing: " << filename << std::endl;
            throw FileOperationException();
        }
        for (const auto& line : lines) {
            file << line << std::endl;
        }
        file.close();
        std::cout << std::setw(15) << std::left << "[Success]" << "Wrote " << lines.size() << " lines to " << filename << std::endl;
    } catch (const std::exception& e) {
        std::cout << std::setw(15) << std::left << "[Error]" << "Exception: " << e.what() << std::endl;
        throw FileOperationException();
    }
}

void FileManager::loadUsers(std::vector<User*>& users) {
    try {
        std::cout << "\n" << std::setw(15) << std::left << "[Loading]" << "Users..." << std::endl;
        auto userLines = readLines(USERS_FILE);
        for (const auto& line : userLines) {
            std::vector<std::string> parts;
            std::stringstream ss(line);
            std::string part;
            while (std::getline(ss, part, '|')) {
                parts.push_back(part);
            }
            if (parts.size() >= 4) {
                users.push_back(new User(
                    parts[1], // username
                    parts[0], // email
                    parts[2], // password
                    parts[3]  // gender
                ));
            }
        }
        std::cout << std::setw(15) << std::left << "[Success]" << "Loaded " << users.size() << " users" << std::endl;
    } catch (const std::exception& e) {
        for (auto user : users) delete user;
        users.clear();
        std::cout << std::setw(15) << std::left << "[Error]" << "Failed to load users: " << e.what() << std::endl;
        throw FileOperationException();
    }
}

void FileManager::loadPosts(std::vector<User*>& users, std::vector<Post*>& posts) {
    try {
        std::cout << "\n" << std::setw(15) << std::left << "[Loading]" << "Posts..." << std::endl;
        auto postLines = readLines(POSTS_FILE);
        for (const auto& line : postLines) {
            std::vector<std::string> parts;
            std::stringstream ss(line);
            std::string part;
            while (std::getline(ss, part, '|')) {
                parts.push_back(part);
            }
            if (parts.size() >= 4) {
                User* author = nullptr;
                for (auto user : users) {
                    if (user->getUsername() == parts[0]) {
                        author = user;
                        break;
                    }
                }
                if (author) {
                    Post* newPost = new Post(
                        author,
                        parts[1], // content
                        parts[2], // timestamp
                        static_cast<PostPrivacy>(std::stoi(parts[3])) // privacy
                    );
                    posts.push_back(newPost);
                    author->addPost(newPost);
                }
            }
        }
        std::cout << std::setw(15) << std::left << "[Success]" << "Loaded " << posts.size() << " posts" << std::endl;
    } catch (const std::exception& e) {
        for (auto post : posts) delete post;
        posts.clear();
        std::cout << std::setw(15) << std::left << "[Error]" << "Failed to load posts: " << e.what() << std::endl;
        throw FileOperationException();
    }
}

void FileManager::loadFriendships(std::vector<User*>& users) {
    try {
        std::cout << "\n" << std::setw(15) << std::left << "[Loading]" << "Friendships..." << std::endl;
        auto friendLines = readLines(FRIENDS_FILE);
        for (const auto& line : friendLines) {
            std::vector<std::string> parts;
            std::stringstream ss(line);
            std::string part;
            while (std::getline(ss, part, '|')) {
                parts.push_back(part);
            }
            if (parts.size() == 2) {
                User* user1 = nullptr;
                User* user2 = nullptr;
                for (auto* user : users) {
                    if (user->getUsername() == parts[0]) user1 = user;
                    if (user->getUsername() == parts[1]) user2 = user;
                }
                if (user1 && user2) {
                    user1->addFriend(user2->getUsername());
                    user2->addFriend(user1->getUsername());
                }
            }
        }
        std::cout << std::setw(15) << std::left << "[Success]" << "Loaded friendships" << std::endl;
    } catch (const std::exception& e) {
        std::cout << std::setw(15) << std::left << "[Error]" << "Failed to load friendships: " << e.what() << std::endl;
        throw FileOperationException();
    }
}

void FileManager::saveUsers(const std::vector<User*>& users) {
    try {
        std::cout << "\n" << std::setw(15) << std::left << "[Saving]" << "Users..." << std::endl;
        std::vector<std::string> userLines;
        for (const auto& user : users) {
            userLines.push_back(
                user->getUsername() + "|" +
                user->getEmail() + "|" +
                user->getPassword() + "|" +
                user->getGender()
            );
        }
        writeLines(USERS_FILE, userLines);
        std::cout << std::setw(15) << std::left << "[Success]" << "Saved " << users.size() << " users" << std::endl;
    } catch (const std::exception& e) {
        std::cout << std::setw(15) << std::left << "[Error]" << "Failed to save users: " << e.what() << std::endl;
        throw FileOperationException();
    }
}

void FileManager::savePosts(const std::vector<Post*>& posts) {
    try {
        std::cout << "\n" << std::setw(15) << std::left << "[Saving]" << "Posts..." << std::endl;
        std::vector<std::string> postLines;
        for (const auto& post : posts) {
            postLines.push_back(
                post->getUser()->getUsername() + "|" +
                post->getContent() + "|" +
                post->getTimestamp() + "|" +
                std::to_string(static_cast<int>(post->getPrivacy()))
            );
        }
        writeLines(POSTS_FILE, postLines);
        std::cout << std::setw(15) << std::left << "[Success]" << "Saved " << posts.size() << " posts" << std::endl;
    } catch (const std::exception& e) {
        std::cout << std::setw(15) << std::left << "[Error]" << "Failed to save posts: " << e.what() << std::endl;
        throw FileOperationException();
    }
}

void FileManager::saveFriendships(const std::vector<User*>& users) {
    try {
        std::cout << "\n" << std::setw(15) << std::left << "[Saving]" << "Friendships..." << std::endl;
        std::vector<std::string> friendLines;
        std::set<std::string> processedPairs; // To avoid duplicates
        
        for (const auto* user : users) {
            for (const auto& friendUsername : user->getFriends()) {
                std::string pair1 = user->getUsername() + "|" + friendUsername;
                std::string pair2 = friendUsername + "|" + user->getUsername();
                
                // Only save one direction of the friendship
                if (user->getUsername() < friendUsername && 
                    processedPairs.find(pair1) == processedPairs.end() &&
                    processedPairs.find(pair2) == processedPairs.end()) {
                    friendLines.push_back(pair1);
                    processedPairs.insert(pair1);
                    processedPairs.insert(pair2);
                }
            }
        }
        
        writeLines(FRIENDS_FILE, friendLines);
        std::cout << std::setw(15) << std::left << "[Success]" << "Saved " << friendLines.size() << " friendships" << std::endl;
    } catch (const std::exception& e) {
        std::cout << std::setw(15) << std::left << "[Error]" << "Failed to save friendships: " << e.what() << std::endl;
        throw FileOperationException();
    }
}

void FileManager::loadData(std::vector<User*>& users,
                         std::vector<Post*>& posts,
                         std::map<std::string, std::vector<std::pair<std::string, std::string>>>& conversations) {
    loadUsers(users);
    loadPosts(users, posts);
    loadFriendships(users);
    loadConversations(conversations);
}

void FileManager::saveData(const std::vector<User*>& users,
                         const std::vector<Post*>& posts,
                         const std::map<std::string, std::vector<std::pair<std::string, std::string>>>& conversations) {
    saveUsers(users);
    savePosts(posts);
    saveFriendships(users);
    saveConversations(conversations);
}

void FileManager::loadConversations(std::map<std::string, std::vector<std::pair<std::string, std::string>>>& conversations) {
    // TO DO: implement loadConversations
}

void FileManager::saveConversations(const std::map<std::string, std::vector<std::pair<std::string, std::string>>>& conversations) {
    // TO DO: implement saveConversations
}
