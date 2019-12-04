#include <windows.h>

#include "Packet.h"
#include "exception.h"



//namespace c2
//{
Packet::Packet() : buffer{ nullptr }, payload_capacity{ kMaximumSegmentSize }, write_head{ 0 }, read_head{ 0 }
{
	buffer = new char[kMaximumSegmentSize];
}

Packet::Packet(const Packet& other) : buffer{ other.buffer }, payload_capacity{ other.payload_capacity }, write_head{ other.write_head }, read_head{ other.read_head }
{
}

Packet::Packet(Packet&& other) noexcept : buffer{ other.buffer }, payload_capacity{ other.payload_capacity }, write_head{ other.write_head }, read_head{ other.read_head }
{
#ifdef PROFILE_ON
	PROFILE_FUNC;
#endif
	other.buffer = nullptr;
}

Packet& Packet::operator=(const Packet& other)
{
#ifdef PROFILE_ON
	PROFILE_FUNC;
#endif
	if (this == &other)
		return *this;

	this->buffer = other.buffer;
	this->payload_capacity = other.payload_capacity;
	this->write_head = other.write_head;
	this->read_head = other.read_head;

	//*this->ref_count += 1;

	return *this;
}


Packet& Packet::operator=(Packet&& other) noexcept
{
#ifdef PROFILE_ON
	PROFILE_FUNC;
#endif
	if (this == &other)
		return *this;

	this->buffer = other.buffer;
	this->payload_capacity = other.payload_capacity;
	this->write_head = other.write_head;
	this->read_head = other.read_head;
	//this->ref_count = other.ref_count;

	//memset(this ,0, sizeof(Packet));
	other.buffer = nullptr;
	other.payload_capacity = 0;
	other.write_head = 0;
	other.read_head = 0;
	//other.ref_count = nullptr;

	return *this;
}

Packet::~Packet()
{
#ifdef PROFILE_ON
	PROFILE_FUNC;
#endif
	//release();
	delete[] buffer;

	read_head = 0;
	write_head = 0;
}

void Packet::clear(void)
{
#ifdef PROFILE_ON
	PROFILE_FUNC;
#endif
	write_head = 0;
	read_head = 0;
}

char* Packet::getWriteBufferPtr() const noexcept
{
	return (char*)&buffer[this->write_head];
}

char* Packet::getReadBufferPtr() const noexcept
{
	return (char*)&buffer[this->read_head];
}

char* Packet::getBufferPtr() const noexcept
{
	return buffer;
}

void Packet::rewind() noexcept
{
	this->read_head = 0;
}


void Packet::resize(size_t length)
{
	char* new_buffer = new char[length] {};
	size_t new_capacity = length > payload_capacity ? payload_capacity : length;

	memcpy(new_buffer, this->buffer, new_capacity);

	delete[] this->buffer;

	this->buffer = new_buffer;
	this->payload_capacity = length;
	this->write_head = length > this->write_head ? this->write_head : length;
	this->read_head = length > this->read_head ? this->read_head : length;
}

void Packet::reset() noexcept
{
	this->read_head = 0;
	this->write_head = 0;
}

size_t Packet::capacity() const noexcept
{
#ifdef PROFILE_ON
	PROFILE_FUNC;
#endif
	return payload_capacity;
}

size_t Packet::size() const noexcept
{
#ifdef PROFILE_ON
	PROFILE_FUNC;
#endif
	return write_head-read_head;
}

void Packet::moveWriteHead(size_t length)
{
	write_head += length;
}

inline void Packet::moveReadHead(size_t length)
{
	read_head += length;
}


void Packet::write(const void* data, size_t size)
{
#ifdef PROFILE_ON
	PROFILE_FUNC;
#endif
	if (write_head + size >= payload_capacity) // idx니깐 같아져도 문제가 생김.
	{
		size_t new_payload_capacity = payload_capacity * 2;
		size_t new_size = write_head + size > new_payload_capacity ? write_head + size : new_payload_capacity;

		resize(new_size);
	}

	// memcpy 때리는거보다.
	switch (size)
	{
	case 1:
		this->buffer[write_head] = *(uint8_t*)data;
		break;
	case 2:
		*(uint16_t*)(&this->buffer[write_head]) = *(uint16_t*)data;
		break;

	case 3:
		memcpy(&buffer[write_head], data, size); // 현재 인덱스에 복사를 함.
		break;

	case 4:
		*(uint32_t*)(&this->buffer[write_head]) = *(uint32_t*)data;
		break;

	case 5:
	case 6:
	case 7:
		memcpy(&buffer[write_head], data, size); // 현재 인덱스에 복사를 함.
		break;

	case 8:
		*(uint64_t*)(&this->buffer[write_head]) = *(uint64_t*)data;
		break;

	default: // 8 초과하는 데이터를이렇게 받는다.
		memcpy(&buffer[write_head], data, size); // 현재 인덱스에 복사를 함.
		break;
	}
	write_head += size;
}

void Packet::read(Out void* data, size_t size)
{
	//#ifdef PROFILE_ON
	//		PROFILE_FUNC;
	//#endif
	if (read_head + size > write_head) // idx니깐 같아져도 문제가 생김.
	{
		// 이건 무족너 문제 있는 것.
		// error handling... 
		c2::util::crash();
	}


	// 1 2 4  8 바이트 읽기에 대해서도 최적화를 해준다.
	switch (size)
	{
	case 1:
		*(uint8_t*)data = *(uint8_t*)&this->buffer[read_head];
		break;

	case 2:
		*(uint16_t*)data = *(uint16_t*)&this->buffer[read_head];
		break;

	case 3:
		memcpy(data, &buffer[read_head], size);
		break;

	case 4:
		*(uint32_t*)data = *(uint32_t*)&this->buffer[read_head];
		break;

	case 5:
	case 6:
	case 7:
		memcpy(data, &buffer[read_head], size);
		break;

	case 8:
		*(uint64_t*)data = *(uint64_t*)&this->buffer[read_head];
		break;

	default: // 8 초과하는 데이터를이렇게 받는다.
		//memcpy(&buffer[write_head], data, size); // 현재 인덱스에 복사를 함.
		memcpy(data, &buffer[read_head], size);
		break;
	}

	read_head += size;
}

void Packet::peek(Out void* data, size_t size)
{
	if (read_head + size > write_head)
	{
		c2::util::crash();
	}

	// 1 2 4  8 바이트 읽기에 대해서도 최적화를 해준다.
	switch (size)
	{
	case 1:
		*(uint8_t*)data = *(uint8_t*)&this->buffer[read_head];
		break;

	case 2:
		*(uint16_t*)data = *(uint16_t*)&this->buffer[read_head];
		break;

	case 3:
		memcpy(data, &buffer[read_head], size);
		break;

	case 4:
		*(uint32_t*)data = *(uint32_t*)&this->buffer[read_head];
		break;

	case 5:
	case 6:
	case 7:
		memcpy(data, &buffer[read_head], size);
		break;

	case 8:
		*(uint64_t*)data = *(uint64_t*)&this->buffer[read_head];
		break;

	default:
		memcpy(data, &buffer[read_head], size);
		break;
	}
}


//// primitive types version 
//template<typename T>
//Packet& Packet::operator<<(const T& src)
//{
//	static_assert(std::is_arithmetic<T>::value || std::is_enum<T>::value, "Generic Write only supports primitive data types"); // is_arithmetic<> int or float
//
//	write(&src, sizeof(T));
//
//	return *this;
//}


//template<typename T>
//Packet& Packet::operator>>(Out T& dest)
//{
//	static_assert(std::is_arithmetic<T>::value || std::is_enum<T>::value, "Generic Write only supports primitive data types"); // is_arithmetic<> int or float
//
//	read(&dest, sizeof(T));
//
//	return *this;
//};
//
