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
    TOOL_CONVERT_TO_ARRAY,
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
    toolsMenu->Append(TOOL_CONVERT_TO_ARRAY, "Конвертировать в массив C++");

    wxMenu *helpMenu = new wxMenu;
    helpMenu->Append(HELP_ABOUT, "&About\tF1", "Show about dialog");

    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(fileMenu, "&File");
    menuBar->Append(toolsMenu, "Инструменты");
    menuBar->Append(helpMenu, "&Help");

    SetMenuBar(menuBar);

    CreateStatusBar(2);
    SetStatusText("Welcome to wxWidgets!");

    Bind(wxEVT_MENU, &Frame::OnSCPI, this, TOOL_SCPI);
    Bind(wxEVT_MENU, &Frame::OnConvertToArray, this, TOOL_CONVERT_TO_ARRAY);
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


void Frame::OnConvertToArray(wxCommandEvent &)
{
    wxFileDialog dialog(this, _("Выбор файла для преобразования"), "", "",
        "All files (*.*)|*.*", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (dialog.ShowModal() == wxID_OK)
    {
        wxString path = dialog.GetPath();

        wxFileInputStream istream(path);

        if (istream.IsOk())
        {
            wxFileName name(dialog.GetPath());
            wxFile ofile;
            ofile.Open(dialog.GetPath() + ".cpp", wxFile::write);

            ofile.Write(wxString::Format("static const unsigned char bmp_zip_%s[] =\n{\n", name.GetName()));

            int num_symbols = 0;

            while (!istream.Eof())
            {
                if (num_symbols == 0)
                {
                    ofile.Write("    ");
                }

                uint8 byte;
                istream.Read(&byte, 1);
                num_symbols++;
                ofile.Write(wxString::Format("0x%02X", byte));

                if (!istream.Eof())
                {
                    ofile.Write(",");

                    if (num_symbols != 16)
                    {
                        ofile.Write(" ");
                    }
                }

                if (num_symbols == 16)
                {
                    num_symbols = 0;
                    ofile.Write("\n");
                }
            }

            if (num_symbols != 0)
            {
                ofile.Write("\n");
            }

            ofile.Write("};\n");
        }
    }
}
