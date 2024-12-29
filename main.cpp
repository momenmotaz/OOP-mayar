#include <iostream>
#include <string>
#include <limits>
#include "include/FacebookSystem.h"
#include "include/Exceptions.h"

// Forward declarations at the top of the file
void displayMenu();
void displayUserMenu();
void handleViewFriendsPosts(User* currentUser);
void handleAddFriend(FacebookSystem& system, User* currentUser);
void handleLikePost(User* currentUser);
void handleSharePost(User* currentUser);
void handleSendMessage(FacebookSystem& system, User* currentUser);
void handleViewMessages(User* currentUser);
void clearInputBuffer();
void clearScreen();
void displayPost(Post* post, User* currentUser);

using namespace std;

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void displayMenu() {
    cout << "\n====== Facebook Management System ======\n";
    cout << "1. Register\n";
    cout << "2. Login\n";
    cout << "3. Exit\n";
    cout << "=======================================\n";
    cout << "Choose an option: ";
}

void displayUserMenu() {
    cout << "\n============== User Menu ==============\n";
    cout << "1. Create Post\n";
    cout << "2. View My Posts\n";
    cout << "3. View Friends' Posts\n";
    cout << "4. Search Users\n";
    cout << "5. Add/Remove Friend\n";
    cout << "6. Like/Unlike Post\n";
    cout << "7. Share Post\n";
    cout << "8. Send Message\n";
    cout << "9. View Messages\n";
    cout << "10. Logout\n";
    cout << "=======================================\n";
    cout << "Choose an option: ";
}

void handleRegistration(FacebookSystem& system) {
    string email, name, password, gender, birthDate;
    
    clearInputBuffer();
    cout << "\n====== User Registration ======\n";
    cout << "Enter email: ";
    getline(cin, email);
    cout << "Enter name: ";
    getline(cin, name);
    cout << "Enter password: ";
    getline(cin, password);
    cout << "Enter gender (M/F): ";
    getline(cin, gender);
    cout << "Enter birthdate (DD/MM/YYYY): ";
    getline(cin, birthDate);
    
    try {
        system.registerUser(email, name, password, gender, birthDate);
        cout << "\nRegistration successful!\n";
    } catch (const exception& e) {
        cout << "\nError: " << e.what() << endl;
    }
}

void handleLogin(FacebookSystem& system) {
    string email, password;
    
    clearInputBuffer();
    cout << "\n====== User Login ======\n";
    cout << "Enter email: ";
    getline(cin, email);
    cout << "Enter password: ";
    getline(cin, password);
    
    try {
        User* user = system.login(email, password);
        cout << "\nWelcome back, " << user->getName() << "!\n";
        
        while (true) {
            displayUserMenu();
            int choice;
            cin >> choice;
            
            switch (choice) {
                case 1: {
                    // Create Post
                    clearInputBuffer();
                    string content;
                    char privacy;
                    cout << "Enter post content: ";
                    getline(cin, content);
                    cout << "Make post public? (y/n): ";
                    cin >> privacy;
                    user->createPost(content, privacy == 'y');
                    cout << "Post created successfully!\n";
                    break;
                }
                case 2: {
                    // View My Posts
                    auto posts = user->getPosts();
                    cout << "\n====== Your Posts ======\n";
                    for (Post* post : posts) {
                        cout << "Post #" << post->getId() << ": " << post->getContent() << endl;
                    }
                    break;
                }
                case 3: {
                    handleViewFriendsPosts(user);
                    break;
                }
                case 4: {
                    // Search Users
                    clearInputBuffer();
                    string query;
                    cout << "Enter search query: ";
                    getline(cin, query);
                    auto results = system.searchUsers(query);
                    cout << "\nSearch Results:\n";
                    for (User* result : results) {
                        cout << "- " << result->getName() << " (" << result->getEmail() << ")\n";
                    }
                    break;
                }
                case 5: {
                    handleAddFriend(system, user);
                    break;
                }
                case 6: {
                    handleLikePost(user);
                    break;
                }
                case 7: {
                    handleSharePost(user);
                    break;
                }
                case 8: {
                    handleSendMessage(system, user);
                    break;
                }
                case 9: {
                    handleViewMessages(user);
                    break;
                }
                case 10: {
                    system.logout();
                    cout << "Logged out successfully!\n";
                    return;
                }
                default:
                    cout << "Invalid option!\n";
            }
        }
    } catch (const exception& e) {
        cout << "\nError: " << e.what() << endl;
    }
}

void handleAddFriend(FacebookSystem& system, User* currentUser) {
    clearInputBuffer();
    string email;
    cout << "Enter friend's email: ";
    getline(cin, email);
    
    try {
        auto users = system.searchUsers(email);
        if (users.empty()) {
            cout << "User not found!\n";
            return;
        }
        
        User* friendUser = users[0];
        char restricted;
        cout << "Add as restricted friend? (y/n): ";
        cin >> restricted;
        
        currentUser->addFriend(friendUser, restricted == 'y');
        cout << "Friend added successfully!\n";
    } catch (const exception& e) {
        cout << "Error: " << e.what() << endl;
    }
}

void handleViewFriendsPosts(User* currentUser) {
    cout << "\n====== Friends' Posts ======\n";
    for (const auto& friendUser : currentUser->getFriends()) {
        auto posts = friendUser->getVisiblePosts(currentUser);
        if (!posts.empty()) {
            cout << "\nPosts from " << friendUser->getName() << ":\n";
            for (const auto& post : posts) {
                displayPost(post, currentUser);
            }
        }
    }
}

void handleLikePost(User* currentUser) {
    cout << "Enter post ID to like/unlike: ";
    int postId;
    cin >> postId;
    
    // Search for post in friends' posts
    for (const auto& friendUser : currentUser->getFriends()) {
        auto posts = friendUser->getVisiblePosts(currentUser);
        for (auto post : posts) {
            if (post->getId() == postId) {
                if (post->isLikedBy(currentUser)) {
                    post->removeLike(currentUser);
                    cout << "Post unliked!\n";
                } else {
                    post->addLike(currentUser);
                    cout << "Post liked!\n";
                }
                return;
            }
        }
    }
    cout << "Post not found!\n";
}

void handleSharePost(User* currentUser) {
    cout << "Enter post ID to share: ";
    int postId;
    cin >> postId;
    
    // Search for post in friends' posts
    for (const auto& friendUser : currentUser->getFriends()) {
        auto posts = friendUser->getVisiblePosts(currentUser);
        for (auto post : posts) {
            if (post->getId() == postId) {
                currentUser->sharePost(post);
                cout << "Post shared successfully!\n";
                return;
            }
        }
    }
    cout << "Post not found!\n";
}

void handleViewMessages(User* currentUser) {
    auto conversations = currentUser->getConversations();
    if (conversations.empty()) {
        cout << "No messages found!\n";
        return;
    }
    
    cout << "\n====== Your Conversations ======\n";
    for (const auto& conv : conversations) {
        auto participants = conv->getParticipants();
        cout << "Conversation with: ";
        for (const auto& participant : participants) {
            if (participant != currentUser) {
                cout << participant->getName() << " ";
            }
        }
        cout << "\nMessages:\n";
        
        for (const auto& msg : conv->getMessages()) {
            cout << msg->getSender()->getName() << ": " << msg->getContent() << "\n";
        }
        cout << "------------------------\n";
    }
}

void handleSendMessage(FacebookSystem& system, User* currentUser) {
    clearInputBuffer();
    string email;
    cout << "Enter recipient's email: ";
    getline(cin, email);
    
    try {
        auto users = system.searchUsers(email);
        if (users.empty()) {
            cout << "User not found!\n";
            return;
        }
        
        User* recipient = users[0];
        string message;
        cout << "Enter your message: ";
        getline(cin, message);
        
        currentUser->sendMessage(recipient, message);
        cout << "Message sent successfully!\n";
    } catch (const exception& e) {
        cout << "Error: " << e.what() << endl;
    }
}

void displayPost(Post* post, User* currentUser) {
    cout << "Post #" << post->getId() << ":\n";
    
    // Show original post info if this is a share
    if (post->getOriginalPost()) {
        cout << currentUser->getName() << " shared " << post->getOriginalPost()->getAuthor()->getName() << "'s post:\n";
    }
    
    cout << "Content: " << post->getContent() << "\n";
    cout << "Author: " << post->getAuthor()->getName() << "\n";
    cout << "Time: " << post->getTimestamp();
    cout << "Likes: " << post->getLikesCount() << " | ";
    cout << "Comments: " << post->getCommentsCount() << " | ";
    cout << "Shares: " << post->getSharesCount() << "\n";
    
    // Show comments
    if (post->getCommentsCount() > 0) {
        cout << "\nComments:\n";
        for (const auto& comment : post->getComments()) {
            cout << comment->getAuthor()->getName() << ": " << comment->getContent() << "\n";
            cout << "Likes: " << comment->getLikesCount() << "\n";
            
            // Show replies
            const auto& replies = comment->getReplies();
            if (!replies.empty()) {
                cout << "Replies:\n";
                for (const auto& reply : replies) {
                    cout << "  " << reply->getAuthor()->getName() << ": " << reply->getContent() << "\n";
                    cout << "  Likes: " << reply->getLikesCount() << "\n";
                }
            }
        }
    }
    cout << "------------------------\n";
}

int main() {
    FacebookSystem system;
    system.initialize();
    
    while (true) {
        displayMenu();
        int choice;
        cin >> choice;
        
        switch (choice) {
            case 1:
                handleRegistration(system);
                break;
            case 2:
                handleLogin(system);
                break;
            case 3:
                system.saveChanges();
                cout << "Thank you for using Facebook Management System!\n";
                return 0;
            default:
                cout << "Invalid option!\n";
        }
    }
    
    return 0;
}
