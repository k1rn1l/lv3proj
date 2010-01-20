#ifndef _TOOLS_H
#define _TOOLS_H

#include "common.h"

void mtRandSeed(uint32 seed);
int32 irand(int32 min, int32 max);
uint32 urand(uint32 min, uint32 max);
int32 rand32(void);
double rand_norm(void);
double rand_chance(void);
void printchex(std::string,bool);
void printchex(char *in, uint32 len, bool);
std::string stringToUpper(std::string);
std::string stringToLower(std::string);
uint64 toInt(std::string);
std::string toHexDump(uint8* array,uint32 size,bool spaces=true,uint32 per_line=0);
std::deque<std::string> GetFileList(std::string);
bool FileExists(std::string);
bool CreateDir(const char*);
uint32 getMSTime(void);
uint32 getMSTimeDiff(uint32, uint32);
uint32 GetFileSize(const char*);
void _FixFileName(std::string&);
std::string _PathToFileName(std::string);
std::string NormalizeFilename(std::string);
uint32 HexStrToInt(const char*);
uint32 fastintpow(uint32 base, uint32 power);
uint64 fastintpow64(uint64 base, uint32 power);
std::string GetTimeString(void);
std::string GetDateString(void);

template <class T> void StrSplit(const std::string &src, const std::string &sep, T& container)
{
    std::string s;
    for (std::string::const_iterator i = src.begin(); i != src.end(); i++)
    {
        if (sep.find(*i) != std::string::npos)
        {
            if (s.length()) container.push_back(s);
            s = "";
        }
        else
        {
            s += *i;
        }
    }
    if (s.length()) container.push_back(s);
}

#endif
