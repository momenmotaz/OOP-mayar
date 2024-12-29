#include "../include/FacebookSystem.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <set>
#include <chrono>
#include <ctime>
#include <filesystem>
#include "../include/FileManager.h"

FacebookSystem::FacebookSystem() : currentUser(nullptr) {
    try {
        // Initialize vectors
        users.clear();
        posts.clear();
        
        // Create data directory if it doesn't exist
        std::filesystem::path dataDir = "../data";
        if (!std::filesystem::exists(dataDir)) {
            std::filesystem::create_directories(dataDir);
            std::cout << "[Created]      Data directory at: " << dataDir.string() << std::endl;
        }

        // Create default bots
        CreateDefaultBots();

        // Load existing data
        loadUsers();
        loadPosts();
        loadFriends();
        loadMessages();
        
        // Add default users if they don't exist
        if (users.empty()) {
            std::cout << "[Creating]     Default users..." << std::endl;
            createDefaultUsers();
            
            // Create some default posts
            User* john = nullptr;
            User* sarah = nullptr;
            User* mike = nullptr;
            
            for (auto* user : users) {
                if (user->getUsername() == "john") john = user;
                else if (user->getUsername() == "sarah") sarah = user;
                else if (user->getUsername() == "mike") mike = user;
            }
            
            if (john) {
                currentUser = john;
                createPost("Hey everyone! Just joined this awesome platform! 👋");
                createPost("Beautiful day for coding! #programming #cpp");
            }
            
            if (sarah) {
                currentUser = sarah;
                createPost("Hello world! Excited to be here! 😊");
                createPost("Just finished my latest project! #developer");
            }
            
            if (mike) {
                currentUser = mike;
                createPost("Greetings from Mike! Ready to connect with fellow developers!");
                createPost("Working on some cool C++ stuff! #coding");
            }
            
            currentUser = nullptr;
            
            // Save all data
            FileManager::saveUsers(users);
            saveFriends();
            saveMessages();
        }
    } catch (const std::exception& e) {
        std::cout << "[Error]        In FacebookSystem constructor: " << e.what() << std::endl;
        throw;
    }
}

void FacebookSystem::createDefaultUsers() {
    // Create some default users if none exist
    if (users.empty()) {
        User* user1 = new User("john", "john@example.com", "password123", "male");
        User* user2 = new User("jane", "jane@example.com", "password456", "female");
        User* user3 = new User("bob", "bob@example.com", "password789", "male");
        
        users.push_back(user1);
        users.push_back(user2);
        users.push_back(user3);
        
        // Add some friend connections
        user1->addFriend(user2->getUsername());
        user2->addFriend(user1->getUsername());
        
        user2->addFriend(user3->getUsername());
        user3->addFriend(user2->getUsername());
        
        // Create some posts
        createPost("Hello world!", user1);
        createPost("Having a great day!", user2);
        createPost("Check out my new project!", user3);
    }
}

void FacebookSystem::CreateDefaultBots() {
    // Create only 2 default bots to avoid overwhelming the system
    bool hasAlice = false;
    bool hasBob = false;
    
    for (const auto& user : users) {
        if (user->getUsername() == "Bot_Alice") {
            hasAlice = true;
        } else if (user->getUsername() == "Bot_Bob") {
            hasBob = true;
        }
    }
    
    std::string timestamp = getCurrentTimestamp();
    
    if (!hasAlice) {
        registerUser("Bot_Alice", "bot1@example.com", "bot123", "female");
        for (const auto& user : users) {
            if (user->getUsername() == "Bot_Alice") {
                createPost("Welcome to our Facebook community! 👋", user);
                break;
            }
        }
    }

    if (!hasBob) {
        registerUser("Bot_Bob", "bot2@example.com", "bot123", "male");
        for (const auto& user : users) {
            if (user->getUsername() == "Bot_Bob") {
                createPost("Feel free to connect with others! 🤝", user);
                break;
            }
        }
    }
}

void FacebookSystem::AddDefaultBots() {
    std::cout << "Creating default bots..." << std::endl;
    
    // Create only one bot that will make all 5 posts
    User* mainBot = new User("Bot_Alice", "bot.alice@bot.com", "bot123", "bot");
    mainBot->setBot(true);
    mainBot->setPublic(true);
    users.push_back(mainBot);
    std::cout << "Created main bot: " << mainBot->getUsername() << std::endl;
    CreateBotPosts(mainBot);

    // Create 4 more bots just for friend requests
    const std::vector<std::string> otherBots = {
        "Bot_Bob", "Bot_Charlie", "Bot_David", "Bot_Eve"
    };
    
    for (const auto& botName : otherBots) {
        User* bot = new User(botName, botName + "@bot.com", "bot123", "bot");
        bot->setBot(true);
        bot->setPublic(true);
        users.push_back(bot);
        std::cout << "Created friend request bot: " << bot->getUsername() << std::endl;
    }
    std::cout << "Finished creating all bots" << std::endl;
}

void FacebookSystem::CreateBotPosts(User* bot) {
    if (!bot || !bot->isBot()) {
        std::cout << "Skip creating posts: Invalid bot" << std::endl;
        return;
    }

    std::cout << "Creating posts for bot: " << bot->getUsername() << std::endl;
    const std::vector<std::string> samplePosts = {
        "Welcome to our social network! 👋",
        "Hope you're having a great day! 🌟",
        "Feel free to connect with others! 🤝",
        "Share your thoughts and experiences! 💭",
        "Don't forget to check out new features! ✨"
    };
    
    for (const auto& content : samplePosts) {
        auto now = std::chrono::system_clock::now();
        auto timestamp = std::to_string(std::chrono::system_clock::to_time_t(now));
        Post* post = new Post(bot, content, timestamp);
        posts.push_back(post);
        bot->addPost(post);
        std::cout << "Created post: " << content << std::endl;
    }
    std::cout << "Finished creating posts for bot: " << bot->getUsername() << std::endl;
}

void FacebookSystem::SendBotFriendRequests() {
    if (!currentUser || currentUser->isBot()) {
        std::cout << "Skip sending friend requests: No current user or user is a bot" << std::endl;
        return;
    }

    std::cout << "Starting to send friend requests..." << std::endl;
    int requestsSent = 0;
    
    for (User* user : users) {
        // Skip if we've already sent enough requests
        if (requestsSent >= 5) {
            std::cout << "Reached maximum friend requests (5)" << std::endl;
            break;
        }
        
        // Only send request if:
        // 1. The user is a bot
        // 2. Not already friends
        // 3. No pending request exists
        if (user->isBot() && 
            !currentUser->hasFriend(user->getUsername()) && 
            !hasPendingFriendRequest(user, currentUser)) {
            
            std::cout << "Sending friend request from bot: " << user->getUsername() << std::endl;
            user->addFriendRequest(currentUser->getUsername());
            requestsSent++;
        }
    }
    
    std::cout << "Finished sending friend requests. Total sent: " << requestsSent << std::endl;
}

bool FacebookSystem::login(const std::string& email, const std::string& password) {
    std::cout << "Attempting login for email: " << email << std::endl;
    
    // Reset current user
    currentUser = nullptr;
    
    // Clear any existing notifications
    notifications.clear();
    
    for (User* user : users) {
        if (user->getEmail() == email && user->getPassword() == password) {
            std::cout << "Login successful for user: " << user->getUsername() << std::endl;
            currentUser = user;
            
            // Send friend requests from bots if not already friends
            if (!user->isBot()) {
                std::cout << "Sending bot friend requests..." << std::endl;
                SendBotFriendRequests();
            }
            
            return true;
        }
    }
    std::cout << "Login failed: Invalid credentials" << std::endl;
    return false;
}

void FacebookSystem::logout() {
    std::cout << "Logging out current user" << std::endl;
    if (currentUser) {
        std::cout << "User " << currentUser->getUsername() << " logged out" << std::endl;
        FileManager::saveUsers(users);
        saveFriends();
        savePosts();  // Save posts when logging out
        saveMessages();
        currentUser = nullptr;
        notifications.clear();
    }
}

bool FacebookSystem::areFriends(const User* user1, const User* user2) const {
    if (!user1 || !user2) return false;
    return user1->hasFriend(user2->getUsername()) && user2->hasFriend(user1->getUsername());
}

bool FacebookSystem::hasPendingFriendRequest(const User* fromUser, const User* toUser) const {
    if (!fromUser || !toUser) return false;
    return toUser->hasFriendRequest(fromUser->getUsername());
}

bool FacebookSystem::sendFriendRequest(const std::string& toUsername) {
    if (!currentUser) return false;
    
    User* toUser = findUserByUsername(toUsername);
    if (!toUser || currentUser->getUsername() == toUsername) return false;
    
    // Check if already friends
    if (areFriends(currentUser, toUser)) return false;
    
    // Check if request already exists
    if (hasPendingFriendRequest(currentUser, toUser)) return false;
    
    // Add friend request
    toUser->addFriendRequest(currentUser->getUsername());
    addNotification(toUser, currentUser->getUsername() + " sent you a friend request");
    return true;
}

void FacebookSystem::acceptFriendRequest(const std::string& fromUsername) {
    if (!currentUser) return;
    
    User* fromUser = findUserByUsername(fromUsername);
    if (!fromUser) return;
    
    // Add each other as friends
    currentUser->addFriend(fromUsername);
    fromUser->addFriend(currentUser->getUsername());
    
    // Remove friend request
    currentUser->removeFriendRequest(fromUsername);
    
    // Add notification
    addNotification(fromUser, currentUser->getUsername() + " accepted your friend request");
    
    // Save changes
    saveFriends();
}

void FacebookSystem::rejectFriendRequest(const std::string& fromUsername) {
    if (!currentUser) return;
    
    User* fromUser = findUserByUsername(fromUsername);
    if (!fromUser) return;
    
    // Check if request exists
    if (!hasPendingFriendRequest(fromUser, currentUser)) return;
    
    // Remove friend request
    currentUser->removeFriendRequest(fromUsername);
    addNotification(fromUser, currentUser->getUsername() + " rejected your friend request");
}

FacebookSystem::~FacebookSystem() {
    FileManager::saveUsers(users);
    saveFriends();
    saveMessages();
    
    // Clean up memory
    for (auto user : users) {
        delete user;
    }
    users.clear();
    
    for (auto post : posts) {
        delete post;
    }
    posts.clear();
}

void FacebookSystem::loadUsers() {
    std::cout << "\n[Loading]      Users..." << std::endl;
    std::string filePath = "../data/users.txt";
    std::cout << "[File]         Reading: " << filePath << std::endl;
    
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cout << "[Warning]      Could not open " << filePath << std::endl;
        return;
    }

    std::string line;
    int lineCount = 0;
    while (std::getline(file, line)) {
        lineCount++;
        std::istringstream iss(line);
        std::string email, username, password, gender;
        if (std::getline(iss, email, '|') &&
            std::getline(iss, username, '|') &&
            std::getline(iss, password, '|') &&
            std::getline(iss, gender)) {
            
            User* user = new User(username, email, password, gender);
            users.push_back(user);
        }
    }
    std::cout << "[Success]      Read " << lineCount << " lines from " << filePath << std::endl;
    std::cout << "[Success]      Loaded " << users.size() << " users\n" << std::endl;
    file.close();
}

void FacebookSystem::loadFriends() {
    std::cout << "\n[Loading]      Friendships..." << std::endl;
    std::string filePath = "../data/friends.txt";
    std::cout << "[File]         Reading: " << filePath << std::endl;
    
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cout << "[Warning]      Could not open " << filePath << std::endl;
        return;
    }

    std::string line;
    int lineCount = 0;
    while (std::getline(file, line)) {
        lineCount++;
        std::istringstream iss(line);
        std::string user1, user2;
        if (std::getline(iss, user1, '|') && std::getline(iss, user2)) {
            User* userObj1 = findUserByUsername(user1);
            User* userObj2 = findUserByUsername(user2);
            
            if (userObj1 && userObj2) {
                userObj1->addFriend(user2);
                userObj2->addFriend(user1);
            }
        }
    }
    std::cout << "[Success]      Read " << lineCount << " lines from " << filePath << std::endl;
    std::cout << "[Success]      Loaded friendships\n" << std::endl;
    file.close();
}

void FacebookSystem::loadPosts() {
    std::cout << "\n[Loading]      Posts..." << std::endl;
    std::string filePath = "../data/posts.txt";
    std::cout << "[File]         Reading: " << filePath << std::endl;
    
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cout << "[Warning]      Could not open " << filePath << std::endl;
        return;
    }

    std::string line;
    int lineCount = 0;
    while (std::getline(file, line)) {
        lineCount++;
        std::istringstream iss(line);
        std::string idStr, username, content, timestamp;
        if (std::getline(iss, idStr, '|') &&
            std::getline(iss, username, '|') &&
            std::getline(iss, content, '|') &&
            std::getline(iss, timestamp)) {
            
            User* user = findUserByUsername(username);
            if (user) {
                Post* post = createPost(content, user);
                posts.push_back(post);
            }
        }
    }
    std::cout << "[Success]      Read " << lineCount << " lines from " << filePath << std::endl;
    std::cout << "[Success]      Loaded " << posts.size() << " posts\n" << std::endl;
    file.close();
}

void FacebookSystem::loadMessages() {
    std::cout << "\n[Loading]      Messages..." << std::endl;
    std::string filePath = "../data/messages.txt";
    std::cout << "[File]         Reading: " << filePath << std::endl;
    
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cout << "[Warning]      Could not open " << filePath << std::endl;
        return;
    }

    std::string line;
    int lineCount = 0;
    while (std::getline(file, line)) {
        lineCount++;
        std::istringstream iss(line);
        std::string from, to, message, timestamp;
        if (std::getline(iss, from, '|') &&
            std::getline(iss, to, '|') &&
            std::getline(iss, message, '|') &&
            std::getline(iss, timestamp)) {
            
            std::string key = createChatKey(from, to);
            conversations[key].push_back({from, message});
        }
    }
    std::cout << "[Success]      Read " << lineCount << " lines from " << filePath << std::endl;
    std::cout << "[Success]      Loaded messages\n" << std::endl;
    file.close();
}

void FacebookSystem::saveMessages() {
    std::cout << "\n[Saving]       Messages..." << std::endl;
    std::string filePath = "../data/messages.txt";
    std::cout << "[File]         Writing to: " << filePath << std::endl;
    
    std::ofstream file(filePath);
    if (!file.is_open()) {
        std::cout << "[Error]        Could not open " << filePath << std::endl;
        return;
    }

    int messageCount = 0;
    for (const auto& conv : conversations) {
        for (const auto& msg : conv.second) {
            file << msg.first << "|" << msg.second << "\n";
            messageCount++;
        }
    }
    std::cout << "[Success]      Saved " << messageCount << " messages\n" << std::endl;
    file.close();
}

void FacebookSystem::saveUsersToFile() {
    std::cout << "\n[Saving]       Users..." << std::endl;
    std::string filePath = "../data/users.txt";
    std::cout << "[File]         Writing to: " << filePath << std::endl;
    
    std::ofstream file(filePath);
    if (!file.is_open()) {
        std::cout << "[Error]        Could not open " << filePath << std::endl;
        return;
    }

    for (const auto* user : users) {
        file << user->getEmail() << "|"
             << user->getUsername() << "|"
             << user->getPassword() << "|"
             << user->getGender() << "\n";
    }
    std::cout << "[Success]      Saved " << users.size() << " users\n" << std::endl;
    file.close();
}

void FacebookSystem::saveFriends() {
    std::cout << "\n[Saving]       Friendships..." << std::endl;
    std::string filePath = "../data/friends.txt";
    std::cout << "[File]         Writing to: " << filePath << std::endl;
    
    std::ofstream file(filePath);
    if (!file.is_open()) {
        std::cout << "[Error]        Could not open " << filePath << std::endl;
        return;
    }

    int friendshipCount = 0;
    for (const auto* user : users) {
        for (const auto& friendUsername : user->getFriends()) {
            if (user->getUsername() < friendUsername) {
                file << user->getUsername() << "|" << friendUsername << "\n";
                friendshipCount++;
            }
        }
    }
    std::cout << "[Success]      Saved " << friendshipCount << " friendships\n" << std::endl;
    file.close();
}

void FacebookSystem::savePosts() {
    std::cout << "\n[Saving]       Posts..." << std::endl;
    std::string filePath = "../data/posts.txt";
    std::cout << "[File]         Writing to: " << filePath << std::endl;
    
    std::ofstream file(filePath);
    if (!file.is_open()) {
        std::cout << "[Error]        Could not open " << filePath << std::endl;
        return;
    }

    for (const auto* post : posts) {
        file << post->getId() << "|"
             << post->getUser()->getUsername() << "|"
             << post->getContent() << "|"
             << post->getTimestamp() << "\n";
    }
    std::cout << "[Success]      Saved " << posts.size() << " posts\n" << std::endl;
    file.close();
}

bool FacebookSystem::registerUser(const std::string& username, const std::string& email,
                                const std::string& password, const std::string& gender) {
    std::cout << "\n[Registering]  User..." << std::endl;
    std::cout << "[Details]      Username: " << username << std::endl;
    std::cout << "[Details]      Email: " << email << std::endl;
    
    // Check if username already exists
    for (const auto* user : users) {
        if (user->getUsername() == username) {
            std::cout << "[Error]        Username already exists" << std::endl;
            return false;
        }
        if (user->getEmail() == email) {
            std::cout << "[Error]        Email already exists" << std::endl;
            return false;
        }
    }

    // Create new user
    User* newUser = new User(username, email, password, gender);
    users.push_back(newUser);
    FileManager::saveUsers(users);
    std::cout << "[Success]      User registered successfully\n" << std::endl;
    return true;
}

bool FacebookSystem::resetPassword(const std::string& email, const std::string& securityAnswer,
                                 const std::string& newPassword) {
    User* user = findUserByEmail(email);
    if (!user) return false;
    
    // In a real application, we would verify the security answer here
    // For this demo, we'll just allow the password reset
    user->setPassword(newPassword);
    FileManager::saveUsers(users);
    return true;
}

void FacebookSystem::createPost(const std::string& content) {
    if (!currentUser) return;
    time_t now = time(0);
    Post* post = new Post(currentUser, content, std::to_string(now));
    posts.push_back(post);
    currentUser->addPost(post);
}

Post* FacebookSystem::createPost(const std::string& content, User* author) {
    if (!author) return nullptr;
    time_t now = time(0);
    Post* post = new Post(author, content, std::to_string(now));
    posts.push_back(post);
    author->addPost(post);
    return post;
}

void FacebookSystem::likePost(Post* post) {
    if (!currentUser || !post) return;
    post->addLike(currentUser->getUsername());
    User* author = findUserByUsername(post->getUser()->getUsername());
    if (author) {
        addNotification(author, currentUser->getUsername() + " liked your post");
    }
}

void FacebookSystem::likePost(int postId) {
    if (!currentUser || postId < 0 || postId >= posts.size()) return;
    Post* post = posts[postId];
    likePost(post);
}

void FacebookSystem::commentOnPost(int postId, const std::string& comment) {
    if (!currentUser) return;
    
    Post* post = nullptr;
    for (auto* p : posts) {
        if (p->getId() == postId) {
            post = p;
            break;
        }
    }
    if (!post) return;
    
    post->addComment(currentUser, comment);
    User* author = post->getUser();
    if (author) {
        addNotification(author, currentUser->getUsername() + " commented on your post");
    }
}

void FacebookSystem::sharePost(int postId) {
    if (!currentUser) return;
    
    Post* originalPost = nullptr;
    for (auto* post : posts) {
        if (post->getId() == postId) {
            originalPost = post;
            break;
        }
    }
    if (!originalPost) return;
    
    std::string newContent = "Shared: " + originalPost->getContent();
    createPost(newContent);
    
    User* author = originalPost->getUser();
    if (author) {
        addNotification(author, currentUser->getUsername() + " shared your post");
    }
}

std::vector<Post*> FacebookSystem::searchPosts(const std::string& query) const {
    std::vector<Post*> results;
    std::string lowerQuery = query;
    std::transform(lowerQuery.begin(), lowerQuery.end(), lowerQuery.begin(), ::tolower);
    
    for (const auto& post : posts) {
        std::string content = post->getContent();
        std::transform(content.begin(), content.end(), content.begin(), ::tolower);
        
        if (content.find(lowerQuery) != std::string::npos ||
            post->getUser()->getUsername().find(query) != std::string::npos) {
            results.push_back(post);
        }
    }
    
    return results;
}

std::vector<User*> FacebookSystem::searchUsers(const std::string& query) const {
    std::vector<User*> results;
    for (const auto& user : users) {
        if (user->getUsername().find(query) != std::string::npos ||
            user->getEmail().find(query) != std::string::npos) {
            results.push_back(user);
        }
    }
    return results;
}

void FacebookSystem::sendMessage(const std::string& to, const std::string& message) {
    if (!currentUser) return;
    
    User* toUser = findUserByUsername(to);
    if (!toUser) return;
    
    std::string chatKey = createChatKey(currentUser->getUsername(), to);
    conversations[chatKey].push_back({currentUser->getUsername(), message});
}

std::vector<std::pair<std::string, std::string>> FacebookSystem::getMessages(const std::string& withUsername) const {
    if (!currentUser) return {};
    
    std::string chatKey = createChatKey(currentUser->getUsername(), withUsername);
    if (conversations.find(chatKey) != conversations.end()) {
        return conversations.at(chatKey);
    }
    return {};
}

void FacebookSystem::removeFriend(const std::string& username) {
    if (!currentUser) return;
    
    User* otherUser = findUserByUsername(username);
    if (!otherUser) return;
    
    // Remove from each other's friends list
    currentUser->removeFriend(username);
    otherUser->removeFriend(currentUser->getUsername());
    
    // Save changes
    saveFriends();
}

void FacebookSystem::addNotification(User* user, const std::string& message) {
    notifications[user->getUsername()].push_back(message);
    if (notifications[user->getUsername()].size() > 50) {
        notifications[user->getUsername()].erase(notifications[user->getUsername()].begin());
    }
}

std::string FacebookSystem::createChatKey(const std::string& user1, const std::string& user2) const {
    return user1 < user2 ? user1 + "_" + user2 : user2 + "_" + user1;
}

void FacebookSystem::clearNotifications() {
    if (!currentUser) return;
    
    auto it = notifications.find(currentUser->getUsername());
    if (it != notifications.end()) {
        it->second.clear();
    }
}

User* FacebookSystem::findUserByUsername(const std::string& username) const {
    for (auto* user : users) {
        if (user->getUsername() == username) {
            return user;
        }
    }
    return nullptr;
}

User* FacebookSystem::findUserByEmail(const std::string& email) const {
    for (auto* user : users) {
        if (user->getEmail() == email) {
            return user;
        }
    }
    return nullptr;
}

Post* FacebookSystem::createPost(const std::string& content, PostPrivacy privacy) {
    if (!currentUser) return nullptr;

    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);
    std::string timestamp = std::ctime(&now_c);
    timestamp.pop_back(); // Remove trailing newline

    Post* post = new Post(currentUser, content, timestamp, privacy);
    posts.push_back(post);
    return post;
}

std::vector<std::string> FacebookSystem::getNotifications() const {
    if (!currentUser) return {};
    
    if (notifications.find(currentUser->getUsername()) != notifications.end()) {
        return notifications.at(currentUser->getUsername());
    }
    return {};
}

std::string FacebookSystem::getCurrentTimestamp() const {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}
