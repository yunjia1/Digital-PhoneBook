#include <stdio.h>
#include <stdlib.h>
#include "contactList.h"
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
// IF IT'S EASIER, YOU NEED NOT BE OVERLY GENERAL/FLEXIBLE
// BUT CAN ASSUME THESE LIMITS
#define MAXCONTACTS 30 /* max # of contacts */
#define MAXPHONES 10 /* max # of phone numbers for each contact */
#define PHONE_NUMBER_MAX_LENGTH 12

// INCLUDE YOUR OWN "HELPER" ROUTINES AS YOU SEE FIT

//Structure definition for contact: Includes name, numbers, the number of numbers for
//That contact, and link to next contact node
typedef struct contact{

	char name[256]; //Size may need to be changed
	char *numbers[MAXPHONES];
	int numberOfNumbers;
	struct contact *link;
}Node,*NodePointer;

//Global head pointer for the head and a new contactList for sorting
NodePointer head;
NodePointer contactsList;

//Use this function to create a new node containing contact information
NodePointer createContact(char newName[], NodePointer head){
	if(head==NULL){
		printf("Memory Allocation failed\n");
	}
	else{
		strcpy((head->name), newName);//Copies the name into the node
    head->link=NULL;
	}
	return head;
}

//If the list is empty, returns true
bool isEmpty(){
	if(head==NULL)
		return true;
	else
		return false;
}

//Converts the array of strings into a linked list
void convertInputToDataStructure() {

	//Find number of strings in the list
	int size=0;
	while(contactList[++size]!=NULL);
	int i=size-1;
	if(i==0)
		head=NULL; //If there is no strings, empty linked list
	else
	{
		int numbers=i;
		//Computing number of numbers in the list(starts searching from the back)
		while(isdigit(contactList[numbers][0])!=0)
			numbers--;
		numbers=i-numbers; //Number of numbers in the list
		int j=0;

		head= (NodePointer)malloc(sizeof(Node)); //Head node for list
		head->numberOfNumbers=numbers;

		while (j<numbers) //Copies the numbers for that contact into the numbers[] array
		{
			head->numbers[j]=malloc(strlen(contactList[i])); //Allocate memory
			strcat(head->numbers[j],contactList[i]); //Copies number
			j++;
			i--;
		}
		head = createContact(contactList[i],head); //Assigns the head the name for that contact
		head->link=NULL;
		NodePointer temp=head;
		i--;

		for(;i>=0;i--)
		{
			Node *n = (NodePointer)malloc(sizeof(Node)); //Creates another node(for the  next contact)

			if(i>0)//If the last name is not yet reached, find phone numbers for this node
			{
				//Finding number of numbers for this contact
				int numbers=i;
				while(isdigit(contactList[numbers][0])!=0)
					numbers--;
				numbers=i-numbers;
				n->numberOfNumbers=numbers;

				int j=0;
				//Copies numbers over to the linked list
				while (j<numbers)
				{
					n->numbers[j]=malloc(strlen(contactList[i]));//Allocate memory
					strcat(n->numbers[j],contactList[i]);//Copy it over
					j++;
					i--;
				}
			}
			n = createContact(contactList[i],n); //Assigns n pointer to a name

			if(i>0){//If we have not yet reached the end name
				temp->link=n; //Link the head/last node to this node
				temp=temp->link;//Reassign temp
			}
			else {//If we have reached the last string in the list
				temp->link=n; //Link the last node to this node
				n->link=NULL;//Link this node to NULL(end!)
			}
		}
	}
}

//eliminate trailing \n
char* rtrim(char* str)
{
   int i;
   for (i = strlen(str) - 1; i >= 0 && str[i] == '\n'; i--)
      str[i] = '\0';
   return str;
}

void PrintContact() {
  //Gets contact name from user
	char contactName[257];
	printf("Enter name: ");
	fgets(contactName, 256,stdin);
	rtrim(contactName); //Gets rid of trailing whitespace

	NodePointer n= head;
	if(isEmpty()) //If list is empty,  don't search.
		printf("Contact not found\n");
	else //If list is not empty
	{
		//While the entered name and name for this node is not the same OR this node is not linked to a NULL node
		while(strcmp(contactName,n->name)!=0 && n->link!=NULL)
			n=n->link; //Traverse the list

			if(strcmp(contactName,n->name)==0){ //If the name entered, and name for node match,
			//Print phone numbers and name
				printf("Name: %s\n",n->name);
				printf("Phone(s):");

				for(int i=0; i<n->numberOfNumbers;i++)
					printf(" %s",n->numbers[i]);

				printf("\n");
			}
			else//If names don't match print contact not found
			printf("Contact not found\n");
	}
}

 /* DeleteContact deletes a contact from the list
 * The user must enter the name of the contact to delete
 *
 */
void DeleteContact(){

	//Get name input from the user
	char contactName[257];
	printf("Enter name: ");
	fgets(contactName, 256,stdin);
	rtrim(contactName);//Get rid of trailing white space

	if(isEmpty())//If empty, nothing to delete
		printf("Contact not found\n");
	else
	{
		if(strcmp(contactName,head->name)==0) //If it is the first element in the list we delete it automatically
		{
			Node *temp = head->link;//Temp is assigned to what head is linked to
			free(head);
			head=temp;//Head is reassigned to element after head
		}
		else//If its not the first element we keep searching
		{
			Node *n=head;

			//Traverse the list(while next node is not NULL or name entered does not match)
			while(n->link!=NULL && strcmp(contactName,n->link->name)!=0)
			  n=n->link;

			if(n->link!=NULL){//If the names match execute

			  Node *temp =n->link;
			  n->link=temp->link;//Reassign what n is linked to to what temp is linked to
			  free(temp);
	    }
	    else
	      printf("Contact not found\n");//If no name in list
	  }
	}
}
//Sorts the contacts in the list into alphabetical order
void sortAllContacts(){

	if(isEmpty()==false)//If is not empty, sort list
	{
		//Finds number of strings in list
		int size=0, nodes=0;
		while(contactList[++size]!=NULL);

		//Find number of nodes(names) in list
		for(int j=0;j<size; j++){
	    if(isalpha(contactList[j][0])!=0)
	      nodes++;
		}
		//Copying linked list into an array of structures(contactsList)
		NodePointer contactsList=malloc(nodes*sizeof(Node));
		Node *n=head;
		for(int i=0;i<nodes;i++)//Copy over the linked list into an array of structures
		{
			strcpy(contactsList[i].name,n->name);//Copy names
			contactsList[i].numberOfNumbers=n->numberOfNumbers;//Copy number of Numbers
			for(int j=0; j<n->numberOfNumbers;j++)//Copies individual numbers over
			{
				contactsList[i].numbers[j]=malloc(strlen(n->numbers[j]));
				strcpy(contactsList[i].numbers[j],n->numbers[j]);
			}
			n=n->link;//Move to next node
		}

		//Sorting the array of structures in alphabetical order using selection sort
		//After each iteration, we dont search the last element placed(its already sorted)
		for(int top=nodes-1;top>0; top--){

			int largestNode=0;//Initialize largest node to 0

			//Finds the largest element and its position in this list
			for(int i=1; i<=top;i++)
				if(strcmp(contactsList[i].name,contactsList[largestNode].name)>0)
					largestNode=i;//If the first element is larger than the second element, reassign the largest node
			//Reassigns last element to the largest element in list
			Node temp = contactsList[top];
			contactsList[top]= contactsList[largestNode];
			contactsList[largestNode]=temp;
		}

		//Replacing the old linked list with a new linked list

		//Finding values for the head node
		strcpy(head->name,contactsList[0].name);//Copies name
		head->numberOfNumbers=contactsList[0].numberOfNumbers;//Copies numberofnumbers
		for(int i=0; i<head->numberOfNumbers;i++)//Copies over numbers for each contact
		{
			head->numbers[i]=malloc(strlen(contactsList[0].numbers[i]));
			strcpy(head->numbers[i],contactsList[0].numbers[i]);
		}
		head->link=NULL;
		NodePointer temp = head;
		for(int i=1; i<nodes;i++)
		{
			NodePointer n = (NodePointer)malloc(sizeof(Node));//Find values for next node
			//Copies over name, number of numbers and numbers for each node
			strcpy(n->name,contactsList[i].name);
			n->numberOfNumbers=contactsList[i].numberOfNumbers;
			for(int j=0;j<n->numberOfNumbers;j++)
			{
				n->numbers[j]=malloc(strlen(contactsList[i].numbers[j]));
				strcpy(n->numbers[j],contactsList[i].numbers[j]);
			}
			if(i<nodes-1)//If we are not yet at the last node
			{
				//Iterate through list by linking old node to this node and reassigning the old
				temp->link=n;
				temp=temp->link;
			}
			else
			{
				//Stop iterating, link old node to this node and this node to NULL(end of list)
				temp->link=n;
				n->link=NULL;
			}
		}
	}
}
void ListAllContacts(){

	//Print sorted linked list
	NodePointer temp = head;
	while(temp!=NULL)//While not reached the end
	{
		printf("Name: %s\n",temp->name);
		printf("Phone(s):");
		for(int i=0; i<temp->numberOfNumbers;i++)
			printf(" %s",temp->numbers[i]);
		printf("\n");
		temp=temp->link;//Move through the list
	}
}
/*
 * findDuplicates finds the names of contacts with
 * a phone number in common
 */
 void FindDuplicates() {

	if(isEmpty())//If is empty, no duplicates to be found
		printf("No duplicates found\n");
	else
	{
	  NodePointer n=head;
		bool noDuplicatesFound=true;

		//While we have not yet searched all contacts
		while(n!=NULL){
	    NodePointer nextContact=n->link;

		//For each of the phone numbers in this contact	compare with each number in all other contacts
		for(int j=0; j<n->numberOfNumbers;j++)
		{
			nextContact=n->link;
			while(nextContact!=NULL)//Search through all contacts after this contact
			{
				//Compare first number with all numbers for neighbouring contact
				for(int i=0; i<nextContact->numberOfNumbers ;i++)
				{
					if(strcmp(n->numbers[j],nextContact->numbers[i])==0){//If duplicate found
						printf("%s and %s have a phone number in common\n",n->name,nextContact->name);
						noDuplicatesFound=false;
						break;//Stop searching for this neighbouring contact
					}
				}
				nextContact=nextContact->link; //Go check the next contact
			}
		}
		//Iterate through next people after the first(which has already been checked over)
			n=n->link;
	  }
		if(noDuplicatesFound)//If no duplicates
			printf("No duplicates found\n");
	}
}


 /* printMenu - Prints out the user menu
 */
void printMenu() {
   printf("\n\nEnter a command by number\n");
   printf("1. List all contacts in alphabetical order\n");
   printf("2. Print phone(s) for a contact\n");
   printf("3. Find duplicate entries\n");
   printf("4. Delete contact\n");
   printf("5. Exit the program\n");
   printf("Your input: ");
}


int main()
{
   // first move data from contactList.h to a data structure
   convertInputToDataStructure();
   sortAllContacts();//Sort the contacts in this data structure and replace with sorted list
   char notDone = 1;
   while (notDone) {
      printMenu();
      char userChoice[100];
      char* s = fgets(userChoice, sizeof(userChoice), stdin);
      if (!s)
      {
         printf("Wrong input\n");
         continue;
      }
      s = rtrim(s);  //eliminate trailing \n
      int userSelection = atoi(s);

      switch (userSelection) {
         case 1: ListAllContacts(); break;
         case 2: PrintContact(); break;
         case 3: FindDuplicates(); break;
         case 4: DeleteContact(); break;
         case 5: notDone = 0; break;
         default: printf("Bad command...\n"); break;
         }

   }

   // PUT ANY POST_RUN CODE HERE

   return 0;
}
