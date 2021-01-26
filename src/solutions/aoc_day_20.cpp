#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <iterator>
#include <cmath>

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

int Tile::get_paired_border(Border border)
{
    switch (border)
    {
        case NorthFront:
            return m_borders[NorthBack];
        case NorthBack:
            return m_borders[NorthFront];
        case EastFront:
            return m_borders[EastBack];
        case EastBack:
            return m_borders[EastFront];
        case SouthFront:
            return m_borders[SouthBack];
        case SouthBack:
            return m_borders[SouthFront];
        case WestFront:
            return m_borders[WestBack];
        case WestBack:
            return m_borders[WestFront];
    }
    return 0;
}

int Tile::get_min_border(Border border1, Border border2)
{
    return (m_borders[border1] <= m_borders[border2] ? m_borders[border1] : m_borders[border2]);
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

Puzzle::Puzzle(int side_len)
{
    m_side_len = side_len;
    m_puzzle = new Tile ** [m_side_len];
    for (int i=0; i<m_side_len; i++)
    {
        m_puzzle[i] = new Tile * [m_side_len];
        for (int j=0; j<m_side_len; j++)
        {
            m_puzzle[i][j]=NULL;
        }
    }
}

Puzzle::~Puzzle()
{
    for (int i=0; i<m_side_len; i++)
    {
        delete m_puzzle[i];
    }
    delete m_puzzle;
}

Tile * Puzzle::get_tile(int x, int y)
{
    return m_puzzle[y][x];
}

void Puzzle::set_tile(int x, int y, Tile * tile)
{
    m_puzzle[y][x]=tile;
}

AocDay20::AocDay20():AocDay(20)
{
    build_manipulation_table();
}

AocDay20::~AocDay20()
{
}

/*
First I will build up a `map<int, vector<Tile *>>` border_index that will point each border value to a list of tiles with that value.  
This will use the minimum value for a Front/Back border pair, to eliminate duplicating sides and to make identificaiton easier.  
*/

void AocDay20::build_border_lookup_map(vector<Tile *> & tiles, map<int, vector<Tile *>> & lookup_map)
{
    lookup_map.clear();
    for (int i=0; i<tiles.size(); i++)
    {
        Tile * tile = tiles[i];
        int north_value = tile->get_min_border(NorthFront, NorthBack);
        if (lookup_map.find(north_value) == lookup_map.end())
        {
            lookup_map[north_value]=vector<Tile *>();
        }
        lookup_map[north_value].push_back(tile);

        int east_value = tile->get_min_border(EastFront, EastBack);
        if (lookup_map.find(east_value) == lookup_map.end())
        {
            lookup_map[east_value]=vector<Tile *>();
        }
        lookup_map[east_value].push_back(tile);

        int south_value = tile->get_min_border(SouthFront, SouthBack);
        if (lookup_map.find(south_value) == lookup_map.end())
        {
            lookup_map[south_value]=vector<Tile *>();
        }
        lookup_map[south_value].push_back(tile);

        int west_value = tile->get_min_border(WestFront, WestBack);
        if (lookup_map.find(west_value) == lookup_map.end())
        {
            lookup_map[west_value]=vector<Tile *>();
        }
        lookup_map[west_value].push_back(tile);
    }
}     

void AocDay20::classify_tiles(vector<Tile *> & tiles, vector<Tile *> & corners, vector<Tile *> & edges, vector<Tile *> & middles, map<int, vector<Tile *>> & lookup_map)
{
    for (int i=0; i<tiles.size(); i++)
    {
        Tile * tile = tiles[i];
        cout << "Classifying tile " << tile->get_id() << endl;;
        int num_singles=0;
        
        int north_value = tile->get_min_border(NorthFront, NorthBack);
        if (lookup_map[north_value].size() == 1)
        {
            cout << " North value " << north_value << " only appears once; this must be on the outside." << endl;
            num_singles++;
        }
        else
        {
            cout << " North value " << north_value << " appears " << lookup_map[north_value].size() << " times. This must be in the middle." << endl;
        }

        int east_value = tile->get_min_border(EastFront, EastBack);
        if (lookup_map[east_value].size() == 1)
        {
            cout << " East value " << east_value << " only appears once; this must be on the outside." << endl;
            num_singles++;
        }
        else
        {
            cout << " East value " << east_value << " appears " << lookup_map[east_value].size() << " times. This must be in the middle." << endl;
        }

        int south_value = tile->get_min_border(SouthFront, SouthBack);
        if (lookup_map[south_value].size() == 1)
        {
            cout << " South value " << south_value << " only appears once; this must be on the outside." << endl;
            num_singles++;
        }
        else
        {
            cout << " South value " << south_value << " appears " << lookup_map[south_value].size() << " times. This must be in the middle." << endl;
        }

        int west_value = tile->get_min_border(WestFront, WestBack);
        if (lookup_map[west_value].size() == 1)
        {
            cout << " West value " << west_value << " only appears once; this must be on the outside." << endl;
            num_singles++;
        }
        else
        {
            cout << " West value " << west_value << " appears " << lookup_map[west_value].size() << " times. This must be in the middle." << endl;
        }
        
        switch (num_singles)
        {
            case 0:
                cout << " There are no single sides. This must be a middle tile" << endl;
                middles.push_back(tile);
                break;
            case 1:
                cout << " There is one single sides. This must be an edge tile" << endl;
                edges.push_back(tile);
                break;
            case 2:
                cout << " There are two single sides. This must be a corner tile" << endl;
                corners.push_back(tile);
                break;
            default:
                cout << "****** There are " << num_singles << " singles. This is an ERROR*****" << endl;
                break;
        }
    }
    cout << "Of the " << tiles.size() << " tiles:" << endl;
    cout << "  There are " << corners.size() << " corners" << endl;
    cout << "  There are " << edges.size() << " edges" << endl;
    cout << "  There are " << middles.size() << " middles" << endl;
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
    vector<Tile *> tiles, corners, edges, middles;
    map<int, vector<Tile *>> border_lookup_map;
    parse_input(filename, tiles);
    long long product = 1;

    for (int i=0; i<tiles.size(); i++)
    {
        tiles[i]->display();
    }
    
    build_border_lookup_map(tiles, border_lookup_map);
    classify_tiles(tiles, corners, edges, middles, border_lookup_map);
    
    for (int i=0; i<corners.size(); i++)
    {
        product *= (long long)(corners[i]->get_id());
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

/* Planned approach
* Choose the first corner in the list to go in the top-left of the puzzle.
* Identify which two sides have only one Tile in the lookup map:
    * If it is North and West, this is good to go.
    * If it is North and East, do a RightRotate270Degrees to align them to North and West
    * If it is East and South, do a RightRotate180Degrees to align them to North and West
    * If it is South and West, do a RightRotate90Degrees to align them to North and West.
* Place the properly-rotate tile in the puzzle[0][0] location.
*/

void AocDay20::align_for_top_left_corner(Tile * tile, map<int, vector<Tile *>> & border_lookup_map)
{
    cout << "Aligning corner tile " << tile->get_id() << " in the top-left corner" << endl;
    
    int north_lookup_count = border_lookup_map[tile->get_min_border(NorthFront, NorthBack)].size();
    int east_lookup_count = border_lookup_map[tile->get_min_border(EastFront, EastBack)].size();
    int south_lookup_count = border_lookup_map[tile->get_min_border(SouthFront, SouthBack)].size();
    int west_lookup_count = border_lookup_map[tile->get_min_border(WestFront, WestBack)].size();
    
    cout << "North lookup count is " << north_lookup_count 
         << " East lookup count is " << east_lookup_count
         << " South lookup count is " << south_lookup_count
         << " West lookup count is " << west_lookup_count << endl;
    
    if (north_lookup_count == 1 && east_lookup_count == 2 && south_lookup_count == 2 && west_lookup_count == 1)
    {
        cout << "No rotation needed" << endl;
    }
    else if (north_lookup_count == 1 && east_lookup_count == 1 && south_lookup_count == 2 && west_lookup_count == 2)
    {
        cout << "Right Rotating by 270 degrees" << endl;
        tile->perform_manipulation(RightRotate270Degrees);
    }
    else if (north_lookup_count == 2 && east_lookup_count == 1 && south_lookup_count == 1 && west_lookup_count == 2)
    {
        cout << "Right Rotating by 180 degrees" << endl;
        tile->perform_manipulation(RightRotate180Degrees);
    }
    else if (north_lookup_count == 2 && east_lookup_count == 2 && south_lookup_count == 1 && west_lookup_count == 1)
    {
        cout << "Right Rotating by 90 degrees" << endl;
        tile->perform_manipulation(RightRotate90Degrees);
    }
    else 
    {
        cout << "*****ERROR determining rotation for top-left tile*****" << endl;
    }
}

string AocDay20::part2(string filename, vector<string> extra_args)
{
    vector<Tile *> tiles, corners, edges, middles;
    map<int, vector<Tile *>> border_lookup_map;
    parse_input(filename, tiles);
    long long product = 1;
    
    build_border_lookup_map(tiles, border_lookup_map);
    classify_tiles(tiles, corners, edges, middles, border_lookup_map);
    
    int puzzle_side_len = (int) sqrt(tiles.size());
    cout << "There are " << tiles.size() << " tiles, so the puzzle is " << puzzle_side_len << "x" << puzzle_side_len << endl;
    
    align_for_top_left_corner(corners[0], border_lookup_map);
    Puzzle puzzle(puzzle_side_len);
    puzzle.set_tile(0, 0, corners[0]);
    
    for (int i=0; i<corners.size(); i++)
    {
        product *= (long long)(corners[i]->get_id());
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

/* 
See day20_manipuliation_table.md for the details of this table.
This will be a lookup table to get the maniuplaiton needed to move a value from any border to any border.
The first index is the FromBorder and the second index is the ToBorder.
*/

Manipulation AocDay20::m_manipulation_table[TILE_NUM_BORDERS][TILE_NUM_BORDERS];

void AocDay20::build_manipulation_table()
{
    cout << "entering build manipulation_table" << endl;
    m_manipulation_table[NorthFront][NorthFront]=RightRotate0Degrees;
    m_manipulation_table[NorthFront][EastFront ]=RightRotate90Degrees;
    m_manipulation_table[NorthFront][SouthFront]=RightRotate180Degrees;
    m_manipulation_table[NorthFront][WestFront ]=RightRotate270Degrees;
    m_manipulation_table[NorthFront][NorthBack ]=VerticalFlip;
    m_manipulation_table[NorthFront][EastBack  ]=ForwardSlashFlip;
    m_manipulation_table[NorthFront][SouthBack ]=HorizontalFlip;
    m_manipulation_table[NorthFront][WestBack  ]=BackwardSlashFlip;

    m_manipulation_table[EastFront ][NorthFront]=RightRotate270Degrees;
    m_manipulation_table[EastFront ][EastFront ]=RightRotate0Degrees;
    m_manipulation_table[EastFront ][SouthFront]=RightRotate90Degrees;
    m_manipulation_table[EastFront ][WestFront ]=RightRotate180Degrees;
    m_manipulation_table[EastFront ][NorthBack ]=ForwardSlashFlip;
    m_manipulation_table[EastFront ][EastBack  ]=HorizontalFlip;
    m_manipulation_table[EastFront ][SouthBack ]=BackwardSlashFlip;
    m_manipulation_table[EastFront ][WestBack  ]=VerticalFlip;

    m_manipulation_table[SouthFront][NorthFront]=RightRotate180Degrees;
    m_manipulation_table[SouthFront][EastFront ]=RightRotate270Degrees;
    m_manipulation_table[SouthFront][SouthFront]=RightRotate0Degrees;
    m_manipulation_table[SouthFront][WestFront ]=RightRotate90Degrees;
    m_manipulation_table[SouthFront][NorthBack ]=HorizontalFlip;
    m_manipulation_table[SouthFront][EastBack  ]=BackwardSlashFlip;
    m_manipulation_table[SouthFront][SouthBack ]=VerticalFlip;
    m_manipulation_table[SouthFront][WestBack  ]=ForwardSlashFlip;

    m_manipulation_table[WestFront ][NorthFront]=RightRotate90Degrees;
    m_manipulation_table[WestFront ][EastFront ]=RightRotate180Degrees;
    m_manipulation_table[WestFront ][SouthFront]=RightRotate270Degrees;
    m_manipulation_table[WestFront ][WestFront ]=RightRotate0Degrees;
    m_manipulation_table[WestFront ][NorthBack ]=BackwardSlashFlip;
    m_manipulation_table[WestFront ][EastBack  ]=VerticalFlip;
    m_manipulation_table[WestFront ][SouthBack ]=ForwardSlashFlip;
    m_manipulation_table[WestFront ][WestBack  ]=HorizontalFlip;

    m_manipulation_table[NorthBack ][NorthFront]=VerticalFlip;
    m_manipulation_table[NorthBack ][EastFront ]=ForwardSlashFlip;
    m_manipulation_table[NorthBack ][SouthFront]=HorizontalFlip;
    m_manipulation_table[NorthBack ][WestFront ]=BackwardSlashFlip;
    m_manipulation_table[NorthBack ][NorthBack ]=RightRotate0Degrees;
    m_manipulation_table[NorthBack ][EastBack  ]=RightRotate90Degrees;
    m_manipulation_table[NorthBack ][SouthBack ]=RightRotate180Degrees;
    m_manipulation_table[NorthBack ][WestBack  ]=RightRotate270Degrees;

    m_manipulation_table[EastBack  ][NorthFront]=ForwardSlashFlip;
    m_manipulation_table[EastBack  ][EastFront ]=HorizontalFlip;
    m_manipulation_table[EastBack  ][SouthFront]=BackwardSlashFlip;
    m_manipulation_table[EastBack  ][WestFront ]=VerticalFlip;
    m_manipulation_table[EastBack  ][NorthBack ]=RightRotate270Degrees;
    m_manipulation_table[EastBack  ][EastBack  ]=RightRotate0Degrees;
    m_manipulation_table[EastBack  ][SouthBack ]=RightRotate90Degrees;
    m_manipulation_table[EastBack  ][WestBack  ]=RightRotate180Degrees;

    m_manipulation_table[SouthBack ][NorthFront]=HorizontalFlip;
    m_manipulation_table[SouthBack ][EastFront ]=BackwardSlashFlip;
    m_manipulation_table[SouthBack ][SouthFront]=VerticalFlip;
    m_manipulation_table[SouthBack ][WestFront ]=ForwardSlashFlip;
    m_manipulation_table[SouthBack ][NorthBack ]=RightRotate180Degrees;
    m_manipulation_table[SouthBack ][EastBack  ]=RightRotate270Degrees;
    m_manipulation_table[SouthBack ][SouthBack ]=RightRotate0Degrees;
    m_manipulation_table[SouthBack ][WestBack  ]=RightRotate90Degrees;

    m_manipulation_table[WestBack  ][NorthFront]=BackwardSlashFlip;
    m_manipulation_table[WestBack  ][EastFront ]=VerticalFlip;
    m_manipulation_table[WestBack  ][SouthFront]=ForwardSlashFlip;
    m_manipulation_table[WestBack  ][WestFront ]=HorizontalFlip;
    m_manipulation_table[WestBack  ][NorthBack ]=RightRotate90Degrees;
    m_manipulation_table[WestBack  ][EastBack  ]=RightRotate180Degrees;
    m_manipulation_table[WestBack  ][SouthBack ]=RightRotate270Degrees;
    m_manipulation_table[WestBack  ][WestBack  ]=RightRotate0Degrees;
    cout << "exiting build manipulation_table" << endl;

}
