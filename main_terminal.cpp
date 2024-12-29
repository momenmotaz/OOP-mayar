#include <iostream>
#include <string>
#include <ctime>
#include <chrono>
#include "FacebookSystem.h"

std::string getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);
    return std::ctime(&time);
}

void showMenu() {
    std::cout << "\nFacebook Terminal Menu:\n";
    std::cout << "1. Create Post\n";
    std::cout << "2. View Feed\n";
    std::cout << "3. Search Users\n";
    std::cout << "4. Manage Friends\n";
    std::cout << "5. Send Message\n";
    std::cout << "6. View Messages\n";
    std::cout << "7. Search Posts\n";
    std::cout << "8. Logout\n";
    std::cout << "Choice: ";
}

void createPost(FacebookSystem& system, User* user) {
    std::string content;
    std::cout << "Enter post content: ";
    std::cin.ignore();
    std::getline(std::cin, content);

    std::cout << "Privacy (1: Public, 2: Friends Only, 3: Private): ";
    int privacyChoice;
    std::cin >> privacyChoice;
    PostPrivacy privacy = PostPrivacy::PUBLIC;
    switch(privacyChoice) {
        case 2: privacy = PostPrivacy::FRIENDS_ONLY; break;
        case 3: privacy = PostPrivacy::PRIVATE; break;
    }

    Post* post = new Post(user, content, getCurrentTimestamp(), privacy);
    user->addPost(post);

    std::cout << "Tag users? (y/n): ";
    char choice;
    std::cin >> choice;
    while (choice == 'y' || choice == 'Y') {
        std::string username;
        std::cout << "Enter username to tag: ";
        std::cin >> username;
        User* taggedUser = system.findUser(username);
        if (taggedUser) {
            post->tagUser(taggedUser);
            std::cout << "Tagged " << username << "\n";
        } else {
            std::cout << "User not found\n";
        }
        std::cout << "Tag another user? (y/n): ";
        std::cin >> choice;
    }
}

void viewFeed(User* user) {
    std::cout << "\n=== Your Feed ===\n";
    for (const auto& post : user->getPosts()) {
        std::cout << "\n" << post->getUser()->getUsername() << " posted:\n";
        std::cout << post->getContent() << "\n";
        std::cout << "Posted at: " << post->getTimestamp();
        std::cout << "Likes: " << post->getLikes().size() << "\n";
        
        std::cout << "\nComments:\n";
        for (const auto& comment : post->getComments()) {
            std::cout << comment->getAuthor()->getUsername() << ": " << comment->getContent() << "\n";
            
            std::cout << "Replies:\n";
            for (const auto& reply : comment->getReplies()) {
                std::cout << "  " << reply->getAuthor()->getUsername() << ": " << reply->getContent() << "\n";
            }
        }
        
        std::cout << "\n1. Like\n2. Comment\n3. Next Post\nChoice: ";
        int choice;
        std::cin >> choice;
        
        if (choice == 1) {
            post->addLike(user->getUsername());
            std::cout << "Post liked!\n";
        }
        else if (choice == 2) {
            std::string comment;
            std::cout << "Enter your comment: ";
            std::cin.ignore();
            std::getline(std::cin, comment);
            post->addComment(user, comment);
            std::cout << "Comment added!\n";
        }
    }
}

void searchUsers(FacebookSystem& system, User* currentUser) {
    std::string username;
    std::cout << "Enter username to search: ";
    std::cin >> username;
    
    User* user = system.findUser(username);
    if (user) {
        std::cout << "\nUser found:\n";
        std::cout << "Username: " << user->getUsername() << "\n";
        std::cout << "Email: " << user->getEmail() << "\n";
        
        if (currentUser->isFriend(user)) {
            std::cout << "Status: Friend\n";
            std::cout << "1. Remove Friend\n2. Restrict Friend\n3. Block User\n4. Back\nChoice: ";
            int choice;
            std::cin >> choice;
            
            switch(choice) {
                case 1:
                    currentUser->removeFriend(user);
                    std::cout << "Friend removed\n";
                    break;
                case 2:
                    currentUser->restrictFriend(user);
                    std::cout << "Friend restricted\n";
                    break;
                case 3:
                    currentUser->blockUser(user);
                    std::cout << "User blocked\n";
                    break;
            }
        } else if (currentUser->isBlocked(user)) {
            std::cout << "Status: Blocked\n";
            std::cout << "1. Unblock User\n2. Back\nChoice: ";
            int choice;
            std::cin >> choice;
            
            if (choice == 1) {
                currentUser->unblockUser(user);
                std::cout << "User unblocked\n";
            }
        } else {
            std::cout << "Status: Not Friend\n";
            std::cout << "1. Add Friend\n2. Block User\n3. Back\nChoice: ";
            int choice;
            std::cin >> choice;
            
            switch(choice) {
                case 1:
                    currentUser->addFriend(user);
                    std::cout << "Friend added\n";
                    break;
                case 2:
                    currentUser->blockUser(user);
                    std::cout << "User blocked\n";
                    break;
            }
        }
    } else {
        std::cout << "User not found\n";
    }
}

void sendMessage(FacebookSystem& system, User* currentUser) {
    std::string username;
    std::cout << "Enter recipient's username: ";
    std::cin >> username;
    
    User* recipient = system.findUser(username);
    if (!recipient) {
        std::cout << "User not found\n";
        return;
    }
    
    if (currentUser->isBlocked(recipient) || recipient->isBlocked(currentUser)) {
        std::cout << "Cannot send message to this user\n";
        return;
    }
    
    std::string message;
    std::cout << "Enter your message: ";
    std::cin.ignore();
    std::getline(std::cin, message);
    
    // Send the message
    std::cout << "Message sent!\n";
}

void searchPosts(User* user) {
    std::cout << "\n=== Your Posts ===\n";
    for (const auto& post : user->getPosts()) {
        std::cout << "\nPost ID: " << post->getId() << "\n";
        std::cout << "Content: " << post->getContent() << "\n";
        std::cout << "Posted at: " << post->getTimestamp();
        std::cout << "Likes: " << post->getLikes().size() << "\n";
        
        std::cout << "Tagged Users: ";
        for (const auto& taggedUser : post->getTaggedUsers()) {
            std::cout << taggedUser->getUsername() << " ";
        }
        std::cout << "\n";
        
        std::cout << "Comments:\n";
        for (const auto& comment : post->getComments()) {
            std::cout << comment->getAuthor()->getUsername() << ": " << comment->getContent() << "\n";
            
            std::cout << "Replies:\n";
            for (const auto& reply : comment->getReplies()) {
                std::cout << "  " << reply->getAuthor()->getUsername() << ": " << reply->getContent() << "\n";
            }
        }
        std::cout << "\n";
    }
}

int main() {
    FacebookSystem system;
    User* currentUser = nullptr;
    
    while (true) {
        if (!currentUser) {
            std::cout << "\n1. Login\n2. Register\n3. Exit\nChoice: ";
            int choice;
            std::cin >> choice;
            
            if (choice == 1) {
                std::string email, password;
                std::cout << "Email: ";
                std::cin >> email;
                std::cout << "Password: ";
                std::cin >> password;
                
                currentUser = system.login(email, password);
                if (!currentUser) {
                    std::cout << "Invalid credentials\n";
                }
            }
            else if (choice == 2) {
                std::string email, username, password, gender, birthdate;
                std::cout << "Email: ";
                std::cin >> email;
                std::cout << "Username: ";
                std::cin >> username;
                std::cout << "Password: ";
                std::cin >> password;
                std::cout << "Gender: ";
                std::cin >> gender;
                std::cout << "Birthdate: ";
                std::cin >> birthdate;
                
                if (system.registerUser(email, username, password, gender, birthdate)) {
                    std::cout << "Registration successful!\n";
                } else {
                    std::cout << "Registration failed\n";
                }
            }
            else if (choice == 3) {
                break;
            }
        }
        else {
            showMenu();
            int choice;
            std::cin >> choice;
            
            switch(choice) {
                case 1: createPost(system, currentUser); break;
                case 2: viewFeed(currentUser); break;
                case 3: searchUsers(system, currentUser); break;
                case 4: // Manage friends
                    break;
                case 5: sendMessage(system, currentUser); break;
                case 6: // View messages
                    break;
                case 7: searchPosts(currentUser); break;
                case 8: 
                    system.logout();
                    currentUser = nullptr;
                    break;
                default:
                    std::cout << "Invalid choice\n";
            }
        }
    }
    
    return 0;
}
