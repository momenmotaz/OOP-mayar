#include "gui_manager.h"
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <iostream>
#include <cstring>

// Initialize static members
GLFWwindow* GUIManager::window = nullptr;
bool GUIManager::initialized = false;

// Theme colors
const ImVec4 GUIManager::COLOR_PRIMARY = ImVec4(0.20f, 0.59f, 0.86f, 1.0f);    // Facebook Blue
const ImVec4 GUIManager::COLOR_SECONDARY = ImVec4(0.27f, 0.27f, 0.27f, 1.0f);  // Dark Gray
const ImVec4 GUIManager::COLOR_ACCENT = ImVec4(0.95f, 0.95f, 0.95f, 1.0f);     // Light Gray
const ImVec4 GUIManager::COLOR_BACKGROUND = ImVec4(0.98f, 0.98f, 0.98f, 1.0f); // Off White
const ImVec4 GUIManager::COLOR_TEXT = ImVec4(0.10f, 0.10f, 0.10f, 1.0f);       // Almost Black
const ImVec4 GUIManager::COLOR_TEXT_DIM = ImVec4(0.50f, 0.50f, 0.50f, 1.0f);   // Medium Gray
const ImVec4 GUIManager::COLOR_SUCCESS = ImVec4(0.28f, 0.70f, 0.30f, 1.0f);    // Green
const ImVec4 GUIManager::COLOR_ERROR = ImVec4(0.86f, 0.26f, 0.27f, 1.0f);      // Red

bool GUIManager::init() {
    if (!glfwInit()) {
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(1280, 720, "Facebook", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    setupImGuiStyle();
    initialized = true;
    return true;
}

void GUIManager::cleanup() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
}

GLFWwindow* GUIManager::getWindow() {
    return window;
}

void GUIManager::setupImGuiStyle() {
    ImGuiStyle& style = ImGui::GetStyle();
    
    // Main
    style.WindowPadding = ImVec2(15, 15);
    style.FramePadding = ImVec2(5, 5);
    style.ItemSpacing = ImVec2(12, 8);
    style.ItemInnerSpacing = ImVec2(8, 6);
    style.IndentSpacing = 25.0f;
    style.ScrollbarSize = 15.0f;
    style.GrabMinSize = 10.0f;

    // Borders
    style.WindowBorderSize = 1.0f;
    style.ChildBorderSize = 1.0f;
    style.PopupBorderSize = 1.0f;
    style.FrameBorderSize = 1.0f;
    style.TabBorderSize = 1.0f;

    // Rounding
    style.WindowRounding = 7.0f;
    style.ChildRounding = 4.0f;
    style.FrameRounding = 3.0f;
    style.PopupRounding = 4.0f;
    style.ScrollbarRounding = 9.0f;
    style.GrabRounding = 3.0f;
    style.TabRounding = 4.0f;

    // Colors
    ImVec4* colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_Text] = COLOR_TEXT;
    colors[ImGuiCol_TextDisabled] = COLOR_TEXT_DIM;
    colors[ImGuiCol_WindowBg] = COLOR_BACKGROUND;
    colors[ImGuiCol_ChildBg] = COLOR_BACKGROUND;
    colors[ImGuiCol_PopupBg] = COLOR_BACKGROUND;
    colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.85f, 0.85f, 0.85f, 1.00f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.91f, 0.91f, 0.91f, 1.00f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.78f, 0.78f, 0.78f, 1.00f);
    colors[ImGuiCol_TitleBg] = COLOR_PRIMARY;
    colors[ImGuiCol_TitleBgActive] = COLOR_PRIMARY;
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.20f, 0.59f, 0.86f, 0.75f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.85f, 0.85f, 0.85f, 1.00f);
    colors[ImGuiCol_ScrollbarGrab] = COLOR_SECONDARY;
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
    colors[ImGuiCol_CheckMark] = COLOR_PRIMARY;
    colors[ImGuiCol_SliderGrab] = COLOR_PRIMARY;
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_Button] = COLOR_PRIMARY;
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
    colors[ImGuiCol_Header] = COLOR_PRIMARY;
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
    colors[ImGuiCol_PlotLines] = COLOR_PRIMARY;
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = COLOR_PRIMARY;
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
}

void GUIManager::showLoginWindow(bool* p_open, const std::function<User*(const std::string&, const std::string&)>& loginCallback) {
    ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f),
                           ImGuiCond_FirstUseEver, ImVec2(0.5f, 0.5f));

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize;
    if (!ImGui::Begin("Login to Facebook", p_open, window_flags)) {
        ImGui::End();
        return;
    }

    static char email[128] = "";
    static char password[128] = "";
    static bool loginFailed = false;

    // Logo and Welcome Text
    float windowWidth = ImGui::GetWindowSize().x;
    float textWidth = ImGui::CalcTextSize("Welcome to Facebook").x;
    ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
    
    ImGui::PushStyleColor(ImGuiCol_Text, COLOR_PRIMARY);
    ImGui::Text("Welcome to Facebook");
    ImGui::PopStyleColor();

    ImGui::Spacing();
    ImGui::Spacing();

    // Input Fields
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 7));
    
    ImGui::Text("Email:");
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);
    ImGui::InputText("##email", email, IM_ARRAYSIZE(email));
    ImGui::PopStyleVar();

    ImGui::Text("Password:");
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);
    ImGui::InputText("##password", password, IM_ARRAYSIZE(password), ImGuiInputTextFlags_Password);
    ImGui::PopStyleVar();

    ImGui::PopStyleVar();

    ImGui::Spacing();
    ImGui::Spacing();

    // Buttons
    float buttonWidth = 120.0f;
    ImGui::SetCursorPosX((windowWidth - buttonWidth * 2.0f - ImGui::GetStyle().ItemSpacing.x) * 0.5f);
    
    ImGui::PushStyleColor(ImGuiCol_Button, COLOR_PRIMARY);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.26f, 0.59f, 0.98f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.06f, 0.53f, 0.98f, 1.0f));
    
    if (ImGui::Button("Login", ImVec2(buttonWidth, 35))) {
        try {
            User* user = loginCallback(email, password);
            if (user) {
                *p_open = false;
                g_showMainWindow = true;
                loginFailed = false;
                memset(email, 0, sizeof(email));
                memset(password, 0, sizeof(password));
            } else {
                loginFailed = true;
            }
        } catch (const std::exception& e) {
            loginFailed = true;
        }
    }

    ImGui::PopStyleColor(3);
    
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, COLOR_SECONDARY);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.35f, 0.35f, 0.35f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.20f, 0.20f, 0.20f, 1.0f));
    
    if (ImGui::Button("Register", ImVec2(buttonWidth, 35))) {
        *p_open = false;
        g_showRegistrationWindow = true;
        loginFailed = false;
        memset(email, 0, sizeof(email));
        memset(password, 0, sizeof(password));
    }

    ImGui::PopStyleColor(3);

    if (loginFailed) {
        ImGui::Spacing();
        ImGui::PushStyleColor(ImGuiCol_Text, COLOR_ERROR);
        float errorText = ImGui::CalcTextSize("Invalid email or password").x;
        ImGui::SetCursorPosX((windowWidth - errorText) * 0.5f);
        ImGui::Text("Invalid email or password");
        ImGui::PopStyleColor();
    }

    ImGui::End();
}

void GUIManager::showRegisterWindow(bool* p_open, const std::function<void(const std::string&, const std::string&, const std::string&, const std::string&, const std::string&)>& registerCallback) {
    ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f),
                           ImGuiCond_FirstUseEver, ImVec2(0.5f, 0.5f));

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize;
    if (!ImGui::Begin("Create New Account", p_open, window_flags)) {
        ImGui::End();
        return;
    }

    static char email[128] = "";
    static char username[128] = "";
    static char password[128] = "";
    static char gender[32] = "";
    static char birthdate[32] = "";
    static bool registrationFailed = false;

    float windowWidth = ImGui::GetWindowSize().x;
    float textWidth = ImGui::CalcTextSize("Join Facebook").x;
    ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
    
    ImGui::PushStyleColor(ImGuiCol_Text, COLOR_PRIMARY);
    ImGui::Text("Join Facebook");
    ImGui::PopStyleColor();

    ImGui::Spacing();
    ImGui::Spacing();

    // Input Fields
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 7));
    
    ImGui::BeginChild("RegisterForm", ImVec2(0, -50), false);

    ImGui::Text("Email:");
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);
    ImGui::InputText("##email", email, IM_ARRAYSIZE(email));
    ImGui::PopStyleVar();

    ImGui::Text("Username:");
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);
    ImGui::InputText("##username", username, IM_ARRAYSIZE(username));
    ImGui::PopStyleVar();

    ImGui::Text("Password:");
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);
    ImGui::InputText("##password", password, IM_ARRAYSIZE(password), ImGuiInputTextFlags_Password);
    ImGui::PopStyleVar();

    ImGui::Text("Gender:");
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);
    if (ImGui::BeginCombo("##gender", gender)) {
        if (ImGui::Selectable("Male", strcmp(gender, "Male") == 0)) {
            strcpy(gender, "Male");
        }
        if (ImGui::Selectable("Female", strcmp(gender, "Female") == 0)) {
            strcpy(gender, "Female");
        }
        ImGui::EndCombo();
    }
    ImGui::PopStyleVar();

    ImGui::Text("Birthdate (YYYY-MM-DD):");
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);
    ImGui::InputText("##birthdate", birthdate, IM_ARRAYSIZE(birthdate));
    ImGui::PopStyleVar();

    ImGui::EndChild();

    ImGui::PopStyleVar();

    // Buttons
    float buttonWidth = 120.0f;
    ImGui::SetCursorPosX((windowWidth - buttonWidth * 2.0f - ImGui::GetStyle().ItemSpacing.x) * 0.5f);
    
    ImGui::PushStyleColor(ImGuiCol_Button, COLOR_PRIMARY);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.26f, 0.59f, 0.98f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.06f, 0.53f, 0.98f, 1.0f));
    
    if (ImGui::Button("Register", ImVec2(buttonWidth, 35))) {
        try {
            registerCallback(email, username, password, gender, birthdate);
            registrationFailed = false;
            *p_open = false;
            g_showLoginWindow = true;
            memset(email, 0, sizeof(email));
            memset(username, 0, sizeof(username));
            memset(password, 0, sizeof(password));
            memset(gender, 0, sizeof(gender));
            memset(birthdate, 0, sizeof(birthdate));
        } catch (const std::exception& e) {
            registrationFailed = true;
        }
    }

    ImGui::PopStyleColor(3);
    
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, COLOR_SECONDARY);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.35f, 0.35f, 0.35f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.20f, 0.20f, 0.20f, 1.0f));
    
    if (ImGui::Button("Back to Login", ImVec2(buttonWidth, 35))) {
        *p_open = false;
        g_showLoginWindow = true;
        registrationFailed = false;
        memset(email, 0, sizeof(email));
        memset(username, 0, sizeof(username));
        memset(password, 0, sizeof(password));
        memset(gender, 0, sizeof(gender));
        memset(birthdate, 0, sizeof(birthdate));
    }

    ImGui::PopStyleColor(3);

    if (registrationFailed) {
        ImGui::Spacing();
        ImGui::PushStyleColor(ImGuiCol_Text, COLOR_ERROR);
        float errorText = ImGui::CalcTextSize("Registration failed. Please try again.").x;
        ImGui::SetCursorPosX((windowWidth - errorText) * 0.5f);
        ImGui::Text("Registration failed. Please try again.");
        ImGui::PopStyleColor();
    }

    ImGui::End();
}

void GUIManager::showFeedWindow(bool* p_open, const std::vector<Post*>& posts) {
    ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin("Feed", p_open)) {
        ImGui::End();
        return;
    }

    if (ImGui::Button("Create Post", ImVec2(120, 0))) {
        extern bool g_showPostWindow;
        g_showPostWindow = true;
    }

    ImGui::SameLine();
    if (ImGui::Button("Logout", ImVec2(120, 0))) {
        *p_open = false;
        extern bool g_showLoginWindow;
        g_showLoginWindow = true;
    }

    ImGui::Separator();

    for (const auto* post : posts) {
        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
        if (ImGui::BeginChild(std::to_string(post->getId()).c_str(), ImVec2(ImGui::GetWindowWidth() - 20, 120), true)) {
            ImGui::Text("Posted by: %s", post->getUser()->getUsername().c_str());
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "on %s", post->getTimestamp().c_str());
            ImGui::Separator();
            ImGui::TextWrapped("%s", post->getContent().c_str());
        }
        ImGui::EndChild();
        ImGui::PopStyleColor();
        ImGui::Spacing();
    }

    ImGui::End();
}

void GUIManager::showPostWindow(bool* p_open, const std::function<void(const std::string&)>& onPost) {
    ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin("Create Post", p_open, ImGuiWindowFlags_NoCollapse)) {
        ImGui::End();
        return;
    }

    static char content[1024] = "";
    static std::string errorMessage;

    ImGui::Text("Write your post:");
    ImGui::InputTextMultiline("##content", content, IM_ARRAYSIZE(content), ImVec2(-1.0f, 150));

    if (ImGui::Button("Post", ImVec2(120, 0))) {
        try {
            onPost(content);
            memset(content, 0, sizeof(content));
            *p_open = false;
        } catch (const std::exception& e) {
            errorMessage = e.what();
        }
    }

    if (!errorMessage.empty()) {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "%s", errorMessage.c_str());
    }

    ImGui::End();
}

void GUIManager::showMainWindow(bool* p_open, FacebookSystem& fbSystem) {
    // Set window properties
    ImGui::SetNextWindowSize(ImVec2(1920, 1080), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);

    if (!ImGui::Begin("Facebook", p_open, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize)) {
        ImGui::End();
        return;
    }

    setupImGuiStyle();

    // Top bar with user info and logout
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.2f, 0.3f, 0.7f, 1.0f));
    if (ImGui::BeginChild("TopBar", ImVec2(ImGui::GetWindowWidth(), 60), true)) {
        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
        ImGui::Text("Welcome, %s!", fbSystem.getCurrentUser()->getUsername().c_str());
        ImGui::PopFont();
        
        ImGui::SameLine(ImGui::GetWindowWidth() - 100);
        if (ImGui::Button("Logout", ImVec2(80, 30))) {
            fbSystem.logout();
            *p_open = false;
            extern bool g_showLoginWindow;
            g_showLoginWindow = true;
        }
    }
    ImGui::EndChild();
    ImGui::PopStyleColor();

    // Left panel for friend requests and friends list
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
    if (ImGui::BeginChild("LeftPanel", ImVec2(300, ImGui::GetWindowHeight() - 80), true)) {
        // Friend Requests Section
        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
        ImGui::Text("Friend Requests");
        ImGui::PopFont();
        ImGui::Separator();

        auto pendingRequests = fbSystem.getPendingFriendRequests();
        if (pendingRequests.empty()) {
            ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "No pending requests");
        } else {
            for (const auto& request : pendingRequests) {
                ImGui::PushID(request.c_str());
                ImGui::Text("%s", request.c_str());
                if (ImGui::Button("Accept##req", ImVec2(80, 25))) {
                    fbSystem.acceptFriendRequest(request);
                }
                ImGui::SameLine();
                if (ImGui::Button("Reject##req", ImVec2(80, 25))) {
                    fbSystem.rejectFriendRequest(request);
                }
                ImGui::PopID();
                ImGui::Separator();
            }
        }

        ImGui::Spacing();
        ImGui::Spacing();

        // Friends List Section
        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
        ImGui::Text("Friends");
        ImGui::PopFont();
        ImGui::Separator();

        auto friends = fbSystem.getFriends();
        if (friends.empty()) {
            ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "No friends yet");
        } else {
            for (const auto& friend_ : friends) {
                ImGui::Text("%s", friend_.c_str());
                ImGui::Separator();
            }
        }
    }
    ImGui::EndChild();
    ImGui::PopStyleColor();

    ImGui::SameLine();

    // Main content area for posts
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.95f, 0.95f, 0.95f, 1.0f));
    if (ImGui::BeginChild("MainContent", ImVec2(ImGui::GetWindowWidth() - 620, ImGui::GetWindowHeight() - 80), true)) {
        // Create Post Section
        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
        if (ImGui::BeginChild("CreatePost", ImVec2(ImGui::GetWindowWidth() - 20, 120), true)) {
            static char postContent[1024] = "";
            ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
            ImGui::Text("Create Post");
            ImGui::PopFont();
            ImGui::Separator();
            ImGui::InputTextMultiline("##postcontent", postContent, IM_ARRAYSIZE(postContent), 
                                    ImVec2(ImGui::GetWindowWidth() - 20, 40));
            
            if (ImGui::Button("Post", ImVec2(100, 30))) {
                if (strlen(postContent) > 0) {
                    fbSystem.createPost(postContent);
                    memset(postContent, 0, sizeof(postContent));
                }
            }
        }
        ImGui::EndChild();
        ImGui::PopStyleColor();

        ImGui::Spacing();
        ImGui::Spacing();

        // Posts Feed
        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
        ImGui::Text("Posts Feed");
        ImGui::PopFont();
        ImGui::Separator();

        auto posts = fbSystem.getPosts();
        if (posts.empty()) {
            ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "No posts yet");
        } else {
            for (const auto* post : posts) {
                ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
                if (ImGui::BeginChild(std::to_string(post->getId()).c_str(), ImVec2(ImGui::GetWindowWidth() - 20, 120), true)) {
                    ImGui::Text("%s", post->getUser()->getUsername().c_str());
                    ImGui::SameLine();
                    ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "posted:");
                    ImGui::Separator();
                    ImGui::TextWrapped("%s", post->getContent().c_str());
                }
                ImGui::EndChild();
                ImGui::PopStyleColor();
                ImGui::Spacing();
            }
        }
    }
    ImGui::EndChild();
    ImGui::PopStyleColor();

    ImGui::SameLine();

    // Right panel for adding friends
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
    if (ImGui::BeginChild("RightPanel", ImVec2(300, ImGui::GetWindowHeight() - 80), true)) {
        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
        ImGui::Text("Add Friends");
        ImGui::PopFont();
        ImGui::Separator();

        auto nonFriends = fbSystem.getNonFriendUsers();
        if (nonFriends.empty()) {
            ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "No users to add");
        } else {
            for (const auto& user : nonFriends) {
                ImGui::PushID(user.c_str());
                ImGui::Text("%s", user.c_str());
                if (ImGui::Button("Add Friend", ImVec2(100, 25))) {
                    fbSystem.sendFriendRequest(user);
                }
                ImGui::PopID();
                ImGui::Separator();
            }
        }
    }
    ImGui::EndChild();
    ImGui::PopStyleColor();

    ImGui::End();
}
