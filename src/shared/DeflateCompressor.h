
#ifndef DEFLATE_COMPRESSOR_H
#define DEFLATE_COMPRESSOR_H

#include "ICompressor.h"


// implements a raw deflate stream, not zlib wrapped, and not checksummed.
class DeflateCompressor : public ICompressor
{
public:
    DeflateCompressor();
    virtual ~DeflateCompressor();
    virtual void Compress(uint32 level = 1, ProgressCallback pcb = NULL);
    virtual void Decompress(void);

protected:
    int _windowBits; // read zlib docs to know what this means
    bool _forceCompress;

private:
    static void decompress(void *dst, uint32 *origsize, const void *src, uint32 size, int wbits);
    static void compress(void* dst, uint32 *dst_size, const void* src, uint32 src_size,
        uint8 level, int wbits, ProgressCallback pcb);
};

// the output produced by this stream contains a minimal gzip header,
// and can be directly written to a .gz file.
class GzipCompressor : public DeflateCompressor
{
public:
    GzipCompressor();
};

#endif
