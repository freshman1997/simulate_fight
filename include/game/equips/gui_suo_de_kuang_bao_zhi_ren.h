#ifndef __GUI_SUO_DE_KUANG_BAO_ZHI_REN_H__
#define __GUI_SUO_DE_KUANG_BAO_ZHI_REN_H__

#include "game/equip.h"
class GuiSuoDeKuangBaoZhiRen : public EquipmentBase
{
public:
    virtual Object * clone();
    virtual Object * clone_and_clean();

    virtual bool init();
    virtual void deinit();

    virtual void update(float deltaTime);

    virtual void on_being_hurt(int damage, int type, int sub_type);
    virtual void on_attack(float &damage);
    virtual void on_performed_skill();

private:
    bool changed;
    int lv;
};

#endif