#include "timer/timer.h"

int64_t Timer::gid = 0;

// 根据触发时间对结点进行排序
// 基类引用，多态特性，基类代替timerNode结点，避免拷贝构造子类
bool operator < (const TimerNodeBase &lhd, const TimerNodeBase &rhd) {
    // 先比较触发时间
    if (lhd.expire < rhd.expire)
        return true;
    else if (lhd.expire > rhd.expire) 
        return false;
    // 触发时间相同，比较插入的先后顺序
    // 比较id大小，先插入的结点id小，先执行
    return lhd.id < rhd.id;
}