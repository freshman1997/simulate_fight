#ifndef __CFG_H__
#define __CFG_H__
#include <ios>
#include <string>
#include <fstream>
#include <iostream>

#include "nlohmann/json.hpp"
#include "nlohmann/json_fwd.hpp"

using namespace nlohmann;

class CfgBase
{
public:
    virtual bool load() = 0;

public:

#define READ_CFG(_name, func) do { if (!parse_config(_name, func)) { return false; } } while(false);

    template<class T>
    bool parse_config(const char* name, bool(T::*pFunc)(json &))
    {
        static_assert(std::is_base_of<CfgBase, T>::value, "T must base from CfgBase");
        const auto caller = dynamic_cast<T *>(this);
	    assert(caller);

        std::fstream file;
        const std::string config_path_file_name = std::string(config_path) + name;
        file.open(config_path_file_name);
        if (!file.good()) {
            return false;            
        }

        try {
            json json_data = json::parse(file);
            return (caller->*pFunc)(json_data);
        } catch(json::parse_error e) {
            std::cerr << e.what() << std::endl;
            return false;
        } catch (...) {
            return false;
        }
    }

    void set_cfg_name(const char *_name) { this->cfg_name = _name; }
    void set_config_path(const char *_name) { this->config_path = _name; }

protected:
    const char * config_path;
    std::string cfg_name;
};

#endif