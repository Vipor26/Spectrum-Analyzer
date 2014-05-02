#include "RemapOctave.h"

// Octaves are defined in a doubling of frequency each column, this would however leave
// holes in the display, so the following was the approximations

// Multiplier 1.344 R^2 error = 0.9855 close enough to doubling?
//0	1	1	1		171.875	343.75	HZ
//1	2	2	1		343.75	515.625	HZ
//2	2	3	2		343.75	687.5	HZ
//3	3	4	2		515.625	859.375	HZ
//4	4	5	2		687.5	1031.25	HZ
//5	5	7	3		859.375	1375	HZ
//6	7	9	3		1203.125	1718.75	HZ
//7	9	12	4		1546.875	2234.375	HZ
//8	12	16	5		2062.5	2921.875	HZ
//9	16	22	7		2750	3953.125	HZ
//10	22	30	9		3781.25	5328.125	HZ
//11	30	40	11		5156.25	7046.875	HZ
//12	40	54	15		6875	9453.125	HZ
//13	54	73	20		9281.25	12718.75	HZ
//14	73	98	26		12546.875	17015.625	HZ
//15	98	127	30		16843.75	22000	HZ

uint8_t re128to16[32] = {  1,   1,
                           2,   2,
                           2,   3,
                           3,   4,
                           4,   5,
                           5,   7,
                           7,   9,
                           9,  12,
                          12,  16,
                          16,  22,
                          22,  30,
                          30,  40,
                          40,  54,
                          54,  73,
                          73,  98,
                          98, 127 };


// Multiplier 1.12 R^2 error = 0.97336 close enough to doubling?
// To  From L  From H  Diff
// 0	1	1	1		171.875	343.75	HZ
// 1	2	2	1		343.75	515.625	HZ
// 2	3	3	1		515.625	687.5	HZ
// 3	4	4	1		687.5	859.375	HZ
// 4	5	6	2		859.375	1203.125	HZ
// 5	6	7	2		1031.25	1375	HZ
// 6	7	8	2		1203.125	1546.875	HZ
// 7	8	9	2		1375	1718.75	HZ
// 8	9	10	2		1546.875	1890.625	HZ
// 9	10	11	2		1718.75	2062.5	HZ
// 10	11	12	2		1890.625	2234.375	HZ
// 11	12	13	2		2062.5	2406.25	HZ
// 12	13	15	3		2234.375	2750	HZ
// 13	15	17	3		2578.125	3093.75	HZ
// 14	17	19	3		2921.875	3437.5	HZ
// 15	19	21	3		3265.625	3781.25	HZ
// 16	21	24	4		3609.375	4296.875	HZ
// 17	24	27	4		4125	4812.5	HZ
// 18	27	30	4		4640.625	5328.125	HZ
// 19	30	34	5		5156.25	6015.625	HZ
// 20	34	38	5		5843.75	6703.125	HZ
// 21	38	43	6		6531.25	7562.5	HZ
// 22	43	48	6		7390.625	8421.875	HZ
// 23	48	54	7		8250	9453.125	HZ
// 24	54	60	7		9281.25	10484.375	HZ
// 25	60	67	8		10312.5	11687.5	HZ
// 26	67	75	9		11515.625	13062.5	HZ
// 27	75	83	9		12890.625	14437.5	HZ
// 28	83	93	11		14265.625	16156.25	HZ
// 29	95	105	11		16328.125	18218.75	HZ
// 30	105	116	12		18046.875	20109.375	HZ
// 31	116	127	12		19937.5	22000	HZ

uint8_t re128to32[64] ={  1,  1,
                          2,  2,
                          3,  3,
                          4,  4,
                          5,  6,
                          6,  7,
                          7,  8,
                          8,  9,
                          9,  10,
                         10,  11,
                         11,  12,
                         12,  13,
                         13,  15,
                         15,  17,
                         17,  19,
                         19,  21,
                         21,  24,
                         24,  27,
                         27,  30,
                         30,  34,
                         34,  38,
                         38,  43,
                         43,  48,
                         48,  54,
                         54,  60,
                         60,  67,
                         67,  75,
                         75,  83,
                         83,  93,
                         95,  105,
                        105,  116,
                        116,  127  };


/* old remap method
LogRemap({ 0, 1, 2, 3, 4, 5, 6, 7,
             8, 9,10,11,12,13,14,15,
            15,16,16,17,17,18,18,18,
            19,19,19,20,20,21,21,21,
            22,22,22,22,23,23,23,23,
            23,24,24,24,24,24,24,25,
            25,25,25,25,25,25,26,26,
            26,26,26,26,26,26,27,27,
            27,27,27,27,27,27,27,27,
            27,28,28,28,28,28,28,28,
            28,28,28,28,28,29,29,29,
            29,29,29,29,29,29,29,29,
            29,29,29,29,30,30,30,30,
            30,30,30,30,30,30,30,30,
            30,30,30,30,30,31,31,31,
            31,31,31,31,31,31,31} */

RemapOctave::RemapOctave(uint8_t ReMapTo, OctaveCompression comp): m_dataptr(NULL), m_size(0), compression(comp)
 {
   switch(ReMapTo)
   {
     case 16:
       m_dataptr = re128to16;
       m_size = 32; //start and stop for each index 2*16=32
       break;
     case 32:
       m_dataptr = re128to32;
       m_size = 64; //start and stop for each index 2*32=64
       break;
     default: break;
   }
 }

void RemapOctave::remap(DataBuffer &data)
{
  if( (m_dataptr == NULL) || (m_size == 0) ) return;
  
  uint8_t indexTo, start, stop;
  
  for(indexTo=0; indexTo<m_size; indexTo+=2)
  {
    start = m_dataptr[ indexTo     ];
    stop  = m_dataptr[ indexTo + 1 ];
    
    switch(compression)
    {
      case OctaveAverage: useAverage(start, stop, indexTo, data); break;
      case OctaveAll:         useAll(start, stop, indexTo, data); break;
      case OctaveMax:         useMax(start, stop, indexTo, data); break;
    }
  }
  data.size = m_size;
}

void RemapOctave::useAverage(const uint8_t &start,
                          const uint8_t &stop,
                          const uint8_t &where,
                          DataBuffer &data)
{
  uint8_t indexFrom;
  unsigned long count = (uint16_t)stop + 1 - start;
  unsigned long sum = 0;
  for(indexFrom=start; indexFrom<=stop; ++indexFrom)
  {
    sum += data.data[indexFrom].Y;
  }
  data.data[where].X = where;
  data.data[where].Y = (uint16_t)(sum/count);
}
                
void RemapOctave::useAll(const uint8_t &start,
                      const uint8_t &stop,
                      const uint8_t &where,
                      DataBuffer &data)
{
  uint8_t indexFrom;
  for(indexFrom=start; indexFrom<=stop; ++indexFrom)
  {
    data.data[indexFrom].X = where;
    //data.data[where].Y
  }
}

void RemapOctave::useMax(const uint8_t &start,
                      const uint8_t &stop,
                      const uint8_t &where,
                      DataBuffer &data)
{
  uint8_t indexFrom;
  uint16_t max, temp;
  
  max = 0;
  for(indexFrom=start; indexFrom<=stop; ++indexFrom)
  {
    temp = data.data[indexFrom].Y;
    if(temp > max)  {
      max = temp;
    }
  }
  
  data.data[where].X = where;
  data.data[where].Y = max;
}
