/* ****************************** IAED 1st project 2020/2021 - Kanban board **************************** *\
| Guilherme Matos - nº56963                                                                               |
\* ***************************************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* **************************************** MACROS **************************************** */

#define MAX_TASKS 10000
#define MAX_ACTIVITIES 10
#define MAX_USERS 50
#define TASK_DESCRIPTION 51
#define USER_DESCRIPTION 21
#define ACTIVITY_DESCRIPTION 21
#define BUFFER_SIZE 101
#define TO_DO_INDEX 0
#define TO_DO "TO DO"
#define IN_PROGRESS "IN PROGRESS"
#define DONE "DONE"

/* ****************************** i18n MACROS (LANGUAGE SUPPORT) ************************************** */

#define TOO_MANY_TASKS "too many tasks"
#define TOO_MANY_USERS "too many users"
#define TOO_MANY_ACTIVITIES "too many activities"

#define DUPLICATE_DESCRIPTION "duplicate description"
#define DUPLICATE_ACTIVITY "duplicate activity"

#define INVALID_DURATION "invalid duration"
#define INVALID_TIME "invalid time"
#define INVALID_DESCRIPTION "invalid description"

#define NO_SUCH_TASK "no such task"
#define NO_SUCH_USER "no such user"
#define NO_SUCH_ACTIVITY "no such activity"

#define USER_EXISTS "user already exists"
#define TASK_ALREADY_STARTED "task already started"

#define TASK "task"
#define DURATION "duration"
#define SLACK "slack"

/* **************************************** GLOBAL DEFINITIONS **************************************** */

/* Struct task: Structure node that stores the tasks (main task list)
This list is sorted by insertion order;
- Description, with no more than 51 characters including \0
- Estimated Duration, an integer
- Start Time, an integer
- User assigned, name must not contain more than 21 chars including \0
- Status identifier (Activity)
*/
typedef struct task {
   char description[TASK_DESCRIPTION];
   int duration;
   int start_time;
   int user_index;
   int activity_index;
} Task;

/* Struct taskIdElement: Structure node that stores de task indexs sorted by description alphabetical order
- task_index: unique task index in the main tasks list
- next: pointer to the next taskIdElement node in the list
*/
typedef struct taskIdElement {
    int task_index;
    struct taskIdElement* next;
} *pTaskId;

/* Static pointers for the main task list: head and tail */
static pTaskId headTaskIdList;
static pTaskId lastTaskIdList;

/* Static pointers for the sorted task list: head and tail */
static pTaskId headSortedTaskIdList;
static pTaskId lastSortedTaskIdList;

/* **************************************** AUXILIAR FUNCTIONS **************************************** */

/* createNewTaskIdElement: Creates a node 'taskIdElement'
- taskIndex, the taskIndex to store in the taskIdElement
*/
pTaskId createNewTaskIdElement(int taskIndex) {
    pTaskId new = (pTaskId)malloc(sizeof(struct taskIdElement));
    new->task_index = taskIndex;
    new->next = NULL;

    return new;
}

/* freeTaskIdElement: Frees the memory allocated by a 'taskIdElement' nodes
- elem, the element to free
*/
void freeTaskIdElement(pTaskId elem) {
    free(elem);
}

/* freeTaskIdElement: Frees the memory allocated by all the 'taskIdElement' nodes of the main task list */
void freeTaskList()
{
    pTaskId current = headTaskIdList;
    pTaskId elem;

    while (current != NULL)
    {
        elem = current;
        current = current->next;
        freeTaskIdElement(elem);
    }
    
    headTaskIdList = NULL;
    lastTaskIdList = NULL;
}

/* freeTaskIdElement: Frees the memory allocated by all the 'taskIdElement' nodes of the sorted task list */
void freeSortedTaskList()
{
    pTaskId current = headSortedTaskIdList;
    pTaskId elem;

    while (current != NULL)
    {
        elem = current;
        current = current->next;
        freeTaskIdElement(elem);
    }
    
    headSortedTaskIdList = NULL;
    lastSortedTaskIdList = NULL;
}

/* **************************************** PROTOTYPES **************************************** */

/* Task structure manipulation */
void insertTask(Task *tasks, Task newTask, int taskIndex);
void insertTaskIdElementByDescriptionAlphabeticalOrder(pTaskId elem, Task *tasks);
void insertTaskIdElementByStartTimeAndDescriptionAlphabeticalOrder(pTaskId elem, Task *tasks);

/* Activities structure/array manipulation */
int getIndexOfActivityDescription(char** activities, char* activityDescription, int maxActivitiesIndex);

/* Users structure/array manipulation */
int getIndexOfUserName(char users[MAX_USERS][USER_DESCRIPTION], char userName[USER_DESCRIPTION], int maxUsersIndex);

/* Validations */
int compareTasks(Task taskA, Task taskB);
int isExistingTaskDescription(Task *tasks, char* taskDescription, int maxTasksIndex);
int isExistingUserName(char users[MAX_USERS][USER_DESCRIPTION], char userName[USER_DESCRIPTION], int maxUsersIndex);
int isExistingActivityDescription(char** activities, char* activityDescription, int maxActivitiesIndex);
int isAllCapsActivityDescription(char* activityDescription);

/* Printing */
void printUsers(char users[MAX_USERS][USER_DESCRIPTION], int maxUsersIndex);
void printActivities(char **activities, int maxActivitiesIndex);
void printSingleTaskByIndex(Task *tasks, char **activities, int taskIndex);
void printTasksByDescriptionAlphabeticalOrder(Task *tasks, char **activities, pTaskId headList); /* for 'l' command */
void printTasksByStartTimeAndDescriptionAlphabeticalOrder(Task* tasks, pTaskId headList); /* for 'd' command */

/* **************************************** MAIN FUNCTION **************************************** */

int main()
{

    /* command inserted */
    char command;

    /* main structures/arrays */
    Task tasks[MAX_TASKS];
    char users[MAX_USERS][USER_DESCRIPTION];
    char* activities[MAX_ACTIVITIES] = {TO_DO, IN_PROGRESS, DONE};

    /* auxiliar pointers/values */
    int i;
    int duration;

    pTaskId newTaskIdElement;
    
    int numberOfTasks = 0;
    int numberOfUsers = 0;
    int numberOfActivities = 3;
    int time = 0;
    
    headTaskIdList = NULL;
    lastTaskIdList = NULL;
    headSortedTaskIdList = NULL;
    lastSortedTaskIdList = NULL;
    
    while (1)
    {
        Task task;

        char buffer[BUFFER_SIZE];
        char userName[USER_DESCRIPTION];
        char activityDescription[ACTIVITY_DESCRIPTION];

        char in[1];
        char ch;
        int lenght = 0;
        int taskIdInput;
        int activityIndex;

        /* String reset / initializer declarations */
        buffer[0] = '\0';
        userName[0] = '\0';
        activityDescription[0] = '\0';
        in[0] = '\0';
        
        command = getchar();

        switch (command) {

            case 'q':

                freeSortedTaskList();
                return EXIT_SUCCESS;
            
            case 't':

                scanf("%d %[^\n]", &task.duration, task.description);

                if (numberOfTasks == MAX_TASKS)
                {
                    printf("%s\n", TOO_MANY_TASKS);

                } else if ((numberOfTasks > 0) && (isExistingTaskDescription(tasks, task.description, numberOfTasks) != 0))
                {
                    printf("%s\n", DUPLICATE_DESCRIPTION);

                } else if (task.duration <= 0)
                {
                    printf("%s\n", INVALID_DURATION);

                } else
                {
                    insertTask(tasks, task, numberOfTasks);
                    newTaskIdElement = createNewTaskIdElement(numberOfTasks);
                    insertTaskIdElementByDescriptionAlphabeticalOrder(newTaskIdElement, tasks);
                    printf("%s %d\n", TASK, numberOfTasks + 1);
                    numberOfTasks += 1;
                }

                break;
            
            case 'l':

                scanf("%1[^\n]", in);

                lenght = strlen(in);

                if (lenght == 0)
                {
                    printTasksByDescriptionAlphabeticalOrder(tasks, activities, headSortedTaskIdList);
                } else
                {
                    while ((ch = getchar()) != '\n')
                    {
                        if (!isspace(ch))
                        {                      
                            ungetc(ch, stdin);

                            scanf("%d", &taskIdInput);

                            if ((taskIdInput <= 0) || (taskIdInput > numberOfTasks))
                            {
                                printf("%d: %s\n", taskIdInput, NO_SUCH_TASK);
                            } else
                            {
                                printSingleTaskByIndex(tasks, activities, taskIdInput - 1);
                            }
                        }
                    }
                    ungetc('\n', stdin);    
                }
                
                break;

            case 'n':

                scanf("%d", &duration);

                if (duration < 0)
                {
                    printf("%s\n", INVALID_TIME);
                } else if (duration == 0)
                {
                    printf("%d\n", time);
                } else
                {
                    time += duration;
                    printf("%d\n", time);
                }
                
                break;

            case 'u':

                scanf("%[^\n]", buffer);

                i = strspn(buffer, " ");
                lenght = strlen(buffer + i);

                if (lenght > 0)
                {
                    sscanf(buffer, "%s", buffer);
                    strcpy(userName, buffer);
                }
                
                if (lenght == 0)
                {
                    printUsers(users, numberOfUsers);
                } else if (isExistingUserName(users, userName, numberOfUsers) == 1)
                {
                    printf("%s\n", USER_EXISTS);
                } else if (numberOfUsers == MAX_USERS)
                {
                    printf("%s\n", TOO_MANY_USERS);
                } else
                {
                    strcpy(users[numberOfUsers], userName);
                    numberOfUsers += 1;
                }
                
                break;

            case 'm':
                
                scanf("%d %s %[^\n]", &taskIdInput, userName, activityDescription);

                taskIdInput -= 1;

                if ((taskIdInput < 0) || (taskIdInput >= numberOfTasks))
                {
                    printf("%s\n", NO_SUCH_TASK);
                } else if (strcmp(activityDescription, TO_DO) == 0)
                {
                    if (strcmp(activities[tasks[taskIdInput].activity_index], activityDescription) != 0)
                    {
                        printf("%s\n", TASK_ALREADY_STARTED);
                    }
                } else if (isExistingUserName(users, userName, numberOfUsers) == 0)
                {
                    printf("%s\n", NO_SUCH_USER);
                } else if (isExistingActivityDescription(activities, activityDescription, numberOfActivities) == 0)
                {
                    printf("%s\n", NO_SUCH_ACTIVITY);
                } else if (strcmp(activities[tasks[taskIdInput].activity_index], activityDescription) == 0)
                {
                    tasks[taskIdInput].user_index = getIndexOfUserName(users, userName, numberOfUsers);
                } else
                {
                    task = tasks[taskIdInput];

                    if (tasks[taskIdInput].activity_index == TO_DO_INDEX)
                    {
                        tasks[taskIdInput].start_time = time;
                    }
                
                    tasks[taskIdInput].activity_index = getIndexOfActivityDescription(activities, activityDescription, numberOfActivities);
                    tasks[taskIdInput].user_index = getIndexOfUserName(users, userName, numberOfUsers);

                    if (strcmp(activityDescription, DONE) == 0)
                    {
                        duration = time - tasks[taskIdInput].start_time;
                        printf("%s=%d %s=%d\n", DURATION, duration, SLACK, duration - tasks[taskIdInput].duration);
                    }
                }
                
                break;

            case 'd':

                scanf("%[^\n]", buffer);

                i = strspn(buffer, " ");
                strncpy(activityDescription, buffer + i, ACTIVITY_DESCRIPTION);

                activityIndex = getIndexOfActivityDescription(activities, activityDescription, numberOfActivities);

                if (activityIndex < 0)
                {
                    printf("%s\n", NO_SUCH_ACTIVITY);
                } else
                {
                    for (i = 0; i < numberOfTasks; i++)
                    {
                        if (tasks[i].activity_index == activityIndex)
                        {
                            newTaskIdElement = createNewTaskIdElement(i);
                            insertTaskIdElementByStartTimeAndDescriptionAlphabeticalOrder(newTaskIdElement, tasks);
                        } 
                    }
                    printTasksByStartTimeAndDescriptionAlphabeticalOrder(tasks, headTaskIdList);
                    freeTaskList();
                }
                
                break;

            case 'a':

                scanf("%[^\n]", buffer);

                lenght = strlen(buffer);

                if (lenght > 0)
                {
                    i = strspn(buffer, " ");
                    strncpy(activityDescription, buffer + i, ACTIVITY_DESCRIPTION);
                }

                if (lenght == 0)
                {
                    printActivities(activities, numberOfActivities);
                } else if (isExistingActivityDescription(activities, activityDescription, numberOfActivities) == 1)
                {
                    printf("%s\n", DUPLICATE_ACTIVITY);
                } else if (isAllCapsActivityDescription(activityDescription) == 0)
                {
                    printf("%s\n", INVALID_DESCRIPTION);
                } else if (numberOfActivities == MAX_ACTIVITIES)
                {
                    printf("%s\n", TOO_MANY_ACTIVITIES);
                } else
                {
                    activities[numberOfActivities] = malloc(lenght + 1);
                    strcpy(activities[numberOfActivities], activityDescription);
                    numberOfActivities += 1;
                }

                break;
 
            default:

                break;
        }
        getchar();
    }

    return EXIT_FAILURE;

}

/* **************************************** FUNCTIONS **************************************** */

/* ---- Task structure manipulation ---- */

/* insertTask: Inserts a node 'Task' into the main tasks array (insertion order)
- Tasks, the list of tasks
- newTask, the new 'Task' node
- taskIndex, the task index where to store the node
*/
void insertTask(Task *tasks, Task newTask, int taskIndex)
{
    newTask.start_time = 0;
    newTask.activity_index = 0; /* activity_index starts as 0 which corresponds to the index of the 'TO DO' activity in the activities list */
    newTask.user_index = -1; /* user_index starts as -1 to represent 'not assigned' */
    tasks[taskIndex] = newTask;
}

/* insertTaskIdElementByDescriptionAlphabeticalOrder: Inserts a node 'taskIdElement' into the sorted tasks list by description alphabetical order (based on description)
- elem, the element to insert
- tasks, the task list where to insert the node
*/
void insertTaskIdElementByDescriptionAlphabeticalOrder(pTaskId elem, Task *tasks)
{
    pTaskId current = headSortedTaskIdList;
    pTaskId last = lastSortedTaskIdList;

    if ((current == NULL) || (strcmp(tasks[current->task_index].description, tasks[elem->task_index].description) > 0))
    {
        elem->next = current;
        headSortedTaskIdList = elem;
        if (elem->next == NULL) lastSortedTaskIdList = elem;
        return;
    
    } else if (strcmp(tasks[last->task_index].description, tasks[elem->task_index].description) < 0)
    {
        last->next = elem;
        lastSortedTaskIdList = elem;
        return;
    }
    
    while ((current->next != NULL) && (strcmp(tasks[current->next->task_index].description, tasks[elem->task_index].description) < 0))
    {
        current = current->next;
    }

    if (current == last) lastSortedTaskIdList = elem;

    elem->next = current->next;
    current->next = elem;
    
}

/* insertTaskIdElementByStartTimeAndDescriptionAlphabeticalOrder: Inserts a node 'task' into the sorted tasks list by start time and description alphabetical order (based on start time and description)
- elem, the element to insert
- tasks, the task list where to insert the node
*/
void insertTaskIdElementByStartTimeAndDescriptionAlphabeticalOrder(pTaskId elem, Task *tasks)
{
    pTaskId current = headTaskIdList;
    pTaskId previous = NULL;

    if ((current == NULL) || compareTasks(tasks[elem->task_index], tasks[current->task_index]) < 0)
    {
        elem->next = current;
        headTaskIdList = elem;
        return;
    }

    while ((current != NULL) && compareTasks(tasks[elem->task_index], tasks[current->task_index]) > 0)
    {
        previous = current;
        current = current->next;
    }

    previous->next = elem;
    elem->next = current;
    
}

/* ---- Validations ---- */

/* compareTasks: Compares two tasks start time and desciption to allow for start time asc and alphabetical order
taskA, the first task to compare
taskB, the second task to compare
*/
int compareTasks(Task taskA, Task taskB) {

    int cmp;
    cmp = taskA.start_time - taskB.start_time;

    if (cmp == 0)
    {
        cmp = strcmp(taskA.description, taskB.description);
    }
    
    return cmp;
}

/* isExistingTaskDescription: Validates if a provided task description already exists in the task list
- tasks, the task list where to verify
- taskDescription, the task description to verify
- maxTasksIndex, current number of tasks (max index)
*/
int isExistingTaskDescription(Task *tasks, char* taskDescription, int maxTasksIndex)
{
    int i;
    
    for (i = 0; i < maxTasksIndex; i++)
    {
        if (strcmp(tasks[i].description, taskDescription) == 0)
        {
            return 1;
        }  
    }
    return 0;
}

/* isExistingUserName: Validates if a provided user name already exists in the users list
- users, the user list where to verify
- userName, the user name to verify
- maxUsersIndex, current number of users (max index)
*/
int isExistingUserName(char users[MAX_USERS][USER_DESCRIPTION], char userName[USER_DESCRIPTION], int maxUsersIndex)
{
    int i;
    
    for (i = 0; i < maxUsersIndex; i++)
    {
        if (strcmp(users[i], userName) == 0)
        {
            return 1;
        }  
    }
    return 0;
}

/* isExistingActivityDescription: Validates if a provided activity description already exists in the activities list
- activities, the activities list where to verify
- activityDescription, the activity description to verify
- maxActivitiesIndex, current number of activities (max index)
*/
int isExistingActivityDescription(char** activities, char* activityDescription, int maxActivitiesIndex)
{
    int i;

    for (i = 0; i < maxActivitiesIndex; i++)
    {
        if (strcmp(activities[i], activityDescription) == 0)
        {
            return 1;
        }
    }
    return 0;
}

/* isAllCapsActivityDescription: Validates if a provided activity description only contains upper characters
- activityDescription, the activity description to validate
*/
int isAllCapsActivityDescription(char* activityDescription)
{
    int i;
    int size = strlen(activityDescription);
    char chr;

    for (i = 0; i < size; i++)
    {
        chr = activityDescription[i];
        if (islower(chr) || (isspace(chr) && chr != ' ' && chr != '\t'))
        {
            return 0;
        }
        
    }
    return 1;
}


/* ---- Activities structure/array manipulation ---- */

/* getIndexOfActivityDescription: Gets the index of an activity description from the activities list
- activities, the list of activities
- activityDescription, the activity description to look for
- maxActivitiesIndex, current number of activities (max index)
*/
int getIndexOfActivityDescription(char** activities, char* activityDescription, int maxActivitiesIndex)
{
    int i;

    for (i = 0; i < maxActivitiesIndex; i++)
    {
        if (strcmp(activities[i], activityDescription) == 0)
        {
            return i;
        }
    }
    return -1;
}

/* ---- Users structure/array manipulation ---- */

/* getIndexOfUserName: Gets the index of a user name from the users list
- users, the list of users
- userName, the user name to look for
- maxUsersIndex, current number of users (max index)
*/
int getIndexOfUserName(char users[MAX_USERS][USER_DESCRIPTION], char userName[USER_DESCRIPTION], int maxUsersIndex)
{
    int i;
    
    for (i = 0; i < maxUsersIndex; i++)
    {
        if (strcmp(users[i], userName) == 0)
        {
            return i;
        }  
    }
    return -1;
}



/* ---- Printing ---- */

/* printSingleTaskByIndex: Prints a single task from the tasks list
- tasks, the list of tasks
- activities, the list of activities (optional) - when 'activities' is provided, prints task with 'l' command format. Otherwise prints taks with 'd' command format. 
- taskIndex, the task index to print
*/
void printSingleTaskByIndex(Task *tasks, char **activities, int taskIndex)
{
    int taskId = taskIndex + 1;
    char* taskDescription = tasks[taskIndex].description;


    if (activities == NULL){

        int startTime = tasks[taskIndex].start_time;
        
        printf("%d %d %s\n", taskId, startTime, taskDescription);

    } else {
        
        int activityIndex = tasks[taskIndex].activity_index;
        char* activityDescription = activities[activityIndex];
        int duration = tasks[taskIndex].duration;
      
        printf("%d %s #%d %s\n", taskId, activityDescription, duration, taskDescription);    
    }
    
}

/* printTasksByDescriptionAlphabeticalOrder: Prints all the tasks from the taskIdElement list which is sorted by description alphabetical order - called by command 'l' with no arguments
- tasks, the list of tasks
- activities, the list of activities
- headList, the first element (head) of the taskIdElement list which is sorted by description alphabetical order
*/
void printTasksByDescriptionAlphabeticalOrder(Task *tasks, char **activities, pTaskId headList)
{
    pTaskId current = headList;

    while (current != NULL)
    {
        printSingleTaskByIndex(tasks, activities, current->task_index);
        current = current->next;
    }
   
}

/* printTasksByStartTimeAndDescriptionAlphabeticalOrder: Prints all the tasks from the sorted tasks list which contains taks sorted by start time and description alphabetical order - called by command 'd'
- tasks, the list of tasks
- headList, the first element (head) of the task list
*/
void printTasksByStartTimeAndDescriptionAlphabeticalOrder(Task* tasks, pTaskId headList)
{
    pTaskId current = headList;

    while (current != NULL)
    {
        printSingleTaskByIndex(tasks, NULL, current->task_index);
        current = current->next;
    }
    
}

/* printUsers: Prints users list - called by command 'u' with no arguments
- users, the list of users
- maxUsersIndex, current number of users (max index)
*/
void printUsers (char users[MAX_USERS][USER_DESCRIPTION], int maxUsersIndex)
{
    int i = 0;

    for (i = 0; i < maxUsersIndex; i++)
    {
        printf("%s\n", users[i]);
    }
    
}

/* printActivities: Prints activities list - called by command 'a' with no arguments
- activities, the list of activities
- maxActivitiesIndex, current number of activities (max index)
*/
void printActivities (char **activities, int maxActivitiesIndex)
{
    int i;

    for (i = 0; i < maxActivitiesIndex; i++)
    {
        printf("%s\n", activities[i]);
    }
    
}
