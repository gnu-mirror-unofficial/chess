// frontend/lexpgn.cc

#include <cstdio>

#include "frontend/common.h"
#include "frontend/lexpgn.h"

char *pgn_event;
char *pgn_site;
char *pgn_date;
char *pgn_round;
char *pgn_whiteELO;
char *pgn_blackELO;
char *pgn_white;
char *pgn_black;
char *pgn_result;
char *pgn_othertags;
char *initial_comments;

FILE *yyin;
enum data_destination_t data_dest;

int yylex (void) { return 0; }
