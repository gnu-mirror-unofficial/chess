#include <fstream>

#include "catch.hpp"

#include "frontend/common.h"
#include "frontend/cmd.cc"

extern char mock_SetDataToEngine_data[];

TEST_CASE("load: Loads a position in EPD format from disk into memory.", "[cmd_load]") {
    char test_epd_filename[] = "../data/Polgar-Karpov.epd";
    strcpy(inputstr, "load ");
    strcat(inputstr, test_epd_filename);
    split_input();

    const char epd[] = "2r2nk1/4qpp1/4p3/p2nN1P1/PpNP4/1PrR1Q2/5P2/4R1K1 w - - bm 1; id 1;";
    char expected_data_to_engine[BUF_SIZE]="setboard ";
    strcat(expected_data_to_engine, epd);

    SECTION("Data sent to the engine: 'setboard <epd game>'") {
        cmd_load();
        REQUIRE( strcmp(mock_SetDataToEngine_data, expected_data_to_engine) == 0 );
    }
}

TEST_CASE("epdload: Loads a position in EPD format from disk into memory.", "[cmd_epdload]") {
    char test_epd_filename[] = "../data/Polgar-Karpov.epd";
    strcpy(inputstr, "epdload ");
    strcat(inputstr, test_epd_filename);
    split_input();

    const char epd[] = "2r2nk1/4qpp1/4p3/p2nN1P1/PpNP4/1PrR1Q2/5P2/4R1K1 w - - bm 1; id 1;";
    char expected_data_to_engine[BUF_SIZE]="setboard ";
    strcat(expected_data_to_engine, epd);

    SECTION("Data sent to the engine: 'setboard <epd game>'") {
        cmd_load();
        REQUIRE( strcmp(mock_SetDataToEngine_data, expected_data_to_engine) == 0 );
    }
}

TEST_CASE("User input can be tokenized", "[split_input]") {

    /*
     * Global variables
     *
     * input:
     *
     *   char inputstr[BUF_SIZE]  // BUF_SIZE = 4096
     *
     * output:
     *
     *   char *token[TOKENS]  // TOKENS = 3
     */

    SECTION("Empty input yields empty tokens") {
        strcpy(inputstr, "");
        split_input();
        for (int i = 0; i < TOKENS; ++i) {
            REQUIRE( strcmp(token[i], "") == 0 );
        }
    }

    SECTION("One token") {
        const char *t1 = "token1";
        strcpy(inputstr, t1);
        split_input();
        REQUIRE( strcmp(token[0], t1) == 0 );
        for (int i = 1; i < TOKENS; ++i) {
            REQUIRE( strcmp(token[i], "") == 0 );
        }
    }

    SECTION("Two tokens") {
        const char *t1 = "token1";
        const char *t2 = "token2";
        char t12[BUF_SIZE] = "";
        strcpy(t12, t1);
        strcat(t12, " ");
        strcat(t12, t2);
        strcpy(inputstr, t12);
        split_input();
        REQUIRE( strcmp(token[0], t12) == 0 );
        REQUIRE( strcmp(token[1], t2) == 0 );
        for (int i = 2; i < TOKENS; ++i) {
            REQUIRE( strcmp(token[i], "") == 0 );
        }
    }

    SECTION("Three tokens") {
        const char *t1 = "token1";
        const char *t2 = "token2";
        const char *t3 = "token3";
        char t123[BUF_SIZE] = "";
        char t23[BUF_SIZE] = "";
        strcpy(t123, t1);
        strcat(t123, " ");
        strcat(t23, t2);
        strcat(t23, " ");
        strcat(t23, t3);
        strcat(t123, " ");
        strcat(t123, t23);
        strcpy(inputstr, t123);
        split_input();
        REQUIRE( strcmp(token[0], t123) == 0 );
        REQUIRE( strcmp(token[1], t23) == 0 );
        REQUIRE( strcmp(token[2], t3) == 0 );
    }

    SECTION("More than three tokens result in a possibly unexpected split") {
        strcpy(inputstr, "one two three four");
        split_input();
        REQUIRE( strcmp(token[0], "one two three four") == 0 );
        REQUIRE( strcmp(token[1], "two three four") == 0 );
        REQUIRE( strcmp(token[2], "three four") == 0 );
    }
}
