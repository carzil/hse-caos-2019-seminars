#include <stdio.h>
#include <unistd.h>

#define BOLD(s) ("\033[1m" s "\033[0m")
#define RED(s) ("\033[31m" s "\033[0m")
#define GREEN(s) ("\033[32m" s "\033[0m")
#define YELLOW(s) ("\033[33m" s "\033[0m")

float update_percent(float old) {
    usleep(10000);
    return old + 0.2;
}

int main() {
    setbuf(stdout, NULL);

    const char* frames[] = {"⠋", "⠙", "⠹", "⠸", "⠼", "⠴", "⠦", "⠧", "⠇", "⠏"};
    const char* colors[] = {"\033[31m", "\033[33m", "\033[32m", "\033[34m"};
    const size_t colors_len = sizeof(colors) / sizeof(colors[0]);

    printf("Blood\n");
    printf(YELLOW("Sun\n"));
    printf(GREEN("Grass\n"));

    float percent = 0;
    size_t pos = 0;
    for (;;) {
        int integer_percent = percent;
        printf("\r[\033[1m%s%s\033[0m] Loading... %.1f%%", colors[integer_percent % 3], frames[pos], percent);
        pos += 1;
        pos %= sizeof(frames) / sizeof(frames[0]);
        usleep(100000);
        percent += 0.2;
    }
}
