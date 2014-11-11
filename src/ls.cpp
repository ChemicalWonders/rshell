#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>

using namespace std;

int main()
{
    const char *dirName = ".";
    DIR *dirp = opendir(dirName);
    dirent *direntp;

    while ((direntp = readdir(dirp)) != NULL)
        cout << direntp->d_name << endl;
        // use stat here to find attributes of file
    closedir(dirp);
}
