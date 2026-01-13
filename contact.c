#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "contact.h"
#include "file.h"
#include<ctype.h>

void listContacts(AddressBook *addressBook) 
{
    printf("\n");
    printf("%-10s %-13s %-22s %s\n", "Sl.No", "Name", "Phone No", "Email ID");
    
    for(int i=0;i<addressBook->contactCount;i++)
    {
        printf("%-8d",i+1);
        printf("%-15s %-19s %s\n",addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email);
        
    }
    printf("\n");
    // Display total contact count
    printf("Total contacts: %d\n", addressBook->contactCount);
}

void initialize(AddressBook *addressBook)
{
   addressBook->contactCount = 0;

    FILE *file = fopen("contact.txt", "r");
    if (!file)
    {
        printf("No existing contacts found. Starting fresh.\n");
        return;
    }

    // Read each contact from the file using a for loop
    for (int i = 0; i < MAX_CONTACTS; i++)
    {
        if (fscanf(file, "%[^,],%[^,],%[^\n]\n",addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email) == 3)
        {
            addressBook->contactCount++;
        }
        else
        {
            break; 
        }
    }

   fclose(file);
}

void saveAndExit(AddressBook *addressBook) {
    saveContactsToFile(addressBook); // Save contacts to file
    exit(EXIT_SUCCESS); // Exit the program
}


void createContact(AddressBook *addressBook)
{
	/* Define the logic to create a Contacts */
    if(addressBook->contactCount>=MAX_CONTACTS)
    {
        printf("AddressBook is full!\n");
        return; 
    }
    // -----------------NAME VALIDATION------------------
    printf("\n");

    char name[50];
    printf("Enter Name:");
    scanf(" %[^\n]s",name);
    if(!isalpha(name[0]))
    {
        printf("Name must start with an alphabet");
    }
    strcpy(addressBook->contacts[addressBook->contactCount].name, name);
    
   // ---------- ----PHONE NUMBER VALIDATION ----------
    char number[20];
    printf("Enter Phone Number:");
    scanf(" %[^\n]s",number);
    //Check length
    if (strlen(number) != 10) 
    {
        printf("Phone number must be exactly 10 digits!\n");
        return;
    }
    // Check digits only
    for(int i=0;i<10;i++)
    {
        if(!isdigit(number[i]))
        {
            printf("Phone number must contain only digits!\n");
            return;
        }
    }
    //Check uniqueness
    for (int i = 0; i < addressBook->contactCount; i++)
    {
        if (strcmp(addressBook->contacts[i].phone, number) == 0) 
        {
            printf("This phone number already exists in contacts!\n");
            return;
        }
    }
    strcpy(addressBook->contacts[addressBook->contactCount].phone, number);

 // -----------------EMAIL VALIDATION ----------------
   char email[50];
    int valid = 0;

    while (!valid) 
    {
        printf("Enter Email ID: ");
        scanf(" %s", email);

        valid = 1; // Assume valid, check for issues below

        // 1. Check for uppercase letters
        for (int i = 0; email[i] != '\0'; i++) {
            if (isupper(email[i])) {
                printf("Email must be lowercase only.\n");
                valid = 0;
                break;
            }
        }
        if (!valid) 
        {
            continue;
        }

        // 2. Check for exactly one '@'
        int atCount = 0;
        for (int i = 0; email[i] != '\0'; i++)
        {
            if (email[i] == '@') {
                atCount++;
            }
        }
        if (atCount != 1) 
        {
            printf("Email must contain exactly one '@'.\n");
            valid = 0;
            continue;
        }
    
    //Must end with ".com"
    int len = strlen(email);
    if (len < 5 || strcmp(email + len - 4, ".com") != 0)
    {
        printf("email must end with '.com'\n");
        continue;
    }
    //Check uniqueness
    for (int i = 0; i < addressBook->contactCount; i++)
    {
        if (strcmp(addressBook->contacts[i].email,email) == 0) 
        {
            printf("This email  already exists in contacts!\n");
            return;
        }
    }
}
   strcpy(addressBook->contacts[addressBook->contactCount].email, email);

   addressBook->contactCount++;
   printf("Contact added successfully!\n");

}
void searchContact(AddressBook *addressBook) 
{
    int choice;
    do
    {
        printf("Search by: \n");
        printf("1.Name\n");
        printf("2.Phone No\n");
        printf("3.Email ID\n");
        printf("4.Exit\n");
        printf("Enter your choice:");
        scanf("%d",&choice);
        switch (choice)
        {
        case 1:
        {
           char names[50];
           int found=0;
           int match[MAX_CONTACTS];
           int matchcount=0;
           int choice;
           // clear input buffer or clear leftover newline after previous scanf
           int c;
           while ((c = getchar()) != '\n' && c != EOF);
           printf("Enter a name to search:");
           fgets(names, sizeof(names), stdin);
           names[strcspn(names, "\n")] = '\0';
           //Search for Name
           for (int i = 0; i < addressBook->contactCount; i++) 
           {
                    if (strcmp(addressBook->contacts[i].name,names) == 0) 
                    {
                        found = 1;
                        match[matchcount] = i;  
                        matchcount++;
                    }
            }

            if (!found)
            {
                printf("No contact found with the name: %s\n", names);
                return;
                   
            }
            if (matchcount == 1) 
            {
            // Only one match - no need to ask user to select
            int selectedIndex = match[0];
            Contact selected = addressBook->contacts[selectedIndex];
            printf("Contact Found:\n");
            printf("Name: %s  Number: %s  Email ID: %s\n", selected.name, selected.phone, selected.email);
            } 
            //more than 2 similar names
            else
            {
                printf("Multiple contacts found:\n");
                for (int i = 0; i < matchcount; i++) 
                {
                    int idx = match[i];
                    printf("%d. Name: %s  Number: %s  Email ID: %s\n",i + 1, addressBook->contacts[idx].name,addressBook->contacts[idx].phone,addressBook->contacts[idx].email);
                }

                 printf("Select a contact by entering a number (1 to %d): ", matchcount);
                 scanf("%d", &choice);

                 if (choice >= 1 && choice <= matchcount) 
                 {    
                    int selectedIndex = match[choice - 1];
                    Contact selected = addressBook->contacts[selectedIndex];
                    printf("\nYou selected:\n");
                    printf("Name: %s  Number: %s  Email ID: %s\n",selected.name, selected.phone, selected.email);
                } 
                else 
                {
                    printf("Invalid selection.\n");
                }
            }
           
             break;   
        }
         
        case 2:
        {
           char phoneNo[20];
           int found=0;

           // clear input buffer or clear leftover newline after previous scanf
           int c;
           while ((c = getchar()) != '\n' && c != EOF);
           printf("Enter a Phone Number to search:");
           fgets(phoneNo, sizeof(phoneNo), stdin);
           phoneNo[strcspn(phoneNo, "\n")] = '\0';
           //Check length
           if (strlen(phoneNo) != 10)
           {
                printf("Phone number must be exactly 10 digits!\n");
                return;
           }
           //Search phoneNo
           for (int i = 0; i < addressBook->contactCount; i++) 
           {
                    if (strcmp(addressBook->contacts[i].phone,phoneNo) == 0) 
                    {
                        printf("Contact Found:\n");
                        printf("Name:%s  Number:%s  EmailID:%s\n",addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email);
                        found = 1;
                        break; 
                    }
            }

                if (!found)
                {
                    printf("No contact found with the Phone Number: %s\n", phoneNo);
                   
                }
             break;   
        }
         
        case 3:
        {
           char emailID[20];
           int found=0;

           // clear input buffer or clear leftover newline after previous scanf
           int c;
           while ((c = getchar()) != '\n' && c != EOF);
           printf("Enter a name to search:");
           fgets(emailID, sizeof(emailID), stdin);
           emailID[strcspn(emailID, "\n")] = '\0';
           //Search Email ID
           for (int i = 0; i < addressBook->contactCount; i++) 
           {
                    if (strcmp(addressBook->contacts[i].email,emailID) == 0) 
                    {
                        printf("Contact Found:\n");
                        printf("Name:%s  Number:%s  EmailID:%s\n",addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email);
                        found = 1;
                        break;
                    }
            }

                if (!found)
                {
                    printf("No contact found with the EmailID: %s\n",emailID);
                   
                }
             break;   
        }
         
        case 4:
        printf("Exiting search...\n");
        break;
        default:
           printf("Invalid Option!Try again.\n");
        }
    } while (choice!=4);
    

}

void editContact(AddressBook *addressBook)
{
	/* Define the logic for Editcontact */
    int choice;
    do
    {
        printf("Search by: \n");
        printf("1.Name\n");
        printf("2.Phone No\n");
        printf("3.Email ID\n");
        printf("4.Exit\n");
        printf("Enter your choice:");
        scanf("%d",&choice);
        switch (choice)
        {
            case  1:
            {
           char names[50];
           int found=0;
           int match[MAX_CONTACTS];
           int matchcount=0;
           int choice;
           int selectedIndex=-1;
           // clear input buffer or clear leftover newline after previous scanf
           int c;
           while ((c = getchar()) != '\n' && c != EOF);

           printf("Enter a name to search:");
           fgets(names, sizeof(names), stdin);
           names[strcspn(names, "\n")] = '\0';
           //Search for Name
           for (int i = 0; i < addressBook->contactCount; i++) 
           {
                    if (strcmp(addressBook->contacts[i].name,names) == 0) 
                    {
                        found = 1;
                        match[matchcount] = i;  
                        matchcount++;
                    }
            }

            if (!found)
            {
                printf("No contact found with the name: %s\n", names);
                return;
                   
            }
            if (matchcount == 1) 
            {
            // Only one match - no need to ask user to select
            selectedIndex = match[0];
            Contact selected = addressBook->contacts[selectedIndex];
            printf("Contact Found:\n");
            printf("Name: %s  Number: %s  Email ID: %s\n", selected.name, selected.phone, selected.email);
            } 
            //more than 2 similar names
            else
            {
                printf("Multiple contacts found:\n");
                for (int i = 0; i < matchcount; i++) 
                {
                    int idx = match[i];
                    printf("%d. Name: %s  Number: %s  Email ID: %s\n",i + 1, addressBook->contacts[idx].name,addressBook->contacts[idx].phone,addressBook->contacts[idx].email);
                }

                 printf("Select a contact by entering a number (1 to %d): ", matchcount);
                 scanf("%d", &choice);
                 while ((c = getchar()) != '\n' && c != EOF);
                
                if (choice >= 1 && choice <= matchcount) 
                 {    
                     selectedIndex = match[choice - 1];
                    Contact selected = addressBook->contacts[selectedIndex];
                    printf("\nYou selected:\n");
                    printf("Name: %s  Number: %s  Email ID: %s\n",selected.name, selected.phone, selected.email);
                } 
                else 
                {
                    printf("Invalid selection.\n");
                    return;
                }
                
            }
                //Edit
                char newName[50];
                printf("Enter new name: ");
                fgets(newName, sizeof(newName), stdin);
                newName[strcspn(newName, "\n")] = '\0';
                if (strlen(newName) > 0) 
                {
                    strncpy(addressBook->contacts[selectedIndex].name, newName, sizeof(addressBook->contacts[selectedIndex].name));
                    printf("Name updated successfully.\n");
                } 
                else 
                {
                    printf("Name not changed.\n");
                }
                break;   
            }

           case 2:
           {
            char phoneNo[20];
            int found=0;
            int selectedIndex=-1;
           // clear input buffer or clear leftover newline after previous scanf
           int c;
           while ((c = getchar()) != '\n' && c != EOF);

           printf("Enter a Phone Number to search:");
           fgets(phoneNo, sizeof(phoneNo), stdin);
           phoneNo[strcspn(phoneNo, "\n")] = '\0';
           //Check length
           if (strlen(phoneNo) != 10)
           {
                printf("Phone number must be exactly 10 digits!\n");
                return;
           }
           //Search phoneNo
           for (int i = 0; i < addressBook->contactCount; i++) 
           {
                    if (strcmp(addressBook->contacts[i].phone,phoneNo) == 0) 
                    {
                        printf("Contact Found:\n");
                        printf("Name:%s  Number:%s  EmailID:%s\n",addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email);
                        found = 1;
                        selectedIndex = i;
                        break; 
                    }
            }

                if (!found)
                {
                    printf("No contact found with the Phone Number: %s\n", phoneNo);
                    return;
                }
                //EDIT
                char newphone[20];
                printf("Enter new Phone Number: ");
                fgets(newphone, sizeof(newphone), stdin);
                newphone[strcspn(newphone, "\n")] = '\0';
                if (strlen(newphone) ==10) 
                {
                    strncpy(addressBook->contacts[selectedIndex].phone, newphone, sizeof(addressBook->contacts[selectedIndex].phone));
                    printf("Name updated successfully.\n");
                } 
                else 
                {
                    printf("Name not changed.\n");
                }
                break;   
             
        }
         
        case 3:
        {
           char emailID[20];
           int found=0;
           int selectedIndex=-1;
           // clear input buffer or clear leftover newline after previous scanf
           int c;
           while ((c = getchar()) != '\n' && c != EOF);
           printf("Enter a name to search:");
           fgets(emailID, sizeof(emailID), stdin);
           emailID[strcspn(emailID, "\n")] = '\0';
           //Search Email ID
           for (int i = 0; i < addressBook->contactCount; i++) 
           {
                    if (strcmp(addressBook->contacts[i].email,emailID) == 0) 
                    {
                        printf("Contact Found:\n");
                        printf("Name:%s  Number:%s  EmailID:%s\n",addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email);
                        found = 1;
                        selectedIndex = i; 
                        break;
                    }
            }

                if (!found)
                {
                    printf("No contact found with the EmailID: %s\n",emailID);
                    return;
                   
                }
                //EDIT
                char newemail[50];
                printf("Enter new Email ID: ");
                fgets(newemail, sizeof(newemail), stdin);
                newemail[strcspn(newemail, "\n")] = '\0';
                if (strlen(newemail)>0) 
                {
                    strncpy(addressBook->contacts[selectedIndex].email, newemail, sizeof(addressBook->contacts[selectedIndex].email));
                    printf("Name updated successfully.\n");
                } 
                else 
                {
                    printf("Name not changed.\n");
                }
                
             break;   
        }
         
        case 4:
        printf("Exiting search...\n");
        break;
        default:
           printf("Invalid Option!Try again.\n");
       


        }
    
    }while (choice!=4);
}

void deleteContact(AddressBook *addressBook)
{
	/* Define the logic for deletecontact */
   


    int choice;
    do
    {
        printf("Search by: \n");
        printf("1.Name\n");
        printf("2.Phone No\n");
        printf("3.Email ID\n");
        printf("4.Exit\n");
        printf("Enter your choice:");
        scanf("%d",&choice);
        switch (choice)
        {
        case 1:
        {
           char names[50];
           int found=0;
           int match[MAX_CONTACTS];
           int matchcount=0;
           int choice;
           // clear input buffer or clear leftover newline after previous scanf
           int c;
           while ((c = getchar()) != '\n' && c != EOF);
           printf("Enter a name to search:");
           fgets(names, sizeof(names), stdin);
           names[strcspn(names, "\n")] = '\0';
           //Search for Name
           for (int i = 0; i < addressBook->contactCount; i++) 
           {
                    if (strcmp(addressBook->contacts[i].name,names) == 0) 
                    {
                        found = 1;
                        match[matchcount] = i;  
                        matchcount++;
                    }
            }

            if (!found)
            {
                printf("No contact found with the name: %s\n", names);
                return;
                   
            }
            if (matchcount == 1) 
            {
            // Only one match - no need to ask user to select
            int selectedIndex = match[0];
            Contact selected = addressBook->contacts[selectedIndex];
            printf("Contact Found:\n");
            printf("Name: %s  Number: %s  Email ID: %s\n", selected.name, selected.phone, selected.email);
            } 
            //more than 2 similar names
            else
            {
                printf("Multiple contacts found:\n");
                for (int i = 0; i < matchcount; i++) 
                {
                    int idx = match[i];
                    printf("%d. Name: %s  Number: %s  Email ID: %s\n",i + 1, addressBook->contacts[idx].name,addressBook->contacts[idx].phone,addressBook->contacts[idx].email);
                }

                 printf("Select a contact by entering a number (1 to %d): ", matchcount);
                 scanf("%d", &choice);

                 if (choice >= 1 && choice <= matchcount) 
                 {    
                    int selectedIndex = match[choice - 1];
                    Contact selected = addressBook->contacts[selectedIndex];
                    printf("\nYou selected:\n");
                    printf("Name: %s  Number: %s  Email ID: %s\n",selected.name, selected.phone, selected.email);
                } 
                else 
                {
                    printf("Invalid selection.\n");
                }
            }
            //DELETE
            char name[50];
            printf("Enter the name of the contact to delete: ");
            scanf(" %[^\n]", name);

            for (int i = 0; i < addressBook->contactCount; i++) 
            {
                //checking if the name should be modified is present in the list or not
                if (strcmp(addressBook->contacts[i].name, name) == 0)
                {
                    for (int j = i; j < addressBook->contactCount - 1; j++) 
                    {
                        addressBook->contacts[j] = addressBook->contacts[j + 1];
                    }
                    addressBook->contactCount--;
                    printf("Contact deleted.\n");
                    return;
                }
            }
            printf("Contact not found.\n");
            return; 
            break;   
        }
         
        case 2:
        {
           char phoneNo[20];
           int found=0;

           // clear input buffer or clear leftover newline after previous scanf
           int c;
           while ((c = getchar()) != '\n' && c != EOF);
           printf("Enter a Phone Number to search:");
           fgets(phoneNo, sizeof(phoneNo), stdin);
           phoneNo[strcspn(phoneNo, "\n")] = '\0';
           //Check length
           if (strlen(phoneNo) != 10)
           {
                printf("Phone number must be exactly 10 digits!\n");
                return;
           }
           //Search phoneNo
           for (int i = 0; i < addressBook->contactCount; i++) 
           {
                    if (strcmp(addressBook->contacts[i].phone,phoneNo) == 0) 
                    {
                        printf("Contact Found:\n");
                        printf("Name:%s  Number:%s  EmailID:%s\n",addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email);
                        found = 1;
                        break; 
                    }
            }

                if (!found)
                {
                    printf("No contact found with the Phone Number: %s\n", phoneNo);
                   
                }
                //DELETE
            char phone[20];
            printf("Enter the phone number of the contact to delete: ");
            scanf(" %[^\n]",phone);

            for (int i = 0; i < addressBook->contactCount; i++) 
            {
                //checking if the name should be modified is present in the list or not
                if (strcmp(addressBook->contacts[i].phone, phone) == 0)
                {
                    for (int j = i; j < addressBook->contactCount - 1; j++) 
                    {
                        addressBook->contacts[j] = addressBook->contacts[j + 1];
                    }
                    addressBook->contactCount--;
                    printf("Contact deleted.\n");
                    return;
                }
            }
            printf("Contact not found.\n");
            return; 
            
             break;   
        }
         
        case 3:
        {
           char emailID[20];
           int found=0;

           // clear input buffer or clear leftover newline after previous scanf
           int c;
           while ((c = getchar()) != '\n' && c != EOF);
           printf("Enter a name to search:");
           fgets(emailID, sizeof(emailID), stdin);
           emailID[strcspn(emailID, "\n")] = '\0';
           //Search Email ID
           for (int i = 0; i < addressBook->contactCount; i++) 
           {
                    if (strcmp(addressBook->contacts[i].email,emailID) == 0) 
                    {
                        printf("Contact Found:\n");
                        printf("Name:%s  Number:%s  EmailID:%s\n",addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email);
                        found = 1;
                        break;
                    }
            }

                if (!found)
                {
                    printf("No contact found with the EmailID: %s\n",emailID);
                }
                //DELETE
            char email[50];
            printf("Enter the Email ID of the contact to delete: ");
            scanf(" %[^\n]",email);

            for (int i = 0; i < addressBook->contactCount; i++) 
            {
                //checking if the name should be modified is present in the list or not
                if (strcmp(addressBook->contacts[i].email,email) == 0)
                {
                    for (int j = i; j < addressBook->contactCount - 1; j++) 
                    {
                        addressBook->contacts[j] = addressBook->contacts[j + 1];
                    }
                    addressBook->contactCount--;
                    printf("Contact deleted.\n");
                    return;
                }
            }
            printf("Contact not found.\n");
            return; 
            break;   
        }
         
        case 4:
        printf("Exiting search...\n");
        break;
        default:
           printf("Invalid Option!Try again.\n");
        }
    } while (choice!=4);
    
}


