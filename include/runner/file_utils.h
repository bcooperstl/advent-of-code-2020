#ifndef __FILE_UTILS_H__
#define __FILE_UTILS_H__

#include <vector>
#include <string>

class FileUtils {
    private:
        std::vector<std::string> split_line_to_strings(std::string input, char delimiter, char quote_char, char comment_char);
    public:
        bool read_as_list_of_strings(std::string filename, std::vector<std::string> & lines);
        bool read_as_list_of_split_strings(std::string filename, std::vector<std::vector<std::string>> & split_strings, char delimiter, char quote_char, char comment_char);
        bool read_as_list_of_split_longs(std::string filename, std::vector<std::vector<long>> & split_longs, char delimiter, char quote_char, char comment);
        bool safe_strtol(std::string str_long, long & l);// TODO: Probably move this to another file
};

#endif
