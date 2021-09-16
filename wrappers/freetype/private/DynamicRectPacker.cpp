#include "DynamicRectPacker.hpp"

DynamicRectPacker::DynamicRectPacker(glm::vec2 initialSize)
    : m_initialSize(initialSize)
{
}

int DynamicRectPacker::insert(glm::vec2 size, bool *doubled)
{
    auto method = rbp::MaxRectsBinPack::RectBestShortSideFit;
    rbp::Rect rect {0, 0, 0, 0};
    bool doubledLocal = false;

    if(size.x != 0.0f && size.y != 0.0f)
    {
        // If the size requested is zero, we can always add it

        while(rect.width == 0 && rect.height == 0)
        {
            rect = m_packers[m_currentMag.x][m_currentMag.y].Insert(size.x, size.y, method);

            // Empty rect, no more space
            // This loop should be done either 0 or 1 times

            // Go to next coord
            doubledLocal = goToNextMagnitude();
        }
    }

    // Add an offset based on the magnitude of the packer
    rect.x += m_currentMag.x * m_initialSize.x;
    rect.y += m_currentMag.x * m_initialSize.y;

    Rect rect2;
    rect2.size = {rect.width, rect.height};
    rect2.origin = {rect.x, rect.y};

    int ret = m_nextID;
    m_dictionary[ret] = rect2;
    m_nextID++;

    if(doubled)
    {
        *doubled = doubledLocal;
    }

    return ret;
}

const Rect &DynamicRectPacker::operator[](int id) const
{
    return m_dictionary.at(id);
}

bool DynamicRectPacker::goToNextMagnitude()
{
    bool doubled = false;

    if(m_currentMag.y == m_magnitude - 1 && m_currentMag.x == m_magnitude - 1)
    {
        // Last magnitude, double the texture
        doubleMagnitude();
        doubled = true;
    }

    // After resizing, the magnitude may be already occuped, we need to advance until an empty packer is found
    while(packerExists(m_currentMag))
    {
        // Go to next coord, row-first

        if(m_currentMag.x == m_magnitude - 1)
        {
            // Next row
            m_currentMag.y++;
            m_currentMag.x = 0;
        }
        else
        {
            // Next column
            m_currentMag.x++;
        }
    }

    return doubled;
}

void DynamicRectPacker::doubleMagnitude()
{
    // Go to first free index (valid after doubling)
    m_currentMag.x = m_magnitude;
    m_currentMag.y = 0;

    // Always double to keep a size multiple of 2
    m_magnitude *= 2;
}

rbp::MaxRectsBinPack &DynamicRectPacker::getPacker(glm::ivec2 mag)
{
    if(!packerExists(mag))
    {
        // Do not allow flip to simplify management of TextureRects
        m_packers[mag.x][mag.y].Init(m_initialSize.x, m_initialSize.y, false);
    }

    return m_packers[mag.x][mag.y];
}

bool DynamicRectPacker::packerExists(glm::ivec2 mag) const
{
    return m_packers.contains(mag.x) && m_packers.at(mag.x).contains(mag.y);
}
