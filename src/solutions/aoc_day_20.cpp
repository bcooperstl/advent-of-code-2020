#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <iterator>

#include "aoc_day_20.h"
#include "file_utils.h"

using namespace std;

Tile::Tile(int id)
{
    m_id = id;
    for (int i=0; i<TILE_SIDE_LEN; i++)
    {
        for (int j=0; j<TILE_SIDE_LEN; j++)
        {
            m_map[i][j]=TILE_CHAR_OFF;
        }
    }
}

Tile::~Tile()
{
}

void Tile::set_map(vector<string> map_lines)
{
    for (int i=0; i<TILE_SIDE_LEN; i++)
    {
        for (int j=0; j<TILE_SIDE_LEN; j++)
        {
            m_map[i][j]=map_lines[i][j];
        }
    }
    build_border_values();
}

void Tile::display()
{
    cout << "Tile " << m_id << ":" << endl;
    for (int y=0; y<TILE_SIDE_LEN; y++)
    {
        for (int x=0; x<TILE_SIDE_LEN; x++)
        {
            cout << m_map[y][x];
        }
        cout << endl;
    }
    cout << endl;
}

int Tile::get_id()
{
    return m_id;
}

vector<string> Tile::get_tile_map()
{
    vector<string> lines;
    ostringstream line;
    for (int y=0; y<TILE_SIDE_LEN; y++)
    {
        line.str("");
        for (int x=0; x<TILE_SIDE_LEN; x++)
        {
            line << m_map[y][x];
        }
        lines.push_back(line.str());
    }
    return lines;
}

int Tile::get_border(Border border)
{
    return m_borders[border];
}

map<Border, int> Tile::get_borders()
{
    map<Border, int> borders;
    for (int i=0; i<TILE_NUM_BORDERS; i++)
    {
        borders[static_cast<Border>(i)]=m_borders[i];
    }
    return borders;
}

/* Initial plan:
* Initialize all 8 border values to 0.
* For i from 0 to 9
    * if map[0][i] is '#' (The i'th value on the north edge is #; bit=1)
        * Set border[NorthFront] |= (1 << 9-i) // For clockwise NorthFront, most sig bit is first (i=0) and least sig bit is last (i=9)
        * Set border[NorthBack] |= (1 << i) // For counter-clockwise NorthBack, most sig bit is last (i=9) and least sig bit is first (i=0)
    * if map[9][i] is '#' (The i'th value on the south edge is #; bit=1)
        * Set border[SouthFront] |= (1 << i) // For clockwise SouthFront, most sig bit is last (i=9) and least sig bit is first (i=0)
        * Set border[SouthBack] |= (1 << 9-i) // For counter-clockwise SouthBack, most sig bit is first (i=0) and least sig bit is last (i=9)
    * if map[i][0] is '#' (The i'th value on the west edge is #; bit=1)
        * Set border[WestFront] |= (1 << i) // For clockwise WestFront, most sig bit is last (i=9) and least sig bit is first (i=0)
        * Set border[WestBack] |= (1 << 9-i) // For counter-clockwise WestBack, most sig bit is first (i=0) and least sig bit is last (i=9)
    * if map[i][9] is '#' (The i'th value on the east edge is #; bit=1)
        * Set border[EastFront] |= (1 << 9-i) // For clockwise EastFront, most sig bit is first (i=0) and least sig bit is last (i=9)
        * Set border[EastBack] |= (1 << i) // For counter-clockwise EastBack, most sig bit is last (i=9) and least sig bit is first (i=0)
*/
void Tile::build_border_values()
{
    for (int i=0; i<TILE_NUM_BORDERS; i++)
    {
        m_borders[i]=0;
    }
    
    for (int i=0; i<=9; i++)
    {
        if (m_map[0][i] == TILE_CHAR_ON)
        {
            m_borders[NorthFront] |= (1 << (9-i));
            m_borders[NorthBack]  |= (1 << i);
        }
        if (m_map[9][i] == TILE_CHAR_ON)
        {
            m_borders[SouthFront] |= (1 << i);
            m_borders[SouthBack]  |= (1 << (9-i));
        }
        if (m_map[i][0] == TILE_CHAR_ON)
        {
            m_borders[WestFront] |= (1 << i);
            m_borders[WestBack]  |= (1 << (9-i));
        }
        if (m_map[i][9] == TILE_CHAR_ON)
        {
            m_borders[EastFront] |= (1 << (9-i));
            m_borders[EastBack]  |= (1 << i);
        }
    }
    /* Palindrom check */
    if (m_borders[NorthFront] == m_borders[NorthBack])
    {
        cout << "PALINDROME detected on tile " << m_id << " For the North edge" << endl;
    }
    if (m_borders[EastFront] == m_borders[EastBack])
    {
        cout << "PALINDROME detected on tile " << m_id << " For the East edge" << endl;
    }
    if (m_borders[SouthFront] == m_borders[SouthBack])
    {
        cout << "PALINDROME detected on tile " << m_id << " For the South edge" << endl;
    }
    if (m_borders[WestFront] == m_borders[WestBack])
    {
        cout << "PALINDROME detected on tile " << m_id << " For the West edge" << endl;
    }
}

/*
All of these (except right rotate 0 degrees) will copy the current border values to old_borders and their current map to old_map. 
They will then update the internal structures as described below.

See the documentation that describes how to perform these 8 manipulations on the tile. Not going to copy-and-paste 100 lines in here.
*/

void Tile::perform_manipulation(Manipulation manipulation)
{
    map<Border, int> old_borders = get_borders();
    char old_map[TILE_SIDE_LEN][TILE_SIDE_LEN];
    for (int y=0; y<TILE_SIDE_LEN; y++)
    {
        for (int x=0; x<TILE_SIDE_LEN; x++)
        {
            old_map[y][x]=m_map[y][x];
        }
    }
    switch (manipulation)
    {
        case RightRotate0Degrees:
            // Nothing to do here for the right-rotate 0 degrees
            break;
        case RightRotate90Degrees:
            m_borders[EastFront]  = old_borders[NorthFront];
            m_borders[EastBack]   = old_borders[NorthBack];
            m_borders[SouthFront] = old_borders[EastFront];
            m_borders[SouthBack]  = old_borders[EastBack];
            m_borders[WestFront]  = old_borders[SouthFront];
            m_borders[WestBack]   = old_borders[SouthBack];
            m_borders[NorthFront] = old_borders[WestFront];
            m_borders[NorthBack]  = old_borders[WestBack];  
            for (int old_y=0; old_y<TILE_SIDE_LEN; old_y++)
            {
                for (int old_x=0; old_x<TILE_SIDE_LEN; old_x++)
                {
                    m_map[old_x][9-old_y] = old_map[old_y][old_x];
                }
            }
            break;
        case RightRotate180Degrees:
            m_borders[SouthFront] = old_borders[NorthFront];
            m_borders[SouthBack]  = old_borders[NorthBack];
            m_borders[WestFront]  = old_borders[EastFront];
            m_borders[WestBack]   = old_borders[EastBack];
            m_borders[NorthFront] = old_borders[SouthFront];
            m_borders[NorthBack]  = old_borders[SouthBack];
            m_borders[EastFront]  = old_borders[WestFront];
            m_borders[EastBack]   = old_borders[WestBack];  
            for (int old_y=0; old_y<TILE_SIDE_LEN; old_y++)
            {
                for (int old_x=0; old_x<TILE_SIDE_LEN; old_x++)
                {
                    m_map[9-old_y][9-old_x] = old_map[old_y][old_x];
                }
            }
            break;
        case RightRotate270Degrees:
            m_borders[WestFront]  = old_borders[NorthFront];
            m_borders[WestBack]   = old_borders[NorthBack];
            m_borders[NorthFront] = old_borders[EastFront];
            m_borders[NorthBack]  = old_borders[EastBack];
            m_borders[EastFront]  = old_borders[SouthFront];
            m_borders[EastBack]   = old_borders[SouthBack];
            m_borders[SouthFront] = old_borders[WestFront];
            m_borders[SouthBack]  = old_borders[WestBack];  
            for (int old_y=0; old_y<TILE_SIDE_LEN; old_y++)
            {
                for (int old_x=0; old_x<TILE_SIDE_LEN; old_x++)
                {
                    m_map[9-old_x][old_y] = old_map[old_y][old_x];
                }
            }
            break;
        case HorizontalFlip:
            m_borders[SouthBack]  = old_borders[NorthFront];
            m_borders[SouthFront] = old_borders[NorthBack];
            m_borders[EastBack]   = old_borders[EastFront];
            m_borders[EastFront]  = old_borders[EastBack];
            m_borders[NorthBack]  = old_borders[SouthFront];
            m_borders[NorthFront] = old_borders[SouthBack];
            m_borders[WestBack]   = old_borders[WestFront];
            m_borders[WestFront]  = old_borders[WestBack];  
            for (int old_y=0; old_y<TILE_SIDE_LEN; old_y++)
            {
                for (int old_x=0; old_x<TILE_SIDE_LEN; old_x++)
                {
                    m_map[9-old_y][old_x] = old_map[old_y][old_x];
                }
            }
            break;
        case VerticalFlip:
            m_borders[NorthBack]  = old_borders[NorthFront];
            m_borders[NorthFront] = old_borders[NorthBack];
            m_borders[WestBack]   = old_borders[EastFront];
            m_borders[WestFront]  = old_borders[EastBack];
            m_borders[SouthBack]  = old_borders[SouthFront];
            m_borders[SouthFront] = old_borders[SouthBack];
            m_borders[EastBack]   = old_borders[WestFront];
            m_borders[EastFront]  = old_borders[WestBack];  
            for (int old_y=0; old_y<TILE_SIDE_LEN; old_y++)
            {
                for (int old_x=0; old_x<TILE_SIDE_LEN; old_x++)
                {
                    m_map[old_y][9-old_x] = old_map[old_y][old_x];
                }
            }
            break;
        case ForwardSlashFlip:
            m_borders[EastBack]   = old_borders[NorthFront];
            m_borders[EastFront]  = old_borders[NorthBack];
            m_borders[NorthBack]  = old_borders[EastFront];
            m_borders[NorthFront] = old_borders[EastBack];
            m_borders[WestBack]   = old_borders[SouthFront];
            m_borders[WestFront]  = old_borders[SouthBack];
            m_borders[SouthBack]  = old_borders[WestFront];
            m_borders[SouthFront] = old_borders[WestBack];  
            for (int old_y=0; old_y<TILE_SIDE_LEN; old_y++)
            {
                for (int old_x=0; old_x<TILE_SIDE_LEN; old_x++)
                {
                    m_map[9-old_x][9-old_y] = old_map[old_y][old_x];
                }
            }
            break;
        case BackwardSlashFlip:
            m_borders[WestBack]   = old_borders[NorthFront];
            m_borders[WestFront]  = old_borders[NorthBack];
            m_borders[SouthBack]  = old_borders[EastFront];
            m_borders[SouthFront] = old_borders[EastBack];
            m_borders[EastBack]   = old_borders[SouthFront];
            m_borders[EastFront]  = old_borders[SouthBack];
            m_borders[NorthBack]  = old_borders[WestFront];
            m_borders[NorthFront] = old_borders[WestBack];  
            for (int old_y=0; old_y<TILE_SIDE_LEN; old_y++)
            {
                for (int old_x=0; old_x<TILE_SIDE_LEN; old_x++)
                {
                    m_map[old_x][old_y] = old_map[old_y][old_x];
                }
            }
            break;            
    }
}

AocDay20::AocDay20():AocDay(20)
{
}

AocDay20::~AocDay20()
{
}

void AocDay20::parse_input(string filename, vector<Tile *> & tiles)
{
    FileUtils fileutils;
    vector<string> data;
    if (!fileutils.read_as_list_of_strings(filename, data))
    {
        cerr << "Error reading in the data from " << filename << endl;
    }
    
    for (int i=0; i<data.size(); i+=12) // twelve lines per entry - one for ID line, ten for map, one for blank between tiles
    {
        string id_str = data[i].substr(5, 4);
        int id = strtol(id_str.c_str(), NULL, 10);
        cout << "Parsing tile " << id << endl;
        Tile * tile = new Tile(id);
        // the map goes from lines i+1 to i+10. creating the subvector needs i+11 since the last element isn't included
        vector<string>::iterator first_line=data.begin()+(i+1);
        vector<string>::iterator last_line=data.begin()+(i+11);
        tile->set_map(vector<string>(first_line, last_line));
        tiles.push_back(tile);
    }
    
    return;
}


string AocDay20::part1(string filename, vector<string> extra_args)
{
    vector<Tile *> tiles;
    parse_input(filename, tiles);
    long long product = 1;

    for (int i=0; i<tiles.size(); i++)
    {
        tiles[i]->display();
    }
    
    ostringstream out;
    out << product;
    
    // Clean up the dynamically allocated tiles
    for (int i=0; i<tiles.size(); i++)
    {
        delete tiles[i];
    }
    tiles.clear();
    return out.str();
}

