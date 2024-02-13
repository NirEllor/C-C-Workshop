#include "cipher.h"
//#include <stdio.h>
#define FIRST_LOWER_LETTER 'a'
#define FIRST_UPPER_LETTER 'A'
#define LAST_LOWER_LETTER 'z'
#define LAST_UPPER_LETTER 'Z'
#define NUM_OF_LETTERS 26

/// IN THIS FILE, IMPLEMENT EVERY FUNCTION THAT'S DECLARED IN cipher.h.


// See full documentation in header file

/**
 * Deals with all forms of k
 * @param c - given char
 * @param k - given k
 * @param start - a/A
 * @param end - z/Z
 * @return adjusted letter, based on the situation
 */
int k_situations(int c, int k, int start, int end)
{
    if (c + k > end)
    {
        if (k >= NUM_OF_LETTERS)
        {
            k = k % NUM_OF_LETTERS;
            if (c + k > end)
            {
               return c - NUM_OF_LETTERS + k;
            }
            return c + k;
        }
        return start + ((c + k) % end) - 1;
    }
    if (c + k < start)
    {
        if (k<= -NUM_OF_LETTERS)
        {
            k = -(k % NUM_OF_LETTERS);
            if (c - k < start)
            {
                return c + NUM_OF_LETTERS - k;
            }
            return c - k;
        }
        return end - (start % (c + k)) + 1;
    }
    else
    {
        return c + k;
    }
}
/**
 * Enters a and z to k_situations
 * @param c - given char
 * @param k  - k
 * @return adjusted letter
 */
char encode_lower(char c, int k)
{
    return k_situations(c, k, FIRST_LOWER_LETTER, LAST_LOWER_LETTER);

}
/**
 * Enters A and Z to k_situations
 * @param c - given char
 * @param k  - k
 * @return adjusted letter
 */
char encode_upper(char c, int k)
{
    return k_situations(c, k, FIRST_UPPER_LETTER, LAST_UPPER_LETTER);
}
/**
 * The main of encode, deals with all chars
 * @param s - string of letters\signs\etc..
 * @param k - shift of char
 */
void encode (char s[], int k)
{
  int i = 0;
  while (s[i] != '\0' )
  {
      if (FIRST_LOWER_LETTER <= s[i]  && s[i] <= LAST_LOWER_LETTER)
      {
          s[i] = encode_lower(s[i], k);

      }
      else if (FIRST_UPPER_LETTER <= s[i] && s[i] <= LAST_UPPER_LETTER)
      {
          s[i] = encode_upper(s[i], k);
      }
      ++i;
  }
}

// See full documentation in header file
/**
 * The main of decode, deals with all chars
 * @param s - string of letters\signs\etc..
 * @param k - shift of char
 */
void decode (char s[], int k)
{
    encode(s, -k);
}
