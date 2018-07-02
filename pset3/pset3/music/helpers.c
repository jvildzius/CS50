// Helper functions for music

#include <cs50.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "helpers.h"

// Converts a fraction formatted as X/Y to eighths
int duration(string fraction)
{
    //converts string X/Y to fraction
    float x = atoi(&fraction[0]);
    float y = atoi(&fraction[2]);
    float z = (x / y) * 8.00;
    return round(z);

}

// Calculates frequency (in Hz) of a note
int frequency(string note)
{
  int semitone = 0;
    char letter = note[0];
    int octave = atoi(&note[(strlen(note) - 1)]);
    float freq = 0;

    //first figure out note letter's semitones away from A
    switch (letter)
    {
        case 'C' :
            semitone = -9;
            break;
        case 'D' :
            semitone = -7;
            break;
        case 'E' :
            semitone = -5;
            break;
        case 'F' :
            semitone = -4;
            break;
        case 'G' :
            semitone = -2;
            break;
        case 'A' :
            semitone = 0;
            break;
        case 'B' :
            semitone = 2;
            break;
    }

    //adjust for accidental
    if (note[1] == '#')
        semitone ++;
    else if (note[1] == 'b')
        semitone --;

    //calculate the note's frequency in the 4th octave
    float powerof2 = abs(semitone) / 12.00;
    if (semitone < 0)
        freq = 440 / powf(2.00, powerof2);
    else if (semitone > 0)
        freq = 440 * powf(2.00, powerof2); //formula only returns 440
    else
        freq = 440;

    //adjust for octave
    int shift;
    if (octave > 4)
    {
        shift = octave - 4;
        freq = freq * (2 * shift);
    }
    else if (octave < 4)
    {
        shift = 4 - octave;
        freq = freq / (2 * shift);
    }
    return round(freq);
}

// Determines whether a string represents a rest
bool is_rest(string s)
{
    if (strncmp(s, "", 1))
        return false;
    else
        return true;
}
