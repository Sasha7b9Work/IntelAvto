// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "defines.h"
#include "Display/Colors.h"
#include "Keyboard/Keyboard.h"
#include "Utils/Observer.h"


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
    Item(pchar hintRu, pchar hintEn);
    virtual ~Item() {};

    static const int HEIGHT = 35;

    // ������� ���������
    virtual void Draw(int x, int y, int width, bool selected = false) = 0;

    // ������� ��������� ������� ������/�����
    virtual void OnEnterKeyGovernor(const Control &) { };

    // ��������� �������� �����
    virtual void OnRotateGovernor(const Control &) { };

    Color ColorBackground(bool selected);

    static Color ColorDraw(bool selected);

    pchar GetHint() const;

protected:
    // ����� ����� ��������� ��� ������� �����
    pchar hint[2];

private:
    // ������� ��������� ��� �����
    virtual void CreateHint(String &hint) const = 0;
};


// ������ ����������
class Button : public Item
{
public:
    Button(pchar text_ru, pchar text_en, pchar hintRu, pchar hintEn, void (*funcPress)()) :
        Item(hintRu, hintEn), funcOnPress(funcPress)
    {
        text[0] = text_ru;
        text[1] = text_en;
    };
    virtual void Draw(int x, int y, int width, bool selected = false);
    virtual void OnEnterKeyGovernor(const Control &);
    pchar Title() const;
private:
    pchar text[2];
    void (*funcOnPress)();
    virtual void CreateHint(String &hint) const;
};


// ������ ����������
class Choice : public Item
{
public:
    Choice(pchar hintRu, pchar hintEn, pchar *_namesRu, pchar *_namesEn, void (*funcPress)(), uint8 *_state) :
        Item(hintRu, hintEn), colorBack(Color::MENU_UNSELECT), state(_state), funcOnPress(funcPress)
    {
        namesRu = _namesRu;
        namesEn = _namesEn;
    }

    virtual void Draw(int x, int y, int width, bool selected = false);
    virtual void OnEnterKeyGovernor(const Control &);
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
    virtual void CreateHint(String &) const;
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

    GovernorChannelColor(Type type, pchar hintRu, pchar hintEn, uint8 *_state, void (*func)(uint8)) :
        Item(hintRu, hintEn), state(_state), typeColor(type), funcChanged(func)
    {
    }
    virtual void Draw(int x, int y, int width, bool selected = false);
    virtual void OnRotateGovernor(const Control &);
    int Value() const { return (int)*state; }
    void SetValue(uint8 value);
private:
    uint8 *state;
    Type typeColor;
    virtual void CreateHint(String &) const;
    void (*funcChanged)(uint8);
    Color ColorFill() const;
};


// ��� �������� �����������
class Switch : public Item
{
    friend class Page;
    friend class PageModes;
public:

    Switch(pchar textRu, pchar textEn, pchar hintRu, pchar hintEn, pchar *_namesRu, pchar *_namesEn,
        pchar *_ugoRu, pchar *_ugoEn, Enumeration *_state, void(*_onClick)()) :
        Item(hintRu, hintEn), funcOnPress(_onClick), state(_state)
    {
        text[0] = textRu;
        text[1] = textEn;

        state->namesRu = _namesRu;
        state->namesEn = _namesEn;

        state->ugoRu = _ugoRu;
        state->ugoEn = _ugoEn;

        state->sw = this;
    };
    virtual void Draw(int x, int y, int width, bool selected = false);
    virtual void OnEnterKeyGovernor(const Control &control);

    void FuncOnPress() const { if (funcOnPress) { funcOnPress(); }; }

    uint8 Value() const { return state->value; }

    // ��� ���������� �� SCPI
    void FuncForSCPI(int i, bool correct);

    pchar Title() const;

    bool SetValue(uint8 v);

private:
    pchar       text[2];            // ������� �� �������������
    void        (*funcOnPress)();   // ��� ������� ���������� ����� ��������� ��������� �������������
    Enumeration *state;             // ����� ���������� � ���������� �������������
    virtual void CreateHint(String &hint) const;
    // ����������� � ��������� ���������
    void  NextChoice();
};


class Page : public Item, public Observer
{
    friend struct Channel;

public:
    Page(Item **_items, void (*_onEvent)(EventType::E), void (*_additionalDraw)(), bool equalItems = false) :
        Item("", ""),
        selectedItem(0), items(_items), onEvent(_onEvent), additionalDraw(_additionalDraw), equal_width_items(equalItems)
    {};

    virtual void Draw(int x, int y, int width, bool selected = false);

    // ���������� ��������� �� ���������� ����� ����
    Item *SelectedItem() { return items[selectedItem]; };

    // ���������� true, ���� ��� �������� �������
    bool IsPageModes() const;

    // ���������� true, ���� ��� �������� ��������
    bool IsPageSettings() const;

    void OnKeyRight();
    void OnKeyLeft();

    // ��������� �� ������������ ����� ����������� �����. ���� �� ������, ��� ���������� ������ - ���������������
    void VerifySelectedItem();

    virtual void OnEvent(EventType::E);

    // ���������� true, ���� �������� ����� �������������� ������� ���������
    bool IsAddition() const { return additionalDraw != nullptr; }

    // ����� ���������� �����
    int selectedItem;

protected:

    // ���������� ���������� ������ �� ��������
    int NumItems() const;

    virtual void CreateHint(String &_hint) const { _hint.Free(); };

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
    PageModes(Item **items, void (*onEvent)(EventType::E)) : Page(items, onEvent, nullptr) {}

    // ������� ������������� ��� ������� ������� �������
    TypeMeasure *GetTypeMeasure() const;
    int GetModeMeasure() const;
    bool ExistTypeMeasure(uint8 type) const;
    void ResetTypeAndModeMeasure();
    bool ExistModeMeasure(int mode) const;
    void ResetModeMeasure();
};
