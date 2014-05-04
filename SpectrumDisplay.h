#include "SmartMatrix.h"

// System includes
#include "ArraySharedPtr.h"


#include "RemapBase.h"
#include "DisplayBase.h"

class SpectrumDisplay
{
 public:
 
  //template<typename DerivcedType>
  void registerRemappers( ArraySharedPtr<RemapBase> Remappers)
  {
    // void deepCopy( const ArraySharedPtr<BDT> &to, const ArraySharedPtr<BDT> &from)
    m_remappers = Remappers;
  }
  
  void registerDisplayers( ArraySharedPtr<DisplayBase> Displayers)
  {
    // void deepCopy( const ArraySharedPtr<BDT> &to, const ArraySharedPtr<BDT> &from)
    m_displayers = Displayers;
  }
  
  void display(DataBuffer &data, SmartMatrix *matrix);
  
 private:
  // I know this is weird but it should look clean in the end
  ArraySharedPtr< RemapBase > m_remappers;
  ArraySharedPtr<DisplayBase> m_displayers;
};
