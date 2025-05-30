// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#pragma warning(push, 0)
#include <wx/wx.h>
#pragma warning(pop)


class Application : public wxApp
{
public:
    virtual bool OnInit() wxOVERRIDE;
};


class Frame : public wxFrame
{
public:

    Frame(const wxString& title);

    void OnSize(wxCommandEvent& event);
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnTimer(wxTimerEvent& event);
    void OnTimerLong(wxTimerEvent& event);

    static Frame *Self();

private:
    wxTimer timer;
    // ������ ��� "�������� �������" ������
    wxTimer timerLongPress;

    void DrawFPS();
    void HandlerEvents();

public:
    void OnDown(wxCommandEvent &event);
    void OnUp(wxCommandEvent &event);
    void OnSavePicture(wxCommandEvent &event);
    void OnTimerComPort(wxTimerEvent &);

    static Frame *self;

    static bool needSave;

    wxTimer timerComPort;
};
