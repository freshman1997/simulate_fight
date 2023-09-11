#ifndef  OUTPUT_FRAME_BUFF_QUEUE_H__
#define OUTPUT_FRAME_BUFF_QUEUE_H__
#include "nlohmann/json_fwd.hpp"
#include <list>

enum class ouput_type
{
    unit_move,
    unit_atk,
    unit_perform_skill,
    unit_buff_add,
    unit_buff_rm,
    
};

class OutputFrameBuffQueue
{
public:
    void update();

private:
    void add_output(const nlohmann::json &);

public:
    void fight_begin();
    void update_position();
    void update_properties();

private:

};

#endif // ! OUTPUT_FRAME_BUFF_QUEUE_H__
