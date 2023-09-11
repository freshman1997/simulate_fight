#include <ctime>
#include "time/time.h"

namespace datetime { 
    
    time_t now()
    {
        return std::time(nullptr);
    }

    std::string time_2_string()
    {
        #ifdef _WIN32
        time_t currentTime;
        currentTime = time(NULL);
        struct tm time_sct;
        localtime_s(&time_sct,&currentTime);
        struct tm *t_tm = &time_sct;
    #else
        time_t currentTime;
        time(&currentTime);
        tm* t_tm = localtime(&currentTime);
    #endif
        char formatTime[64] = {0};
        snprintf(formatTime, 64, "%04d-%02d-%02d %02d:%02d:%02d", 
                                t_tm->tm_year + 1900,
                                t_tm->tm_mon + 1,
                                t_tm->tm_mday,
                                t_tm->tm_hour,
                                t_tm->tm_min,
                                t_tm->tm_sec);
                                
        return std::string(formatTime);
    }
}