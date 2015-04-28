#ifdef WIN32
#include <windows.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include "hidapi.h"

#define MAX_STR 255

int main(int argc, char* argv[])
{
	int res;
	unsigned char buf[65]={0};
	wchar_t wstr[MAX_STR];
	hid_device *handle;
	int i;

	// Initialize the hidapi library
	res = hid_init();

	// Open the device using the VID, PID,
	// and optionally the Serial number.
	handle = hid_open(0x4d8, 0x3f, NULL);


	// Display message on LCD at specific row
	char message[25]={0};
	int row_number;

	printf("Type any message: ");
	scanf("%s", message);
	printf("Enter the row number to display message on LCD (0-6): ");
	scanf("%d", &row_number);
	printf("Displaying message on LCD screen at row number...\n");

	buf[0] = 0x0;
	buf[1] = 0x1;				// 0: do nothing, 1: PIC read data, 2: PIC reply data
	buf[2] = row_number;		// row number
	
	for (i = 0; i < 25; i++) {
		buf[i+3] = message[i];
	}

	res = hid_write(handle, buf, 65);



	// If text displaying on LCD has finished, then tell accelerometer portion to start
	res = hid_read(handle, buf, 65);

	if (buf[0]==0x1 && buf[1]==0x3) {
		printf("Message has been written to LCD.\n");
		buf[0] = 0x0;
		buf[1] = 0x2;				// 0: do nothing, 1: PIC read data, 2: PIC reply data
		res = hid_write(handle, buf, 65);
		printf("Accelerometer values being read 100 times at 100 Hz ...\n");
	}




	// Read accelerometer values 100 times
	short accels_x[100], accels_y[100], accels_z[100];

	int counter = 0;
	while (counter < 100) {
		res = hid_read(handle, buf, 65);		

		if (buf[0]==0x1 && buf[1]==0x2) {
			accels_x[counter] = (buf[2]<<8) | buf[3];
			accels_y[counter] = (buf[4]<<8) | buf[5];		
			accels_z[counter] = (buf[6]<<8) | buf[7];

			buf[0] = 0x0;
			buf[1] = 0x2;
			res = hid_write(handle, buf, 65);

			counter++;
		}

		
	}



	// Toggle LED to indicate process has completed.
	printf("Accelerometer reading has completed.\n");
	buf[0] = 0x0;
	buf[1] = 0x80;
	res = hid_write(handle, buf, 65);



	// Write accelerometer readings to text file
	printf("Writing accelerometer values to text file for display purposes in Matlab.\n");

	FILE *ofp;

	ofp = fopen("accels.txt", "w");

	for (i=0; i<100; i++) {
		fprintf(ofp,"%d, %d, %d\r\n",accels_x[i],accels_y[i],accels_z[i]);
	}

	fclose(ofp);




	// Finalize the hidapi library
	res = hid_exit();

	return 0;
}
