#include "header.h"

/*
 * Converts the string to lower case
 *
 * Calls tolower string function for each char in
 * the string.
 *
 * @param[in] string in upper case
 *
 */
void to_lower(char *word) {
  // Lowercase
  for (char *p = word; *p; p++)
    *p = tolower((unsigned char)*p);
  return;
}

/*
 * Normalizes a word for comparison.
 *
 * Converts the given word from uppercase to lowercase
 * and strips trailing punctuation to ensure uniformity
 * with dictionary entries.
 *
 * @param[in,out]  Word to normalize (modified in place)
 */
void normalize_word(char *word) {
  // Lowercase
  to_lower(word);

  // remove puntuations
  char *write = word, *read = word;
  while (*read) {
    if (!ispunct((unsigned char)*read)) {
      *write++ = *read;
    }
    read++;
  }
  *write = '\0';
  word = write;
  return;
}

/*
 * Converts a string to uppercase.
 *
 * Iterates through each character and applies `toupper`.
 * If no dictionary match is found, the original word is
 * printed back to the user in uppercase.
 *
 * @param[in]  Input string to be converted
 *
 * @return Pointer to the modified string in uppercase
 */
char *upper_case(char *word) {
  for (char *p = word; *p; p++)
    *p = toupper((unsigned char)*p);
  return word;
}
