### Splay Data Compression

This software project was a final project for Stanford's CS166 , Spring 2016. The project was a collaboration between Collin Kincaid, Mitchell Douglass, and Ryan Hermstein.

The software project resulted in an (unpublished) [academic paper](https://github.com/mitrydoug/splay-compr/raw/master/paper/SplayDataCompression.pdf) and a [final presentation](https://github.com/mitrydoug/splay-compr/raw/master/paper/SplayDataCompressionPres.pdf). They provide the best description of the motivation and outcomes of this project, check them out!

#### Overview

This repository includes the following:

1. A basic set of classes for providing streaming of file/image data, allowing various data transformations to be chained in order to produce the encodings/decodings of (2).
2. A number of lossless data-compressing algorithms, complete with encoding and decoding. These include:
    * **Run-Length Coder**: Encode sequenses of identical bytes by a tuple `(c, b)`, where `c` is the number repeated occurrences and `b` is the byte value.
    * **Move-to-Front Coder**: Maintains a list of all byte values. Encodes a byte by cleverly indicating the index of its value in this list, then moves this byte value to the front of the list (thus, shortening the representation of its index in the list). The decode step follows an inverse procedure.
    * **Huffman-Coder**: The widely-popular and heavily-used Huffman code.
    * **Splay-Coder**: The main object of study in this project. Utilizes a modified splay tree data structure to encode data via a dynamic prefix code (see paper for details).
