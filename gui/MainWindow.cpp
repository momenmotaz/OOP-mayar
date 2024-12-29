#include "MainWindow.h"
#include <wx/wx.h>
#include <wx/richtext/richtextctrl.h>
#include <wx/statline.h>
#include <wx/datetime.h>
#include <wx/msgdlg.h>
#include <wx/timer.h>

wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
    EVT_BUTTON(wxID_ANY, MainWindow::OnLogin)
    EVT_BUTTON(wxID_ANY, MainWindow::OnRegister)
    EVT_BUTTON(wxID_ANY, MainWindow::OnLogout)
    EVT_BUTTON(wxID_ANY, MainWindow::OnCreatePost)
    EVT_BUTTON(wxID_ANY, MainWindow::OnAcceptFriend)
    EVT_BUTTON(wxID_ANY, MainWindow::OnRejectFriend)
    EVT_TIMER(wxID_ANY, MainWindow::OnNotificationTimer)
wxEND_EVENT_TABLE()

MainWindow::MainWindow(const wxString& title)
    : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(1200, 800)),
      notificationTimer(this, ID_NOTIFICATION_TIMER)
{
    // Initialize the Facebook system
    fbSystem = new FacebookSystem();
    currentUser = nullptr;

    // Create panels
    loginPanel = new wxPanel(this, wxID_ANY);
    mainPanel = new wxPanel(this, wxID_ANY);
    
    // Create the UI
    InitializeStyles();
    CreateLoginPanel();
    CreateMainPanel();
    
    // Start with login panel
    mainPanel->Hide();
    loginPanel->Show();
    
    // Create main sizer
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(loginPanel, 1, wxEXPAND);
    mainSizer->Add(mainPanel, 1, wxEXPAND);
    SetSizer(mainSizer);
    
    // Center the window
    Centre();
    
    // Layout
    Layout();
}

void MainWindow::SwitchToPanel(wxPanel* panel) {
    // Hide all panels
    loginPanel->Hide();
    mainPanel->Hide();
    
    // Show the requested panel
    panel->Show();
    
    // Update layout
    if (panel == loginPanel) {
        GetSizer()->Show(loginPanel);
        GetSizer()->Hide(mainPanel);
    } else {
        GetSizer()->Hide(loginPanel);
        GetSizer()->Show(mainPanel);
    }
    
    // Force layout update
    GetSizer()->Layout();
    Refresh();
    Update();
}

void MainWindow::InitializeStyles() {
    // Initialize fonts
    titleFont = wxFont(24, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
    normalFont = wxFont(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    
    // Initialize colors
    primaryColor = wxColour(59, 89, 152);    // Facebook blue
    secondaryColor = wxColour(242, 242, 242); // Light gray
    accentColor = wxColour(66, 103, 178);    // Darker blue
    backgroundColor = wxColour(255, 255, 255); // White
}

wxButton* MainWindow::CreateStyledButton(wxWindow* parent, int id, const wxString& label) {
    wxButton* button = new wxButton(parent, id, label);
    button->SetFont(normalFont);
    return button;
}

wxTextCtrl* MainWindow::CreateStyledTextCtrl(wxWindow* parent, const wxString& placeholder) {
    wxTextCtrl* ctrl = new wxTextCtrl(parent, wxID_ANY, placeholder);
    ctrl->SetFont(normalFont);
    return ctrl;
}

wxStaticText* MainWindow::CreateStyledText(wxWindow* parent, const wxString& text, bool isTitle) {
    wxStaticText* staticText = new wxStaticText(parent, wxID_ANY, text);
    staticText->SetFont(isTitle ? titleFont : normalFont);
    return staticText;
}

void MainWindow::CreateLoginPanel()
{
    loginPanel = new wxPanel(this, wxID_ANY);
    loginPanel->SetBackgroundColour(wxColor(59, 89, 152)); // Facebook blue

    // Create a main vertical sizer with extra spacing
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->AddSpacer(40);

    // Add Facebook logo/title
    wxStaticText* titleText = new wxStaticText(loginPanel, wxID_ANY, "facebook", 
                                              wxDefaultPosition, wxDefaultSize,
                                              wxALIGN_CENTER_HORIZONTAL);
    titleText->SetForegroundColour(wxColor(255, 255, 255));
    wxFont titleFont = titleText->GetFont();
    titleFont.SetPointSize(24);
    titleFont.SetWeight(wxFONTWEIGHT_BOLD);
    titleText->SetFont(titleFont);
    mainSizer->Add(titleText, 0, wxALIGN_CENTER | wxALL, 20);

    // Create a white panel for login controls
    wxPanel* loginControlsPanel = new wxPanel(loginPanel, wxID_ANY);
    loginControlsPanel->SetBackgroundColour(wxColor(255, 255, 255));
    wxBoxSizer* controlsSizer = new wxBoxSizer(wxVERTICAL);

    // Email field
    emailField = new wxTextCtrl(loginControlsPanel, wxID_ANY, "", 
                               wxDefaultPosition, wxSize(300, 35));
    emailField->SetHint("Email");
    controlsSizer->Add(emailField, 0, wxALL | wxEXPAND, 10);

    // Password field
    passwordField = new wxTextCtrl(loginControlsPanel, wxID_ANY, "", 
                                  wxDefaultPosition, wxSize(300, 35),
                                  wxTE_PASSWORD);
    passwordField->SetHint("Password");
    controlsSizer->Add(passwordField, 0, wxALL | wxEXPAND, 10);

    // Login button
    loginButton = new wxButton(loginControlsPanel, wxID_ANY, "Log In",
                              wxDefaultPosition, wxSize(300, 40));
    loginButton->SetBackgroundColour(wxColor(66, 103, 178));
    loginButton->SetForegroundColour(wxColor(255, 255, 255));
    controlsSizer->Add(loginButton, 0, wxALL | wxEXPAND, 10);

    // Register button
    registerButton = new wxButton(loginControlsPanel, wxID_ANY, "Create New Account",
                                 wxDefaultPosition, wxSize(300, 40));
    registerButton->SetBackgroundColour(wxColor(66, 183, 42));
    registerButton->SetForegroundColour(wxColor(255, 255, 255));
    controlsSizer->Add(registerButton, 0, wxALL | wxEXPAND, 10);

    // Forgot password link
    forgotPasswordButton = new wxButton(loginControlsPanel, wxID_ANY, "Forgot Password?",
                                      wxDefaultPosition, wxDefaultSize, wxBORDER_NONE);
    forgotPasswordButton->SetBackgroundColour(wxColor(255, 255, 255));
    forgotPasswordButton->SetForegroundColour(wxColor(66, 103, 178));
    controlsSizer->Add(forgotPasswordButton, 0, wxALIGN_CENTER | wxALL, 10);

    loginControlsPanel->SetSizer(controlsSizer);

    // Add the white panel to main sizer with some margin
    mainSizer->Add(loginControlsPanel, 0, wxALIGN_CENTER | wxALL, 20);

    loginPanel->SetSizer(mainSizer);

    // Bind events
    loginButton->Bind(wxEVT_BUTTON, &MainWindow::OnLogin, this);
    registerButton->Bind(wxEVT_BUTTON, &MainWindow::OnShowRegister, this);
    forgotPasswordButton->Bind(wxEVT_BUTTON, &MainWindow::OnForgotPassword, this);
}

void MainWindow::CreateRegisterPanel()
{
    registerPanel = new wxPanel(this, wxID_ANY);
    registerPanel->SetBackgroundColour(wxColor(66, 103, 178)); // Facebook blue
    
    // Create a white panel for controls
    wxPanel* registerControlsPanel = new wxPanel(registerPanel, wxID_ANY);
    registerControlsPanel->SetBackgroundColour(*wxWHITE);
    
    // Main sizer for the register panel
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    
    // Sizer for the white controls panel
    wxBoxSizer* controlsSizer = new wxBoxSizer(wxVERTICAL);
    
    // Add title
    wxStaticText* title = new wxStaticText(registerControlsPanel, wxID_ANY, 
                                          "Create New Account",
                                          wxDefaultPosition, wxDefaultSize,
                                          wxALIGN_CENTER);
    wxFont titleFont = title->GetFont();
    titleFont.SetPointSize(titleFont.GetPointSize() + 4);
    titleFont.SetWeight(wxFONTWEIGHT_BOLD);
    title->SetFont(titleFont);
    controlsSizer->Add(title, 0, wxALL | wxALIGN_CENTER, 20);
    
    // Add registration fields
    regEmailField = new wxTextCtrl(registerControlsPanel, wxID_ANY, "",
                                  wxDefaultPosition, wxSize(300, -1));
    regEmailField->SetHint("Email");
    
    regUsernameField = new wxTextCtrl(registerControlsPanel, wxID_ANY, "",
                                     wxDefaultPosition, wxSize(300, -1));
    regUsernameField->SetHint("Username");
    
    regPasswordField = new wxTextCtrl(registerControlsPanel, wxID_ANY, "",
                                    wxDefaultPosition, wxSize(300, -1),
                                    wxTE_PASSWORD);
    regPasswordField->SetHint("Password");
    
    wxArrayString genderChoices;
    genderChoices.Add("Male");
    genderChoices.Add("Female");
    genderCombo = new wxComboBox(registerControlsPanel, wxID_ANY, "Select Gender",
                                wxDefaultPosition, wxSize(300, -1),
                                genderChoices, wxCB_DROPDOWN | wxCB_READONLY);
    
    // Add fields to sizer with spacing
    controlsSizer->Add(regEmailField, 0, wxALL | wxALIGN_CENTER, 10);
    controlsSizer->Add(regUsernameField, 0, wxALL | wxALIGN_CENTER, 10);
    controlsSizer->Add(regPasswordField, 0, wxALL | wxALIGN_CENTER, 10);
    controlsSizer->Add(genderCombo, 0, wxALL | wxALIGN_CENTER, 10);
    
    // Add buttons
    regRegisterButton = new wxButton(registerControlsPanel, wxID_ANY, "Sign Up",
                                   wxDefaultPosition, wxSize(300, -1));
    regRegisterButton->SetBackgroundColour(wxColor(66, 183, 42)); // Green
    regRegisterButton->SetForegroundColour(*wxWHITE);
    
    regCancelButton = new wxButton(registerControlsPanel, wxID_ANY, "Cancel",
                                 wxDefaultPosition, wxSize(300, -1));
    regCancelButton->SetBackgroundColour(wxColor(242, 242, 242)); // Light gray
    
    controlsSizer->Add(regRegisterButton, 0, wxALL | wxALIGN_CENTER, 10);
    controlsSizer->Add(regCancelButton, 0, wxALL | wxALIGN_CENTER, 10);
    
    // Set the sizer for the white panel
    registerControlsPanel->SetSizer(controlsSizer);
    
    // Add the white panel to the main sizer with some margin
    mainSizer->Add(registerControlsPanel, 0, wxALL | wxALIGN_CENTER, 20);
    
    // Set the main sizer for the register panel
    registerPanel->SetSizer(mainSizer);
    
    // Bind events
    regRegisterButton->Bind(wxEVT_BUTTON, &MainWindow::OnRegister, this);
    regCancelButton->Bind(wxEVT_BUTTON, &MainWindow::OnCancelRegister, this);
}

void MainWindow::CreateMainPanel()
{
    mainPanel = new wxPanel(this, wxID_ANY);
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    // Welcome text
    welcomeText = new wxStaticText(mainPanel, wxID_ANY, "Welcome!");
    mainSizer->Add(welcomeText, 0, wxALL, 10);

    // Create post section
    wxStaticBox* postBox = new wxStaticBox(mainPanel, wxID_ANY, "Create Post");
    wxStaticBoxSizer* postSizer = new wxStaticBoxSizer(postBox, wxVERTICAL);
    
    postInput = new wxTextCtrl(postBox, wxID_ANY, "", 
                              wxDefaultPosition, wxSize(300, 100),
                              wxTE_MULTILINE);
    postSizer->Add(postInput, 0, wxALL | wxEXPAND, 5);
    
    createPostButton = new wxButton(postBox, wxID_ANY, "Post");
    postSizer->Add(createPostButton, 0, wxALL | wxALIGN_RIGHT, 5);
    
    mainSizer->Add(postSizer, 0, wxALL | wxEXPAND, 10);

    // Friend requests section
    wxStaticBox* friendBox = new wxStaticBox(mainPanel, wxID_ANY, "Friend Requests");
    wxStaticBoxSizer* friendSizer = new wxStaticBoxSizer(friendBox, wxVERTICAL);
    
    friendRequestsList = new wxListBox(friendBox, wxID_ANY,
                                     wxDefaultPosition, wxSize(300, 100));
    friendSizer->Add(friendRequestsList, 1, wxALL | wxEXPAND, 5);
    
    wxBoxSizer* friendButtonSizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton* acceptButton = new wxButton(friendBox, wxID_ANY, "Accept");
    rejectButton = new wxButton(friendBox, wxID_ANY, "Reject");
    
    friendButtonSizer->Add(acceptButton, 1, wxRIGHT, 5);
    friendButtonSizer->Add(rejectButton, 1, wxLEFT, 5);
    friendSizer->Add(friendButtonSizer, 0, wxALL | wxEXPAND, 5);
    
    mainSizer->Add(friendSizer, 0, wxALL | wxEXPAND, 10);

    // Navigation buttons
    wxBoxSizer* navSizer = new wxBoxSizer(wxHORIZONTAL);
    
    refreshButton = new wxButton(mainPanel, wxID_ANY, "Refresh");
    addFriendButton = new wxButton(mainPanel, wxID_ANY, "Add Friend");
    viewFriendsButton = new wxButton(mainPanel, wxID_ANY, "View Friends");
    logoutButton = new wxButton(mainPanel, wxID_ANY, "Logout");
    
    navSizer->Add(refreshButton, 1, wxRIGHT, 5);
    navSizer->Add(addFriendButton, 1, wxRIGHT, 5);
    navSizer->Add(viewFriendsButton, 1, wxRIGHT, 5);
    navSizer->Add(logoutButton, 1);
    
    mainSizer->Add(navSizer, 0, wxALL | wxEXPAND, 10);

    mainPanel->SetSizer(mainSizer);
    
    // Bind events
    createPostButton->Bind(wxEVT_BUTTON, &MainWindow::OnCreatePost, this);
    refreshButton->Bind(wxEVT_BUTTON, &MainWindow::OnRefresh, this);
    addFriendButton->Bind(wxEVT_BUTTON, &MainWindow::OnAddFriend, this);
    viewFriendsButton->Bind(wxEVT_BUTTON, &MainWindow::OnViewFriends, this);
    logoutButton->Bind(wxEVT_BUTTON, &MainWindow::OnLogout, this);
    acceptButton->Bind(wxEVT_BUTTON, &MainWindow::OnAcceptFriend, this);
    rejectButton->Bind(wxEVT_BUTTON, &MainWindow::OnRejectFriend, this);
}

void MainWindow::AddPostToPanel(const Post* post) {
    if (!post) return;

    wxPanel* postPanel = new wxPanel(postsPanel);
    postPanel->SetBackgroundColour(wxColour(245, 245, 245));
    
    wxBoxSizer* postSizer = new wxBoxSizer(wxVERTICAL);
    
    // Header with author and timestamp
    wxBoxSizer* headerSizer = new wxBoxSizer(wxHORIZONTAL);
    
    wxStaticText* authorText = new wxStaticText(postPanel, wxID_ANY, post->getAuthorUsername());
    wxFont authorFont = authorText->GetFont();
    authorFont.SetWeight(wxFONTWEIGHT_BOLD);
    authorText->SetFont(authorFont);
    
    wxStaticText* timestampText = new wxStaticText(postPanel, wxID_ANY, FormatTimestamp(post->getTimestamp()));
    timestampText->SetForegroundColour(wxColour(128, 128, 128));
    
    headerSizer->Add(authorText, 0, wxALL, 5);
    headerSizer->AddSpacer(10);
    headerSizer->Add(timestampText, 0, wxALL, 5);
    
    // Content
    wxStaticText* contentText = new wxStaticText(postPanel, wxID_ANY, post->getContent());
    contentText->Wrap(postsPanel->GetSize().GetWidth() - 40);
    
    postSizer->Add(headerSizer, 0, wxEXPAND | wxALL, 10);
    postSizer->Add(contentText, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);
    
    postPanel->SetSizer(postSizer);
    postsSizer->Insert(0, postPanel, 0, wxEXPAND | wxALL, 5);
    
    postsPanel->FitInside();
    postsPanel->Layout();
}

void MainWindow::RefreshMainPanel() {
    if (!currentUser) return;
    
    // Clear existing posts
    postsSizer->Clear(true);
    
    // Get and display posts
    const auto& allPosts = fbSystem->getPosts();
    for (const Post* post : allPosts) {
        AddPostToPanel(post);
    }
    
    // Refresh friend requests
    RefreshFriendRequests();
    
    // Layout update
    postsPanel->FitInside();
    postsPanel->Layout();
    mainPanel->Layout();
}

void MainWindow::OnLogin(wxCommandEvent& event)
{
    wxString email = emailField->GetValue();
    wxString password = passwordField->GetValue();

    if (email.IsEmpty() || password.IsEmpty()) {
        wxMessageBox("Please enter both email and password", "Login Error",
                    wxOK | wxICON_ERROR);
        return;
    }

    // Try to login
    bool success = fbSystem->login(email.ToStdString(), password.ToStdString());

    if (success) {
        currentUser = fbSystem->getCurrentUser();
        SwitchToPanel(mainPanel);
        RefreshMainPanel();
    } else {
        wxMessageBox("Invalid email or password", "Login Error",
                    wxOK | wxICON_ERROR);
    }
}

void MainWindow::OnLogout(wxCommandEvent& event) {
    currentUser = nullptr;
    SwitchToPanel(loginPanel);
    loginStatus->SetLabel("");
    emailField->SetValue("");
    passwordField->SetValue("");
}

void MainWindow::OnCreatePost(wxCommandEvent& event) {
    wxString content = postInput->GetValue();
    
    if (content.IsEmpty()) {
        wxMessageBox("Post content cannot be empty", "Error", wxICON_ERROR | wxOK);
        return;
    }
    
    if (currentUser) {
        fbSystem->createPost(content.ToStdString());
        postInput->SetValue("");
        RefreshMainPanel();
    }
}

void MainWindow::OnAcceptFriend(wxCommandEvent& event)
{
    int selection = friendRequestsList->GetSelection();
    if (selection == wxNOT_FOUND) {
        wxMessageBox("Please select a friend request first", "Error",
                    wxOK | wxICON_ERROR);
        return;
    }

    wxString username = friendRequestsList->GetString(selection);
    fbSystem->acceptFriendRequest(username.ToStdString());
    wxMessageBox("Friend request accepted!", "Success",
                wxOK | wxICON_INFORMATION);
    RefreshMainPanel();
}

void MainWindow::OnRejectFriend(wxCommandEvent& event)
{
    int selection = friendRequestsList->GetSelection();
    if (selection == wxNOT_FOUND) {
        wxMessageBox("Please select a friend request first", "Error",
                    wxOK | wxICON_ERROR);
        return;
    }

    wxString username = friendRequestsList->GetString(selection);
    fbSystem->rejectFriendRequest(username.ToStdString());
    wxMessageBox("Friend request rejected", "Success",
                wxOK | wxICON_INFORMATION);
    RefreshMainPanel();
}

void MainWindow::OnRegister(wxCommandEvent& event)
{
    // Get values from registration fields
    wxString email = regEmailField->GetValue();
    wxString username = regUsernameField->GetValue();
    wxString password = regPasswordField->GetValue();
    wxString gender = genderCombo->GetValue();

    // Validate input
    if (email.IsEmpty() || username.IsEmpty() || password.IsEmpty()) {
        wxMessageBox("Please fill in all fields", "Registration Error",
                    wxOK | wxICON_ERROR);
        return;
    }

    // Convert gender to lowercase for consistency
    gender.MakeLower();

    // Create new user
    if (fbSystem->registerUser(username.ToStdString(), email.ToStdString(), 
                              password.ToStdString(), gender.ToStdString())) {
        wxMessageBox("Registration successful! Please login.", "Success",
                    wxOK | wxICON_INFORMATION);
        
        // Clear fields
        regEmailField->Clear();
        regUsernameField->Clear();
        regPasswordField->Clear();
        genderCombo->SetSelection(0);
        
        // Switch back to login panel
        SwitchToPanel(loginPanel);
    } else {
        wxMessageBox("Registration failed. Username or email may already exist.",
                    "Registration Error", wxOK | wxICON_ERROR);
    }
}

void MainWindow::OnCancelRegister(wxCommandEvent& event)
{
    // Clear fields
    regEmailField->Clear();
    regUsernameField->Clear();
    regPasswordField->Clear();
    genderCombo->SetSelection(0);
    
    // Switch back to login panel
    SwitchToPanel(loginPanel);
}

void MainWindow::OnShowRegister(wxCommandEvent& event)
{
    SwitchToPanel(registerPanel);
}

void MainWindow::OnForgotPassword(wxCommandEvent& event)
{
    wxMessageBox("Please contact support to reset your password.", "Forgot Password",
                wxOK | wxICON_INFORMATION);
}

void MainWindow::CheckNotifications() {
    if (!currentUser) return;
    
    const auto& notifications = fbSystem->getNotifications();
    if (!notifications.empty()) {
        wxString message;
        message = "You have new notifications:\n\n";
        
        for (const auto& notif : notifications) {
            message += "- " + notif + "\n";
        }
        
        if (!message.IsEmpty()) {
            wxMessageDialog dialog(this, message, "Notifications",
                wxOK | wxICON_INFORMATION);
            dialog.ShowModal();
        }
    }
}

void MainWindow::OnNotificationTimer(wxTimerEvent& event) {
    CheckNotifications();
}

wxString MainWindow::FormatTimestamp(const std::string& timestamp) {
    return wxString(timestamp);
}

void MainWindow::RefreshFriendRequests() {
    // Clear existing friend requests
    friendRequestsList->Clear();
    
    // Get current user's friend requests
    const std::vector<std::string>& friendRequests = fbSystem->getCurrentUser()->getFriendRequests();
    
    // Add friend requests to list
    for (const auto& requesterUsername : friendRequests) {
        friendRequestsList->Append(requesterUsername);
    }
}

void MainWindow::OnRefresh(wxCommandEvent& event)
{
    RefreshMainPanel();
    wxMessageBox("Page refreshed!", "Success",
                wxOK | wxICON_INFORMATION);
}

void MainWindow::OnAddFriend(wxCommandEvent& event)
{
    wxTextEntryDialog dialog(this, "Enter username to add as friend:",
                            "Add Friend", "",
                            wxOK | wxCANCEL | wxCENTRE);
    
    if (dialog.ShowModal() == wxID_OK) {
        wxString username = dialog.GetValue();
        if (username.IsEmpty()) {
            wxMessageBox("Please enter a username", "Error",
                        wxOK | wxICON_ERROR);
            return;
        }
        
        if (fbSystem->sendFriendRequest(username.ToStdString())) {
            wxMessageBox("Friend request sent!", "Success",
                        wxOK | wxICON_INFORMATION);
        } else {
            wxMessageBox("Failed to send friend request", "Error",
                        wxOK | wxICON_ERROR);
        }
    }
}

void MainWindow::OnViewFriends(wxCommandEvent& event)
{
    std::vector<std::string> friends = currentUser->getFriends();
    
    if (friends.empty()) {
        wxMessageBox("You don't have any friends yet", "Friends List",
                    wxOK | wxICON_INFORMATION);
        return;
    }
    
    wxDialog* friendsDialog = new wxDialog(this, wxID_ANY, "Friends List",
                                         wxDefaultPosition, wxSize(300, 400));
    
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    wxListBox* friendsList = new wxListBox(friendsDialog, wxID_ANY);
    
    for (const auto& friendName : friends) {
        friendsList->Append(friendName);
    }
    
    sizer->Add(friendsList, 1, wxALL | wxEXPAND, 10);
    
    wxButton* closeButton = new wxButton(friendsDialog, wxID_CANCEL, "Close");
    sizer->Add(closeButton, 0, wxALL | wxALIGN_CENTER, 10);
    
    friendsDialog->SetSizer(sizer);
    friendsDialog->ShowModal();
    friendsDialog->Destroy();
}

MainWindow::~MainWindow() {
    delete fbSystem;
}
