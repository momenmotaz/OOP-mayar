#include "ResetPasswordDialog.h"

BEGIN_EVENT_TABLE(ResetPasswordDialog, wxDialog)
    EVT_BUTTON(wxID_OK, ResetPasswordDialog::OnSubmit)
END_EVENT_TABLE()

ResetPasswordDialog::ResetPasswordDialog(wxWindow* parent, FacebookSystem* fbSystem)
    : wxDialog(parent, wxID_ANY, "Reset Password", wxDefaultPosition, wxDefaultSize),
      fbSystem(fbSystem)
{
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    // Add description
    wxStaticText* description = new wxStaticText(this, wxID_ANY, 
        "To reset your password, please enter your email address and answer your security question.");
    description->Wrap(300);
    mainSizer->Add(description, 0, wxALL | wxEXPAND, 10);

    // Email input
    mainSizer->Add(new wxStaticText(this, wxID_ANY, "Email:"), 0, wxALL, 5);
    emailInput = new wxTextCtrl(this, wxID_ANY);
    mainSizer->Add(emailInput, 0, wxEXPAND | wxLEFT | wxRIGHT, 5);

    // Security question answer input
    mainSizer->Add(new wxStaticText(this, wxID_ANY, "What is your mother's maiden name?"), 0, wxALL, 5);
    securityAnswerInput = new wxTextCtrl(this, wxID_ANY);
    mainSizer->Add(securityAnswerInput, 0, wxEXPAND | wxLEFT | wxRIGHT, 5);

    // New password input
    mainSizer->Add(new wxStaticText(this, wxID_ANY, "New Password:"), 0, wxALL, 5);
    newPasswordInput = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD);
    mainSizer->Add(newPasswordInput, 0, wxEXPAND | wxLEFT | wxRIGHT, 5);

    // Status text
    statusText = new wxStaticText(this, wxID_ANY, "");
    statusText->SetForegroundColour(*wxRED);
    mainSizer->Add(statusText, 0, wxALL | wxALIGN_CENTER, 5);

    // Buttons
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    submitButton = new wxButton(this, wxID_OK, "Reset Password");
    wxButton* cancelButton = new wxButton(this, wxID_CANCEL, "Cancel");
    buttonSizer->Add(submitButton, 1, wxALL, 5);
    buttonSizer->Add(cancelButton, 1, wxALL, 5);
    mainSizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxALL, 5);

    SetSizerAndFit(mainSizer);
    CenterOnParent();
}

void ResetPasswordDialog::OnSubmit(wxCommandEvent& event) {
    wxString email = emailInput->GetValue();
    wxString securityAnswer = securityAnswerInput->GetValue();
    wxString newPassword = newPasswordInput->GetValue();

    if (email.IsEmpty() || securityAnswer.IsEmpty() || newPassword.IsEmpty()) {
        statusText->SetLabel("Please fill in all fields");
        return;
    }

    try {
        if (fbSystem->resetPassword(email.ToStdString(), securityAnswer.ToStdString(), newPassword.ToStdString())) {
            wxMessageBox("Password reset successful!", "Success", wxICON_INFORMATION | wxOK);
            EndModal(wxID_OK);
        } else {
            statusText->SetLabel("Invalid email or security answer");
        }
    } catch (const std::exception& e) {
        statusText->SetLabel(e.what());
    }
}
