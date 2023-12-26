// 2023/09/08 21:53:53 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "defines.h"
#include "Display/Colors.h"
#include "Keyboard/Keyboard.h"
#include "Utils/Observer.h"
#include "Utils/Value.h"


struct TypeMeasure;
class Switch;


struct Enumeration
{
    uint8  value;           // ������� ��������� ������������
    pchar *namesRu;
    pchar *namesEn;
    pchar *ugoRu;
    pchar *ugoEn;
    const bool *correct;    // ���� ��������� �� ���� ������ �� ����� nullptr, �� ��� �������� ������������ ���������
                            // ������� : value �� ����� �������� ��������, ��������������� false
    explicit Enumeration(uint8 v, const bool *_correct = nullptr, int states = 0);
    operator int()         { return (int)value; }
    String ToString() const;
    pchar UGO() const;
    int NumStates() const;
    // ���������� ������ �� ������� names, �������������� �������� value
    int IndexName() const;
    bool Is(uint8 v) const { return value == v; }
    Switch *sw;
    bool SetValue(uint8 v);
    bool ValidValue(uint8 v) const;
    int numStates;
};


class Item
{
    friend class Hint;
public:
    Item() {}
    virtual ~Item() {}

    static const int HEIGHT = 35;

    // ������� ���������
    virtual void Draw(int x, int y, int width, bool selected = false) = 0;

    // ������� ��������� ������� ������/�����
    virtual void OnEnterKeyGovernor(const Control &) { }

    // ��������� �������� �����
    virtual void OnRotateGovernor(const Control &) { }

    Color ColorBackground(bool selected);

    static Color ColorDraw(bool selected);
};


// ������ ����������
class Button : public Item
{
public:
    Button(pchar text_ru, pchar text_en, void (*funcPress)()) :
        Item(), funcOnPress(funcPress)
    {
        text[0] = text_ru;
        text[1] = text_en;
    }
    virtual void Draw(int x, int y, int width, bool selected = false) override;
    virtual void OnEnterKeyGovernor(const Control &) override;
    pchar GetTitle() const;
    void SetTitle(pchar ru, pchar en);
private:
    pchar text[2];
    void (*funcOnPress)();
};


// ������ ����������
class Choice : public Item
{
public:
    Choice(pchar *_namesRu, pchar *_namesEn, void (*funcPress)(), uint8 *_state) :
        Item(), colorBack(Color::MENU_UNSELECT), state(_state), funcOnPress(funcPress)
    {
        namesRu = _namesRu;
        namesEn = _namesEn;
    }

    virtual void Draw(int x, int y, int width, bool selected = false) override;
    virtual void OnEnterKeyGovernor(const Control &) override;
    pchar Title() const;
    int Value() const { return (int)*state; }
    void SetColorBackground(const Color &color) { colorBack = color; }
    Color colorBack;        // ���� ������ ����� ������������ ��� � ������ Choice ��� ������ �����
private:
    pchar *namesRu;
    pchar *namesEn;
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
    virtual void Draw(int x, int y, int width, bool selected = false) override;
    virtual void OnRotateGovernor(const Control &) override;
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
    Parameter(pchar title_ru, pchar title_en, Value &_value) : Item(), value(_value)
    {
        title[0] = title_ru;
        title[1] = title_en;
        
        (void)value;
    }

    virtual void Draw(int x, int y, int width, bool selected = false) override;
    virtual void OnRotateGovernor(const Control &) override;

    pchar Title() const;

    Value GetValue() const { return value; }

private:

    pchar title[2];
    Value &value;
};


// ��� �������� �����������
class Switch : public Item
{
    friend class Page;
    friend class PageModes;
public:

    Switch(pchar textRu, pchar textEn, pchar *_namesRu, pchar *_namesEn,
        pchar *_ugoRu, pchar *_ugoEn, Enumeration *_state, void(*_onClick)()) :
        Item(), funcOnPress(_onClick), state(_state)
    {
        text[0] = textRu;
        text[1] = textEn;

        state->namesRu = _namesRu;
        state->namesEn = _namesEn;

        state->ugoRu = _ugoRu;
        state->ugoEn = _ugoEn;

        state->sw = this;
    }
    virtual void Draw(int x, int y, int width, bool selected = false) override;
    virtual void OnEnterKeyGovernor(const Control &control) override;

    void FuncOnPress() const { if (funcOnPress) { funcOnPress(); } }

    uint8 Value() const { return state->value; }

    pchar Title() const;

    bool SetValue(uint8 v);

private:
    pchar       text[2];            // ������� �� �������������
    void        (*funcOnPress)();   // ��� ������� ���������� ����� ��������� ��������� �������������
    Enumeration *state;             // ����� ���������� � ���������� �������������
    // ����������� � ��������� ���������
    void  NextChoice();
};


class Page : public Item, public Observer
{
    friend struct Channel;

public:
    Page(Item **_items, void (*_onEvent)(EventType::E), void (*_additionalDraw)(), bool equalItems = false) :
        Item(),
        selectedItem(0), items(_items), onEvent(_onEvent), additionalDraw(_additionalDraw), equal_width_items(equalItems)
    {}

    virtual void Draw(int x, int y, int width, bool selected = false) override;

    // ���������� ��������� �� ���������� ����� ����
    Item *SelectedItem() { return items[selectedItem]; }

    void OnKeyRight();
    void OnKeyLeft();

    // ��������� �� ������������ ����� ����������� �����. ���� �� ������, ��� ���������� ������ - ���������������
    void VerifySelectedItem();

    virtual void OnEvent(EventType::E) override;

    // ���������� true, ���� �������� ����� �������������� ������� ���������
    bool IsAddition() const { return additionalDraw != nullptr; }

    // ����� ���������� �����
    int selectedItem;

protected:

    // ���������� ���������� ������ �� ��������
    int NumItems() const;

    // ���������� ������ �������� ���� � ������� num
    int WidthItem(int num) const;

    // ��������� �� ������ ��������� ����. ������������� ����.
    Item **items;

    void (*onEvent)(EventType::E);

    void (*additionalDraw)();

    bool equal_width_items;         // ���� true, �� ������ ������ ��� ��������� ����������
};


class PageModes : public Page
{
public:
    PageModes(Item **_items, void (*_onEvent)(EventType::E)) : Page(_items, _onEvent, nullptr) {}

    // ������� ������������� ��� ������� ������� �������
    TypeMeasure *GetTypeMeasure() const;
    int GetModeMeasure() const;
    bool ExistTypeMeasure(uint8 type) const;
    void ResetTypeAndModeMeasure();
    bool ExistModeMeasure(int mode) const;
    void ResetModeMeasure();
};
