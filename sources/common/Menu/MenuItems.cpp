// 2023/09/08 21:53:49 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Settings/Settings.h"
#include "Display/Display.h"
#include "Display/Primitives.h"
#include "Display/Text.h"
#include "Menu/MenuItems.h"
#include "Utils/Math.h"
#include <cstring>


using namespace Primitives;


Parameter *Parameter::current = nullptr;


int Choice::NumStates() const
{
    int result = 0;

    while (namesRu[result])
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

        Rect(WidthItem(i) - 1, HEIGHT - 1).FillRounded(x, y + 1, 2, colorBack, Color::FILL);

        items[i]->Draw(x, y + 5, WidthItem(i), selected);

        y += HEIGHT;
    }

    if (additionalDraw)
    {
        additionalDraw();
    }
}


int Page::WidthItem(int num) const
{
    if (equal_width_items)
    {
        return Display::PHYSICAL_WIDTH / NumItems();
    }

    int result = ((num % 2) == 0) ? ((Display::PHYSICAL_WIDTH - 1) / 6) : (Display::PHYSICAL_WIDTH / 6);

//    if (NumItems() == 5)
//    {
//        result = ((num % 2) == 0) ? ((Display::PHYSICAL_WIDTH - 1) / 5) : (Display::PHYSICAL_WIDTH / 5);
//
//        if (num == 4) { result += 3; }
//
//        return result;
//    }
//
//    if (NumItems() == 4)
//    {
//        result = ((num % 2) == 0) ? ((Display::PHYSICAL_WIDTH - 1) / 4) : (Display::PHYSICAL_WIDTH / 4);
//
//        if (num == 3) { result += 2; }
//
//        return result;
//    }
//
//    if (num == 5) { result += 3; }

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

    Text(Title()).Write(x0, y0 + 7, width);
}


void Parameter::Draw() const
{
    GetValue().Draw(x, y);

    if (IsSelected())
    {
        Color color = Color::GetCurrent();

        Rect(125, 21).Draw(x - 3, y - 3, Color::WHITE);

        color.SetAsCurrent();
    }
}


void Button::Draw(int x, int y, int width, bool selected)
{
    ColorDraw(selected).SetAsCurrent();

    Text(GetTitle()).Write(x, y + 7, width);
}


void Choice::Draw(int x, int y, int width, bool selected)
{
    ColorDraw(selected).SetAsCurrent();

    Text(Title()).Write(x, y + 7, width);
}


void GovernorChannelColor::Draw(int _x, int _y, int _width, bool selected)
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

    Text(String("%0.3d", (int)*state)).Write(_x, _y + 6, _width, Color::WHITE);
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
    return text[gset.language];
}


void Button::SetTitle(pchar ru, pchar en)
{
    text[0] = ru;
    text[1] = en;
}


pchar Choice::Title() const
{
    if (LANG_IS_RU)
    {
        return namesRu[*state];
    }

    return namesEn[*state];
}


pchar Parameter::Title() const
{
    return title[gset.language];
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


bool Parameter::OnEventControl(const Control &)
{
    return false;
}


bool Page::OnEventControl(const Control &control)
{

    if (control.key == Key::Left || control.key == Key::GovLeft)
    {
        Math::CircleIncrease<int>(&selectedItem, 0, NumItems() - 1);

        Parameter::current = SelectedItem()->IsParameter() ? (Parameter *)SelectedItem() : nullptr;

        return true;
    }
    else if (control.key == Key::Right || control.key == Key::GovRight)
    {
        Math::CircleDecrease<int>(&selectedItem, 0, NumItems() - 1);

        Parameter::current = SelectedItem()->IsParameter() ? (Parameter *)SelectedItem() : nullptr;

        return true;
    }

    return false;
}


Value &Parameter::GetValue() const
{
    return *value[VoltageMode::Current()];
}
