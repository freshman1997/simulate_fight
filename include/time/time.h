#ifndef __TIME_H__
#define __TIME_H__
#include <crtdefs.h>
#include <string>

namespace datetime {

    time_t now();

    std::string time_2_string();

}

#endif // end time