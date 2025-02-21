// 2023/09/08 21:53:49 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Settings/Settings.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Pages.h"
#include "Display/Primitives_.h"
#include "Display/Display_.h"
#include "Display/Text_.h"
#include "Hardware/Keyboard/Keyboard_.h"
#include "LAN/ServerTCP.h"
#include "Utils/Math_.h"
#include "Menu/Menu.h"
#include "Hardware/HAL/HAL.h"
#include "Utils/StringUtils_.h"
#include <cstring>


using namespace Primitives;


Parameter *Parameter::current = nullptr;
Parameter *Parameter::editable = nullptr;


int Item::Height()
{
    return Page::IsSignal(Menu::OpenedPage()) ? 30 : 30;
}


int Choice::NumStates() const
{
    int result = 0;

    while (names[result])
    {
        result++;
    }

    return result;
}


void Page::Draw(int x, int y, int, bool)
{
    for (int i = 0; i < NumItems(); i++)
    {
        bool selected = (i == selectedItem);

        Color colorBack = items[i]->ColorBackground(selected);

        Rect(WidthItem(i) - 1, Height() - 1).FillRounded(x, y + 1, 2, colorBack, Color::FILL);

        items[i]->Draw(x, y + 5, WidthItem(i), selected);

        y += Height();
    }

    if (additionalDraw)
    {
        additionalDraw();
    }
}


const Page *Page::ForCurrentSignal()
{
    static const Page *pages[TypeSignal::Count] =
    {
        PageSignal1::self,
        PageSignal2a::self,
        PageSignal2b::self,
        PageSignal3a::self,
        PageSignal3b::self,
        PageSignal4::self,
        PageSignal5a::self,
        PageSignal5b::self
    };

    return pages[TypeSignal::Current()];
}



bool Page::ConsistOpenedItems() const
{
    for (int i = 0; i < NumItems(); i++)
    {
        if (items[i]->IsOpened())
        {
            return true;
        }
    }

    return false;
}


void Page::DrawParameters() const
{
    for (int i = 0; i < NumItems(); i++)
    {
        if (items[i]->IsParameter())
        {
            Parameter *param = (Parameter *)items[i];

            if (param->IsNowEdited())
            {
                param->Draw();

                return;
            }
        }
    }

    for (int i = 0; i < NumItems(); i++)
    {
        if (items[i]->IsParameter())
        {
            Parameter *param = (Parameter *)items[i];

            param->Draw();
        }
    }
}


int Page::WidthItem(int num) const
{
    int div = Page::IsSignal(Menu::OpenedPage()) ? 6 : 4;

    int result = ((num % 2) == 0) ? ((Display::PHYSICAL_WIDTH - 1) / div) : (Display::PHYSICAL_WIDTH / div);

    return result;
}


int Page::NumItems() const
{
    int i = 0;
    while (items[i] != nullptr)
    {
        i++;
    }
    return i;
}


void Page::VerifySelectedItem()
{
    if (selectedItem >= NumItems())
    {
        selectedItem = NumItems() - 1;
    }
}


void Parameter::Draw(int x0, int y0, int width, bool selected)
{
    ColorDraw(selected).SetAsCurrent();

    if (!selected)
    {
        Color::WHITE.SetAsCurrent();
    }

    Text(Title()).Write(x0, y0 + DeltaTextt(), width);
}


void Parameter::Draw() const
{
    const int width = 125;
    const int height = 21;

    if (IsNowEdited())
    {
        Color color = Color::GetCurrent();

        Rect(width - 2, height - 2).Fill(x - 2, y - 2, Color::BACK);

        color.SetAsCurrent();
    }

    // Нарисовать редактируемое значение на экране

    if(IsNowEdited())
    {
        ds.Draw(x, y);

        Color color = Color::GetCurrent();

        Color::BACK.SetAsCurrent();

        GetMin().Draw(x, y + 25);

        GetMax().Draw(x, y - 25);

        color.SetAsCurrent();
    }
    else
    {
        GetValue().Draw(x, y);
    }

    if (IsNowSelected())
    {
        Color color = Color::GetCurrent();

        Rect(width, height).Draw(x - 3, y - 3, Color::WHITE);

        color.SetAsCurrent();
    }
}


void Button::Draw(int x, int y, int width, bool selected)
{
    ColorDraw(selected).SetAsCurrent();

    if (!selected)
    {
        Color::WHITE.SetAsCurrent();
    }

    Text(GetTitle()).Write(x, y + DeltaTextt(), width);
}


void Choice::Draw(int x, int y, int width, bool selected)
{
    ColorDraw(selected).SetAsCurrent();

    if (!selected)
    {
        Color::WHITE.SetAsCurrent();
    }

    Text(Title()).Write(x, y + DeltaTextt(), width);
}


Color Item::ColorBackground(bool selected)
{
    return selected ? Color::MENU_SELECT : Color::MENU_UNSELECT;
}


Color Item::ColorDraw(bool selected)
{
    return selected ? Color::MENU_UNSELECT : Color::MENU_SELECT;
}


pchar Button::GetTitle() const
{
    return text;
}


void Button::SetTitle(pchar ru)
{
    text = ru;
}


pchar Choice::Title() const
{
    return names[*state];
}


pchar Parameter::Title() const
{
    return title;
}


bool Button::OnEventControl(const Control &control)
{
    if (control.key == Key::OK || control.key == Key::GovButton)
    {
        if (funcOnPress)
        {
            funcOnPress();

            return true;
        }
    }

    return false;
}


bool Choice::OnEventControl(const Control &control)
{
    if (control.key == Key::OK || control.key == Key::GovButton)
    {
        *state = (uint8)((*state) + 1);

        if (*state == NumStates())
        {
            *state = 0;
        }

        if (funcOnPress)
        {
            funcOnPress();
        }

        return true;
    }

    return false;
}


bool Parameter::OnEventControl(const Control &control)
{
    if (control.key == Key::OK ||
        control.key == Key::GovButton)
    {
        if (IsNowEdited())
        {
            editable = nullptr;

            Value new_value(0);

            if (ds.ToValue(&new_value))
            {
                if (new_value.ToIntAbs() < GetMin().ToIntAbs())
                {
                    GetValue() = Value(GetMin().GetRaw());
                }
                else if (new_value.ToIntAbs() > GetMax().ToIntAbs())
                {
                    GetValue() = Value(GetMax().GetRaw());
                }
                else
                {
                    GetValue() = new_value;
                }
            }
        }
        else
        {
            editable = this;

            ds.Set(this);
        }

        return true;
    }

    if (IsNowEdited())
    {
        ds.PressKey(control.key);

        return true;
    }

    return false;
}


bool Page::OnEventControl(const Control &control)
{
    if (control.key == Key::Left || control.key == Key::GovLeft)
    {
        if (!Parameter::editable)
        {
            Math::CircleIncrease<int>(&selectedItem, 0, NumItems() - 1);

            Parameter::current = SelectedItem()->IsParameter() ? (Parameter *)SelectedItem() : nullptr;

            return true;
        }
    }
    else if (control.key == Key::Right || control.key == Key::GovRight)
    {
        if (!Parameter::editable)
        {
            Math::CircleDecrease<int>(&selectedItem, 0, NumItems() - 1);

            Parameter::current = SelectedItem()->IsParameter() ? (Parameter *)SelectedItem() : nullptr;

            return true;
        }
    }

    return false;
}


Value &Parameter::GetValue() const
{
    return *value[VoltageMode::Current()];
}


const Value &Parameter::GetMin() const
{
    return VoltageMode::Is12() ? min12 : min24;
}


const Value &Parameter::GetMax() const
{
    return VoltageMode::Is12() ? max12 : max24;
}


void Page::StartTest() const
{
    if (func_start_test)
    {
        func_start_test();
    }
}


bool Page::IsSignal(Page *page)
{
    return
        page == PageSignal1::self ||
        page == PageSignal2a::self ||
        page == PageSignal2b::self ||
        page == PageSignal3a::self ||
        page == PageSignal3b::self ||
        page == PageSignal4::self ||
        page == PageSignal5a::self ||
        page == PageSignal5b::self;
}


void Page::DrawConnectionServer()
{
    int x = Display::xConstParameters;

    int y = Display::yConstParameters - 25;

    Text("IT6523C : ").Write(x, y, Color::WHITE);

    x += 90;

    if (ServerTCP::IsConnected())
    {
        Text("ПОДКЛЮЧЕН").Write(x, y, Color::GREEN);
    }
    else
    {
        if ((TIME_MS / 500) % 2)
        {
            Text("НЕ ПОДКЛЮЧЕН").Write(x, y, Color::RED);
        }
    }
}


void IAddressIP::Draw(int x, int y, int widht, bool selected /* = false */)
{
    if (IsOpened())
    {
        Button::Draw(x, y, widht, selected);

        x += 130;

        Rect(150, 30).FillRounded(x, y - 6, 1, Color::BACK, Color::WHITE);

        Text(buffer).Write(x + 5, y + 2, Color::WHITE);
    }
    else
    {
        Button::Draw(x, y, widht, selected);
    }
}


bool IAddressIP::OnEventControl(const Control &control)
{
    if (IsOpened())
    {
        if (control.key == Key::OK)
        {
            uint8 bytes[4];

            if (ConvertStringToAddress(bytes))
            {
                std::memcpy(address, bytes, 4);
            }

            Close();
        }
        else if (control.key == Key::Esc)
        {
            Close();
        }
        else if (control.key == Key::Dot || (control.key >= Key::_1 && control.key <= Key::_0))
        {
            if (!edited)
            {
                buffer[0] = '\0';
            }

            edited = true;

            if (std::strlen(buffer) < 15)
            {
                std::strcat(buffer, Key::Name(control.key));
            }
        }

        return true;
    }
    else
    {
        if (control.key == Key::OK)
        {
            Open();

            return true;
        }
    }

    return false;
}


bool IAddressIP::ConvertStringToAddress(uint8 bytes[4]) const
{
    uint length = std::strlen(buffer);

    int counter = 0;

    for (uint i = 0; i < length; i++)
    {
        if (buffer[i] == '.')
        {
            counter++;
        }
    }

    if (counter != 3)
    {
        return false;
    }

    const char *pointer = buffer;

    for (uint i = 0; i < 4; i++)
    {
        const char *start = pointer;

        while (*pointer != '.' && *pointer != '\0')
        {
            pointer++;
        }

        // Нашли конец числа

        const char *end = pointer;

        pointer++;

        char str[10] = { '\0' };

        while (start != end)
        {
            char symbol[2] = { *start, '\0' };
            std::strcat(str, symbol);
            start++;
        }

        int value = 0;
        char *e = nullptr;

        SU::String2Int(str, &value, &e);

        if (value > 255)
        {
            return false;
        }

        bytes[i] = (uint8)value;
    }

    return true;
}


void IAddressIP::Open()
{
    std::strcpy(buffer, Text("%d.%d.%d.%d", address[0], address[1], address[2], address[3]).c_str());

    edited = false;

    Button::Open();
}


void IAddressIP::Close()
{
    Button::Close();
}
