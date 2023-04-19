# cpp-comtrade

(Partial) C++ implementation of COMTRADE 1999 configuration and data file parsing.

This code was made using the following document:
    Identification: IEEE Std C37.111-1999
    Title: Common Format for Transient Data Exchange (COMTRADE) for Power Systems
    Revision: R2005
    Date: 02 Feb 2005
    URL: [https://ieeexplore.ieee.org/stamp/stamp.jsp?tp=&arnumber=798772](https://ieeexplore.ieee.org/document/798772)

This implementation parses most data from the configuration file and all analog data from a binary file. This implementation does not parse digital channel information, and it does not handle multiple different sampling rates.

I am not liable for issues arising from use of this code for personal, research, industrial, safety-critical, or any other applications. However, feel free to fork the code and make your own updates.
