# Infrastructure Rundown


### Makefile
The Makefile in our project is modelled after those used in our pset CS166 projects. Suppose you add two new files

- **PigLatinEncoder.h**
- **PigLatinEncoder.cpp**

Then you just need to do into the Makefile and add **PigLatinEncoder.o** to the OBJECTS variable so that it gets built into the rest of the project.

### StreamTransformers
This is an abstraction that allows a class to accept and input stream and an output stream, and then pass the stream data along while at the same time performing some transformation/processing on the data. The important parts of this interface that you'll use are the following functions of the **StreamTransformer** class

- **readByte/readBit**: reads a byte/bit from the input stream.
- **writeByte/writeBit**: writes to the output stream.

By extending StreamTransformer, it's super easy to make one of these. See **VerbatimTransformer**, **VerbatimBitTransformer**, **ReverseTransformer**, **PrefixTransformer**.

We'll need this abstraction more when we're reading image files (using space filling curves).

### StreamEncoders
A slight extension to the StreamTransformer idea. These have two possible methods for transforming the stream: **ENCODE** and **DECODE**. See **AlphaRunLengthEncoder** for a good example as to how one of these might work. Our data compressors will be modelled after this StreamEncoder interface.

### Connecting StreamTransformers
There's a really sweet way to hook these transformers together. See **Main.cpp** for two great examples. Essentially, the basic framework is

```cpp
ifstream is("inputfile.txt"); // could be any input stream, like cin for instance
ofstream os("outputfile.txt");
CoolTransformer ct;
AwesomeTransformer at;
connect(is, os, &ct, &at); // Can have as many Transformers in here as you like; here there's only 2 (&ct and &at)
```
