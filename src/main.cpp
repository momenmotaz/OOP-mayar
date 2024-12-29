#include "../gui/MainWindow.h"
#include <wx/wx.h>

class FacebookApp : public wxApp {
public:
    virtual bool OnInit() {
        if (!wxApp::OnInit())
            return false;

        wxInitAllImageHandlers();
        
        MainWindow* window = new MainWindow("Facebook");
        window->Show(true);
        SetTopWindow(window);
        
        return true;
    }
};

wxIMPLEMENT_APP(FacebookApp);
