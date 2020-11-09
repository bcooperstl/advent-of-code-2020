#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__

#define DATA_DIR "../data/"

// Test file format is a CSV file, using a single quote as the quote character and a # as the comment character
#define TEST_INDEX_FILE "test_index.txt"
#define TEST_INDEX_DELIM ','
#define TEST_INDEX_QUOTE '\''
#define TEST_INDEX_COMMENT '#'

// This is the list of debugger names
#define DEBUGGER_ALL "all"
#define DEBUGGER_RUNNER "runner"
#define DEBUG_RUNNER (*debugger)["runner"]


#endif
