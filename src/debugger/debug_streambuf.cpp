#include <streambuf>
#include <string>

using namespace std;

#include "debug_streambuf.h"

typedef typename DebugStreambuf::int_type int_type;

DebugStreambuf::DebugStreambuf()
{
    m_is_on = true;
}

DebugStreambuf::DebugStreambuf(bool is_on)
{
    m_is_on = is_on;
}

DebugStreambuf::DebugStreambuf(const DebugStreambuf & other)
{
    m_is_on=other.m_is_on;
}

int_type DebugStreambuf::overflow(int_type c)
{
    if (c != EOF)
    {
        if (m_is_on)
        {
            if (putchar(c) == EOF)
            {
                return EOF;
            }
        }
    }
    return c;
}

bool DebugStreambuf::is_on()
{
    return m_is_on;
}

void DebugStreambuf::set_on()
{
    m_is_on = true;
}

void DebugStreambuf::set_off()
{
    m_is_on = false;
}
