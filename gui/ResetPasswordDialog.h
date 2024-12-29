#ifndef RESETPASSWORDDIALOG_H
#define RESETPASSWORDDIALOG_H

#include <wx/wx.h>
#include "../include/FacebookSystem.h"

class ResetPasswordDialog : public wxDialog {
public:
    ResetPasswordDialog(wxWindow* parent, FacebookSystem* fbSystem);

private:
    // Controls
    wxTextCtrl* emailInput;
    wxTextCtrl* securityAnswerInput;
    wxTextCtrl* newPasswordInput;
    wxButton* submitButton;
    wxStaticText* statusText;
    FacebookSystem* fbSystem;

    // Event handlers
    void OnSubmit(wxCommandEvent& event);

    DECLARE_EVENT_TABLE()
};

#endif // RESETPASSWORDDIALOG_H
