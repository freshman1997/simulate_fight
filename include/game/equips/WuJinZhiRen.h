#ifndef __WuJinZhiRen_H__
#define __WuJinZhiRen_H__
#include "../equip.h"

class HeroBase;

// +10%攻击力，+20%暴击几率。 提供20%额外攻击力和15额外暴击几率。
class WuJinZhiRen : public EquipBase
{
public:
    virtual void update();

    // 进入战斗前计算
    virtual void on_begin() = 0;

};

#endif