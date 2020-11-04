#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>

#include "file_utils.h"

// Really basic way to parse a line based on delimiters. 
// parameters:
//  input = the input string to split. assume newline is removed, or else it will be addded to the last item
//  delimiter = the value to split on
//  quote = an optional parameter - a quote character to indicate a 
std::vector<std::string> FileUtils::split_line_to_strings(std::string input, char delimiter, char quote_char, char comment_char)
{
    //TODO: Wrap this in my debug logic
    std::cout << "original input is [" << input << "]" << std::endl;

    std::vector<std::string> splits;
    if (comment_char)
    {
        std::size_t comment_pos = input.find(comment_char); // find the position of the comment character
        input = input.substr(comment_pos); // will truncate off starting at the comment character. if no comment character, this will make no changes
        //TODO: Wrap this in my debug logic
        std::cout << "after dropping comment " << comment_char << ", input to parse is [" << input << "]" << std::endl;
    }
    
    char * pos = (char *)input.c_str();
    
    bool in_quote = false;
    std::ostringstream current;
    while (*pos != '\0')
    {
        if (quote_char && in_quote)
        {
            if (*pos == quote_char)
            {
                in_quote = false;
            }
            else
            {
                current << *pos;
            }
        }
        else
        {
            if (quote_char && *pos == quote_char)
            {
                in_quote = true;
            }
            else if (*pos == delimiter)
            {
                // ABCDE,ABCDE,ABCDE
                // first string goes from 0 to 4...construct as string(0,5). pos will be 5 for the comma. so 5-0=5
                // second string goes from 6 to 10...construct as string(6,5)...pos wil be 11 for the comma. so 11-6=5
                // third string goes from 12 to 16...construct as string(12,5) but need to do this out of loop as its the last string
                //TODO: Wrap this in my debug logic
                std::cout << "appending [" << current.str() << "] as a string" << std::endl;
                splits.push_back(current.str());
                current = std::ostringstream();
            }
            else
            {
                current << (*pos);
            }
            pos++;
        }
    }
    // append the last string. pos will be pointed to the null terminator at 17, so string(12,5) would be pos(17)-start(12)
    //TODO: Wrap this in my debug logic
    std::cout << "appending [" << current.str() << "] as the last string" << std::endl;
    splits.push_back(current.str());
    return splits;
}

bool FileUtils::read_as_list_of_strings(std::string filename, std::vector<std::string> & lines)
{
    std::ifstream infile(filename);
    if (!infile)
    {
        std::cerr << "*****Error opening file " << filename << std::endl;
        return false;
    }
    std::string line;
    while (std::getline(infile, line))
    {
        //TODO Add debugging logic
        std::cout << "Read line [" << line << "] from file" << std::endl;
        lines.push_back(line);
    }
    infile.close();
    return true;
}

bool FileUtils::read_as_list_of_split_strings(std::string filename, std::vector<std::vector<std::string>> & split_strings, char delimiter, char quote_char, char comment_char)
{
    std::vector<std::string> lines;
    if (!read_as_list_of_strings(filename, lines))
    {
        return false;
    }
    for (std::vector<std::string>::iterator iter = lines.begin(); iter != lines.end(); ++iter)
    {
        split_strings.push_back(split_line_to_strings(*iter, delimiter, quote_char, comment_char));
    }
    return true;
}

bool FileUtils::read_as_list_of_split_longs(std::string filename, std::vector<std::vector<long>> & split_longs, char delimiter, char quote_char, char comment_char)
{
    std::vector<std::string> lines;
    if (!read_as_list_of_strings(filename, lines))
    {
        return false;
    }
    for (std::vector<std::string>::iterator iter = lines.begin(); iter != lines.end(); ++iter)
    {
        std::vector<std::string> long_strings = split_line_to_strings(*iter, delimiter, quote_char, comment_char);
        std::vector<long> longs;
        for (std::vector<std::string>::iterator str_long_iter = long_strings.begin(); str_long_iter != long_strings.end(); ++str_long_iter)
        {
            std::string str_long = *str_long_iter;
            long l;
            if (safe_strtol(str_long, l))
            {
                std::cerr << "Error on conversion to long integer from [" << str_long << "]" << std::endl;
                return false;
            }
            longs.push_back(l);
        }
        split_longs.push_back(longs);
    }
    return true;
}

bool FileUtils::safe_strtol(std::string str_long, long & l)
{
    char * endptr;
    l = strtol(str_long.c_str(), &endptr, 10);
    if (endptr != 0)
    {
        std::cerr << "Error on conversion to long integer from [" << str_long << "]" << std::endl;
        return false;
    }
    return true;
}
