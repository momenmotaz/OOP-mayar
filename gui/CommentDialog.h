#pragma once

#include <wx/wx.h>
#include "../include/Post.h"
#include "../include/User.h"
#include "../include/Comment.h"

class CommentDialog : public wxDialog {
public:
    CommentDialog(wxWindow* parent, Post* post);
    wxString GetCommentText() const { return commentInput->GetValue(); }

private:
    Post* post;
    wxTextCtrl* commentInput;
    wxButton* submitButton;
    wxButton* cancelButton;

    void OnSubmit(wxCommandEvent& event);
    void OnCancel(wxCommandEvent& event);

    DECLARE_EVENT_TABLE()
};
