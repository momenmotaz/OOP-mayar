#include "MessageDialog.h"

enum {
    ID_USER_LIST = 1,
    ID_SEND_MESSAGE
};

BEGIN_EVENT_TABLE(MessageDialog, wxDialog)
    EVT_LISTBOX(ID_USER_LIST, MessageDialog::OnUserSelect)
    EVT_BUTTON(ID_SEND_MESSAGE, MessageDialog::OnSend)
END_EVENT_TABLE()

MessageDialog::MessageDialog(wxWindow* parent, FacebookSystem* fbSystem, const wxString& withUser)
    : wxDialog(parent, wxID_ANY, "Messages", wxDefaultPosition, wxSize(600, 400)),
      fbSystem(fbSystem), currentChatUser(withUser)
{
    CreateControls();
    UpdateUserList();
    
    if (!currentChatUser.IsEmpty()) {
        userList->SetStringSelection(currentChatUser);
        UpdateChat();
    }
}

void MessageDialog::CreateControls() {
    wxBoxSizer* mainSizer = new wxBoxSizer(wxHORIZONTAL);
    
    // User list on the left
    userList = new wxListBox(this, ID_USER_LIST, wxDefaultPosition, wxSize(150, -1));
    mainSizer->Add(userList, 0, wxEXPAND | wxALL, 5);
    
    // Chat area on the right
    wxBoxSizer* chatSizer = new wxBoxSizer(wxVERTICAL);
    
    // Chat history
    chatHistory = new wxRichTextCtrl(this, wxID_ANY, "", wxDefaultPosition,
                                   wxDefaultSize, wxRE_MULTILINE | wxRE_READONLY);
    chatSizer->Add(chatHistory, 1, wxEXPAND | wxALL, 5);
    
    // Message input area
    wxBoxSizer* inputSizer = new wxBoxSizer(wxHORIZONTAL);
    messageInput = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition,
                                wxDefaultSize, wxTE_MULTILINE);
    inputSizer->Add(messageInput, 1, wxEXPAND | wxRIGHT, 5);
    
    sendButton = new wxButton(this, ID_SEND_MESSAGE, "Send");
    sendButton->Enable(false);
    inputSizer->Add(sendButton, 0);
    
    chatSizer->Add(inputSizer, 0, wxEXPAND | wxALL, 5);
    
    mainSizer->Add(chatSizer, 1, wxEXPAND | wxALL, 5);
    
    SetSizer(mainSizer);
}

void MessageDialog::UpdateUserList() {
    userList->Clear();
    
    // Get current user's friends
    const auto& friends = fbSystem->getCurrentUser()->getFriends();
    for (const auto& friend_ : friends) {
        userList->Append(friend_);
    }
    
    // Get all users we've messaged with
    const auto& conversations = fbSystem->getConversations();
    for (const auto& [key, messages] : conversations) {
        // Extract usernames from the conversation key
        size_t pos = key.find('|');
        if (pos != std::string::npos) {
            std::string user1 = key.substr(0, pos);
            std::string user2 = key.substr(pos + 1);
            
            // Add the other user if they're not already in the list
            std::string otherUser = (user1 == fbSystem->getCurrentUser()->getUsername()) ? user2 : user1;
            if (std::find(friends.begin(), friends.end(), otherUser) == friends.end() &&
                userList->FindString(otherUser) == wxNOT_FOUND) {
                userList->Append(otherUser);
            }
        }
    }
}

void MessageDialog::UpdateChat() {
    chatHistory->Clear();
    
    if (currentChatUser.IsEmpty()) return;
    
    const auto& messages = fbSystem->getMessages(currentChatUser.ToStdString());
    for (const auto& msg : messages) {
        chatHistory->BeginBold();
        chatHistory->WriteText(msg.first);
        chatHistory->EndBold();
        chatHistory->WriteText(": " + msg.second + "\n");
    }
    
    chatHistory->ShowPosition(chatHistory->GetLastPosition());
    sendButton->Enable(true);
}

void MessageDialog::OnUserSelect(wxCommandEvent& event) {
    currentChatUser = event.GetString();
    UpdateChat();
}

void MessageDialog::OnSend(wxCommandEvent& event) {
    if (currentChatUser.IsEmpty() || messageInput->IsEmpty()) return;
    
    try {
        fbSystem->sendMessage(currentChatUser.ToStdString(), 
                            messageInput->GetValue().ToStdString());
        messageInput->Clear();
        UpdateChat();
    } catch (const std::exception& e) {
        wxMessageBox(e.what(), "Error", wxOK | wxICON_ERROR);
    }
}
