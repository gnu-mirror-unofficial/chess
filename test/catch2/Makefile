# Makefile for gnuchess/src.
#
# Copyright (C) 2001-2020 Free Software Foundation, Inc.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

TARGET = test_cmd test_pgn
LIBS =
CC = g++
CPPFLAGS = -I../../src/ -I../../lib/
CXXFLAGS = -g -Wall -std=c++11 -DLOCALEDIR=\".\"
LDFLAGS = -std=c++11

.PHONY: default all clean test

default: $(TARGET)
all: default test

OBJECTS = $(patsubst %.cpp, %.o, $(wildcard *.cpp))
HEADERS = $(wildcard *.h)

%.o: %.cc %.cpp $(HEADERS)
	$(CC) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

#$(TARGET): $(OBJECTS)
	#$(CC) $(OBJECTS) -Wall $(LDFLAGS) -o $@ $(LIBS)

test_cmd: test_cmd.o test_gnuchess.o stub_main.o stub_debug.o stub_engine.o stub_epd.o stub_genmove.o stub_init.o stub_lexpgn.o stub_move.o stub_output.o stub_pgn.o stub_players.o stub_solve.o stub_util.o
	$(CC) test_cmd.o test_gnuchess.o stub_main.o stub_debug.o stub_engine.o stub_epd.o stub_genmove.o stub_init.o stub_lexpgn.o stub_move.o stub_output.o stub_pgn.o stub_players.o stub_solve.o stub_util.o -Wall $(LDFLAGS) -o $@ $(LIBS)

test_pgn: test_pgn.o test_gnuchess.o stub_main.o stub_init.o stub_lexpgn.o stub_output.o
	$(CC) test_pgn.o test_gnuchess.o stub_main.o stub_init.o stub_lexpgn.o stub_output.o -Wall $(LDFLAGS) -o $@ $(LIBS)

clean:
	-rm -f *.o
	-rm -f $(TARGET)

test:
	./test_cmd
	./test_pgn
