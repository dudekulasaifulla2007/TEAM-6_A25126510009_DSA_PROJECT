#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char document[1000] = "";

typedef struct Node {
    char operation[10];
    char text[100];
    struct Node *next;
} Node;

typedef struct {
    Node *top;
} Stack;

void initStack(Stack *s) {
    s->top = NULL;
}

void push(Stack *s, char operation[], char text[]) {

    Node *newNode = malloc(sizeof(Node));

    strcpy(newNode->operation, operation);
    strcpy(newNode->text, text);

    newNode->next = s->top;
    s->top = newNode;
}

Node* pop(Stack *s) {

    if (s->top == NULL) {
        return NULL;
    }

    Node *temp = s->top;
    s->top = s->top->next;

    return temp;
}

int isEmpty(Stack *s) {
    return s->top == NULL;
}

void insertText(char document[], Stack *undo, Stack *redo) {

    char text[100];

    printf("Enter text to insert: "); 
    scanf(" %[^\n]", text);

    strcat(document, text);

    push(undo, "INSERT", text);

    clearStack(redo);
}

void deleteText(char document[], Stack *undo, Stack *redo) {

    int n;

    printf("How many characters to delete? ");
    scanf("%d", &n);

    int len = strlen(document);

    if (n > len) {
        printf("Cannot delete %d characters.\n", n);
        return;
    }

    char deleted[100];

    strcpy(deleted, document + len - n);

    document[len - n] = '\0';

    push(undo, "DELETE", deleted);

    clearStack(redo);
}

void clearStack(Stack *s) {

    while (s->top != NULL) {

        Node *temp = s->top;
        s->top = s->top->next;

        free(temp);
    }
}

void undo(char document[], Stack *undo, Stack *redo) {

    Node *action = pop(undo);

    if (action == NULL) {
        printf("Nothing to undo.\n");
        return;
    }

    if (strcmp(action->operation, "INSERT") == 0) {

        int len = strlen(document);
        int n = strlen(action->text);

        document[len - n] = '\0';
    }

    else if (strcmp(action->operation, "DELETE") == 0) {

        strcat(document, action->text);
    }

    push(redo, action->operation, action->text);

    free(action);
}

void redo(char document[], Stack *undo, Stack *redo) {

    Node *action = pop(redo);

    if (action == NULL) {
        printf("Nothing to redo.\n");
        return;
    }

    if (strcmp(action->operation, "INSERT") == 0) {

        strcat(document, action->text);
    }

    else if (strcmp(action->operation, "DELETE") == 0) {

        int len = strlen(document);
        int n = strlen(action->text);

        document[len - n] = '\0';
    }

    push(undo, action->operation, action->text);

    free(action);
}

void displayStack(Stack *s) {

    Node *temp = s->top;

    if (temp == NULL) {
        printf("EMPTY\n");
        return;
    }

    while (temp != NULL) {

        printf("%s \"%s\"\n",
               temp->operation,
               temp->text);

        temp = temp->next;
    }
}

int main() {

    char document[1000] = "";

    Stack undoStack;
    Stack redoStack;

    initStack(&undoStack);
    initStack(&redoStack);

    int choice;

    while (1) {

        printf("\n====================================\n");
        printf("          UNDO - REDO SYSTEM\n");
        printf("====================================\n");

        printf("\nCurrent Document: %s\n", document);

        printf("\n1. Insert Text");
        printf("\n2. Delete Characters");
        printf("\n3. Undo");
        printf("\n4. Redo");
        printf("\n5. Exit");

        printf("\n\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice) {

            case 1:
                insertText(document, &undoStack, &redoStack);
                break;

            case 2:
                deleteText(document, &undoStack, &redoStack);
                break;

            case 3:
                undo(document, &undoStack, &redoStack);
                break;

            case 4:
                redo(document, &undoStack, &redoStack);
                break;

            case 5:
                clearStack(&undoStack);
                clearStack(&redoStack);

                printf("\nProgram terminated.\n");
                return 0;

            default:
                printf("\nInvalid choice. Please try again.\n");
        }
    }
}