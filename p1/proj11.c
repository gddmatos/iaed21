#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TASKS 10000
#define MAX_ACTIVITIES 10
#define MAX_USERS 50
#define TASK_DESCRIPTION 51
#define USER_DESCRIPTION 21
#define ACTIVITY_DESCRIPTION 21

typedef struct task{
   char description[TASK_DESCRIPTION];
   int duration;
   int start_time;
   int user_id;
   int status_id;
} Task;

/* Definição do nó de id de tarefas fornecidas pelo comando 'l' */
typedef struct taskIdElement {
    int task_id;
    struct taskIdElement* next;
} *pTaskId;

static pTaskId headtaskIdList;
static pTaskId lasttaskIdList;

static pTaskId headSortedTaskIdList;
static pTaskId lastSortedTaskIdList;

pTaskId newTaskIdElement(int task_id) {
    pTaskId new = (pTaskId)malloc(sizeof(struct taskIdElement));
    new->task_id = task_id;
    new->next = NULL;

    return new;
}

void freeTaskIdElement(pTaskId elem) {
    free(elem);
}

void freeTaskList() {
    pTaskId current = headtaskIdList;
    pTaskId elem;

    while (current != NULL)
    {
        elem = current;
        current = current->next;
        freeTaskIdElement(elem);
    }
    
    headtaskIdList = NULL;
    lasttaskIdList = NULL;
}

void freeSortedTaskList() {
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


void add_task(Task *tasks, Task new_task, int current_id);
void list_task(Task *tasks, char **activities, int task_index);
void list_tasks(Task *tasks, char **activities, pTaskId headList);
int description_exists(Task *tasks, char* description, int current_index);
void insertTaskIdElementByNum(pTaskId elem);
void insertTaskIdElementByAlphabeticalOrder(pTaskId elem, Task *tasks);
void printUsers(char users[MAX_USERS][USER_DESCRIPTION], int current_index);
int user_exists(char user[USER_DESCRIPTION], char users[MAX_USERS][USER_DESCRIPTION], int current_index);
int activity_exists(char* activity, char** activities, int current_index);
int isAllCaps(char* string);
int getActivityID(char* activity, char** activities, int current_index);
int getUserID(char user[USER_DESCRIPTION], char users[MAX_USERS][USER_DESCRIPTION], int current_index);
void insertTaskIdElementByStartTimeAndAlphabeticalOrder(pTaskId elem, Task *tasks);
int compareTasks(Task a, Task b);
void list_tasks_by_activity(Task* tasks, pTaskId headList);
void printList(pTaskId head); /* TODO: remove print function */
void printActivities(char **activities, int activityNum);

int lastSpace(char *string) {
    int lenght = strlen(string);
    int i;

    for (i = lenght; i > 0; i--)
    {
        if (isspace(string))
        {
            return i;
        }
    }
   return -1; 
}


int main() {
    char comando;

    Task tasks[MAX_TASKS];
    char users[MAX_USERS][USER_DESCRIPTION];
    int i;
    int duration;
    pTaskId task_id_list_element;
    
    char* activities[MAX_ACTIVITIES] = {"TO DO", "IN PROGRESS", "DONE"};
    int current_task_index = 0;
    int time = 0;
    int current_user_index = 0;
    int new_activity_index = 3;
    headtaskIdList = NULL;
    lasttaskIdList = NULL;
    headSortedTaskIdList = NULL;
    lastSortedTaskIdList = NULL;

    while (1)
    {
        Task task;
        char temp[MAX_TASKS];
        char user[USER_DESCRIPTION];
        char activity[ACTIVITY_DESCRIPTION];
        /* char integer[5]; */
        char in[1];
        char a;
        int lenght = 0;
        int task_id;
        int activity_id;

        temp[0] = '\0';
        user[0] = '\0';
        activity[0] = '\0';
        in[0] = '\0';
        
        
        comando = getchar();

        switch (comando) {
            case 'q':
                freeSortedTaskList();
                return EXIT_SUCCESS;
            
            case 't':
                scanf("%d %[^\n]", &task.duration, task.description);
                
                if (current_task_index == MAX_TASKS)
                {
                    printf("too many tasks\n");

                } else if ((description_exists(tasks, task.description, current_task_index) != 0) && (current_task_index > 0))
                {
                    printf("duplicate description\n");

                } else if (task.duration <= 0)
                {
                    printf("invalid duration\n");

                } else
                {
                    add_task(tasks, task, current_task_index);
                    task_id_list_element = newTaskIdElement(current_task_index);
                    insertTaskIdElementByAlphabeticalOrder(task_id_list_element, tasks);
                    printf("task %d\n", current_task_index + 1);
                    current_task_index += 1;

                }

                break;
            
            case 'l':
                /* scanf("%[^\n]", temp);

                if (strlen(temp) == 0)
                {

                    list_tasks(tasks, activities, headSortedTaskIdList);
                    
                } else
                {
                    a = strtok(temp, " ");

                    for (i = 0; a != NULL; i++)
                    {
                        
                        sscanf(a, "%d", &task_id);

                        if ((task_id <= 0) || (task_id > current_task_index))
                        {
                            printf("%d: no such task\n", task_id);
                        } else
                        {
                            printf("%d %s #%d %s\n", task_id, activities[tasks[task_id - 1].status_id], tasks[task_id - 1].duration, tasks[task_id - 1].description);
                        }
                        
                        a = strtok(NULL, " ");
                    }
                } */

                scanf("%1[^\n]", in);

                lenght = strlen(in);

                if (lenght == 0)
                {
                    list_tasks(tasks, activities, headSortedTaskIdList);
                } else
                {
                    while ((a = getchar()) != '\n')
                    {
                        /* if (!isspace(a) && (isdigit(a) || a == '-'))
                        {
                            task_id = (task_id * 10) + (a - '0');
                            in_num = 1;
                        } else if (isspace(a) && (in_num == 1))
                        {
                            if ((task_id <= 0) || (task_id > current_task_index))
                            {
                                printf("%d: no such task\n", task_id);
                            } else
                            {
                                printf("%d %s #%d %s\n", task_id, activities[tasks[task_id - 1].status_id], tasks[task_id - 1].duration, tasks[task_id - 1].description);
                            }

                            in_num = 0;
                            task_id = 0;
                        } */
                        if (!isspace(a))
                        {                      
                            ungetc(a, stdin);

                            scanf("%d", &task_id);

                            if ((task_id <= 0) || (task_id > current_task_index))
                            {
                                printf("%d: no such task\n", task_id);
                            } else
                            {
                                /* printf("%d %s #%d %s\n", task_id, activities[tasks[task_id - 1].status_id], tasks[task_id - 1].duration, tasks[task_id - 1].description); */
                                list_task(tasks, activities, task_id - 1);
                            }
                        }
                    }

                    ungetc('\n', stdin);
                    
                    
                    /* while (fgets(integer, sizeof integer, stdin) != NULL)
                    {
                        lenght = strlen(integer);
                        printf("fgets %s\n", integer);

                        if (i = lastSpace(integer) != lenght)
                        {
                            strcpy(temp, integer + i);
                            temp[i] = '\0';
                            strncpy(integer, integer, i);
                            integer[i] = '\0';
                        }
                        
                    
                        for (a = strtok(integer, " "); a; a = strtok (NULL, " "))
                        {
                            printf("int %s\n", a);
                            
                            sscanf(a, "%d", &task_id);

                             if ((task_id <= 0) || (task_id > current_task_index))
                            {
                                printf("%d: no such task\n", task_id);
                            } else
                            {
                                printf("%d %s #%d %s\n", task_id, activities[tasks[task_id - 1].status_id], tasks[task_id - 1].duration, tasks[task_id - 1].description);
                                printf("int %s\n", task_id);
                            }
                        }

                        if (integer[lenght - 1] == '\n')
                        {
                            ungetc(integer[lenght - 1], stdin);
                            break;
                        }  
                    } */
                       
                }
                
                break;

            case 'n':
                scanf("%d", &duration);

                if (duration < 0)
                {
                    printf("invalid time\n");
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
                scanf("%[^\n]", temp);

                /* fazer trim ao espacos no input 'u\n' deve ser igual a 'u    \n' */
                i = strspn(temp, " ");
                lenght = strlen(temp + i);
                if (lenght > 0)
                {
                    sscanf(temp, "%s", temp);
                    strcpy(user, temp);
                }
                
                if (lenght == 0)
                {
                    printUsers(users, current_user_index);
                } else if (user_exists(user, users, current_user_index) == 1)
                {
                    printf("user already exists\n");
                } else if (current_user_index == MAX_USERS)
                {
                    printf("too many users\n");
                } else
                {
                    strcpy(users[current_user_index], user);
                    current_user_index += 1;
                }
                
                break;

            case 'm':
                
                scanf("%d %s %[^\n]", &task_id, user, activity);

                task_id -= 1;

                if ((task_id < 0) || (task_id >= current_task_index))
                {
                    printf("no such task\n");
                } else if (strcmp(activity, "TO DO") == 0)
                {
                    if (strcmp(activities[tasks[task_id].status_id], activity) != 0)
                    {
                        printf("task already started\n");
                    }
                } else if (user_exists(user, users, current_user_index) == 0)
                {
                    printf("no such user\n");
                } else if (activity_exists(activity, activities, new_activity_index) == 0)
                {
                    printf("no such activity\n");
                } else if (strcmp(activities[tasks[task_id].status_id], activity) == 0)
                {
                    tasks[task_id].user_id = getUserID(user, users, current_user_index);
                } else
                {
                    task = tasks[task_id];

                    if (tasks[task_id].status_id == 0)
                    {
                        tasks[task_id].start_time = time;
                    }
                
                    tasks[task_id].status_id = getActivityID(activity, activities, new_activity_index);
                    tasks[task_id].user_id = getUserID(user, users, current_user_index);

                    if (strcmp(activity, "DONE") == 0)
                    {
                        duration = time - tasks[task_id].start_time;
                        printf("duration=%d slack=%d\n", duration, duration - tasks[task_id].duration);
                    }
                }
                
                break;

            case 'd':
                scanf("%[^\n]", temp);

                i = strspn(temp, " ");
                strncpy(activity, temp + i, ACTIVITY_DESCRIPTION);

                activity_id = getActivityID(activity, activities, new_activity_index);

                if (activity_id < 0)
                {
                    printf("no such activity\n");
                } else
                {
                    for (i = 0; i < current_task_index; i++)
                    {
                        if (tasks[i].status_id == activity_id)
                        {
                            task_id_list_element = newTaskIdElement(i);
                            insertTaskIdElementByStartTimeAndAlphabeticalOrder(task_id_list_element, tasks);
                        } 
                    }
                    list_tasks_by_activity(tasks, headtaskIdList);
                    freeTaskList();
                }
                
                break;

            case 'a':
                scanf("%[^\n]", temp);

                lenght = strlen(temp);

                if (lenght > 0)
                {
                    i = strspn(temp, " ");
                    strncpy(activity, temp + i, ACTIVITY_DESCRIPTION);
                }

                if (lenght == 0)
                {
                    printActivities(activities, new_activity_index);
                } else if (activity_exists(activity, activities, new_activity_index) == 1)
                {
                    printf("duplicate activity\n");
                } else if (isAllCaps(activity) == 0)
                {
                    printf("invalid description\n");
                } else if (new_activity_index == MAX_ACTIVITIES)
                {
                    printf("too many activities\n");
                } else
                {
                    activities[new_activity_index] = malloc(lenght + 1);
                    strcpy(activities[new_activity_index], activity);
                    new_activity_index += 1;
                }

                break;
 
            default:
                /* printf("ERRO: Comando desconhecido\n"); */
                break;
        }
        getchar();
    }

    return EXIT_FAILURE;

}

void add_task(Task *tasks, Task new_task, int current_id) {
    new_task.start_time = 0;
    new_task.status_id = 0;
    new_task.user_id = -1;
    tasks[current_id] = new_task;
}

int description_exists(Task *tasks, char* description, int current_index) {
    int i;
    
    for (i = 0; i < current_index; i++)
    {
        if (strcmp(tasks[i].description, description) == 0)
        {
            return 1;
        }  
    }
    return 0;
}

void insertTaskIdElementByNum(pTaskId elem) {

    pTaskId current = headtaskIdList;

    /* if ((current == NULL) || (current->task_id > elem->task_id))
    {
        elem->next = current;
        headtaskIdList = elem;
        return;
    }
    
    while ((current->next != NULL) && (current->next->task_id < elem->task_id))
    {
        current = current->next;
    }

    elem->next = current->next;
    current->next = elem; */

    if (current == NULL) {
        elem->next = current;
        headtaskIdList = elem;
        return;
    }

    while (current->next != NULL)
    {
        current = current->next;
    }

    elem->next = current->next;
    current->next = elem;
    
}

void insertTaskIdElementByAlphabeticalOrder(pTaskId elem, Task *tasks) {

    pTaskId current = headSortedTaskIdList;
    pTaskId last = lastSortedTaskIdList;

    if ((current == NULL) || (strcmp(tasks[current->task_id].description, tasks[elem->task_id].description) > 0))
    {
        elem->next = current;
        headSortedTaskIdList = elem;
        if (elem->next == NULL) lastSortedTaskIdList = elem;
        /* printf("inseri %s no primeiro\n", tasks[elem->task_id].description); */
        return;
    } else if (strcmp(tasks[last->task_id].description, tasks[elem->task_id].description) < 0)
    {
        last->next = elem;
        lastSortedTaskIdList = elem;
        /* printf("inseri %s no ultimo\n", tasks[elem->task_id].description); */
        return;
    }
    
    while ((current->next != NULL) && (strcmp(tasks[current->next->task_id].description, tasks[elem->task_id].description) < 0))
    {
        current = current->next;
    }

    if (current == last) lastSortedTaskIdList = elem;
    elem->next = current->next;
    current->next = elem;
    /* printf("inseri %s no meio\n", tasks[elem->task_id].description); */
}

void insertTaskIdElementByStartTimeAndAlphabeticalOrder(pTaskId elem, Task *tasks) {
    pTaskId current = headtaskIdList;
    pTaskId previous = NULL;

    if ((current == NULL) || compareTasks(tasks[elem->task_id], tasks[current->task_id]) < 0)
    {
        elem->next = current;
        headtaskIdList = elem;
        return;
    }

    while ((current != NULL) && compareTasks(tasks[elem->task_id], tasks[current->task_id]) > 0)
    {
        previous = current;
        current = current->next;
    }

    previous->next = elem;
    elem->next = current;
    
}

int compareTasks(Task a, Task b) {
    int cmp;
    cmp = a.start_time - b.start_time;

    if (cmp == 0)
    {
        cmp = strcmp(a.description, b.description);
    }
    
    return cmp;
}

void list_task(Task *tasks, char **activities, int task_index) {
    int task_id = task_index + 1;
    int activity_id = tasks[task_index].status_id;
    int duration = tasks[task_index].duration;
    char* description = tasks[task_index].description;

    printf("%d %s #%d %s\n", task_id, activities[activity_id], duration, description);
}

void list_tasks(Task *tasks, char **activities, pTaskId headList) {

    pTaskId current = headList;

    while (current != NULL)
    {
        list_task(tasks, activities, current->task_id);
        current = current->next;
    }
   
}

void print_task_start_and_desc(Task *tasks, int task_index) {
    int task_id = task_index + 1;
    Task task = tasks[task_index];
    printf("%d %d %s\n", task_id, task.start_time, task.description);
}

void list_tasks_by_activity(Task* tasks, pTaskId headList) {
    pTaskId current = headList;

    while (current != NULL)
    {
        print_task_start_and_desc(tasks, current->task_id);
        current = current->next;
    }
    
}

int user_exists(char user[USER_DESCRIPTION], char users[MAX_USERS][USER_DESCRIPTION], int current_index) {
    int i;
    
    for (i = 0; i < current_index; i++)
    {
        if (strcmp(users[i], user) == 0)
        {
            return 1;
        }  
    }
    return 0;
}

int getUserID(char user[USER_DESCRIPTION], char users[MAX_USERS][USER_DESCRIPTION], int current_index) {
    int i;
    
    for (i = 0; i < current_index; i++)
    {
        if (strcmp(users[i], user) == 0)
        {
            return i;
        }  
    }
    return -1;
}

int activity_exists(char* activity, char** activities, int current_index) {
    int i;

    for (i = 0; i < current_index; i++)
    {
        if (strcmp(activities[i], activity) == 0)
        {
            return 1;
        }
    }
    return 0;
}

int getActivityID(char* activity, char** activities, int current_index) {
    int i;

    for (i = 0; i < current_index; i++)
    {
        if (strcmp(activities[i], activity) == 0)
        {
            return i;
        }
    }
    return -1;
}

int isAllCaps(char* string) {
    int i;
    int size = strlen(string);
    char chr;

    for (i = 0; i < size; i++)
    {
        chr = string[i];
        if (islower(chr) || (isspace(chr) && chr != ' ' && chr != '\t'))
        {
            return 0;
        }
        
    }
    return 1;
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

void printList(pTaskId head) {
    pTaskId current = head;

    while (current != NULL)
    {
        printf("%d ", current->task_id);
        current = current->next;
    }
    printf("\n");
}

/* printSingleTaskByActivity: Prints a single sorted by start time and alphabetical description task
- tasks, the list of task
- headList, the first element (head) of the task list
*/
/*
void printSingleTaskByActivity (Task *tasks, int taskIndex)
{
    int taskId = taskIndex + 1;
    int startTime = tasks[taskIndex].start_time;
    char* taskDescription = tasks[taskIndex].description;

    printf("%d %d %s\n", taskId, startTime, taskDescription);

}
*/