// 2023/09/08 21:53:53 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "defines.h"
#include "Settings/Settings.h"
#include "Menu/ParameterDrawStruct.h"
#include <cstring>


struct TypeMeasure;
class Switch;
struct Control;


class Item
{
    friend class Hint;
public:
    Item() {}

    static int Height();

    // ������� ���������
    virtual void Draw(int x, int y, int width, bool selected = false) = 0;

    virtual bool IsParameter() const { return false; }

    // ������� ��������� ������� ������/�����
    virtual bool OnEventControl(const Control &) { return false; }

    Color ColorBackground(bool selected);

    static Color ColorDraw(bool selected);

    bool IsOpened() const
    {
        return is_opened;
    }

    virtual void Open()
    {
        is_opened = true;
    }

    virtual void Close()
    {
        is_opened = false;
    }

protected:

    int DeltaTextt() const { return 3; }

    bool is_opened = false;
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
    virtual void Draw(int x, int y, int width, bool selected = false) override;
    virtual bool OnEventControl(const Control &) override;
    pchar GetTitle() const;
    void SetTitle(pchar);
private:
    pchar text;
    void (*funcOnPress)();
};


enum TypeFieldLAN
{
    Address,
    Port,
    MAC
};


// ���� ��� ����� ��������� ���� - ������, ����� ��� MAC
class FieldLAN : public Button
{
public:
    FieldLAN(TypeFieldLAN _type, pchar text_ru, uint8 *_address) :
        Button(text_ru, [](){}), type(_type), address(_address) { }

    virtual void Draw(int x, int y, int widht, bool selected = false) override;
    virtual bool OnEventControl(const Control &) override;

    virtual void Open() override;
    virtual void Close() override;

    bool ConvertStringToAddress(uint8[4]) const;

private:

    TypeFieldLAN type;
    uint8 *address;
    static const int SIZE_BUFFER = 64;
    char buffer[SIZE_BUFFER];           // ����� �������� � ��������� ����
    bool edited = false;                // ���� true, �� ��������� � ����� �������������� - ��� ��� ����� ����
};


// ������ ����������
class Choice : public Item
{
public:
    Choice(pchar *_names, void (*funcPress)(), uint8 *_state) :
        Item(), colorBack(Color::MENU_UNSELECT), state(_state), funcOnPress(funcPress)
    {
        names = _names;
    }

    virtual void Draw(int x, int y, int width, bool selected = false) override;
    virtual bool OnEventControl(const Control &) override;
    pchar Title() const;
    int Value() const { return (int)*state; }
    void SetColorBackground(const Color &color) { colorBack = color; }
    Color colorBack;        // ���� ������ ����� ������������ ��� � ������ Choice ��� ������ �����
private:
    pchar *names;
    uint8 *state;
    void (*funcOnPress)();
    int NumStates() const;
};


class Parameter : public Item
{
public:
    Parameter(pchar title_ru,
        Value *value12, const Value &_min12, const Value &_max12,
        Value *value24, const Value &_min24, const Value &_max24,
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

    virtual void Draw(int x, int y, int width, bool selected = false) override;

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

    ParameterDrawStruct ds;
};


class VParameter : public Parameter
{
public:
    VParameter(pchar title_ru,
        Value *value12, const Voltage &_min12, const Voltage &_max12,
        Value *value24, const Voltage &_min24, const Voltage &_max24,
        int _x, int _y) :
        Parameter(title_ru, value12, _min12, _max12, value24, _min24, _max24, _x, _y)
    {
    }
};


class TParameter : public Parameter
{
public:
    TParameter(pchar title_ru,
        Value *value12, const Time &_min12, const Time &_max12,
        Value *value24, const Time &_min24, const Time &_max24,
        int _x, int _y) :
        Parameter(title_ru, value12, _min12, _max12, value24, _min24, _max24, _x, _y)
    {
    }
};

class CParameter : public Parameter
{
public:
    CParameter(pchar title_ru,
        Value *value12, const Counter &_min12, const Counter &_max12,
        Value *value24, const Counter &_min24, const Counter &_max24,
        int _x, int _y) :
        Parameter(title_ru, value12, _min12, _max12, value24, _min24, _max24, _x, _y)
    {
    }
};

class Page : public Item
{
    friend struct Channel;

public:
    Page(Item **_items, void (*_additionalDraw)(), void (*_func_start_test)()) :
        Item(),
        items(_items), additionalDraw(_additionalDraw), func_start_test(_func_start_test), selectedItem(0)
    {}

    virtual void Draw(int x, int y, int width, bool selected = false) override;

    virtual bool OnEventControl(const Control &) override;

    // ���������� ��������� �� ���������� ����� ����
    Item *SelectedItem() { return items[selectedItem]; }

    // ��������� �� ������������ ����� ����������� �����. ���� �� ������, ��� ���������� ������ - ���������������
    void VerifySelectedItem();

    // ���������� true, ���� �������� ����� �������������� ������� ���������
    bool IsAddition() const { return additionalDraw != nullptr; }

    // ���������� �������� ���������� �� ����������� �������
    void DrawParameters() const;

    static const Page *ForCurrentSignal();

    // ��������� ����
    void StartTest() const;

    // ���������� true, ���� ������� �������� - �������� �������
    static bool IsSignal(Page *);

    // ���������� ���������� ������ �� ��������
    int NumItems() const;

    // ���������� ��������� �������
    static void DrawConnectionServer();

    bool ConsistOpenedItems() const;

protected:

    // ���������� ������ �������� ���� � ������� num
    int WidthItem(int num) const;

    // ��������� �� ������ ��������� ����. ������������� ����.
    Item **items;

    void (*additionalDraw)();

    void (*func_start_test)();

private:

    // ����� ���������� �����
    int selectedItem;
};


class PageModes : public Page
{
public:
    PageModes(Item **_items) : Page(_items, nullptr, nullptr) {}
};
