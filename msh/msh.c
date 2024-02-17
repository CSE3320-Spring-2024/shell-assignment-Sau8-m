// The MIT License (MIT)
// 
// Copyright (c) 2024 Trevor Bakker 
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

#define MAX_NUM_ARGUMENTS 5
#define MAX_COMMAND_SIZE 255
#define WHITESPACE "\t \n"

void exe(char *argv[]);

int main( int argc, char *argv[] )
{
    char *command_string = (char*)malloc(MAX_COMMAND_SIZE);
    char *path_Directory[4] = {"/bin","/urs/bin","/urs/local/bin","./"};
    char error_message[30] = "An error has occurred\n";
    FILE *file = NULL;

    if (argc ==2)
    {
       file = fopen(argv[1],"r");
       if(file == NULL)
       {
        
       }
    }
    
    else if (argc == 1)
    while(1)
    {
        printf("msh> ");
        
        while (!fgets(command_string,MAX_COMMAND_SIZE,stdin));
    
        char *token[MAX_NUM_ARGUMENTS];
        int token_count =0;
        char *argument_ptr;
        char *working_string = strdup(command_string);
        char *head_prt = working_string;

        while(((argument_ptr= strsep(&working_string,WHITESPACE)) !=NULL) && (token_count<MAX_NUM_ARGUMENTS))
        {
        token[token_count] = strndup(argument_ptr,MAX_COMMAND_SIZE);
        if(strlen(token[token_count]) == 0)
        {
            token[token_count] =NULL;
        }
        token_count++;
        }
        //for(int i =0 ; i<token_count;i++)
        //{
         //   printf("%d   %s\n",i, token[i]);
        //}
        
        if (token[0] && strcmp(token[0], "exit") == 0)
        {
                if(token[1] != NULL)
                {
                    write(STDERR_FILENO,error_message,strlen(error_message));
                }
                else
                    exit(0);
        }
        else if (token[0] && strcmp(token[0], "pwd")==0)
        {
            char pwd[MAX_COMMAND_SIZE];
            printf("%s\n",getcwd(pwd,50));
            
        }
        //else if(token[0] && strcmp(token[0],"ls")==0)
        //{
        //    exe(token);
        //}
        else if(token[0] && strcmp(token[0],"cd")==0)
        {
            
            if( token[1] == NULL || token[2] != NULL)
            {
                write(STDERR_FILENO,error_message,strlen(error_message));
            }
            else 
            {
                chdir(token[1]);
                //printf("%s\n",getcwd(token[1],50));
            }
            
        }
        
        else
        {
            int found = 0;
            char path[MAX_COMMAND_SIZE];
            for(int i=0; i< sizeof(path_Directory)/sizeof(path_Directory[0]);i++ )
            {
                sprintf(path,"%s/%s", path_Directory[i],token[0]);
                if(access(path,X_OK)==0)
                {
                    found = 1;
                    break;
                }
            }

            //printf ("%s\n",path);
            //printf("%d\n",found);

            pid_t pid = fork();
            if (pid == 0)
            {
                if (found == 1)
                {
                    for(int i =0; i<MAX_NUM_ARGUMENTS;i++)
                    {
                        if(token[i] && strcmp(token[i], ">")==0)
                        {
                            int fd = open(token[i+1],O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
                            if (fd < 0)
                            {
                                write(STDERR_FILENO,error_message,strlen(error_message));
                                exit(0);
                            }
                            dup2(fd,1);
                            close(fd);

                            token[i] = NULL;
                        }  
                    }
                    execvp(token[0],token);
                }
            }
            else if(pid >0)
            {
                int status;
                wait(&status);
            }
            else  
            {
                write(STDERR_FILENO,error_message,strlen(error_message));
            }   
        }
    free(head_prt);
    }

return 0;
}


/*void exe(char *argv[])
{
    pid_t pid = fork();
    if(pid ==0)
    {
        if(access("/bin/ls",X_OK)==0)
        {
            int ret = execvp(argv[0], argv);
            if(ret == -1)
            {
                perror("execfail");
            }
        }

    }
    else{
        int status;
        wait(&status);
    }
    
}*/