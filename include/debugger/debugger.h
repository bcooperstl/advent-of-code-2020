#ifndef __DEBUGGERS_H__
#define __DEBUGGERS_H__

#include <ostream>
#include <string>

#include "debug_streambuf.h"

using namespace std;

class Debugger
{
    private:
        DebugStreambuf m_runner_buf;
        ostream * m_runner_ostream;

        static Debugger * m_instance;
        Debugger();
        ~Debugger();
        bool set_is_on(string name, bool is_on);
    public:
        static Debugger * get_instance();
        bool set_on(string name);
        bool set_off(string name);
        ostream & operator [] (string name);
};

#endif
