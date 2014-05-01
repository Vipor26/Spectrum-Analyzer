/*
 * Circular Buffer - memory reserved at Compile time.
 *
 * Copyright (c) 2014 Brian Hamilton
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

template <typename DT, unsigned int BUFFER_SIZE>
class CircularBuffer
{
  public:
	CircularBuffer();
	CircularBuffer(const CircularBuffer &rhs); // only operation that is not const time
	~CircularBuffer();
	
	void push(const DT &data);
	void pop(const DT *data);
	
	void clear();
	void dropLast(unsigned int elements);
	
	unsigned int size() const;
	unsigned int maxSize() const;
	bool isFull() const;
	bool isEmpty() const;
	

	DT operator[](unsigned int idx) const; //get
 	DT& operator[](unsigned int idx); //set
	
  protected:
	unsigned int m_front, m_size;
	DT m_array[ BUFFER_SIZE ];
};

template<typename DT, unsigned int BUFFER_SIZE>
CircularBuffer<DT,BUFFER_SIZE>::CircularBuffer() : m_front(0), m_size( 0)
{	}

template<typename DT, unsigned int BUFFER_SIZE>
CircularBuffer<DT,BUFFER_SIZE>::CircularBuffer(const CircularBuffer &rhs) :
	m_front(0),
	m_size( rhs.m_size )
{
	for(unsigned int index=0; index<m_size; ++index)	{
		m_array[index] = rhs.m_array[ (rhs.m_front + index)%BUFFER_SIZE ];
	}
}

template<typename DT, unsigned int BUFFER_SIZE>
CircularBuffer<DT,BUFFER_SIZE>::~CircularBuffer()
{	}


template<typename DT, unsigned int BUFFER_SIZE>
void CircularBuffer<DT,BUFFER_SIZE>::push(const DT &data)
{
    unsigned int back = (m_front + m_size) % BUFFER_SIZE;
    m_array[back] = data;
    if (m_size == BUFFER_SIZE)	{
    	// if full then drop entries, should have check if this accrued?
        m_front = (m_front + 1) % BUFFER_SIZE;
    }
    else	{
        ++ m_size;
    }
}

template<typename DT, unsigned int BUFFER_SIZE>
void CircularBuffer<DT,BUFFER_SIZE>::pop(const DT *data)
{	
    *data = m_array[m_front];
    m_front = (m_front + 1) % BUFFER_SIZE;
    -- m_size;
}

template<typename DT, unsigned int BUFFER_SIZE>
DT CircularBuffer<DT,BUFFER_SIZE>::operator[](unsigned int idx) const
{
	return m_array[ (m_front + idx)%BUFFER_SIZE ];
}

template<typename DT, unsigned int BUFFER_SIZE>
DT& CircularBuffer<DT,BUFFER_SIZE>::operator[](unsigned int idx)
{
	return m_array[ (m_front + idx)%BUFFER_SIZE ];
}
 
template<typename DT, unsigned int BUFFER_SIZE>
void CircularBuffer<DT,BUFFER_SIZE>::clear()
{
	m_size = 0;
}

template<typename DT, unsigned int BUFFER_SIZE>
void CircularBuffer<DT,BUFFER_SIZE>::dropLast(unsigned int elements)
{
	if( elements > m_size )	{
		return;
	}
	m_size -= elements;
}

template<typename DT, unsigned int BUFFER_SIZE>
unsigned int CircularBuffer<DT,BUFFER_SIZE>::size() const
{
	return m_size;
}

template<typename DT, unsigned int BUFFER_SIZE>
unsigned int CircularBuffer<DT,BUFFER_SIZE>::maxSize() const
{
	return BUFFER_SIZE;
}

template<typename DT, unsigned int BUFFER_SIZE>
bool CircularBuffer<DT,BUFFER_SIZE>::isFull() const
{
	return ( m_size == BUFFER_SIZE );
}

template<typename DT, unsigned int BUFFER_SIZE>
bool CircularBuffer<DT,BUFFER_SIZE>::isEmpty() const
{
	return (m_size == 0);
}
