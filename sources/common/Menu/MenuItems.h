// 2023/09/08 21:53:53 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "defines.h"
#include "Display/Colors.h"
#include "Utils/Observer.h"
#include "Settings/Settings.h"
#include "Hardware/Keyboard/Keyboard.h"


struct TypeMeasure;
class Switch;


class Item
{
    friend class Hint;
public:
    Item() {}
    virtual ~Item() {}

    static const int HEIGHT = 30;

    // ������� ���������
    virtual void DrawMenuItem(int x, int y, int width, bool selected = false) = 0;

    virtual bool IsParameter() const { return false; }

    // ������� ��������� ������� ������/�����
    virtual bool OnEventControl(const Control &) { return false; }

    Color ColorBackground(bool selected);

    static Color ColorDraw(bool selected);

protected:

    int DeltaTextt() const { return 3; }
};


// ������ ����������
class Button : public Item
{
public:
    Button(pchar text_ru, void (*funcPress)()) :
        Item(), funcOnPress(funcPress)
    {
        text = text_ru;
    }
    virtual void DrawMenuItem(int x, int y, int width, bool selected = false) override;
    virtual bool OnEventControl(const Control &) override;
    pchar GetTitle() const;
    void SetTitle(pchar);
private:
    pchar text;
    void (*funcOnPress)();
};


// ������ ����������
class Choice : public Item
{
public:
    Choice(pchar *_namesRu, void (*funcPress)(), uint8 *_state) :
        Item(), colorBack(Color::MENU_UNSELECT), state(_state), funcOnPress(funcPress)
    {
        namesRu = _namesRu;
    }

    virtual void DrawMenuItem(int x, int y, int width, bool selected = false) override;
//    virtual bool OnEventControl(const Control &) override;
    pchar Title() const;
    int Value() const { return (int)*state; }
    void SetColorBackground(const Color &color) { colorBack = color; }
    Color colorBack;        // ���� ������ ����� ������������ ��� � ������ Choice ��� ������ �����
private:
    pchar *namesRu;
    uint8 *state;
    void (*funcOnPress)();
    int NumStates() const;
};


// ��������� ������ ��������� ������
class GovernorChannelColor : public Item
{
public:

    enum Type
    {
        Red,
        Green,
        Blue
    };

    GovernorChannelColor(Type type, uint8 *_state, void (*func)(uint8)) :
        Item(), state(_state), typeColor(type), funcChanged(func)
    {
    }
    virtual void DrawMenuItem(int x, int y, int width, bool selected = false) override;
    int Value() const { return (int)*state; }
    void SetValue(uint8 value);
private:
    uint8 *state;
    Type typeColor;
    void (*funcChanged)(uint8);
    Color ColorFill() const;
};


class Parameter : public Item
{
public:
    Parameter(pchar title_ru,
        Value *value12, Value _min12, Value _max12,
        Value *value24, Value _min24, Value _max24,
        int _x, int _y) :
        Item(),
        min12(_min12), max12(_max12),
        min24(_min24), max24(_max24),
        x(_x), y(_y)
    {
        value[VoltageMode::_12] = value12;
        value[VoltageMode::_24] = value24;

        title = title_ru;
    }

    virtual void DrawMenuItem(int x, int y, int width, bool selected = false) override;

    void Draw() const;

    virtual bool OnEventControl(const Control &) override;

    pchar Title() const;

    Value &GetValue() const;

    const Value &GetMin() const;
    const Value &GetMax() const;

    int GetX() const { return x; }
    int GetY() const { return y; }

    virtual bool IsParameter() const override { return true; }

    // ������ �������
    bool IsNowSelected() const { return Parameter::current == this; }

    // ������ �������������
    bool IsNowEdited() const { return Parameter::editable == this; }

    // "�������" �������� - ���, ������� ������ ������� � ����. nullptr, ���� �������� �������
    // ���� - �� ��������
    static Parameter *current;

    // ������������� � ������ ������
    static Parameter *editable;

private:

    pchar title;

    Value *value[VoltageMode::Count];

    const Value min12;
    const Value max12;

    const Value min24;
    const Value max24;

    int x = 0;
    int y = 0;
};


class Page : public Item, public Observer
{
    friend struct Channel;

public:
    Page(Item **_items, void (*_onEvent)(EventType::E), void (*_additionalDraw)(), void (*_func_start_test)()) :
        Item(),
        items(_items), onEvent(_onEvent), additionalDraw(_additionalDraw), func_start_test(_func_start_test), selectedItem(0)
    {}

    virtual void DrawMenuItem(int x, int y, int width, bool selected = false) override;

    virtual bool OnEventControl(const Control &) override;

    // ���������� ��������� �� ���������� ����� ����
    Item *SelectedItem() { return items[selectedItem]; }

    // ��������� �� ������������ ����� ����������� �����. ���� �� ������, ��� ���������� ������ - ���������������
    void VerifySelectedItem();

    virtual void OnEvent(EventType::E) override;

    // ���������� true, ���� �������� ����� �������������� ������� ���������
    bool IsAddition() const { return additionalDraw != nullptr; }

    void DrawParameters() const;

    static const Page *ForCurrentSignal();

    // ��������� ����
    void StartTest() const;

protected:

    // ���������� ���������� ������ �� ��������
    int NumItems() const;

    // ���������� ������ �������� ���� � ������� num
    int WidthItem(int num) const;

    // ��������� �� ������ ��������� ����. ������������� ����.
    Item **items;

    void (*onEvent)(EventType::E);

    void (*additionalDraw)();

    void (*func_start_test)();

private:

    // ����� ���������� �����
    int selectedItem;
};


class PageModes : public Page
{
public:
    PageModes(Item **_items, void (*_onEvent)(EventType::E)) : Page(_items, _onEvent, nullptr, nullptr) {}
};
