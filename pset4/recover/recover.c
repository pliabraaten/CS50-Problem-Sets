#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define BLOCK_SIZE 512

int main(int argc, char *argv[])
{
    // Ensure only one command line argument
    while (argc != 2)
    {
        printf("Usage: ./recover FILE\n");
        return 1;
    }

    // Open memory card for reading
    FILE *card = fopen(argv[1], "r");
    if (card == NULL)
    {
        printf("Could not open file\n");
        return 2;
    }

    // DECLARATIONS
    // Create a buffer for the JPEG data blocks (this is creating an array of 512 bytes in units of
    // 1 byte [8 bits])
    uint8_t data_buffer[BLOCK_SIZE];

    // Declare file pointer to store the JPEG files
    FILE *jpg_pointer;

    // Create buffer for the JPEG file names ###.jpg
    char jpg_name_buffer[57];

    // Create variable to count how many JPEG files have been recovered for the naming convention
    int recovered_count = 0;

    // Create flag to indicate if a jpeg file is currently open
    int jpg_open = 0;

    // RECOVERY
    // If there is still data on memory card, loop through memory blocks
    // If it reads 512 bytes, then it was successful
    // Function fread: read of 512 single bytes to buffer address from card
    while (fread(data_buffer, 1, BLOCK_SIZE, card) == BLOCK_SIZE)
    {
        // Look for start of JPEG in each data block
        // first four bytes of JPEGs are 0xff, 0xd8, 0xff, 0xe- (- is anything up to 0-f)
        // (buffer[3] & 0xf0) == 0xe0 -> Only looks at first 4 bits and set the last 4 bits to 0
        // since they don't matter
        if (data_buffer[0] == 0xff && data_buffer[1] == 0xd8 && data_buffer[2] == 0xff &&
            (data_buffer[3] & 0xf0) == 0xe0)
        {
            // If there is a jpeg open
            if (jpg_open == 1)
            {
                // Close the last JPEG file stored at in memory where jpg_pointer is pointing
                fclose(jpg_pointer);
                // Update jpeg open flag
                jpg_open = 0;

                // Increase recovered_count after closing recovered jpeg
                recovered_count++;
            }

            // Create filename: ###.jpg, starting at 000.jpg so files are in order of recovery
            sprintf(jpg_name_buffer, "%03i.jpg", recovered_count);

            // Make a new JPEG file
            jpg_pointer = fopen(jpg_name_buffer, "wb");
            // Return error if unable to create JPEG file
            if (jpg_pointer == NULL)
            {
                printf("Could not create new JPEG file\n");
                return 3;
            }

            // Update jpeg open flag
            jpg_open = 1;

            // write to this new open file
            fwrite(data_buffer, 1, BLOCK_SIZE, jpg_pointer);
        }

        // If there is a JPEG already open, then it is continuation of last JPEG and keep writing
        else if (jpg_open == 1)
        {
            fwrite(data_buffer, 1, BLOCK_SIZE, jpg_pointer);
        }
    }
    // If the last file is still open, close it
    if (jpg_open == 1)
    {
        fclose(jpg_pointer);
        jpg_open = 0;
    }
    // Close the memory card file
    fclose(card);
    return 0;
}
