// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Menu/MenuItems.h"


#define DEF_BUTTON(name, titleRu, funcPress)                                                               \
static Button name(titleRu, funcPress)

#define DEF_CHOICE_2(name, hintRu, nameRu1, nameRu2, nameEn1, nameEn2, func)                                \
static pchar n##name##Ru[] = { nameRu1, nameRu2, nullptr };                                                         \
static uint8 state##name;                                                                                           \
static Choice name(hintRu, hintEn, n##name##Ru, n##name##En, func, &state##name);

#define DEF_CHOICE_3(name, hintRu, nameRu1, nameRu2, nameRu3, nameEn1, nameEn2, nameEn3, state, func)       \
static pchar n##name##Ru[] = { nameRu1, nameRu2, nameRu3, nullptr };                                                \
static Choice name(hintRu, hintEn, n##name##Ru, n##name##En, func, &state);

#define DEF_CHOICE_4(name, hintRu,                                                                          \
    nameRu1, nameRu2, nameRu3, nameRu4, nameEn1, nameEn2, nameEn3, nameEn4, func)                                   \
static pchar n##name##Ru[] = { nameRu1, nameRu2, nameRu3, nameRu4, nullptr };                                       \
static uint8 state##name;                                                                                           \
static Choice name(hintRu, hintEn, n##name##Ru, n##name##En, func, &state##name);

#define DEF_CHOICE_5(name, hintRu,                                                                          \
    nameRu1, nameRu2, nameRu3, nameRu4, nameRu5, nameEn1, nameEn2, nameEn3, nameEn4, nameEn5, func)                 \
static pchar n##name##Ru[] = { nameRu1, nameRu2, nameRu3, nameRu4, nameRu5, nullptr };                              \
static pchar n##name##En[] = { nameEn1, nameEn2, nameEn3, nameEn4, nameEn5, nullptr };                              \
static uint8 state##name;                                                                                           \
static Choice name(hintRu, hintEn, n##name##Ru, n##name##En, func, &state##name);

#define DEF_CHOICE_6(name, hintRu, hintEn,                                                                          \
    nameRu1, nameRu2, nameRu3, nameRu4, nameRu5, nameRu6,                                                           \
    nameEn1, nameEn2, nameEn3, nameEn4, nameEn5, nameEn6, func)                                                     \
static pchar n##name##Ru[] = { nameRu1, nameRu2, nameRu3, nameRu4, nameRu5, nameRu6, nullptr };                     \
static pchar n##name##En[] = { nameEn1, nameEn2, nameEn3, nameEn4, nameEn5, nameEn6, nullptr };                     \
static uint8 state##name;                                                                                           \
static Choice name(hintRu, hintEn, n##name##Ru, n##name##En, func, &state##name);

#define DEF_CHOICE_7(name, hintRu, hintEn,                                                                          \
    nameRu1, nameRu2, nameRu3, nameRu4, nameRu5, nameRu6, nameRu7,                                                  \
    nameEn1, nameEn2, nameEn3, nameEn4, nameEn5, nameEn6, nameEn7, func)                                            \
static pchar n##name##Ru[] = { nameRu1, nameRu2, nameRu3, nameRu4, nameRu5, nameRu6, nameRu7, nullptr };            \
static uint8 state##name;                                                                                           \
static Choice name(hintRu, hintEn, n##name##Ru, n##name##En, func, &state##name);

#define DEF_CHOICE_8(name, hintRu, hintEn,                                                                          \
    nameRu1, nameRu2, nameRu3, nameRu4, nameRu5, nameRu6, nameRu7, nameRu8,                                         \
    nameEn1, nameEn2, nameEn3, nameEn4, nameEn5, nameEn6, nameEn7, nameEn8, func)                                   \
static pchar n##name##Ru[] = { nameRu1, nameRu2, nameRu3, nameRu4, nameRu5, nameRu6, nameRu7, nameRu8, nullptr };   \
static uint8 state##name;                                                                                           \
static Choice name(hintRu, hintEn, n##name##Ru, n##name##En, func, &state##name);

#define DEF_CHOICE_9(name, hintRu, hintEn,                                                                          \
    nameRu1, nameRu2, nameRu3, nameRu4, nameRu5, nameRu6, nameRu7, nameRu8, nameRu9,                                \
    nameEn1, nameEn2, nameEn3, nameEn4, nameEn5, nameEn6, nameEn7, nameEn8, nameEn9, func)                          \
static pchar n##name##Ru[] = { nameRu1, nameRu2, nameRu3, nameRu4, nameRu5, nameRu6, nameRu7, nameRu8, nameRu9,     \
    nullptr };                                                                                                      \
static uint8 state##name;                                                                                           \
static Choice name(hintRu, hintEn, n##name##Ru, n##name##En, func, &state##name);

#define DEF_COLOR_GOVERNOR(name, type, hintRu, hintEn, func)                                                        \
static uint8 state##name;                                                                                           \
static GovernorChannelColor name(type, hintRu, hintEn, &state##name, func)

#define DEF_SWITCH_UGO_2(name, titleRu, titleEn, hintRu, hintEn,                                                    \
    nameRu1, nameRu2, nameEn1, nameEn2, ugoRu1, ugoRu2, ugoEn1, ugoEn2, state, func)                                \
static pchar n##name##Ru[] = { nameRu1, nameRu2, nullptr };                                                         \
static pchar ugo##name##Ru[] = { ugoRu1, ugoRu2, nullptr };                                                         \
static Switch name(titleRu, titleEn, hintRu, hintEn, n##name##Ru, n##name##En, ugo##name##Ru, ugo##name##En,        \
    &state, func);

#define DEF_SWITCH_2(name, titleRu, titleEn, hintRu, hintEn,                                                        \
    nameRu1, nameRu2, nameEn1, nameEn2, state, func)                                                                \
static pchar n##name##Ru[] = { nameRu1, nameRu2, nullptr };                                                         \
static Switch name(titleRu, titleEn, hintRu, hintEn, n##name##Ru, n##name##En, nullptr, nullptr, &state, func);


#define DEF_SWITCH_3(name, titleRu, titleEn, hintRu, hintEn,                                                        \
    nameRu1, nameRu2, nameRu3, nameEn1, nameEn2, nameEn3, state, func)                                              \
static pchar n##name##Ru[] = { nameRu1, nameRu2, nameRu3, nullptr };                                                \
static Switch name(titleRu, titleEn, hintRu, hintEn, n##name##Ru, n##name##En, nullptr, nullptr, &state, func);

#define DEF_SWITCH_4(name, titleRu, titleEn, hintRu, hintEn,                                                        \
    nameRu1, nameRu2, nameRu3, nameRu4, nameEn1, nameEn2, nameEn3, nameEn4, state, func)                            \
static pchar n##name##Ru[] = { nameRu1, nameRu2, nameRu3, nameRu4, nullptr };                                       \
static Switch name(titleRu, titleEn, hintRu, hintEn, n##name##Ru, n##name##En, nullptr, nullptr, &state, func);

#define DEF_SWITCH_5(name, titleRu, hintRu,                                                                         \
    nameRu1, nameRu2, nameRu3, nameRu4, nameRu5, nameEn1, nameEn2, nameEn3, nameEn4, nameEn5, state, func)          \
static pchar n##name##Ru[] = { nameRu1, nameRu2, nameRu3, nameRu4, nameRu5, nullptr };                              \
static Switch name(titleRu, titleEn, hintRu, hintEn, n##name##Ru, n##name##En, nullptr, nullptr, &state, func);

#define DEF_SWITCH_6(name, titleRu, hintRu,                                                                         \
    nameRu1, nameRu2, nameRu3, nameRu4, nameRu5, nameRu6, state, func)                                              \
static pchar n##name##Ru[] = { nameRu1, nameRu2, nameRu3, nameRu4, nameRu5, nameRu6, nullptr };                     \
static Switch name(titleRu, titleEn, hintRu, hintEn, n##name##Ru, n##name##En, nullptr, nullptr, &state, func);

#define DEF_SWITCH_8(name, titleRu,                                                                                 \
    nameRu1, nameRu2, nameRu3, nameRu4, nameRu5, nameRu6, nameRu7, nameRu8, state, func)                            \
static pchar n##name##Ru[] = { nameRu1, nameRu2, nameRu3, nameRu4, nameRu5, nameRu6, nameRu7, nameRu8, nullptr };   \
static Switch name(titleRu, titleEn, n##name##Ru, n##name##En, nullptr, nullptr, &state, func);
