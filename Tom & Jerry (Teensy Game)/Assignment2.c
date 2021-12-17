#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include <avr/io.h> 
#include <avr/interrupt.h>
#include <util/delay.h>
#include <macros.h>
#include <graphics.h>
#include <cpu_speed.h>
#include <lcd.h>
#include <lcd_model.h>
#include <usb_serial.h>


// Define constants:
#define FREQ (8000000.0)
int const STATUS_BAR_HEIGHT = 8;


// Define global variables:
// Variables displayed in the status bar.
int level = 1;
int lives = 5;
int score = 0;
int timer_m = 0;
int timer_s = 0;

// Bool variables.
bool game_paused = false;
bool next_level_state = false;
bool firework_state;

// Variables that correlate to the debounce.
volatile uint8_t bit_count = 0;
volatile uint8_t is_pressed;

// Variables that correlate to the timer.
volatile int overflow_counter = 0;
volatile int cheese_counter = 0;
volatile int mousetrap_counter = 0;
volatile int milk_counter = 0;

// Buffer variable.
char buffer[32];

// Variables that correlate to Tom.
int x_tom, y_tom;
int dx_tom, dy_tom;
int x_sprite_tom[5], y_sprite_tom[5];

// Variables that correlate to Jerry.
int x_jerry, y_jerry;
int x_sprite_jerry[6], y_sprite_jerry[6];

// Variables that correlate to Cheese.
int cheese = 0;
int x_cheese[5], y_cheese[5];
int x_sprite_cheese[6], y_sprite_cheese[6];

// Variables that correlate to Mousetrap.
int mousetrap = 0;
int x_mousetrap[5], y_mousetrap[5];
int x_sprite_mousetrap[9], y_sprite_mousetrap[9];

// Variables that correlate to Door.
int x_door, y_door;
int x_sprite_door[15], y_sprite_door[15];

// Variables that correlate to Firework.
int firework = 0;
int x_firework[20], y_firework[20];

// Variables that correlate to Milk.
int milk = 0;
int x_milk[1], y_milk[1];
int x_sprite_milk[8], y_sprite_milk[8];


// Declaration of next_level and game_over functions located here to avoid order of function problems.
void next_level(void);
void game_over(void);



// Interrupt Service Routine for Timer 0.
ISR(TIMER0_OVF_vect)
{
    // Debouncing of the Right button.
    bit_count = (bit_count << 1);
    volatile uint8_t mask = 0b01111111;
    bit_count = (bit_count & mask);
    bit_count = (bit_count | BIT_IS_SET(PINF, 5));
    
    if (bit_count == mask)
    {
        is_pressed = 1;
    }

    else if (bit_count == 0)
    {
        is_pressed = 0;
    }
}



// Creates a timer for a given parameter.
double now(int overflow_counter)
{
    double time = (overflow_counter * 65536.0 + TCNT3) * 64.0 / FREQ;
    return time;
}

// Processes the amount of time the TeensyPewPew has been running for.
// Creates a timer.
void process_time(void)
{
	timer_s = now(overflow_counter);

	if (timer_s == 60)
    {
		timer_m++;
		timer_s = 0;
		overflow_counter = 0;
	}
}

// Interrupt Service Routine for Timer 3.
ISR(TIMER3_OVF_vect)
{
    // If the game is not paused.
    if (game_paused == false)
    {
        overflow_counter++;

        if (cheese < 5)
        {
            cheese_counter++;
        }

        if (mousetrap < 5)
        {
            mousetrap_counter++;
        }

        if (milk < 1)
        {
            milk_counter++;
        }
    }
}



// Removes the array of a given character and index specified.
void array_remove(int *array, int index)
{
    int i;

    for (i = index; i < cheese; i++)
    {
        array[i] = array[i + 1];
    }
}




void setup_sprites(void)
{
    // Tom sprite
    int *pointer_x_tom = x_sprite_tom;
    *pointer_x_tom++ = -1; *pointer_x_tom++ = 0; *pointer_x_tom++ = 1; *pointer_x_tom++ = 0;
    *pointer_x_tom++ = 0;

    int *pointer_y_tom = y_sprite_tom;
    *pointer_y_tom++ = -1; *pointer_y_tom++ = -1; *pointer_y_tom++ = -1; *pointer_y_tom++ = 0;
    *pointer_y_tom++ = 1;


    // Jerry sprite
    int *pointer_x_jerry = x_sprite_jerry;
    *pointer_x_jerry++ = -1; *pointer_x_jerry++ = 0; *pointer_x_jerry++ = 1; *pointer_x_jerry++ = 0;
    *pointer_x_jerry++ = 0; *pointer_x_jerry++ = -1;

    int *pointer_y_jerry = y_sprite_jerry;
    *pointer_y_jerry++ = -1; *pointer_y_jerry++ = -1; *pointer_y_jerry++ = -1; *pointer_y_jerry++ = 0;
    *pointer_y_jerry++ = 1; *pointer_y_jerry++ = 1;


    // Cheese sprite
    int *pointer_x_cheese = x_sprite_cheese;
    *pointer_x_cheese++ = -1; *pointer_x_cheese++ = 0; *pointer_x_cheese++ = 1; *pointer_x_cheese++ = 1;
    *pointer_x_cheese++ = 1; *pointer_x_cheese++ = 0;

    int *pointer_y_cheese = y_sprite_cheese;
    *pointer_y_cheese++ = 1; *pointer_y_cheese++ = 1; *pointer_y_cheese++ = 1; *pointer_y_cheese++ = 0;
    *pointer_y_cheese++ = -1; *pointer_y_cheese++ = 0;


    // Mousetrap sprite
    int *pointer_x_mousetrap = x_sprite_mousetrap;
    *pointer_x_mousetrap++ = 0; *pointer_x_mousetrap++ = -1; *pointer_x_mousetrap++ = 0; *pointer_x_mousetrap++ = 1;
    *pointer_x_mousetrap++ = -1; *pointer_x_mousetrap++ = 1; *pointer_x_mousetrap++ = -1; *pointer_x_mousetrap++ = 0;
    *pointer_x_mousetrap++ = 1;

    int *pointer_y_mousetrap = y_sprite_mousetrap;
    *pointer_y_mousetrap++ = -2; *pointer_y_mousetrap++ = -1; *pointer_y_mousetrap++ = -1; *pointer_y_mousetrap++ = -1;
    *pointer_y_mousetrap++ = 0; *pointer_y_mousetrap++ = 0; *pointer_y_mousetrap++ = 1; *pointer_y_mousetrap++ = 1;
    *pointer_y_mousetrap++ = 1;


    // Milk sprite
    int *pointer_x_milk = x_sprite_milk;
    *pointer_x_milk++ = -1; *pointer_x_milk++ = -1; *pointer_x_milk++ = -1; *pointer_x_milk++ = 0;
    *pointer_x_milk++ = 0; *pointer_x_milk++ = 1; *pointer_x_milk++ = 1; *pointer_x_milk++ = 1;

    int *pointer_y_milk = y_sprite_milk;
    *pointer_y_milk++ = -1; *pointer_y_milk++ = 0; *pointer_y_milk++ = 1; *pointer_y_milk++ = 0;
    *pointer_y_milk++ = 1; *pointer_y_milk++ = 1; *pointer_y_milk++ = 0; *pointer_y_milk++ = -1;
}

void setup_door_sprite(void)
{
    // Door sprite
    int *pointer_x_door = x_sprite_door;
    *pointer_x_door++ = 0; *pointer_x_door++ = 1; *pointer_x_door++ = 1; *pointer_x_door++ = 1;
    *pointer_x_door++ = 0; *pointer_x_door++ = -1; *pointer_x_door++ = -2; *pointer_x_door++ = -2;
    *pointer_x_door++ = -2; *pointer_x_door++ = -2; *pointer_x_door++ = -2; *pointer_x_door++ = -1;
    *pointer_x_door++ = 0; *pointer_x_door++ = 1; *pointer_x_door++ = 1;

    int *pointer_y_door = y_sprite_door;
    *pointer_y_door++ = 0; *pointer_y_door++ = 0; *pointer_y_door++ = -1; *pointer_y_door++ = -2;
    *pointer_y_door++ = -2; *pointer_y_door++ = -2; *pointer_y_door++ = -2; *pointer_y_door++ = -1;
    *pointer_y_door++ = 0; *pointer_y_door++ = 1; *pointer_y_door++ = 2; *pointer_y_door++ = 2;
    *pointer_y_door++ = 2; *pointer_y_door++ = 2; *pointer_y_door++ = 1;
}




// Functions that correlate to Tom:
void setup_tom(void)
{
    x_tom = (LCD_X - 10); // Changed to allow for fluidity in toms movement.
    y_tom = (LCD_Y - 9);

    dx_tom = 1;
    dy_tom = 1;
}

void draw_tom(void)
{
    draw_pixel(x_tom, y_tom, FG_COLOUR);
    draw_pixel(x_tom + 1, y_tom, FG_COLOUR);
    draw_pixel(x_tom + 2, y_tom, FG_COLOUR);
    draw_pixel(x_tom + 1, y_tom + 1, FG_COLOUR);
    draw_pixel(x_tom + 1, y_tom + 2, FG_COLOUR);
}

void update_tom(void)
{
    // If the game is not paused.
    if (game_paused) return;

    x_tom += dx_tom;
    y_tom += dy_tom;

    // If Toms position is located on the edge of either x-axis, reverse his movement (bounce). 
    if (x_tom <= 0 || x_tom >= (-3 + LCD_X))
    {
        dx_tom = -dx_tom;
    }

    // If Toms position is located on the edge of either y-axis, reverse his movement (bounce).
    if (y_tom <= 9 || y_tom >= (-3 + LCD_Y))
    {
        dy_tom = -dy_tom;
    }
}


// Functions that correlate to Jerry:
void setup_jerry(void)
{
    x_jerry = 0;
    y_jerry = (STATUS_BAR_HEIGHT + 1);
}

void draw_jerry()
{
    draw_pixel(x_jerry, y_jerry, FG_COLOUR);
    draw_pixel(x_jerry + 1, y_jerry, FG_COLOUR);
    draw_pixel(x_jerry + 2, y_jerry, FG_COLOUR);
    draw_pixel(x_jerry + 1, y_jerry + 1, FG_COLOUR);
    draw_pixel(x_jerry, y_jerry + 2, FG_COLOUR);
    draw_pixel(x_jerry + 1, y_jerry + 2, FG_COLOUR);
}

void jerry_movement(void)
{
    // If the switch is closed and Jerry is not already in the top row of pixels,
    // move Jerry up one pixel.
    if (BIT_IS_SET(PIND, 1) && y_jerry > 9)
    {
        y_jerry -= 1;
    }

    // If the switch is closed and Jerry is not already in the bottom row of pixels,
    // move Jerry down one pixel.
    if (BIT_IS_SET(PINB, 7) && y_jerry < 45)
    {
        y_jerry += 1;
    }

    // If the switch is closed and Jerry is not already in the left-most row of pixels,
    // move Jerry left one pixel.
    if (BIT_IS_SET(PINB, 1) && x_jerry > 0)
    {
        x_jerry -= 1;
    }

    // If the switch is closed and Jerry is not already in the right-most row of pixels,
    // move Jerry right one pixel.
    if (BIT_IS_SET(PIND, 0) && x_jerry < 81)
    {
        x_jerry += 1;
    }

    // Counting over the pixels of Jerry.
    for (int i = 0; i < 6; i++)
    {
        // Counting over the pixels of Tom.
        for (int j = 0; j < 5; j++)
        {
            // Detect collison between Jerry and Tom.
            if (x_jerry + x_sprite_jerry[i] == x_tom + x_sprite_tom[j] && y_jerry + y_sprite_jerry[i] == y_tom + y_sprite_tom[j])
            {
                lives--;

                while (lives <= 0)
                {
                    game_over();
                }
            }
        }
    }
}


// Functions that correlate to Cheese:
void spawn_cheese(void)
{
    // Create a seperate timer for the cheese_counter.
    float cheese_spawn = now(cheese_counter);
    
    // If cheese is less than 5 and cheese_counter is at an interval of 2 seconds, spawn cheese.
    if (cheese < 5 && (fmod(cheese_spawn, 2) > -0.045 && fmod(cheese_spawn, 2) < 0.045) && cheese_spawn >= 2)
    {
        x_cheese[cheese] = rand() % (-10 + LCD_X) + 7;
        y_cheese[cheese] = rand() % (-12 + LCD_Y) + 9;

        cheese++;
    }

    // Draw cheese.
    for (int i = 0; i < cheese; i++)
    {
        draw_pixel(x_cheese[i] + 2, y_cheese[i], FG_COLOUR);
        draw_pixel(x_cheese[i] + 1, y_cheese[i] + 1, FG_COLOUR);
        draw_pixel(x_cheese[i] + 2, y_cheese[i] + 1, FG_COLOUR);
        draw_pixel(x_cheese[i], y_cheese[i] + 2, FG_COLOUR);
        draw_pixel(x_cheese[i] + 1, y_cheese[i] + 2, FG_COLOUR);
        draw_pixel(x_cheese[i] + 2, y_cheese[i] + 2, FG_COLOUR);

    }


    for (int i = 0; i < cheese; i++)
    {
        // Counting over the pixels of Jerry.
        for (int j = 0; j < 6; j++)
        {
            // Counting over the pixels of cheese.
            for (int k = 0; k < 6; k++)
            {
                // Detect collison between Jerry and the array number of cheese.
                if (x_jerry + x_sprite_jerry[j] == x_cheese[i] + x_sprite_cheese[k] && y_jerry + y_sprite_jerry[j] == y_cheese[i] + y_sprite_cheese[k])
                {
                    array_remove(x_cheese, i);
                    array_remove(y_cheese, i);

                    cheese--;
                    score++;
                }
            }
        }
    }
}


// Functions that correlate to Mousetrap:
void spawn_mousetrap(void)
{
    // Create a seperate timer for the mousetrap_counter.
    float mousetrap_spawn = now(mousetrap_counter);
    
    // If mousetrap is less than 5 and mousetrap_counter is at an interval of 3 seconds, spawn mousetrap.
    if (mousetrap < 5 && (fmod(mousetrap_spawn, 3) > -0.045 && fmod(mousetrap_spawn, 3) < 0.045) && mousetrap_spawn >= 3)
    {
        x_mousetrap[mousetrap] = x_tom;
        y_mousetrap[mousetrap] = y_tom;

        mousetrap++;
    }

    // Draw mousetrap.
    for (int i = 0; i < mousetrap; i++)
    {
        draw_pixel(x_mousetrap[i] + 1, y_mousetrap[i], FG_COLOUR);
        draw_pixel(x_mousetrap[i], y_mousetrap[i] + 1, FG_COLOUR);
        draw_pixel(x_mousetrap[i] + 1, y_mousetrap[i] + 1, FG_COLOUR);
        draw_pixel(x_mousetrap[i] + 2, y_mousetrap[i] + 1, FG_COLOUR);
        draw_pixel(x_mousetrap[i], y_mousetrap[i] + 2, FG_COLOUR);
        draw_pixel(x_mousetrap[i] + 2, y_mousetrap[i] + 2, FG_COLOUR);
        draw_pixel(x_mousetrap[i], y_mousetrap[i] + 3, FG_COLOUR);
        draw_pixel(x_mousetrap[i] + 1, y_mousetrap[i] + 3, FG_COLOUR);
        draw_pixel(x_mousetrap[i] + 2, y_mousetrap[i] + 3, FG_COLOUR);
    }

    for (int i = 0; i < mousetrap; i++)
    {
        // Counting over the pixels of Jerry.
        for (int j = 0; j < 6; j++)
        {
            // Counting over the pixels of mousetrap.
            for (int k = 0; k < 9; k++)
            {
                // Detect collison between Jerry and the array number of mousetrap.
                if (x_jerry + x_sprite_jerry[j] == x_mousetrap[i] + x_sprite_mousetrap[k] && y_jerry + y_sprite_jerry[j] == y_mousetrap[i] + y_sprite_mousetrap[k])
                {
                    array_remove(x_mousetrap, i);
                    array_remove(y_mousetrap, i);

                    mousetrap--;
                    lives--;

                    while (lives <= 0)
                    {
                        game_over();
                    }
                }
            }
        }
    }
}


// Functions that correlate to Door:
void setup_door(void)
{
    x_door = rand() % (-10 + LCD_X) + 3;
    y_door = rand() % (LCD_Y - 10) + 9;
}

void draw_door(void)
{
    draw_pixel(x_door, y_door, FG_COLOUR);
    draw_pixel(x_door + 1, y_door, FG_COLOUR);
    draw_pixel(x_door + 2, y_door, FG_COLOUR);
    draw_pixel(x_door + 3, y_door, FG_COLOUR);
    draw_pixel(x_door, y_door + 1, FG_COLOUR);
    draw_pixel(x_door + 3, y_door + 1, FG_COLOUR);
    draw_pixel(x_door, y_door + 2, FG_COLOUR);
    draw_pixel(x_door + 2, y_door + 2, FG_COLOUR);
    draw_pixel(x_door + 3, y_door + 2, FG_COLOUR);
    draw_pixel(x_door, y_door + 3, FG_COLOUR);
    draw_pixel(x_door + 3, y_door + 3, FG_COLOUR);
    draw_pixel(x_door, y_door + 4, FG_COLOUR);
    draw_pixel(x_door + 1, y_door + 4, FG_COLOUR);
    draw_pixel(x_door + 2, y_door + 4, FG_COLOUR);
    draw_pixel(x_door + 3, y_door + 4, FG_COLOUR);
}

void spawn_door(void)
{
    if (score >= 5)
    {
        draw_door();
        setup_door_sprite();
    }

    // Counting over the pixels of Jerry.
    for (int j = 0; j < 6; j++)
    {
        // Counting over the pixels of door.
        for (int k = 0; k < 15; k++)
        {
            // Detect collison between Jerry and the door.
            if (x_jerry + x_sprite_jerry[j] == x_door + x_sprite_door[k] && y_jerry + y_sprite_jerry[j] == y_door + y_sprite_door[k])
            {
                //next_level(); Not working properly (unknown issue)
                game_over();
            }
        }
    }
}


// Functions that correlate to Firework:
void draw_firework(void)
{
	double dx = 1;
	double dy = 1;

    // If the firework state is true and the amount of fireworks is 20 or less.
	if (firework_state && firework <= 20)
	{
		x_firework[firework] = x_jerry;
		y_firework[firework] = y_jerry;

		firework_state = false;
		firework++;
	}

    // Moving the firework towards Toms position.
	for (int i = 0; i < firework; i++)
	{
		if (x_tom > x_firework[i]) dx = fabs(dx);
		else dx = fabs(dx) * (-1);
		x_firework[i] += dx;

		if (y_tom > y_firework[i]) dy = fabs(dy);
		else dy = fabs(dy) * (-1);
		y_firework[i] += dy;
	}

    // Draw firework
	if (firework > 0)
	{
        for (int i = 0; i < firework; i++)
        {
            draw_pixel(x_firework[i], y_firework[i], FG_COLOUR);
        }
    }

    for (int i = 0; i < firework; i++)
    {
        // Counting over the pixels of Tom.
        for (int j = 0; j < 5; j++)
        {
            if (x_firework[i] == floor(x_tom) + x_sprite_tom[j] && y_firework[i] == floor(y_tom) + y_sprite_tom[j])
            {
                array_remove(x_firework, i);

                firework--;
                score++;

                x_tom = (LCD_X - 10);
                y_tom = (LCD_Y - 9);
            }
        }
    }
}

void spawn_firework(void)
{
    // If score is 3 or greater and the Centre switch is closed.
    if (score >= 3 && BIT_IS_SET(PINB, 0))
    {
        firework_state = true;
        draw_firework();
    }
}


// Functions that correlate to Milk:
void spawn_milk(void)
{
    // Create a seperate timer for the milk_counter.
    float milk_spawn = now(milk_counter);
    
    // If milk is less than 1 and milk_counter is at an interval of 5 seconds, spawn milk.
    if (milk < 1 && (fmod(milk_spawn, 5) > -0.045 && fmod(milk_spawn, 5) < 0.045) && milk_spawn >= 5)
    {
        x_milk[milk] = x_tom;
        y_milk[milk] = y_tom;

        milk++;
    }

    // Draw milk.
    for (int i = 0; i < milk; i++)
    {
        draw_pixel(x_milk[i], y_milk[i], FG_COLOUR);
        draw_pixel(x_milk[i] + 2, y_milk[i], FG_COLOUR);
        draw_pixel(x_milk[i], y_milk[i] + 1, FG_COLOUR);
        draw_pixel(x_milk[i] + 1, y_milk[i] + 1, FG_COLOUR);
        draw_pixel(x_milk[i] + 2, y_milk[i] + 1, FG_COLOUR);
        draw_pixel(x_milk[i], y_milk[i] + 2, FG_COLOUR);
        draw_pixel(x_milk[i] + 1, y_milk[i] + 2, FG_COLOUR);
        draw_pixel(x_milk[i] + 2, y_milk[i] + 2, FG_COLOUR);
    }
}




// Draws the starting screen.
void start_screen(void)
{
    clear_screen();
    draw_string(10, 0, "Noah Treichel", FG_COLOUR);
    draw_string(20, 10, "n10533206", FG_COLOUR);
    draw_string(4, 25, "Tom and Jerry's", FG_COLOUR);
    draw_string(5, 35, "Frantic Pursuit", FG_COLOUR);
    show_screen();
}


// Draws the game setup screen.
void game_setup(void)
{
    clear_screen();

    // Draw status bar.
    sprintf(buffer, "%d", level);
    draw_string(2, 0, buffer, FG_COLOUR);

    sprintf(buffer, "%d", lives);
    draw_string(20, 0, buffer, FG_COLOUR);

    sprintf(buffer, "%d", score);
    draw_string(38, 0, buffer, FG_COLOUR);
    
    sprintf(buffer, "%.2d:%.2d", timer_m, timer_s);
    draw_string(56, 0, buffer, FG_COLOUR);

    draw_line(0, 8, 84, 8, FG_COLOUR);

    // Draw characters and walls.
    draw_tom();  
    draw_jerry();
    draw_line(18, 15, 13, 25, FG_COLOUR);
    draw_line(25, 35, 25, 45, FG_COLOUR);
    draw_line(45, 10, 60, 10, FG_COLOUR);
    draw_line(58, 25, 72, 30, FG_COLOUR);

    // Spawn cheese, mousetrap and door.
    spawn_cheese();
    spawn_mousetrap();
    spawn_door();

    // Spawn firework.
    spawn_firework();
    
    // If the next level state is true, spawn milk.
    if (next_level_state == true)
    {
        spawn_milk();
    }

    show_screen();
}


// Pauses and unpauses the game.
void pause_game(void)
{
    if (BIT_IS_SET(PINF, 5))
    {
		game_paused = !game_paused;
	}
}


// Creates the next level.
void next_level(void)
{
    next_level_state = true;

    clear_screen();
    game_setup();
    setup_tom();
    setup_jerry();
    level = 2;
    lives = 5;
    score = 0;
    overflow_counter = 0;
    cheese = 0;
    mousetrap = 0;
}


// Draws the game over screen.
void game_over(void)
{
    clear_screen();
    draw_string(20, 5, "Game Over", FG_COLOUR);
    draw_string(26, 20, "Thanks", FG_COLOUR);
    draw_string(15, 30, "For Playing", FG_COLOUR);
    show_screen();
}



// Setup the usb serial.
void setup_usb_serial(void)
{
	usb_init();

	while (!usb_configured())
    {
		// Block until USB is ready.
	}
}

// Transmits a string via usb_serial.
void usb_serial_send(char * message)
{
	// Cast to avoid "error: pointer targets in passing argument 1 
	// of 'usb_serial_write' differ in signedness"
	usb_serial_write((uint8_t *) message, strlen(message));
}

// Transmits an integer value via usb_serial.
void usb_serial_send_int(int value)
{
	static char buffer[8];
	snprintf(buffer, sizeof(buffer), "%d", value);
	usb_serial_send(buffer);
}




void setup(void)
{
	set_clock_speed(CPU_8MHz);
	lcd_init(LCD_DEFAULT_CONTRAST);

    // Initialise Timer 0 in normal mode.
    TCCR0A = 0;
	TCCR0B = 4;

	// Enable the timer overflow interrupt for Timer 0.
    TIMSK0 = 1;


    // Initialise timer 3 in normal mode.
	TCCR3A = 0;
	TCCR3B = 3;

    // Enable the timer overflow interrupt for Timer 3.
	TIMSK3 = 1;

	// Enable timer overflow, and turn on interrupts.
	sei();

    // Enable input from the Left and Right button.
    CLEAR_BIT(DDRF, 6);
    CLEAR_BIT(DDRF, 5);

    // Enable input from the Left, Right, Up, Down and Centre switches of the joystick.
    CLEAR_BIT(DDRB, 1);
    CLEAR_BIT(DDRD, 0);
    CLEAR_BIT(DDRD, 1);
    CLEAR_BIT(DDRB, 7);
    CLEAR_BIT(DDRB, 0);

    // Set up sprites.
    setup_sprites();

    // Set up Tom and Jerry.
    setup_tom();
    setup_jerry();

    // Set up Door.
    setup_door();

    // Initialise the USB serial.
    setup_usb_serial();
}


void process(void)
{   
    process_time();
    game_setup();
    update_tom();
    jerry_movement();
    /*pause_game(); Commented out due to the functionality not being consistent when the button is pressed.
                    Works completely normal when called in the serial console.*/               
    
    if (BIT_IS_SET(PINF, 6))
    {
        next_level();
    }
}


int main(void)
{
    setup();

    // While the right button is not pressed.
    while (!BIT_IS_SET(PINF, 5))
    {
        start_screen();
        //overflow_counter = 0;
    }
    // Generate random seed.
    srand(TCNT3);  
    
    while (1)
    {
        process();
        //_delay_ms(10);

        if (usb_serial_available())
        {
            // Read usb port.
            int c = usb_serial_getchar();
            
            // Check character and perform an action.
            // Jerry movement
            if (c == 'w' && y_jerry > 9)
            {
                y_jerry -= 1;
            }

            if (c == 'a' && x_jerry > 0)
            {
                x_jerry -= 1;
            }

            if (c == 's' && y_jerry < 45)
            {
                y_jerry += 1;
            }

            if (c == 'd' && x_jerry < 81)
            {
                x_jerry += 1;
            }

            // Pause
            if (c == 'p')
            {
                game_paused = !game_paused;
            }

            // Firing fireworks
            if (c == 'f')
            {
                // ???
            }

            // Change level
            if (c == 'l')
            {
                next_level();
            }


            // Game state information
            if (c == 'i')
            {
			    snprintf(buffer, sizeof(buffer), "Timestamp: %.2d:%.2d \r\n", timer_m, timer_s);
                usb_serial_send(buffer);

                snprintf(buffer, sizeof(buffer), "Current Level: %d \r\n", level);
                usb_serial_send(buffer);

                snprintf(buffer, sizeof(buffer), "Jerry's Lives: %d \r\n", lives);
                usb_serial_send(buffer);

                snprintf(buffer, sizeof(buffer), "Score: %d \r\n", score);
                usb_serial_send(buffer);

                snprintf(buffer, sizeof(buffer), "Fireworks on Screen: %d \r\n", firework);
                usb_serial_send(buffer);

                snprintf(buffer, sizeof(buffer), "Mousetraps on Screen: %d \r\n", mousetrap);
                usb_serial_send(buffer);

                snprintf(buffer, sizeof(buffer), "Cheese on Screen: %d \r\n", cheese);
                usb_serial_send(buffer);

                //snprintf(buffer, sizeof(buffer), "Cheese Consumed: %d \r\n", ???);
                //usb_serial_send(buffer);

                //snprintf(buffer, sizeof(buffer), "Is Jerry in Super Mode: %c \r\n", ???);
                //usb_serial_send(buffer);

                //snprintf(buffer, sizeof(buffer), "Is the Game Paused: %c \r\n", ???);
                //usb_serial_send(buffer);
            }			              
        }
    }
    
    return 0;
}

