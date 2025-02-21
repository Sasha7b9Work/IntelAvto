// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Menu/MenuItems.h"


inline void FuncVV() { }

#define DEF_BUTTON(name, titleRu, funcPress)                                                                        \
static Button name(titleRu, funcPress)

#define DEF_CHOICE_2(name, nameRu1, nameRu2, states, func)                                                          \
static pchar n##name##Ru[] = { nameRu1, nameRu2, nullptr };                                                         \
static Choice name(n##name##Ru, func, ((uint8 *)&states));

#define DEF_CHOICE_3(name, hintRu, nameRu1, nameRu2, nameRu3, nameEn1, nameEn2, nameEn3, state, func)               \
static pchar n##name##Ru[] = { nameRu1, nameRu2, nameRu3, nullptr };                                                \
static Choice name(hintRu, hintEn, n##name##Ru, n##name##En, func, &state);
