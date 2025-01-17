// 2023/09/08 21:53:49 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Settings/Settings.h"
#include "Display/Display.h"
#include "Display/Primitives.h"
#include "Display/Text.h"
#include "Menu/MenuItems.h"
#include "Utils/Math.h"
#include "Menu/Pages/Pages.h"
#include "Connector/Messages_.h"
#include <cstring>


using namespace Primitives;


Parameter *Parameter::current = nullptr;
Parameter *Parameter::editable = nullptr;


int Choice::NumStates() const
{
    int result = 0;

    while (namesRu[result])
    {
        result++;
    }

    return result;
}


void Page::DrawMenuItem(int x, int y, int, bool)
{
    for (int i = 0; i < NumItems(); i++)
    {
        bool selected = (i == selectedItem);

        Color colorBack = items[i]->ColorBackground(selected);

        Rect(WidthItem(i) - 1, HEIGHT - 1).FillRounded(x, y + 1, 2, colorBack, Color::FILL);

        items[i]->DrawMenuItem(x, y + 5, WidthItem(i), selected);

        y += HEIGHT;
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
    int result = ((num % 2) == 0) ? ((Display::PHYSICAL_WIDTH - 1) / 6) : (Display::PHYSICAL_WIDTH / 6);

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


void Parameter::DrawMenuItem(int x0, int y0, int width, bool selected)
{
    ColorDraw(selected).SetAsCurrent();

    Text(Title()).Write(x0, y0 + DeltaTextt(), width);
}


void Parameter::Draw() const
{
    const int width = 125;
    const int height = 21;

    if (IsNowEdited())
    {
        Color color = Color::GetCurrent();

        Rect(width - 2, height - 2).Fill(x - 2, y - 2, Color::TYPE_BLUE);

        color.SetAsCurrent();
    }

    // Нарисовать редактируемое значение на экране
    GetValue().Draw(this, x, y);

    if (IsNowSelected())
    {
        Color color = Color::GetCurrent();

        Rect(width, height).Draw(x - 3, y - 3, Color::WHITE);

        color.SetAsCurrent();
    }
}


void Button::DrawMenuItem(int x, int y, int width, bool selected)
{
    ColorDraw(selected).SetAsCurrent();

    Text(GetTitle()).Write(x, y + DeltaTextt(), width);
}


void Choice::DrawMenuItem(int x, int y, int width, bool selected)
{
    ColorDraw(selected).SetAsCurrent();

    Text(Title()).Write(x, y + DeltaTextt(), width);
}


void GovernorChannelColor::DrawMenuItem(int _x, int _y, int _width, bool selected)
{
    if (selected)
    {
        const int x = _x + 4;
        const int y = _y;
        const int width = _width - 8;
        const int height = HEIGHT - 8;

        Rect(width, height).Fill(x, y, ColorFill());

        HLine hLine(2);
        Point point;

        hLine.Draw(x, y, Color::MENU_SELECT);
        point.Draw(x, y + 1);

        hLine.Draw(x + width - 2, y);
        point.Draw(x + width - 1, y + 1);

        hLine.Draw(x, y + height - 1);
        point.Draw(x, y + height - 2);

        hLine.Draw(x + width - 2, y + height - 1);
        point.Draw(x + width - 1, y + height - 2);
    }
    else
    {
        const int x = _x + 1;
        const int y = _y - 3;
        const int width = _width - 3;
        const int height = HEIGHT - 3;

        Rect(width - 1, height - 1).Fill(x + 1, y + 1, ColorFill());
        Rect(width, height).DrawRounded(x, y, 1, ColorFill());
    }

    Text("%0.3d", (int)*state).Write(_x, _y + 6, _width, Color::WHITE);
}


Color GovernorChannelColor::ColorFill() const
{
    static Color colors[3] = { Color::TYPE_RED, Color::TYPE_GREEN, Color::TYPE_BLUE };

    return colors[typeColor];
}


void GovernorChannelColor::SetValue(uint8 value)
{
    *state = value;

    if (typeColor == Red)
    {
        Color::TYPE_RED.SetRED(*state);
    }
    else if (typeColor == Green)
    {
        Color::TYPE_GREEN.SetGREEN(*state);
    }
    else
    {
        Color::TYPE_BLUE.SetBLUE(*state);
    }
}


Color Item::ColorBackground(bool selected)
{
    return selected ? Color::MENU_SELECT : Color::MENU_UNSELECT;
}


Color Item::ColorDraw(bool selected)
{
    return selected ? Color::MENU_UNSELECT : Color::MENU_SELECT;
}


void Page::OnEvent(EventType::E event)
{
    if (onEvent)
    {
        onEvent(event);
    }
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
    return namesRu[*state];
}


pchar Parameter::Title() const
{
    return title;
}


bool Button::OnEventControl(const Control &control)
{
    if ((control.key == Key::OK || control.key == Key::GovButton) && control.IsPress())
    {
        if (funcOnPress)
        {
            funcOnPress();

            return true;
        }
    }

    return false;
}


bool Parameter::OnEventControl(const Control &control)
{
    if (control.IsRelease())
    {
        if (control.key == Key::OK || control.key == Key::GovButton)
        {
            if (IsNowEdited())
            {
                editable = nullptr;

                if (GetValue().FromDrawStrut(GetMin(), GetMax()))
                {
                    Message::SetVoltage(GetValue()).Transmit();
                }
            }
            else
            {
                editable = this;

                GetValue().ds.Clear(this);
            }

            return true;
        }

        if (IsNowEdited())
        {
            GetValue().ds.PressKey(control.key);
        }
    }

    return false;
}


bool Page::OnEventControl(const Control &control)
{
    if ((control.key == Key::Left || control.key == Key::GovLeft) && control.IsPress())
    {
        if (!Parameter::editable)
        {
            Math::CircleIncrease<int>(&selectedItem, 0, NumItems() - 1);

            Parameter::current = SelectedItem()->IsParameter() ? (Parameter *)SelectedItem() : nullptr;

            return true;
        }
    }
    else if ((control.key == Key::Right || control.key == Key::GovRight) && control.IsPress())
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
