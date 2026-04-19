#include "header.h"

#define BILLION  1000000000L;

/*
 * Measures and prints the total execution time of the algorithm
 * using QNX time event helper APIs.
 */
void cal_time(void) {
	double accum = (stop.tv_sec - start.tv_sec)
			+ (double) (stop.tv_nsec - start.tv_nsec) / (double) BILLION
	;
	printf("Total time: %.2lf seconds\n", accum);
	return;
}

// Prints the count of correctly and wrongly spelled words
void status(void) {
	printf("\nCorrect Words: %d  Incorrect Words: %d\n ", correct_spell,
			wrong_spell);
}

/*
 * Close file pointers and report statics
 *
 * Closes the input and dictionary file, prints out
 * the number of correctly and wrongly spelled words
 * along with the total execution time of spell checker.
 *
 * @param[in] fdict: dictionary.txt file pointer
 * @param[in] ftext: input.txt file pointer
 *
 */
void clean_and_report(FILE *fdict, FILE *ftext) {

	fclose(fdict);
	fclose(ftext);
	status();
	cal_time();

	return;
}

int main(int argc, char *argv[]) {

	if (argc != 3) {
		printf("Usage: %s <dictionary.txt> <input.txt>\n", argv[0]);
		return 1;
	}

	printf("Reading dictionary file: %s...\n", argv[1]);
	FILE *fdict = fopen(argv[1], "r");
	if (!fdict) {
		perror(argv[1]);
		return 1;
	}

	printf("Reading input file: %s...\n", argv[2]);
	FILE *ftext = fopen(argv[2], "r");
	if (!ftext) {
		perror(argv[2]);
		fclose(fdict);
		return 1;
	}

	printf("\nStarting Analysis of %s...\n\n", argv[2]);

	word_analyze(fdict, ftext);

	printf("Analysis completed!\n");

	clean_and_report(fdict, ftext);

	return 0;
}
