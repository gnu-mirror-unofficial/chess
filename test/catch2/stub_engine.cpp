// frontend/engine.cc

#include <cstring>

#include "frontend/common.h"

char mock_SetDataToEngine_data[BUF_SIZE];

char inputstr[BUF_SIZE]="";

void SetDataToEngine( const char data[] )
{
    strncpy(mock_SetDataToEngine_data, data, BUF_SIZE);
}

void ExpectAnswerFromEngine( int answerExpected ) {}
void SetUserInputValidMove( int valid ) {}
void ChangeColor( int change ) {}
void SetAutoGo( int go ) {}
