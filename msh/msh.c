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

#define MAX_NUM_ARGUMENTS 5
#define MAX_COMMAND_SIZE 255
#define WHITESPACE "\t \n"

void exe(char *argv[]);

int main( int argc, char *argv[] )
{
    char *command_string = (char*)malloc(MAX_COMMAND_SIZE);
    char error_message[30] = "An error has occurred\n";
  
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
    for(int i =0 ; i<token_count;i++)
    {
        printf("%d   %s\n",i, token[i]);
    }
    
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
    pid_t pid = fork();
        if(pid ==0)
        {
            printf("%s\n",getcwd(token[1],50));
        }
        else{
            int status;
            wait(&status);
        }
   }
   else if(token[0] && strcmp(token[0],"ls")==0)
   {
        exe(token);
   }
   else if(token[0] && strcmp(token[0],"cd")==0)
   {
    pid_t pid = fork();
    
        if(pid==0)
        {
            if( token[1] == NULL || token[2] != NULL)
            {
                write(STDERR_FILENO,error_message,strlen(error_message));
            }
            else 
            {
                chdir(token[1]);
                printf("%s\n",getcwd(token[1],50));
            }
        }
        else{
            int status;
            wait(&status);
        }
    
   }
   
   free(head_prt);
  }
  
  return 0;
}

void exe(char *argv[])
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
    
}

