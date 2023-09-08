// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#define WIN32_LEAN_AND_MEAN
#include "Application_win.h"
#include "GUI/ComPort.h"

#undef main


extern void update();
extern void init();



enum
{
    File_Size = wxID_HIGHEST + 1,
    File_Quit = wxID_EXIT,
    Help_About = wxID_ABOUT
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

    wxMenu *helpMenu = new wxMenu;
    helpMenu->Append(Help_About, "&About\tF1", "Show about dialog");

    //fileMenu->Append(File_Size, "&Size", "Resize screen");

    fileMenu->Append(File_Quit, "E&xit\tAlt-X", "Quit this program");

    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(fileMenu, "&File");
    menuBar->Append(helpMenu, "&Help");

    Bind(wxEVT_MENU, &Frame::OnSize, this, File_Size);
    Bind(wxEVT_MENU, &Frame::OnQuit, this, File_Quit);
    Bind(wxEVT_MENU, &Frame::OnAbout, this, Help_About);

    SetMenuBar(menuBar);

    CreateStatusBar(2);
    SetStatusText("Welcome to wxWidgets!");

    timer.Bind(wxEVT_TIMER, &Frame::OnTimer, this);

    timer.Start(0);

    self = this;

    timerComPort.Bind(wxEVT_TIMER, &Frame::OnTimerComPort, this);
    timerComPort.Start(10);
}


void Frame::OnTimerComPort(wxTimerEvent &)
{
//    static uint displayFrame[272 * 480];
//
//    int received = ComPort::Receive((char *)displayFrame, 272 * 480 * 4);
//
//    if (received != 0)
//    {
//        received = received;
//    }
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
