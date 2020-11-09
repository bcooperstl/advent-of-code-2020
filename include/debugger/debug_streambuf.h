#ifndef __DEBUG_STREAMBUF_H__
#define __DEBUG_STREAMBUF_H__

#include <streambuf>
#include <string>

using namespace std;

class DebugStreambuf : public streambuf
{
private:
    bool m_is_on;
protected:
    virtual int_type overflow (int_type c);
public:
    DebugStreambuf();
    DebugStreambuf(bool is_on);
    DebugStreambuf(const DebugStreambuf & other);
    bool is_on();
    void set_is_on(bool is_on);
};

#endif
