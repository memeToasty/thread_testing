#include <3ds.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#define SCREEN_WIDTH  400
#define SCREEN_HEIGHT 240

#define MAIN_MENU_OFFSET 1

//Global variables//

PrintConsole top, bottom;

int thread_count = 2;
int selector = 1;
int menu_count = 4;
int array_length = 100;
int preffered_core = 0;

// function Definitions//

void merge_sort(int i, int j, int a[], int aux[]);

/**
 * @brief handles MenuSelection
 * @param screen 0 = Upper; 1 = Lower; 2 = both
 */
void clearConsoles(int screen);
void enterHandler(void);
void startProcessing(void);
void generateCountArray(int array[]);
void inputValue(int *var, int digits);
void shuffleArray(int *array);

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
	consoleInit(GFX_BOTTOM, &bottom);
	consoleInit(GFX_TOP, &top);

	consoleSelect(&top);

	// Main loop
	while (aptMainLoop())
	{
		hidScanInput();

		// Respond to user input
		u32 kDown = hidKeysDown();
		if (kDown & KEY_START) break; // break in order to return to hbmenu
		if (kDown & KEY_DUP) handleSelector(true);
		if (kDown & KEY_DDOWN) handleSelector(false);
		if (kDown & KEY_A) enterHandler();

		consoleSelect(&top);
		printf("\x1b[1;3H3DS Thread testing");
		printf("\x1b[2;3HThread Count: %i", thread_count);
		printf("\x1b[3;3HArray Length: %i", array_length);
		printf("\x1b[4;3Hpreffered Core: %i", preffered_core);
		printf("\x1b[5;3HStart");

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

void generateCountArray(int array[]) {
	for (int x = 0; x < array_length; x++) {
		array[x] = x;
	}
}

void shuffleArray(int *array) {
	for (int x = 0; x < array_length; x++) {
		int randomPos = rand() % (array_length - 1);
		int temp = array[randomPos];
		array[randomPos] = array[x];
		array[x] = temp;
	}
}

void startProcessing(void) {
	int *array = (int*)malloc(array_length * sizeof(int));
	generateCountArray(array);
	shuffleArray(array);
	consoleSelect(&bottom);
	for (int x = 0; x < 30; x++) {
		printf("\x1b[%i;1HArray[0]: %i", x+1, array[x]);
	}
	free(array);
	
}

void inputValue(int *val, int digits) {
	char mybuf[8];
	SwkbdState swkbd;
	swkbdInit(&swkbd, SWKBD_TYPE_NUMPAD, 1, digits);
	swkbdSetValidation(&swkbd, SWKBD_ANYTHING, 0, 0);
	swkbdSetFeatures(&swkbd, SWKBD_FIXED_WIDTH);
	swkbdInputText(&swkbd, mybuf, sizeof(mybuf));
	*val = atoi(mybuf);
}

void enterHandler(void) {
	if (selector == menu_count) startProcessing();
	if (selector == 1) inputValue(&thread_count, 6);
	if (selector == 2) inputValue(&array_length, 6);
	if (selector == 3) inputValue(&preffered_core, 6);

}

void handleSelector(bool keyDir) {
	clearConsoles(0);
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

void clearConsoles(int screen) {
	switch(screen) {
		case 0:
			consoleSelect(&top);
			consoleClear();
			break;
		case 1:
			consoleSelect(&bottom);
			consoleClear();
			break;
		case 2:
			consoleSelect(&top);
			consoleClear();
			consoleSelect(&bottom);
			consoleClear();
			break;
		
	}
}