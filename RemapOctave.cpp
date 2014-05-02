#include "RemapOctave.h"

// Octaves are defined in a doubling of frequency each column, this would however leave
// holes in the display, so the following was the approximations

// Multiplier 1.235 R^2 error = 0.98674 close enough to doubling?
// To  From L  From H  Diff             min(hz)  max(hz)
//0	1	1	1		171.875	343.75	HZ
//1	2	2	1		343.75	515.625	HZ
//2	3	3	1		515.625	687.5	HZ
//3	4	5	2		687.5	1031.25	HZ
//4	6	7	2		1031.25	1375	HZ
//5	8	10	3		1375	1890.625	HZ
//6	11	14	4		1890.625	2578.125	HZ
//7	15	19	5		2578.125	3437.5	HZ
//8	20	25	6		3437.5	4468.75	HZ
//9	26	32	7		4468.75	5671.875	HZ
//10	33	41	9		5671.875	7218.75	HZ
//11	42	52	11		7218.75	9109.375	HZ
//12	53	65	13		9109.375	11343.75	HZ
//13	66	82	17		11343.75	14265.625	HZ
//14	83	103	21		14265.625	17875	HZ
//15	104	127	24		17875	22000	HZ

uint8_t re128to16[32] = {  1,  1,
                           2,  2,
                           3,  3,
                           4,  5,
                           6,  7,
                           8,  10,
                          11,  14,
                          15,  19,
                          20,  25,
                          26,  32,
                          33,  41,
                          42,  52,
                          53,  65,
                          66,  82,
                          83, 103,
                         104, 127 };


// Multiplier 1.0721 R^2 error = 0.97336 close enough to doubling?
// To  From L  From H  Diff             min(hz)  max(hz)
//0	1	1	1		171.875	343.75	HZ
//1	2	2	1		343.75	515.625	HZ
//2	3	3	1		515.625	687.5	HZ
//3	4	5	2		687.5	1031.25	HZ
//4	6	6	1		1031.25	1203.125	HZ
//5	7	8	2		1203.125	1546.875	HZ
//6	9	10	2		1546.875	1890.625	HZ
//7	11	12	2		1890.625	2234.375	HZ
//8	13	14	2		2234.375	2578.125	HZ
//9	15	16	2		2578.125	2921.875	HZ
//10	17	18	2		2921.875	3265.625	HZ
//11	19	20	2		3265.625	3609.375	HZ
//12	21	23	3		3609.375	4125	HZ
//13	24	26	3		4125	4640.625	HZ
//14	27	29	3		4640.625	5156.25	HZ
//15	30	32	3		5156.25	5671.875	HZ
//16	33	35	3		5671.875	6187.5	HZ
//17	36	39	4		6187.5	6875	HZ
//18	40	43	4		6875	7562.5	HZ
//19	44	47	4		7562.5	8250	HZ
//20	48	51	4		8250	8937.5	HZ
//21	52	56	5		8937.5	9796.875	HZ
//22	57	61	5		9796.875	10656.25	HZ
//23	62	66	5		10656.25	11515.625	HZ
//24	67	72	6		11515.625	12546.875	HZ
//25	73	78	6		12546.875	13578.125	HZ
//26	79	85	7		13578.125	14781.25	HZ
//27	86	92	7		14781.25	15984.375	HZ
//28	93	100	8		15984.375	17359.375	HZ
//29	101	108	8		17359.375	18734.375	HZ
//30	109	117	9		18734.375	20281.25	HZ
//31	118	127	10		20281.25	22000	HZ

uint8_t re128to32[64] = {  1,  1,
                           2,  2,
                           3,  3,
                           4,  5,
                           6,  6,
                           7,  8,
                           9,  10,
                          11,  12,
                          13,  14,
                          15,  16,
                          17,  18,
                          19,  20,
                          21,  23,
                          24,  26,
                          27,  29,
                          30,  32,
                          33,  35,
                          36,  39,
                          40,  43,
                          44,  47,
                          48,  51,
                          52,  56,
                          57,  61,
                          62,  66,
                          67,  72,
                          73,  78,
                          79,  85,
                          86,  92,
                          93, 100,
                         101, 108,
                         109, 117,
                         118, 127 };


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
  rgb24 color;
  uint16_t count = (uint16_t)stop + 1 - start;
  uint16_t Rsum = 0, Bsum = 0, Gsum = 0;
  unsigned long sum = 0;
  for(indexFrom=start; indexFrom<=stop; ++indexFrom)
  {
    sum += data.data[indexFrom].Y;
    color = data.data[indexFrom].C;
    Rsum += color.red;
    Gsum += color.green;
    Bsum += color.blue;
  }
  data.data[where].X = where;
  data.data[where].Y = (uint16_t)(sum/count);
  color.red   = (uint8_t)(Rsum/count);
  color.green = (uint8_t)(Gsum/count);
  color.blue  = (uint8_t)(Bsum/count);
}
                
void RemapOctave::useAll(const uint8_t &start,
                      const uint8_t &stop,
                      const uint8_t &where,
                      DataBuffer &data)
{
  uint8_t indexFrom;
  for(indexFrom=start; indexFrom<=stop; ++indexFrom)
  {
    // Just reassign the X axis
    data.data[indexFrom].X = where;
  }
}

void RemapOctave::useMax(const uint8_t &start,
                      const uint8_t &stop,
                      const uint8_t &where,
                      DataBuffer &data)
{
  uint8_t indexFrom;
  uint16_t max, temp;
  rgb24 maxcolor = {0x00,0x00,0x00};
  
  max = 0;
  for(indexFrom=start; indexFrom<=stop; ++indexFrom)
  {
    temp = data.data[indexFrom].Y;
    if(temp > max)  {
      max = temp;
      maxcolor = data.data[indexFrom].C;
    }
  }
  
  data.data[where].X = where;
  data.data[where].Y = max;
  data.data[where].C = maxcolor;
}
