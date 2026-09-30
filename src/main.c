#include<stdio.h>
#include<string.h>

#define INPUT_SIZE 256 

int main(void){

    char command[INPUT_SIZE];

    printf("Mini started.\n");
    printf("Type /help for available commands.\n\n");


    while(1){
        printf("MiniDB> ");

        // READ user Input 
        if(fgets(command , INPUT_SIZE , stdin)==NULL){
            break ;
        }

        // Remove the newLine Character 
        command[strcspn(command , "\n")] = '\0';

        if(strcmp(command , ".exit") ==0){
            printf("Goodbye!\n");
            break ;
        }

        else if(strcmp(command , ".help")==0){
            printf("\nAvailable commands:\n");
            printf(".help - Show available commands\n");
            printf(".exit - Exit MiniDB\n\n");
        }

        else{
            printf("Unknow command .Type .help for available commands.\n");
        }
    }

    return 0 ;
}