#include <stdio.h>
#include <stdlib.h>


// Structure to hold grayscale pixel data
typedef struct {
    unsigned char value;
} PixelGray;

// Function to read a PGM image into a 2D array
PixelGray** readPGM(const char* filename, int* width, int* height);

// Function to write a 2D matrix as a PGM image
void writePGM(const char* filename, PixelGray** matrix, int* width, int* height);

// Function to threshold the image matrix
PixelGray** threshold(PixelGray** matrix, int* width, int* height);

// Function to rotate the image matrix
PixelGray** rotate(PixelGray** matrix, int* width, int* height);

//main function - DO NOT MODIFY
int main() {
    int width, height; // variable to hold width and height. Use reference in other functions

    PixelGray** image_original = readPGM("lenna.pgm", &width, &height);
    // Now you have the grayscale image data in the 'image_original' 2D array
    // Access pixel data using image[row][col].value
    // For example, to access the pixel at row=2, col=3:
    // unsigned char pixel_value = image[2][3].value;

    // Create a new 2D array 'image_thresh' to store the threshold image
    PixelGray** image_thresh = threshold(image_original, &width, &height);

    //write the image data as "threshold.pgm"
    writePGM("threshold.pgm", image_thresh, &width, &height);

    // Create a new 2D array 'image_rotate' to store the rotated image
    PixelGray** image_rotate = rotate(image_original, &width, &height);

    //write the image data as "rotate.pgm"
    writePGM("rotate.pgm", image_rotate, &width, &height);
    image_rotate = rotate(image_rotate, &width, &height);

    //write the image data as "rotate_again.pgm"
    writePGM("rotate_again.pgm", image_rotate, &width, &height);

    // Free the allocated memory when you're done
    for (int i = 0; i < height; ++i) {
        free(image_original[i]);
        free(image_thresh[i]);
        free(image_rotate[i]);

    }
        free(image_original);
        free(image_thresh);
        free(image_rotate);
        return 0;
}

PixelGray** readPGM (const char* filename, int* width, int* height) {
    // Open file for image
    FILE* image_file = fopen(filename, "r");
    // If image file is NULL, print error
    if (image_file == NULL) {
        printf("Not able to open file\n");
        exit(1);
    }

    // Get Magic Number
    char magic_number[3];
    fscanf(image_file, "%2s\n", magic_number);
    // If first line of header is not P5, print error
    if (magic_number[0] != 'P' || magic_number[1] != '5') {
        printf("Not a PGM file\n");
        fclose(image_file);
        exit(1);
    }
    // Get Dimensions
    fscanf(image_file, "%d %d\n", width, height);

    // Get Max Pixel Value
    int max_pixel;
    fscanf(image_file, "%d\n", &max_pixel);

    // Dynamic Memory Allocation for image data
    PixelGray** image = (PixelGray**)malloc(*height * sizeof(PixelGray*));
        for (int i = 0; i < *height; ++i) {
            image[i] = (PixelGray*)malloc(*width * sizeof(PixelGray));
        }

    // Fill image array with file data
    for (int i = 0; i < *height; ++i) {
        for (int j = 0; j < *width; ++j) {
            fread(&image[i][j].value, sizeof(unsigned char), 1, image_file);
        }
    }

    fclose(image_file);

    // Return image array
    return image;
}

void writePGM (const char* filename, PixelGray** matrix, int* width, int* height) {
    // Open file for image
    FILE* image_file = fopen(filename, "w");
    // If image file is NULL, print error
    if (image_file == NULL) {
        printf("Error opening file");
        exit(1);
    }

    // Write magic number (P5)
    fprintf(image_file, "P5\n");

    // Write width and height
    fprintf(image_file, "%d %d\n", *width, *height);

    // Write maximum gray value
    fprintf(image_file, "255\n");

    // Write pixel data
    for (int i = 0; i < *height; ++i) {
        for (int j = 0; j < *width; ++j) {
            fwrite(&matrix[i][j].value, sizeof(unsigned char), 1, image_file);
        }
    }

    fclose(image_file);
}

PixelGray** threshold(PixelGray** matrix, int* width, int* height) {
    // Dynamic Memory Allocation for threshold image
    PixelGray** threshold = (PixelGray**)malloc(*height * sizeof(PixelGray*));
    for (int i = 0; i < *height; ++i) {
        threshold[i] = (PixelGray*)malloc(*width * sizeof(PixelGray));
        for (int j = 0; j < *width; ++j) {
            threshold[i][j].value = (matrix[i][j].value > 80) ? 255 : 0;
        }
    }
    // Return threshold image array
    return threshold;
}

PixelGray** rotate(PixelGray** matrix, int* width, int* height) {
    // Dynamic Memory Allocation for rotated image
    PixelGray** rotated = (PixelGray**)malloc(*width * sizeof(PixelGray*));
    for (int i = 0; i < *width; ++i) {
        rotated[i] = (PixelGray*)malloc(*height * sizeof(PixelGray));
    }
    // Transpose for rotation
    for (int i = 0; i < *height; ++i) {
        for (int j = 0; j < *width; ++j) {
            rotated[j][i].value = matrix[i][j].value;
        }
    }
    // Update width and height
    int temp = *width;
    *width = *height;
    *height = temp;

    // Return rotated image array
    return rotated;
}
