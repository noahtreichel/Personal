#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <cab202_graphics.h>
#include <cab202_timers.h>


// Set this to true when the game is over.
bool game_over = false;

// Set this to true when Jerry completes a level.
bool next_level = false;

// Set this to true when the game is paused.
bool game_pause = false;

// Variables displayed in the status bar
int student_number = 10533206;
int score = 0;
int lives = 5;
char player = 'J';
int timer_m = 0;
int timer_s = 0;
int cheese = 1;
int mousetraps = 1;
int fireworks = 0;
int level = 0;

// Creates the timer and declares the start time.
time_t time(time_t *time_timer);
double start_time;

// Variable that creates a space
char space = 32;

// Screen dimensions
int W, H, SBH;

// Declares four variables of type double.
double x_1, y_1, x_2, y_2;


// Detects collision between objects that each occupy a single pixel.
bool collided(double x0, double y0, double x1, double y1)
{
    return round(x0) == round(x1) && round(y0) == round(y1);
}


// Displays when Jerry either runs out of lives or completes a room.
void do_collided()
{
    clear_screen();
    
    // Displays a 'Game Over' message to the screen
    draw_formatted((screen_width() / 2) - 5, screen_height() * 0.35, "GAME OVER!");
    
    draw_formatted((screen_width() / 2) - 25, screen_height() * 0.45,
    "Your game ended on level %d, with a final score of %d", level, score);

    draw_formatted((screen_width() / 2) - 21, screen_height() * 0.5,
    "Press 'r' to restart  OR  Press 'q' to quit");
    
    show_screen();

    // Reads character from standard input stream 
    char decision = getchar();

    // If the user inputs 'r', restart the game to its original state.
    if (decision == 'r') 
    {
        next_level = true;
        start_time = time(0);
        score = 0;
        lives = 5;
        player = 'J';
        timer_m = 0;
        timer_s = 0;
        cheese = 1;
        mousetraps = 1;
        fireworks = 0;
        level = 0;
    }

    // Else if the user inputs 'q', end the game.
    else if (decision == 'q')
    {
        game_over = true;
        return;
    }

    // Else repeat this function.
    else
    {
        do_collided();
    }   
}


// Walls
char wall = '*';


// Jerry
double jerry_x, jerry_y;
char Jerry = 'J';

// Initialises Jerry's x and y position
void setup_jerry()
{
    jerry_x = x_1 * W;
    jerry_y = (y_1 * H) + SBH;
}

void draw_jerry()
{
    draw_char(round(jerry_x), round(jerry_y), Jerry);
}

// Updates Jerry's movement via keyboard input of w, a, s and d characters
// Detects if the following position of Jerry is a wall
void update_jerry(int movement) 
{
    if (movement == 'w' && jerry_y > 4 && scrape_char(jerry_x, jerry_y - 1) != '*')
    {
        jerry_y--;
    }

    else if (movement == 'a' && jerry_x > 0 && scrape_char(jerry_x - 1, jerry_y) != '*')
    {
        jerry_x--;
    }

    else if (movement == 's' && jerry_y < H - (-4) && scrape_char(jerry_x, jerry_y + 1) != '*')
    {
        jerry_y++;
    }

    else if (movement == 'd' && jerry_x < W && scrape_char(jerry_x + 1, jerry_y) != '*')
    {
        jerry_x++;
    }
}


// Tom
char Tom = 'T';
double tom_x, tom_y, tom_dx, tom_dy;
double pi = 3.14;

// Initialises Toms's x and y position randomly
// Initialises Tom's randomised movement
void setup_tom()
{
    tom_x = 1 + rand() % (W - 2);
    tom_y = 4 + rand() % (H - 4);

    double tom_dir = rand() * pi * 2 / RAND_MAX;
    const double step = 0.125;

    tom_dx = step * cos(tom_dir);
    tom_dy = step * sin(tom_dir);
}

// If Tom's initialised position is in a wall, re-initialise the position and draw
void draw_tom()
{
    if (scrape_char(tom_x, tom_y) != '*')
    {
        draw_char(round(tom_x), round(tom_y), Tom);
    }
    
    if (scrape_char(tom_x, tom_y) == '*')
    {
        setup_tom();
        draw_tom();
    }
}

void move_tom()
{
    // Assume that Tom has not already collided with the borders.
    // Predicts the next screen position of Tom
    int new_x = round(tom_x + tom_dx);
    int new_y = round(tom_y + tom_dy);

    bool bounced = false;

    if (new_x == 0 || new_x == W)
    {
        // Bounce off left or right wall: reverse horizontal direction
        tom_dx = -tom_dx;
        bounced = true;
    }

    if (new_y == 3 || new_y == H - (-5))
    {
        // Bounce off top or bottom wall: reverse vertical direction
        tom_dy = -tom_dy;
        bounced = true;
    }

    // Detects if the following position of Tom along the y-axis is a wall
    if (scrape_char(tom_x, tom_y + 1) == '*' || scrape_char(tom_x, tom_y - 0.1) == '*')
    {
        tom_dy = -tom_dy;
    }

    // Detects if the following position of Tom along the x-axis is a wall
    if (scrape_char(tom_x + 1, tom_y) == '*' || scrape_char(tom_x - 0.1, tom_y) == '*')
    {
        tom_dx = -tom_dx;
    }

    if (!bounced)
    {
        // No bounce: move instead
        tom_x += tom_dx;
        tom_y += tom_dy;
    }
}

// Updates Tom's (and Jerry's) position if they collide
// If Tom and Jerry collide, Jerry loses 1 life. This repeats untill Jerry reachers 0 lives, where the game will then end.
void update_tom(int key)
{
    // Checks if the game is paused
    if (game_pause) return;

    if (key < 0)
    {
        move_tom();
    }

    if (collided(jerry_x, jerry_y, tom_x, tom_y))
    {
        //setup_jerry();
        setup_tom();
        lives--;

        if (lives <= 0)
        {
            do_collided();
        }
    }
}


// Cheese
double cheese_x, cheese_y;
char Cheese = 'C';

// Initialises the cheeses x and y position randomly
void setup_cheese()
{
    cheese_x = 1 + rand() % (W - 2);
    cheese_y = 4 + rand() % (H - 4);
}

// If the cheeses initialised position is in a wall, re-initialise the position and draw
void draw_cheese()
{
    if (scrape_char(cheese_x, cheese_y) != '*')
    {
        draw_char(round(cheese_x), round(cheese_y), Cheese);
    }
    else
    {
        setup_cheese();
        draw_cheese();
    }
}

// Updates the score by +1 if Jerry collides with the cheese
void update_cheese(int key)
{
    // If the amount of cheese present on the screen is below 5, draw a cheese every 2 seconds
    /*  
        if (cheese < 5 && ((timer_s + 2) / ((cheese + 1)) % 2 == 0))
        {
            draw_cheese();
            setup_cheese();
            cheese++;
        }
    */

    if (collided(jerry_x, jerry_y, cheese_x, cheese_y))
    {
        score++;
        setup_cheese();
        //cheese--;
    }
}


// Mousetraps
double mousetrap_x, mousetrap_y;
char Mousetrap = 'M';

// Initialises the mousetraps x and y position
void setup_mousetrap()
{
    mousetrap_x = tom_x;
    mousetrap_y = tom_y;
}

// If the mousetraps initialised position is in a wall, re-initialise the position and draw
void draw_mousetrap()
{
    if (scrape_char(mousetrap_x, mousetrap_y) != '*')
    {
        draw_char(round(mousetrap_x), round(mousetrap_y), Mousetrap);
    }
    else
    {
        setup_mousetrap();
        draw_mousetrap();
    }
}

// Updates the amount of lives by -1 if Jerry collides with a mousetrap
void update_mousetrap(int key)
{
    if (collided(jerry_x, jerry_y, mousetrap_x, mousetrap_y))
    {
        lives--;
        setup_mousetrap();
        if (lives <= 0)
        {
            do_collided();
        }
    }
}


// Door
double door_x, door_y;
char Door = 'X';

// Initialises the doors x and y position randomly
void setup_door()
{
    door_x = 1 + rand() % (W - 2);
    door_y = 4 + rand() % (H - 4);
}

// If the doors initialised position is in a wall, re-initialise the position and draw
void draw_door()
{
    if (scrape_char(door_x, door_y) != '*')
    {
        draw_char(round(door_x), round(door_y), Door);
    }
    else
    {
        setup_door();
        draw_door();
    }
}

// When the score reaches 5 the door will be displayed on the screen.
// If the score is of 5 or higher and Jerry collides with the door the next level will display.
void update_door(int key)
{
    if (score >= 5)
    {
        draw_door();
    }

    if (score >= 5 && collided(jerry_x, jerry_y, door_x, door_y))
    {
        //next_level = true;
        do_collided();
    }  
}


// Fireworks
double firework_x, firework_y, firework_dx, firework_dy;
int t_x, t_y;
char Firework = 'F';

// Initialises the fireworks x and y position
void setup_firework()
{
    firework_x = jerry_x;
    firework_y = jerry_y;

    int t_x = tom_x;
    int t_y = tom_y;
    double d = sqrt(t_x*t_x + t_y*t_y);
    firework_dx = t_x * 0.1 / d;
    firework_dy = t_y * 0.1 / d;
}

void draw_firework()
{
    draw_char(round(firework_x), round(firework_y), Firework);
}

// Updates the fireworks position, with the direction being towards Tom
// Updates the score by +1 if the firework collides with Tom
void update_firework(int key)
{
    firework_x = firework_x + firework_dx;
    firework_y = firework_y + firework_dy;

    draw_char(round(firework_x), round(firework_y), Firework);

    if (round(firework_x) == t_x && round(firework_y) == t_y)
    {
        score++;
    }
}


void show_rooms(FILE * stream)
{
    // While stream has not reached the end of input:
    while (!feof (stream))
    {
        // Declares a variable of type char.
        char command;

        // Uses a single call to fscanf to (attempt to) read a char and four double 
        // values into command, and the double variables. Captures the value 
        // returned by fscanf for later use.
        int items_scanned = fscanf(stream, "%c %lf %lf %lf %lf", &command, &x_1, &y_1, &x_2, &y_2);

        // If the number of items scanned is 5:
        if (items_scanned == 5)
        {
            // If the command is 'W' a line will be drawn which represents the wall.
            if (command == 'W')
            {
                draw_line(x_1 * W, (y_1 * H) + SBH, x_2 * W, (y_2 * H) + SBH, wall);
            }
        }

        // Else if the number of items scanned is 3:
        else if (items_scanned == 3)
        {
            // If the command is 'T' a character will be drawn which represents Tom.
            if (command == 'T')
            {
                draw_char(x_1 * W, (y_1 * H) + SBH, Tom);
            }

            // Else if the command is 'J' a character will be drawn which represents Jerry.
            else if (command == 'J')
            {
                draw_char(x_1 * W, (y_1 * H) + SBH, Jerry);
            }
        }
    }
}


void draw_status()
{
    // Draw screen row 0 of the status bar.
    draw_formatted(0, 0, "Student Number: n%d    Score: %d    Lives: %d    Player: %c    Time: %.2d:%.2d",
    student_number, score, lives, player, timer_m, timer_s);
    
    // Draw screen row 2 of the status bar.
    draw_formatted(0, 2, "Cheese: %d    Mousetraps: %d    Fireworks: %d    Level: %d",
    cheese, mousetraps, fireworks, level);
    
    // Draw screen row 3 of the status bar.
    const int border = '-';
    draw_line(0, 3, (W - 1), 3, border);
}


// Draws all of the draw functions within a singular function
void draw_all()
{
    draw_status();
    draw_jerry();
    draw_tom();
    draw_cheese();
    draw_mousetrap();
    show_screen();
    // Draws a space in the current position before Jerry or Tom moves to the next position 
    draw_char(round(jerry_x), round(jerry_y), space);
    draw_char(round(tom_x), round(tom_y), space);   
}


void setup()
{
    srand(get_current_time());
    W = screen_width() - 1;
    H = screen_height() - 5;
    SBH = 4;
    setup_jerry();
    setup_tom();
    setup_cheese();
    setup_mousetrap();
    setup_door();
    // Stops the game from continuously displaying next levels. 
    next_level = false;
}


void loop()
{
    // Reads character from standard input stream 
    int key = get_char();
    
    // Pause game
    if (key == 'p')
    {
        game_pause = !game_pause;
        return;
    }

    // Quit game
    if (key == 'q')
    {
        game_over = true;
        return;
    }

    // Go to next level
    if (key == 'l')
    {
        next_level = true;
        return;
    }

    // Shoot firework
    if (key == 'f')
    {
        setup_firework();
        draw_firework();
        update_firework(key);
        return;
    }

    update_jerry(key);
    update_tom(key);
    update_cheese(key);
    update_mousetrap(key);
    update_door(key);
}


int main(int argc, char *argv[])
{
    const int delay = 10;
    setup_screen();
    setup();
    draw_status();

    // Initialises the start time
    start_time = time(0);

    // Initialises the pause duration as 0
    int pause_duration = 0;

    // Reads from the specified file
    for (int i = 1; i < argc; i++) 
    {
        FILE * stream = fopen(argv[i], "r");
        if (stream != NULL)
        {
            show_rooms(stream);
            fclose(stream);
        }
        show_screen();
        wait_char();
    }

    while (!game_over)
    {
        // Sets up the next level
        if (next_level == true && level + 1 < argc)
        {
            clear_screen();
            level++;

            FILE * stream = fopen(argv[level], "r");
            if (stream != NULL)
            {
                show_rooms(stream);
                fclose(stream);
            }
        }

        // Acquires the runtime of the game
        if (game_pause == false) timer_s = time(0) - start_time - pause_duration;

        // Determins how long the game was paused for
        if (game_pause == true) pause_duration = (time(0) - start_time - timer_s);
        
        // When the amount of seconds exceeds 59, the timer will be reset and the minutes will increment +1.
        if (timer_s > 59)
        {
            timer_s = 0;
            timer_m++;
            start_time = time(0);
        }

        draw_all();
        loop();
        timer_pause(delay);
    }
    return 0;
}