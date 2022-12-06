#include "app.h"
#include <wx/wx.h>
#include "mainFrame.h"

wxIMPLEMENT_APP(app);

// This creates the window and window tile/size
bool app::OnInit() {
	mainFrame* mainFramee = new mainFrame("COP3530 JohnVedanshEdwin");
	mainFramee->SetClientSize(800, 600);
	mainFramee->Center();
	mainFramee->Show();
	return true;
}
