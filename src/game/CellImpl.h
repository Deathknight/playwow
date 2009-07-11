/*
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef MANGOS_CELLIMPL_H
#define MANGOS_CELLIMPL_H

#include "Cell.h"
#include "Map.h"
#include <cmath>

inline Cell::Cell(CellPair const& p)
{
    data.Part.grid_x = p.x_coord / MAX_NUMBER_OF_CELLS;
    data.Part.grid_y = p.y_coord / MAX_NUMBER_OF_CELLS;
    data.Part.cell_x = p.x_coord % MAX_NUMBER_OF_CELLS;
    data.Part.cell_y = p.y_coord % MAX_NUMBER_OF_CELLS;
    data.Part.nocreate = 0;
    data.Part.reserved = 0;
}

template<class LOCK_TYPE,class T, class CONTAINER>
inline void
Cell::Visit(const CellLock<LOCK_TYPE> &l, TypeContainerVisitor<T, CONTAINER> &visitor, Map &m) const
{
    const CellPair &standing_cell = l.i_cellPair;
    if (standing_cell.x_coord >= TOTAL_NUMBER_OF_CELLS_PER_MAP || standing_cell.y_coord >= TOTAL_NUMBER_OF_CELLS_PER_MAP)
        return;

    uint16 district = (District)this->data.Part.reserved;
    if(district == CENTER_DISTRICT)
    {
        m.Visit(l, visitor);
        return;
    }

    // set up the cell range based on the district
    // the overloaded operators handle range checking
    CellPair begin_cell = standing_cell;
    CellPair end_cell = standing_cell;

    switch( district )
    {
        case ALL_DISTRICT:
        {
            begin_cell << 1; begin_cell -= 1;               // upper left
            end_cell >> 1; end_cell += 1;                   // lower right
            break;
        }
        case UPPER_LEFT_DISTRICT:
        {
            begin_cell << 1; begin_cell -= 1;               // upper left
            break;
        }
        case UPPER_RIGHT_DISTRICT:
        {
            begin_cell -= 1;                                // up
            end_cell >> 1;                                  // right
            break;
        }
        case LOWER_LEFT_DISTRICT:
        {
            begin_cell << 1;                                // left
            end_cell += 1;                                  // down
            break;
        }
        case LOWER_RIGHT_DISTRICT:
        {
            end_cell >> 1; end_cell += 1;                   // lower right
            break;
        }
        case LEFT_DISTRICT:
        {
            begin_cell -= 1;                                // up
            end_cell >> 1; end_cell += 1;                   // lower right
            break;
        }
        case RIGHT_DISTRICT:
        {
            begin_cell << 1; begin_cell -= 1;               // upper left
            end_cell += 1;                                  // down
            break;
        }
        case UPPER_DISTRICT:
        {
            begin_cell << 1; begin_cell -= 1;               // upper left
            end_cell >> 1;                                  // right
            break;
        }
        case LOWER_DISTRICT:
        {
            begin_cell << 1;                                // left
            end_cell >> 1; end_cell += 1;                   // lower right
            break;
        }
        default:
        {
            assert( false );
            break;
        }
    }

    // loop the cell range
    for(uint32 x = begin_cell.x_coord; x <= end_cell.x_coord; x++)
    {
        for(uint32 y = begin_cell.y_coord; y <= end_cell.y_coord; y++)
        {
            CellPair cell_pair(x,y);
            Cell r_zone(cell_pair);
            r_zone.data.Part.nocreate = l->data.Part.nocreate;
            CellLock<LOCK_TYPE> lock(r_zone, cell_pair);
            m.Visit(lock, visitor);
        }
    }
}

inline int CellHelper(const float radius)
{
    if(radius < 1.0f)
        return 0;

    return (int)ceil(radius/SIZE_OF_GRID_CELL);
}

template<class LOCK_TYPE, class T, class CONTAINER>
inline void
Cell::Visit(const CellLock<LOCK_TYPE> &l, TypeContainerVisitor<T, CONTAINER> &visitor, Map &m, const WorldObject &obj, float radius) const
{
    const CellPair &standing_cell = l.i_cellPair;
    if (standing_cell.x_coord >= TOTAL_NUMBER_OF_CELLS_PER_MAP || standing_cell.y_coord >= TOTAL_NUMBER_OF_CELLS_PER_MAP)
       return;

    //no jokes here... Actually placing ASSERT() here was good idea, but
    //we had some problems with DynamicObjects, which pass radius = 0.0f (DB issue?)
    //maybe it is better to just return when radius <= 0.0f?
    if(radius <= 0.0f)
    {
        m.Visit(l, visitor);
        return;
    }

    //we should increase search radius by object's radius, otherwise
    //we could have problems with huge creatures, which won't attack nearest players
    radius += obj.GetObjectSize();
    //lets calculate object coord offsets from cell borders.
    //TODO: add more correct/generic method for this task
    const float x_offset = (obj.GetPositionX() - CENTER_GRID_CELL_OFFSET)/SIZE_OF_GRID_CELL;
    const float y_offset = (obj.GetPositionY() - CENTER_GRID_CELL_OFFSET)/SIZE_OF_GRID_CELL;

    const float x_val = floor(x_offset + CENTER_GRID_CELL_ID + 0.5f);
    const float y_val = floor(y_offset + CENTER_GRID_CELL_ID + 0.5f);
    const float x_off = (x_offset - x_val + CENTER_GRID_CELL_ID) * SIZE_OF_GRID_CELL;
    const float y_off = (y_offset - y_val + CENTER_GRID_CELL_ID) * SIZE_OF_GRID_CELL;

    int left = 0, right = 0, upper = 0, lower = 0;

    const float tmp_diff = radius - CENTER_GRID_CELL_OFFSET;
    //lets calculate upper/lower/right/left corners for cell search
    right = CellHelper(tmp_diff + x_off);
    left = CellHelper(tmp_diff - x_off);
    upper = CellHelper(tmp_diff + y_off);
    lower = CellHelper(tmp_diff - y_off);

    //if radius fits inside standing cell
    if(!left && !right && !upper && !lower)
    {
        m.Visit(l, visitor);
        return;
    }

    CellPair begin_cell = standing_cell;
    CellPair end_cell = standing_cell;

    begin_cell << left;
    begin_cell -= lower;
    end_cell >> right;
    end_cell += upper;

    //ALWAYS visit standing cell first!!!
    m.Visit(l, visitor);

    // loop the cell range
    for(uint32 x = begin_cell.x_coord; x <= end_cell.x_coord; x++)
    {
        for(uint32 y = begin_cell.y_coord; y <= end_cell.y_coord; y++)
        {
            CellPair cell_pair(x,y);
            //lets skip standing cell since we already visited it
            if(cell_pair != standing_cell)
            {
                Cell r_zone(cell_pair);
                r_zone.data.Part.nocreate = l->data.Part.nocreate;
                CellLock<LOCK_TYPE> lock(r_zone, cell_pair);
                m.Visit(lock, visitor);
            }
        }
    }
}

#endif
