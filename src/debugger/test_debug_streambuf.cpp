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
    off.set_is_on(true);
    on.set_is_on(false);
    outOff << "This should now show" << endl;
    outOn << "This should now not show" << endl;
    
    return 0;
}