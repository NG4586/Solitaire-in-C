// Nathaniel Graves

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

const char RANKS[13] = {'A', '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K'};
const char* SUITS[4] = {"♠", "♥", "♦", "♣"};

/* state
   0 - does not exist
   1 - top card, face down
   2 - top card, face up
   3 - card in vertical stack, face down
   4 - card in vertical stack, face up
   5 - card in horizontal stack (always face up) */

struct card
{
   int state;
   int rank;
   int suit;
};

struct table
{
   struct card deck[24];
   int topCard;
   struct card discard[24];
   int topDiscard;
   int taken;
   struct card foundation[4];
   struct card tableau[7][19];
};

struct table setup()
{
   struct table game;
   int order[52];
   for (int n = 0; n < 52; n ++)
   {
      order[n] = rand();
   }
   int codes[52];
   int counter;
   for (int n = 0; n < 52; n ++)
   {
      counter = 0;
      for (int m = 0; m < 52; m ++)
      {
         if (order[m] < order[n])
         {
            counter ++;
         }
         else if (order[m] == order[n] && n > m)
         {
            counter ++;
         }
      }
      codes[n] = counter;
   }
   counter = 0;
   for (int r = 0; r < 19; r ++)
   {
      if (r < 6)
      {
         game.tableau[r][r].rank = codes[counter] / 4;
         game.tableau[r][r].suit = codes[counter] % 4;
         game.tableau[r][r].state = 2;
         counter ++;
      }
      for (int c = 0; c < 7; c ++)
      {
         if (c < r)
         {
            game.tableau[c][r].state = 0;
         }
         else if (c > r)
         {
            game.tableau[c][r].rank = codes[counter] / 4;
            game.tableau[c][r].suit = codes[counter] % 4;
            game.tableau[c][r].state = 3;
            counter ++;
         }
      }
   }
   game.tableau[6][6].rank = codes[counter] / 4;
   game.tableau[6][6].suit = codes[counter] % 4;
   game.tableau[6][6].state = 2;
   counter ++;
   for (int n = 0; n < 24; n ++)
   {
      game.discard[n].state = 0;
      game.deck[n].rank = codes[counter] / 4;
      game.deck[n].suit = codes[counter] % 4;
      game.deck[n].state = 1;
      counter ++;
   }
   game.topCard = 23;
   game.topDiscard = 0;
   game.taken = 0;
   for (int n = 0; n < 4; n ++)
   {
      game.foundation[n].state = 0;
   }
   return game;
}

void row(struct card content, int offset)
{
   if (offset == 0) // vertical distance from top row of card
   {
      if (content.state != 5)
      {
         printf("┌───┐");
      }
      else
      {
         printf("┌─");
      }
   }
   else if (offset == 1)
   {
      if (content.state == 0)
      {
         printf("│   │");
      }
      else if (content.state == 1)
      {
         printf("│  X│");
      }
      else if (content.state == 2)
      {
         printf("│%s  │", SUITS[content.suit]);
      }
      else if (content.state == 4)
      {
         printf("│%s%c │", SUITS[content.suit], RANKS[content.rank]);
      }
      else
      {
         printf("│%s", SUITS[content.suit]);
      }
   }
   else if (offset == 2)
   {
      if (content.state == 0)
      {
         printf("│   │");
      }
      else if (content.state == 1)
      {
         printf("│ X │");
      }
      else if (content.state == 2)
      {
         printf("│ %c │", RANKS[content.rank]);
      }
      else
      {
         printf("│%c", RANKS[content.rank]);
      }
   }
   else if (offset == 3)
   {
      if (content.state == 0)
      {
         printf("│   │");
      }
      else if (content.state == 1)
      {
         printf("│X  │");
      }
      else if (content.state == 2)
      {
         printf("│  %s│", SUITS[content.suit]);
      }
      else
      {
         printf("│ ");
      }
   }
   else if (offset == 4)
   {
      if (content.state != 5)
      {
         printf("└───┘");
      }
      else
      {
         printf("└─");
      }
   }
   else
   {
      if (content.state != 5)
      {
         printf("     ");
      }
      else
      {
         printf("  ");
      }
   }
}

void output(struct table game)
{
   printf("\r\n");
   for (int r = 0; r < 5; r ++)
   {
      row(game.deck[game.topCard], r);
      for (int n = 2 - game.taken; n >= 0; n --)
      {
         if (game.topDiscard - n >= 0 && game.discard[game.topDiscard - n].state != 0)
         {
            row(game.discard[game.topDiscard - n], r);
         }
      }
      if (game.discard[game.topDiscard].state == 0)
      {
         printf("          ");
      }
      else if (game.taken == 2 || game.topDiscard == 0)
      {
         printf("     ");
      }
      else if (game.taken == 1 || game.topDiscard == 1)
      {
         printf("   ");
      }
      else
      {
         printf(" ");
      }
      for (int n = 0; n < 4; n ++)
      {
         row(game.foundation[n], r);
      }
      printf("\r\n");
   }
   printf("\n");
   int progress[7] = {0, 0, 0, 0, 0, 0, 0};
   int increment[7] = {0, 0, 0, 0, 0, 0, 0};
   int lines = 35;
   int complete = 0;
   while (complete < 7)
   {
      for (int n = 0; n < 7; n ++)
      {
         row(game.tableau[n][progress[n]], increment[n]);
         if (game.tableau[n][progress[n]].state == 3)
         {
            progress[n] ++;
         }
         else if (game.tableau[n][progress[n]].state == 4)
         {
            if (increment[n] == 1)
            {
               progress[n] ++;
               increment[n] = 0;
            }
            else
            {
               increment[n] ++;
            }
         }
         else
         {
            if (increment[n] == 4)
            {
               complete ++;
            }
            increment[n] ++;
         }
      }
      printf("\r\n");
      lines --;
   }
   while (lines > 0)
   {
      printf("\n");
      lines --;
   }
}

bool draw(struct table* game)
{
   if ((game -> deck)[0].state == 0) // stronger topCard == 0
   {
      if ((game -> topDiscard) < 3)
      {
         printf("No remaining cards.\r\n");
         return false;
      }
      while ((game -> topDiscard) >= 0) // turning the discard over: end -> start
      {
         (game -> deck)[game -> topCard] = (game -> discard)[game -> topDiscard];
         (game -> deck)[game -> topCard].state = 1;
         (game -> discard)[game -> topDiscard].state = 0;
         (game -> topCard) ++;
         (game -> topDiscard) --;
      }
      (game -> topCard) --; // off by one correction
      (game -> topDiscard) ++;
   }
   if ((game -> topDiscard) > 0 || (game -> discard)[0].state == 2)
   {
      (game -> discard)[game -> topDiscard].state = 5;
   }
   else
   {
      (game -> topDiscard) --;
   }
   if ((game -> topCard) == 0)
   {
      (game -> discard)[(game -> topDiscard) + 1] = (game -> deck)[0];
      (game -> discard)[(game -> topDiscard) + 1].state = 2;
      (game -> deck)[0].state = 0;
      (game -> topDiscard) ++;
      (game -> taken) = 2;
   }
   else if ((game -> topCard) == 1)
   {
      (game -> discard)[(game -> topDiscard) + 1] = (game -> deck)[1];
      (game -> discard)[(game -> topDiscard) + 1].state = 5;
      (game -> deck)[1].state = 0;
      (game -> discard)[(game -> topDiscard) + 2] = (game -> deck)[0];
      (game -> discard)[(game -> topDiscard) + 2].state = 2;
      (game -> deck)[0].state = 0;
      (game -> topCard) --;
      (game -> topDiscard) += 2;
      (game -> taken) = 1;
   }
   else
   {
      for (int n = 0; n < 3; n ++)
      {
         (game -> discard)[(game -> topDiscard) + 1] = (game -> deck)[game -> topCard];
         (game -> discard)[(game -> topDiscard) + 1].state = 5;
         (game -> deck)[game -> topCard].state = 0;
         (game -> topCard) --;
         (game -> topDiscard) ++;
      }
      (game -> discard)[game -> topDiscard].state = 2;
      if ((game -> topCard) == -1)
      {
         (game -> topCard) ++;
      }
      (game -> taken) = 0;
   }
   return true;
}

bool match(struct card cardA, struct card cardB) // Above, Below
{
   if (cardA.state == 0)
   {
      cardA.rank = 13;
      cardA.suit = 4;
   }
   if (cardA.rank != cardB.rank + 1)
   {
      return false;
   }
   if (cardA.suit == 0 || cardA.suit == 3)
   {
      return cardB.suit == 1 || cardB.suit == 2;
   }
   if (cardA.suit == 1 || cardA.suit == 2)
   {
      return cardB.suit == 0 || cardB.suit == 3;
   }
   return true;
}

bool place(struct table* game)
{
   if ((game -> discard)[0].state == 0)
   {
      printf("The draw pile is empty.\r\n");
      return false;
   }
   int stack;
   scanf("%d", &stack);
   if ((stack < 1) || (stack > 7))
   {
      printf("There is no stack %d.\r\n", stack);
      return false;
   }
   stack --;
   struct card cardA;
   int space;
   for (space = 0; space < 18; space ++)
   {
      if ((game -> tableau)[stack][space + 1].state == 0)
      {
         cardA = (game -> tableau)[stack][space];
         break;
      }
   }
   struct card cardB = (game -> discard)[game -> topDiscard];
   if (!match(cardA, cardB))
   {
      printf("Invalid placement.\r\n");
      return false;
   }
   if (cardA.state == 0)
   {
      (game -> tableau)[stack][0] = cardB;
   }
   else
   {
      (game -> tableau)[stack][space + 1] = cardB;
      (game -> tableau)[stack][space].state = 4;
   }
   (game -> discard)[game -> topDiscard].state = 0;
   if ((game -> topDiscard) > 0)
   {
      (game -> discard)[(game -> topDiscard) - 1].state = 2;
      (game -> topDiscard) --;
   }
   if ((game -> taken) < 2)
   {
      (game -> taken) ++;
   }
   return true;
}

bool move(struct table* game)
{
   int stackB;
   scanf("%d", &stackB);
   int cutoff;
   scanf("%d", &cutoff);
   int stackA;
   scanf("%d", &stackA);
   if ((stackB < 1) || (stackB > 7))
   {
      printf("There is no stack %d.\r\n", stackB);
      return false;
   }
   stackB --;
   if ((cutoff < 0) || (cutoff > 18) || (game -> tableau)[stackB][cutoff].state == 0)
   {
      int count = 19;
      while (count > 0 && (game -> tableau)[stackB][count - 1].state == 0)
      {
         count --;
      }
      printf("You cannot take %d cards from stack %d.\r\n", count - cutoff, stackB + 1);
      return false;
   }
   if ((game -> tableau)[stackB][cutoff].state == 3)
   {
      printf("You cannot move an unrevealed card.\n");
      return false;
   }
   if ((stackA < 1) || (stackA > 7))
   {
      printf("There is no stack %d.\r\n", stackA);
      return false;
   }
   stackA --;
   int attatchment = 0;
   while ((game -> tableau)[stackA][attatchment].state > 2)
   {
      attatchment ++;
   }
   if (!match((game -> tableau)[stackA][attatchment], (game -> tableau)[stackB][cutoff]))
   {
      printf("Invalid placement.\r\n");
      return false;
   }
   if (cutoff > 0)
   {
      (game -> tableau)[stackB][cutoff - 1].state = 2;
   }
   if ((game -> tableau)[stackA][0].state == 0)
   {
      (game -> tableau)[stackA][0] = (game -> tableau)[stackB][cutoff];
      (game -> tableau)[stackA][0].state = 4;
      (game -> tableau)[stackB][cutoff].state = 0;
      cutoff ++;
   }
   for (int n = cutoff; n < 19 && (game -> tableau)[stackB][n].state != 0; n ++)
   {
      (game -> tableau)[stackA][attatchment + 1] = (game -> tableau)[stackB][n];
      (game -> tableau)[stackA][attatchment].state = 4;
      (game -> tableau)[stackB][n].state = 0;
      attatchment ++;
   }
   (game -> tableau)[stackA][attatchment].state = 2;
   return true;
}

bool found(struct table* game)
{
   int stack;
   scanf("%d", &stack);
   if ((stack < 0) || (stack > 7))
   {
      printf("There is no stack %d.\r\n", stack);
      return false;
   }
   stack --;
   struct card cardB;
   int space;
   if (stack == -1)
   {
      if ((game -> discard)[0].state == 0)
      {
         printf("The draw pile is empty.\r\n");
         return false;
      }
      cardB = (game -> discard)[game -> topDiscard];
   }
   else
   {
      if ((game -> tableau)[stack][0].state == 0)
      {
         printf("Stack %d is empty.\r\n", stack + 1);
      }
      space = 0;
      while ((game -> tableau)[stack][space].state != 2)
      {
         space ++;
      }
      cardB = (game -> tableau)[stack][space];
   }
   struct card cardA = (game -> foundation)[cardB.suit];
   if (cardA.state == 0)
   {
      cardA.rank = -1;
   }
   if (cardA.rank != cardB.rank - 1)
   {
      printf("The foundation does not contain the preceding card.\r\n");
      return false;
   }
   (game -> foundation)[cardB.suit] = cardB;
   if (stack == -1)
   {
      (game -> discard)[game -> topDiscard].state = 0;
      if ((game -> topDiscard) > 0)
      {
         (game -> discard)[(game -> topDiscard) - 1].state = 2;
         (game -> topDiscard) --;
      }
      if ((game -> taken) < 2)
      {
         (game -> taken) ++;
      }
   }
   else
   {
      (game -> tableau)[stack][space].state = 0;
      if (space > 0)
      {
         (game -> tableau)[stack][space - 1].state = 2;
      }
   }
   return true;
}

void act(bool (*action)(struct table*), struct table* game, char* input)
{
   if ((*action)(game))
   {
      output(*game);
   }
   for (int n = 0; n < 4; n ++)
   {
      if ((game -> foundation)[n].rank != 12)
      {
         *input = 'i';
         return;
      }
   }
   printf("You win!\r\n");
   *input = 'q';
}

int main(int argc, char** argv)
{
   if (argc == 2)
   {
      unsigned int seed = strtol(argv[1], NULL, 10);
      srand(seed);
   }
   else
   {
      srand(time(NULL));
   }
   struct table game = setup();
   output(game);
   char input = 'i';
   while (input != 'q')
   {
      scanf("%c", &input);
      if (input == 'd' || input == '1')
      {
         act(draw, &game, &input);
      }
      else if (input == 'p' || input == '2')
      {
         act(place, &game, &input);
      }
      else if (input == 'm' || input == '3')
      {
         act(move, &game, &input);
      }
      else if (input == 'f' || input == '4')
      {
         act(found, &game, &input);
      }
   }
   return 0;
}
