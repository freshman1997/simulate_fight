#include "game/equips/gui_suo_de_kuang_bao_zhi_ren.h"
#include "game/hero.h"

Object * GuiSuoDeKuangBaoZhiRen::clone()
{
    return new GuiSuoDeKuangBaoZhiRen;
}

Object * GuiSuoDeKuangBaoZhiRen::clone_and_clean()
{
    return clone();
}

bool GuiSuoDeKuangBaoZhiRen::init()
{
    lv = 0;
    changed = false;
    return true;
}

void GuiSuoDeKuangBaoZhiRen::deinit()
{

}

void GuiSuoDeKuangBaoZhiRen::update(float deltaTime)
{
    if (changed) {
        ++lv;
        
        this->owner->add_atk_speed(1);
    }
}

void GuiSuoDeKuangBaoZhiRen::on_being_hurt(int damage, int type, int sub_type)
{
    // 累积伤害？
    changed = true;
}

void GuiSuoDeKuangBaoZhiRen::on_attack(float &damage)
{
    changed = true;
}

void GuiSuoDeKuangBaoZhiRen::on_performed_skill()
{
    
}
