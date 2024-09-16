/* Rawand Bawatneh 1211403*/
//the name of file that I use unitial.txt for initial text
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define max 200

typedef struct undoStack {
    struct Unode *top;
    int size;
} undoStack;

typedef struct redoStack {
    struct Rnode *top;
    int size;
} redoStack;

typedef struct Unode {
    struct Unode *next;
    char ustr[max];
    int index;
    char status[max];
} Unode;

typedef struct Rnode {
    struct Rnode *next;
    char rstr[max];
    int index;
    char status[max];
} Rnode;

typedef struct Qnode {
    struct Qnode *next;
    char inStr[max];
} Qnode;

typedef struct Queue {
    Qnode *Front;
    Qnode *Rear;
    int size;
} Queue;

//////////FUNCTIONS//////////////

void displayMenu();
void load_file();
void print_loaded();
void insertQueue(Queue *q);
void pushUndo(undoStack *stack, const char *operation, const char *word, int index);
void printQueue(Queue *queue);
void PrintUndoStack(undoStack *stack);
void PrintRedoStack(redoStack *stack);
void removeWordFromFile(const char *fileName, const char *wordToRemove, undoStack *undo, redoStack *redo);
void addToUndoStack(undoStack *stack, const char *textToRemove, const char *status, int index);
void addToRedoStack(redoStack *stack, const char *textToAdd);
void undoOperation(undoStack *undo, redoStack *redo);
void writeToFile(const char *inputFileName, const char *outputFileName);
void redoOperation(undoStack *undo, redoStack *redo);
int main() {
    Queue q;
    q.Front = NULL;
    q.Rear = NULL;
    q.size = 0;

    undoStack undo;
    undo.top = NULL;
    undo.size = 0;

    redoStack redo;
    redo.top = NULL;
    redo.size = 0;

    int select;
    char removeText[max];
    displayMenu();
    scanf("%d", &select);
    while (select != 9) {
        switch (select) {
            case 1:
                load_file();
                break;
            case 2:
                print_loaded();
                break;
            case 3:
                insertQueue(&q);
                Qnode *curr = q.Front;
                while (curr != NULL) {
                pushUndo(&undo, "Insert", curr->inStr, -1);
                curr = curr->next;
                }
                printQueue(&q);
                break;
            case 4:
                printf("\n Enter the text that you want to remove from the text:\n");
                scanf("%s", removeText);
                removeWordFromFile("initial.txt", removeText, &undo, &redo);
                break;
            case 5:
                printf("Performing Undo operation...\n");
                undoOperation(&undo, &redo);
                break;
            case 6:
                printf("Performing Redo operation...\n");
                redoOperation(&undo, &redo);
                break;
            case 7:
                printf("\nUndo Stack:\n");
                PrintUndoStack(&undo);
                printf("\nRedo Stack:\n");
                PrintRedoStack(&redo);
                break;
           case 8:
                writeToFile("initial.txt", "output.txt");
                break;
            default:
                printf("Invalid selection. Please try again.\n");
                break;
        }
        displayMenu();
        scanf("%d", &select);
    }

    return 0;
}


// Display the menu
void displayMenu() {
    printf("\n1. Load the input file which contains the initial text.\n");
    printf("2. Print the loaded text.\n");
    printf("3. Insert strings to the text.\n");
    printf("4. Remove strings from the text.\n");
    printf("5. Perform Undo operation.\n");
    printf("6. Perform Redo operation.\n");
    printf("7. Print the Undo Stack and the Redo stack.\n");
    printf("8. Save the updated text to the output file.\n");
    printf("9. Exit.\n");
}

// Load data from file
void load_file() {
    FILE *input; // pointer to input file
    char fileName[100]; // string to store file name
    printf("\nPLEASE ENTER THE NAME OF THE INPUT FILE: \t");
    scanf("%s", fileName);
    input = fopen(fileName, "r");

    while (input == NULL) { // to check if the file does not exist

        printf("\n---------------------------------------------------------------------------\n\n");
        printf("FAILED TO OPEN THE FILE. THIS FILE DOES NOT EXIST.");
        printf("\n\n---------------------------------------------------------------------------\n\n");
        printf("\nPLEASE ENTER THE NAME OF THE INPUT FILE: \t ");
        scanf("%s", fileName);
        input = fopen(fileName, "r");
    }
    printf("The file is opened :)\n");
    fclose(input);
}

//print the loaded data
void print_loaded() {
    FILE *input; // pointer to input file
    char fileName[max]; // string to store file name
    printf("\nPLEASE ENTER THE NAME OF THE INPUT FILE: \t");
    scanf("%s", fileName);
    input = fopen(fileName, "r");

    while (input == NULL) { //to check if the file does not exist

        printf("\n---------------------------------------------------------------------------\n\n");
        printf("FAILED TO OPEN THE FILE. THIS FILE DOES NOT EXIST.");
        printf("\n\n---------------------------------------------------------------------------\n\n");
        printf("\nPLEASE ENTER THE NAME OF THE INPUT FILE: \t ");
        scanf("%s", fileName);
        input = fopen(fileName, "r");
    }
    char singleLine[max];
    while (fgets(singleLine, sizeof(singleLine), input) != NULL) {
        printf("%s", singleLine);
    }
    fclose(input);
}
void insertQueue(Queue *q) {
    // Clear the existing queue
    while (q->Front != NULL) {
        Qnode *temp = q->Front;
        q->Front = q->Front->next;
        free(temp);
    }
    q->Rear = NULL;
    q->size = 0;

    // Get input from the user
    char input[max];
    printf("\n Enter the statement to insert: ");
    getchar(); // Clear input buffer
    // Read the input statement including spaces
    fgets(input, sizeof(input), stdin);
    // Remove trailing newline character
    input[strcspn(input, "\n")] = '\0';
    // Add the input statement to the queue
    Qnode *n = (Qnode *)malloc(sizeof(Qnode));
    if (n == NULL) {
        printf("Memory allocation failed.");
        exit(EXIT_FAILURE);
    }
    strcpy(n->inStr, input);
    n->next = NULL;

    if (q->Front == NULL) {
        q->Front = n;
        q->Rear = n;
    } else {
        q->Rear->next = n;
        q->Rear = n;
    }
    q->size++;
}

void pushUndo(undoStack *stack, const char *operation, const char *word, int index) {
    Unode *n = (Unode *)malloc(sizeof(Unode));
    if (n == NULL) {
        printf("Memory allocation failed.");
        exit(EXIT_FAILURE);
    }
    n->next = NULL;

    // Copy operation, word, and index
    strcpy(n->status, operation);
    strcpy(n->ustr, word);
    n->index = index;

    // Push the new node to the undo stack
    if (stack->top == NULL) {
        stack->top = n;
    } else {
        n->next = stack->top;
        stack->top = n;
    }
}
void PrintUndoStack(undoStack *stack) {
    if (stack->top == NULL) {
        printf("Undo stack is empty.\n");
        return;
    }

    Unode *current = stack->top;
    printf("Contents of Undo Stack:\n");
    while (current != NULL) {
        printf("Index: %d, Status: %s, Content: %s\n", current->index, current->status, current->ustr);
        current = current->next;
    }
}

void PrintRedoStack(redoStack *stack) {
    if (stack->top == NULL) {
        printf("Redo stack is empty.\n");
        return;
    }

    Rnode *current = stack->top;
    printf("Contents of Redo Stack:\n");
    while (current != NULL) {
        printf("Index: %d, Status: %s, Content: %s\n", current->index, current->status, current->rstr);
        current = current->next;
    }
}

void printQueue(Queue *queue) {
    if (queue->Front == NULL) {
        printf("Queue is empty.\n");
        return;
    }
    FILE *output = fopen("initial.txt", "a");
    // Open the file in append mode to add new data at the end
    if (output == NULL) {
        printf("Error opening output file.\n");
        return;
    }

    Qnode *current = queue->Front;

    // Move to the end of the file
    fseek(output, 0, SEEK_END);

    // If the file is not empty, add a space before appending new data
    if (ftell(output) > 0) {
        fprintf(output, " ");
    }

    // Traverse the queue and write the content to the file
    while (current != NULL) {
    // Write the content of inStr to the file
        fprintf(output, "%s ", current->inStr);
        current = current->next;
    }

    fclose(output);
}

void addToUndoStack(undoStack *stack, const char *textToRemove, const char *status, int index) {
    Unode *current = stack->top;
    while (current != NULL) {
        if (strcmp(current->ustr, textToRemove) == 0) {
            strcpy(current->status, status);
            current->index = index;
            return;
        }
        current = current->next;
    }

    Unode *newNode = (Unode *)malloc(sizeof(Unode));
    if (newNode == NULL) {
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    strcpy(newNode->status, status);
    newNode->index = index;
    strcpy(newNode->ustr, textToRemove);
    newNode->next = stack->top;
    stack->top = newNode;
    stack->size++;
}

void addToRedoStack(redoStack *stack, const char *textToAdd) {
    Rnode *newNode = (Rnode *)malloc(sizeof(Rnode));
    if (newNode == NULL) {
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    strcpy(newNode->status, "Insert");
    newNode->index = -1;
    strcpy(newNode->rstr, textToAdd);
    newNode->next = stack->top;
    stack->top = newNode;
    stack->size++;
}
void removeWordFromFile(const char *fileName, const char *wordToRemove, undoStack *undo, redoStack *redo) {
    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    // Open a temporary file for writing
    FILE *tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL) {
        printf("Error creating temp file.\n");
        fclose(file);
        return;
    }

    char line[max];
    int startIndex = -1;
    while (fgets(line, sizeof(line), file) != NULL) {
        char *word = strtok(line, " \n");
        while (word != NULL) {
            if (strcmp(word, wordToRemove) != 0) {
                if (ftell(tempFile) != 0) {
                    fprintf(tempFile, " ");
                }
                fprintf(tempFile, "%s", word);
            } else {
                if (startIndex == -1) {
                    startIndex = ftell(tempFile) - strlen(word) - 1;
                }
                addToUndoStack(undo, wordToRemove, "remove", startIndex);
                addToRedoStack(redo, wordToRemove);
            }
            word = strtok(NULL, " \n");
        }
    }

    fclose(file);
    fclose(tempFile);

    if (remove(fileName) != 0) {
        printf("Error removing original file.\n");
        return;
    }

    if (rename("temp.txt", fileName) != 0) {
        printf("Error updating file.\n");
        return;
    }
}

void undoOperation(undoStack *undo, redoStack *redo) {
    if (undo->top == NULL) {
        printf("Undo stack is empty.\n");
        return;
    }

    Unode *topUndo = undo->top;
    FILE *file = fopen("initial.txt", "r+");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    fseek(file, topUndo->index, SEEK_SET);

    fprintf(file, "%s ", topUndo->ustr);

    fclose(file);

    undo->top = undo->top->next;
    free(topUndo);

    addToRedoStack(redo, topUndo->ustr);
}


void redoOperation(undoStack *undo, redoStack *redo) {
    if (redo->top == NULL) {
        printf("Redo stack is empty.\n");
        return;
    }

    Rnode *topRedo = redo->top;
    addToUndoStack(undo, topRedo->rstr, "Insert", -1);
    removeWordFromFile("initial.txt", topRedo->rstr, undo, redo);
}

void writeToFile(const char *inputFileName, const char *outputFileName) {
    FILE *inputFile = fopen(inputFileName, "r");
    if (inputFile == NULL) {
        printf("Error opening input file.\n");
        return;
    }

    FILE *outputFile = fopen(outputFileName, "w");
    if (outputFile == NULL) {
        printf("Error opening output file.\n");
        fclose(inputFile);
        return;
    }

    char line[max];
    while (fgets(line, sizeof(line), inputFile) != NULL) {
        fputs(line, outputFile);
    }
    fclose(inputFile);
    fclose(outputFile);
}




