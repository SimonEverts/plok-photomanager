#include "lut.h"

#include <QAtomicInt>
#include <QDebug>

Lut::Lut (void) :
    m_red (NULL),
    m_green (NULL),
    m_blue (NULL),
    m_ref (NULL)
{

}

Lut::Lut (int depth) :
    m_depth (depth)
{
    int size = 1 << depth;

    m_red = new int [size];
    m_green = new int [size];
    m_blue = new int [size];

    memset (m_red, 0, sizeof (int) * size);
    memset (m_green, 0, sizeof (int) * size);
    memset (m_blue, 0, sizeof (int) * size);

    m_ref = new QAtomicInt();
    m_ref->ref();
}

Lut::Lut (const Lut& lut)
{
    m_depth = lut.m_depth;

    m_red = lut.m_red;
    m_green = lut.m_green;
    m_blue = lut.m_blue;

    m_ref = lut.m_ref;
    m_ref->ref();
}

Lut::~Lut (void)
{
    if (!m_ref)
        return;

    if (!m_ref->deref())
    {
        if (m_red)
            delete[] m_red;
        m_red = NULL;

        if (m_green)
            delete[] m_green;
        m_green = NULL;

        if (m_blue)
            delete[] m_blue;
        m_blue = NULL;

        delete m_ref;
        m_ref = NULL;
    }
}

Lut& Lut::operator= (const Lut& lut)
{
    if (&lut != this && m_ref)
    {
        if (!m_ref->deref())
        {
            if (m_red)
                delete[] m_red;
            m_red = NULL;

            if (m_green)
                delete[] m_green;
            m_green = NULL;

            if (m_blue)
                delete[] m_blue;
            m_blue = NULL;

            delete m_ref;
            m_ref = NULL;
        }
    }

    if (lut.m_ref)
    {
        m_depth = lut.m_depth;

        m_red = lut.m_red;
        m_green = lut.m_green;
        m_blue = lut.m_blue;

        m_ref = lut.m_ref;
        m_ref->ref();
    }

    return *this;
}
