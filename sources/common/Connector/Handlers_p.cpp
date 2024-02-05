// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Connector/Messages.h"
#include "Connector/Handlers_p.h"
#include "Display/Console.h"


namespace PHandlers
{
    static bool Request(BaseMessage *);
    static bool FreqMeasure(BaseMessage *);
    static bool Log(BaseMessage *);
    static bool RequestPictureDDSfromStorage(BaseMessage *);
}


static bool E(BaseMessage *)
{
    return false;
}


bool PHandlers::Processing(BaseMessage *msg)
{
    typedef bool(*pFuncBpM)(BaseMessage *);

    uint8 command = msg->TakeUINT8();

    pFuncBpM func = E;

    switch(command)
    {
    case Command::RequestData:                  func = Request;                      break;
    case Command::FreqMeasure:                  func = FreqMeasure;                  break;
    case Command::Log:                          func = Log;                          break;
    case Command::SCPI_Data:                    func = SCPI::Handler::Processing;    break;
    case Command::RequestPictureDDSfromStorage: func = RequestPictureDDSfromStorage; break;

    case Command::FDrive_NumDirsAndFiles:
    case Command::FDrive_Mount:
    case Command::FDrive_RequestDir:
    case Command::FDrive_RequestFile:
    case Command::FDrive_RequestFileSize:
    case Command::FDrive_RequestFileString:
    case Command::FDrive_LoadDDSfromFile:
    case Command::FDrive_RequestPictureDDSfromFile:
        func = FDrive::Handler::Processing;
        break;
    }

    if (command < Command::Count)
    {
        return func(msg);
    }
    else
    {
        LOG_ERROR("Неправильная команда");
    }

    return false;
}


static bool PHandlers::Request(BaseMessage *)
{
    return false;
}


static bool PHandlers::FreqMeasure(BaseMessage *msg)
{
    PFreqMeter::SetMeasure(msg->TakeUINT());
    return true;
}


static bool PHandlers::Log(BaseMessage *msg)
{
    Console::AddString(msg->String(1));
    return true;
}


static bool PHandlers::RequestPictureDDSfromStorage(BaseMessage *message)
{
    Chan ch(message->TakeUINT8());

    uint8 pic[240];

    message->TakeRemainigData(pic);

    Form::SetFormFlash(ch, pic);

    return true;
}
