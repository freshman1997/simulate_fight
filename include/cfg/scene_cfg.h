#ifndef __SCENE_CFG_H__
#define __SCENE_CFG_H__

#include "cfg/cfg.h"

struct Scene
{
    int id;
    
};

class SceneCfg : public CfgBase
{
public:
    virtual bool load();


};

#endif // !__SCENE_CFG_H__
