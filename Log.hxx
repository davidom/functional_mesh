#ifndef __LOG_INCLUDE
#define __LOG_INCLUDE

#include <iostream>
#include <sstream>

using std::stringstream;
using std::ostream;

//#define DEBUG 1

#ifdef DEBUG
  #define LOGFUNCTION LOG<<__PRETTY_FUNCTION__<<"\n"
#else
  #define LOGFUNCTION
#endif
#define PRINTLOG std::cout<<LOG<<std::endl

class Log
{
    stringstream _ss;

   public:
    template <typename T>
    friend Log& operator<<(Log& log, T& t) {
        log._ss << t;
        return log;
    }
    friend ostream& operator<<(ostream& stream, Log const& log) {
        stream << log._ss.str();
        return stream;
    }
};

extern Log LOG;

#endif
