#pragma once

#include <functional>
#include <string>
#include <vector>
#include <imgui.h>
#include <GLFW/glfw3.h>
#include "../include/Post.h"
#include "../include/FacebookSystem.h"

// Forward declarations of global variables
extern bool g_showLoginWindow;
extern bool g_showRegistrationWindow;
extern bool g_showMainWindow;
extern bool g_showFeedWindow;
extern bool g_showPostWindow;
extern bool g_showMessagesWindow;

class GUIManager {
public:
    static bool init();
    static void cleanup();
    static GLFWwindow* getWindow();
    static void setupImGuiStyle();

    // Window management
    static void showLoginWindow(bool* p_open, const std::function<User*(const std::string&, const std::string&)>& loginCallback);
    static void showRegisterWindow(bool* p_open, const std::function<void(const std::string&, const std::string&, const std::string&, const std::string&, const std::string&)>& registerCallback);
    static void showMainWindow(bool* p_open, FacebookSystem& fbSystem);
    static void showFeedWindow(bool* p_open, const std::vector<Post*>& posts);
    static void showPostWindow(bool* p_open, const std::function<void(const std::string&)>& postCallback);
    static void showMessagesWindow(bool* p_open, FacebookSystem& fbSystem);

private:
    static GLFWwindow* window;
    static bool initialized;

    // Helper functions for UI components
    static void renderUserCard(User* user);
    static void renderPostCard(Post* post);
    static void renderMessageCard(Message* message);
    static void renderFriendRequestCard(const std::string& username);
    
    // Theme colors
    static const ImVec4 COLOR_PRIMARY;
    static const ImVec4 COLOR_SECONDARY;
    static const ImVec4 COLOR_ACCENT;
    static const ImVec4 COLOR_BACKGROUND;
    static const ImVec4 COLOR_TEXT;
    static const ImVec4 COLOR_TEXT_DIM;
    static const ImVec4 COLOR_SUCCESS;
    static const ImVec4 COLOR_ERROR;
};
