#include <stdio.h>
#include <conio.h>
#include <string.h>
#include "library.h"

/*
 * Read ID function
 */
int read_id() {
    FILE* filer = fopen (ID_FILE, "r");
    int i = 0;
    fscanf(filer, "%d", &i);
    fclose(filer);
    FILE* filew = fopen (ID_FILE, "w");
    i++;
    fprintf(filew, "%d", i);
    fclose(filew);
    return i;
}

/*
 * Pause function
 */
void pause() {
     printf("\n\nPress any key to continue...");
     getch();
}

/*
 * Get the size of a file
 */
long get_file_size(char* x) {
     FILE *fp = fopen(x, "rb");
     fseek(fp, 0L, SEEK_END);
     long sz = ftell(fp);
     fclose(fp);
     return sz;
}

/*
 * ADMIN MAIN MENU
 */
void admin(struct teachers* teachers_list, struct reservations* reservations_list) {
    int flag;

    for ( ; ; ) {
        system("cls");
        printf("############################\n");
        printf("# ADMIN PANEL              #\n");
        printf("############################\n\n");
        printf("1) LIST ALL THE TEACHERS\n");
        printf("2) FIND A TEACHER\n");
        printf("3) ADD A NEW TEACHER\n");
        printf("4) [UN]SUSPEND A TEACHER\n");
        printf("5) DELETE A TEACHER\n");
        printf("6) CHANGE A TEACHER'S PASSWORD\n");
        printf("7) SEARCH RESERVATIONS\n");
        printf("8) VIEW TEACHERS' RESERVATIONS\n");
        printf("9) DELETE RESERVATIONS\n");
        printf("\n0) SAVE & EXIT\n\n\n");
        printf("Your choice: ");
        
        scanf("%i", &flag);
        if (flag == 0) {
           save_to_file_t(teachers_list);
           save_to_file_r(reservations_list);
           break;
        }
        else if (flag == 1)
             print_teachers(teachers_list);
        else if (flag == 2)
             find_teacher(teachers_list);
        else if (flag == 3)
             teachers_list = add_teacher(teachers_list);
        else if (flag == 4)
             teachers_list = suspend_teacher(teachers_list);
        else if (flag == 5)
             teachers_list = del_teacher(teachers_list);
        else if (flag == 6)
             teachers_list = admin_change_teacher_pwd(teachers_list);
        else if (flag == 7) {
             system("cls");
             printf("############################\n");
             printf("# SEARCH RESERVATIONS      #\n");
             printf("############################\n\n");
             printf("1) FIND AVAILABLE ROOMS BY GIVEN DATE AND TIME\n");
             printf("2) LOOK UP A ROOM AVAILABILITY\n");
             printf("\n0) BACK TO MAIN MENU\n\n\n");
             printf("Your choice: ");
             scanf("%i", &flag);
             if (flag == 1)
                find_rooms(reservations_list);
             else if (flag == 2)
                  lookup_room(reservations_list);
             else if (flag == 0)
                  continue;
        }
        else if (flag == 8)
             print_teachers_reservations(reservations_list);
        else if (flag == 9) {
             Tuple ret = del_reservation(teachers_list, reservations_list);
             teachers_list = ret.t;
             reservations_list = ret.r;
        }
    }
    exit(0);
}

/*
 * TEACHER MAIN MENU
 */
void teacher(int id, struct teachers* teachers_list, struct reservations* reservations_list) {
    int flag;

    for ( ; ; ) {
        printf("############################\n");
        printf("# TEACHER PANEL            #\n");
        printf("############################\n\n");
        printf("1) CHANGE PASSWORD\n");
        printf("2) FIND AVAILABLE ROOMS BY GIVEN DATE AND TIME\n");
        printf("3) LOOK UP A ROOM AVAILABILITY\n");
        printf("4) MAKE A RESERVATION\n");
        printf("\n0) SAVE & EXIT\n\n\n");
        printf("Your choice: ");
        
        scanf("%i", &flag);
        if (flag == 0) {
           save_to_file_t(teachers_list);
           save_to_file_r(reservations_list);
           break;
        }
        else if (flag == 1)
             teachers_list = change_teacher_pwd(id, teachers_list);
        else if (flag == 2)
             find_rooms(reservations_list);
        else if (flag == 3)
             lookup_room(reservations_list);
        else if (flag == 4)
             reservations_list = new_reservation(id, reservations_list);
        system("cls");
    }
    exit(0);
}

/*
 * Load teachers data from file
 */
struct teachers *load_from_file_t(struct teachers* p) {
       FILE *fp = fopen(TEACHERS_FILE, "r");
       struct teachers *pointer;
       Teacher to_add;
       // If file is empty return
       if (get_file_size(TEACHERS_FILE) == 0)
          return p;
       // We don't want to lose the pointer to the head of the list
       pointer = p;
       while (!feof(fp)) {
             fscanf(fp, "%d;%[^;];%[^;];%[^;];%[^;];%d\n", &to_add.id, &to_add.first_name, &to_add.last_name, &to_add.username, &to_add.password, &to_add.alert);
             
             if (p != NULL) {
                // Enqueue new elements
                while (p->next != NULL)
                      p = p->next;
                p->next = (struct teachers *) malloc(sizeof(struct teachers));
                p->next->t = to_add;
                p->next->next = NULL;
             }
             else {
                  // First element
                  p = (struct teachers *) malloc(sizeof(struct teachers));
                  p->t = to_add;
                  p->next = NULL;
                  pointer = p;
             }
       }
       fclose(fp);
       return pointer;
}

/*
 * Load reservations data from file
 */
struct reservations *load_from_file_r(struct reservations* p) {
       FILE *fp = fopen(RESERVATIONS_FILE, "r");
       struct reservations *pointer;
       Reservation to_add;
       // If file is empty return
       if (get_file_size(RESERVATIONS_FILE) == 0)
          return p;
       // We don't want to lose the pointer to the head of the list
       pointer = p;
       while (!feof(fp)) {
             fscanf(fp, "%d;%d;%d;%[^;];%[^;];%d;%[^;];%d;%d;%d\n", &to_add.id, &to_add.tid, &to_add.cid, &to_add.title, &to_add.course, &to_add.cfu, &to_add.type, &to_add.date, &to_add.time, &to_add.length);
             
             if (p != NULL) {
                // Enqueue new elements
                while (p->next != NULL)
                      p = p->next;
                p->next = (struct reservations *) malloc(sizeof(struct reservations));
                p->next->r = to_add;
                p->next->next = NULL;
             }
             else {
                  // First element
                  p = (struct reservations *) malloc(sizeof(struct reservations));
                  p->r = to_add;
                  p->next = NULL;
                  pointer = p;
             }
       }
       fclose(fp);
       return pointer;
}

/*
 * Load classrooms data from file
 */
struct classrooms *load_from_file_c(struct classrooms* p) {
       FILE *fp = fopen(CLASSROOMS_FILE, "r");
       struct classrooms *pointer;
       Classroom to_add;
       // If file is empty return
       if (get_file_size(CLASSROOMS_FILE) == 0)
          return p;
       // We don't want to lose the pointer to the head of the list
       pointer = p;
       while (!feof(fp)) {
             fscanf(fp, "%d\n", &to_add.id);
             
             if (p != NULL) {
                // elementi successivi
                while (p->next != NULL)
                      p = p->next;
                p->next = (struct classrooms *) malloc(sizeof(struct classrooms));
                p->next->c = to_add;
                p->next->next = NULL;
             }
             else {
                  // primo elemento
                  p = (struct classrooms *) malloc(sizeof(struct classrooms));
                  p->c = to_add;
                  p->next = NULL;
                  pointer = p;
             }
       }
       fclose(fp);
       return pointer;
}

/*
 * Save teachers data to file
 */
void save_to_file_t(struct teachers* p) {
     FILE *fp = fopen(TEACHERS_FILE, "w");
     if (p == NULL)
        return;
     else {
          while (p->next != NULL) { // p->next because the last one will be saved with no \n
                fprintf(fp, "%d;%s;%s;%s;%s;%d\n", p->t.id, p->t.first_name, p->t.last_name, p->t.username, p->t.password, p->t.alert);
                p = p->next;
          }
          fprintf(fp, "%d;%s;%s;%s;%s;%d", p->t.id, p->t.first_name, p->t.last_name, p->t.username, p->t.password, p->t.alert);
     }
     fclose(fp);
}

/*
 * Save reservations data to file
 */
void save_to_file_r(struct reservations* p) {
     FILE *fp = fopen(RESERVATIONS_FILE, "w");
     if (p == NULL)
        return;
     else {
          while (p->next != NULL) { // p->next because the last one will be saved with no \n
                fprintf(fp, "%d;%d;%d;%s;%s;%d;%s;%d;%d;%d\n", p->r.id, p->r.tid, p->r.cid, p->r.title, p->r.course, p->r.cfu, p->r.type, p->r.date, p->r.time, p->r.length);
                p = p->next;
          }
          fprintf(fp, "%d;%d;%d;%s;%s;%d;%s;%d;%d;%d", p->r.id, p->r.tid, p->r.cid, p->r.title, p->r.course, p->r.cfu, p->r.type, p->r.date, p->r.time, p->r.length);
     }
     fclose(fp);
}

/*
 * Print all the teachers
 */
void print_teachers(struct teachers* p) {
     system("cls");
     printf("############################\n");
     printf("# TEACHERS' LIST           #\n");
     printf("############################\n");
     if (p == NULL)
           printf("\nThere are no teachers.\n");
     else {
          while (p != NULL) {
                printf("\nID: %i\n", p->t.id);
                printf("First Name: %s\n", p->t.first_name);
                printf("Last Name: %s\n", p->t.last_name);
                printf("Username: %s\n", p->t.username);
                //printf("Password: %s\n", p->t.password);
                p = p->next;
          }
     }
     pause();
}

/*
 * Find information about a teacher from their name
 */
void find_teacher(struct teachers* p) {
     system("cls");
     printf("############################\n");
     printf("# FIND A TEACHER           #\n");
     printf("############################\n\n");
     if (p == NULL)
           printf("There are no teachers.\n");
     else {
          char keyword[LEN];
          
          printf("Keyword: ");
          scanf("%s", &keyword);
          while (p != NULL) {
                // We'll search first names, last names and usernames
                if (strstr(p->t.first_name, keyword) != NULL || strstr(p->t.last_name, keyword) != NULL || strstr(p->t.username, keyword) != NULL) {
                   printf("\nID: %i\n", p->t.id);
                   printf("First Name: %s\n", p->t.first_name);
                   printf("Last Name: %s\n", p->t.last_name);
                   printf("Username: %s\n", p->t.username);
                   //printf("Password: %s\n", p->t.password);
                }
                p = p->next;
          }
          printf("\n\nEnd of the list.");
     }
     pause();
}

/*
 * Add a new teacher to the top of the linked list
 */
struct teachers *add_teacher(struct teachers *p) {
       Teacher to_add = { read_id() };
       struct teachers *pointer;
       int id;
       
       system("cls");
       printf("############################\n");
       printf("# ADD A NEW TEACHER        #\n");
       printf("############################\n\n");
       printf("ID: %d\n", to_add.id);
       printf("First Name: ");
       scanf("%s", &to_add.first_name);
       printf("Last Name: ");
       scanf("%s", &to_add.last_name);
       printf("Username: ");
       scanf("%s", &to_add.username);
       printf("Password: ");
       scanf("%s", &to_add.password);
       to_add.alert = 111;
       
       if (p != NULL) {
             // Enqueue new elements
             pointer = (struct teachers *) malloc(sizeof(struct teachers));
             pointer->t = to_add;
             pointer->next = p;
       }
       else {
            // First element
            p = (struct teachers *) malloc(sizeof(struct teachers));
            p->t = to_add;
            p->next = NULL;
            pointer = p;
       }
       printf("\nThe new entry was successfully added!");
       pause();
       return pointer;
}

/*
 * [Un]Suspend a teacher
 */
struct teachers *suspend_teacher(struct teachers* p) {
       system("cls");
       printf("############################\n");
       printf("# [UN]SUSPEND A TEACHER    #\n");
       printf("############################\n");
       printf("Status Legend: 1 = OK, 2 = Suspended\n\n");
       if (p == NULL)
           printf("There are no teachers.\n");
       else {
            struct teachers *saved_p = p;
            int id;
            
            // Show all the teachers and their relative status
            while (p != NULL) {
                  printf("[%d]\t%s %s - Status: %d\n", p->t.id, p->t.first_name, p->t.last_name, p->t.alert % 100 / 10);
                  p = p->next;
           }
           
           // Let's go back to the start of the linked list
           p = saved_p;
           printf("\nID of the teacher that you want to [un]suspend: ");
           scanf("%d", &id);
           while (p != NULL) {
                 if (p->t.id == id) {
                    // Found it!
                    if (p->t.alert % 100 / 10 == 1)
                       p->t.alert = p->t.alert + 10;
                    else
                        p->t.alert = p->t.alert - 10;
                    printf("\nThe teacher was successfully [un]suspended!");
                    p = saved_p; // Go to the top again
                    pause();
                    return p;
                 }
                 p = p->next;
           }
           printf("\nThere's no teacher with that ID!");
           p = saved_p; // Go to the top again
       }
       pause();
       return p;
}

/*
 * Delete a teacher
 */
struct teachers *del_teacher(struct teachers *p) {
       system("cls");
       printf("############################\n");
       printf("# DELETE A TEACHER         #\n");
       printf("############################\n\n");
       if (p == NULL)
           printf("There are no teachers.\n");
       else {
            struct teachers *saved_p = p;
            int id;
            
            while (p != NULL) {
                 printf("[%d]\t%s %s\n", p->t.id, p->t.first_name, p->t.last_name);
                 p = p->next;
            }
            
            // Let's go back to the start of the linked list
            p = saved_p;
            
            printf("\nID of the teacher that you want to permanently delete: ");
            scanf("%d", &id);

            // If the teacher we'd like to remove is on the top of the list...
            if (p->t.id == id) {
               p = p->next; // ... we put the one next to him on the top of the list
               printf("\nThe entry was successfully deleted!");
               pause();
               return p;
            }
            else {
                 while (p->next != NULL) {
                       if (p->next->t.id != id)
                          p = p->next;
                       else {
                            // Found it!
                            p->next = p->next->next;
                            p = saved_p;
                            printf("\nThe entry was successfully deleted!");
                            pause();
                            return p;
                       }
                 }
                 printf("\nThere's no teacher with that ID.");
                 pause();
                 return saved_p;
            }
       }
}

/*
 * Change a teacher's password
 */
struct teachers *admin_change_teacher_pwd(struct teachers* p) {
       system("cls");
       printf("############################\n");
       printf("# CHANGE PASSWORD          #\n");
       printf("############################\n\n");
       if (p == NULL)
           printf("There are no teachers.\n");
       else {
            struct teachers *saved_p = p;
            int id;
            
            while (p != NULL) {
                  printf("[%d]\t%s\n", p->t.id, p->t.username);
                  printf("\t%s %s\n\n", p->t.first_name, p->t.last_name);
                  p = p->next;
            }
           
            // Let's go back to the start of the linked list
            p = saved_p;
            printf("\nTeacher's ID: ");
            scanf("%d", &id);
               
            while (p != NULL) {
                  if (p->t.id != id)
                     p = p->next;
                  else {
                       // Found it!
                       printf("New password: ");
                       scanf("%s", p->t.password);
                       printf("\nThe entry was successfully modified!");
                       if (p->t.alert % 10 == 1)
                          p->t.alert = p->t.alert + 1;
                       p = saved_p;
                       pause();
                       return p;
                  }
            }
            printf("\n\nThere's no teacher with that ID.");
            p = saved_p;
       }
       pause();
       return p;
}

/*
 * View a teacher's reservations
 */
void print_teachers_reservations(struct reservations* p) {
     system("cls");
     printf("############################\n");
     printf("# TEACHER'S RESERVATIONS   #\n");
     printf("############################\n\n");
     if (p == NULL)
           printf("There are no reservations.\n");
     else {
          int tid;
          int period1, period2;
          char c;
          
          printf("Teacher's ID: ");
          scanf("%d", &tid);
          printf("Show all the reservations?\nPress any key and hit Enter or type N if you want to specify a time period: ");
          scanf("%s", &c);
          if (c == 'N' || c == 'n') {
                printf("Period 1 [YYMMDD]: ");
                scanf("%d", &period1);
                printf("Period 2 [YYMMDD]: ");
                scanf("%d", &period2);
          }
          else {
               period1 = 0;
               period2 = 999999;
          }
          
          while (p != NULL) {
                if (p->r.tid == tid && p->r.date >= period1 && p->r.date <= period2) {
                   printf("\n[%d]\t[%s] - Room %d\n", p->r.id, p->r.title, p->r.cid);
                   printf("\tCourse: %s - %d CFU - %s - %d %d - %d hours\n", p->r.course, p->r.cfu, p->r.type, p->r.date, p->r.time, p->r.length);
                }
                p = p->next;
          }
          printf("\n\nEnd of the list.");
     }
     pause();
}

/*
 * Delete a reservation
 */
Tuple del_reservation(struct teachers* m, struct reservations* p) {
       Tuple ret;
       
       system("cls");
       printf("############################\n");
       printf("# DELETE RESERVATIONS      #\n");
       printf("############################\n\n");
       if (p == NULL)
           printf("There are no reservations.\n");
       else {
            struct reservations *saved_p = p;
            struct teachers *saved_m = m;
            ret.r = p;
            int id, flag = 0;
            char c;
            
            while (p != NULL) {
                 printf("[%d] - %s - %d %d\n", p->r.id, p->r.title, p->r.date, p->r.time);
                 p = p->next;
           }
           for ( ; ; ) {
               // Let's go back to the start of the modified linked list
               p = ret.r;
               if (p == NULL) {
                  printf("There are no more reservations.\n");
                  pause();
                  break;
               }
               printf("\nID of the reservation that you want to permanently delete: ");
               scanf("%d", &id);
               // If the reservation we'd like to remove is on the top of the list...
               if (p->r.id == id) {
                  while (m != NULL) {
                        if (m->t.id == p->r.tid && m->t.alert / 100 == 1)
                           m->t.alert = m->t.alert + 100;
                        else
                            m = m->next;
                  }
                  m = saved_m;
                  p = p->next; // ... we put the one next to him on the top of the list
                  // Return tuple with modified reservations_list
                  ret.r = p;
                  ret.t = m;
               }
               else {
                    while (p->next != NULL) {
                          if (p->next->r.id != id)
                             p = p->next;
                          else {
                               // Found it!
                               flag = 1;
                               p->next = p->next->next;
                               p = saved_p;
                               // Return tuple with modified reservations_list
                               ret.r = p;
                               ret.t = m;
                          }
                    }
                    if (flag == 0) {
                       printf("\nThere's no reservation with that ID.\n");
                       continue;
                    }
                    // Return tuple, no changes to reservations_list
                    ret.r = saved_p;
                    ret.t = m;
               }
               printf("\nThe entry was successfully deleted!\n\n");
               printf("Do you want to delete another reservation?\nType Y if Yes, press any key and hit Enter to go back to the main menu: ");
               scanf("%s", &c);
               
               if (c != 'Y' && c != 'y')
                  break;
           }
           return ret;
       }
}

/*
 * Find available rooms by given date and time
 */
void find_rooms(struct reservations* p) {
     int date;
     int time;
     struct classrooms *tmp = NULL;
     struct classrooms *saved_tmp;
     
     tmp = load_from_file_c(tmp);
     saved_tmp = tmp;
     
     system("cls");
     printf("############################\n");
     printf("# FIND A CLASSROOM         #\n");
     printf("############################\n\n");     
     printf("Insert the date [YYMMDD]: ");
     scanf("%d", &date);
     printf("Insert the time [HHMM]: ");
     scanf("%d", &time);
     
     // Let's flag every classroom as available
     while (tmp != NULL) {
           tmp->c.flag = 0;
           tmp = tmp->next;
     }
     // Go back to the head
     tmp = saved_tmp;
     // Every record in the reservations file is compared to the given date and time
     while (p != NULL) {
           if (p->r.date == date && (time >= p->r.time && time <= p->r.time + p->r.length*100 + 15)) {
              while (tmp != NULL) {
                    if (p->r.cid == tmp->c.id)
                       tmp->c.flag = 1; // If a match is found, the classroom is marked as unavailable
                    tmp = tmp->next;
              }
           }
           p = p->next;
     }
     // Go back to the head
     tmp = saved_tmp;
     // Let's print only the classrooms that were not flagged
     printf("\nAvailable rooms: \n");
     while (tmp != NULL) {
           if (tmp->c.flag != 1)
              printf("- Room %d\n", tmp->c.id);
           tmp = tmp->next;
     }
     pause();
     system("cls");
}

/*
 * Check the availability of a given room
 */
void lookup_room(struct reservations* p) {
     int num, d1, d2;
     
     system("cls");
     printf("############################\n");
     printf("# LOOK UP ROOM AVAILABILITY#\n");
     printf("############################\n");
     printf("No showed records under room name means it's available.\n\n");
     
     printf("Room # you want to check: ");
     scanf("%d", &num);
     printf("From [YYMMDD]: ");
     scanf("%d", &d1);
     printf("to [YYMMDD]: ");
     scanf("%d", &d2);
     
     printf("\nRoom [%d]: \n", num);
     while (p != NULL) {
           if (p->r.cid == num && p->r.date >= d1 &&  p->r.date <= d2)
              printf("- Reserved from %d to %d on %d: %s\n", p->r.time, p->r.time + p->r.length*100 + 15, p->r.date, p->r.title);
           p = p->next;
     }
     pause();
     system("cls");
}

/*
 * Change password, only for teachers
 */
struct teachers *change_teacher_pwd(int id, struct teachers* p) {
       struct teachers *saved_p = p;
       
       system("cls");
       printf("############################\n");
       printf("# CHANGE PASSWORD          #\n");
       printf("############################\n\n");
       while (p != NULL) {
             if (p->t.id != id)
                p = p->next;
             else {
                  // Found it!
                  printf("New password: ");
                  scanf("%s", p->t.password);
                  printf("\nYou've successfully modified your password!");
                  break;
             }
       }
       p = saved_p;

       pause();
       system("cls");
       return p;
}

/*
 * New reservation
 */
struct reservations *new_reservation(int id, struct reservations* p) {
       Reservation to_add = { read_id() };
       struct reservations *pointer;
       struct reservations *c;
       struct classrooms *tmp = NULL;
       struct classrooms *saved_tmp;
       tmp = load_from_file_c(tmp);
       saved_tmp = tmp;
       int flag = 1;
       
       system("cls");
       printf("############################\n");
       printf("# MAKE A RESERVATION       #\n");
       printf("############################\n\n");
       printf("ID: %d\n", to_add.id);
       to_add.tid = id;
       while (flag) {
           printf("Classroom #: ");
           scanf("%d", &to_add.cid);
           // Check if classroom exists
           tmp = saved_tmp;
           while (tmp != NULL) {
                 if (tmp->c.id != to_add.cid)
                    tmp = tmp->next;
                 else {
                      flag = 0;
                      break;
                 }
           }
       }
       printf("Title: ");
       scanf("%s", &to_add.title);
       printf("Course: ");
       scanf("%s", &to_add.course);
       printf("CFU #: ");
       scanf("%d", &to_add.cfu);
       printf("Type [Written/Oral]: ");
       scanf("%s", &to_add.type);
       printf("Date [YYMMDD]: ");
       scanf("%d", &to_add.date);
       for ( ; ; ) {
           printf("Time [HHMM]: ");
           scanf("%d", &to_add.time);
           if (to_add.time < 800 || to_add.time >= 1900)
              printf("Time must be between 800 and 1900.\n");
           else
               break;
       }
       printf("Length (hrs): ");
       scanf("%d", &to_add.length);
       
       // Check availability
       c = p;
       while (c != NULL) {
             if ((c->r.cid == to_add.cid && c->r.date == to_add.date && c->r.time <= to_add.time && to_add.time <= c->r.time + c->r.length * 100 + 15) || (c->r.cid == to_add.cid && c->r.date == to_add.date && to_add.time + to_add.length * 100 + 15 >= c->r.time && to_add.time <= c->r.time)) {
                printf("Room # %d seems unavailable at the specified time.", to_add.cid);
                pause();
                system("cls");
                return p;
             }
             c = c->next;
       }
       if (p != NULL) {
             // Enqueue new elements
             pointer = (struct reservations *) malloc(sizeof(struct reservations));
             pointer->r = to_add;
             pointer->next = p;
       }
       else {
            // First element
            p = (struct reservations *) malloc(sizeof(struct reservations));
            p->r = to_add;
            p->next = NULL;
            pointer = p;
       }
       printf("\nThe new entry was successfully added!");
       pause();
       system("cls");
       return pointer;
}

/*
 * MAIN
 */
int main() {
    struct teachers *tl = NULL;
    struct teachers *save_tl;
    struct reservations *rl = NULL;
    char username[LEN];
    char password[LEN];
    int i;
    
    // Load data from files
    tl = load_from_file_t(tl);
    save_tl = tl;
    rl = load_from_file_r(rl);
    
    // Max. 3 attempts to login
    for (i = 0; i <= 2; i++) {
        system("cls");
        printf("############################\n");
        printf("# LOG IN                   #\n");
        printf("############################\n\n");
        printf("Username: ");
        scanf("%s", &username);
        printf("Password: ");
        scanf("%s", &password);
        
        // Login
        while(tl != NULL) {
                 if (strcmp(tl->t.username,username) == 0 && strcmp(tl->t.password,password) == 0) {
                    system("cls"); // Clear login screen, then print out alerts
                    if (tl->t.alert % 10 == 2) {
                       printf("\nYour password has been reset by the administrator, please change it asap.\n\n");
                       tl->t.alert -= 1;
                    }
                    if (tl->t.alert % 100 / 10 == 2) {
                       printf("\nYour account has been suspended.\n");
                       break;
                    }
                    if (tl->t.alert / 100 == 2) {
                       printf("\nA reservation has been deleted by the administrator.\n\n");
                       tl->t.alert -= 100;
                    }
                    // To the teacher module
                    teacher(tl->t.id, save_tl, rl);
                 }
                 else
                     tl = tl->next;
        }
        if (strcmp(username,"admin") == 0 && strcmp(password,"admin") == 0)
           admin(save_tl, rl); // To the admin module
        printf("\nFailed to log in.");
        tl = save_tl;
        pause();
    }
    return 0;
}
