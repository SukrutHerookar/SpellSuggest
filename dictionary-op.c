#include "header.h"

// Store unique seen words
char seen[MAX_WORDS][WORD_LEN];
int seen_count = 0;
int correct_spell = 0, wrong_spell = 0;
struct timespec start, stop;

/*
 * Checks if a word has already been processed.
 *
 * If a repeated word appears in the input and has
 * already been found in the dictionary, it will be
 * skipped and not processed again.
 *
 * @param[in]   Word from input.txt
 *
 * @return 1 if the word was already found, 0 otherwise
 */
int already_seen(const char *word) {
	for (int i = 0; i < seen_count; i++) {
		if (strcmp(seen[i], word) == 0)
			return 1;
	}
	return 0;
}

/*
 * Mark a word as processed
 *
 * To avoid redundant processing of a word,
 * mark it as seen before comparing it with
 * the dictionary.
 *
 * @param[in] Word from input.txt
 */
void mark_seen(const char *word) {
	if (seen_count < MAX_WORDS) {
		strncpy(seen[seen_count], word, WORD_LEN - 1);
		seen[seen_count][WORD_LEN - 1] = '\0';
		seen_count++;
	}
}

/*
 *  Search in dictionary file for the word
 *
 *  @param[in] string from input.txt
 *
 *  @returns[out] 1 if the word is found else 0
 */
int in_dictionary(FILE *dict, const char *needle) {
	char word[WORD_LEN];
	rewind(dict);
	while (fscanf(dict, "%29s", word) == 1) {
		normalize_word(word);
		if (strcmp(word, needle) == 0)
			return 1;
	}
	return 0;
}

/*
 * Finds words present in the dictionary.
 *
 * Iterates through each word in the input file. If the
 * word has already been processed, it is skipped. Otherwise,
 * it is checked against the dictionary for validation.
 *
 * QNX clock event APIs are used to evaluate time duration
 *
 * @param[in] fdict: dictionary.txt file pointer
 * @param[in] ftext: input.txt file pointer
 *
 */
void word_analyze(FILE *fdict, FILE *ftext) {
	char word[WORD_LEN];
	clock_gettime(CLOCK_MONOTONIC, &start);
	while (fscanf(ftext, "%29s", word) == 1) {
		normalize_word(word);
		if (already_seen(word)) {
			correct_spell++;
			continue;
		}
		mark_seen(word);

		if (in_dictionary(fdict, word))
			correct_spell++;
		else {
			printf("Word %s NOT FOUND in dictionary\n", upper_case(word));
			wrong_spell++;
			spell_suggest(fdict, word);
		}
	}
	clock_gettime(CLOCK_MONOTONIC, &stop);
}
