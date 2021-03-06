/* epd.c

   GNU Chess frontend

   Copyright (C) 2001-2021 Free Software Foundation, Inc.

   GNU Chess is based on the two research programs
   Cobalt by Chua Kong-Sian and Gazebo by Stuart Cracraft.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.

   Contact Info:
     bug-gnu-chess@gnu.org
     cracraft@ai.mit.edu, cracraft@stanfordalumni.org, cracraft@earthlink.net
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "common.h"

#define EPDCLOSE 1

/* A line read from an EPD file, used in solve.c */
char epd_line[MAXSTR]="";

short ReadEPDFile (const char *file, short op)
/****************************************************************************
 *
 *  Reads in an EPD file.  The first call will read the first EPD line,
 *  the second call will read the 2nd line and so on.   To improve
 *  performance, the file is never closed.  Closing of the file happens
 *  only on 2 conditions:  (i) A ReadEPDFile failed because there is no
 *  more lines to read.  (ii) A called to ReadEPDFile to explicitly
 *  request that the file is closed (op = EPDCLOSE);
 *  If op == 2, then we work silently.
 *  Comment lines are stripped off. Comment mark is '#'.
 *
 ****************************************************************************/
{
   static FILE *fp = NULL;
   char line[MAXSTR];

   /*  If first time through, must open file  */
   if (fp == NULL)
   {
      fp = fopen (file, "r");
      if (fp == NULL)
      {
         printf ("Error opening file %s\n", file);
         return (false);
      }
   }

   /*  Is this a close request? */
   if (op == EPDCLOSE)
   {
      fclose (fp);
      fp = NULL;
      return (false);
   }

next_line:
   /*  Okay, we read in an EPD entry  */
   if ( fgets (line, MAXSTR-1, fp) == NULL ) {
      /* Do nothing - just avoid compilation warning - further refactoring is tricky. */
   }
   strcpy( epd_line, line );
   if (!feof(fp))
   {
      /* Skip comment lines */
      unsigned int i=0;
      for (i = 0; i < strlen( line ); ++i) {
         if ( isblank( line[i] ) ) {
            continue;
         }
         if ( line[i] == '#' ) {
            goto next_line;
         } else {
            break;
         }
      }
      int ret = ParseEPD (line);

      /* For now just ignore malformed lines */
      if (ret != EPD_SUCCESS) goto next_line;
      if (op != 2)
         printf ("\n%s : Best move = %s\n", id, solution);
      return (true);
   }
   /* finished, must close file */
   else
   {
      fclose (fp);
      fp = NULL;
      return (false);
   }
}

/*
 * Returns EPD_SUCCESS on success, EPD_ERROR on error. We try to be
 * quite tough on the format. However, as of yet no legality checking
 * is done and the board is not reset on error, this should be done by
 * the caller.
 */

int ParseEPD (char *p)
/**************************************************************************
 *
 *  Parses an EPD input line.  A few global variables are updated e.g.
 *  current board, side to move, en passant, castling status, etc.
 *
 **************************************************************************/
{
   int r, c, sq;
   char *str_p;

   r = 56;
   c = 0;
   memset (&board, 0, sizeof (board));

   while (p && *p != ' ')
   {
     sq = r + c;
     switch (*p)
     {
        case 'P' :  SETBIT (board.b[white][pawn], sq);
		    SETBIT (board.blockerr90, r90[sq]);
		    SETBIT (board.blockerr45, r45[sq]);
		    SETBIT (board.blockerr315, r315[sq]);
		    board.material[white] += ValueP;
		    break;
        case 'N' :  SETBIT (board.b[white][knight], sq);
		    SETBIT (board.blockerr90, r90[sq]);
		    SETBIT (board.blockerr45, r45[sq]);
		    SETBIT (board.blockerr315, r315[sq]);
		    board.material[white] += ValueN;
		    break;
        case 'B' :  SETBIT (board.b[white][bishop], sq);
		    SETBIT (board.blockerr90, r90[sq]);
		    SETBIT (board.blockerr45, r45[sq]);
		    SETBIT (board.blockerr315, r315[sq]);
		    board.material[white] += ValueB;
		    break;
        case 'R' :  SETBIT (board.b[white][rook], sq);
		    SETBIT (board.blockerr90, r90[sq]);
		    SETBIT (board.blockerr45, r45[sq]);
		    SETBIT (board.blockerr315, r315[sq]);
		    board.material[white] += ValueR;
		    break;
        case 'Q' :  SETBIT (board.b[white][queen], sq);
		    SETBIT (board.blockerr90, r90[sq]);
		    SETBIT (board.blockerr45, r45[sq]);
		    SETBIT (board.blockerr315, r315[sq]);
		    board.material[white] += ValueQ;
		    break;
        case 'K' :  SETBIT (board.b[white][king], sq);
		    SETBIT (board.blockerr90, r90[sq]);
		    SETBIT (board.blockerr45, r45[sq]);
		    SETBIT (board.blockerr315, r315[sq]);
		    break;
        case 'p' :  SETBIT (board.b[black][pawn], sq);
		    SETBIT (board.blockerr90, r90[sq]);
		    SETBIT (board.blockerr45, r45[sq]);
		    SETBIT (board.blockerr315, r315[sq]);
		    board.material[black] += ValueP;
		    break;
        case 'n' :  SETBIT (board.b[black][knight], sq);
		    SETBIT (board.blockerr90, r90[sq]);
		    SETBIT (board.blockerr45, r45[sq]);
		    SETBIT (board.blockerr315, r315[sq]);
		    board.material[black] += ValueN;
		    break;
        case 'b' :  SETBIT (board.b[black][bishop], sq);
		    SETBIT (board.blockerr90, r90[sq]);
		    SETBIT (board.blockerr45, r45[sq]);
		    SETBIT (board.blockerr315, r315[sq]);
		    board.material[black] += ValueB;
		    break;
        case 'r' :  SETBIT (board.b[black][rook], sq);
		    SETBIT (board.blockerr90, r90[sq]);
		    SETBIT (board.blockerr45, r45[sq]);
		    SETBIT (board.blockerr315, r315[sq]);
		    board.material[black] += ValueR;
		    break;
        case 'q' :  SETBIT (board.b[black][queen], sq);
		    SETBIT (board.blockerr90, r90[sq]);
		    SETBIT (board.blockerr45, r45[sq]);
		    SETBIT (board.blockerr315, r315[sq]);
                    board.material[black] += ValueQ;
		    break;
        case 'k' :  SETBIT (board.b[black][king], sq);
		    SETBIT (board.blockerr90, r90[sq]);
		    SETBIT (board.blockerr45, r45[sq]);
		    SETBIT (board.blockerr315, r315[sq]);
		    break;
        case '/' :  r -= 8;
	 	    c = -1;
		    break;
        default  :  break;
     }
     if (isdigit (*p))
        c += (*p - '0');
     else
        c++;

     /*
      * Special case, a trailing "/" is accepted on the
      * end of the board settings.
      */

     if (r == -8 && p[1] == ' ')
	     r = 0;

     if (r < 0 || c > 8) return EPD_ERROR;
     if (c == 8 && p[1] != '/' && p[1] != ' ') return EPD_ERROR;
     p++;
   }

   board.pmaterial[white] = board.material[white] -
				nbits(board.b[white][pawn]) * ValueP;
   board.pmaterial[black] = board.material[black] -
				nbits(board.b[black][pawn]) * ValueP;
   board.king[white] = leadz (board.b[white][king]);
   board.king[black] = leadz (board.b[black][king]);
   UpdateFriends ();
   UpdateCBoard ();
   UpdateMvboard ();

   /*  Get side to move  */
   if (!++p) return EPD_ERROR;
   if      (*p == 'w') board.side = white;
   else if (*p == 'b') board.side = black;
   else return EPD_ERROR;

   /* Isn't this one cute? */
   if (!++p || *p != ' ' || !++p) return EPD_ERROR;

   /*  Castling status  */
   while (p && *p != ' ') {
      if      (*p == 'K') board.flag |= WKINGCASTLE;
      else if (*p == 'Q') board.flag |= WQUEENCASTLE;
      else if (*p == 'k') board.flag |= BKINGCASTLE;
      else if (*p == 'q') board.flag |= BQUEENCASTLE;
      else if (*p == '-') { p++; break; }
      else return EPD_ERROR;
      p++;
   }
   if (!p || *p != ' ' || !++p) return EPD_ERROR;

   /*
    * En passant square, can only be '-' or [a-h][36]
    * In fact, one could add more sanity checks here.
    */
   if (*p != '-') {
      if (!p[1] || *p < 'a' || *p > 'h' ||
	  !(p[1] == '3' || p[1] == '6')) return EPD_ERROR;
      board.ep = (*p - 'a') + (p[1] - '1')*8;
      p++;
   } else {
      board.ep = -1;
   }

   solution[0] = '\0';
   id[0] = '\0';

   if (!++p) return EPD_SUCCESS;

   /* The opcodes are optional, so we should not generate errors here */

   /*  Read in best move; "bm" operator */
   str_p = strstr(p, "bm");
   if (str_p) sscanf (str_p, "bm %63[^;];", solution);

   /*  Read in the description; "id" operator */
   str_p = strstr(p, "id");
   if (str_p) sscanf (p, "id %31[^;];", id);

   phase = PHASE;

   return EPD_SUCCESS;
}


void LoadEPD (char *p)
/**************************************************************************
 *
 *  This routine reads in the next or the Nth position in the file.
 *
 **************************************************************************/
{
   char file[MAXSTR];
   int N = 1;

   sscanf (p, "%127s %d ", file, &N);
   if (strcmp (file, "next") == 0)
   {
      ReadEPDFile (file, 0);
   }
   else
   {
      ReadEPDFile (file, 1);
      while (--N)
      {
         if (ReadEPDFile (file, 2) == false)
         {
	    printf ("File position exceeded\n");
	    return;
         }
      }
      ReadEPDFile (file, 0);
   }
   ShowBoard ();
   NewPosition ();
}


void EPD2str (char *pos)
/**************************************************************************
 *
 *  This routine writes the current position in EPD format into a string.
 *
 **************************************************************************/
{
   int r, c, sq, k;
   char c1;

   for (r = A8; r >= A1; r -= 8)
   {
      k = 0;
      for (c = 0; c < 8; c++)
      {
         sq = r + c;
         if (cboard[sq] == empty)
            k++;
         else
         {
            if (k)
               sprintf(pos + strlen(pos), "%1d", k);
            k = 0;
            c1 = notation[cboard[sq]];
            if (BitPosArray[sq] & board.friends[black])
               c1 = tolower (c1);
            sprintf(pos + strlen(pos), "%c", c1);
         }
      }
      if (k)
         sprintf(pos + strlen(pos), "%1d", k);
      if (r > A1)
         sprintf(pos + strlen(pos), "/");
   }

   /* Print other stuff */
   sprintf(pos + strlen(pos), (board.side == white ? " w " : " b "));

   if (board.flag & WKINGCASTLE)
      sprintf(pos + strlen(pos), "K");
   if (board.flag & WQUEENCASTLE)
      sprintf(pos + strlen(pos), "Q");
   if (board.flag & BKINGCASTLE)
      sprintf(pos + strlen(pos), "k");
   if (board.flag & BQUEENCASTLE)
      sprintf(pos + strlen(pos), "q");
   if (!(board.flag & (WCASTLE | BCASTLE)))
      sprintf(pos + strlen(pos), "-");

   sprintf(pos + strlen(pos), " %s", (board.ep > -1 ? algbr[board.ep] : "-"));
   sprintf(pos + strlen(pos), " bm 1; id 1;");
}


void SaveEPD (char *p)
/**************************************************************************
 *
 *  This routine appends the current position in EPD format into a file.
 *
 **************************************************************************/
{
   char file[MAXSTR];
   FILE *fp;
   char pos[MAXSTR] = "";

   EPD2str(pos);
   sscanf (p, "%s ", file);
   fp = fopen (file, "a");
   fprintf(fp, "%s\n", pos);
   fclose (fp);
}
