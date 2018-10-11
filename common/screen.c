#include "screen.h"

#define BLACK         0x0
#define BLUE          0x1
#define GREEN         0x2
#define RED           0x4
#define WHITE         0x7
#define FG_COLOR      WHITE
#define BG_COLOR      BLACK

#define MAX_COL       VGA_WIDTH	
#define MAX_ROW       VGA_HEIGHT
#define VRAM_SIZE     (MAX_COL*MAX_ROW)
#define DEF_VRAM_BASE 0xb8000

extern int cursor_x;
extern int cursor_y;

char lshift_pressed = 0;
char rshift_pressed = 0;
char caps_pressed = 0;

#define PUT(c) ( ((unsigned short *) (DEF_VRAM_BASE)) \
	[(cursor_y * MAX_COL) + cursor_x] = (FG_COLOR << 8 | (BG_COLOR << 12)) | (c))

static void cons_putc(int c) {
    switch (c) {
    case '\t':
        do {
            cons_putc(' ');
        } while ((cursor_x % 4) != 0);
        break;
    case '\r':
        cursor_x = 0;
        break;
    case '\n':
        cursor_y += 1;
        if (cursor_y >= MAX_ROW) {
            cursor_y = 0;
        }
        break;
    case '\b':
        if (cursor_x > 0) {
            cursor_x -= 1;
            PUT(' ');
        }
        break;
    default:
        PUT(c);
        cursor_x += 1;
        if (cursor_x >= MAX_COL) {
            cursor_x = 0;
            cursor_y += 1;
            if (cursor_y >= MAX_ROW) {
                cursor_y = 0;
            }
        }
    };
}

void _putchar(int c) {
    if (c == '\n') 
        cons_putc('\r');
    cons_putc(c);
}

void putchar(int c) {
    _putchar(c);
    update_cursor();
}

int puts(const char* s) {
	char c;
	int i = -1;
	while ((c = s[++i]) != '\0') {
		_putchar(c);
	}
	update_cursor();
	return i;
}

void clear_screen() {
    cursor_x = 0;
    cursor_y = 0;
    
    for (int i = 0; i < VRAM_SIZE; i++)
        cons_putc(' ');
    
    cursor_x = 0;
    cursor_y = 0;
    move_cursor(0, 0);
}

int getchar() {
	for (;;) {
		unsigned char status = inb(0x64);
		if ((status & 1) && ((status & 0x20) == 0)) {
			int c = inb(0x60);
			char shift_pressed = lshift_pressed | rshift_pressed;
			switch (c) {
			case 2:
				if (shift_pressed) {
					return '!';
				}
				return '1';
			case 3:
				if (shift_pressed) {
					return '@';
				}
				return '2';
			case 4:
				if (shift_pressed) {
					return '#';
				}
				return '3';
			case 5:
				if (shift_pressed) {
					return '$';
				}
				return '4';
			case 6:
				if (shift_pressed) {
					return '%';
				}
				return '5';
			case 7:
				if (shift_pressed) {
					return '^';
				}
				return '6';
			case 8:
				if (shift_pressed) {
					return '&';
				}
				return '7';
			case 9:
				if (shift_pressed) {
					return '*';
				}
				return '8';
			case 0xa:
				if (shift_pressed) {
					return '(';
				}
				return '9';
			case 0xb:
				if (shift_pressed) {
					return ')';
				}
				return '0';
			case 0xc:
				if (shift_pressed) {
					return '_';
				}
				return '-';
			case 0xd:
				if (shift_pressed) {
					return '+';
				}
				return '=';
			case 0xe: // Backspace press
				return '\b';
			case 0xf: // Tab press
				return '\t';
			case 0x10:
				if (shift_pressed | caps_pressed) {
					return 'Q';
				}
				return 'q';
			case 0x11:
				if (shift_pressed | caps_pressed) {
					return 'W';
				}
				return 'w';
			case 0x12:
				if (shift_pressed | caps_pressed) {
					return 'E';
				}
				return 'e';
			case 0x13:
				if (shift_pressed | caps_pressed) {
					return 'R';
				}
				return 'r';
			case 0x14:
				if (shift_pressed | caps_pressed) {
					return 'T';
				}
				return 't';
			case 0x15:
				if (shift_pressed | caps_pressed) {
					return 'Y';
				}
				return 'y';
			case 0x16:
				if (shift_pressed | caps_pressed) {
					return 'U';
				}
				return 'u';
			case 0x17:
				if (shift_pressed | caps_pressed) {
					return 'I';
				}
				return 'i';
			case 0x18:
				if (shift_pressed | caps_pressed) {
					return 'O';
				}
				return 'o';
			case 0x19:
				if (shift_pressed | caps_pressed) {
					return 'P';
				}
				return 'p';
			case 0x1a:
				if (shift_pressed) {
					return '{';
				}
				return '[';
			case 0x1b:
				if (shift_pressed) {
					return '}';
				}
				return ']';
			case 0x1c: // Enter press
				return '\n';
			case 0x1d: // Lctrl press
				break;
			case 0x1e:
				if (shift_pressed | caps_pressed) {
					return 'A';
				}
				return 'a';
			case 0x1f:
				if (shift_pressed | caps_pressed) {
					return 'S';
				}
				return 's';
			case 0x20:
				if (shift_pressed | caps_pressed) {
					return 'D';
				}
				return 'd';
			case 0x21:
				if (shift_pressed | caps_pressed) {
					return 'F';
				}
				return 'f';
			case 0x22:
				if (shift_pressed | caps_pressed) {
					return 'G';
				}
				return 'g';
			case 0x23:
				if (shift_pressed | caps_pressed) {
					return 'H';
				}
				return 'h';
			case 0x24:
				if (shift_pressed | caps_pressed) {
					return 'J';
				}
				return 'j';
			case 0x25:
				if (shift_pressed | caps_pressed) {
					return 'K';
				}
				return 'k';
			case 0x26:
				if (shift_pressed | caps_pressed) {
					return 'L';
				}
				return 'l';
			case 0x27:
				if (shift_pressed) {
					return ':';
				}
				return ';';
			case 0x28:
				if (shift_pressed) {
					return '\"';
				}
				return '\'';
			case 0x29:
				if (shift_pressed) {
					return '~';
				}
				return '`';
			case 0x2a: // Lshift press
				lshift_pressed = 1;
				break;
			case 0x2b:
				if (shift_pressed) {
					return '|';
				}
				return '\\';
			case 0x2c:
				if (shift_pressed | caps_pressed) {
					return 'Z';
				}
				return 'z';
			case 0x2d:
				if (shift_pressed | caps_pressed) {
					return 'X';
				}
				return 'x';
			case 0x2e:
				if (shift_pressed | caps_pressed) {
					return 'C';
				}
				return 'c';
			case 0x2f:
				if (shift_pressed | caps_pressed) {
					return 'V';
				}
				return 'v';
			case 0x30:
				if (shift_pressed | caps_pressed) {
					return 'B';
				}
				return 'b';
			case 0x31:
				if (shift_pressed | caps_pressed) {
					return 'N';
				}
				return 'n';
			case 0x32:
				if (shift_pressed | caps_pressed) {
					return 'M';
				}
				return 'm';
			case 0x33:
				if (shift_pressed) {
					return '<';
				}
				return ',';
			case 0x34:
				if (shift_pressed) {
					return '>';
				}
				return '.';
			case 0x35:
				if (shift_pressed) {
					return '?';
				}
				return '/';
			case 0x36: // Rshift press
				rshift_pressed = 1;
				break;
			case 0x37: // Numpad '*' press
				break;
			case 0x38: // Lalt press
				break;
			case 0x39: // Space press
				return ' ';
			case 0x3a: // Caps press
				caps_pressed = !caps_pressed;
				break;
			case 0x48: // Arrow up press
				break;
			case 0x4b: // Arrow left press
				break;
			case 0x4d: // Arrow right press
				break;
			case 0x50: // Arrow down press
				break;	
			case 0xaa: // Lshift release
				lshift_pressed = 0;
				break;
			case 0xb6: // Rshift release
				rshift_pressed = 0;
				break;
			case 0xba: // Caps release
				break;
			}
		}
	}
}

char* gets(char* s) {
	char c;
	int i = 0;
	while ((c = getchar()) != '\n') {
		putchar(c);
		s[i++] = c;
	}
	s[i] = '\0';
	return s;
}

