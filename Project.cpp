#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structures
struct Item {
    char name[30];
    int price;
};

struct SelectedItem {
    struct Item item;
    int quantity;
};

// Function Declarations
void showWelcome();
void getCustomerNameGraphic(char *name);
void showMenu(struct Item items[], int itemCount);
void showSelected(struct SelectedItem selected[], int count);
void selectItem(struct Item items[], int itemCount, struct SelectedItem selected[], int *selectedCount);
int showBill(struct SelectedItem selected[], int count);
void saveBillToFile(char *customerName, struct SelectedItem selected[], int count);

// Global Variables
char customerName[50];
FILE *file;

int main() {
    int gd = DETECT, gm;
    initwindow(1200, 800, "Sports Mart");

    struct Item items[10] = {
        {"Football", 500},
        {"Basketball", 600},
        {"Tennis Racket", 1200},
        {"Cricket Bat", 1500},
        {"Hockey Stick", 800},
        {"Badminton", 400},
        {"Volleyball", 550},
        {"Skateboard", 1300},
        {"Baseball Bat", 1000},
        {"Golf Stick", 2000}
    };

    struct SelectedItem selected[20];
    int selectedCount = 0;
    int action = 0;

    showWelcome();
    getch();
    cleardevice();

    // Get customer name
    getCustomerNameGraphic(customerName);

    // Open file and save name
    file = fopen("mart.txt", "a");
    fprintf(file, "\nCustomer Name: %s\n", customerName);
    fclose(file);

    do {
        cleardevice();
        showMenu(items, 10);
        selectItem(items, 10, selected, &selectedCount);

        if (selectedCount == 0) {
            setbkcolor(LIGHTBLUE);
            cleardevice();
            setcolor(RED);
            settextstyle(3, HORIZ_DIR, 4);
            outtextxy(300, 300,(char*) "No items selected! Exiting...");
            delay(1500);
            closegraph();
            exit(0);
        }

        cleardevice();
        action = showBill(selected, selectedCount);

        if (action == 2) { // Edit
            selectedCount = 0; // Restart selection
        } else if (action == 0) { // Cancel
            setbkcolor(LIGHTBLUE);
            cleardevice();
            setcolor(RED);
            settextstyle(3, HORIZ_DIR, 4);
            outtextxy(300, 300,(char*) "Order Cancelled.");
            delay(1500);
            closegraph();
            exit(0);
        } else if (action == 1) { // Pay
            saveBillToFile(customerName, selected, selectedCount);
            setbkcolor(LIGHTBLUE);
            cleardevice();
            setcolor(RED);
            settextstyle(3, HORIZ_DIR, 4);
            outtextxy(300, 300,(char*) "Payment Successful.");
            delay(2000);
            break;
        }

    } while (1);

    getch();
    closegraph();
    return 0;
}

// Functions

void showWelcome() {
    setbkcolor(BLACK);
    cleardevice();
    setcolor(WHITE);
    settextstyle(3, HORIZ_DIR, 5);
    outtextxy(200, 300,(char*) "WELCOME TO SPORTS MART");
}

void getCustomerNameGraphic(char *name) {
    int index = 0;
    char ch;
    setbkcolor(LIGHTMAGENTA);
    cleardevice();
    setcolor(BLUE);
    settextstyle(3, HORIZ_DIR, 4);
    outtextxy(100, 100,(char*) "Enter Customer Name:");
    
    // Box for typing
    rectangle(100, 200, 800, 250);
    setcolor(RED);
    settextstyle(3, HORIZ_DIR, 3);

    while (1) {
        ch = getch();
        if (ch == 13) { // Enter
            name[index] = '\0';
            break;
        }
        else if (ch == 8) { // Backspace
            if (index > 0) {
                index--;
                name[index] = '\0';
                setfillstyle(SOLID_FILL, LIGHTMAGENTA);
                bar(105, 205, 795, 245);
                outtextxy(110, 210, name);
            }
        }
        else if (index < 49) {
            name[index++] = ch;
            name[index] = '\0';
            setfillstyle(SOLID_FILL, LIGHTMAGENTA);
            bar(105, 205, 795, 245);
            outtextxy(110, 210, name);
        }
    }
}

void showMenu(struct Item items[], int itemCount) {
    setbkcolor(LIGHTGREEN);
    cleardevice();
    settextstyle(3, HORIZ_DIR, 4);
    setcolor(BROWN);
    outtextxy(50, 30,(char*) "Select Sports Items:");

    settextstyle(3, HORIZ_DIR, 2);
    for (int i = 0; i < itemCount; i++) {
        rectangle(50, 100 + i*60, 350, 150 + i*60);
        outtextxy(70, 110 + i*60, items[i].name);
        char price[20];
        sprintf(price, "Rs %d", items[i].price);
        outtextxy(250, 110 + i*60, price);
    }

    setcolor(RED);
    outtextxy(375, 700,(char*) "Click to select, Press ENTER to finish");
}

void showSelected(struct SelectedItem selected[], int count) {
    int x = 700, y = 100;
    settextstyle(3, HORIZ_DIR, 4);
    setcolor(BROWN);
    outtextxy(650, 30,(char*) "Selected Items:");
    setfillstyle(SOLID_FILL, LIGHTGREEN);
    bar(650, 90, 1100, 700);
    setcolor(BLUE);
    settextstyle(3, HORIZ_DIR, 2);
    for (int i = 0; i < count; i++) {
        char buffer[100];
        sprintf(buffer, "%s : %d", selected[i].item.name, selected[i].quantity);
        outtextxy(x, y + i*40, buffer);
    }
}

void selectItem(struct Item items[], int itemCount, struct SelectedItem selected[], int *selectedCount) {
    int x, y;
    int done = 0;

    while (!done) {
        if (ismouseclick(WM_LBUTTONDOWN)) {
            getmouseclick(WM_LBUTTONDOWN, x, y);

            for (int i = 0; i < itemCount; i++) {
                if (x > 50 && x < 350 && y > 100 + i*60 && y < 150 + i*60) {
                    // Check if already selected
                    int found = 0;
                    for (int j = 0; j < *selectedCount; j++) {
                        if (strcmp(selected[j].item.name, items[i].name) == 0) {
                            selected[j].quantity++;
                            found = 1;
                            break;
                        }
                    }
                    if (!found) {
                        selected[*selectedCount].item = items[i];
                        selected[*selectedCount].quantity = 1;
                        (*selectedCount)++;
                    }
                    showMenu(items, itemCount);
                    showSelected(selected, *selectedCount);
                }
            }
        }

        if (kbhit()) {
            char ch = getch();
            if (ch == 13) { // Enter key
                done = 1;
            }
        }
    }
}

int showBill(struct SelectedItem selected[], int count) {
    int total = 0;
    char buffer[100];
    int x, y;

    setbkcolor(WHITE);
    cleardevice();
    settextstyle(3, HORIZ_DIR, 3);
    setcolor(BLACK);
    outtextxy(100, 30,(char*) "FINAL BILL:");

    // Set headings
    settextstyle(3, HORIZ_DIR, 2);
    outtextxy(100, 80,(char*) "Item Name");
    outtextxy(400, 80, (char*)"Quantity");
    outtextxy(600, 80,(char*) "Base Price");
    outtextxy(850, 80, (char*)"Total Price");

    int startY = 120;
    for (int i = 0; i < count; i++) {
        outtextxy(100, startY + i*40, selected[i].item.name);

        sprintf(buffer, "%d", selected[i].quantity);
        outtextxy(400, startY + i*40, buffer);

        sprintf(buffer, "Rs %d", selected[i].item.price);
        outtextxy(600, startY + i*40, buffer);

        int itemTotal = selected[i].item.price * selected[i].quantity;
        sprintf(buffer, "Rs %d", itemTotal);
        outtextxy(850, startY + i*40, buffer);

        total += itemTotal;
    }

    setcolor(RED);
    settextstyle(3, HORIZ_DIR, 3);
    sprintf(buffer, "TOTAL AMOUNT: Rs %d", total);
    outtextxy(100, startY + count * 40 + 40, buffer);

    // Buttons
    setcolor(BLACK);
    setfillstyle(SOLID_FILL, LIGHTBLUE);
    bar(700, startY + count * 40 + 100, 950, startY + count * 40 + 150);
    outtextxy(770, startY + count * 40 + 110,(char*) "EDIT");

    setfillstyle(SOLID_FILL, LIGHTRED);
    bar(700, startY + count * 40 + 170, 950, startY + count * 40 + 220);
    outtextxy(760, startY + count * 40 + 180,(char*) "CANCEL");

    setfillstyle(SOLID_FILL, LIGHTGREEN);
    bar(700, startY + count * 40 + 240, 950, startY + count * 40 + 290);
    outtextxy(780, startY + count * 40 + 250,(char*) "PAY");

    while (1) {
        if (ismouseclick(WM_LBUTTONDOWN)) {
            getmouseclick(WM_LBUTTONDOWN, x, y);
            if (x > 700 && x < 950) {
                if (y > startY + count * 40 + 100 && y < startY + count * 40 + 150) return 2; // Edit
                else if (y > startY + count * 40 + 170 && y < startY + count * 40 + 220) return 0; // Cancel
                else if (y > startY + count * 40 + 240 && y < startY + count * 40 + 290) return 1; // Pay
            }
        }
    }
}

void saveBillToFile(char *customerName, struct SelectedItem selected[], int count) {
    int total = 0;

    file = fopen("mart.txt", "a");
    fprintf(file, "---- Bill ----\n");
    for (int i = 0; i < count; i++) {
        fprintf(file, "%s x%d = Rs %d\n", selected[i].item.name, selected[i].quantity, selected[i].item.price * selected[i].quantity);
        total += selected[i].item.price * selected[i].quantity;
    }
    fprintf(file, "Total = Rs %d\n", total);
    fprintf(file, "--------------------\n");
    fclose(file);
}
