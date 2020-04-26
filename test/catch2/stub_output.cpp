// frontend/output.cc

#include "frontend/common.h"

bool mock_ShowBoard = false;

void ShowBoard(void) { mock_ShowBoard = true; }
void ShowGame(void) {}
void ShowMoveList (int ply) {}
void ShowTime (void) {}
