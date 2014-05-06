#include "SmartMatrix.h"

// System includes
#include "ArraySharedPtr.h"


#include "RemapBase.h"
#include "DisplayBase.h"

class SpectrumDisplay
{
 public:
 
  //template<typename DerivcedType>
  void registerRemappers( const ArraySharedPtr<RemapBase> &Remappers)
  {
    // void deepCopy( const ArraySharedPtr<BDT> &to, const ArraySharedPtr<BDT> &from)
    m_remappers = Remappers;
  }
  
  void registerDisplayers( const ArraySharedPtr<DisplayBase> &Displayers)
  {
    // void deepCopy( const ArraySharedPtr<BDT> &to, const ArraySharedPtr<BDT> &from)
    m_displayers = Displayers;
  }
  
  void display(DataBuffer &data, SmartMatrix *matrix);
  
 private:
  ArraySharedPtr< RemapBase > m_remappers;
  ArraySharedPtr<DisplayBase> m_displayers;
};
