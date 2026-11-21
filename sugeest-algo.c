#include "header.h"

/*
 * Computes the Levenshtein edit distance between two strings
 * using the Wagner–Fischer algorithm.
 *
 * @param[in] a: word to suggest from dictionary
 * @param[in] b: word under suggestion
 *
 * @returns[out] the edit distance
 */
int wagner_fischer(const char *a, const char *b) {
  size_t m = strlen(a), n = strlen(b);
  if (m == 0)
    return (int)n;
  if (n == 0)
    return (int)m;

  // ensure n <= m to minimize memory
  if (n > m) {
    const char *t = a;
    a = b;
    b = t;
    size_t tmp = m;
    m = n;
    n = tmp;
  }

  // two rolling rows
  int prev[n + 1];
  int curr[n + 1];

  for (size_t j = 0; j <= n; ++j)
    prev[j] = (int)j;

  for (size_t i = 1; i <= m; ++i) {
    curr[0] = (int)i;
    for (size_t j = 1; j <= n; ++j) {
      int cost = (a[i - 1] == b[j - 1]) ? 0 : 1;
      int del = prev[j] + 1;        // delete a[i-1]
      int ins = curr[j - 1] + 1;    // insert b[j-1]
      int sub = prev[j - 1] + cost; // substitute
      int best = del < ins ? del : ins;
      if (sub < best)
        best = sub;
      curr[j] = best;
    }
    // roll rows
    for (size_t j = 0; j <= n; ++j)
      prev[j] = curr[j];
  }
  return prev[n];
}

/*
 * Print suggested words from dictionary
 *
 * Prints all the suggested words from the array, when the array
 * is not empty.
 *
 * @param[in] suggestions: array containing suggested words
 * @param[in] suggestion_count: size of array
 *
 */
void print_suggestion(char suggestions[MAX_SUGGEST][WORD_LEN],
                      int suggestion_count) {

  if (!suggestion_count) {
    printf("No suggestions found\n");
    return;
  }
  for (int i = INITIAL_VAL; i < suggestion_count; i++)
    printf("%s, ", suggestions[i]);
  printf("\b\b?\n");

  return;
}

/*
 * Checks each word in the dictionary against the given word
 * using edit distance. If the distance equals WORD_DIST_LIMIT,
 * the dictionary word is added to the suggestions.
 *
 * @param[in] dict:   Pointer to the open dictionary file
 * @param[in] needle: Word to check and suggest alternatives for
 */
void spell_suggest(FILE *dict, const char *needle) {
  char suggestion[MAX_SUGGEST][WORD_LEN];
  char word[WORD_LEN];
  rewind(dict);
  to_lower((char *)needle);
  int i = INITIAL_VAL;
  while (fscanf(dict, "%29s", word) == 1) {

    normalize_word(word);
    if ((wagner_fischer((const char *)word, needle)) == WORD_DIST_LIMIT) {
      strncpy(suggestion[i++], word, WORD_LEN - 1);
    }
  }
  print_suggestion(suggestion, i);

  return;
}
