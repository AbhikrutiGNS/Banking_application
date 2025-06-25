#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct transaction {
    float amount;
    char type[10]; 
    int day;       
};

struct account {
    char name[100];
    int acc_id;
    float cur_bal;
    struct transaction transactions[32]; 
};

int acc_id_counter = 1000; 
struct account *ac[100];

void create_account(struct account **, int);
void dep_wit(struct account **, int);
void display_transactions(struct account**, int, int);
void deposit_transactions(struct account**, int, int);
void wit_transactions(struct account**, int, int);
void savetofile(struct account*, const char*);
void inputfromfile(struct account*, const char*);

void create_account(struct account **ac, int i) {
    ac[i] = (struct account *)malloc(sizeof(struct account));
    printf("\nEnter your Name Below\n");
    scanf(" %[^\n]s", ac[i]->name);
    getchar(); 
    ac[i]->acc_id = acc_id_counter++;
    printf("\nYour account id is as follows\n");
    printf("%d\n", ac[i]->acc_id);
    printf("\nAdd a minimum amount to your account for account creation\n");
    scanf("%f", &ac[i]->cur_bal);

    for (int j = 1; j <= 31; j++) {
        ac[i]->transactions[j].amount = 0.0;
        strcpy(ac[i]->transactions[j].type, "None");
        ac[i]->transactions[j].day = j;
    }

    printf("\nYour account details are as follows \n");
    printf("\nName: %s\n", ac[i]->name);
    printf("\nAccount ID: %d\n", ac[i]->acc_id);
    printf("\nCurrent Balance: %.2f\n", ac[i]->cur_bal);
    printf("\nBranch: UB1, PESIT\n");
    printf("\nRate of Interest: 6.5 percent\n");

    
    ac[i]->transactions[1].amount = ac[i]->cur_bal;
    strcpy(ac[i]->transactions[1].type, "Deposit");

    savetofile(ac[i], "transactions.txt");
}

void dep_wit(struct account **ac, int num_accounts) {
    int acc_id, date, choice;
    printf("\nEnter your account id: ");
    scanf("%d", &acc_id);
    getchar(); 
    printf("\nEnter date: ");
    scanf("%d", &date);

    printf("\nPress 1 to deposit\nPress 2 to withdraw\n");
    scanf("%d", &choice);

    int index = -1;
    for (int j = 0; j < num_accounts; j++) {
        if (ac[j]->acc_id == acc_id) {
            index = j;
            break;
        }
    }

    if (index == -1) {
        printf("\nAccount not found!\n");
        return;
    }

    inputfromfile(ac[index], "transactions.txt");

    switch (choice) {
        case 1: { 
            float amount;
            printf("\nEnter the amount to deposit: ");
            scanf("%f", &amount);
            ac[index]->cur_bal += amount;

            ac[index]->transactions[date].amount = amount;
            strcpy(ac[index]->transactions[date].type, "Deposit");

            printf("\nDeposit successful!\n");
            printf("\nCurrent balance = %.2f\n", ac[index]->cur_bal);
            break;
}
        case 2: { 
            float amount;
            printf("\nEnter the amount to withdraw: ");
            scanf("%f", &amount);
            if (amount > ac[index]->cur_bal) {
                printf("\nInsufficient balance!\n");
            } 
            else {
                ac[index]->cur_bal -= amount;

                ac[index]->transactions[date].amount = -amount;
                strcpy(ac[index]->transactions[date].type, "Withdrawal");

                printf("\nWithdrawal successful!\n");
                printf("\nCurrent balance = %.2f\n", ac[index]->cur_bal);
            }
            break;
        }
        default: {
            printf("\nInvalid choice!\n");
            break;
        }
    }

    savetofile(ac[index], "transactions.txt");
}

void display_transactions(struct account **ac, int num_accounts, int acc_id) {
    int index = -1;

    for (int i = 0; i < num_accounts; i++) {
        if (ac[i]->acc_id == acc_id) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("\nAccount not found!\n");
        return;
    }

    inputfromfile(ac[index], "transactions.txt");

    printf("\nTransactions for account ID %d:\n", acc_id);
    printf("Day            Type        Amount         Balance\n");

    float balance = 0.0;
    for (int day = 1; day <= 31; day++) {
        if (strcmp(ac[index]->transactions[day].type, "None") != 0) {
            balance += ac[index]->transactions[day].amount;
            printf("%d          %s          %.2f            %.2f\n", day, ac[index]->transactions[day].type, ac[index]->transactions[day].amount, balance);
        }
    }
}

void savetofile(struct account *acc, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    for (int day = 1; day <= 31; day++) {
        if (strcmp(acc->transactions[day].type, "None") != 0) {
            fprintf(file, "%d %d %s %.2f %.2f\n", acc->acc_id, day, acc->transactions[day].type, acc->transactions[day].amount, acc->cur_bal);
        }
    }

    fclose(file);
}

void inputfromfile(struct account *acc, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    int acc_id, day;
    char type[10];
    float amount, balance;

    while (fscanf(file, "%d %d %s %f %f", &acc_id, &day, type, &amount, &balance) == 5) {
        if (acc_id == acc->acc_id) {

            if (strcmp(type, "Withdrawal") == 0) {
                acc->transactions[day].amount = -amount;
            } 
            else {
                acc->transactions[day].amount = amount;
            }

            strcpy(acc->transactions[day].type, type);
            acc->transactions[day].day = day;
            acc->cur_bal = balance;
        }
    }

    fclose(file);
}


void deposit_transactions(struct account **ac, int num_accounts, int acc_id) {
    int index = -1;

    for (int i = 0; i < num_accounts; i++) {
        if (ac[i]->acc_id == acc_id) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("\nAccount not found!\n");
        return;
    }

    inputfromfile(ac[index], "transactions.txt");

    printf("\nDeposit Transactions for account ID %d:\n", acc_id);
    printf("Day     Amount  Balance\n");

    float balance = 0.0;
    for (int day = 1; day <= 31; day++) {
        if (strcmp(ac[index]->transactions[day].type, "Deposit") == 0) {
            balance += ac[index]->transactions[day].amount;
            printf("%d\t%.2f\t%.2f\n", day, ac[index]->transactions[day].amount, balance);
        }
    }
}
void wit_transactions(struct account **ac, int num_accounts, int acc_id) {
    int index = -1;

    for (int i = 0; i < num_accounts; i++) {
        if (ac[i]->acc_id == acc_id) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("\nAccount not found!\n");
        return;
    }

    inputfromfile(ac[index], "transactions.txt");

    printf("\nWithdrawal Transactions for account ID %d:\n", acc_id);
    printf("Day\tAmount\tBalance\n");

    float balance = ac[index]->cur_bal;
    for (int day = 1; day <= 31; day++) {
        if (strcmp(ac[index]->transactions[day].type, "Withdrawal") == 0) {
            balance -= ac[index]->transactions[day].amount; 
            printf("%d\t%.2f\t%.2f\n", day, -ac[index]->transactions[day].amount, balance);
        }
    }
}


int main() {
    int i = 0; 
    int choice = 0; 
    int acc_id;
    while (choice != 6) {
        printf("\nWELCOME TO 'THE BANK!'\n");
        printf("Choose from the Below menu!\n");
        printf("1. ACCOUNT CREATION\n");
        printf("2. DEPOSIT AND WITHDRAWAL\n");
        printf("3. CHECK ALL TRANSACTIONS\n");
        printf("4. CHECK DEPOSIT TRANSACTIONS\n");
        printf("5. CHECK WITHDRAWAL TRANSACTIONS\n");
        printf("6. EXIT\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); 

        switch (choice) {
            case 1:
                printf("\nYou have chosen to create an account\n");
                if (i < 100) 
                {
                    create_account(ac, i);
                    i++; 
                } 
                else 
                {
                    printf("\nMaximum number of accounts reached!\n");
                }
                break;
            case 2:
                printf("\nYou have chosen DEPOSIT AND WITHDRAWAL\n");
                dep_wit(ac, i);
                break;
            case 3:
                printf("\nYou have chosen TO CHECK ALL TRANSACTIONS\n");
                printf("\nEnter your account ID: ");
                scanf("%d", &acc_id);
                getchar();
                display_transactions(ac, i, acc_id);
                break;
            case 4:
                printf("\nYou have chosen TO CHECK DEPOSIT TRANSACTIONS\n");
                printf("\nEnter your account ID: ");
                scanf("%d", &acc_id);
                getchar();
                deposit_transactions(ac, i, acc_id);
                break;
            case 5:
                printf("\nYou have chosen TO CHECK WITHDRAWAL TRANSACTIONS\n");
                printf("\nEnter your account ID: ");
                scanf("%d", &acc_id);
                getchar();
                wit_transactions(ac, i, acc_id);
               break;
            case 6:
                printf("\nTHANK YOU FOR VISITING THE BANK!\n");
                break;
            default:
                printf("\nInvalid option\n");
                break;
        }
    }

    for (int j = 0; j < i; j++) {
        free(ac[j]);
    }

    return 0;
}

