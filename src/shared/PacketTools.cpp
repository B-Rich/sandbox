/**
 * *********************************************************************
 * OpenSWG Sandbox Server
 * Copyright (C) 2006 OpenSWG Team <http://www.openswg.com>
 * *********************************************************************
 * Copyright (C) 2006 Team SWGEmu <http://www.swgemu.com>
 * ********************************************************************* *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * To read the license please visit http://www.gnu.org/copyleft/gpl.html
 * *********************************************************************
 *
 * All Encryption and Compression functions by Team SWGEmu!
 * Packet Loading Function by OpenSWG Team
 */


#include "PacketTools.h"
#define MAX_PACKET_SIZE 1000

#include <malloc.h>
#include <zlib.h>

#ifdef WIN32		
#include <tchar.h>
#endif
		
#include <fstream>
#include <string>
#include <regex>

static const unsigned int crc32table[256] =
{
0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f,
0xe963a535, 0x9e6495a3, 0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988,
0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91, 0x1db71064, 0x6ab020f2,
0xf3b97148, 0x84be41de, 0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec, 0x14015c4f, 0x63066cd9,
0xfa0f3d63, 0x8d080df5, 0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172,
0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b, 0x35b5a8fa, 0x42b2986c,
0xdbbbc9d6, 0xacbcf940, 0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423,
0xcfba9599, 0xb8bda50f, 0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924,
0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d, 0x76dc4190, 0x01db7106,
0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d,
0x91646c97, 0xe6635c01, 0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e,
0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457, 0x65b0d9c6, 0x12b7e950,
0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7,
0xa4d1c46d, 0xd3d6f4fb, 0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0,
0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9, 0x5005713c, 0x270241aa,
0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81,
0xb7bd5c3b, 0xc0ba6cad, 0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a,
0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683, 0xe3630b12, 0x94643b84,
0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb,
0x196c3671, 0x6e6b06e7, 0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc,
0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5, 0xd6d6a3e8, 0xa1d1937e,
0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55,
0x316e8eef, 0x4669be79, 0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236,
0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f, 0xc5ba3bbe, 0xb2bd0b28,
0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f,
0x72076785, 0x05005713, 0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38,
0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21, 0x86d3d2d4, 0xf1d4e242,
0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69,
0x616bffd3, 0x166ccf45, 0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2,
0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db, 0xaed16a4a, 0xd9d65adc,
0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693,
0x54de5729, 0x23d967bf, 0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94,
0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
};



std::tr1::shared_ptr<ByteBuffer> LoadPacketFromTextFile(const std::string& name)
{
    std::string line_buffer;
    std::tr1::shared_ptr<ByteBuffer> packet(new ByteBuffer());

    static const std::tr1::regex pattern("0x([0-9a-fA-F]+)");
    const int keep[] = {1}; 
    std::tr1::smatch result;

    std::ifstream file_stream(name.c_str());
    while (std::getline(file_stream, line_buffer)) {
        
        // Loop over the line searching for the pattern. Note the "keep"
        // is used to select the 1st subpattern to keep. Leaving this off
        // matches the 0x as well.
        const std::tr1::sregex_token_iterator end;
        for (std::tr1::sregex_token_iterator i(
                line_buffer.begin(), line_buffer.end(), pattern, keep); 
             i != end; ++i) {
                 *packet << axtoi((*i).str().c_str());
        }
    }

    return packet;
}


void Compress(std::tr1::shared_ptr<ByteBuffer> packet)
{
    // Grab a reference to the internals of the packet. Generally
    // this should not be done but this is one of the special circumstances
    // the raw() was implemented for. This allows us to work on
    // the raw data with a non-standard library and minimize the amount
    // of copying.
    std::vector<uint8_t>& packet_data = packet->raw();  

    // Determine the offset to begin compressing data at.
    uint16_t offset = (packet_data[0] == 0x00) ? 2 : 1;

    // Create a container for the compressed data and initialize the 
    // z_stream with the necessary default values.
    z_stream stream;

    stream.zalloc   = Z_NULL;
    stream.zfree    = Z_NULL;
    stream.opaque   = Z_NULL;
    stream.avail_in = 0;
    stream.next_in  = Z_NULL;

    deflateInit(&stream, Z_DEFAULT_COMPRESSION); 
 
    // Prepare the stream for compression and then compress the data.
    std::vector<uint8_t> compression_output(packet->size() + 20);
 
    stream.next_in   = reinterpret_cast<Bytef *>(&packet_data[offset]);
    stream.avail_in  = packet->size() - offset - 3;
    stream.next_out  = reinterpret_cast<Bytef *>(&compression_output[0]);
    stream.avail_out = packet->size() + 20;

    deflate(&stream, Z_FINISH);

    // SOE protocol requires this bit at the end of compressed data.
    compression_output[stream.total_out] = 0x01;

    // Replace the section of the packet data copy between the offset and 
    // the crc bits.
    packet_data.insert(
        packet_data.erase(packet_data.begin() + offset, packet_data.end() - 2),
        compression_output.begin(),
        compression_output.begin() + stream.total_out + 1 // The +1 is for the added SOE bit.
        );

    deflateEnd(&stream);
}


void Decompress(std::tr1::shared_ptr<ByteBuffer> packet)
{
    // Grab a reference to the internals of the packet. Generally
    // this should not be done but this is one of the special circumstances
    // the raw() was implemented for. This allows us to work on
    // the raw data with a non-standard library and minimize the amount
    // of copying.
    std::vector<uint8_t>& packet_data = packet->raw();  

    // Determine the offset to begin compressing data at.
    uint16_t offset = (packet_data[0] == 0x00) ? 2 : 1;

    // Create a container for the compressed data and initialize the 
    // z_stream with the necessary default values.
    z_stream stream;

    stream.zalloc   = Z_NULL;
    stream.zfree    = Z_NULL;
    stream.opaque   = Z_NULL;
    stream.avail_in = 0;
    stream.next_in  = Z_NULL;

    inflateInit(&stream);
 
    // Prepare the stream for compression and then compress the data.
    std::vector<uint8_t> compression_output(packet->size() + 20);
 
    stream.next_in   = reinterpret_cast<Bytef *>(&packet_data[offset]);
    stream.avail_in  = packet->size() - offset - 3;
    stream.next_out  = reinterpret_cast<Bytef *>(&compression_output[0]);
    stream.avail_out = packet->size() + 20;

    inflate(&stream, Z_FINISH);

    // SOE protocol requires this bit at the end of compressed data.
    compression_output[stream.total_out] = 0x01;

    // Replace the section of the packet data copy between the offset and 
    // the crc bits.
    packet_data.insert(
        packet_data.erase(packet_data.begin() + offset, packet_data.end() - 2),
        compression_output.begin(),
        compression_output.begin() + stream.total_out + 1 // The +1 is for the added SOE bit.
        );

    inflateEnd(&stream);
}


void Encrypt(std::tr1::shared_ptr<ByteBuffer> packet, uint32_t seed)
{
    std::vector<uint8_t> packet_data(packet->data(), packet->data() + packet->size());
    char* pData = reinterpret_cast<char*>(&packet_data[0]);

    uint16_t nLength = packet->size();

    unsigned int *Data;
    if(pData[0] == 0x00)
    {
     nLength-=4;
     Data = (unsigned int*)(pData+2);
    }
    else
    {
     nLength-=3;
     Data = (unsigned int*)(pData+1);
    }
    short block_count = (nLength / 4);
    short byte_count = (nLength % 4);
 
    for(short count = 0;count<block_count;count++)
    {
        seed ^= *Data;
        *Data = seed;
        //*Data ^= nCrcSeed;
        //nCrcSeed = *Data;
        Data++;
    }
    pData = (char*)Data;
    for(short count = 0;count<byte_count;count++)
    {
        *pData ^= (char)seed;
        pData++;
    }

    ByteBuffer tmp(reinterpret_cast<uint8_t*>(&packet_data[0]), packet_data.size());
    packet->swap(tmp);
}


void Decrypt(std::tr1::shared_ptr<ByteBuffer> packet, uint32_t seed, uint16_t seedLength)
{
    // Grab a reference to the internals of the packet. Generally
    // this should not be done but this is one of the special circumstances
    // the raw() was implemented for. This allows us to work on
    // the raw data with a non-standard library and minimize the amount
    // of copying.
    std::vector<uint8_t>& packet_data = packet->raw();  

    // Determine the offset to begin decrypting data at.
    uint16_t offset = (packet_data[0] == 0x00) ? 2 : 1;

    // Determine the block and byte counts to assist in processing.
    uint16_t block_count = (packet_data.size() - offset - seedLength) / 4;
    uint16_t byte_count  = (packet_data.size() - offset - seedLength) % 4;

    // Grab the value at the current position and store it in the tmp
    // holder before processing.
    for (uint16_t count = 0; count < block_count; ++count) {
        uint32_t* current = reinterpret_cast<uint32_t *>(&packet_data[offset + (count * 4)]);
        
        uint32_t tmp = *current;
        *current    ^= seed;
        seed         = tmp;
    }

    for (uint16_t count = 0; count < byte_count; ++count) {
        uint8_t* current = &packet_data[offset + (block_count * 4) + count];
        *current ^= seed;
    }
}


void AppendCrc(std::tr1::shared_ptr<ByteBuffer> packet, uint32_t seed, uint16_t seedLength)
{
    std::vector<char> pData(packet->data(), packet->data() + packet->size());
    uint16_t nLength = packet->size();

    if (seedLength > 0)
    {
        unsigned int crc = GenerateCrc(&pData[0], (nLength-seedLength), seed);
       // pData += (nLength-seedLength);
        for( short i = 0; i < seedLength; i++ )
        {
            pData[(nLength - 1) - i] = (char)((crc >> (8 * i)) & 0xFF);
        }
    }

    ByteBuffer tmp(reinterpret_cast<uint8_t*>(&pData[0]), pData.size());
    packet->swap(tmp);
}


char* loadPacket(const std::string& name, unsigned short* length = NULL) 
{
	// Create a container for the packet data and a buffer
	// to use when reading in the file data.
 	std::string data;
	std::string buffer;
		
	// Remove leading and trailing whitespace
	static const char whitespace[] = " \n\t\v\r\f";

	// Read in the file a line at a time.
    std::ifstream ifs(name.c_str());
	while (std::getline(ifs, buffer)) {
		// Remove any comments.
		buffer = buffer.substr( 0, buffer.find('#'));	
		
		// Remove any whitespace from the beginning/end of the line.
		buffer.erase(0, buffer.find_first_not_of(whitespace));
		buffer.erase(buffer.find_last_not_of(whitespace) + 1U);

		// Append the buffer to the packet data.
		data.append(buffer);
	}

	char* packet;				// Container for the final packet data.
	char* tokenPtr;				// Tokenizer for breaking up the string.
	char tmp[MAX_PACKET_SIZE]; // Buffer 
	*length = 0;				// Initialize the packet size.

	// This for loop breaks apart the packet data string and converts
	// each string value to is hexidecimal value and stores the
	// resulting data in the buffer. Also, at the end of the for loop
	// the next token pointer is found and the length is increased.
	for (tokenPtr = strtok(const_cast<char*>(data.c_str()), " ,\n");
		 tokenPtr;
		 tokenPtr = strtok(NULL, " ,\n"), *length += 1)
	{		
		*(unsigned char*)(tmp+*length) = (unsigned char)axtoi(tokenPtr+2);
	}

	// Copy the packet data from the buffer to the packet container.
	packet = new char[*length];
	for (int j = 0; j < *length; j++)
	{
		*(unsigned char*)(packet+j) = (unsigned char)*(tmp+j);
	}
	
	/* Uncomment this to display an output of each packet as it's loaded.
	printf("LOADED PACKET\nPacket Size: %d \nPacket Data: \n", *length);
	for(int k = 0; k < *length; k++)
	{
		printf("0x%02x ",(unsigned char)*(packet+k));
	}
	printf("\n");
	*/

	return packet;
}

uint8_t axtoi(const char * const hexString) {
	uint8_t n = 0;         // position in string
	uint8_t m = 0;         // position in digit[] to shift
	uint8_t count;         // loop index
	uint8_t intValue = 0;  // integer value of hex string
	uint8_t digit[5];      // hold values to convert
	while (n < 4) {
		if (hexString[n]=='\0')
			break;
		if (hexString[n]!='\n'&&hexString[n]!=' '&&hexString[n]!='\a') {
			if (hexString[n] > 0x2f && hexString[n] < 0x40 ) //if 0 to 9
				digit[n] = hexString[n] & 0x0f;            //convert to int
			else if (hexString[n] >='a' && hexString[n] <= 'f') //if a to f
				digit[n] = (hexString[n] & 0x0f) + 9;      //convert to int
			else if (hexString[n] >='A' && hexString[n] <= 'F') //if A to F
				digit[n] = (hexString[n] & 0x0f) + 9;      //convert to int
			else break;
		}
		n++;
	}
	
	count = n;
	m = n - 1;
	n = 0;
	
	while(n < count) {
		// digit[n] is value of hex digit at position n
		// (m << 2) is the number of positions to shift
		// OR the bits into return value
		intValue = intValue | (digit[n] << (m << 2));
		m--;   // adjust the position to set
		n++;   // next digit to process
	}
  
	return (intValue);
}



/** SWGEmu Code Below This Point
 *	Copyright (C) 2006 Team SWGEmu <http://www.swgemu.com>
 */

unsigned int GenerateCrc(char* pData,unsigned short nLength,unsigned int nCrcSeed)
{
    unsigned int nCrc = crc32table[(~nCrcSeed) & 0xFF];
    nCrc ^= 0x00FFFFFF;
    unsigned int nIndex = (nCrcSeed >> 8) ^ nCrc;
    nCrc = (nCrc >> 8) & 0x00FFFFFF;
    nCrc ^= crc32table[nIndex & 0xFF];
    nIndex = (nCrcSeed >> 16) ^ nCrc;
    nCrc = (nCrc >> 8) & 0x00FFFFFF;
    nCrc ^= crc32table[nIndex & 0xFF];
    nIndex = (nCrcSeed >> 24) ^ nCrc;
    nCrc = (nCrc >> 8) &0x00FFFFFF;
    nCrc ^= crc32table[nIndex & 0xFF];

    for( short i = 0; i < nLength; i++ )
    {
        nIndex = (pData[i]) ^ nCrc;
        nCrc = (nCrc >> 8) & 0x00FFFFFF;
        nCrc ^= crc32table[nIndex & 0xFF];
    }

    return ~nCrc;
}


bool CrcTest(char *pData, unsigned short nLength,unsigned int nCrcSeed, short nCrcLength)
{
    bool crctest = true;
    if(nCrcLength > 0)
    {
        unsigned int p_crc = GenerateCrc(pData,(nLength-nCrcLength),nCrcSeed);
        unsigned int  crc = 0;
        unsigned int mask = 0;
        unsigned int pullbyte = 0;
        pData = pData + (nLength-nCrcLength);
        for( short i = 0; i < nCrcLength; i++ )
        {
            pullbyte = (unsigned char)pData[i];
            crc |=(pullbyte << (((nCrcLength - 1) - i) * 8));
            mask <<= 8;
            mask |= 0xFF;
        }
        p_crc &= mask;
        if(p_crc != crc)
            crctest = false;
    }
    return crctest;
}

void AppendCRC(char *pData, unsigned short nLength, unsigned int nCrcSeed, short nCrcLength)
{
    if (nCrcLength > 0)
    {
        unsigned int crc = GenerateCrc(pData,(nLength-nCrcLength),nCrcSeed);
        pData += (nLength-nCrcLength);
        for( short i = 0; i < nCrcLength; i++ )
        {
            pData[(nCrcLength - 1) - i] = (char)((crc >> (8 * i)) & 0xFF);
        }
    }

}


void Decrypt(char *pData,unsigned short nLength,unsigned int nCrcSeed)
{
    unsigned int *Data;
    if(pData[0] == 0x00)
    {
     nLength-=4;
     Data = (unsigned int*)(pData+2);
    }
    else
    {
     nLength-=3;
     Data = (unsigned int*)(pData+1);
    }
    short block_count = (nLength / 4);
    short byte_count = (nLength % 4);
    unsigned int itemp;
    for(short count = 0;count<block_count;count++)
    {
        itemp = *Data;
        *Data ^= nCrcSeed;
        nCrcSeed = itemp;
        Data++;
    }
    pData = (char*)Data;
    for(short count = 0;count<byte_count;count++)
    {
        *pData ^= nCrcSeed;
        pData++;
    }
}

void Encrypt(char *pData, unsigned short nLength,unsigned int nCrcSeed)
{
    unsigned int *Data;
    if(pData[0] == 0x00)
    {
     nLength-=4;
     Data = (unsigned int*)(pData+2);
    }
    else
    {
     nLength-=3;
     Data = (unsigned int*)(pData+1);
    }
    short block_count = (nLength / 4);
    short byte_count = (nLength % 4);
 
    for(short count = 0;count<block_count;count++)
    {
        nCrcSeed ^= *Data;
        *Data = nCrcSeed;
        //*Data ^= nCrcSeed;
        //nCrcSeed = *Data;
        Data++;
    }
    pData = (char*)Data;
    for(short count = 0;count<byte_count;count++)
    {
        *pData ^= (char)nCrcSeed;
        pData++;
    }
}

char *Decompress(char *pData, unsigned short &nLength)
{
    
    unsigned short offset;
    if(pData[0] == 0x00)
     offset = 2;
    else
     offset = 1;
    z_stream packet;
    char output[CompBuf];
    unsigned short newLength=0;
    packet.zalloc = Z_NULL;
    packet.zfree = Z_NULL;
    packet.opaque = Z_NULL;
    packet.avail_in = 0;
    packet.next_in = Z_NULL;
    inflateInit(&packet);
    packet.next_in = (Bytef*)(pData+offset);
    packet.avail_in = (nLength - offset -3);
    packet.next_out = (Bytef*)output;
    packet.avail_out = CompBuf;
    inflate(&packet,Z_FINISH);
    newLength = static_cast<unsigned short>(packet.total_out);
    inflateEnd(&packet);
    char *Decomp_pData  = new char [newLength + offset + 3];
    char *begDecomp_pData = Decomp_pData;
    *Decomp_pData = pData[0];
    Decomp_pData++;
    if(offset == 2)
	{
	 *Decomp_pData = pData[1];
     Decomp_pData++;
	}
    for(short x=0;x<newLength;x++)
    {
        *Decomp_pData = output[x];
        Decomp_pData++;
    }
	*Decomp_pData = 0x01;
    Decomp_pData++;
	pData += (nLength-2);
    *(unsigned short*)Decomp_pData = *(unsigned short*)pData;
    Decomp_pData = begDecomp_pData;
    nLength = newLength + offset + 3;
    return Decomp_pData;

}

std::vector<char> Decompress(std::vector<char> pData)
{
    
    unsigned short offset;
    if(pData[0] == 0x00)
     offset = 2;
    else
     offset = 1;
    z_stream packet;
    char output[CompBuf];
    unsigned short newLength=0;
    packet.zalloc = Z_NULL;
    packet.zfree = Z_NULL;
    packet.opaque = Z_NULL;
    packet.avail_in = 0;
    packet.next_in = Z_NULL;
    inflateInit(&packet);
    packet.next_in = (Bytef*)(&pData[0]+offset);
    packet.avail_in = (pData.size() - offset -3);
    packet.next_out = (Bytef*)output;
    packet.avail_out = CompBuf;
    inflate(&packet,Z_FINISH);
    newLength = static_cast<unsigned short>(packet.total_out);
    inflateEnd(&packet);
    
    std::vector<char> tmpdata(newLength + offset + 3);

    char *Decomp_pData  = &tmpdata[0];// new char [newLength + offset + 3];
    char *begDecomp_pData = Decomp_pData;
    *Decomp_pData = pData[0];
    Decomp_pData++;
    if(offset == 2)
	{
	 *Decomp_pData = pData[1];
     Decomp_pData++;
	}
    for(short x=0;x<newLength;x++)
    {
        *Decomp_pData = output[x];
        Decomp_pData++;
    }
	*Decomp_pData = 0x01;
    Decomp_pData++;

    *(unsigned short*)Decomp_pData = *(reinterpret_cast<unsigned short*>(&pData[pData.size()-2]));
    Decomp_pData = begDecomp_pData;
    //nLength = newLength + offset + 3;
    return tmpdata;

}

