#include "SmartMatrix.h"

#include <vector>
#include <memory>

#include "RemapBase.h"
#include "DisplayBase.h"

class SpectrumDisplay
{
 public:
  void registerRemappers(std::vector<std::shared_ptr<RemapBase>> Remappers)
  { m_remappers = Remappers; }
  
  void registerDisplayers(std::vector<std::shared_ptr<DisplayBase>> Displayers)
  { m_displayers = Displayers; }
  
  
  void display(DataBuffer &data, SmartMatrix *matrix);
  
 private:
  std::vector<std::shared_ptr<RemapBase>> m_remappers;
  std::vector<std::shared_ptr<DisplayBase>> m_displayers;
};
