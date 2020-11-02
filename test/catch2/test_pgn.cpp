#include "catch.hpp"

#include <fstream>

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

    // void PGNSaveToFile (const char *file, const char *resultstr)

    SECTION("The file is saved if it does not exist") {
        const char test_pgn_filename[] = ".tmp.pgn";
        remove(test_pgn_filename);
        char resultstr[1024]=""; 
        PGNSaveToFile(test_pgn_filename, resultstr);
        FILE *f = fopen(test_pgn_filename, "r");
        REQUIRE( f != NULL );
        char game[1024] = "";
        char s[64];
        while (fgets(s, 64, f) != NULL) {
            strcat(game, s);
        }
        REQUIRE( strstr(game, "Event") != NULL );
        REQUIRE( strstr(game, "Result") != NULL );
        REQUIRE( strstr(game, "1.") != NULL );
        fclose(f);
        remove(test_pgn_filename);
    }

    SECTION("The file is not saved if it already exists") {
        const char test_pgn_filename[] = ".tmp.pgn";
        remove(test_pgn_filename);
        // Add dummy contents to file
        std::ofstream test_pgn_file(test_pgn_filename);
        test_pgn_file << "dummy game" << std::endl;
        // Try to save game to file
        char resultstr[1024]=""; 
        PGNSaveToFile(test_pgn_filename, resultstr);
        FILE *f = fopen(test_pgn_filename, "r");
        REQUIRE( f != NULL );
        char game[1024] = "";
        char s[64];
        while (fgets(s, 64, f) != NULL) {
            strcat(game, s);
        }
        REQUIRE( strstr(game, "Event") == NULL );
        REQUIRE( strstr(game, "Result") == NULL );
        REQUIRE( strstr(game, "1.") == NULL );
        fclose(f);
        remove(test_pgn_filename);
    }
}
