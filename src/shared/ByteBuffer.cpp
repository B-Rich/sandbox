/*
 * libgalaxy - ByteBuffer.cpp
 *
 * @copyright (c) 2008 Spira Studios LLC
 * @author Eric S. Barr Jr. <eric.barr@ericscottbarr.com>
 */

#include <algorithm>
#include <iomanip>
#include <iostream>
 
#include "ByteBuffer.h"

ByteBuffer::ByteBuffer()
	: read_position_(0)
	, write_position_(0)
	{}

ByteBuffer::ByteBuffer(size_t length)
	: data_(length)
	, read_position_(0)
	, write_position_(0)
	{}
	
ByteBuffer::ByteBuffer(std::vector<unsigned char>& data)
	: data_(data.begin(), data.end())
	, read_position_(0)
	, write_position_(data.size())
	{}

ByteBuffer::ByteBuffer(const unsigned char* data, size_t length)
	: data_(data, data+length)
	, read_position_(0)
	, write_position_(length)
	{}

ByteBuffer::~ByteBuffer() {}

ByteBuffer::ByteBuffer(const ByteBuffer& from)
	: data_(from.data_.begin(), from.data_.end())
	, read_position_(0)
	, write_position_(from.size())
	{}

ByteBuffer& ByteBuffer::operator= (const ByteBuffer& from)
{
	ByteBuffer temp(from);
	swap(temp);

	return *this;
}

void ByteBuffer::swap(ByteBuffer& from)
{
	std::swap(data_, from.data_);
	std::swap(read_position_, from.read_position_);
	std::swap(write_position_, from.write_position_);
}

void ByteBuffer::append(const ByteBuffer& from)
{
    write(from.data(), from.size());
}

void ByteBuffer::reserve(size_t length)
{
  data_.reserve(length);
}

size_t ByteBuffer::size() const
{
	return data_.size();
}

size_t ByteBuffer::capacity() const
{
	return data_.capacity();
}

void ByteBuffer::write(const unsigned char* data, size_t size)
{
	data_.insert(data_.begin() + write_position_, data, data + size);
	write_position_ += size;
}

void ByteBuffer::write(size_t offset, const unsigned char* data, size_t size)
{
	data_.insert(
        data_.erase(data_.begin() + offset, data_.begin() + offset + size),
        data, 
        data + size);
}

void ByteBuffer::clear()
{
	data_.clear();
}

size_t ByteBuffer::readPosition() const
{
    return read_position_;
}

void   ByteBuffer::readPosition(size_t position)
{
    read_position_ = position;
}

size_t ByteBuffer::writePosition() const
{
    return write_position_;
}

void   ByteBuffer::writePosition(size_t position)
{
    write_position_ = position;
}

const unsigned char* ByteBuffer::data() const
{
	return &data_[0];
}

std::vector<unsigned char>& ByteBuffer::raw()
{
    return data_;
}

template<> void ByteBuffer::swapEndian(uint16_t& data)
{
	data = (data >> 8) |
		   (data << 8);
}

template<> void ByteBuffer::swapEndian(uint32_t& data)
{
	data = (data  >> 24) |
	       ((data << 8) && 0x00FF0000) |
	       ((data >> 8) && 0x0000FF00) |
	       (data  << 24);
}

template<> void ByteBuffer::swapEndian(uint64_t& data)
{
	data = (data  >> 56) |
#ifdef _WIN32
		   ((data << 40) && 0x00FF000000000000) |
		   ((data << 24) && 0x0000FF0000000000) |
		   ((data << 8)  && 0x000000FF00000000) |
		   ((data >> 8)  && 0x00000000FF000000) |
		   ((data >> 24) && 0x0000000000FF0000) |
		   ((data >> 40) && 0x000000000000FF00) |
#else
		   ((data << 40) && 0x00FF000000000000LLU) |
		   ((data << 24) && 0x0000FF0000000000LLU) |
		   ((data << 8)  && 0x000000FF00000000LLU) |
		   ((data >> 8)  && 0x00000000FF000000LLU) |
		   ((data >> 24) && 0x0000000000FF0000LLU) |
		   ((data >> 40) && 0x000000000000FF00LLU) |
#endif
	       (data  << 56);
}

template<> ByteBuffer& ByteBuffer::write<std::string>(std::string data)
{
	write<uint16_t>(static_cast<uint16_t>(data.length()));
	write(reinterpret_cast<const unsigned char*>(data.c_str()), data.length());

	return *this;
}

template<> const std::string ByteBuffer::read<std::string>(bool do_swap_endian)
{
	uint16_t length = read<uint16_t>();

	if (data_.size() < read_position_ + length) {
		throw std::out_of_range("Read past end of buffer");
	}

	std::string data(data_.begin() + read_position_, data_.begin() + read_position_ + length);
	read_position_ += length;

	return data;
}

template<> ByteBuffer& ByteBuffer::write<std::wstring>(std::wstring data)
{
	uint32_t length = data.length();

	write<uint32_t>(length);

	if (data_.size() < write_position_ + length * 2)	{
		data_.resize(write_position_ + length * 2);
	}
	
	for (size_t i = 0; i < length; ++i)
		*reinterpret_cast<wchar_t*>(&data_[write_position_+(i*2)]) = static_cast<uint8_t>(data[i]);

	write_position_ += length * 2;

	return *this;
}

template<> const std::wstring ByteBuffer::read<std::wstring>(bool do_swap_endian)
{
	uint32_t length = read<uint32_t>();

	if (data_.size() < read_position_ + (length * 2)) {
		throw std::out_of_range("Read past end of buffer");
	}

    wchar_t* string_data = reinterpret_cast<wchar_t*>(&data_[read_position_]);

	std::wstring data(string_data, string_data+length);
	read_position_ += length * 2;

	return data;
}

std::ostream& operator<<(std::ostream& message, const ByteBuffer& buffer)
{
	size_t length = buffer.size();
	const unsigned char* data = buffer.data();

	// Calculate the number of lines and extra bits.
	short lines = (short)(length / 16);
	short extra = (short)(length % 16);

	// Save the formatting state of the stream.
	std::ios_base::fmtflags flags = message.flags(message.hex);
	char fill = message.fill('0');
	int width = message.width(2);

	// The byte buffer should be printed out in lines of 16 characters and display both
	// hex and ascii values for each character, see most hex editors for reference.
	for (short i = 0; i <= lines; i++) {
		// Print out a line number.
		message << std::setw(4) << (i * 16) << ":   ";

		// Loop through the characters of this line (max 16)
		for (short j = 0; j < 16; ++j) {
			// For the last line there may not be 16 characters. In this case filler
			// whitespace should be added to keep column widths consistent.
			if (i == lines && j >= extra) {
				message << "   ";
			} else {
				message << std::setw(2) << static_cast<unsigned>(data[(i * 16)+j]) << " ";
			}
		}

		message << "  ";

		for (short k = 0; k < 16; ++k) {
			// For the last line there may not be 16 characters. In this case
			// print a ' ' for these characters.
			if ((i == lines) & (k >= extra)) {
				message << " ";
			} else if (data[(i * 16)+k] < ' ' || data[(i * 16)+k] > '~') {
				// Else if it's not an ascii value print a '.'
				message << '.';
			} else {
				message << data[(i * 16)+k];
			}
		}
	    
        message << std::endl;
	}

	message << std::endl;

	// Return formatting of stream to its previous state.
	message.flags(flags);
	message.fill(fill);
	message.width(width);

	return message;
}

