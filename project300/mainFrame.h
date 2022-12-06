#pragma once
#include <wx/wx.h>

//THis is where you run the code
class mainFrame : public wxFrame
{
public:
	mainFrame(const wxString& title);
private:
	void pressedEnt(wxCommandEvent& event);
};
