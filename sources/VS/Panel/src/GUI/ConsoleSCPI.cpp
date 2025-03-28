// 2023/09/08 21:00:17 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "GUI/ConsoleSCPI.h"
#include "GUI/ComPort.h"
#include "Display/Text_.h"

#pragma warning(push, 0)
#include <wx/wx.h>
#include <wx/fontdlg.h>
#pragma warning(pop)


enum
{
    ID_LINE
};


static wxTextCtrl *text = nullptr;
static wxTextCtrl *line = nullptr;

static ConsoleSCPI *self = nullptr;

pString ConsoleSCPI::DIRECT_PROMT = ">>> ";
pString ConsoleSCPI::REVERSE_PROMT = "<<< ";


ConsoleSCPI::ConsoleSCPI(wxFrame *parent) : wxFrame(parent, wxID_ANY, wxT("SCPI"))
{
    text = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, { 600, 300 }, wxTE_MULTILINE | wxTE_READONLY);

    line = new wxTextCtrl(this, ID_LINE, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    line->SetFocus();

    wxFont fnt(11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Courier New"));
    line->SetFont(fnt);
    text->SetFont(fnt);

    Bind(wxEVT_SIZE, &ConsoleSCPI::OnSize, this);
    line->Bind(wxEVT_TEXT_ENTER, &ConsoleSCPI::OnTextEnter, this, ID_LINE);
    line->Bind(wxEVT_KEY_DOWN, &ConsoleSCPI::OnTextControlKeyDown, this, ID_LINE);
    Bind(wxEVT_CLOSE_WINDOW, &ConsoleSCPI::OnClose, this);

    Show();

    if (ComPort::Open())
    {
        AddLine("���������� ������� ����������");
        timerComPort.Bind(wxEVT_TIMER, &ConsoleSCPI::OnTimerComPort, this);
        timerComPort.Start(10);
    }
    else
    {
        AddLine("������� ���������� �� ����������. �������� ��������");
    }

    timerTest.Bind(wxEVT_TIMER, &ConsoleSCPI::OnTimerTest, this);
}

ConsoleSCPI::~ConsoleSCPI()
{
    ComPort::Close();
}


void ConsoleSCPI::OnSize(wxSizeEvent &)
{
    wxPoint clientOrigin = GetClientAreaOrigin();

    wxSize clientSize = GetClientSize();

    int heightLine = line->GetSize().y;

    wxSize sizeText = clientSize;
    sizeText.y -= heightLine;

    text->SetSize(sizeText);

    line->SetPosition({ clientOrigin.x, clientSize.y - heightLine });
    line->SetSize({ text->GetSize().x, heightLine });
}


static int Consist0D(char *buffer, int size)
{
    int i = 0;
    while (*buffer != 0x0d)
    {
        i++;
        buffer++;
        if (i == size)
        {
            return -1;
        }
    }
    return i;
}


void ConsoleSCPI::OnTimerComPort(wxTimerEvent &)
{
    if (ComPort::IsOpened())
    {
        static char fullBuffer[4096 * 16] = { 0 };  // ������ �����
            
        char buffer[4096];

        int n = ComPort::Receive(buffer, 4095);

        if (n)
        {
            int positionOD = Consist0D(buffer, n);

            if (positionOD < 0)
            {
                buffer[n] = '\0';
                std::strcat(fullBuffer, buffer);
            }
            else
            {
                Text message(">>> %s", fullBuffer);
                AddText(message.c_str());

                buffer[positionOD] = 0;
                AddText(buffer);

                fullBuffer[0] = 0;
            }
        }
    }
}


ConsoleSCPI *ConsoleSCPI::Self()
{
    if (!self)
    {
        self = new ConsoleSCPI(nullptr);
    }

    return self;
}


void ConsoleSCPI::OnTextEnter(wxCommandEvent &)
{
    history.Add(line->GetLineText(0));

    AddText(DIRECT_PROMT);

    AddLine(line->GetLineText(0));

    SendToSCPI(line->GetLineText(0).c_str());

    line->Clear();
}


void ConsoleSCPI::OnTimerTest(wxTimerEvent &)
{
    const char *names[] =
    {
        "NONE",
        "FUNCTION",
        "MEASURE",
        "MEMORY",
        "SERVICE",
        "1",
        "2",
        "TIME",
        "START",
        "TRIG",
        "DISPLAY",
        "RANGE1+",
        "RANGE1-",
        "RSHIFT1+",
        "RSHIFT1-",
        "RANGE2+",
        "RANGE2-",
        "RSHIFT2+",
        "RSHIFT2-",
        "TBASE+",
        "TBASE-",
        "TSHIFT+",
        "TSHIFT-",
        "TRIGLEV+",
        "TRIGLEV-",
        "LEFT",
        "RIGHT",
        "UP",
        "DOWN",
        "ENTER",
        "F1",
        "F2",
        "F3",
        "F4",
        "F5"
    };

    Text message(":key:press %s", names[(rand() % 34) + 1]);

    SendToSCPI(message.c_str());
}


void ConsoleSCPI::SendToSCPI(const char *txt)
{
    if (ComPort::IsOpened())
    {
        Text message("%s\x0d", txt);

        ComPort::Send(message.c_str());
    }
    else
    {

    }
}


void ConsoleSCPI::StartTest()
{
    AddLine("���� ���������");
    srand((uint)time(0));
    timerTest.Start(500);
}


void ConsoleSCPI::StopTest()
{
    AddLine("���� ��������");
    timerTest.Stop();
}


void ConsoleSCPI::OnTextControlKeyDown(wxKeyEvent &event)
{
    if (event.GetKeyCode() == WXK_UP)
    {
        wxString txt = history.Prev();

        if (txt.size())
        {
            line->Clear();
            line->WriteText(txt);
        }
    }
    else if (event.GetKeyCode() == WXK_DOWN)
    {
        wxString txt = history.Next();

        if (txt.size())
        {
            line->Clear();
            line->WriteText(txt);
        }
    }
    else
    {
        event.Skip();
    }
}


void ConsoleSCPI::AddLine(const wxString &str)
{
    AddText(str);
    AddText(wxT("\n"));
}

void ConsoleSCPI::AddText(const wxString &str)
{
    text->WriteText(str);
}


void ConsoleSCPI::SwitchVisibility()
{
    Self()->Show(!Self()->IsShown());
}


void ConsoleSCPI::OnClose(wxCloseEvent &)
{
    Self()->Show(false);
}


void ConsoleSCPI::History::Add(const wxString &txt)
{
    if ((history.size() == 0) || 
        (history[history.size() - 1].compare(txt) != 0))
    {
        history.push_back(txt);
        position = history.size() - 1;
    }
}


wxString ConsoleSCPI::History::Next()
{
    if (history.size() == 0)
    {
        return "";
    }

    wxString result = history[position];

    position++;
    if (position == history.size())
    {
        position = 0;
    }

    return result;
}


wxString ConsoleSCPI::History::Prev()
{
    if (history.size() == 0)
    {
        return "";
    }

    wxString result = history[position];

    position = (position == 0) ? (history.size() - 1) : (position - 1);

    return result;
}
