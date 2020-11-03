#ifndef __FILE_UTILS_H__
#define __FILE_UTILS_H__

#include <vector>
#include <string>

class FileUtils {
    private:
        std::vector<std::string> split_line_to_strings(std::string input, char delimiter, char quote_char, char comment);
    
};

#endif
