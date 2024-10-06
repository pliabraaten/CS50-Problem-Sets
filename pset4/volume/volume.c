// Modifies the volume of an audio file

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Number of bytes in .wav header
const int HEADER_SIZE = 44;

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    // Open files and determine scaling factor
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    float factor = atof(argv[3]);

    // Copy header from input file to output file
    // Create array the size of the file's header
    uint8_t header[HEADER_SIZE];

    // Read and then write the header array from the input file to the output file
    fread(header, HEADER_SIZE, 1, input);
    fwrite(header, HEADER_SIZE, 1, output);

    // Read samples from input file and write updated data to output file
    // Create a buffer for temporary storage of single 2 byte (16 bit) elements
    int16_t buffer;

    // Read single sample into buffer (copy 16-bit element to the buffer address)
    // While fread still has samples to read (returns 1 for each sample until no more samples)
    while (fread(&buffer, sizeof(int16_t), 1, input) != 0)
    {
        // Modify volume of sample in buffer by the factor amount in argv
        buffer *= factor;

        // Write the sample in buffer to output file
        fwrite(&buffer, sizeof(int16_t), 1, output);
    }

    // Close files
    fclose(input);
    fclose(output);
}
