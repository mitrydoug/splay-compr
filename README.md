## Splay Data Compression

This software project was a final project for Stanford's CS166 , Spring 2016. The project was a collaboration between Collin Kincaid, Mitchell Douglass, and Ryan Hermstein.

The software project resulted in an (unpublished) [academic paper](https://github.com/mitrydoug/splay-compr/raw/master/paper/SplayDataCompression.pdf) and a [final presentation](https://github.com/mitrydoug/splay-compr/raw/master/paper/SplayDataCompressionPres.pdf). They provide the best description of the motivation and outcomes of this project, check them out!

**Acknowledgments**: A special thanks to Keith Schwarz and the TAs of CS166 for the well-planned and well-taught lectures on this material, as well as for providing the motivation to take on a project like this one.

### Overview

This repository includes the following:

1. A basic set of classes for providing streaming of file/image data, allowing various data transformations to be chained in order to produce the coders of (2).
2. A number of lossless data-compressing algorithms, complete with encoding and decoding algorithms. These include:
    * **Run-Length Coder**: Encode sequenses of identical bytes by a tuple `(c, b)`, where `c` is the number repeated occurrences and `b` is the byte value.
    * **Move-to-Front Coder**: Maintains a list of all byte values. Encodes a byte by cleverly indicating the index of its value in this list, then moves this byte value to the front of the list (thus, shortening the representation of its index in the list). The decode step follows an inverse procedure.
    * **Huffman-Coder**: The widely-popular and heavily-used Huffman code.
    * **Splay-Coder**: The main object of study in this project. Utilizes a modified splay tree data structure to encode data via a dynamic prefix code (see paper for details).
3. A collection of data files, including text files and image files, used to verify correctness and compare performance of the coders of (2), as well as a script for automatically measuring coder performance on each file.

#### The Huffman Code

[Huffman coding](https://en.wikipedia.org/wiki/Huffman_coding) is a very popular data compression scheme that is particularly good at removing redundancy brought about by non-uniform character frequencies within a data source. For instance, suppose you have a data source composed of 50% `'A'`s, 25% `'B'`s, 15% `'C'`s, and 10% `'D'`s, yet the relative positions of these characters is completely random. In this case, a Huffman code is actually an optimal code, and will be able to encode your data source with a number of bits in accordance with its [Shannon Entropy](https://en.wikipedia.org/wiki/Entropy_(information_theory)), approx 1.74 bits/byte.

Unfortunately, Huffman coding cannot seek out and eliminate any other kind of redundancy in data. For instance, given an alphabet of `'A'-'Z'`, a Huffman coder would not recognize the redundancy in `"ABC...XYZABC...XYZ"`. The reality with many forms of data is that there indeed is much redundancy beyond character frequencies. Indeed, often the probability of a particular character appearing next in a data stream depends heavily on context, particularly the previous and next characters in the stream (can you fill in the hole in q\_ick?).

#### An Abstract Framework

In our project we explore an abstract framework for codes, one that generalizes some of the crucial machinery of Huffman codes. In particular, our framework shows how any deterministic, possibly dynamic tree datastructure can be directly used to produce invertible codes. The *dynamic* part here is the crucial extension to Huffman codes that allows us to break through the Shannon Entropy ceiling of data. We consider using splay trees within our framework to produce a "Splay Coder" (this is not an original idea, see [2]). See [our paper](https://github.com/mitrydoug/splay-compr/raw/master/paper/SplayDataCompression.pdf) for details.

### Project Motivation & Goals

#### The Splay Tree

In CS166, we learned many surprising properties about Splay Trees. Here I'll list the names of three theorems about splay trees and describe the implications for our Splay Coder.

1. **Balance Theorem**: This theorem implies that a Splay Coder encoding ***m*** characters from an ***n***-character alphabet has runtime  ***O(m log n + n log n)*** or simply ***O(m log n)*** if ***n = O(m)***. This implies two things about Splay Coders. First, it ties the runtime of the Splay Coder to the size of the encoding output by the Splay Coder. Secondly, it bounds both the runtime and the size of an encoding of a Splay Coder by ***O(m log n)***, which is ***O(m)*** if ***n*** is a constant, such as the number of distinct byte values.
2. **Static Optimality Theorem**: This theorem implies that a Splay Coder will perform within a constant factor of a Huffman Coder, that is, a Splay Coder achieves an average ammortized ***O(1 + H)*** bits/byte, where ***H*** is the Shannon Entropy of the data being encoded.
3. **Working Set Theorem**: This theorem reveals the true power of the splay tree. It says that a Splay Coder adapts to "working sets" in the data (i.e. relatively long sequences of characters from a strict subset of all possible characters), and behaves like an optimal Huffman Coder for each working set in the data. For instance, if a Splay Encoder encounters a sufficiently long working set composed of only of 16 unique characters, it will encode this working set with at most 4 bits/byte ammortized, regardless of the nature of data outside of the working set.

It would seem like we might best leverage the properties of a Splay Coder by feeding it data with a high tendancy to exhibit these "working sets". We identified image data as a likely candidate for experiencing reasonably-high compression when encoded by a Splay Coder.

#### Space-Filling Curves

One of the major contributions of this work comes from our choice of algorithm for enumerating the pixels of a source image. A traditional method for enumerating pixels within an image is called "row-order" enumeration, where each row is enumerated left-to-right and top-to-bottom. While adjacent pixels in a row-order enumeration are highly likely to be adjacent in the source image, row-order enumerations still have the unfortunate property that pixels at distance ***k*** in the enumeration can be a distance ***k*** in the source image.

Enter the [Hilbert Curve](https://en.wikipedia.org/wiki/Hilbert_curve). It is what is known as a "space-filling curve", and essentially that this means is that we can use it to derive an enumeration of the pixels of an image that is different from a row-order enumeration, ordering pixels by the order in which the Hilbert Curve visits the space they occupy. By looking at the pattern of Hilbert Curve approximations, one can see that it generally visits an entire region of space before moving on to the next. This is formalized by an important property of the Hilbert Curve called *Holder Continuity*, which tells us that pixels at a distance of ***k*** in the Hilbert enumeration are at a distance of at most ***sqrt(6k)*** in the source image. For ***k=100***, this is the difference between ***k=100*** and ***sqrt(6k) = 24***.

### Results

Here is a summary of the results from the experiments that we ran. A brief description of each of the files used:

* **dictionary**: This file is a large list of words in the English language. Due to the alphabetical ordering of words, it simulates a small degree of clustering for the benefit of splay and move-to-front encoding. 
* **Hamlet**: This file is a large corpus of English-language text, where all characters in the English alphabet (as well as some punctuation symbols, numbers, etc.) appear. It serves as our typical-use text file exemplar.
* **google**: This file is the source code of a webpage. It extends the relevant alphabet in our tree from standard text characters (alphanumeric, punctuation, etc.) to almost all ASCII characters.
* **random**: This image is composed of random pixels from a uniform distribution. It shows how compressors do when there is no trend in the data.
* **expRandom**: This image is 1/2 a random pixel, 1/4 of a different random pixel, 1/8 of another, and so forth. The pixels are randomly distributed throughout the image. It shows how compressors do with high frequency characters randomly distributed throughout the image. 
* **ryan**: This image is a picture of a human face. It is included to show how these compressors would do on an everyday image.
* **fern**: This image is a picture of a fern and shows how these compressors would do on a nature image, which is another popular type of image.
* **city**: This is a high resolution picture of a city. It shows how these compressors do on large images. 
* **dorm**: This is a picture of a simple dorm room. It shows how these compressors do on an image without much variation.

#### Experiment 1

The first experiment compares Huffman, Move-To-Front, and Splay encoding on the aforementioned text files. Initially, we expect Huffman to perform best on these tests. The purpose of our splay encoding scheme is to exploit locality benefits in our data sets, but we do not expect to see this effect in text files. Nonetheless, for the sake of completeness, we run the algorithms on text corpora and obtain the following results:

+--------------------+--------+------------------+-----------+
| Text File          | Method | Size Compr. (KB) | Bits/Byte |
+====================+========+==================+===========+
| <b>hamlet</b>\     | Huff\  | 105\             | (4.38)\   |
| (192 KB)\          | MTF\   | 180\             | (7.50)\   |
| H = 4.37           | Splay  | 126              | (5.25)    |
+--------------------+--------+------------------+-----------+
| <b>dictionary</b>\ | Huff\  | 144\             | (4.25)\   |
| (271 KB)\          | MTF\   | 215\             | (6.35)\   |
| H = 4.24           | Splay  | 153              | (4.52)    |
+--------------------+--------+------------------+-----------+
| <b>google</b>\     | Huff\  | 139\             | (5.70)\   |
| (195 KB)\          | MTF\   | 229\             | (9.39)\   |
| H = 5.57           | Splay  | 150              | (6.13)    |
+--------------------+--------+------------------+-----------+

**H** refers to the Shannon Entropy of the source file. Note that the Huffman coder approaches the Shannon Entropy quite nicely, as expected. Text files do not exhibit the "working set" property that would allow the Move-to-Front or Splay Coders to outperform the Huffman Coder.

#### Experiement 2

The second experiment compares Huffman, Move-To-Front, and Splay encoding on image files enumerated in row-order (**without** Hilbert Curve).

+-------------------+--------+------------------+-----------+
| Image File        | Method | Size Compr. (KB) | Bits/Byte |
+===================+========+==================+===========+
| <b>random</b>\    | Huff\  | 750\             | (8.00)\   |
| (750 KB)\         | MTF\   | 1344\            | (14.3)\   |
| H = 8.00          | Splay  | 906              | (9.66)    |
+-------------------+--------+------------------+-----------+
| <b>expRandom</b>\ | Huff\  | 187\             | (1.99)\   |
| (750 KB)\         | MTF\   | 261\             | (2.78)\   |
| H = 1.989         | Splay  | 216              | (2.30)    |
+-------------------+--------+------------------+-----------+
| <b>ryan</b>\      | Huff\  | 253\             | (7.50)\   |
| (270 KB)\         | MTF\   | 265\             | (7.85)\   |
| H = 7.468         | Splay  | 202              | (5.99)    |
+-------------------+--------+------------------+-----------+
| <b>fern</b>\      | Huff\  | 4779\            | (7.47)\   |
| (5117 KB)\        | MTF\   | 5574\            | (8.71)\   |
| H = 7.447         | Splay  | 4267             | (6.67)    |
+-------------------+--------+------------------+-----------+
| <b>city</b>\      | Huff\  | 5614\            | (7.50)\   |
| (5989 KB)\        | MTF\   | 5176\            | (6.91)\   |
| H = 7.467         | Splay  | 3806             | (5.08)    |
+-------------------+--------+------------------+-----------+
| <b>dorm</b>\      | Huff\  | 500\             | (7.59)\   |
| (527 KB)\         | MTF\   | 480\             | (7.29)\   |
| H = 7.564         | Splay  | 372              | (5.65)    |
+-------------------+--------+------------------+-----------+

Again, we notice that the Huffman Coder encodes at a compression ratio given by the Shannon Entropy of the source files. Note that Huffman performs better than its competitors on **random** and **expRandom** because these files exhibit only the kind of redundancy that a Huffman Coder is designed to remove. More importantly, notice how the Splay Coder outperforms Huffman on every other image, and the Move-to-Front outperforms Huffman on both **city** and **dorm**.

#### Experiment 3

Finally, we compare Huffman, Move-To-Front, and Splay encoding on image files enumerated via the Hilbert Curve. Recall that the Hilbert Curve enumerates pixels in such a way that close pixels in the source image are likely to be close in the enumeration. This gives the stream of image data a "working set" property, since images tend to have regions of pixels of nearly homogeneous color and intensity.

+--------------+--------+-------------+-----------+
| Image File   | Method | Size Compr. | Bits/Byte |
+==============+========+=============+===========+
| <b>ryan</b>\ | Huff\  | 253 KB\     | (7.50)\   |
| (270 KB)\    | MTF\   | 188 KB\     | (5.57)\   |
| H = 7.468    | Splay  | 152 KB      | (4.50)\   |
+--------------+--------+-------------+-----------+
| <b>fern</b>\ | Huff\  | 4779 KB\    | (7.47)\   |
| (5117 KB)\   | MTF\   | 4431 KB\    | (6.93)\   |
| H = 7.447    | Splay  | 3347 KB     | (5.23)\   |
+--------------+--------+-------------+-----------+
| <b>city</b>\ | Huff\  | 5614 KB\    | (7.50)\   |
| (5989 KB)\   | MTF\   | 4682 KB\    | (6.25)\   |
| H = 7.467    | Splay\ | 3427 KB     | (4.58)\   |
+--------------+--------+-------------+-----------+
| <b>dorm</b>\ | Huff\  | 500 KB\     | (7.59)\   |
| (527 KB)\    | MTF\   | 320 KB\     | (4.86)\   |
| H = 7.564    | Splay\ | 269 KB      | (4.08)\   |
+--------------+--------+-------------+-----------+

Here we see the true power of this combination of Splay Coder with the Hilbert-Curve. On average, the Splay Coder with Hilbert Curve pixel enumeration achieved compression sizes 61% that of the Huffman Coder, and 79% that of the Splay Coder without Hilbert Curve pixel enumeration.
