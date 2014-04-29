class SpectrumDisplayBase
{
 public:
  virtual ~SpectrumDisplayBase() {}
  
  // output is expected to be [128] from the fft
  virtual void display(uint16_t &output, SmartMatrix *matrix) = 0;
  
};