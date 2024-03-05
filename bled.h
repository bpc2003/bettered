#define END -1

// file operations
char **readfile(char *filename);
void writefile(char *filename, char **buf);

// buffer operations
void printbuf(char **buf, int sl, int start, int end);
void insertbuf(char **buf, int pos, int ow, int ins);
