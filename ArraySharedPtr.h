#ifndef ARRAY_SHARRED_PTR
#define ARRAY_SHARRED_PTR

#include <new>
#include <memory>


template<typename DT>
class ArraySharedPtr
{
 public:
  ArraySharedPtr() : data(NULL), m_size(0), m_maxSize(0)
  {  }
  
  ArraySharedPtr(const ArraySharedPtr &rhs)
  {
    copy(rhs);
  }
  
  ~ArraySharedPtr()
  {
    clear();
  }
  
  bool initalize(size_t size)
  {
    if(data != NULL)
    {
      // If there needs more size
      if( size > m_maxSize )
      {
        delete[] data;
        m_maxSize = 0;
        m_size = 0;
        data = new std::shared_ptr<DT>[size]; //std::nothrow?
        if(data == NULL)
        {
          return false;
        }
        m_maxSize = size;
      }
    }
    else
    {
      // Initalize to size
      data = new std::shared_ptr<DT>[size];
      if(data == NULL)
      {
        return false;
      }
      m_maxSize = size;
    }
    
    //Set the current requested size
    m_size = size;
    return true;
  }
  
  void clear()
  {
    if(data!=NULL)
    {
      delete[] data;
    }
    m_maxSize = 0;
    m_size = 0;
    data = NULL;
  }
  
  void reset()
  {
    uint16_t index;
    for(index=0; index<m_size; ++index)
    {
      data[index].reset();
    }
  }
  
  void copy(const ArraySharedPtr &rhs)
  {
    if( !initalize( rhs.m_maxSize ) ) //initlaize will handle if clear needs to be called
    {
      Serial.println("Copy Constructor inistalization failed");
    }
    
    m_size = rhs.m_size;
    for(uint16_t index=0; index<m_size; ++index)
    {
        data[ index ] = rhs.data[ index ];
    }
  }
  
  ArraySharedPtr& operator=(const ArraySharedPtr &rhs)
  {
    if( this == &rhs )  {
      return *this;
    }
    
    copy(rhs);
    
    return *this;
  }
  
  void setSize(uint16_t Size)
  {
    m_size = Size;
  }
  
  uint16_t size() const
  {
    return m_size;
  }
  
  uint16_t maxSize() const
  {
    return m_maxSize;
  }
  
  // get the shared ptr
  std::shared_ptr<DT>& operator[](uint16_t index)
  {
    return data[index];
  }
  
 private:
  std::shared_ptr<DT> *data;
  uint16_t m_size, m_maxSize;
};

//This function deep copies, the derived classes into the base class.
template<typename BDT, typename DDT>
void deepCopy( const ArraySharedPtr<BDT> &to, const ArraySharedPtr<BDT> &from)
{
  if( to.initalize(from.maxSize()) )  {
    Serial.println("Deep Copy initalization failed");
  }
  uint16_t index, size;
  std::shared_ptr<BDT> fromData;
  std::shared_ptr<DDT> toData;
  
  size = from.size();
  for(index=0; index<size; ++index)
  {
    to.reset( new DDT( *(from[index]) ) );
  }
}

#endif // ARRAY_SHARRED_PTR
