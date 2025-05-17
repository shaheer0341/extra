#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX 100
#define FILENAME "records.dat"

// Structure For Crime Records
typedef struct {
	int caseID ;
	char criminalName[50] ;
	char crimeType[50] ; 
	char crimeLocation[50] ;     // Added field
	char date[20] ;     //Added case status
	char caseStatus[50] ;
}CrimeRecord;


// Function declarations

  int login();         // New login function 
  void addRecord();
  void viewRecords();
  void searchByCaseID(int);
  void deleteRecord();    //delete function
  void totalRecords();
  int loadFromFile(CrimeRecord*);
  
  int main(){
  	
  	if (!login()) {
  		printf("Too many failed attempts. Exiting program.\n");
  		exit(0);
	  }

	int choice,caseID;
	//Input options
	while (1){
		printf("\n---- Crime Records Management System ----\n");
		printf("1. Add Crime Records\n"); 
		printf("2. View All Records\n");
		printf("3. Search by Case ID\n");
		printf("4. Show Stored Records\n");
		printf("5. Delete Records\n");
		printf("6. Exit\n");
		
		printf("Enter your choice: ");
		
		if (scanf("%d",&choice) !=1){
			printf("Invalid input! Please Enter a valid number.\n");
			while (getchar() != '\n');   //clear Invalid Input Buffer
			continue;
		}
		
		switch (choice){
			case 1: addRecord();
			break;
			case 2: viewRecords();
			break;
			case 3: 
			  printf("Enter Case Id to search:");		
	        	if(scanf("%d",&caseID) != 1) {
				printf("Invalid input! Please Enter a valid number.\n");
				while(getchar() != '\n');        //Clear Input Buffer
				continue;
			}
			searchByCaseID(caseID);
			break;
			case 4: totalRecords(); break;
			case 5: deleteRecord();
			break;
			case 6: 
			printf("Exiting program. Goodbye!\n");
			exit(0);
			default:
				printf("Invalid Choice! Please try again.\n ");
		}
	}
	return 0;
}

//login Function 
  int login(){
  	char username[20], password[20];
    int attempts = 0;
 
  while(attempts < 3) {
  	
  	printf("\n--- Login Page ---\n");
  	printf("Username: ");
	scanf("%s",username); 
  	printf("Password: ");
  	scanf("%s",password);
  	
  	// You can modify these 
  	if (strcmp(username,"cslab") == 0 && strcmp(password,"4567") == 0)       //Checking and Comparing Two Strings
{
	printf("Login Successful! Welcome, %s.\n", username);
	return 1;
}
	else{
	printf("Incorrect Username or Password :Plz Try again./n");
	attempts++;
    }
  }
    return 0;
}


// Function (add new crime record)
void addRecord() {
	FILE *fp = fopen(FILENAME, "ab");     //Append in binary mode
	if(!fp) {
		printf("Error Opening File!\n");
		return;
	} 
	
	CrimeRecord cr;      //cr variable for crimerecord
	printf("Enter Case ID: ");
	if (scanf("%d",&cr.caseID) !=1) {
		printf("Invalid Input! Case ID Must Be a Number.\n");
		while(getchar() !='\n');       //Clear input Buffer
		fclose (fp);         //close the files to free system of space 
		return;
	}
	
	getchar();          //clear newline
	
	printf("Enter Criminal Name: ");
	fgets(cr.criminalName,sizeof(cr.criminalName),stdin);       //Reads a string input
	cr.criminalName[strcspn(cr.criminalName,"\n")] = '\0';     //Change Newline with Null 
	
	
	printf("Enter Crime Type: ");
	fgets(cr.crimeType,sizeof(cr.crimeType),stdin);
	cr.crimeType[strcspn(cr.crimeType, "\n")] ='\0';
	
	printf("Enter Crime Location: ");
	fgets(cr.crimeLocation,sizeof(cr.crimeLocation),stdin);
	cr.crimeLocation[strcspn(cr.crimeLocation, "\n")] = '\0';

    printf("Enter Date (dd/mm/yyyy): ");
    scanf("%19s", cr.date);

    getchar();	        // clear newline before reading case status
    
	printf("Enter Case Status (Open, Closed, Pending): ");
    fgets(cr.caseStatus, sizeof(cr.caseStatus), stdin);
    cr.caseStatus[strcspn(cr.caseStatus, "\n")] = '\0';

    fwrite(&cr, sizeof(CrimeRecord), 1, fp);     //Writes one Complete Record to Binary File
    fclose(fp);

    printf("Record added successfully!\n");
}
	

// function view all crime records

void viewRecords() {
	
	CrimeRecord records[MAX];          //Records array can holds 100 files 
	int count=loadFromFile(records);     //Call load functions and pass records array
	
	if (count==0) {
		printf("No Records Found.\n");
		return;
	}
	
	// %-Xs  print a string x character wide and left align(-) 
	printf("\n%-10d %-20s %-20s %-20s %-15s %-15s\n","Case ID","Criminal Name","Crime Type","Location","Date","Status");
	printf("------------------------------------------------------------------------------------------------------\n");
	
	for(int i = 0; i < count; i++) {
		 	printf("%-10d %-20s %-20s %-20s %-15s %-15s\n", records[i].caseID,
			       records[i].criminalName,records[i].crimeType,
				   records[i].crimeLocation,records[i].date,records[i].caseStatus);
		//prit records[0].......etc
	}
}


//function to search record by case ID
void searchByCaseID(int id){
	CrimeRecord cr;                //variable cr
	FILE*fp =fopen(FILENAME,"rb");        //read in binary mode
	if(!fp){
		printf("Error opening file!\n");
		return;
	}
	
	int found = 0;
	while(fread(&cr,sizeof(CrimeRecord),1,fp)){
		if(cr.caseID ==id){
			printf("\nRecord Found:\n");
			printf("CaseID: %d\n",cr.caseID);
			printf("Criminal Name: %s\n",cr.criminalName);
			printf("Crime Type: %s\n",cr.crimeType);
			printf("Crime Location: %s\n",cr.crimeLocation);
			printf("Date: %s\n",cr.date);
			printf("Case Status: %s\n",cr.caseStatus);
			found = 1;
			break;
		}
	}
	
	fclose(fp);
	if(!found)
	printf("Record With Case ID %d not found.\n",id);
	
}

 // Function to show stored records
 void totalRecords(){
 	
 	CrimeRecord records[MAX];
 	int total = loadFromFile(records);
 	
 	if (total == 0) {
 		printf("\nNo records found in file.\n");
	 }
	 else{
	 	printf("\nTotal number of crime records stored: %d\n",total);
	 }
 }

// Function to delete records
void deleteRecord() {
	int caseID;
	printf("Enter Case ID to Delete: ");
	 
	if (scanf("%d",&caseID) != 1) {
		printf("Invalid Input! Case ID must be a number.\n");
		while (getchar() != '\n');           //clear buffer
		return;
		
	}
	
	FILE *fp = fopen(FILENAME, "rb");     //read in binary mode
	if (!fp){
		printf("Error opening file!\n");
		return;
	}
	
	FILE *temp = fopen("temp.dat", "wb");     //open a temporary file temp.dat
	if(!temp){
		printf("Error opening temporary file!\n");
		fclose(fp);
		return;
	}
	
	CrimeRecord cr;
	int found = 0;
	while (fread(&cr, sizeof(CrimeRecord), 1, fp)) {    //loop to reads all file one by one
		if (cr.caseID == caseID) {
			found = 1;	//record to delete
		}
		else{
			fwrite(&cr, sizeof(CrimeRecord), 1, temp);     //writes the original into new file  
		}
	}
	
	fclose(fp);
	fclose(temp);
	
	if (found) {                         //checks the flag which was set in whileloop
		remove(FILENAME);                //deletes the original file contains deleted record
		rename("temp.dat",FILENAME);     //renames temp.dat to record.dat
	
		printf("Record with Case ID %d deleted successfully.\n", caseID);	
	}
	else{
		printf("Record with Case ID %d not found.\n",caseID);
	}
	
}


// Function to Load All Records From File
int loadFromFile(CrimeRecord *records){
	FILE *fp = fopen(FILENAME, "rb");    //read in binary mode
	if(!fp){
	return 0;}                  //no records were loaded
	
	int count = 0;            //a counter to keep track of records  successful 
	while (fread(&records[count], sizeof(CrimeRecord), 1, fp)){      //reads one crimerecord into records[count]
		count++;
		if(count >= MAX)        //ensures we do not exceed limit
		break;
	}
	
	fclose(fp);     //close the file
	return count;
} 
  
  
