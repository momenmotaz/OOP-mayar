#pragma once

#include <wx/wx.h>
#include <wx/richtext/richtextctrl.h>
#include <wx/srchctrl.h>
#include <wx/dateevt.h>
#include "../include/FacebookSystem.h"
#include "../include/User.h"
#include "../include/Post.h"
#include "CommentDialog.h"
#include "MessageDialog.h"

class MainWindow : public wxFrame {
public:
    MainWindow(const wxString& title);
    ~MainWindow();

private:
    // Event IDs
    enum {
        ID_LOGIN = wxID_HIGHEST + 1,
        ID_REGISTER,
        ID_FORGOT_PASSWORD,
        ID_LOGOUT,
        ID_CREATE_POST,
        ID_ACCEPT_FRIEND,
        ID_REJECT_FRIEND,
        ID_NOTIFICATION_TIMER
    };

    // UI Elements
    wxPanel* loginPanel;
    wxPanel* registerPanel;
    wxPanel* mainPanel;
    wxScrolledWindow* postsPanel;
    wxBoxSizer* postsSizer;
    
    // Login controls
    wxTextCtrl* emailField;
    wxTextCtrl* passwordField;
    wxButton* loginButton;
    wxButton* registerButton;
    wxButton* forgotPasswordButton;
    wxStaticText* loginStatus;
    
    // Register controls
    wxTextCtrl* regEmailField;
    wxTextCtrl* regUsernameField;
    wxTextCtrl* regPasswordField;
    wxComboBox* genderCombo;
    wxButton* regRegisterButton;
    wxButton* regCancelButton;
    
    // Main panel controls
    wxStaticText* welcomeText;
    wxTextCtrl* postInput;
    wxListBox* postsList;
    wxButton* createPostButton;
    wxButton* logoutButton;
    wxButton* refreshButton;
    wxButton* addFriendButton;
    wxButton* viewFriendsButton;
    wxButton* rejectButton;
    wxListBox* friendRequestsList;
    
    // Timer for notifications
    wxTimer notificationTimer;
    
    // System
    FacebookSystem* fbSystem;
    User* currentUser;

    // Styling
    wxFont titleFont;
    wxFont normalFont;
    wxColour primaryColor;
    wxColour secondaryColor;
    wxColour accentColor;
    wxColour backgroundColor;

    // Helper methods
    void InitializeStyles();
    wxButton* CreateStyledButton(wxWindow* parent, int id, const wxString& label);
    wxTextCtrl* CreateStyledTextCtrl(wxWindow* parent, const wxString& placeholder = "");
    wxStaticText* CreateStyledText(wxWindow* parent, const wxString& text, bool isTitle = false);
    void DisplayError(const wxString& message);
    void DisplaySuccess(const wxString& message);

    // Panel creation methods
    void CreateLoginPanel();
    void CreateRegisterPanel();
    void CreateMainPanel();
    
    // UI update methods
    void RefreshMainPanel();
    void RefreshFriendRequests();
    void AddPostToPanel(const Post* post);
    wxString FormatTimestamp(const std::string& timestamp);
    
    // Event handlers
    void OnLogin(wxCommandEvent& event);
    void OnShowRegister(wxCommandEvent& event);
    void OnRegister(wxCommandEvent& event);
    void OnCancelRegister(wxCommandEvent& event);
    void OnForgotPassword(wxCommandEvent& event);
    void OnLogout(wxCommandEvent& event);
    void OnCreatePost(wxCommandEvent& event);
    void OnRefresh(wxCommandEvent& event);
    void OnAddFriend(wxCommandEvent& event);
    void OnViewFriends(wxCommandEvent& event);
    void OnAcceptFriend(wxCommandEvent& event);
    void OnRejectFriend(wxCommandEvent& event);
    void OnNotificationTimer(wxTimerEvent& event);
    
    // Helper methods
    void SwitchToPanel(wxPanel* panel);
    void CheckNotifications();

    wxDECLARE_EVENT_TABLE();
};
