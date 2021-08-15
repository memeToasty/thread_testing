#include <3ds.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#define SCREEN_WIDTH  400
#define SCREEN_HEIGHT 240

#define MAIN_MENU_OFFSET 1

//Global variables//

PrintConsole *top;
PrintConsole *bottom;

int thread_count = 2;
int selector = 1;
int menu_count = 2;
int array_length = 100;

// function Definitions//

void merge_sort(int i, int j, int a[], int aux[]);
void clearConsoles(void);

/**
 * @brief handles MenuSelection
 * @param keyDir true = Up ; false = Down
 */
void handleSelector(bool keyDir);

//---------------------------------------------------------------------------------
int main(int argc, char* argv[]) {
//---------------------------------------------------------------------------------
	//Matrix containing the name of each key. Useful for printing when a key is pressed
	char keysNames[32][32] = {
		"KEY_A", "KEY_B", "KEY_SELECT", "KEY_START",
		"KEY_DRIGHT", "KEY_DLEFT", "KEY_DUP", "KEY_DDOWN",
		"KEY_R", "KEY_L", "KEY_X", "KEY_Y",
		"", "", "KEY_ZL", "KEY_ZR",
		"", "", "", "",
		"KEY_TOUCH", "", "", "",
		"KEY_CSTICK_RIGHT", "KEY_CSTICK_LEFT", "KEY_CSTICK_UP", "KEY_CSTICK_DOWN",
		"KEY_CPAD_RIGHT", "KEY_CPAD_LEFT", "KEY_CPAD_UP", "KEY_CPAD_DOWN"
	};

	// Initialize RNG
	time_t t;
	srand((unsigned) time(&t));

	// Init libs
	gfxInitDefault();
	top = consoleInit(GFX_BOTTOM, NULL);
	bottom = consoleInit(GFX_TOP, NULL);

	consoleSelect(top);

	// Main loop
	while (aptMainLoop())
	{
		hidScanInput();

		// Respond to user input
		u32 kDown = hidKeysDown();
		if (kDown & KEY_START) break; // break in order to return to hbmenu
		if (kDown & KEY_DUP) handleSelector(true);
		if (kDown & KEY_DDOWN) handleSelector(false);
	
		printf("\x1b[1;3H3DS Thread testing");
		printf("\x1b[2;3HThread Count: %i", thread_count);
		printf("\x1b[3;3HArray Length: %i", array_length);

		//Print Selector
		printf("\x1b[%i;1H->", selector + MAIN_MENU_OFFSET);

		// Flush and swap framebuffers
		gfxFlushBuffers();
		gfxSwapBuffers();

		//Wait for VBlank
		gspWaitForVBlank();
		
	}

	// Deinit libs
	gfxExit();
	return 0;
}

void handleSelector(bool keyDir) {
	clearConsoles();
	if (keyDir) {
		if (selector - 1 < 1) selector = menu_count;
		else selector--;
	} else {
		if (selector + 1 > menu_count) selector = 1;
		else selector++;
	}
}

// function to sort the subsection a[i .. j] of the array a[]
void merge_sort(int i, int j, int a[], int aux[]) {
    if (j <= i) {
        return;     // the subsection is empty or a single element
    }
    int mid = (i + j) / 2;

    // left sub-array is a[i .. mid]
    // right sub-array is a[mid + 1 .. j]
    
    merge_sort(i, mid, a, aux);     // sort the left sub-array recursively
    merge_sort(mid + 1, j, a, aux);     // sort the right sub-array recursively

    int pointer_left = i;       // pointer_left points to the beginning of the left sub-array
    int pointer_right = mid + 1;        // pointer_right points to the beginning of the right sub-array
    int k;      // k is the loop counter

    // we loop from i to j to fill each element of the final merged array
    for (k = i; k <= j; k++) {
        if (pointer_left == mid + 1) {      // left pointer has reached the limit
            aux[k] = a[pointer_right];
            pointer_right++;
        } else if (pointer_right == j + 1) {        // right pointer has reached the limit
            aux[k] = a[pointer_left];
            pointer_left++;
        } else if (a[pointer_left] < a[pointer_right]) {        // pointer left points to smaller element
            aux[k] = a[pointer_left];
            pointer_left++;
        } else {        // pointer right points to smaller element
            aux[k] = a[pointer_right];
            pointer_right++;
        }
    }

    for (k = i; k <= j; k++) {      // copy the elements from aux[] to a[]
        a[k] = aux[k];
    }
}

void clearConsoles(void) {
	consoleSelect(top);
	consoleClear();
	consoleSelect(bottom);
	consoleClear();
}