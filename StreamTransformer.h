#ifndef StreamTransformer_Included
#define StreamTransformer_Included

#include <iostream>
#include <sstream>
#include <initializer_list>

/* A typedef for representing an abstract 8-bit alphabet character. */
typedef unsigned char byte;

/* A typedef for representing single bits */
typedef bool bit;

typedef struct TransformDigest {
    TransformDigest()
      : bytesIn(0), bytesOut(0) {}
    size_t bytesIn;
    size_t bytesOut;
    friend std::ostream& operator<<(std::ostream& out, 
            struct TransformDigest &dg);
    friend std::ostream& operator<<(std::ostream& out, 
            struct TransformDigest &&dg);
} TransformDigest;

/* Abstract Class: StreamTransformer
 * ---------------------------------
 * Describes abstract interface for a class that performs a transformation on
 * a stream of bytes.
 */
class StreamTransformer {
  protected:

    /* Constructs a StreamTransformer with an input and output source */
    StreamTransformer()
      : input(nullptr),
        output(nullptr),
        readBits(0),
        writeBits(0),
        readByteBuf(0),
        writeByteBuf(0) {/* empty constructor */}

    /* Helper functions for the transform() function, to access the input and
     * output stream in a homogenious way
     */

    /* Read a byte from the input stream */
    bool readByte(byte &by);

    /* Read a bit from the input stream. Note: when called, it is the
     * responsibility of code reading the stream to read an entire byte in
     * bits, or otherwise to call dropByte() to discard remaining bits.
     */
    bool readBit(bit &bi);

    /* Drops the remainder of a byte that has been partially read by readBit()
     */
    void dropByte();

    /* Write a byte to the output stream. */
    void writeByte(byte by);

    /* Write a bit to the output stream. Note: when called, it is the
     * responsibility of code writting the stream to write an entire byte in
     * bits, or otherwise to call flushByte().
     */
    void writeBit(bit bi);

    /* Flushes 0 bits as the remainder of the byte being written via calls to
     * writeBit()
     */
    void flushByte();

  private:

    /* Bytes will be read from the input stream, eventually to be deposited to
     * the output stream after transformation
     */
    std::istream *input;
    std::ostream *output;

    /* Determines if a partial byte is being read by readBit() */
    size_t readBits;

    /* Determines if a partial byte is being written by writeBit() */
    size_t writeBits;

    /* Storage buffer for readBit() */
    byte readByteBuf;

    /* Storage buffer for writeBit() */
    byte writeByteBuf;

    /* Function used by connect() to send stream data through this transformer
     * and back into the output stream.
     */
    void pipe(std::istream &is, std::ostream &os);

    /* A list of information about any particular stream transformation. */
    TransformDigest digest;

  public:

    /* The main interface of the stream transformer. This function is
     * responsible for using the read/write bit/byte functions below to perform
     * the desired transformation on the byte stream.
     *
     * This is an abstract function, meaning it has no implementation in the
     * StreamTransformer class. Instead, this function must be overriden by
     * deriving classes. (it's like an interface or abstract class in Java).
     *
     * The "=0;" is C++ syntax that enforces that this function cannot be
     * defined in StreamTransformer, so then must be transformed by deriving
     * classes. 
     */
    virtual void transform()=0;

    TransformDigest getDigest();

    template<typename... Args>
    friend void connect(std::istream &is, std::ostream &os, StreamTransformer* st,
                        Args... sts) {
        std::stringstream ss;
        st->pipe(is, ss);
        connect(ss, os, sts...); 
    }

    friend void connect(std::istream &is, std::ostream &os, StreamTransformer* st) {
        st->pipe(is, os);
    }

};

#endif
