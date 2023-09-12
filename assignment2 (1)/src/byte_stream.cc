#include "byte_stream.hh"

#include <algorithm>

// You will need to add private members to the class declaration in `byte_stream.hh`

/* Replace all the dummy definitions inside the methods in this file. */

#include <iostream>

using namespace std;

ByteStream::ByteStream(const size_t capa) : capacity(capa), inputended(false), writecounter(0), readcounter(0)
{}

size_t ByteStream::write(const string &data) {
    if (inputended ) {
        // Stream has ended or encountered an error, cannot write.
        return 0;
    }

    size_t space_available = remaining_capacity();
    size_t bytes_to_write_in_available_buffer = std::min( data.length() , space_available );

    // Calculate iterators for the range of data to insert into the deque.
    auto dataBegin = data.begin();
    auto dataEnd = dataBegin + bytes_to_write_in_available_buffer;

    // Insert the data into the deque.
    buffer.insert(buffer.end(), dataBegin, dataEnd);

    // Update the write counter.
    writecounter += bytes_to_write_in_available_buffer;

    return bytes_to_write_in_available_buffer;
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {

  size_t BYTES_AVAILABLE = buffer.size();
  size_t bytes_to_peek = std::min(len, BYTES_AVAILABLE);
  
  std::string result( buffer.begin(), buffer.begin() + bytes_to_peek );
  return result;
  }

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) {

    if (len > buffer.size()) {
        set_error();
        return;
    }
    
    size_t Bytes_to_remove_from_buffer = std::min(len, buffer.size());
    readcounter += Bytes_to_remove_from_buffer; // not sure on this
    buffer.erase(buffer.begin(), buffer.begin() + Bytes_to_remove_from_buffer);
    
}

//! Read (i.e., copy and then pop) the next "len" bytes of the stream
//! \param[in] len bytes will be popped and returned
//! \returns a string

std::string ByteStream::read(const size_t len) {

 if (len > buffer.size()) {
        set_error();
        return ""; // e.sg., an empty string
    }
    
    std::string result(buffer.begin(), buffer.begin() + len);

    // Erase the read data from the buffer.
    buffer.erase(buffer.begin(), buffer.begin() + len);

    readcounter += len;
    return result;

}

void ByteStream::end_input() { inputended = true;}

bool ByteStream::input_ended() const { return inputended;;}

size_t ByteStream::buffer_size() const {return buffer.size(); }

bool ByteStream::buffer_empty() const {return buffer.empty() ; } 

bool ByteStream::eof() const { return  inputended && buffer.empty(); }

size_t ByteStream::bytes_written() const { return writecounter; }

size_t ByteStream::bytes_read() const { return readcounter;  }

size_t ByteStream::remaining_capacity() const { return capacity - buffer.size(); }
