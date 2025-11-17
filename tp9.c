#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>

const int canyon_size = 20;
const int refresh_time = 50;

//print a canyon line of size size and beginning at column col
void draw_line(int size, int line, int col)
{   
    mvprintw(line, col, "|");
    mvprintw(line, col + size, "|");
}

void generate_first_state(int state_tab[LINES])
{

    for (int line = LINES-1; line > LINES-30; line--)
    {
        state_tab[line] = (COLS/2) - (canyon_size/2);
    }

    int col = (COLS/2) - (canyon_size/2);

    for (int line = 0; line < LINES-30; line++)
    {
        state_tab[line] = col;

        if (rand() % 3 == 0)
        {
            if (col + canyon_size < COLS - 1)
            {
                col++;
            }
        } 
        else if (rand() % 3 == 1)
        {
            if (col > 0)
            {
                col--;
            }
        }
    }

    for (int line = LINES-30; line < LINES; line++)
    {
        state_tab[line] = (COLS/2) - (canyon_size/2);
    }
}

void draw_state(int state_tab[LINES])
{
    for (int line = 0; line < LINES; line++)
    {
        draw_line(canyon_size, line, state_tab[line]);
    }

    refresh();
}

void next_step(int state_tab[LINES])
{
    for (int i = LINES-1; i > 0; i--)
    {
        state_tab[i] = state_tab[i-1];
    }

    int random = rand() % 3;
    if (random == 0)
    {
        if (state_tab[0] + canyon_size < COLS - 1)
        {
            state_tab[0] = state_tab[1] + 1;
        } 
    }
    else if (random == 1)
    {
        if (state_tab[0] > 0)
        {
            state_tab[0] = state_tab[1] - 1;
        }
    }
    else
    {
        state_tab[0] = state_tab[1];
    }
}

void draw_player(int pos)
{
    mvprintw(LINES-1, pos, "^");
}

void countdown(void)
{

}

bool check_for_death(int player_pos, int state_tab[LINES])
{
    if (player_pos <= state_tab[LINES-1])
    {
        return false;
    }

    if (player_pos >= state_tab[LINES-1] + canyon_size )
    {
        return false;
    }

    return true;
}

int move_player(int player_pos)
{
    char input = getch();

    if (input == 'q')
    {
        return player_pos - 1;
    }
    else if (input == 'd')
    {
        return player_pos + 1;
    }

    return player_pos;
}

void game(void)
{
    initscr(); // initialise l'écran
    noecho();
    curs_set(0);

    int state_tab[LINES];
    bool player_is_alive = true;
    int line_counter = 0;
    int player_pos = COLS/2;

    generate_first_state(state_tab);

    draw_state(state_tab);
    draw_player(player_pos);

    countdown();

    nodelay(stdscr, TRUE);

    while (player_is_alive)
    {
        next_step(state_tab);
        clear();

        player_pos = move_player(player_pos);

        draw_player(player_pos);
        draw_state(state_tab);

        player_is_alive = check_for_death(player_pos, state_tab);
        
        line_counter++;
        usleep(20000);
    }

    mvprintw(LINES-2, 0, "you noob, you only survived %d lines (or %d seconds) which makes you the worst player i have ever seen", line_counter, line_counter*(1/50));
    mvprintw(LINES-1, 0, "press q to exit...");

    nodelay(stdscr, FALSE);

    char input = getch();
    while (input != 'a')
    {
        input = getch();
    }
    
    endwin();
}

int main(void)
{
    srand(time(NULL));

    game();
    return EXIT_SUCCESS;
}