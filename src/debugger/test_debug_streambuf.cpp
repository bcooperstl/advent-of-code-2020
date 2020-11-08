#include <iostream>

#include "debug_streambuf.h"

int main(int argc, char * argv[])
{
    DebugStreambuf on(true);
    DebugStreambuf off(false);
    
    std::ostream outOn(&on);
    std::ostream outOff(&off);
    
    outOn << "This should show" << endl;
    outOff << "This should not show" << endl;
    off.set_on();
    on.set_off();
    outOff << "This should now show" << endl;
    outOn << "This should now not show" << endl;
    
    return 0;
}