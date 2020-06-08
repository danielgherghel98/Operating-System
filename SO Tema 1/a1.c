#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
void listDir(const char *path)
{
    DIR *dir = NULL;
    struct dirent *entry = NULL;
    char filePath[512];
    
    dir = opendir(path);
    if(dir == NULL) {
        printf("ERROR\n invalid directory path");
        exit(0);
    }
    printf("SUCCESS\n");
    while((entry = readdir(dir)) != NULL) {
        
        if(strcmp(entry->d_name,".") && strcmp(entry->d_name,"..")){
            
            snprintf(filePath, 512, "%s/%s", path, entry->d_name);
            printf("%s\n", filePath);
        }
        
    }
    closedir(dir);
    
}
void listgreater(const char *path,int SIZE)
{
    DIR *dir = NULL;
    struct dirent *entry = NULL;
    char filePath[512];
    struct stat statbuf;
    
    dir = opendir(path);
    if(dir == NULL) {
        printf("ERROR\n invalid directory path");
        exit(0);
    }
    printf("SUCCESS\n");
    while((entry = readdir(dir)) != NULL) {
        
        if(strcmp(entry->d_name,".") && strcmp(entry->d_name,"..")){
            snprintf(filePath, 512, "%s/%s", path, entry->d_name);
            if(lstat(filePath, &statbuf) == 0)
            if(S_ISREG(statbuf.st_mode)){
                int fd = open(filePath, O_RDONLY);
                if(fd == -1) {
                    perror("Could not open input file");
                    //return 1;
                }

                int size = lseek(fd, 0, SEEK_END);
                //printf("%d  %d\n",size,fd);
                if(size>SIZE){
                    
                    printf("%s\n", filePath);
                }
            }
        }
        
    }
    closedir(dir);
    
}
void listgreaterR(const char *path,int SIZE)
{
    DIR *dir = NULL;
    struct dirent *entry = NULL;
    char filePath[512];
    struct stat statbuf;
    
    dir = opendir(path);
    if(dir == NULL) {
        printf("ERROR\n invalid directory path");
        return;
    }
        //printf("SUCCESS\n");
        while((entry = readdir(dir)) != NULL) {
            
            if(strcmp(entry->d_name,".") && strcmp(entry->d_name,"..")){
                snprintf(filePath, 512, "%s/%s", path, entry->d_name);
                if(lstat(filePath, &statbuf) == 0){
                    if(S_ISDIR(statbuf.st_mode))
                    {
                        listgreaterR(filePath,SIZE);
                    }
                    else
                    {
                        int fd = open(filePath, O_RDONLY);
                        if(fd == -1) {
                        perror("Could not open input file");
                        //return 1;
                    }

                    int size = lseek(fd, 0, SEEK_END);
                    //printf("%d  %d\n",size,fd);
                    if(size>SIZE){
                        
                        printf("%s\n", filePath);
                    }
                }
                }
            }
            
        }
        closedir(dir);
    
    
}
void listRec(const char *path)
{
    DIR *dir = NULL;
    struct dirent *entry = NULL;
    char fullPath[512];
    struct stat statbuf;

    dir = opendir(path);
    if(dir == NULL) {
        printf("ERROR\n invalid directory path");
        exit(0);
    }
    //printf("SUCCESS\n");
    while((entry = readdir(dir)) != NULL) {
        if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            snprintf(fullPath, 512, "%s/%s", path, entry->d_name);
            if(lstat(fullPath, &statbuf) == 0) {
                printf("%s\n", fullPath);
                if(S_ISDIR(statbuf.st_mode)) {
                    listRec(fullPath);
                }
            }
        }
    }
    closedir(dir);
}
void listnamestartwith(const char *path,const char *name)
{
    DIR *dir = NULL;
    struct dirent *entry = NULL;
    char filePath[512];
    //char* c="";
    
    dir = opendir(path);
    if(dir == NULL) {
        printf("ERROR\n invalid directory path");
        exit(0);
    }
    printf("SUCCESS\n");
    while((entry = readdir(dir)) != NULL) {
        
        if(strcmp(entry->d_name,".") && strcmp(entry->d_name,".."))
        {
            if(strncmp(entry->d_name,name,strlen(name))==0){
            snprintf(filePath, 512, "%s/%s", path, entry->d_name);
            printf("%s\n", filePath);
            }
        }
        
    }
    closedir(dir);
    
}

void listnamestartwithR(const char *path,const char *name)
{
    DIR *dir = NULL;
    struct dirent *entry = NULL;
    char fullPath[512];
    struct stat statbuf;
    
    
    dir = opendir(path);
    if(dir == NULL) {
        printf("ERROR\n invalid directory path");
        exit(0);
    }
    printf("SUCCESS\n");
    while((entry = readdir(dir)) != NULL) {
        if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
          if(strncmp(entry->d_name,name,strlen(name))==0){
            snprintf(fullPath, 512, "%s/%s", path, entry->d_name);
            if(lstat(fullPath, &statbuf) == 0) {
                printf("%s\n", fullPath);
                break;
                if(S_ISDIR(statbuf.st_mode)) {
                    listRec(fullPath);
                }
            }
          }
        }
    }
    closedir(dir);
    
}
void findall(const char *path)
{
    DIR *dir = NULL;
    struct dirent *entry = NULL;
    char fullPath[512];
    struct stat statbuf;
    char magic[3];
    int header[2];
    int version=0;
    int sections=0;
    char name[10];
    int type=0;
    char offset[5];
    int size=0;
    

    dir = opendir(path);
    if(dir == NULL) {
        printf("ERROR\n invalid directory path");
        exit(0);
    }
    //printf("SUCCESS\n");
    while((entry = readdir(dir)) != NULL) {
        if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            snprintf(fullPath, 512, "%s/%s", path, entry->d_name);
            if(lstat(fullPath, &statbuf) == 0) {
                if(S_ISDIR(statbuf.st_mode)) {
                    findall(fullPath);
                }
                else{
                    int fd = open(fullPath, O_RDONLY);
                    int numbertype=0;
                    if(fd == -1) {
                        perror("Could not open input file\n");
                        close(fd);
                        exit(0);
                    }
                    if(read(fd, magic, 2)!=2)
                    {
                        //printf("Nu s-a putut citi magic.\n");
                        exit(0);
                    }
                    magic[2]=0;
                    if(read(fd, header, 2)!=2)
                    {
                        //printf("Nu s-a putut citi header-ul.\n");
                        exit(0);
                    }
                    if(read(fd, &version, 1)!=1)
                    {
                        //printf("Nu s-a putut citi version.\n");
                        exit(0);
                    }
                    if(read(fd, &sections, 1)!=1)
                    {
                        //printf("Nu s-a putut citi nr_sections.\n");
                        exit(0);
                    }
                    //printf("%d  %d\n",version,sections);
                    
                    if(strcmp(magic,"TG")!=0)
                    {
                        //printf("ERROR\n");
                        //printf("Wrong magic\n");
                        exit(0);
                    }
                    
                    else if((version<77)||(version>135))
                    {
                        //printf("ERROR\n");
                        //printf("Wrong version\n");
                        exit(0);
                    }
                    else if((sections<2)||(sections>12))
                    {
                        //printf("ERROR\n");
                        //printf("Wrong sect_nr\n");
                        exit(0);
                    }
                    else {
                        for(int i=1; i<=sections; i++)
                        {
                            read(fd, name, 10);
                            read(fd, &type, 1);
                            read(fd, offset, 4);
                            read(fd, &size, 4);
                            if((type<10)||(type>85))
                            {
                                exit(0);
                            }
                            else if(type==10) { 
                                numbertype=numbertype+1;
                                
                            }
                        }
                    
                    }
                    close(fd);
                    if(numbertype>=3)
                    {
                        printf("%s\n", fullPath);
                                    
                    }
                }
            }
        }
    }
    
    closedir(dir);
}
void parse(const char *path)
{   
    char magic[3];
    int header[2];
    int version=0;
    int sections=0;
    char name[10];
    int type=0;
    char offset[5];
    int size=0;
    int fd = open(path, O_RDONLY);
    if(fd == -1) {
        perror("Could not open input file\n");
        exit(0);
    }
    if(read(fd, magic, 2)!=2)
    {
        printf("Nu s-a putut citi magic.\n");
        exit(0);
    }
    magic[2]=0;
    if(read(fd, header, 2)!=2)
    {
        printf("Nu s-a putut citi header-ul.\n");
        exit(0);
    }
     if(read(fd, &version, 1)!=1)
    {
        printf("Nu s-a putut citi version.\n");
        exit(0);
    }
    if(read(fd, &sections, 1)!=1)
    {
        printf("Nu s-a putut citi nr_sections.\n");
        exit(0);
    }
    //printf("%d  %d\n",version,sections);
    
    if(strcmp(magic,"TG")!=0)
    {
        printf("ERROR\n");
        printf("Wrong magic\n");
        exit(0);
    }
    
    else if((version<77)||(version>135))
    {
        printf("ERROR\n");
        printf("Wrong version\n");
        exit(0);
    }
    else if((sections<2)||(sections>12))
    {
        printf("ERROR\n");
        printf("Wrong sect_nr\n");
        exit(0);
    }
    else {
        printf("SUCCESS\n");
        printf("version=%d\n",version);
        printf("nr_sections=%d\n",sections);
        for(int i=1; i<=sections; i++)
        {
            read(fd, name, 10);
            read(fd, &type, 1);
            read(fd, offset, 4);
            read(fd, &size, 4);
            if((type<10)||(type>85))
            {
                printf("ERROR\n");
                printf("Wrong sect_types\n");
                exit(0);
            }
            else{ 
                printf("section%d: %s %d %d\n",i,name,type,size);
            }
        }
    
    }
    close(fd);
    
}

int corrupted(const char *path)
{   
    char magic[3];
    int header[2];
    int version=0;
    int sections=0;
    char name[10];
    int type=0;
    char offset[5];
    int size=0;
    int fd = open(path, O_RDONLY);
    if(fd == -1) {
        perror("Could not open input file\n");
        return 1;
    }
    if(read(fd, magic, 2)!=2)
    {
        printf("Nu s-a putut citi magic.\n");
        return 1;
    }
    magic[2]=0;
    if(read(fd, header, 2)!=2)
    {
        printf("Nu s-a putut citi header-ul.\n");
        return 1;
    }
     if(read(fd, &version, 1)!=1)
    {
        printf("Nu s-a putut citi version.\n");
        return 1;
    }
    if(read(fd, &sections, 1)!=1)
    {
        printf("Nu s-a putut citi nr_sections.\n");
        return 1;
    }
    
    if(strcmp(magic,"TG")!=0)
    {
        printf("ERROR\n");
        printf("wrong magic\n");
        return 1;
    }
    
    else if((version<77)||(version>135))
    {
        printf("ERROR\n");
        printf("wrong version\n");
        return 1;
    }
    else if((sections<2)||(sections>12))
    {
        printf("ERROR\n");
        printf("wrong sect_nr\n");
        return 1;
    }
    else {
        for(int i=1; i<=sections; i++)
        {
            read(fd, name, 10);
            read(fd, &type, 1);
            read(fd, offset, 4);
            read(fd, &size, 4);
            if((type<10)||(type>85))
            {
                printf("ERROR\n");
                printf("wrong sect_types\n");
                return 1;
            }
            
        }
    
    }
    close(fd);
    return 0;
    
}
void extract(const char *path,int line,int sectiune)
{   
    char magic[3];
    int header[2];
    int version=0;
    int sections=0;
    char name[10];
    int type=0;
    int offset=0;;
    int size=0;
    int fd = open(path, O_RDONLY);
    if(fd == -1) {
        perror("Could not open input file\n");
        exit(0);
    }
    if(read(fd, magic, 2)!=2)
    {
        printf("Nu s-a putut citi magic.\n");
        exit(0);
    }
    magic[2]=0;
    if(read(fd, header, 2)!=2)
    {
        printf("Nu s-a putut citi header-ul.\n");
        exit(0);
    }
     if(read(fd, &version, 1)!=1)
    {
        printf("Nu s-a putut citi version.\n");
        exit(0);
    }
    if(read(fd, &sections, 1)!=1)
    {
        printf("Nu s-a putut citi nr_sections.\n");
        exit(0);
    }
    //printf("%d  %d\n",version,sections);
    
    if(strcmp(magic,"TG")!=0)
    {
        printf("ERROR\n");
        printf("Wrong magic\n");
        exit(0);
    }
    
    else if((version<77)||(version>135))
    {
        printf("ERROR\n");
        printf("Wrong version\n");
        exit(0);
    }
    else if((sections<2)||(sections>12))
    {
        printf("ERROR\n");
        printf("Wrong sect_nr\n");
        exit(0);
    }
    else {
        
        if(sectiune<sections){
            for(int i=1; i<=sectiune; i++)
            {
                read(fd, name, 10);
                read(fd, &type, 1);
                read(fd, &offset, 4);
                read(fd, &size, 4);
                if((type<10)||(type>85))
                {
                    printf("ERROR\n");
                    printf("Wrong sect_types\n");
                    exit(0);
                }
                
            }
	    printf("SUCCESS\n");
            lseek(fd, offset, SEEK_END);
		int rand=0;
		char c=0;
		while (rand!=line)
		{
			if(read(fd, &c, 1)!=1)
				{
					exit(0);
				}
			if(c==0x0A)
				rand=rand+1;
		}
		c=0;
		while (c!=0x0A)
		{
			if(read(fd, &c, 1)!=1)
				{
					exit(0);
				}
			if(c!=0x0A)
				printf("%c",c);
		}
		printf("\n");
        }

    }
    close(fd);
    
}
int main(int argc, char **argv){
    
    if(argc >= 2){
        if ((strcmp(argv[1], "extract")==0))
        {
            char* path=argv[2];
                path=path+strlen("path=");
            char* section=argv[3];
                section=section+strlen("section=");
                char* line=argv[4];
            line=line+strlen("'line=");
            int linie=0;
            int sectiune=0;
            sscanf(section,"%d",&sectiune);
                sscanf(line,"%d",&linie);
                extract(path,linie,sectiune);
        }
        else if ((strcmp(argv[1], "findall")==0)&&(strncmp(argv[2],"path=",strlen("path="))==0))
        {
            char* path=argv[2];
            path=path+strlen("path=");
            printf("SUCCESS\n");
            findall(path);
        }
        if (strcmp(argv[1], "parse")==0)
        {
            char* path=argv[2];
            path=path+strlen("path=");
            if(corrupted(path)==0)
            parse(path);
            
        }
        if ((strcmp(argv[1], "list")==0)&&(strncmp(argv[2],"size_greater=",strlen("size_greater="))==0)&&(strcmp(argv[3], "recursive") == 0))
        {
            char* c=argv[2];
            int SIZE;
            c=c+strlen("size_greater=");
            sscanf(c,"%d",&SIZE);
            char* path=argv[4];
            path=path+strlen("path=");
            //printf("%d %s\n",SIZE,path);
            printf("SUCCESS\n");
            listgreaterR(path,SIZE);

        }
        else if ((strcmp(argv[1], "list")==0)&&(strcmp(argv[2], "recursive") == 0)&&(strncmp(argv[3],"size_greater=",strlen("size_greater="))==0))
        {
            char* c=argv[3];
            int SIZE;
            c=c+strlen("size_greater=");
            sscanf(c,"%d",&SIZE);
            char* path=argv[4];
            path=path+strlen("path=");
            //printf("%d %s\n",SIZE,path);
            printf("SUCCESS\n");
            listgreaterR(path,SIZE);

        }
        else if ((strcmp(argv[1], "list")==0)&&(strncmp(argv[2],"size_greater=",strlen("size_greater="))==0))
        {
            char* c=argv[2];
            int SIZE;
            c=c+strlen("size_greater=");
            sscanf(c,"%d",&SIZE);
            char* path=argv[3];
            path=path+strlen("path=");
            //printf("%d %s\n",SIZE,path);
            
            listgreater(path,SIZE);

        }
        else if ((strcmp(argv[1], "list")==0)&&(strcmp(argv[2], "recursive") == 0)&&(strncmp(argv[3],"name_starts_with=",strlen("name_starts_with="))==0))
        {
            char* c=argv[3];
            char* path=argv[4];
            c=c+strlen("name_starts_with=");
            path=path+strlen("path=");
            
            listnamestartwithR(path,c);
        }
        else if ((strcmp(argv[1], "list")==0)&&(strncmp(argv[2],"name_starts_with=",strlen("name_starts_with="))==0)&&(strcmp(argv[3], "recursive") == 0))
        {
            char* c=argv[2];
            char* path=argv[4];
            c=c+strlen("name_starts_with=");
            path=path+strlen("path=");
            
            listnamestartwithR(path,c);
        }
        else if ((strcmp(argv[1], "list")==0)&&(strncmp(argv[2],"name_starts_with=",strlen("name_starts_with="))==0))
        {
            char* c=argv[2];
            char* path=argv[3];
            c=c+strlen("name_starts_with=");
            path=path+strlen("path=");
            
            listnamestartwith(path,c);
        }
        else if(strcmp(argv[1], "variant") == 0){
            printf("57861\n");
        }
        else if((strcmp(argv[1], "list") == 0)&&(strcmp(argv[2], "recursive") != 0))
        {
            char* path=argv[2];
            path=path+strlen("path=");
            
            listDir(path);
            
        }
        else if((strcmp(argv[1], "list") == 0)&&(strcmp(argv[2], "recursive") == 0))
        {
            
            char* path=argv[3];
            path=path+strlen("path=");
            //printf("%s\n",path);
            printf("SUCCESS\n");
            listRec(path);
            
        }
        
    }
    return 0;
}