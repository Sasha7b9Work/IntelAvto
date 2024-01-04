// 2023/09/08 21:53:53 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "defines.h"
#include "Display/Colors.h"
#include "Keyboard/Keyboard.h"
#include "Utils/Observer.h"
#include "Utils/Value.h"


struct TypeMeasure;
class Switch;


class Item
{
    friend class Hint;
public:
    Item() {}
    virtual ~Item() {}

    static const int HEIGHT = 35;

    // Функция отрисовки
    virtual void Draw(int x, int y, int width, bool selected = false) = 0;

    // Функция обработки нажатия кнопки/ручки
    virtual void OnEnterKeyGovernor(const Control &) { }

    // Обработак поворота ручки
    virtual void OnRotateGovernor(const Control &) { }

    Color ColorBackground(bool selected);

    static Color ColorDraw(bool selected);
};


// Общего назначения
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


// Общего назначения
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
    Color colorBack;        // Этим цветом будем отрисовывать фон в случае Choice для выбора цвета
private:
    pchar *namesRu;
    pchar *namesEn;
    uint8 *state;
    void (*funcOnPress)();
    int NumStates() const;
};


// Настройка одного цветового канала
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
    Parameter(pchar title_ru, pchar title_en, Value &_value, int _x, int _y) : Item(), value(_value), x(_x), y(_y)
    {
        title[0] = title_ru;
        title[1] = title_en;
        
        (void)value;
    }

    virtual void Draw(int x, int y, int width, bool selected = false) override;
    void Draw() const;
    virtual void OnRotateGovernor(const Control &) override;

    pchar Title() const;

    Value GetValue() const { return value; }

    int GetX() const { return x; }
    int GetY() const { return y; }

private:

    pchar title[2];
    Value &value;
    int x;
    int y;
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

    // Возвращает указатель на выделенный пункт меню
    Item *SelectedItem() { return items[selectedItem]; }

    void OnKeyRight();
    void OnKeyLeft();

    // Проверить на корректность номер выделенного итема. Если он больше, чем количество итемов - скорректировать
    void VerifySelectedItem();

    virtual void OnEvent(EventType::E) override;

    // Возвращает true, если страница имеет дополнительную функцию отрисовки
    bool IsAddition() const { return additionalDraw != nullptr; }

    // Номер выбранного итема
    int selectedItem;

protected:

    // Возвращает количество итемов на странице
    int NumItems() const;

    // Возвращает ширину элемента меню с номером num
    int WidthItem(int num) const;

    // Указатель на массив элементов меню. Заканчивается нулём.
    Item **items;

    void (*onEvent)(EventType::E);

    void (*additionalDraw)();

    bool equal_width_items;         // Если true, то ширина итемов при отрисовке одинаковая
};


class PageModes : public Page
{
public:
    PageModes(Item **_items, void (*_onEvent)(EventType::E)) : Page(_items, _onEvent, nullptr) {}
};
