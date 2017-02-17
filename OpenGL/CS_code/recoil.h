
#ifndef RECOIL_H
#define RECOIL_H

float GetVecSpread(float *velocity);
void GetSpreadXY(unsigned int seed, int future, float *velocity, float *vec);
void GetRecoilOffset(unsigned int seed, int future, float *inangles, float *velocity, float *outangles);

#endif
