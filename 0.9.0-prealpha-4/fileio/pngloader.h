/* Copyright (C) 2016, Chesley Kraniak
 *
 * This code is distributed under the terms of the GPLv3 license, which should
 * have been included with this file in the root directory as "LICENSE.txt". If
 * you do not have a copy of the license, see:
 *     http://www.gnu.org/licenses/gpl-3.0.txt
 * or:
 *     https://github.com/CKraniak/OpenRS
 * for the license.
 */
#ifndef PNGLOADER_H
#define PNGLOADER_H

#include "../utility.h"

#include <fstream>
#include <vector>
#include <map>
#include <string>

// Png-style Huffman code

// INFLATE

// A PNG is structured in what the spec calls "chunks", which include 4 main
// pieces of information (in order):
//  - Length of Data (4 bytes)
//  - Chunk type (4 bytes)
//  - Data (length-of-data bytes)
//  - CRC-32 (4 bytes)
//
// The chunk type is an ASCII string. If the first letter is uppercase, then
// it's "critical", and must be handled by the decoder (us), otherwise it's
// permissible to ignore it. I only know of 4 "critical" chunk types:
//  - IHDR
//  - IDAT
//  - PLTE
//  - IEND
// These are the header, data, palette, and end chunks, respectively.
//
// Most of the important information is in the IHDR chunk: image height
// and width, pixel type, whether or not it's in color, etc. The data is
// compressed with a hybrid Huffman-LZ77 that I'm not perfectly clear about
// yet. The end has no data, and should appear the same all the time. The
// palette is only used if the pixel type in IHDR is one of the kinds that
// requires a palette to function.
//
// The CRC ... I'm still figuring out. It is appended to each chunk, so it
// goes in the overarching PngChunk struct.

struct PngChunk {
    // Chunks have a clearly defined chunk type
    typedef enum {
        IHDR,
        IEND,
        IDAT,
        PLTE,
        UNSUPPORTED
    } PNGL_CHUNK_TYPE;
    PNGL_CHUNK_TYPE pngl_type;
    // Chunk data length
    unsigned int length;
    // The chunk type, as a char[5] (e.g. "IHDR\0")
    char type_charstr[5];
    // The data. May be zero-length
    std::vector<unsigned char> data;
    // CRC of the data
    unsigned int crc;
    bool is_critical; // true if the chunk is a "critical" chunk, i.e. the
                      // first letter in its type is uppercase
    bool loaded; // Tells loadChunkFromBuffer / subclasses that this chunk has
                 // already had loadChunkFromBuffer called, and it shouldn't
                 // need to be called again
    int bytes_in_chunk; // length + 12, or the total chunk size in the PNG file,
                        // not just the length of the data

    // Calculate the CRC of the data and check to see if it's equal to
    // the crc passed in.
    static bool checkCrc(std::vector<unsigned char> & data, unsigned int crc);
    // Given a buffer of data and an offset into the buffer, try to read a PNG
    // chunk. Returns # of bytes read (including the 12 from length, type, and
    // CRC), or 0 on error.
    int loadChunkFromBuffer(std::vector<char> & buffer_, int offset);

    void wikipediaTestCrc();
    void iendTestCrc();

    PngChunk() : loaded(false) {}
};

// An IHDR PNG Chunk.
//
// Subclasses PngChunk and adds some specific things regarding the data field
// that are present in an IHDR chunk:
//  - image width
//  - image height
//  - image color type
//  - color depth
//  - compression, filter, and interlace methods
struct IhdrPngChunk : PngChunk {
    // Image dimensions in px, 0 is invalid
    typedef enum {
        IC_INVALID,
        IC_GRAYSCALE,
        IC_COLOR,
        IC_COLORPALETTE,
        IC_GRAYSCALEALPHA,
        IC_COLORALPHA
    } IHDR_COLOR_TYPE;

    typedef enum {
        ICOM_NONE
    } IHDR_COMPRESSION_METHOD;

    typedef enum {
        IFIL_NONE
    } IHDR_FILTER_METHOD;

    typedef enum {
        IIL_NONE,
        IIL_ADAM7
    } IHDR_INTERLACE_METHOD;

    unsigned int    width;
    unsigned int    height;
    unsigned char   depth;
    IHDR_COLOR_TYPE         color_type;
    IHDR_COMPRESSION_METHOD compression_method;
    IHDR_FILTER_METHOD      filter_method;
    IHDR_INTERLACE_METHOD   interlace_method;

    // An IHDR chunk should always appear at offset 8 (to accomodate the PNG
    // signature). The reason I leave it as an argument is ... mostly because
    // it matches all the other chunk loader prototypes. But I don't think
    // anyone will ever need a different offset. Might make this private later,
    // and just have a public method that calls this.
    //
    // Calls PngChunk::loadChunkFromBuffer(), then fills the IHDR-specific
    // struct fields with info from the data vector. Returns number of bytes
    // processed (including the 12 from length, type, and CRC), or 0 on error.
    int loadIhdrChunkFromBuffer(std::vector<char> & buffer_, int offset = 8);
};

// Literally empty. IEND chunks do nothing special.
//
// Might add something to this struct to indicate whether it's at the end of
// the file or not.
struct IendPngChunk : PngChunk {
    int loadIendChunkFromBuffer(std::vector<char> & buffer_, int offset);
};

// Most complicated chunk type I will handle.
//
// The IDAT information is compressed with teh DEFLATE method, I believe.
// This involves two huffman tables (with some special rules) and an LZ-77
// list thing. I will work on being able to decompress such data later.
//
// On top of that, the image data is organized into what they call "scanlines",
// which have a pretty interesting set of rules. The filter method applies on a
// line-by-line basis, too.
//
// So I believe I have to:
//  - Decompress the data field of the base PngChunk using INFLATE
//  - Read the lines using either no interlacing or the ADAM7 thing
//  - Unfilter each line individually
//  - Get / apply a palette, if need be
//     - Might have to wait?
//     - No, have an applyPalette() funciton that takes a PLTE chunk struct
//       as an argument, then indicate whether the image is ready or not
//       so the caller will know whether this struct needs to be given the
//       palette information.
//  - Blow it up into an actual image structure that I can call something like
//    getPixel() and get the actual data I want
//  - Store a pointer to the image structure
struct IdatPngChunk : PngChunk {

};

// I don't know anything about this chunk yet.
// Woudn't surprise me if I needed to do a raindance-decompression on this one
// too.
struct PltePngChunk : PngChunk {

};

// The interface ot the outside world.
//
// The other structs may end up all going in the .cpp file later, but this is
// the class that's actually meant to interface with everyone else.
//
// Mainly, I want to be able to feed in a filename and get out either some
// image type / information OR good error data.
//
// I will probably need to write a special Image class to actually be the
// output from this class.
class PngLoader
{
    std::ifstream file_;
    std::vector<char> buffer_; // Sequence of bytes in the file.
                               // I might make a function to explicitly unload
                               // this later ... OR I'll just delete the
                               // PngLoader wholesale and hang on to the Image
                               // it spits out.
    std::vector<PngChunk> chunk_vector_;
    IhdrPngChunk i_hdr; // for testing, to eventually ONLY be chunk_vector_[0]
    int file_size; // # of bytes in file. Should be equal to buffer_.size().

    // Grabs the contents of filename and puts them in buffer_.
    int loadFileToBuffer(std::string filename);
    // Exactly what it says. All PNG files start with a specific 8-byte
    // signature; this function looks for that 8 bytes in the buffer_.
    bool isPngHeaderPresentinBuffer();
    // Calls the IhdrPngChunk load function with buffer_ as the buffer.
    // Mostly here for test purposes. Future implementation may handle things
    // more generically.
    int loadIhdrChunk();

public:
    // Still being built piece-by-piece. It'll eventually be given an interface,
    // but right now it's calling some test code when it gets constructed.
    PngLoader();
};

struct Block {
    bool bfinal;
    typedef enum {
        UNCOMPRESSED,
        FIXED_HUFFMAN,
        DYNAMIC_HUFFMAN,
        RESERVED
    } DB_TYPE;
    DB_TYPE btype;


};

// Rather closely models the Huffman setup in Adler's puff.c
//class huffmanTable {
//    std::vector<int> symbols_of_given_length;
//    std::map<unsigned int> value_map;


//};

// Decompress RFC-1950 data_in_
class Decompressor {
    std::vector<unsigned char> data_in_;
    std::vector<unsigned char> data_out_;
    unsigned char cmf; // Should be CM = 8 and CINFO = 7 (0x78)
                       // not sure if CINFO has to be 7, but I believe CM
                       // does have to be 8.
    unsigned char flag; // 0bxxxFCCCC, where F is fdict_set, and CCCC makes
                        // ((CMF * 256 + FLG) % 31 == 0) true
    int data_len;
    unsigned int crc;
    bool fdict_set;

    void wikipediaTestCrc();

public:
    Decompressor(std::vector<unsigned char> & data_in);
    int decompress(std::vector<unsigned char> & data_out,
                   std::vector<unsigned char> & data_in);
    std::vector<unsigned char> getOutput() { return data_out_; }
    static bool checkCrc(std::vector<unsigned char> data, unsigned int crc);
};

#endif // PNGLOADER_H
