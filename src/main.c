#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "bled.h"

int main(int argc, char **argv)
{
	if (argc < 2)
		return 1;
	FILE *tmp = tmpfile();

	char *filename = argv[1];
	char cmdstr[100] = "";
	char *shcmd = (char *)calloc(99, sizeof(char));

	char **buf = readfile(filename);
	if (buf == NULL)
		return 1;

	int start = 1;
	int end = END;
	while (strcmp(cmdstr, "q\n")) {
		fgets(cmdstr, 100, stdin);
		char cmd;

		int dst = 0;
		int i = 0;

		while (!isalpha(cmdstr[i]) && cmdstr[i] != '!') {
			if (isdigit(cmdstr[i])) {
				for (end = 0; isdigit(cmdstr[i]); i++)
					end = 10 * end + (cmdstr[i] - '0');
				start = end;
				if (cmdstr[i] == ',')
					i++;
				else
					break;
				if (cmdstr[i] == '$') {
					end = END;
					i++;
				} else {
					for (end = 0; isdigit(cmdstr[i]); i++)
						end =
						    10 * end + (cmdstr[i] -
								'0');
				}
			} else if (cmdstr[i] == ',') {
				start = 1;
				end = END;
				i++;
			} else {
				break;
			}
		}
		cmd = cmdstr[i];
		if (!isalpha(cmd) && cmd != '!')
			continue;
		if (cmdstr[i + 1]) {
			char *tmp = shcmd;
			while (cmdstr[i]) {
				if (isdigit(cmdstr[i]))
					dst = 10 * dst + (cmdstr[i] - '0');
				else if(cmdstr[0] == '!') {
					if(i < 98 && cmdstr[i])
						*tmp++ = cmdstr[i+1];
				}
				++i;
			}
		}

		switch (cmd) {
		case 'p':
		case 'n':
			printbuf(buf, cmd == 'n', start, end);
			break;
		case 'a':
			writetmp(tmp, buf);
			insertbuf(buf, end, 0, 0);
			break;
		case 'i':
			writetmp(tmp, buf);
			insertbuf(buf, start, 0, 1);
			break;
		case 'c':
			writetmp(tmp, buf);
			insertbuf(buf, start, (end - start + 1), 0);
			break;
		case 'd':
			writetmp(tmp, buf);
			dellines(buf, start, end);
			break;
		case 'm':
		case 't':
			writetmp(tmp, buf);
			movelines(buf, start, end, dst, cmd == 't');
			break;
		case 'w':
			writefile(filename, buf);
			break;
		case 'u':
			undo(tmp, buf);
			break;
		case '!':
			system(shcmd);
			break;
		case 'q':
			break;
		default:
			fprintf(stderr, "?\n");
			break;
		}
	}

	fclose(tmp);
	for (int i = 0; buf[i]; ++i)
		free(buf[i]);
	free(buf);
	return 0;
}
