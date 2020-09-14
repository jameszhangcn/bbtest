#!/bin/bash
g++ -c sctpserver.cpp -o sctpserver.o
g++ -c cwrap.cpp -o cwrap.o
ar rcs sctpserver.a sctpserver.o cwrap.o
