// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#pragma warning(push, 0)
#include <wx/wx.h>
#include <wx/wfstream.h>
#include <wx/filename.h>
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

    void OnSize(wxCommandEvent &);
    void OnQuit(wxCommandEvent &);
    void OnSCPI(wxCommandEvent &);
    void OnConvertToArray(wxCommandEvent &);
    void OnAbout(wxCommandEvent &);
    void OnTimer(wxTimerEvent &);
    void OnTimerLong(wxTimerEvent &);
    void OnClose(wxCloseEvent &);

    static Frame *Self();

private:
    wxTimer timer;
    // Таймер для "длинного нажатия" кнопок
    wxTimer timerLongPress;

    void DrawFPS();
    void HandlerEvents();

public:
    void OnDown(wxCommandEvent &event);
    void OnUp(wxCommandEvent &event);

    static Frame *self;
};
