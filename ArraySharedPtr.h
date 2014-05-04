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
    if(data != 0)
    {
      if( size > m_maxSize )
      {
        clear();
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
      data = new std::shared_ptr<DT>[size];
      if(data == NULL)
      {
        return false;
      }
      m_maxSize = size;   
    }
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
    initalize(rhs.m_maxSize); //initlaize will handle if clear needs to be called
    
    for(uint16_t index=0; index<rhs.m_size; ++index)
    {
        data[index] = rhs.data[index];
    }
    
    m_size = rhs.m_size;
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
  
  // get a read only refrence to the data in shared_ptr
  const DT& operator[](uint16_t index) const
  {
    return *(data[index]);
  }
  
 private:
  std::shared_ptr<DT> *data;
  uint16_t m_size, m_maxSize;
};

//This function deep copies, the derived classes into the base class.
template<typename BDT, typename DDT>
void deepCopy( const ArraySharedPtr<BDT> &to, const ArraySharedPtr<BDT> &from)
{
  to.initalize(from.maxSize());
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
