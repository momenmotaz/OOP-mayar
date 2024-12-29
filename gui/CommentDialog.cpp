#include "CommentDialog.h"

enum {
    ID_SUBMIT_COMMENT = 1,
    ID_CANCEL_COMMENT
};

BEGIN_EVENT_TABLE(CommentDialog, wxDialog)
    EVT_BUTTON(ID_SUBMIT_COMMENT, CommentDialog::OnSubmit)
    EVT_BUTTON(ID_CANCEL_COMMENT, CommentDialog::OnCancel)
END_EVENT_TABLE()

CommentDialog::CommentDialog(wxWindow* parent, Post* post)
    : wxDialog(parent, wxID_ANY, "Add Comment", wxDefaultPosition, wxSize(400, 200)),
      post(post)
{
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    // Add comment input
    wxStaticText* label = new wxStaticText(this, wxID_ANY, "Your comment:");
    mainSizer->Add(label, 0, wxALL, 5);

    commentInput = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition,
                                wxDefaultSize, wxTE_MULTILINE);
    mainSizer->Add(commentInput, 1, wxEXPAND | wxALL, 5);

    // Add buttons
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    submitButton = new wxButton(this, ID_SUBMIT_COMMENT, "Submit");
    cancelButton = new wxButton(this, ID_CANCEL_COMMENT, "Cancel");

    buttonSizer->Add(submitButton, 0, wxALL, 5);
    buttonSizer->Add(cancelButton, 0, wxALL, 5);

    mainSizer->Add(buttonSizer, 0, wxALIGN_RIGHT | wxALL, 5);

    SetSizer(mainSizer);
    Centre();
}

void CommentDialog::OnSubmit(wxCommandEvent& event)
{
    if (!commentInput->GetValue().IsEmpty()) {
        EndModal(wxID_OK);
    } else {
        wxMessageBox("Comment cannot be empty!", "Error", 
                    wxOK | wxICON_ERROR, this);
    }
}

void CommentDialog::OnCancel(wxCommandEvent& event)
{
    EndModal(wxID_CANCEL);
}
