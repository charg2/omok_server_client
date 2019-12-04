#ifndef C2_PACKET_H
#define C2_PACKET_H

#include <cstdint>
#include <type_traits>
#include <vcruntime_string.h>

#define Out

// peek 추가 해야함. 
// peek 추가 함. 

//namespace c2
//{
constexpr size_t kMaximumSegmentSize = 1460;

class Packet
{
public:
	Packet();
	Packet(const Packet& other);
	Packet(Packet&& other) noexcept;
	~Packet();

	Packet& operator=(const Packet& other);
	Packet& operator=(Packet&& other) noexcept;

	void clear();

	char* getBufferPtr()		const noexcept;
	char* getWriteBufferPtr()	const noexcept;
	char* getReadBufferPtr()	const noexcept;

	void rewind()				noexcept;
	void reset()				noexcept;
	void resize(size_t length);


	size_t capacity() const noexcept;
	size_t size() const noexcept;

	void moveWriteHead(size_t length);
	void moveReadHead(size_t length);

	void write(const void* src, size_t size);
	void read(Out void* src, size_t size);
	void peek(Out void* src, size_t size);

	//// primitive types version 
	//template<typename T>
	//Packet& operator<<(const T& src);
	////template<typename T>
	////Packet& operator<<(T&& src);
	//template<typename T>
	//Packet& operator>>(Out T& dest);


private:
	char*	buffer;
	size_t	payload_capacity;

	size_t	write_head;
	size_t	read_head;
};
//}
#endif
