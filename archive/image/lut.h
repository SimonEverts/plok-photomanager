#ifndef LUT_H
#define LUT_H

class QAtomicInt;

class Lut
{
public:
    Lut (void);
    Lut (int depth);
    Lut (const Lut& lut);

    virtual ~Lut (void);

    Lut& operator= (const Lut& lut);

    int depth (void) const {return m_depth;}

    int* red (void) const {return m_red;}
    int* green (void) const {return m_green;}
    int* blue (void) const {return m_blue;}
private:
    int m_depth;

    int* m_red;
    int* m_green;
    int* m_blue;

    QAtomicInt* m_ref;
};

#endif // LUT_H
