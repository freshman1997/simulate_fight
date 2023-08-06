#ifndef __TIMER_H__
#define __TIMER_H__

//#include <sys/epoll.h>
#include <functional>
#include <chrono>
#include <set>
#include <memory>
#include <iostream>

using namespace std;

// 定时结点的基类，存储唯一标识的元素，轻量级，用于比较
struct TimerNodeBase {
    time_t expire;  // 任务触发时间
    int64_t id;     // 用来描述插入先后顺序，int64_t，能记录5000多年
};

// 定时结点，包含定时任务等
struct TimerNode : public TimerNodeBase {
    // 定时器任务回调函数
    // 函数对象拷贝代价高，在容器内拷贝构造后不会再去移动
    using Callback = std::function<void(const TimerNode &node)>;
    Callback func;

    // 构造函数，容器内部就地拷贝构造调用一次，此后不会再去调用
    TimerNode(int64_t id, time_t expire, Callback func) : func(func) {
        this->expire = expire;
        this->id = id;
    }
};

// 根据触发时间对结点进行排序
// 基类引用，多态特性，基类代替timerNode结点，避免拷贝构造子类
bool operator < (const TimerNodeBase &lhd, const TimerNodeBase &rhd);

// 定时器类的实现
class Timer {
public:
    // 获取当前时间
    static time_t GetTick() {
        // 获取系统时间戳，系统启动到当前的时间
        auto sc = chrono::time_point_cast<chrono::milliseconds>(chrono::steady_clock::now());
        // 获取到时间戳的时间段
        auto temp = chrono::duration_cast<chrono::milliseconds>(sc.time_since_epoch());
        return temp.count();
    }

    // 2、添加定时任务
    // 参数： msec 任务触发时间间隔，func 任务执行的回调函数
    TimerNodeBase AddTimer(time_t msec, TimerNode::Callback func) {
        time_t expire = GetTick() + msec;

        // 如果timermap为空，或者触发时间<=timermap的最后一个（最大的）结点的时间，正常插入
        if (timermap.empty() || expire <= timermap.crbegin()->expire){
            auto pairs = timermap.emplace(GenID(), expire, std::move(func));
            return static_cast<TimerNodeBase>(*pairs.first);
        }
        // 否则直接插入最后，emplace_hint插入时间复杂度是O(1)
        auto ele = timermap.emplace_hint(timermap.crbegin().base(), GenID(), expire, std::move(func));
        return static_cast<TimerNodeBase>(*ele);
    }

    // 3、删除/取消定时任务
    bool DelTimer(TimerNodeBase &node) {
        // C++14的新特性：只需传递等价 key 比较，无需创建 key 对象比较，
        // 代替子类结点，避免函数对象复制控制和移动
        auto iter = timermap.find(node);
        // 若存在，则删除该结点
        if (iter != timermap.end()) {
            timermap.erase(iter);
            return true;
        }
        return false;
    }

    // 4、检测定时任务是否被触发，触发则执行定时任务
    bool CheckTimer() {
        auto iter = timermap.begin();
        if (iter != timermap.end() && iter->expire <= GetTick()) {
            // 定时任务被触发，则执行对应的定时任务
            iter->func(*iter);
            // 删除执行完毕的定时任务
            timermap.erase(iter);
            return true;
        }
        return false;
    }

    // 5、返回最近定时任务触发时间，作为timeout的参数
    time_t TimeToSleep() {
        auto iter = timermap.begin();
        if (iter == timermap.end()) {
            return -1;
        }
        // 最近任务的触发时间 = 最近任务初始设置的触发时间 - 当前时间
        time_t diss = iter->expire - GetTick();
        // 最近要触发的任务时间 > 0，继续等待；= 0，立即处理任务
        return diss > 0 ? diss : 0;
    }

private:
    // 产生 id 的方法
    static int64_t GenID() {
        return gid++;
    }
    static int64_t gid;
    // 利用 set 排序快速查找要到期的任务 
    set<TimerNode, std::less<>> timermap;
};


#endif