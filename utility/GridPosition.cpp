#include "GridPosition.hpp"
#include <glm/glm.hpp>

GridPosition::GridPosition(int columns)
    : m_columns(columns)
{
}

GridPosition& GridPosition::operator++()
{
    if(m_pos.x == m_columns - 1)
    {
        // Last column
        // Go to next row
        
        m_pos.x = 0;
        m_pos.y++;
    }
    else
    {
        // Go to next column
        m_pos.x++;
    }
    
    return *this;
}

int GridPosition::getIndex() const
{
    return m_pos.y * m_columns + m_pos.x;
}

void GridPosition::setPosition(glm::ivec2 newPos)
{
    m_pos.x = glm::clamp(newPos.x, 0, m_columns);
    m_pos.y = glm::max(newPos.y, 0);
}
