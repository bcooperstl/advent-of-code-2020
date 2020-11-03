#include <string>
#include <vector>
#include <iostream>
#include <sstream>

#include "file_utils.h"

// Really basic way to parse a line based on delimiters. 
// parameters:
//  input = the input string to split. assume newline is removed, or else it will be addded to the last item
//  delimiter = the value to split on
//  quote = an optional parameter - a quote character to indicate a 
std::vector<std::string> FileUtils::split_line_to_strings(std::string input, char delimiter, char quote_char, char comment)
{
    //TODO: Wrap this in my debug logic
    std::cout << "original input is [" << input << "]" << std::endl;

    std::vector<std::string> splits;
    if (comment)
    {
        std::size_t comment_pos = input.find(comment); // find the position of the comment character
        input = input.substr(comment_pos); // will truncate off starting at the comment character. if no comment character, this will make no changes
        //TODO: Wrap this in my debug logic
        std::cout << "after dropping comment " << comment << ", input to parse is [" << input << "]" << std::endl;
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
