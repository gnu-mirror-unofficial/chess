#include "catch.hpp"

#include "frontend/common.h"
#include "frontend/pgn.cc"

extern bool mock_ShowBoard;

TEST_CASE("A game can be read from a file in PGN format", "[PGNReadFromFile]") {

    // void PGNReadFromFile (const char *file, int showheading)

    mock_ShowBoard = false;

    SECTION("Board shown if the PGN file exists") {
        const char test_pgn_filename[] = "../data/Polgar-Karpov.pgn";
        int showheading = 0;
        PGNReadFromFile(test_pgn_filename, showheading);
        REQUIRE( mock_ShowBoard == true );
    }

    SECTION("Board not shown if the PGN file does not exist") {
        //const char test_pgn_filename[] = ".tmp.pgn";
        const char test_pgn_filename[] = "../data/Polgar-Karpov.pgn-";
        int showheading = 0;
        PGNReadFromFile(test_pgn_filename, showheading);
        REQUIRE( mock_ShowBoard == false );
    }
}

TEST_CASE("A game can be saved to a file in PGN format", "[PGNSaveToFile]") {

    SECTION("TBD") {
        const char test_pgn_filename[] = ".tmp.pgn";
        char resultstr[1024]=""; 
        //PGNSaveToFile(test_pgn_filename, resultstr);
        //REQUIRE( strcmp(token[0], "one two three four") == 0 ); TODO
        remove(test_pgn_filename);
    }
}
