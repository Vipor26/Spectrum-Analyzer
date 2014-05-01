/*
#include "CircularBufferCT.hpp"

class DecayFunciton
{
	virtual void apply(uint16_t&,int16_t position) = 0;
};

class GravityDecay : public DecayFunciton
{
	void apply(uint16_t &data, int16_t time)
	{
		data = 0.5*Gravity*time*time;
	}
};


template<unsigned int bufferSize>
class PersistantDisplay
{
public
	void setFadeFunction();
	void ProcessData(uint16_t *input);

private:
	CircularBuffer<int16_t,bufferSize> buffers[128];
	DecayFunction *decayFunction;
};

template<unsigned int BufferSize>
PersistantDisplay::ProcessData(uint16_t *input)
{
	unsigned int index, bufIndex, bufSize;
	for(index=0; index<128; +index)
	{
		bufSize = buffers[index].size();
		for(bufIndex=0; bufIndex<bufSize; ++bufIndex)
		{
			decayFunction(buffers[index][bufIndex],bufIndex);
		}
		
	}
}*/