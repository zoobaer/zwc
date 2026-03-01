#include "wc.h"

#define VERSION "v0.1"
#define HELP ""

/* TODO: handle input file */

int wc_main(int argc, char** argv) {
	FILE* file = NULL;
	int arguments = 0;
	int is_args_set = 0;
	int i;
	int j;
	char ch;
	char prev_ch;
	wchar_t wch;
	wchar_t prev_wch;
	int byte_count = 0;
	int char_count = 0;
	int word_count = 0;
	int line_count = 0;
	int max_line_len = 0;
	int curr_line_len = 0;
	char read_bytes[13];
	int curr_bytes_read = 0;
	int file_name_args_num = 1;
	int read_wchar_bytes_count = 0;
	char* current_mbtowc_pointer;
	mbstate_t state;
	size_t remaining;

	setlocale(LC_ALL, "");

	if (argc == 1) {
		arguments = CHAR_COUNT | LINE_COUNT | WORD_COUNT | BYTE_COUNT | MAX_LINE_LEN;
		file = stdin;
	} else {
		for (i = 1; i < argc; i++) {
			if (argv[i][0] == '-') {

				if (strlen(argv[i]) > 1 && argv[i][1] == '-') { /* Double dashes */
					if (strcmp(argv[i], "--version") == 0) {
						/* version */
					}
					else if (strcmp(argv[i], "--help") == 0) {
						/* help */
					}
					else if (strcmp(argv[i], "--bytes") == 0) {
						if ((arguments & BYTE_COUNT) == BYTE_COUNT) {
							fprintf(stderr, "error: redundant arguments for bytes count!\n");
							return -1;
						}

						arguments |= BYTE_COUNT;
						is_args_set = 1;
					}
					else if (strcmp(argv[i], "--chars") == 0) {
						if ((arguments & CHAR_COUNT) == CHAR_COUNT) {
							fprintf(stderr, "error: redundant arguments for characters count!\n");
							return -1;
						}

						arguments |= CHAR_COUNT;
						is_args_set = 1;
					}
					else if (strcmp(argv[i], "--lines") == 0) {
						if ((arguments & LINE_COUNT) == LINE_COUNT) {
							fprintf(stderr, "error: redundant arguments lines count!\n");
							return -1;
						}

						arguments |= LINE_COUNT;
						is_args_set = 1;
					}
					else if (strcmp(argv[i], "--words") == 0) {
						if ((arguments & WORD_COUNT) == WORD_COUNT) {
							fprintf(stderr, "error: redundant arguments words count!\n");
							return -1;
						}

						arguments |= WORD_COUNT;
						is_args_set = 1;
					}

					return 0;
				}
				else if (strlen(argv[i]) == 1) { 

					file = stdin;
					continue;
				}
				else {

					j = 1;
					while ((argv[i][j]) != '\0') {

						switch (argv[i][j]) {
						case 'c': /* Bytes count*/
							arguments |= BYTE_COUNT;
							is_args_set = 1;
							break;
						case 'm': /* Chars count */
							arguments |= CHAR_COUNT;
							is_args_set = 1;
							break;
						case 'w': /* Words count */
							arguments |= WORD_COUNT;
							is_args_set = 1;
							break;
						case 'l': /* Lines count */
							arguments |= LINE_COUNT;
							is_args_set = 1;
							break;
						case 'L': /* Maximum display width */
							arguments |= MAX_LINE_LEN;
							is_args_set = 1;
							break;
						default:
							fprintf(stderr, "error: invalid flag %c\n", argv[i][j]);
							break;
						}

						j++;
					}

				}

			}
			else {
				if (file != NULL) {
					fprintf(stderr, "error: invalid number of files!\n");
					return -1;
				}

				file = fopen(argv[i], "rb");
				printf("file opened!\n");
				if (file == NULL) {
					fprintf(stderr, "error: failed to open the file %s\n", argv[i]);
					return -1;
				}

				file_name_args_num = i;

			}

		}
	}

	if (file == NULL) {
		file = stdin;
		printf("file is stdin\n");
	}

	if (!is_args_set) {
		arguments = CHAR_COUNT | WORD_COUNT | LINE_COUNT;
	}

	/* Calculate */

	/* Since flags do not depend on each other and they are compatible with each other, there is no need to create a switch statement handling all of their combinations! */
	

	memset(&state, 0, sizeof(state));
	prev_wch = 0;
	while ((curr_bytes_read = fread(&read_bytes, sizeof(char), 12, file)) > 0) {
		read_bytes[12] = '\0';

		current_mbtowc_pointer = read_bytes;

		remaining = curr_bytes_read;

		//printf("%d bytes read!\n", curr_bytes_read);
		while (current_mbtowc_pointer < (read_bytes + curr_bytes_read)) { /* wbtowc does not advance characters, you gotta do that! */
			
			read_wchar_bytes_count = mbtowc(&wch, current_mbtowc_pointer, MB_CUR_MAX);
			if (read_wchar_bytes_count <= 0) {
				break;
			}

			//printf("consumed %d bytes to read the character \n", read_wchar_bytes_count);

			if ((arguments & CHAR_COUNT) == CHAR_COUNT) {

				if ((arguments & CHAR_COUNT) == CHAR_COUNT) {

					char_count++; /* NOTE: There is a problem with character counting. It is alot less than actual....Come back later!!!*/
				
				}

			}

			if ((arguments & BYTE_COUNT) == BYTE_COUNT) {
				byte_count += read_wchar_bytes_count;
			}

			if ((arguments & WORD_COUNT) == WORD_COUNT) {

				if (iswspace(wch) && !iswspace(prev_wch)) {
					word_count++;
				}

			}

			if ((arguments & LINE_COUNT) == LINE_COUNT) {

				if (wch == L'\n') {
					line_count++;
					char_count++; // 
					//printf("line count: %d\n", line_count);

					if (curr_line_len > max_line_len) {
						max_line_len = curr_line_len;
					}
					curr_line_len = 0;
				}

			}

			if ((arguments & MAX_LINE_LEN) == MAX_LINE_LEN) {
			
				if ((arguments & MAX_LINE_LEN) == MAX_LINE_LEN) {
					curr_line_len++;
				}

			
			}

			prev_wch = wch;
			current_mbtowc_pointer += read_wchar_bytes_count;
		}

		
		read_bytes[0] = 0;
		read_bytes[1] = 0;
		read_bytes[2] = 0;
		read_bytes[3] = 0;
		read_bytes[4] = 0;
		read_bytes[5] = 0;
		read_bytes[6] = 0;
		read_bytes[7] = 0;
		read_bytes[8] = 0;
		read_bytes[9] = 0;
		read_bytes[10] = 0;
		

	} 
	
	/*
	prev_ch = 0;
	while ((ch = fgetc(file)) != EOF) {

		if ((arguments & CHAR_COUNT) == CHAR_COUNT) {
			char_count++;
		}

		if ((arguments & WORD_COUNT) == WORD_COUNT) {

			if (isspace(ch) && !isspace(prev_ch)) {
				word_count++;
			}

		}

		if ((arguments & LINE_COUNT) == LINE_COUNT) {

			if (ch == '\n') {
				line_count++;

				if (curr_line_len > max_line_len) {
					max_line_len = curr_line_len;
					curr_line_len = 0;
				}

			}

		}

		if ((arguments & MAX_LINE_LEN) == MAX_LINE_LEN) {
			curr_line_len++;
		}

		prev_ch = ch;
	}
	*/

	/* PRINT THE RESULTS */
	
	if ((arguments & CHAR_COUNT) == CHAR_COUNT) {
		printf("char: %20d\n", char_count);
	}

	if ((arguments & BYTE_COUNT) == BYTE_COUNT) {
		printf("byte: %20d\n", byte_count);
	}

	if ((arguments & WORD_COUNT) == WORD_COUNT) {
		printf("word: %20d\n", word_count);
	}

	if ((arguments & LINE_COUNT) == LINE_COUNT) {
		printf("line: %20d\n", line_count);
	}

	if ((arguments & MAX_LINE_LEN) == MAX_LINE_LEN) {
		printf("max line length: %10d\n", max_line_len);
	}

	fclose(file);

	return 0;
}
