#ifndef __BUFF_CFG_H__
#define __BUFF_CFG_H__
#include "cfg.h"

struct buff_cfg_t
{
    int id;

};

class BuffCfg : public CfgBase
{
public:
    virtual bool load();

public:
    bool load_config(json &);
};

#endif