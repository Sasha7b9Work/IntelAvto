// 2023/09/08 21:53:49 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Settings/Settings.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Pages.h"
#include "Display/Primitives_.h"
#include "Display/Display_.h"
#include "Display/Text_.h"
#include "Hardware/Keyboard/Keyboard_.h"
#include "Utils/Math_.h"
#include <cstring>


using namespace Primitives;


Parameter *Parameter::current = nullptr;
Parameter *Parameter::editable = nullptr;


int Choice::NumStates() const
{
    int result = 0;

    while (names[result])
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

        Rect(width - 2, height - 2).Fill(x - 2, y - 2, Color::BLUE);

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

    if (!selected)
    {
        Color::WHITE.SetAsCurrent();
    }

    Text(GetTitle()).Write(x, y + DeltaTextt(), width);
}


void Choice::DrawMenuItem(int x, int y, int width, bool selected)
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


bool Choice::OnEventControl(const Control &control)
{
    if ((control.key == Key::OK || control.key == Key::GovButton) && control.IsPress())
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
    }

    return true;
}


bool Parameter::OnEventControl(const Control &control)
{
    if (control.IsRelease())
    {
        if (control.key == Key::OK ||
            control.key == Key::GovButton)
        {
            if (IsNowEdited())
            {
                editable = nullptr;

                GetValue().SetValue(GetMin(), GetMax());
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
