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
#include "pngloader.h"

#include <string>

//
// **************************
//
// PNG CRC helper functions.
//
// None of these functions are exposed in "pngloader.h"; they exist only to
// get the PNG engine to work right. If they prove useful elsewhere, they
// might get to be moved to "utility.h".
//

// bitreverse_lut(X) -> reverse bits in X
// given that X is 8-bit

// I *think* I need this to get Png IDAT decoding done right
// I definitely need it for the CRC.
static const unsigned char bitreverse_lut[256] = { 0,
    0b10000000, 0b01000000, 0b11000000, 0b00100000,
    0b10100000, 0b01100000, 0b11100000, 0b00010000,
    0b10010000, 0b01010000, 0b11010000, 0b00110000,
    0b10110000, 0b01110000, 0b11110000, 0b00001000,
    0b10001000, 0b01001000, 0b11001000, 0b00101000,
    0b10101000, 0b01101000, 0b11101000, 0b00011000,
    0b10011000, 0b01011000, 0b11011000, 0b00111000,
    0b10111000, 0b01111000, 0b11111000, 0b00000100,
    0b10000100, 0b01000100, 0b11000100, 0b00100100,
    0b10100100, 0b01100100, 0b11100100, 0b00010100,
    0b10010100, 0b01010100, 0b11010100, 0b00110100,
    0b10110100, 0b01110100, 0b11110100, 0b00001100,
    0b10001100, 0b01001100, 0b11001100, 0b00101100,
    0b10101100, 0b01101100, 0b11101100, 0b00011100,
    0b10011100, 0b01011100, 0b11011100, 0b00111100,
    0b10111100, 0b01111100, 0b11111100, 0b00000010,
    0b10000010, 0b01000010, 0b11000010, 0b00100010,
    0b10100010, 0b01100010, 0b11100010, 0b00010010,
    0b10010010, 0b01010010, 0b11010010, 0b00110010,
    0b10110010, 0b01110010, 0b11110010, 0b00001010,
    0b10001010, 0b01001010, 0b11001010, 0b00101010,
    0b10101010, 0b01101010, 0b11101010, 0b00011010,
    0b10011010, 0b01011010, 0b11011010, 0b00111010,
    0b10111010, 0b01111010, 0b11111010, 0b00000110,
    0b10000110, 0b01000110, 0b11000110, 0b00100110,
    0b10100110, 0b01100110, 0b11100110, 0b00010110,
    0b10010110, 0b01010110, 0b11010110, 0b00110110,
    0b10110110, 0b01110110, 0b11110110, 0b00001110,
    0b10001110, 0b01001110, 0b11001110, 0b00101110,
    0b10101110, 0b01101110, 0b11101110, 0b00011110,
    0b10011110, 0b01011110, 0b11011110, 0b00111110,
    0b10111110, 0b01111110, 0b11111110, 0b00000001,
    0b10000001, 0b01000001, 0b11000001, 0b00100001,
    0b10100001, 0b01100001, 0b11100001, 0b00010001,
    0b10010001, 0b01010001, 0b11010001, 0b00110001,
    0b10110001, 0b01110001, 0b11110001, 0b00001001,
    0b10001001, 0b01001001, 0b11001001, 0b00101001,
    0b10101001, 0b01101001, 0b11101001, 0b00011001,
    0b10011001, 0b01011001, 0b11011001, 0b00111001,
    0b10111001, 0b01111001, 0b11111001, 0b00000101,
    0b10000101, 0b01000101, 0b11000101, 0b00100101,
    0b10100101, 0b01100101, 0b11100101, 0b00010101,
    0b10010101, 0b01010101, 0b11010101, 0b00110101,
    0b10110101, 0b01110101, 0b11110101, 0b00001101,
    0b10001101, 0b01001101, 0b11001101, 0b00101101,
    0b10101101, 0b01101101, 0b11101101, 0b00011101,
    0b10011101, 0b01011101, 0b11011101, 0b00111101,
    0b10111101, 0b01111101, 0b11111101, 0b00000011,
    0b10000011, 0b01000011, 0b11000011, 0b00100011,
    0b10100011, 0b01100011, 0b11100011, 0b00010011,
    0b10010011, 0b01010011, 0b11010011, 0b00110011,
    0b10110011, 0b01110011, 0b11110011, 0b00001011,
    0b10001011, 0b01001011, 0b11001011, 0b00101011,
    0b10101011, 0b01101011, 0b11101011, 0b00011011,
    0b10011011, 0b01011011, 0b11011011, 0b00111011,
    0b10111011, 0b01111011, 0b11111011, 0b00000111,
    0b10000111, 0b01000111, 0b11000111, 0b00100111,
    0b10100111, 0b01100111, 0b11100111, 0b00010111,
    0b10010111, 0b01010111, 0b11010111, 0b00110111,
    0b10110111, 0b01110111, 0b11110111, 0b00001111,
    0b10001111, 0b01001111, 0b11001111, 0b00101111,
    0b10101111, 0b01101111, 0b11101111, 0b00011111,
    0b10011111, 0b01011111, 0b11011111, 0b00111111,
    0b10111111, 0b01111111, 0b11111111
};

// bits from 0-7, counting from the right, e.g.:
// 76543210
// Function gets the value of the specified bit in the given byte (either 0
// or 1).
unsigned char bitInByte(unsigned char byte, int bit) {
    return (byte >> bit) & 0x01;
}

// 5 returnable bytes, indexed like so: 43210
// The poly with no rshift is only in 4321, whereas with rshift = 7, it will
// have its MSb in 4 with the rest of it in 3210.
//
// Function gets the aligned piece of the CRC polynomial given the bitshift
// specified.
unsigned char shiftedPolyByte(unsigned int poly,
                              int rshift_cnt,
                              int byte_num) {
    if(rshift_cnt > 8 || rshift_cnt < 0) {
        return 0;
    }
    switch(byte_num) {
    case 4:
        return ((poly >> rshift_cnt) & 0xff000000) >> 24;
        break;

    case 3:
        return ((poly >> rshift_cnt) & 0x00ff0000) >> 16;
        break;

    case 2:
        return ((poly >> rshift_cnt) & 0x0000ff00) >> 8;
        break;

    case 1:
        return (poly >> rshift_cnt) & 0x000000ff;
        break;

    case 0:
        return (poly << (8 - rshift_cnt)) & 0x000000ff;

    default:
        return 0;
    }
}

// Does the actual computation.
// Kind of involved.
unsigned int getPolyDivisionRemainder(std::vector<unsigned char> & in,
                                      unsigned int poly) {
    std::vector<unsigned char> tmp = in;
    // Space for remainder
    for(int i = 0; i < 4; ++i) {
        tmp.push_back(0x00);
    }
    int len = tmp.size() - 4; // subtract space for remainder
    for(int i = 0; i < len; ++i) {
        for (int bit = 7; bit >= 0; --bit) {
            bool do_xor = (bitInByte(tmp[i], bit) == 1);
            // bitshift is handled by bit counter
            if(do_xor) {
                // divide / xor
                // Apply the poly to the char stream
                // The shifting aligns the poly with the correct bit
                // Because a 4-byte poly is being shifted, I actually need
                // 5 bytes to handle the shifted version.
                for(int j = 0; j < 5; j++) {
                    tmp[i + j] ^= shiftedPolyByte(poly, 8 - bit, 4 - j);
                }
            }
        }
    }
    unsigned int b1 = 0, b2 = 0, b3 = 0, b4 = 0;
    b1 = static_cast<unsigned int>(tmp[len + 0]) << 24;
    b2 = static_cast<unsigned int>(tmp[len + 1]) << 16;
    b3 = static_cast<unsigned int>(tmp[len + 2]) <<  8;
    b4 = static_cast<unsigned int>(tmp[len + 3]) <<  0;
    unsigned int retval = (b1 & 0xff000000) |
                          (b2 & 0x00ff0000) |
                          (b3 & 0x0000ff00) |
                          (b4 & 0x000000ff);
    return retval;
}

unsigned int reverseUIntBits(unsigned int in) {
    return ((bitreverse_lut[(in >> 24) & 0x000000ff] <<  0) & 0x000000ff) |
           ((bitreverse_lut[(in >> 16) & 0x000000ff] <<  8) & 0x0000ff00) |
           ((bitreverse_lut[(in >>  8) & 0x000000ff] << 16) & 0x00ff0000) |
           ((bitreverse_lut[(in >>  0) & 0x000000ff] << 24) & 0xff000000);
}

//
// END PNG helper functions
//
// **************************
//

// Note: I found most of what I needed to know by reading documents about CRC
// calculation, but having a test platform was invaluable. Thanks to the
// guy that made:
//     http://www.zorc.breitbandkatze.de/crc.html
// I couldn't have done this without trying 30 bugged versions on that site.
bool PngChunk::checkCrc(std::vector<unsigned char> & data, unsigned int crc) {
    unsigned int poly       = 0x04c11db7;
    std::vector<unsigned char> tmp;
    // Push on the correct prefix. This was found by experimenting on:
    //     http://www.zorc.breitbandkatze.de/crc.html
    // The right thing to do is to set "initial value" to FFFFFFFF, direct
    // and then hit convert.
    //
    // This has *something* to do with augmented 0 bits in the remainder.
    // I *think* if I pushed FFFFFFFF into the remainder instead of 0, I
    // wouldn't need this here prefix.
    tmp.push_back('\x46');
    tmp.push_back('\xaf');
    tmp.push_back('\x64');
    tmp.push_back('\x49');
    // Push on the data, but reverse each byte as it is added.
    for(    std::vector<unsigned char>::iterator it = data.begin();
            it != data.end();
            ++it) {
        tmp.push_back(bitreverse_lut[*it]);
    }
    // Do the polynomaial division / XORing
    unsigned int crc_result = getPolyDivisionRemainder(tmp, poly);
    // Flip the CRC and NOT the bits
    crc_result = reverseUIntBits(crc_result);
    return (~crc_result) == crc;
}

int PngChunk::loadChunkFromBuffer(std::vector<char> & buffer_, int offset)
{
    // Sanity check on buffer and offset
    int offset_ = offset;
    if(buffer_.size() - (offset_ + 12) < 0) {
        ERR_MSGOUT("Chunk length longer than remaining data");
        return 0;
    }

    // Load the chunk length
    unsigned int b1 = 0, b2 = 0, b3 = 0, b4 = 0;

    b1 = static_cast<unsigned int>(buffer_[offset_ + 0]) << 24;
    b2 = static_cast<unsigned int>(buffer_[offset_ + 1]) << 16;
    b3 = static_cast<unsigned int>(buffer_[offset_ + 2]) << 8;
    b4 = static_cast<unsigned int>(buffer_[offset_ + 3]) << 0;
    length = (b1 & 0xff000000) |
             (b2 & 0x00ff0000) |
             (b3 & 0x0000ff00) |
             (b4 & 0x000000ff);
    // Sanity check the data length
    if(buffer_.size() - (length + 12) < 0) {
        ERR_MSGOUT("Chunk length longer than remaining data");
        return 0;
    }
    offset_ += 4;

    // Load the type information
    type_charstr[0] = buffer_[offset_ + 0];
    type_charstr[1] = buffer_[offset_ + 1];
    type_charstr[2] = buffer_[offset_ + 2];
    type_charstr[3] = buffer_[offset_ + 3];
    type_charstr[4] = '\0';
    // Is it a critical chunk type?
    // Critical if bit 5 is NOT set, otherwise non-critical
    is_critical = (type_charstr[0] & 0x20 ?  false : true);
    // Set the type enum
    std::string type_str = type_charstr;
    if(type_str.compare("IHDR") == 0) {
        pngl_type = PngChunk::IHDR;
    }
    else if(type_str.compare("IEND") == 0) {
        pngl_type = PngChunk::IEND;
    }
    else if(type_str.compare("IDAT") == 0) {
        pngl_type = PngChunk::IDAT;
    }
    else if(type_str.compare("PLTE") == 0) {
        pngl_type = PngChunk::PLTE;
    }
    else {
        pngl_type = PngChunk::UNSUPPORTED;
    }
    offset_ += 4;

    // Load the data
    for(int i = 0; i < length; ++i) {
        // Converting from signed char to unsigned char here
        data.push_back(buffer_[offset_ + i]);
    }
    offset_ += length;

    // Load the CRC
    b1 = 0, b2 = 0, b3 = 0, b4 = 0;
    b1 = static_cast<unsigned int>(buffer_[offset_ + 0]) << 24;
    b2 = static_cast<unsigned int>(buffer_[offset_ + 1]) << 16;
    b3 = static_cast<unsigned int>(buffer_[offset_ + 2]) << 8;
    b4 = static_cast<unsigned int>(buffer_[offset_ + 3]) << 0;
    crc = (b1 & 0xff000000) |
          (b2 & 0x00ff0000) |
          (b3 & 0x0000ff00) |
          (b4 & 0x000000ff);

    // When checkCrc is implemented, this will do the CRC check
    // It'll operate on every chunk
    // Other: the type chars need to be on the front of the data, so you pass
    // TYPE + DATA to this function.
    std::vector<unsigned char> tmp;
    for(int i = 0; i < 4; ++i) {
        tmp.push_back(type_charstr[i]);
    }
    for(    std::vector<unsigned char>::iterator it = data.begin();
            it != data.end();
            ++it) {
        tmp.push_back(*it);
    }
    if(! checkCrc(tmp, crc)) {
        ERR_MSGOUT("Png chunk crc failed");
        return 0;
    }

    // return # of bytes read
    loaded = true;
    bytes_in_chunk = (offset_ + 4) - offset;
    return bytes_in_chunk;
}

void PngChunk::wikipediaTestCrc()
{
    std::vector<unsigned char> ucv;
    ucv.push_back('W');
    ucv.push_back('i');
    ucv.push_back('k');
    ucv.push_back('i');
    ucv.push_back('p');
    ucv.push_back('e');
    ucv.push_back('d');
    ucv.push_back('i');
    ucv.push_back('a');
    if(! PngChunk::checkCrc(ucv, 0xadaac02e)) {
        ERR_MSGOUT("CRC-32 checkCrc did not calculate properly"
                   " in wikipediaTestCrc");
    }
}

void PngChunk::iendTestCrc()
{
    std::vector<unsigned char> ucv;
    ucv.push_back('I');
    ucv.push_back('E');
    ucv.push_back('N');
    ucv.push_back('D');
    unsigned int crcval = 0xae426082;
    if(! PngChunk::checkCrc(ucv, crcval)) {
        ERR_MSGOUT("CRC function bugged");
    }
    else {
        ERR_MSGOUT("CRC OK");
    }
}

int IhdrPngChunk::loadIhdrChunkFromBuffer(std::vector<char> &buffer_,
                                          int offset // = 8
                                          )
{
    if (offset != 8) {
        ERR_MSGOUT("Png header wrong length");
        return 0;
    }
    int bytes_read = 0;
    if(! loaded) {
        bytes_read = loadChunkFromBuffer(buffer_, offset);
        loaded = true;
    }
    bytes_read = bytes_in_chunk;
    if (bytes_read != 25) {
        ERR_MSGOUT("Bad IHDR length");
        return 0;
    }
    unsigned int b1 = 0, b2 = 0, b3 = 0, b4 = 0;
    b1 = static_cast<unsigned int>(data[0]) << 24;
    b2 = static_cast<unsigned int>(data[1]) << 16;
    b3 = static_cast<unsigned int>(data[2]) <<  8;
    b4 = static_cast<unsigned int>(data[3]) <<  0;
    width = (b1 & 0xff000000) |
            (b2 & 0x00ff0000) |
            (b3 & 0x0000ff00) |
            (b4 & 0x000000ff);

    b1 = 0, b2 = 0, b3 = 0, b4 = 0;
    b1 = static_cast<unsigned int>(data[4]) << 24;
    b2 = static_cast<unsigned int>(data[5]) << 16;
    b3 = static_cast<unsigned int>(data[6]) <<  8;
    b4 = static_cast<unsigned int>(data[7]) <<  0;
    height = (b1 & 0xff000000) |
             (b2 & 0x00ff0000) |
             (b3 & 0x0000ff00) |
             (b4 & 0x000000ff);

    depth = this->data[ 8];
    switch(data[ 9]) {
    case 0:
        color_type = IhdrPngChunk::IC_GRAYSCALE;
        break;

    case 2:
        color_type = IhdrPngChunk::IC_COLOR;
        break;

    case 3:
        color_type = IhdrPngChunk::IC_COLORPALETTE;
        break;

    case 4:
        color_type = IhdrPngChunk::IC_GRAYSCALEALPHA;
        break;

    case 6:
        color_type = IhdrPngChunk::IC_COLORALPHA;
        break;

    default:
        ERR_MSGOUT("Bad PNG Color Type");
        color_type = IhdrPngChunk::IC_INVALID;
        return 0;
    }

    switch(depth) {
    // Fall through
    case 1:
    case 2:
    case 4:
        // Only palette and grayscale can have 1, 2, or 4 for depth
        if(!(color_type == IhdrPngChunk::IC_COLORPALETTE) ||
            (color_type == IhdrPngChunk::IC_GRAYSCALE)) {
                return 0;
        }
        break;

    case 8:
        break;

    case 16:
        // Palette may not have depth == 16
        if(color_type == IhdrPngChunk::IC_COLORPALETTE) {
            return 0;
        }
        break;

    default:
        ERR_MSGOUT("Bad PNG Depth");
        return 0;
    }

    if(data[10] != 0) {
        ERR_MSGOUT("Bad Png compression method in IHDR");
        return 0;
    }
    compression_method = IhdrPngChunk::ICOM_NONE;

    if(data[11] != 0) {
        ERR_MSGOUT("Bad Png filter method in IHDR");
        return 0;
    }
    filter_method = IhdrPngChunk::IFIL_NONE;

    switch(data[12]) {
    case 0:
        interlace_method = IhdrPngChunk::IIL_NONE;
        break;

    case 1:
        interlace_method = IhdrPngChunk::IIL_ADAM7;
        break;

    default:
        ERR_MSGOUT("Bad Png interlace method in IHDR");
        return 0;
    }

    return bytes_read;
}

int IendPngChunk::loadIendChunkFromBuffer(std::vector<char> &buffer_, int offset)
{
    int bytes_read = loadChunkFromBuffer(buffer_, offset);
    if (bytes_read != 25) {
        return 0;
    }
}

int PngLoader::loadFileToBuffer(std::string filename)
{
    file_.open(filename.c_str(), std::ios_base::binary |
                                 std::ios_base::ate);
    if(!file_.is_open()) {
        std::stringstream ss;
        ss << filename << " not found";
        ERR_MSGOUT(ss.str().c_str());
        return 0;
    }
    // Get length of file
    int pos = file_.tellg();
    file_.seekg(0, std::ios_base::beg);
    // Load file into buffer
    buffer_ = std::vector<char>(std::istreambuf_iterator<char>(file_),
                                std::istreambuf_iterator<char>());
    // Check the file was read filly
    if(pos != buffer_.size()) {
        ERR_MSGOUT("Unable to read complete Png image");
        file_.close();
        return 0;
    }

    file_.close();

    return buffer_.size();
}

// Checks for the sequence 0x89 "PNG" "\r\n" "^z" "\n"
bool PngLoader::isPngHeaderPresentinBuffer()
{
    if(buffer_.size() < 8) {
        return false;
    }
    if(     buffer_[0] == '\x89' &&
            buffer_[1] == '\x50' &&
            buffer_[2] == '\x4e' &&
            buffer_[3] == '\x47' &&
            buffer_[4] == '\x0d' &&
            buffer_[5] == '\x0a' &&
            buffer_[6] == '\x1a' &&
            buffer_[7] == '\x0a') {
        return true;
    }
    return false;
}

int PngLoader::loadIhdrChunk()
{
    int offset = 0;
    int bytes_read = i_hdr.loadIhdrChunkFromBuffer(buffer_);
    if (bytes_read == 0) {
        ERR_MSGOUT("Failed to load Png image at IHDR");
        return 0;
    }
    return bytes_read;
}

PngLoader::PngLoader()
{
    file_size = loadFileToBuffer("resources\\road_1.png");
    if(! isPngHeaderPresentinBuffer()) {
        ERR_MSGOUT_DEBUG("PNG header not found");
    }
    int offset = loadIhdrChunk();
    ERR_MSGOUT_DEBUG("PNG HERE");
    if(offset == 0) {
        return;
    }
    offset += 8;
    chunk_vector_.push_back(i_hdr);
    while (offset < file_size) {
        PngChunk chunk;
        offset += chunk.loadChunkFromBuffer(buffer_, offset);
        chunk_vector_.push_back(chunk);
    }
}

bool Decompressor::checkCrc(std::vector<unsigned char> data, unsigned int crc)
{
    unsigned int A = 1, B = 0;
    for(    std::vector<unsigned char>::iterator it = data.begin();
            it != data.end();
            ++it) {
        A += *it;
        A %= 65521;
        B += A;
        B %= 65521;
    }
    unsigned int checksum = ((B << 16) & 0xffff0000) |
                            (A & 0x0000ffff);
    return (checksum == crc);
}

void Decompressor::wikipediaTestCrc()
{
    std::vector<unsigned char> ucv;
    ucv.push_back('W');
    ucv.push_back('i');
    ucv.push_back('k');
    ucv.push_back('i');
    ucv.push_back('p');
    ucv.push_back('e');
    ucv.push_back('d');
    ucv.push_back('i');
    ucv.push_back('a');
    if(!Decompressor::checkCrc(ucv, 0x11e60398)){
        ERR_MSGOUT("Inflate checkCrc did not compute properly"
                   " in wikipediaTestCrc");
    }
}

int Decompressor::decompress(std::vector<unsigned char> &data_out,
                             std::vector<unsigned char> &data_in)
{
    data_in_ = data_in;
    data_out_.clear();

    cmf  = data_in_[0];
    flag = data_in_[1];
    data_len = data_in_.size() - 6; // 6 bytes for CMF, FLG, and CRC

    // Set CRC
    unsigned int b1 = 0, b2 = 0, b3 = 0, b4 = 0;

    b1 = static_cast<unsigned int>(data_in_[data_len + 2]) << 24;
    b2 = static_cast<unsigned int>(data_in_[data_len + 3]) << 16;
    b3 = static_cast<unsigned int>(data_in_[data_len + 4]) << 8;
    b4 = static_cast<unsigned int>(data_in_[data_len + 5]) << 0;
    crc = (b1 & 0xff000000) |
          (b2 & 0x00ff0000) |
          (b3 & 0x0000ff00) |
          (b4 & 0x000000ff);

    // Decompress here

    data_out = data_out_;
}
