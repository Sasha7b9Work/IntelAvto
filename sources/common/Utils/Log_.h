// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


#define LOG_WRITE(...)          Log::Message(Text(__VA_ARGS__).c_str())
#define LOG_WRITE_AND_SHOW(...) Log::Message(Text(__VA_ARGS__).c_str()); Painter::EndScene()
#define LOG_ERROR(...)          Log::Message(__FILE__, __LINE__, Text(__VA_ARGS__).c_str())


#define LOG_FUNC_ENTER()        Log::Message(Text("%s %d enter", __FUNCTION__, __LINE__).c_str())
#define LOG_FUNC_LEAVE()        Log::Message(Text("%s %d leave", __FUNCTION__, __LINE__).c_str())


#define LOG_TRACE               Log::Message(Text("%s : %d", __FILE__, __LINE__).c_str())


struct Log
{
    static void Message(pchar message);
    static void Message(pchar file, int line, pchar message);
};
