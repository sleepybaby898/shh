#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>
#include <limits.h>

int main() {
    char input[1024];

    while (1) {
        // prompt
        char cwd[PATH_MAX];
        _getcwd(cwd, sizeof(cwd));
        printf("shh | %s> ", cwd);

        if (!fgets(input, sizeof(input), stdin)) break;
        input[strcspn(input, "\n")] = 0; // remove newlines

        if (strlen(input) == 0) continue;

        // exit shell
        if (strcmp(input, "exit") == 0) break;

        // implement cd
        if (strncmp(input, "cd ", 3) == 0) {
            char *dir = input + 3; // skip "cd " just get whats after
            if (_chdir(dir) != 0) {
                perror("cd failed");
            }
            continue;
        }

        // linux aliases
        char cmd[1100];
        if (strcmp(input, "ls") == 0) {
            snprintf(cmd, sizeof(cmd), "cmd /c dir");
        } else if (strcmp(input, "pwd") == 0) {
            _getcwd(cmd, sizeof(cmd));
            printf("%s\n", cmd);
            continue;
        } else if (strcmp(input, "clear") == 0) {
            system("cls");
            continue;
        } else if (strncmp(input, "cat ", 4) == 0) {
            // run type instead of cat
            snprintf(cmd, sizeof(cmd), "cmd /c type %s", input + 4);
            continue;
        } else if (strncmp(input, "touch", 5) == 0) {
            char *filename = input + 5; // skip "touch"
            while (*filename == ' ') filename++; // skip spaces

            if (strlen(filename) == 0) {
                printf("touch: missing file operand\n");
                continue;
            }

            snprintf(cmd, sizeof(cmd), "cmd /c type nul > \"%s\"", filename); // make blank file similar to touch
            system(cmd);

            continue;
        } else {
            snprintf(cmd, sizeof(cmd), "cmd /c %s", input);
        }

        // if not already done, run command
        int ret = system(cmd);
        if (ret == -1) {
            perror("failllll");
        }
    }

    return 0;
}
