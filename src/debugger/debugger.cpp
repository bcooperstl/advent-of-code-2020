#include <iostream>
#include <ostream>

#include "debugger.h"
#include "debug_streambuf.h"
#include "constants.h"

using namespace std;

Debugger * Debugger::m_instance = NULL;

Debugger::Debugger()
{
    // Create all the buffers with logging turned off
    m_runner_buf = DebugStreambuf(false);
    m_runner_ostream = new ostream(&m_runner_buf);
}

Debugger * Debugger::get_instance()
{
    if (m_instance == NULL)
    {
        m_instance = new Debugger();
    }
    return m_instance;
}

Debugger::~Debugger()
{
    delete m_runner_ostream;
}

bool Debugger::set_is_on(string name, bool is_on)
{
    bool matched = false;
    if (name == DEBUGGER_ALL || name == DEBUGGER_RUNNER)
    {
        m_runner_buf.set_is_on(is_on);
        matched = true;
    }
    
    if (!matched)
    {
        cerr << "No matching debugger for " << name << " found!" << endl;
    }
    
    return !matched;
}

bool Debugger::set_on(string name)
{
    return set_is_on(name, true);
}

bool Debugger::set_off(string name)
{
    return set_is_on(name, false);
}

ostream & Debugger::operator [] (string name)
{
    if (name == DEBUGGER_RUNNER)
        return *m_runner_ostream;
    cerr << "**INVALID DEBUGGER NAME " << name << " GIVEN" << endl;
    return cerr;
}
