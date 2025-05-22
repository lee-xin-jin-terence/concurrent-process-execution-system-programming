
/****************************************
*
* Author: Terence Lee Xin Jin
* 
* Aim: Execute commands in child processes
*	concurrently
*
* Date: 19 June 2020
*
******************************************/


#include <stdio.h>


//For exit()
#include <stdlib.h>


//For fork(), wait()
#include <sys/types.h>


//For fork(), execl()
#include <unistd.h>


//For wait()
#include <wait.h>


#define NO_ARG_EXIT_STATUS 1

#define ZERO_CHILD_EXIT_STATUS 2

//This is for the child process only
#define EXEC_FAILED_EXIT_STATUS 3



	/*-------------------------------------
	 Brief: A struct containing information
		regarding a child process

	 Members:
		processId - process id of the
		   child process

		commandPathPtr - a pointer that
		   points to the corresponding
		   command path of the child
		   process
	-------------------------------------*/

typedef struct  
{
	pid_t processId;

	char * commandPathPtr;

} Process;


/*---------------------------------------------------------*/


	/*-----------------------------------------
	 Brief: Initialise the commmand path in 
		the struct array

	 Parameters:
		commandPathArr - an array of command
			path (input of function)

		childProcessArr - an array of struct
			containing process information
			(associated command path and 
			pid)

		arrSize - size of array of commandPathArr
			and childProcessArr (both arrays
			are expected to be of same size)
	------------------------------------------*/
void structArrayInitialise(char * commandPathArr[],
			   Process * childProcessArr,
			   int arrSize);



	/*--------------------------------------------
	 Brief: Forks off child processes and have 
		these child processes execute the
		commands. The pid of the child processes
		are then stored in the struct array 
		childProcessArr

	 Parameters: 
		childProcessArr - the array of struct that 
			contains information (path and 
			pid) of child array. This is
			both the input and output of the
			function. The command path 
			stored in the array is the input
			while the pid of the child processes
			will be the output

		arrSize - the number of elements in the
			struct array childProcessArr


	 Return:
		int - the number of child processes 
			that are successfully forked. 
	---------------------------------------------*/
int forkAndExecChildProcesses(Process * childProcessArr,
				int arrSize);

	
	
	/*---------------------------------------------
	 Brief: Sorts the array childProcessArr 
		in ascending order according to its
		process id. 

	 Parameters:
		childProcessArr - the array of struct 
			containing information (associated
			command path and pid) of child 
			processes

		arrSize - the number of elements in 
			the struct array childProcessArr
	----------------------------------------------*/
void sortChildProcessArray(Process * childProcessArr, 
		           int arrSize);



	/*--------------------------------------------
	 Brief: Waits for all child processes to 
		terminate and display a message whether
		they have terminated properly

	 Parameter:
		childProcessArr - an array of struct 
			containing information (associated
			command path and pid) of child 
			processes

		arrSize - the number of elements in the
			struct array childProcessArr
	---------------------------------------------*/
void displayAllChildTermMsg(const Process * childProcessArr,
			    int arrSize);


	
	/*--------------------------------------------
	 Brief: Gets the corresponding command path
		of a child process using its process
		id

	 Parameters:
		childProcessArr - the array of struct
			containing information (associated 
			command path and pid) of child
			processes

		arrSize - the number of elements in 
			the array childProcessArr

		targetProcessId - the process id of 
			the child process of interest


	 Return:
		char * - a pointer a command path
	--------------------------------------------*/
char* getCommandPathFromPid(const Process * childProcessArr,
		 	int arrSize, pid_t targetProcessId);



	/*------------------------------------------
	 Brief: Returns a value on whether a child
		process has terminated successfully

		If the child process terminated with
		an exit status of 0 (zero), it means
		that the child process that terminated
		successfully.

		If the child is terminated by a signal
		or has an exit status of non-zero,
		it means that the child did not 
		terminate successfully 
		

	 Parameters:
		waitStatusFlags - an numerical value
		    returned by wait() that contains
		    bitwise information of how a child
		    process has terminated

	 Return:
		int - zero if the program is terminated
		   successfully, and -1 is the program
		   did not terminate successfully
	------------------------------------------*/
int checkIfTermSuccess(int waitStatusFlags);




int main(int argc, char * argv[] )
{

	/*============================================
	 SECTION 1: Declaration of Variables
	=============================================*/

	Process childProcessArr[argc-1];

	int numOfCommands;

	int numOfChildProcesses;



	/*===============================================
	 SECTION 2: Initialisation of Variables
	================================================*/

	numOfCommands = argc - 1;	



	/*================================================
	 SECTION 3: Check if the user has entered any
		    arguments. 

	 If the user did not enter any arguments, 
	 terminate the program
	================================================*/
	if (argc == 1)
	{
		printf("\nError: No Arguments Added.\n");
		printf("\nUsage: %s <commandPath1> "
		     "[<commandPath2> ...]\n\n", argv[0]);
		
		exit(NO_ARG_EXIT_STATUS);
	}




	/*===============================================
	 SECTION 4: Initialise the array childProcessArr
		   

	 Initialise the struct array childProcessArr
	 by  assigning each element with a command path
	 first (simple initialisation)
	================================================*/
	structArrayInitialise(argv + 1, childProcessArr,
			      numOfCommands);



	
	/*==============================================
	 SECTION 5: Fork and Exec Child Processes

	 Child processes are forked,and their respective
	 commands as specified by the user arguments
	 is executed

	 If there is any fork error, any remaining 
	 commands (user arguments) will not be executed.
	 However, the program will wait for child
	 processes that are already forked to terminate.

	 Exit the program if there is zero child process
	 created at all.

	===============================================*/



	printf("\n\n");

	numOfChildProcesses = 
		forkAndExecChildProcesses(childProcessArr,
					  numOfCommands);	



	if (numOfChildProcesses == 0)
	{
		printf("Failed to create any child process\n");

		exit(ZERO_CHILD_EXIT_STATUS);

	}
	else if (numOfChildProcesses < numOfCommands)
	{
		printf("Failed to create fork for the "
			"remaining %d commands. These "
			"remaining commands will not be run\n", 
			numOfCommands - numOfChildProcesses); 
	}



	/*===============================================
	 SECTION 6: Sort the child processes according
		   to their process id

	 Although the process id is almost always already
	 in ascending order, some systems may not assign
	 process id in ascending order. Sorting is 
	 carried out to ensure portability.
	================================================*/
	if (numOfChildProcesses > 1)
	{

		sortChildProcessArray(childProcessArr,
				      numOfChildProcesses);
	}



	
	/*=================================================
	 SECTION 6: Wait for child processes to terminate
	 	    and display whether the command has
		    terminated successfully
	==================================================*/
	
	displayAllChildTermMsg( childProcessArr,
				numOfChildProcesses);


	printf("All done, bye-bye!\n");

	return 0;
}


/*---------------------------------------------------------
 Initialise the command path for the array of Process

  Parameters:
	arrayOfCommandPath - char pointer array representing an array 
 			of command paths strings, where each element of 
    			the array has not been initialized

       childProcessArray - Process pointer representing an array
       			of Process structs

   	arrSize - integer value representing the size of arrayOfCommandPath &
    			childProcessArray. The array size of arrayOfCommandPath,
       			and childProcessArray is expected to be the same
       			
----------------------------------------------------------*/

void structArrayInitialise(char * arrayOfCommandPath[],
		           Process * childProcessArray,
		           int arraySize)
{

	
	/*================================================
	 INITIALISATION: Initialise the command path
	  for the array of Process
	=================================================*/


	for (int index = 0; index < arraySize; index++)
	{
		
		childProcessArray[index].commandPathPtr =
				      arrayOfCommandPath[index];

	}
}



/*---------------------------------------------------------
 Fork out a process and perform exec() for each intended
 processes in childProcessArr

 Parameters:
 	childProcessArray - a pointer representing an array of
  		Process structs (containing intended processes
    		to be fork() and exec()

      	arrSize - the size of the array childProcessArr


 Return:
 	an integer representing the number of processes forked
----------------------------------------------------------*/

int forkAndExecChildProcesses(Process * childProcessArr,
			      int arrSize)
{

	/*=================================================
	 SECTION 1: Declaration of Variables
	==================================================*/

	pid_t currentProcessId;

	int numOfChildProcesses;
	


	/*================================================
	 SECTION 2: Creating and Executing Child Processes

	 Child processes are forked off and executed.

	 The process IDs of the child are then stored
	 in the Process array childProcessArr
	=================================================*/

	for (int index = 0; index < arrSize; index++)
	{
		

		/*------------------------------------
		 (a) Create a child process
		-------------------------------------*/
		currentProcessId = fork();


		/*--------------------------------------
		 (b) If there is any fork error, stop
		 creating any more child processes.
		 End the for-loop.
		---------------------------------------*/
		if (currentProcessId == -1)
		{
			numOfChildProcesses = index;

			break;
		}
		/*---------------------------------------
		 (c) Keeping track of child  processes)
		 (This is for the parent process)

		
		 Store the child process id and its
		 corresponding command path in a struct

		 Next, store the struct in an array
		---------------------------------------*/
		else if (currentProcessId > 0)
		{

			childProcessArr[index].processId =
					     currentProcessId;

			numOfChildProcesses = index + 1;
		}
		/*----------------------------------------
		 (d) Execute the program specified by
		     the command path
		     (This is for the child processes)


		     If the program does not exists,
		     display an error message
		----------------------------------------*/
		else
		{
			execl(childProcessArr[index].
					      commandPathPtr,
			      childProcessArr[index].
					      commandPathPtr,
			       NULL);

			printf("\n\nFailed to execute command "
			     "%s\n\n",
 			     childProcessArr[index].
					    commandPathPtr); 

			exit(EXEC_FAILED_EXIT_STATUS);
		}



	}//end of for-loop



	/*===============================================
	 SECTION 3: Return the number of child processes
	 	    forked
	================================================*/
	return numOfChildProcesses;

}


/*---------------------------------------------------------*/

void displayAllChildTermMsg(const Process * childProcessArr,
			    int arrSize)
{

	/*================================================
	 SECTION 1: Declaration of Variables
	=================================================*/

	pid_t childProcessId;

	int waitStatusFlags;
	
	char * commandPathPtr;

	int terminationSuccess;

	

	
	/*================================================
	 SECTION 2: Wait for each child to terminate
	  and display whether they terminated successfully
	=================================================*/

	while((childProcessId = wait(&waitStatusFlags)) > 0)
	{
		/*----------------------------------------
		 (a) Get the command path of the child
	     	 process that was terminated
		----------------------------------------*/
		commandPathPtr = getCommandPathFromPid(
					childProcessArr,
					arrSize,
					childProcessId);
		
		


		/*---------------------------------------
		(b) Check whether the child process has
		    terminated properly

		    Return value of 0 (zero) means it has
		    properly terminated

		    Return value of -1  means it has
		    not properly terminated
		---------------------------------------*/
		terminationSuccess = 
			checkIfTermSuccess(waitStatusFlags);

		
		
		/*--------------------------------------
		 (c) Display the termination result
		---------------------------------------*/
		if (terminationSuccess == 0)
		{
			printf("\n\nCommand %s has "
			       "completed successfully\n\n",
				commandPathPtr);
		}
		else
		{
			printf("\n\nCommand %s has not "
			       "completed successfully\n\n",
				commandPathPtr);
		}


	}//end of while loop
}



/*---------------------------------------------------------*/

char* getCommandPathFromPid(const Process * childProcessArr,
		 	    int arrSize, pid_t targetProcessId)
{

	/*===============================================
	 SECTION 1: Declaration of variables
	================================================*/

	pid_t currentProcessId;
	
	char* commandPathPtr;

	int startIndex;

	int middleIndex;

	int endIndex;
	


	/*================================================
	  SECTION 2: Initialising Variables
	================================================*/

	commandPathPtr = NULL;

	startIndex = 0;

	endIndex = arrSize - 1;



	/*===============================================
	 SECTION 3: Sorting the array
	================================================*/

	while(startIndex <= endIndex)
	{
		/*------------------------------------
		 the middleIndex is calculated this way
		 to prevent integer overflow

		 same as (startIndex + endIndex)/2
		-------------------------------------*/
		middleIndex = startIndex + 
				(endIndex-startIndex)/2;

		currentProcessId = 
		       childProcessArr[middleIndex].processId;



		if (currentProcessId == targetProcessId)
		{
			
			commandPathPtr = 
			     childProcessArr[middleIndex].
					commandPathPtr;

			break;
		}
		else if(currentProcessId < targetProcessId)
		{
			startIndex = middleIndex +1;
		}
		else
		{
			endIndex  = middleIndex - 1;
		}

	}//end of binary search




	/*===============================================
	 SECTION 4: Returning the pointer to the
		    commandPath
	===============================================*/
	return commandPathPtr;

}


/*--------------------------------------------------------*/


void sortChildProcessArray(Process * childProcessArr, 
		           int arrSize)
{

	/*================================================
	 SECTION 1: Declaration of Variables
	================================================*/
	
	int currentIndex;	

	pid_t tempProcessId;

	pid_t currentProcessId;

	Process tempProcess;



	/*==================================================
	 SECTION 2: Sorting the processArr array
		    using process id as the basis
		    of the sort (Insertion Sort)
	===================================================*/

	for (int unsortedIndex=1; unsortedIndex < arrSize;
					     unsortedIndex++)
	{
		/*--------------------------------------
		 (a) Store the unsorted process in the
		 the variable tempProcess

		 Retrieve the process id of the extracted
		 Process
		----------------------------------------*/

		tempProcess = childProcessArr[unsortedIndex];

		tempProcessId = tempProcess.processId;

		
		/*--------------------------------------
		 (b)Retrieve the process id of the process
		 just right before the unsorted process
		---------------------------------------*/
		currentIndex = unsortedIndex - 1;

		currentProcessId = 
		     childProcessArr[currentIndex].processId; 

		

		/*----------------------------------------
		 (c) If there is any process id that is larger
		 than the process id on its right, shift
		 the larger element to the right
		-----------------------------------------*/
		while (currentIndex >= 0
				 && 
			currentProcessId > tempProcessId)
		{

			childProcessArr[currentIndex + 1] =
				childProcessArr[currentIndex];
			
			currentIndex--;

			currentProcessId =
			     childProcessArr[currentIndex].
						  processId;

		}//end of inner while loop


		/*--------------------------------------
		 (d) Place the current unsorted element at
		 its rightful place
		---------------------------------------*/
		childProcessArr[currentIndex+1] = tempProcess;


	}//end of for-loop

}


/*------------------------------------------------------*/

int checkIfTermSuccess(int waitStatusFlags)
{
	/*==============================================
	 SECTION 1: Declaration of Variables
	===============================================*/

	int returnValue;




	/*===============================================
	 SECTION 2: Find out how the child was terminated
		    and termination status


	 (a) Terminated Normally
	 -----------------------
 	 If the child process was terminated normally,
	  we check the exit status number. 


		(a)(i) Exit status of zero
	 	-------------------------------
	 	If the return status number is 0, it
		 means that the child process has
		 completed successfully


		(a)(ii)Exit status is not zero
		-------------------------------------
		 If the exit status number is not 
		 zero, it means that the child process
		 has not completed successfully


	 (b)Terminated Via Signal or other means
	 -----------------------------------------
	 In this program, any child process that is 
	 terminated via signal or other means is 
	 assumed to have not completed successfully
	================================================*/



	//Terminated normally
	if (WIFEXITED(waitStatusFlags))
	{
		//exit status of zero
		if (WEXITSTATUS(waitStatusFlags) == 0)
		{
			returnValue = 0;
		}
		//exit status of non-zero
		else
		{
			//non-zero exit status
			returnValue = -1;

		}//end ofif (WEXITSTATUS(waitStatus) == 0)
	
	}
	//Terminated abnormally
	else
	{
		returnValue = -1;

	}//end of if (WIFEXITED(waitStatus))
	

	

	/*================================================
	 SECTION 3: Return the exit status of the child
	=================================================*/

	return returnValue;
	
}


