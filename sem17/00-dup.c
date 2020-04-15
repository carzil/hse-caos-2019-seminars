#include <stdio.h>
#include <unistd.h>

int main() {
    int new_fd = dup(1);
    printf("new_fd = %d\n", new_fd);
    write(new_fd, "Privet, 2020!\n", 15);


    close(1);
    printf(":[[\n");
    write(new_fd, "Holy write!\n", 12);

    dup2(new_fd, 1);
    printf("Wtf?!\n");

    return 0;
}
