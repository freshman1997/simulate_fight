﻿#include "game/skills/TianBengDiLie.h"
#include "manager/object_manager.hpp"

REGISTER_TYPE("tian_beng_di_lie", TianBengDiLie)

Object * TianBengDiLie::clone()
{
    return new TianBengDiLie;
}

Object * TianBengDiLie::clone_and_clean()
{
    return clone();
}

void TianBengDiLie::perform(float deltaTime, bool first)
{
    if (!first) {
        return;
    }

    // 设置跳过去的位置坐标
    send_buff();
}

