#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>

#include "md5.h"

#include <switch.h>

#define READ_DATA_SIZE	1024
#define MD5_SIZE		16
#define MD5_STR_LEN		(MD5_SIZE * 2)

//The SD card is automatically mounted as the default device, usable with standard stdio. SD root dir is located at "/" (also "sdmc:/" but normally using the latter isn't needed).
//The default current-working-directory when using relative paths is normally the directory where your application is located on the SD card.

int Compute_file_md5(const char *file_path, char *value);

int main(int argc, char **argv)
{

    char buf[80];
	int ret;
	const char *file_path = "./test.txt";
	char md5_str[MD5_STR_LEN + 1];
	
	consoleInit(NULL);

    DIR* dir;
    struct dirent* ent;

    dir = opendir("");//Open current-working-directory.
	
    
    getcwd(buf, sizeof(buf));
	//printf("current working directory : %s\n", buf);


	ret = Compute_file_md5(file_path, md5_str);//print md5.buffer 
	
	if (0 == ret)
	{
		printf("[file - %s] md5 value:\n", file_path);
		printf("%s\n", md5_str);
	}
	

    if(dir==NULL)
    {
        printf("Failed to open dir.\n");
    }
    else if(0)
    {   
        printf("Dir-listing for '':\n");
        while ((ent = readdir(dir)))
        {
            printf("d_name: %s\n", ent->d_name);
        }
        closedir(dir);
        printf("Done.\n");
    }

    // Main loop
    while(appletMainLoop())
    {
        //Scan all the inputs. This should be done once for each frame
        hidScanInput();

        //hidKeysDown returns information about which buttons have been just pressed (and they weren't in the previous frame)
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

        if (kDown & KEY_PLUS) break; // break in order to return to hbmenu

        consoleUpdate(NULL);
    }

    consoleExit(NULL);
    return 0;
}



int Compute_file_md5(const char *file_path, char *md5_str)
{
	int i;
	int fd;
	int ret;
	unsigned char data[READ_DATA_SIZE];
	unsigned char md5_value[MD5_SIZE];
	unsigned char a[64];

	MD5_CTX md5;
 
	fd = open(file_path, "r");
	
	if (-1 == fd)
	{
		perror("open21");
		return -1;
	}
 
	// init md5
	MD5Init(&md5);
 
	while (1)
	{
		ret = read(fd, data, READ_DATA_SIZE);
		if (-1 == ret)
		{
			perror("read");
			return -1;
		}
 
		MD5Update(&md5, data, ret);
 
		if (0 == ret || ret < READ_DATA_SIZE)
		{
			break;
		}
	}
 
	close(fd);
 
	MD5Final(&md5, md5_value);
	//snprintf(a, 63, "%s", md5.buffer);
	//printf("buffer64:%s\n", a);

 
	for(i = 0; i < MD5_SIZE; i++)
	{
		snprintf(md5_str + i*2, 2+1, "%02x", md5_value[i]);
	}
	md5_str[MD5_STR_LEN] = '\0'; // add end
 
	return 0;
}
