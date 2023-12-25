// 2023/09/08 21:53:49 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Settings.h"
#include "Display/Display.h"
#include "Display/Primitives.h"
#include "Display/Text.h"
#include "Menu/Hint.h"
#include "Menu/MenuItems.h"
#include "Utils/Math.h"
#include <cstring>


using namespace Primitives;


Enumeration::Enumeration(uint8 v, const bool *_correct, int states) :
    value(v), namesRu(nullptr), namesEn(nullptr), ugoRu(nullptr), ugoEn(nullptr),
    correct(_correct), sw(nullptr), numStates(states)
{
    if (_correct)
    {
        for (uint8 i = 0; i < numStates; i++)
        {
            if (_correct[i] == true)
            {
                value = i;
                break;
            }
        }
    }
}


int Enumeration::NumStates() const
{
    int result = 0;

    if (correct == nullptr)
    {
        for (int i = 0; namesRu[i] != nullptr; i++)
        {
            result++;
        }
    }
    else
    {
        return numStates;
    }

    return result;
}


String Enumeration::ToString() const
{
    if (LANG_IS_RU)
    {
        return String(namesRu[IndexName()]);
    }

    return String(namesEn[IndexName()]);
}


bool Enumeration::SetValue(uint8 v)
{
    if (v >= NumStates())
    {
        return false;
    }

    if (correct == nullptr)
    {
        value = v;

        return true;
    }

    if (correct[v] == false)
    {
        return false;
    }

    value = v;
    
    return true;
}


bool Enumeration::ValidValue(uint8 v) const
{
    if (v >= NumStates())
    {
        return false;
    }

    if (correct == nullptr)
    {
        return true;
    }

    return correct[v];
}


int Enumeration::IndexName() const
{
    if (correct == nullptr)
    {
        return value;
    }

    int index = 0;

    for (int i = 0; i < value; i++)
    {
        if (correct[i])
        {
            index++;
        }
    }

    return index;
}


pchar Enumeration::UGO() const
{
    if (ugoRu == nullptr)
    {
        return "";
    }

    if (LANG_IS_RU)
    {
        return ugoRu[IndexName()];
    }

    return ugoEn[IndexName()];
}


void Button::OnEnterKeyGovernor(const Control &)
{
    funcOnPress();
}


void Choice::OnEnterKeyGovernor(const Control &)
{
    Math::CircleIncrease<uint8>(state, 0, (uint8)(NumStates() - 1));

    funcOnPress();
}


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

        Rectangle(WidthItem(i) - 1, HEIGHT - 1).FillRounded(x, y + 1, 2, colorBack, Color::FILL);

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


void Page::OnKeyRight()
{
    Math::CircleIncrease<int>(&selectedItem, 0, NumItems() - 1);
    Hint::Create(SelectedItem());
}


void Page::OnKeyLeft()
{
    Math::CircleDecrease<int>(&selectedItem, 0, NumItems() - 1);
    Hint::Create(SelectedItem());
}


void Page::VerifySelectedItem()
{
    if (selectedItem >= NumItems())
    {
        selectedItem = NumItems() - 1;
    }
}


void Switch::NextChoice()
{
    if (state->correct == nullptr)
    {
        Math::CircleIncrease<uint8>(&state->value, 0, (uint8)(state->NumStates() - 1));
    }
    else
    {
        do 
        {
            Math::CircleIncrease<uint8>(&state->value, 0, (uint8)(state->NumStates() - 1));
        } while (state->correct[state->value] == false);
    }

    FuncOnPress();
}


void Switch::OnEnterKeyGovernor(const Control &control)
{
    if (control.action != Action::Press)
    {
        return;
    }

    if (Hint::Shown() && Hint::UnderItem() == this)
    {
        NextChoice();
    }

    Hint::Create(this);
}


void Switch::Draw(int x, int y, int width, bool selected)
{
    ColorDraw(selected).SetAsCurrent();

    Text(Title()).Write(x, y + 7, width);
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

        Rectangle(width, height).Fill(x, y, ColorFill());

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

        Rectangle(width - 1, height - 1).Fill(x + 1, y + 1, ColorFill());
        Rectangle(width, height).DrawRounded(x, y, 1, ColorFill());
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


void GovernorChannelColor::OnRotateGovernor(const Control &control)
{
    if (control.key == Key::GovLeft)
    {
        *state = (uint8)(*state - 8);
    }
    else if (control.key == Key::GovRight)
    {
        *state = (uint8)(*state + 8);
    }

    funcChanged(*state);

    SetValue(*state);
}


void Switch::CreateHint(String &h) const
{
    h.Free();
    h.Append(GetHint());
    h.Append(": ");
    h.Append(state->ToString().c_str());
}


void Button::CreateHint(String &h) const
{
    h.Free();
    h.Append(GetHint());
}


void Choice::CreateHint(String &h) const
{
    h.Free();
    h.Append(GetHint());
}


void GovernorChannelColor::CreateHint(String &h) const
{
    h.Free();
    h.Append(GetHint());
}


Item::Item(pchar hintRu, pchar hintEn)
{
    hint[0] = hintRu;
    hint[1] = hintEn;
}



Color Item::ColorBackground(bool selected)
{
    if (!selected)
    {
        if (std::strcmp("Choosing a custom color", hint[1]) == 0)
        {
            Choice *choice = (Choice *)this;
            return choice->colorBack;
        }

    }
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


TypeMeasure *PageModes::GetTypeMeasure() const
{
    Switch *it = (Switch *)items[0];

    return (TypeMeasure *)it->state;
}


int PageModes::GetModeMeasure() const
{
    Switch *it = (Switch *)items[1];

    if (it != nullptr)
    {
        Enumeration *state = it->state;

        return state->value;
    }

    return 0;
}


bool PageModes::ExistTypeMeasure(uint8 type) const
{
    Switch *it = (Switch *)items[0];

    return it->state->correct[type];
}


void PageModes::ResetTypeAndModeMeasure()
{
    Switch *type = (Switch *)items[0];

    if (type->state->correct == nullptr)
    {
        type->state->value = 0;
    }
    else
    {
        for (int i = 0; i < type->state->numStates; i++)
        {
            if (type->state->correct[i])
            {
                type->state->value = (uint8)i;
                break;
            }
        }
    }

    ResetModeMeasure();
}


bool PageModes::ExistModeMeasure(int mode) const
{
    Switch *it = (Switch *)items[1];

    return it->state->correct[mode];
}

void PageModes::ResetModeMeasure()
{
    Switch *mode = (Switch *)items[1];

    if (mode->state->correct == nullptr)
    {
        mode->state->value = 0;
    }
    else
    {
        for (int i = 0; i < mode->state->numStates; i++)
        {
            if (mode->state->correct[i])
            {
                mode->state->value = (uint8)i;
                break;
            }
        }
    }
}


bool Switch::SetValue(uint8 v)
{
    if (state->SetValue(v))
    {
        FuncOnPress();
        Display::Update();
        return true;
    }

    return false;
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


pchar Item::GetHint() const
{
    return hint[gset.language];
}


pchar Switch::Title() const
{
    return text[gset.language];
}


void Parameter::Draw(int x, int y, int width, bool selected)
{

}


void Parameter::OnRotateGovernor(const Control &)
{

}


void Parameter::CreateHint(String &) const
{

}
