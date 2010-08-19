#ifndef VFSFILE_LVPA_H
#define VFSFILE_LVPA_H

#include "VFSFile.h"

class LVPAFile;

class VFSFileLVPA : public VFSFile
{
public:
    VFSFileLVPA(LVPAFile *src, uint32 headerId);
    virtual ~VFSFileLVPA();
    virtual bool open(const char *fn = NULL, char *mode = NULL);
    virtual bool isopen(void);
    virtual bool iseof(void);
    virtual const char *name(void);
    virtual bool close(void);
    virtual bool seek(uint64 pos);
    virtual bool flush(void);
    virtual uint64 getpos(void);
    virtual uint32 read(char *dst, uint32 bytes);
    virtual uint32 write(char *src, uint32 bytes);
    virtual uint64 size(void);
    virtual const uint8 *getBuf(void);
    virtual const char *getSource(void) { return "LVPA"; }

protected:
    uint32 _pos;
    uint32 _size;
    uint32 _headerId;
    std::string _name;
    LVPAFile *_lvpa;
};

#endif
