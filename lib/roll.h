#pragma once

#include <random>

int roll(char n=1, char d=20, char p=0)
{
  std::random_device gen;
  std::uniform_int_distribution<char> die(1,d);

  if(d==0)
    return n+p;
  
  int out=p;
  for(char i=0; i<n; i++)
    out+=die(gen);
  return out;
}
