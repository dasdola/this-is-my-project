#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ADMIN_PASS "12345"

// User section
struct User
{
    char username[20];
    char password[20];
    char Fullname[30];
    char phone[20];
    char nid[20];
    double cash_in;
    double cash_out;
    long double netbalance;
};

// Registration
void registerUser()
{
    FILE *file;
    struct User newUser;

    printf("Enter username: ");
    scanf("%s", newUser.username);
    printf("Enter password: ");
    scanf("%s", newUser.password);
    printf("Enter Full name: ");
    scanf("%s", newUser.Fullname);
    printf("Enter phone number: ");
    scanf("%s", newUser.phone);
    printf("Enter NID: ");
    scanf("%s", newUser.nid);

    newUser.cash_in = 0;
    newUser.cash_out = 0;
    newUser.netbalance = 0;

    file = fopen("users.txt", "a");
    if (file == NULL)
    {
        printf("Error opening file for registration.\n");
        return;
    }

    fprintf(file, "%s %s %s %s %s %.2lf %.2lf %.2Lf\n", newUser.username, newUser.password, newUser.Fullname,
            newUser.phone, newUser.nid, newUser.cash_in, newUser.cash_out, newUser.netbalance);
    fclose(file);
    printf("Registration successful.\n");
}

// Login section
int loginUser()
{
    FILE *file;
    char username[20];
    char password[20];

    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);

    file = fopen("users.txt", "r");
    if (file == NULL)
    {
        printf("Error opening file for login.\n");
        return 0;
    }

    struct User newUser;
    int loggedIn = 0;

    while (fscanf(file, "%s %s %s %s %s %lf %lf %Lf\n", newUser.username, newUser.password, newUser.Fullname, newUser.phone, newUser.nid, &newUser.cash_in, &newUser.cash_out, &newUser.netbalance) != EOF)
    {
        if (strcmp(username, newUser.username) == 0 && strcmp(password, newUser.password) == 0)
        {
            printf("Login successful. Welcome, %s!\n", newUser.Fullname);
            printf("username:%s  \t pass:%s \t full name:%s \t phone:%s \n\n nid:%s \t cash in:%.2lf \t cash out:%.2lf \t balance:%.2lf \n", newUser.username, newUser.password, newUser.Fullname, newUser.phone, newUser.nid, newUser.cash_in, newUser.cash_out, newUser.netbalance);
            loggedIn = 1;
            break;
        }
    }

    fclose(file);

    if (!loggedIn)
    {
        printf("Login failed. Incorrect username or password.\n");
    }

    return loggedIn;
}

// Change password
void changePassword()
{
    FILE *file;
    struct User user;

    char username[50];
    char oldPassword[50];
    char newPassword[50];

    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter old password: ");
    scanf("%s", oldPassword);
    printf("Enter new password: ");
    scanf("%s", newPassword);

    file = fopen("users.txt", "r+");
    if (file == NULL)
    {
        printf("Error opening file for changing password.\n");
        return;
    }

    int found = 0;
    while (fscanf(file, "%s %s %s %s %s %lf %lf %Lf\n", user.username, user.password, user.Fullname, user.phone, user.nid, &user.cash_in, &user.cash_out, &user.netbalance) != EOF)
    {
        if (strcmp(username, user.username) == 0 && strcmp(oldPassword, user.password) == 0)
        {
            fseek(file, -strlen(user.username) - strlen(user.password) - 2, SEEK_CUR);
            fprintf(file, "%s %s %s %s %s %.2lf %.2lf %.2Lf\n", user.username, newPassword, user.Fullname, user.phone, user.nid, user.cash_in, user.cash_out, user.netbalance);
            printf("Password changed successfully.\n");
            found = 1;
            break;
        }
    }

    if (!found)
    {
        printf("Password change failed. Incorrect username or password.\n");
    }

    fclose(file);
}

// Upgrade balance
void upgrade_balance()
{
    struct User newUser;
    char given_nid[20];
    printf("Search NID no: ");
    scanf("%s", given_nid);
    FILE *file = fopen("users.txt", "r+");
    FILE *tempFile = fopen("temp_users.txt", "w+");

    if (file == NULL || tempFile == NULL)
    {
        printf("Error opening file for upgrade balance.\n");
        return;
    }

    int found = 0;
    while (fscanf(file, "%s %s %s %s %s %lf %lf %Lf\n", newUser.username, newUser.password, newUser.Fullname, newUser.phone, newUser.nid, &newUser.cash_in, &newUser.cash_out, &newUser.netbalance) != EOF)
    {
        if (strcmp(given_nid, newUser.nid) == 0)
        {
            double new_cash_in, new_cash_out, new_netbalance;
            printf("Enter the new cash_in: ");
            scanf("%lf", &new_cash_in);
            printf("Enter the new cash_out: ");
            scanf("%lf", &new_cash_out);
            printf("Enter the new net balance: ");
            scanf("%Lf", &new_netbalance);

            newUser.cash_in = new_cash_in;
            newUser.cash_out = new_cash_out;
            newUser.netbalance = new_netbalance;

            fprintf(tempFile, "%s %s %s %s %s %.2lf %.2lf %.2Lf\n", newUser.username, newUser.password, newUser.Fullname, newUser.phone, newUser.nid, newUser.cash_in, newUser.cash_out, newUser.netbalance);
            printf("User data updated successfully.\n");

            // Print the updated user details
            printf("Updated User Details:\n");
            printf("username:%s  \t pass:%s \t full name:%s \t phone:%s \n\n nid:%s \t cash in:%.2lf \t cash out:%.2lf \t balance:%.2lf \n", newUser.username, newUser.password, newUser.Fullname, newUser.phone, newUser.nid, newUser.cash_in, newUser.cash_out, newUser.netbalance);
            found = 1;
        }
        else
        {
            fprintf(tempFile, "%s %s %s %s %s %.2lf %.2lf %.2Lf\n", newUser.username, newUser.password, newUser.Fullname, newUser.phone, newUser.nid, newUser.cash_in, newUser.cash_out, newUser.netbalance);
        }
    }

    fclose(file);
    fclose(tempFile);

    remove("users.txt");
    rename("temp_users.txt", "users.txt");

    if (!found)
    {
        printf("User not found with the given NID.\n");
    }
}

// Verification panel
void verification_panel()
{
    char input[20];

    printf("Enter Admin Password: ");
    scanf("%s", input);
    if (strcmp(input, ADMIN_PASS) == 0)
    {
        upgrade_balance();
    }
    else
    {
        printf("Invalid Admin Password\n");
    }
}

int main()
{
    int choice;
    int loggedIn = 0;

    while (1)
    {
        printf("1. Register\n2. Login\n3. Change Password\n4. Admin Panel\n5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            registerUser();
            break;
        case 2:
            loggedIn = loginUser();
            break;
        case 3:
            if (loggedIn)
            {
                changePassword();
            }
            else
            {
                printf("You must be logged in to change the password.\n");
            }
            break;
        case 4:
            verification_panel();
            break;
        case 5:
            return 0;
        default:
            printf("Invalid choice. Try again.\n");
        }
    }
    return 0;
}
