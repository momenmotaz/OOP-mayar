#pragma once

#include <wx/wx.h>
#include <wx/richtext/richtextctrl.h>
#include "../include/FacebookSystem.h"

class MessageDialog : public wxDialog {
public:
    MessageDialog(wxWindow* parent, FacebookSystem* fbSystem, const wxString& withUser = "");

private:
    FacebookSystem* fbSystem;
    wxString currentChatUser;
    
    // UI Components
    wxListBox* userList;
    wxRichTextCtrl* chatHistory;
    wxTextCtrl* messageInput;
    wxButton* sendButton;
    
    void CreateControls();
    void UpdateUserList();
    void UpdateChat();
    
    // Event handlers
    void OnUserSelect(wxCommandEvent& event);
    void OnSend(wxCommandEvent& event);
    
    DECLARE_EVENT_TABLE()
};
