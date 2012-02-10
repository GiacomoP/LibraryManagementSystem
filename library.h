#define LEN 100
#define ID_FILE "database/id.txt"
#define TEACHERS_FILE "database/teachers.txt"
#define RESERVATIONS_FILE "database/reservations.txt"
#define CLASSROOMS_FILE "database/classrooms.txt"

// Teacher and List of Teachers structures
typedef struct {
        int id;
        char first_name[LEN];
        char last_name[LEN];
        char username[LEN];
        char password[LEN];
        int alert;
} Teacher;

struct teachers {
       Teacher t;
       struct teachers* next;
};

// Reservation and List of Reservations structures
typedef struct {
        int id;
        int tid;
        int cid;
        char title[LEN];
        char course[LEN];
        int cfu;
        char type[LEN];
        int date;
        int time;
        int length;
} Reservation;

struct reservations {
       Reservation r;
       struct reservations* next;
};

// Classroom and List of Classrooms structures
typedef struct {
        int id;
        int flag;
} Classroom;

struct classrooms {
       Classroom c;
       struct classrooms* next;
};

// Tuple
typedef struct {
        struct teachers* t;
        struct reservations* r;
} Tuple;

// Functions declarations
// General functions
int read_id();
void pause();
long get_file_size(char* x);

// Show menu functions
void admin(struct teachers* teachers_list, struct reservations* reservations_list);
void teacher(int id, struct teachers* teachers_list, struct reservations* reservations_list);

// Load and sava data from/to file
struct teachers *load_from_file_t(struct teachers* p);
struct reservations *load_from_file_r(struct reservations* p);
struct classrooms *load_from_file_c(struct classrooms* p);
void save_to_file_t(struct teachers* p);
void save_to_file_r(struct reservations* p);

// Admin Functions
void print_teachers(struct teachers* p);
void find_teacher(struct teachers* p);
struct teachers *add_teacher(struct teachers* p);
struct teachers *suspend_teacher(struct teachers* p);
struct teachers *del_teacher(struct teachers* p);
struct teachers *admin_change_teacher_pwd(struct teachers* p);
void print_teachers_reservations(struct reservations* p);
Tuple del_reservation(struct teachers* m, struct reservations* p);

// Teachers and Common Functions
void find_rooms(struct reservations* p);
void lookup_room(struct reservations* p);
struct teachers *change_teacher_pwd(int id, struct teachers* p);
struct reservations *new_reservation(int id, struct reservations* p);
