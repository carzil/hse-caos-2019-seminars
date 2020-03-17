#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>


int main(int argc, char* argv[]) {
    DIR* d = opendir(argv[1]);
    if (d == NULL) {
        perror("opendir");
        return 1;
    }

    size_t parent_len = strlen(argv[1]);
    char entry_path[PATH_MAX];
    memcpy(entry_path, argv[1], parent_len);
    entry_path[parent_len] = '/';
    entry_path[parent_len + 1] = '\0';

    printf("Listing of %s:\n", argv[1]);
    struct dirent* entry = NULL;
    while ((entry = readdir(d)) != NULL) {
        if (!strcmp(entry->d_name, "..") || !strcmp(entry->d_name, ".")) {
            continue;
        }

        size_t entry_len = strlen(entry->d_name);
        memcpy(&entry_path[parent_len + 1], entry->d_name, entry_len);
        entry_path[parent_len + 1 + entry_len] = '\0';

        struct stat st;
        if (lstat(entry_path, &st) < 0) {
            perror("lstat");
            continue;
        }

        mode_t perms = (st.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO));
        printf("  [%o] u:%u g:%u %s\n", perms, st.st_uid, st.st_gid, entry->d_name);
    }

    closedir(d);
    return 0;
}
