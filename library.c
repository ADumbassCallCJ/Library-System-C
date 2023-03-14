#include <stdio.h>      
#include <stdlib.h>     
#include <string.h>
#include <ctype.h>

struct book{
    char *title;
    char *author;
    char *subject;
};

struct library{
    struct book collection;
    int num_books;
    struct library *next;
};

typedef struct library Library;
typedef struct library *LibraryPtr; //a pointer to struct library

//prototype
void copybook(struct book* dest, struct book* source);
void addBook(LibraryPtr * sPtr, char sentence[200], FILE *ofPtr);
void delete(LibraryPtr *sPtr, char sentence[200], FILE *ofPtr);
void deleteByAuthor(LibraryPtr *sPtr, char sentence[200], FILE *ofPtr);
void Search(LibraryPtr sPtr, char sentence[200], FILE *ofPtr);
void SearchByAuthor(LibraryPtr sPtr, char sentence[200], FILE *ofPtr);
void ListByAuthor(LibraryPtr sPtr, char sentence[200], FILE *ofPtr);
void ListBySubject(LibraryPtr sPtr, char sentence[200], FILE *ofPtr);
void listAll(LibraryPtr sPtr, FILE *ofPtr);
//void printList(LibraryPtr currentPtr);//tempo
int isEmpty(LibraryPtr sPtr);

//main
int main(){

    FILE *lfPtr; //library file pointer
    FILE *ofPtr; //output file pointer
    lfPtr=fopen("library.txt", "r"); //open library.txt to read
    ofPtr=fopen("output.txt", "w"); //open output.txt to write

    LibraryPtr startPtr = NULL; //initially no nodes
    
    unsigned int update; //number of update made
    fscanf(lfPtr, "%u", &update); //scan the first number, which is update
    int operations, i;
    struct book Book, *bookPtr;
    Book.title=malloc(sizeof(char)*50);
    Book.author=malloc(sizeof(char)*50);
    Book.subject=malloc(sizeof(char)*50);
    char sentence[200], *token; //sentence for fgets to get the line, token for strtok

    for (i=0; i<update; i++){
        fscanf(lfPtr, "%d", &operations); //scan for operation
        fgets(sentence, 200, lfPtr); //get entire line
        switch (operations){
            case 1: //if 1, add book
                addBook(&startPtr, sentence, ofPtr);        
                //printList(startPtr); for checking
                fprintf(ofPtr, "\n");
                break;

            case 2:
                delete(&startPtr, sentence, ofPtr);
                //printList(startPtr); for checking
                break;

            case 3:
                Search(startPtr, sentence, ofPtr);
                break;
            
            case 4:
                SearchByAuthor(startPtr, sentence, ofPtr);
                break;

            case 5:
                ListByAuthor(startPtr, sentence, ofPtr);
                break;

            case 6:
                ListBySubject(startPtr, sentence, ofPtr);
                break;

            case 7:
                listAll(startPtr, ofPtr);
                break;

            default:
                fprintf(ofPtr, "Invalid operation\n");
                break;
        } //end switch
    } //end for

    fclose(ofPtr);
    fclose(lfPtr);
    return 0;
}//end main

//function

void copybook(struct book* dest, struct book* source){
    strcpy(dest->title, source->title); 
    strcpy(dest->author, source->author);
    strcpy(dest->subject, source->subject);
}

void addBook(LibraryPtr * sPtr, char sentence[200], FILE *ofPtr){
    char *title, *author, *subject, *token;
    title = malloc(sizeof(char)*50);
    author = malloc(sizeof(char)*50);
    subject = malloc(sizeof(char)*50);

    token = strtok(sentence, " ");
    if (token != NULL && strlen(token) <=49){
        strcpy(title, token);
        token = strtok(NULL, " ");
        if (token != NULL && strlen(token) <=49){
            strcpy(author, token);
            token = strtok(NULL, " " );
            if (token != NULL && strlen(token) <=49){
                strcpy(subject, token);
                subject=strtok(subject, "\n");
                
                LibraryPtr newPtr;
                LibraryPtr currentPtr;
                LibraryPtr previousPtr;
                currentPtr = *sPtr; //at first this is also NULL, cuz startPtr = NULL
                previousPtr = NULL;
    
                newPtr=malloc(sizeof(Library)); //create node

                if (newPtr != NULL){ //is space available?
                    newPtr->collection.title = title; //place value into node
                    newPtr->collection.author = author;
                    newPtr->collection.subject = subject;

                    newPtr->next = NULL; //node doesnt link to another node
        
                    while (currentPtr != NULL){   //loop to go last node
                        previousPtr = currentPtr;
                        currentPtr = currentPtr->next;
                    }//end while
                    if (previousPtr == NULL){  //this can only run once, to bind first node as header(*sPtr)
                        newPtr->next = *sPtr;
                        *sPtr = newPtr;
                    }// end if
                    else { 
                        previousPtr->next = newPtr;
                    }//end else
                    currentPtr = newPtr; //insert new node at the end

                    fprintf(ofPtr, "The book %s author %s subject %s has been added to the library\n"
                    , currentPtr->collection.title, currentPtr->collection.author, currentPtr->collection.subject);

                }//end if
            }//end if3
            else {
                fprintf(ofPtr, "ERROR, No input or Character of subject more than 49\n");
            }//end else3
        }//end if2
        else {
            fprintf(ofPtr, "ERROR, No input or Character of author more than 49\n");
        }//end else2
    }//end if1
    else {
        fprintf(ofPtr, "ERROR, No input or Character of title more than 49\n");
    }//end else1
}//end function

void delete(LibraryPtr *sPtr, char sentence[200], FILE *ofPtr){
    int nodeNum=0, bookFound;
    char *title;
    title = malloc(sizeof(char)*50);
    LibraryPtr tempPtr;
    LibraryPtr currentPtr;
    LibraryPtr previousPtr;
    LibraryPtr lastPtr;
    LibraryPtr secLastPtr;
    LibraryPtr thirdLastPtr;
    LibraryPtr nodePtr; //for counting how many nodes
    nodePtr = *sPtr;

    title = strtok(sentence, " ");
    title = strtok(title, "\n");
    if (title != NULL){
        lastPtr = (*sPtr)->next; //place lastPtr at second node
        secLastPtr = (*sPtr); //place secLastPtr at first node
        thirdLastPtr = NULL; //thirdLastPtr NULL first

        while (nodePtr != NULL){
            nodeNum++;
            nodePtr = nodePtr->next;
        }//end while
        nodePtr = *sPtr; //reset it back to original location

        while (lastPtr->next != NULL){ //traverse until the end
            lastPtr = lastPtr->next; //so lastPtr is actually at last node
            thirdLastPtr = secLastPtr; //thirdLastPtr take secLastPtr place, making it real last third node
            secLastPtr = secLastPtr->next; //secLastPtr at second last node
        }// end while
        
        if (strcmp(title, (*sPtr)->collection.title) == 0){  
            
            if (nodeNum < 3){  
                tempPtr = *sPtr;
                *sPtr = (*sPtr)->next;
                free(tempPtr);
            }
            else{
                tempPtr = *sPtr;  
                thirdLastPtr->next = lastPtr; 
                secLastPtr->next = (*sPtr)->next; 
                *sPtr = secLastPtr;
                free(tempPtr);
                bookFound = 1;
            }
            fprintf(ofPtr, "The book %s has been removed from the library\n", title);
        }//end outer if
        else { 
            previousPtr = *sPtr;
            currentPtr = (*sPtr)->next;
            //loop to find correct location in list
            while (currentPtr != NULL && strcmp(currentPtr->collection.title, title) != 0){
                previousPtr = currentPtr;
                currentPtr = currentPtr->next;
            }//end while

            if (currentPtr != NULL){
                if (currentPtr == lastPtr){ 
                    free(lastPtr);
                    secLastPtr->next = NULL;
                }
                else{
                    tempPtr = currentPtr;
                    previousPtr->next = secLastPtr; //link the previous node of the one we want to delete to second last node
                    if (currentPtr != thirdLastPtr){ //to prevent breaking
                        secLastPtr->next = currentPtr->next; //link second last node to the next node of deleted node
                        thirdLastPtr->next = lastPtr;//third last was originally pointing to second last node, so now link it to last node 
                    }                                //as second last is moved to deleted node
                    free(tempPtr);// just straight delete the node if the one we wanted to delete is already the last third node
                    bookFound = 1;
                }
                fprintf(ofPtr, "The book %s has been removed from the library\n", title);
            }//end if
            if (bookFound != 1){ 
                fprintf(ofPtr, "The book %s does not exist in the library\n", title);
            }
        }//end outer else
    }//end if
    else{
        fprintf(ofPtr, "ERROR no input\n");
    }
    fprintf(ofPtr, "\n");
}//end func



void Search(LibraryPtr  sPtr, char sentence[200], FILE *ofPtr){
    char *title;  //search
    title = malloc(sizeof(char)*50);
    int bookFound;

    title = strtok(sentence, " ");
    title = strtok(title, "\n");
    if (title != NULL){
        while (sPtr != NULL){
            if (strcmp(sPtr->collection.title, title)== 0){
                bookFound=1;
                break;
            }//end if
            else{
                sPtr = sPtr->next;
            }//end else
        }//end while
        if (bookFound == 1){
        fprintf(ofPtr, "The book %s is currently in the library\n", title);
            }
        else{
         fprintf(ofPtr, "The book %s is currently not in the library\n", title);
        }
    }//end if
    else{
        fprintf(ofPtr, "ERROR no input\n");
    }
    fprintf(ofPtr, "\n");
}//end func

void SearchByAuthor(LibraryPtr sPtr, char sentence[200], FILE *ofPtr){
    char *author;
    author = malloc(sizeof(char)*50);
    int bookFound;

    author = strtok(sentence, " ");
    author = strtok(author, "\n");
    if (author != NULL){
        fprintf(ofPtr, "List of all book by %s\n", author);
        while (sPtr != NULL){
            if (strcasecmp(sPtr->collection.author, author)== 0){
                fprintf(ofPtr, "%s\n", sPtr->collection.title);
                sPtr = sPtr->next;
                bookFound = 1;
            }//end if
            else{
                sPtr = sPtr->next;
            }//end else
        }//end while
        if (bookFound != 1){
            fprintf(ofPtr, "There are no books authored by %s found in this library\n", author);
        } 
    }//end if
    else{
        fprintf(ofPtr, "ERROR no input\n");
    }
    fprintf(ofPtr, "\n");
}//end func

void ListByAuthor(LibraryPtr sPtr, char sentence[200], FILE *ofPtr){
    char *author, arr[30][50], hold[50]; //use 2D array to store the books
    author = malloc(sizeof(char)*50);
    int bookFound=0, i=0;

    author = strtok(sentence, " ");
    author = strtok(author, "\n");
    if (author != NULL){
        fprintf(ofPtr, "List of all book by %s\n", author);
        while (sPtr != NULL){
            if (strcasecmp(sPtr->collection.author, author)== 0){
                strcpy(arr[i], sPtr->collection.title);
                sPtr = sPtr->next;
                bookFound++;
                i++;
            }//end if
            else{
                sPtr = sPtr->next;
            }//end else
        }//end while
        if (bookFound > 0){ //bubble sort learnt in class
            for (int pass = 1; pass < bookFound; pass++){
                for (int i = 0; i < bookFound-1; i++){
                    if (tolower(arr[i][0]) >= tolower(arr[i+1][0])){
                        strcpy(hold, arr[i]);
                        strcpy(arr[i], arr[i+1]);
                        strcpy(arr[i+1], hold);
                    }//end inner if
                }//end inner for
            }//end outer for

            for (i=0; i<bookFound; i++){ //print book in ascending order
                fprintf(ofPtr, "%s\n", arr[i]);
            }
        }//end if for the arrangement

        if (bookFound == 0){
            fprintf(ofPtr, "There are no books authored by %s found in this library\n", author);
        } 
    }//end if
    else{
        fprintf(ofPtr, "ERROR no input\n");
    }
    fprintf(ofPtr, "\n");
}

void ListBySubject(LibraryPtr sPtr, char sentence[200], FILE *ofPtr){
    char *subject, arr[30][50], hold[50];
    subject = malloc(sizeof(char)*50);
    int bookFound=0, i=0;

    subject = strtok(sentence, " ");
    subject = strtok(subject, "\n");
    if (subject != NULL){
        fprintf(ofPtr, "List of all book on %s\n", subject);
        while (sPtr != NULL){
            if (strcasecmp(sPtr->collection.subject, subject)== 0){
                strcpy(arr[i], sPtr->collection.title);
                sPtr = sPtr->next;
                bookFound++;
                i++;
            }//end if
            else{
                sPtr = sPtr->next;
            }//end else
        }//end while
        if (bookFound > 0){
            for (int pass = 1; pass < bookFound; pass++){
                for (int i = 0; i < bookFound-1; i++){
                    if (tolower(arr[i][0]) <= tolower(arr[i+1][0])){
                        strcpy(hold, arr[i]);
                        strcpy(arr[i], arr[i+1]);
                        strcpy(arr[i+1], hold);
                    }//end inner if
                }//end inner for
            }//end outer for

            for (i=0; i<bookFound; i++){ //print book in descending order
                fprintf(ofPtr, "%s\n", arr[i]);
            }
        }//end if for the arrangement

        if (bookFound == 0){
            fprintf(ofPtr, "There are no books on subject %s found in this library\n", subject);
        } 
    }//end if
    else{
        fprintf(ofPtr, "ERROR no input\n");
    }
    fprintf(ofPtr, "\n");
}

void listAll(LibraryPtr  sPtr, FILE *ofPtr){  //list all function
    if (isEmpty(sPtr)){
        fprintf(ofPtr, "Library is empty.\n");
    }//end if
    else{
        fprintf(ofPtr, "List of all Books\n");
        while (sPtr != NULL){
        fprintf(ofPtr, "Title: %-49s Author: %-49s Subject: %s\n", sPtr->collection.title, sPtr->collection.author, sPtr->collection.subject);
        sPtr = sPtr->next;
        }//end while
    }//end else
    fprintf(ofPtr, "\n");
}//end func

int isEmpty(LibraryPtr sPtr){ //to check if its empty
    return sPtr == NULL;
}//end func

/*
void printList(LibraryPtr currentPtr){
    if (isEmpty(currentPtr)){ //if list is empty
        puts("List is empty.\n");
    }//end if
    else{
        puts("The list is:");
        //while not the end of list
        while (currentPtr != NULL){
            printf("%s --> ", currentPtr->collection.title);
            currentPtr = currentPtr->next;
        }//end while
        puts("NULL\n");
    }//end else
}//end func
*/
/*

-My library.txt when testing
15
1 test jack science 
1 hello_world jack computer_science 
4 test2
1 test2 jon fiction
1 zooBook jill science
5 jAcK
6 JaCk
7 science
4 test2
1 plsHelp john health
1 sendHelp john mental
8
2 hello_world
3 john
8

-My output.txt outcome
The book test author jack subject science has been added to the library

The book hello_world author jack subject computer_science has been added to the library

The book test2 is currently not in the library

The book test2 author jon subject fiction has been added to the library

The book zooBook author jill subject science has been added to the library

List of all book by jAcK
test
hello_world

List of all book by JaCk
hello_world
test

List of all book on science
zooBook
test

The book test2 is currently in the library

The book plsHelp author john subject health has been added to the library

The book sendHelp author john subject mental has been added to the library

List of all Books
Title: test                                              Author: jack                                              Subject: science
Title: hello_world                                       Author: jack                                              Subject: computer_science
Title: test2                                             Author: jon                                               Subject: fiction
Title: zooBook                                           Author: jill                                              Subject: science
Title: plsHelp                                           Author: john                                              Subject: health
Title: sendHelp                                          Author: john                                              Subject: mental

The book hello_world has been removed from the library

Sorry sir, I really tried, but my code won't work

List of all Books
Title: test                                              Author: jack                                              Subject: science
Title: plsHelp                                           Author: john                                              Subject: health
Title: test2                                             Author: jon                                               Subject: fiction
Title: zooBook                                           Author: jill                                              Subject: science
Title: sendHelp                                          Author: john                                              Subject: mental


*/
