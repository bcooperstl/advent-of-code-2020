#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <map>

#include "aoc_day_17.h"
#include "file_utils.h"

//#define DEBUG_DAY17

using namespace std;

Space::Space()
{
    m_cells = NULL;
    m_x_size = 0;
    m_y_size = 0;
    m_z_size = 0;
}

Space::~Space()
{
    if (m_cells)
    {
        deallocate_array(m_cells, m_x_size, m_y_size, m_z_size);
    }
    m_cells = NULL;
}

int Space::get_x_size()
{
    return m_x_size;
}

int Space::get_y_size()
{
    return m_y_size;
}

int Space::get_z_size()
{
    return m_z_size;
}


Cell *** Space::allocate_array(int x_size, int y_size, int z_size)
{
    // Reminder - The arrays are accessed in [z][y][x] order.
    Cell *** ret = new Cell ** [z_size];
    for (int i=0; i<z_size; i++)
    {
        ret[i] = new Cell * [y_size];
        for (int j=0; j<y_size; j++)
        {
            ret[i][j] = new Cell[x_size];
            for (int k=0; k<x_size; k++)
            {
                // Initial clear state: inactive and no active neighbors for both current and previous cycles
                ret[i][j][k].curr_state=STATE_INACTIVE;
                ret[i][j][k].prev_state=STATE_INACTIVE;
                ret[i][j][k].curr_num_neighbors_active=0;
                ret[i][j][k].prev_num_neighbors_active=0;
                ret[i][j][k].next_state=STATE_INACTIVE;
            }
        }
    }
    return ret;
}

void Space::deallocate_array(Cell *** array, int x_size, int y_size, int z_size)
{
    for (int i=0; i<z_size; i++)
    {
        for (int j=0; j<y_size; j++)
        {
            delete [] array[i][j];
        }
        delete[] array[i];
    }
    delete [] array;
}

/* 
* resize - this function will resize the space by adding to any/all of the x, y, and z dimensions

Intial plan:
* Determine the new size. Each dimension will either stay the same size if the grow_n value is false or increase by 2 if the grow_n value is true.
* Allocate the new member. Initialize each struct to have both states as inactive and both counts as 0.
* Loop over the existing 3-D array with z, y, and x to copy the values such that
    * if grow_n is true, then new_n=old_n+1
    * if grow_n is false, then new_n=old_n.
* Pointer swap the arrays and delete the old array.
*/
void Space::resize(bool grow_x, bool grow_y, bool grow_z)
{
#ifdef DEBUG_DAY17
    cout << "Resize with grow_x=" << grow_x << " grow_y=" << grow_y << " grow_z=" << grow_z << endl;
#endif
    if (!(grow_x || grow_y || grow_z))
    {
        // nothing to do. returning with same sized spapce
#ifdef DEBUG_DAY17
    cout << " Nothing to do. Returning" << endl;
#endif
        return;
    }
    int new_x_size = m_x_size + (grow_x ? 2 : 0);
    int new_y_size = m_y_size + (grow_y ? 2 : 0);
    int new_z_size = m_z_size + (grow_z ? 2 : 0);
    
    int x_offset = (grow_x ? 1 : 0);
    int y_offset = (grow_y ? 1 : 0);
    int z_offset = (grow_z ? 1 : 0);
    
#ifdef DEBUG_DAY17
    cout << " X size: old=" << m_x_size << " New=" << new_x_size << " offset=" << x_offset << endl;
    cout << " Y size: old=" << m_y_size << " New=" << new_y_size << " offset=" << y_offset << endl;
    cout << " Z size: old=" << m_z_size << " New=" << new_z_size << " offset=" << z_offset << endl;
#endif

    Cell *** new_cells = allocate_array(new_x_size, new_y_size, new_z_size);
    for (int z=0; z<m_z_size; z++)
    {
        for (int y=0; y<m_y_size; y++)
        {
            for (int x=0; x<m_x_size; x++)
            {
                new_cells[z+z_offset][y+y_offset][x+x_offset] = m_cells[z][y][x];
            }
        }
    }
    deallocate_array(m_cells, m_x_size, m_y_size, m_z_size);
    
    m_cells = new_cells;
    m_x_size = new_x_size;
    m_y_size = new_y_size;
    m_z_size = new_z_size;
#ifdef DEBUG_DAY17
    cout << " After resize: " << endl;
    display();
#endif
}

/*
init_from_plane - set up the space based on the input 2-D plane
The space class will be created based on the 2-D plane of sized *m* x *n* passed in.
* Create the initial array to be size x=m+2, y=n+2, z=3
    * both states will be set to inactive and both counts will be set to 0
* Loop over the 2-D plane with in_x and in_y
    * If plane[in_x][in_y] is active
        * call set_cell_active with x=in_x+1, y=in_y+1, z=1 . This will map that cell to the sub-space inside the 1-cell border.
*/
void Space::init_from_plane(vector<string> init_plane)
{
#ifdef DEBUG_DAY17
    cout << "Init From Plane" << endl;
    display();
#endif

    m_x_size=init_plane[0].size()+2;
    m_y_size=init_plane.size()+2;
    m_z_size=3;
    
#ifdef DEBUG_DAY17
    cout << " The " << init_plane[0].size() << " x " <<  init_plane.size() << " plane results in a " 
         << m_x_size << " x " << m_y_size << " x " << m_z_size << " 3-D array " << endl;
#endif
    m_cells = allocate_array(m_x_size, m_y_size, m_z_size);
    
    for (int y=0; y<init_plane.size(); y++)
    {
        string row = init_plane[y];
        for (int x=0; x<row.size(); x++)
        {
            if (row[x]==STATE_ACTIVE)
            {
                set_cell_active(x+1, y+1, 1); // map the cell as bordered on all sides by 1 inactive cell
#ifdef DEBUG_DAY17
                cout << " Active cell at " << x << "," << y << " results in an active cell in " << x+1 << "," << y+1 << ",1" << endl;
#endif
            }
        }
    }
}

/*
set_cell_active - set a cell active, and have the side effect of incrementing all 26 of the neighbors num_neighbors_active counts
*/

void Space::set_cell_active(int x, int y, int z)
{
    // First mark the cell as active. Remember array is accessed by z,y,z
    m_cells[z][y][x].curr_state=STATE_ACTIVE;
    
    // Now need to increment the active neighbor count on all 26 neighbors.
    
    // All 9 in the above plane and all 9 in the below plane
    for (int i=y-1; i<=y+1; i++)
    {
        for (int j=x-1; j<=x+1; j++)
        {
            m_cells[z-1][i][j].curr_num_neighbors_active++;
            m_cells[z+1][i][j].curr_num_neighbors_active++;
        }
    }
    
    // All 3 in the same plane, above row and all 3 in the same plane, below row
    for (int i=x-1; i<=x+1; i++)
    {
        m_cells[z][y-1][i].curr_num_neighbors_active++;
        m_cells[z][y+1][i].curr_num_neighbors_active++;
    }
    
    // Same plane, same row, left and right of the current cell
    m_cells[z][y][x-1].curr_num_neighbors_active++;
    m_cells[z][y][x+1].curr_num_neighbors_active++;
}

/* run_cycle - this function will run through a cycle of the game

Initial plan:
Run Cycle will do the following operations:
* Initialize all the cells:
    * copy every cell's current_state to its prev_state and its current_num_neighbors_active to its prev_num_neighbors_active. 
    * Set all current_num_neighbors_active to 0 and current_state to inactive.
* loop over every non-border cell
    * If the cell should be active based on the rules above and the prev_state and prev_num_neighbors_active
        * Call the set_cell_active function on that cell. This will flip the current_state value and adjust the 26 neighbor's current_num_neighbors_active values
* initialize grow_x, grow_y, and grow_z bools to false
* loop over every border cell
    * If a border cell is active, set the appropriate grow_n variable(s) to true.
* if any grow_n variable is true
    * call resize, specifying which directions to grow.

The following logic rules apply:
* If a cube is active (`#`) and 2 or 3 of its neighbors are active, that cube remaind active. Else, it flips to inactive (`.`)
* If a cube is inactive (`.`) and 3 of its neighbors are active, that cube flips to active (`#`). Else is stays inactive.

Realized I had a logic flaw while programming. 
I need to add a next_state variable to see which cells I need to activate, so I can do a resize before making them active, since setting them active will increment the neighbor counts.


*/

void Space::run_cycle()
{
#ifdef DEBUG_DAY17
    cout << "Run Cycle:" << endl;
#endif
    // Init the cells
    for (int z=0; z<m_z_size; z++)
    {
        for (int y=0; y<m_y_size; y++)
        {
            for (int x=0; x<m_x_size; x++)
            {
                m_cells[z][y][x].prev_state = m_cells[z][y][x].curr_state;
                m_cells[z][y][x].prev_num_neighbors_active = m_cells[z][y][x].curr_num_neighbors_active;
                m_cells[z][y][x].curr_state = STATE_INACTIVE;
                m_cells[z][y][x].curr_num_neighbors_active = 0;
                m_cells[z][y][x].next_state = STATE_INACTIVE;
            }
        }
    }
    
    // Looping over all cells to run the rules. Can detect if we need to grow while doing this
    bool grow_x=false;
    bool grow_y=false;
    bool grow_z=false;
    
    for (int z=0; z<m_z_size; z++)
    {
        for (int y=0; y<m_y_size; y++)
        {
            for (int x=0; x<m_x_size; x++)
            {
                // First rule - cell was active, 2 or 3 neighbors active - cell stays active
                if ((m_cells[z][y][x].prev_state==STATE_ACTIVE) && 
                    ((m_cells[z][y][x].prev_num_neighbors_active==2) || m_cells[z][y][x].prev_num_neighbors_active==3))
                {
                    m_cells[z][y][x].next_state=STATE_ACTIVE;
#ifdef DEBUG_DAY17
                    cout << " Rule 1 (Active, 2 or 3 active neighbors) results in active cell at " << x << "," << y << "," << z << endl;
#endif
                }
                // Second rule - cell was inactive and 3 neighbors are active - cell flips to active
                else if ((m_cells[z][y][x].prev_state==STATE_INACTIVE) && (m_cells[z][y][x].prev_num_neighbors_active==3))
                {
                    m_cells[z][y][x].next_state=STATE_ACTIVE;
#ifdef DEBUG_DAY17
                    cout << " Rule 2 (Inactive, 3 active neighbors) results in active cell at " << x << "," << y << "," << z << endl;
#endif
                }
                // Border checks
                if (m_cells[z][y][x].next_state==STATE_ACTIVE)
                {
                    if ( (!grow_x) && ((x==0)||(x==(m_x_size-1))) )
                    {
#ifdef DEBUG_DAY17
                        cout << "  Cell at " << x << "," << y << "," << z << " results in the X-border needing to grow. Setting grow_x to true" << endl;
#endif
                        grow_x=true;
                    }
                    if ( (!grow_y) && ((y==0)||(y==(m_y_size-1))) )
                    {
#ifdef DEBUG_DAY17
                        cout << "  Cell at " << x << "," << y << "," << z << " results in the Y-border needing to grow. Setting grow_y to true" << endl;
#endif
                        grow_y=true;
                    }
                    if ( (!grow_z) && ((z==0)||(z==(m_z_size-1))) )
                    {
#ifdef DEBUG_DAY17
                        cout << "  Cell at " << x << "," << y << "," << z << " results in the Z-border needing to grow. Setting grow_z to true" << endl;
#endif
                        grow_z=true;
                    }
                }
            }
        }
    }
    
    // Time to check if we need to grow
    if (grow_x || grow_y || grow_z)
    {
#ifdef DEBUG_DAY17
        cout << " Resizing the Cells " << endl;
#endif
        resize(grow_x, grow_y, grow_z);
    }

    // We are now assured that all of the border cells are inactive. Can activate all of the cells with next_state set to Active
    for (int z=1; z<(m_z_size-1); z++)
    {
        for (int y=1; y<(m_y_size-1); y++)
        {
            for (int x=1; x<(m_x_size-1); x++)
            {
                if (m_cells[z][y][x].next_state==STATE_ACTIVE)
                {
#ifdef DEBUG_DAY17
                    cout << "  Activating cell at " << x << "," << y << "," << z << endl;
#endif
                    set_cell_active(x, y, z);
                }
            }
        }
    }
    return;
}

int Space::count_active()
{
    int count=0;
    for (int z=0; z<m_z_size; z++)
    {
        for (int y=0; y<m_y_size; y++)
        {
            for (int x=0; x<m_x_size; x++)
            {
                if (m_cells[z][y][x].curr_state==STATE_ACTIVE)
                {
                    count++;
                }
            }
        }
    }
    return count;
}

void Space::display()
{
    cout << "CURRENT         PREVIOUS           NEXT" << endl;
    for (int z=0; z<m_z_size; z++)
    {
        cout << "Z = " << z << ":" << endl;
        for (int y=0; y<m_y_size; y++)
        {
            for (int x=0; x<m_x_size; x++)
            {
                cout << m_cells[z][y][x].curr_state;
            }
            cout << "            ";
            for (int x=0; x<m_x_size; x++)
            {
                cout << m_cells[z][y][x].prev_state;
            }
            cout << "            ";
            for (int x=0; x<m_x_size; x++)
            {
                cout << m_cells[z][y][x].next_state;
            }
            
            cout << endl;
        }
        cout << endl;
    }
}

AocDay17::AocDay17():AocDay(17)
{
}

AocDay17::~AocDay17()
{
}

vector<string> AocDay17::read_input(string filename)
{
    FileUtils fileutils;
    vector<string> lines;
    if (!fileutils.read_as_list_of_strings(filename, lines))
    {
        cerr << "Error reading in the data from " << filename << endl;
    }
    return lines;
}

string AocDay17::part1(string filename, vector<string> extra_args)
{
    vector<string> init_plane = read_input(filename);
    
    Space space;
    space.init_from_plane(init_plane);
    
    cout << "The initial grid is:" << endl;
    space.display();

    for (int i=1; i<=6; i++)
    {
        space.run_cycle();
        cout << "After running cycle " << i << " there are " << space.count_active() << " active cells:" << endl;
        space.display();
    }

    cout << endl;
    
    ostringstream out;
    out << space.count_active();
    return out.str();
}