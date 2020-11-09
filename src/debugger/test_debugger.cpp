#include <iostream>

#include "debugger.h"
#include "constants.h"

int main(int argc, char * argv[])
{
    Debugger * debugger = Debugger::get_instance();
    
    debugger->set_on("runner");
    DEBUG_RUNNER << "This should show" << endl;
    debugger->set_off("runner");
    DEBUG_RUNNER << "This should not show" << endl;
    debugger->set_on("all");
    DEBUG_RUNNER << "This should show again" << endl;
    
    return 0;
}