// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "GUI/Application.h"
#include "GUI/ConsoleSCPI.h"
#include "Hardware/HAL/HAL.h"

#undef main


extern void update();
extern void init();



enum
{
    FILE_SIZE = wxID_HIGHEST + 1,
    FILE_QUIT = wxID_EXIT,
    TOOL_SCPI,
    HELP_ABOUT = wxID_ABOUT
};


wxIMPLEMENT_APP_NO_MAIN(Application);


Frame *Frame::self = nullptr;


int main(int argc, char **argv)
{
    setlocale(LC_ALL, "Russian");

    return wxEntry(argc, argv);
}


bool Application::OnInit()
{
    if (!wxApp::OnInit())
    {
        return false;
    }

    init();

    return true;
}


Frame::Frame(const wxString& title)
    : wxFrame(NULL, wxID_ANY, title)
{
    SetIcon(wxICON(sample));

    wxMenu *fileMenu = new wxMenu;
    //fileMenu->Append(File_Size, "&Size", "Resize screen");
    fileMenu->Append(FILE_QUIT, "E&xit\tAlt-X", "Quit this program");

    wxMenu *toolsMenu = new wxMenu;
    toolsMenu->Append(TOOL_SCPI, "SCPI");

    wxMenu *helpMenu = new wxMenu;
    helpMenu->Append(HELP_ABOUT, "&About\tF1", "Show about dialog");

    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(fileMenu, "&File");
    menuBar->Append(toolsMenu, "�����������");
    menuBar->Append(helpMenu, "&Help");

    SetMenuBar(menuBar);

    CreateStatusBar(2);
    SetStatusText("Welcome to wxWidgets!");

    Bind(wxEVT_MENU, &Frame::OnSCPI, this, TOOL_SCPI);
    Bind(wxEVT_MENU, &Frame::OnQuit, this, FILE_QUIT);
    Bind(wxEVT_MENU, &Frame::OnSize, this, FILE_SIZE);
    Bind(wxEVT_MENU, &Frame::OnAbout, this, HELP_ABOUT);
    Bind(wxEVT_CLOSE_WINDOW, &Frame::OnClose, this);

    timer.Bind(wxEVT_TIMER, &Frame::OnTimer, this);

    timer.Start(0);

    timerLongPress.Bind(wxEVT_TIMER, &Frame::OnTimerLong, this);

    self = this;

    ConsoleSCPI::Self()->Show();
}


Frame *Frame::Self()
{
    return self;
}


void Frame::OnTimer(wxTimerEvent&)
{
    update();

    HandlerEvents();

    DrawFPS();
}


void Frame::DrawFPS()
{
    static int count = 0;
    static unsigned int prevTime = 0;

    count++;

    if (TIME_MS - prevTime > 1000)
    {
        float fps = (float)count / (float)(TIME_MS - prevTime) * 1000.0F;

        char buffer[100];
        sprintf(buffer, "fps %f", fps);

        SetStatusText(buffer);

        prevTime = TIME_MS;
        count = 0;
    }
}


void Frame::OnSize(wxCommandEvent&)
{

}


void Frame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Close(true);
}


void Frame::OnClose(wxCloseEvent &)
{
    ConsoleSCPI::Self()->Destroy();

    timer.Stop();

    Destroy();
}


void Frame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox(wxString::Format
    (
        "Welcome to %s!\n"
        "\n"
        "This is the minimal wxWidgets sample\n"
        "running under %s.",
        wxVERSION_STRING,
        wxGetOsDescription()
    ),
        "About wxWidgets minimal sample",
        wxOK | wxICON_INFORMATION,
        this);
}


void Frame::OnSCPI(wxCommandEvent &)
{
    ConsoleSCPI::Self()->SwitchVisibility();
}
