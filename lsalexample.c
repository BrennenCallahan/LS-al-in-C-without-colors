#include <sys/types.h>
#include <sys/dir.h>
#include <sys/param.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <locale.h>
#include <langinfo.h>
#include <stdint.h>
#include <fcntl.h>
#include <stdbool.h>
void ProcessCurrentDirectory();
static char PermissionBuffer[50];
const char *get_permissionsOfFiles(mode_t mode);
void SYSTEMPRINT(char *msg);
static int returnOne (const struct dirent *unused);
char pathname[MAXPATHLEN];
bool is_direct(const char *path);
void ProcessSelectedDirectory(const char *patt);
void ptrch(char *point);


int main(int argc, char *argv[])                                   //USAGE: LSALEXAMPLE directory
{
struct stat s;
stat(argv[1], &s);
const char *patt=argv[1];
if (argc <2){
ProcessCurrentDirectory();
return 0;
}


if(S_ISDIR(s.st_mode)){

ProcessSelectedDirectory(patt);

return 0;

}else {
fprintf(stderr, "ls: cannot access '%s': No such file or directory\n",argv[1]);
}

}


bool is_direct(const char *path){
struct stat s;
if(lstat(path, &s) == -1) {
return false;
}
return S_ISDIR(s.st_mode);
}



const char *get_permissionsOfFiles(mode_t mode)
{
  char FILETYPE = '?';

  if (S_ISREG(mode)) FILETYPE = '-';
  if (S_ISLNK(mode)) FILETYPE = 'l';
  if (S_ISDIR(mode)) FILETYPE = 'd';
  if (S_ISBLK(mode)) FILETYPE = 'b';
  if (S_ISCHR(mode)) FILETYPE = 'c';
  if (S_ISFIFO(mode)) FILETYPE = '|';

  sprintf(PermissionBuffer, "%c%c%c%c%c%c%c%c%c%c %c%c%c", FILETYPE,
  mode & S_IRUSR ? 'r' : '-',
  mode & S_IWUSR ? 'w' : '-',
  mode & S_IXUSR ? 'x' : '-',
  mode & S_IRGRP ? 'r' : '-',
  mode & S_IWGRP ? 'w' : '-',
  mode & S_IXGRP ? 'x' : '-',
  mode & S_IROTH ? 'r' : '-',
  mode & S_IWOTH ? 'w' : '-',
  mode & S_IXOTH ? 'x' : '-',
  mode & S_ISUID ? 'U' : '-',
  mode & S_ISGID ? 'G' : '-',
  mode & S_ISVTX ? 'S' : '-');

  return (const char *)PermissionBuffer;
}



void SYSTEMPRINT(char *msg)
{
  perror(msg);
  exit(0);
}

static int returnOne (const struct dirent *unused)
{
  return 1;
}

void ptrch(char *point){
//point=argv[1];
}


void ProcessCurrentDirectory(){

  int myCounter,i;
  struct direct **files;
  struct stat statbuf;
  char date[256];
  struct passwd pwent;
  struct passwd *pwentp;
  struct group grp;
  struct group *grpt;
  struct tm time;
  char buf[1024];
  
  const char* pat;
  if(!getcwd(pathname, sizeof(pathname)))
    SYSTEMPRINT("Error: No Path Exists");

  myCounter = scandir(pathname, &files, returnOne, alphasort);

  if(myCounter > 0){
    printf("total %d\n",myCounter);
    for (i=0; i<myCounter; ++i){
      if (stat(files[i]->d_name, &statbuf) == 0){
        /* Print out type, permissions, and number of links. */
        printf("%10.10s", get_permissionsOfFiles(statbuf.st_mode));
        printf(" %d", statbuf.st_nlink);

        if (!getpwuid_r(statbuf.st_uid, &pwent, buf, sizeof(buf), &pwentp))
          printf(" %s", pwent.pw_name);
        else
          printf(" %d", statbuf.st_uid);

        if (!getgrgid_r (statbuf.st_gid, &grp, buf, sizeof(buf), &grpt))
          printf(" %s", grp.gr_name);
        else
          printf(" %d", statbuf.st_gid);

        /* Print size of file. */
        printf(" %5d", (int)statbuf.st_size);

        localtime_r(&statbuf.st_mtime, &time);
        /* Get localized date string. */
        strftime(date, sizeof(date), "%b %d %H:%M", &time);

        printf(" %s %s\n", date, files[i]->d_name);
      }

      free (files[i]);
    }
  }
}





void ProcessSelectedDirectory(const char *patt){
//printf("%s",pathname);
  int myCounter,i;
  struct direct **files;
  struct stat statbuf;
  char date[256];
  struct passwd pwent;
  struct passwd *pwentp;
  struct group grp;
  struct group *grpt;
  struct tm time;
  char buf[1024];
DIR *pot = patt;
  
 
  myCounter = scandir(pot, &files, returnOne, alphasort);

  if(myCounter > 0){
    printf("total %d\n",myCounter);
    for (i=0; i<myCounter; ++i){
      if (stat(files[i]->d_name, &statbuf) == 0){
        //print type, perms, and links
        printf("%10.10s", get_permissionsOfFiles(statbuf.st_mode));
        printf(" %d", statbuf.st_nlink);

        if (!getpwuid_r(statbuf.st_uid, &pwent, buf, sizeof(buf), &pwentp))
          printf(" %s", pwent.pw_name);
        else
          printf(" %d", statbuf.st_uid);

        if (!getgrgid_r (statbuf.st_gid, &grp, buf, sizeof(buf), &grpt))
          printf(" %s", grp.gr_name);
        else
          printf(" %d", statbuf.st_gid);

        //print filesize
        printf(" %5d", (int)statbuf.st_size);

        localtime_r(&statbuf.st_mtime, &time);
        //dates
        strftime(date, sizeof(date), "%b %d %H:%M", &time);

        printf(" %s %s\n", date, files[i]->d_name);
      }

      free (files[i]);
    }
  }


}









