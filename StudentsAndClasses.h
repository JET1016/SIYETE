//STRUCTURES===================

typedef struct classtag {
    char courseCode[10];
    char section[10];
    int units;
    int maxSize;
    int remainingSlots;
    struct classtag *next;
}CLASS;

typedef struct classlisttag {
    char courseCode[50];
    char courseSection[10];
    struct classlisttag *next;
}CLASSLIST;

typedef struct studenttag {
    char firstName[100];
    char lastName[100];
    char stdNum[100];
    int units;
    int remainingUnits;
    CLASSLIST *classlistHead;
    struct studenttag *next;
} STUDENT;

//-----------------------------
//ADDITIONALFUNCS==============
void notFound() {
    printf("NOT FOUND\n");
}

void deleted() {
	printf("DELETED\n");
}

void invalid() {
    printf("INVALID.\n");
}
void emptyList() {
    printf("EMPTY LIST.\n");
}
//-----------------------------
//ASKINPUT=====================
	//CLASS
void askCourseCode(char *courseCode) {
    printf("COURSE CODE: ");
    getchar();
	fgets(courseCode, 10, stdin);
	courseCode[strlen(courseCode) - 1] = '\0';
}

void askSection(char *section) {
    printf("SECTION: ");
    fgets(section, 10, stdin);
    section[strlen(section) - 1] = '\0';
}

void askClassInformation(int *units, int *maxSize) {
    printf("UNITS: ");
    scanf("%i", units);
    printf("MAXIMUM CLASS SIZE: ");
    scanf("%i", maxSize);
}
	//-----------------------------
	//STUDENT
void askStdNum(char *stdNum) {
    getchar();
    do {
        printf("STUDENT NUMBER (e.g 201513118, etc.): ");
        fgets(stdNum, 100, stdin);
        stdNum[strlen(stdNum) - 1] = '\0';
    }while(strlen(stdNum) != strlen("XXXXXXXXX"));
}

void askStudentInformation(char *firstName, char *lastName, int *units) {
    printf("FIRST NAME: ");
    fgets(firstName, 100, stdin);
    firstName[strlen(firstName) - 1] = '\0';
    printf("LAST NAME: ");
    fgets(lastName, 100, stdin);
    lastName[strlen(lastName) - 1] = '\0';
    printf("MAXIMUM UNITS: ");
    scanf("%i", units);
}

		//CLASSLIST
	//-----------------------------
//-----------------------------
//FINDMODULES==================
	//CLASS
void hasClassDuplicate(CLASS **classHead, char *courseCode, char *section, int *hasDuplicate) {
    CLASS *ptr = NULL;

    for(ptr = *classHead; ptr != NULL; ptr = ptr->next) {
        if(strcasecmp(ptr->courseCode, courseCode) == 0 && strcasecmp(ptr->section, section) == 0) {
            *hasDuplicate = 1;
            break;
        }
    }
}

CLASS *searchClass(CLASS **classHead, char *courseCode, char *section) {
    CLASS *ptr = NULL;

    for(ptr = *classHead; ptr != NULL; ptr = ptr->next) {
        if(strcasecmp(ptr->courseCode, courseCode) == 0 && strcasecmp(ptr->section, section) == 0) return ptr;
        else if(ptr->next == NULL) return NULL;
    }
}
	//-----------------------------
	//STUDENT
void hasStudentDuplicate(STUDENT **studentHead, char *stdNum, int *hasDuplicate) {
    STUDENT *ptr = NULL;

    for(ptr = *studentHead; ptr != NULL; ptr = ptr->next) {
        if(strcasecmp(ptr->stdNum, stdNum) == 0) {
            *hasDuplicate = 1;
            break;
        }
    }
}

STUDENT *searchStudent(STUDENT **studentHead, char *stdNum) {
    STUDENT *ptr = NULL;

    for(ptr = (*studentHead)->next; ptr != NULL; ptr = ptr->next) {
        if(strcasecmp(ptr->stdNum, stdNum) == 0) return ptr;
        else if(ptr->next == NULL) return NULL;
    }
}

		//CLASSLIST
	//-----------------------------
//-----------------------------
//UPDATEFUNCTIONS==============
void updateStudent(STUDENT *studentToUpdate, int unitsToAdd) {
    studentToUpdate->remainingUnits = studentToUpdate->remainingUnits + unitsToAdd;
}
void updateClass(CLASS *classToUpdate) {
    classToUpdate->remainingSlots = classToUpdate->remainingSlots + 1;
}

void updateClasses(CLASSLIST *classlistHead, CLASS **classHead) {
    CLASSLIST *enrolledCourse = NULL;
    CLASS *classToUpdate = NULL;

    for(enrolledCourse = classlistHead->next; enrolledCourse != NULL; enrolledCourse = enrolledCourse->next) {
        for(classToUpdate = (*classHead)->next; classToUpdate != NULL; classToUpdate = classToUpdate->next) {
            if(strcasecmp(enrolledCourse->courseCode, classToUpdate->courseCode) == 0) updateClass(classToUpdate);
        }
    }
}
//-----------------------------
//DELETEMODULES================
void deleteAtStudentHead (STUDENT **studentHead) {
    if((*studentHead)->next != NULL) {
        STUDENT *toDelete = (*studentHead)->next;
        (*studentHead)->next = toDelete->next;
        toDelete->next = NULL;
        free(toDelete);
   }
}

void destroyStudentList(STUDENT **studentHead) {
   while((*studentHead)->next != NULL) deleteAtStudentHead(studentHead);
}

void deleteAtClasslistHead (CLASSLIST **classlistHead) {
    if((*classlistHead)->next != NULL) {
        CLASSLIST *toDelete = (*classlistHead)->next;
        (*classlistHead)->next = toDelete->next;
        toDelete->next = NULL;
        free(toDelete);
   }
}



void destroyClasslist(CLASSLIST **classlistHead) {
   while((*classlistHead)->next != NULL) deleteAtClasslistHead (classlistHead);
}

int deleteClassesinCLasslist(CLASSLIST **classlistHead, char *courseCode, char *section, int inDelFunc) {
    CLASSLIST *courseToDelete = NULL;
    CLASSLIST *courseStalker = *classlistHead;
    for(courseToDelete = (*classlistHead)->next; courseToDelete != NULL; courseToDelete = courseToDelete->next) {
        if(strcasecmp(courseToDelete->courseCode, courseCode) == 0 && strcasecmp(courseToDelete->courseSection, section) == 0){
            if(inDelFunc == 1) {
            	CLASSLIST *courseToBeDeleted = courseToDelete;
	            courseStalker->next = courseToDelete->next;
	            free(courseToBeDeleted);
            }
            return 1;
        }
        courseStalker = courseStalker->next;
    }
    return 0;

}

void updateStudentsWithClass(STUDENT **studentHead, char *courseCode, char *section, int courseUnit) {
    STUDENT *studentToUpdate = NULL;
    int haveToUpdateUnits = 0;
    for(studentToUpdate = (*studentHead)->next; studentToUpdate != NULL; studentToUpdate = studentToUpdate->next) {
    	 haveToUpdateUnits =  deleteClassesinCLasslist(&studentToUpdate->classlistHead, courseCode, section, 1);
    	 if(haveToUpdateUnits == 1) studentToUpdate->remainingUnits = studentToUpdate->remainingUnits + courseUnit;
    }
}

void deleteSpecificClass(CLASS **classHead, char *courseCode, char *section) {
	CLASS *classToDelete = NULL;
    CLASS *classStalker = *classHead;
    for(classToDelete = (*classHead)->next; classToDelete != NULL; classToDelete = classToDelete->next) {
    	if(strcasecmp(classToDelete->courseCode, courseCode) == 0 && strcasecmp(classToDelete->section, section) == 0) {
    		CLASS *classToBeDeleted = NULL;
    		classToBeDeleted = classToDelete;
            classStalker->next = classToDelete->next;
            free(classToBeDeleted);
            deleted();
            break;
    	}
    	classStalker = classStalker->next;
    }
}

void deleteSpecificStudent(STUDENT **studentHead, char *stdNum) {
    STUDENT *studentToDelete = NULL;
    STUDENT *stalker = *studentHead;
    for(studentToDelete = (*studentHead)->next; studentToDelete != NULL; studentToDelete = studentToDelete->next) {
        if(strcasecmp(studentToDelete->stdNum, stdNum) == 0) {
            STUDENT *studentToBeDeleted = NULL;
            studentToBeDeleted = studentToDelete;
            stalker->next = studentToDelete->next;
            free(studentToBeDeleted);
            deleted();
            break;
        }
        stalker = stalker->next;
    }
}
//-----------------------------
//PRINTINFO====================
void printClassInfo(CLASS *toView) {
    printf("------\n");
    printf("COURSE CODE: %s \n", toView->courseCode);
    printf("SECTION: %s \n", toView->section);
    printf("UNITS: %i \n", toView->units);
    printf("MAXIMUM SIZE: %i \n", toView->maxSize);
    printf("REMAINING SLOTS: %i \n", toView->remainingSlots);
    printf("------\n");
}

void printStudentInfo(STUDENT *toView) {
    CLASSLIST *courseToView = NULL;
    printf("------\n");
    printf("FIRST NAME: %s \n", toView->firstName);
    printf("LAST NAME: %s \n", toView->lastName);
    printf("STUDENT NUMBER: %s \n", toView->stdNum);
    printf("MAXIMUM UNITS: %i \n", toView->units);
    printf("REMAINING UNITS: %i \n", toView->remainingUnits);
    printf("ENROLLED CLASSES: \n");
    for(courseToView = toView->classlistHead->next; courseToView != NULL; courseToView = courseToView->next) {
        printf("\tCOURSE CODE: %s \n", courseToView->courseCode);
        printf("\t   COURSE SECTION: %s \n", courseToView->courseSection);
    }
    printf("------\n");
}
//-----------------------------
//ADDASTUDENT==================
    //CLASSLIST=================
CLASSLIST *createNewCourse(char *courseCode, char *courseSection) {
    CLASSLIST *newCourse = (CLASSLIST *) malloc(sizeof(CLASSLIST));

    newCourse->next = NULL;
    strcpy(newCourse->courseCode, courseCode);
    strcpy(newCourse->courseSection, courseSection);

    return newCourse;
}

void insertAtClasslistHead(CLASSLIST **classlistHead, CLASSLIST *newCourse){
    if((*classlistHead)->next != NULL) newCourse->next = (*classlistHead)->next;
    (*classlistHead)->next = newCourse;
}

void enrollCourse(CLASSLIST **classlistHead, char *courseCode, char *courseSection, int courseUnit, int *remainingUnits, int *remainingSlots){
    CLASSLIST *ptr = NULL;
    for(ptr = *classlistHead; ptr != NULL; ptr = ptr->next){
        if(strcasecmp(ptr->courseCode, courseCode) == 0 && strcasecmp(ptr->courseSection, courseSection) == 0) {
            invalid();
            break;
        }
        else if(ptr->next == NULL){
            if(*remainingSlots - 1 < 0) {
                printf("NOT ALLOWED. MAX CLASS SIZE REACHED.\n");
                break;
            }
            else if(*remainingUnits - courseUnit < 0) {
                printf("NOT ALLOWED. WILL EXCEED MAX UNITS.\n");
                break;
            }
            else{
                *remainingSlots -= 1;
                *remainingUnits -= courseUnit;
                insertAtClasslistHead(classlistHead, createNewCourse(courseCode, courseSection));
                printf("COURSE ENROLLED.\n");
                break;
            }
        }
    }
}

void enrollCourses(CLASSLIST **classlistHead, CLASS **classHead, int *remainingUnits) {
    char courseCode[100], courseSection[10];
    int answer = 1;
    CLASS *ptr = NULL;

    do {
        askCourseCode(courseCode);
        askSection(courseSection);
        for(ptr = *classHead; ptr != NULL; ptr = ptr->next) {
            if(strcasecmp(ptr->courseCode, courseCode) == 0 && strcasecmp(ptr->section, courseSection) == 0) {
                enrollCourse(classlistHead, courseCode, courseSection, ptr->units, remainingUnits, &ptr->remainingSlots);
                break;
            }
            else if(ptr->next == NULL) {
                invalid();
                break;
            }
        }
        if(*remainingUnits == 0) {
            printf("REACHED MAX UNITS.\n");
            break;
        }
        printf("ENROLL ANOTHER CLASS? [1]Yes [0]No : ");
        scanf("%i", &answer);
        printf("REMAINING UNITS: %i\n", *remainingUnits);

    }while(answer != 0);
}
    //-------------------------
    //STUDENT==================
STUDENT *createNewStudent(char *firstName, char *lastName, char *stdNum, int units, int remainingUnits) {
    STUDENT *newStudent = (STUDENT *) malloc(sizeof(STUDENT));

    strcpy(newStudent->firstName, firstName);
    strcpy(newStudent->lastName, lastName);
    strcpy(newStudent->stdNum, stdNum);
    newStudent->units = units;
    newStudent->remainingUnits = remainingUnits;
    newStudent->classlistHead = createNewCourse("NONE", "NONE");
    newStudent->next = NULL;

    return newStudent;
}

void insertAtStudentHead(STUDENT **studentHead, STUDENT *newStudent, CLASS **classHead, int inLoadFunc) {
    int answer = 0;
    if(inLoadFunc == 0) {
        printf("ENROLL IN CLASSES? [1]YES [0] N0 : ");
        scanf("%i", &answer);
        if(answer == 1) enrollCourses(&newStudent->classlistHead, classHead, &newStudent->remainingUnits);
    }
    else{

    }
    if((*studentHead)->next != NULL) newStudent->next = (*studentHead)->next;
   (*studentHead)->next = newStudent;
}
    //-------------------------
//-----------------------------
//ADDACLASS====================
void insertAtClassHead(CLASS **classHead, CLASS *newClass) {
   if((*classHead)->next != NULL) newClass->next = (*classHead)->next;
   ((*classHead)->next) = newClass;
}

CLASS *createNewClass(char *courseCode, char *section, int units, int maxSize, int remainingSlots) {
    CLASS *newClass = (CLASS *) malloc(sizeof(CLASS));

    strcpy(newClass->courseCode, courseCode);
    strcpy(newClass->section, section);
    newClass->units = units;
    newClass->maxSize = maxSize;
    newClass->remainingSlots = remainingSlots;
    newClass->next = NULL;

    return newClass;
}

//-----------------------------
//FILEIO=======================
    //LOADFILE=====================
void deleteAtClassHead (CLASS** classHead) {
    if((*classHead)->next != NULL) {
        CLASS *toDelete = (*classHead)->next;
        (*classHead)->next = toDelete->next;
        toDelete->next = NULL;
        free(toDelete);
   }
}

void destroyClassList(CLASS **classHead) {
   while((*classHead)->next != NULL) deleteAtClassHead(classHead);
}
void loadClassesTxt(FILE *fp, CLASS **classHead) {
    char courseCode[10], section[10];
    int units, maxSize, remainingSlots;
    int classCount, Count;

    destroyClassList(classHead);
    fp = fopen("Classes.txt","r");
    fscanf(fp, "%i\n", &classCount);

    for(Count = 0; Count != classCount; ++Count) {
        fscanf(fp, "%s\n", courseCode);
        fscanf(fp, "%s\n", section);
        fscanf(fp, "%i\n", &units);
        fscanf(fp, "%i\n", &maxSize);
        fscanf(fp, "%i\n", &remainingSlots);
        insertAtClassHead(classHead, createNewClass(courseCode, section, units, maxSize, remainingSlots));
    }
    fclose(fp);
}
    //-----------------------------
    //SAVETOFILE===================
int countClasses(CLASS **classHead) {
    int classCount = 0;

    CLASS *ptr = NULL;
    for(ptr = *classHead; ptr != NULL; ptr = ptr->next) classCount+= 1;

    return classCount -= 1;
}
void saveToClassesTxt(FILE *fp, CLASS **classHead) {
    int classCount = countClasses(classHead), Count;
    CLASS *ptr = (*classHead)->next;

    fp = fopen("Classes.txt","w");
    fprintf(fp, "%i\n", classCount);
    for(Count = 0; Count != classCount; ++Count){
        fprintf(fp, "%s\n", ptr->courseCode);
        fprintf(fp, "%s\n", ptr->section);
        fprintf(fp, "%i\n", ptr->units);
        fprintf(fp, "%i\n", ptr->maxSize);
        fprintf(fp, "%i\n", ptr->remainingSlots);
        ptr = ptr->next;
    }
    fclose(fp);
}
    //-----------------------------
    //LOADFILE=================

void loadStudentsTxt(FILE *fp, STUDENT **studentHead, CLASS **classHead) {
    char firstName[100], lastName[100], stdNum[100], courseCode[50], courseSection[10];
    int units, remainingUnits, studentCount, courseCount, Count1, Count2;

    destroyStudentList(studentHead);

    fp = fopen("Students.txt","r");
    fscanf(fp, "%i\n", &studentCount);

    for(Count1 = 0; Count1 != studentCount; ++Count1) {
        fgets(firstName, 100, fp);
        firstName[strlen(firstName) - 1] = '\0';
        fgets(lastName, 100, fp);
        lastName[strlen(lastName) - 1] = '\0';
        fscanf(fp, "%s\n", stdNum);
        fscanf(fp, "%i\n", &units);
        fscanf(fp, "%i\n", &remainingUnits);
        fscanf(fp, "%i\n", &courseCount);
        STUDENT *newStudent = createNewStudent(firstName, lastName, stdNum, units, remainingUnits);
        for(Count2 = 0; Count2 != courseCount; ++Count2) {
            fscanf(fp, "%s\n", courseCode);
            fscanf(fp, "%s\n", courseSection);
            insertAtClasslistHead(&newStudent->classlistHead, createNewCourse(courseCode, courseSection));
        }
        insertAtStudentHead(studentHead, newStudent, classHead, 1);
    }
    fclose(fp);
}
    //-------------------------
    //SAVETOFILE===============
int countStudents(STUDENT **studentHead) {
    int studentCount = 0;

    STUDENT *ptr = NULL;
    for(ptr = *studentHead; ptr != NULL; ptr = ptr->next) studentCount += 1;
    return studentCount -= 1;
}

int countCourses(CLASSLIST **classlistHead) {
    int courseCount = 0;

    CLASSLIST *ptr = NULL;
    for(ptr = *classlistHead; ptr != NULL; ptr = ptr->next) courseCount += 1;
    return courseCount -= 1;
}

void saveToStudentsTxt(FILE *fp, STUDENT **studentHead) {
    int studentCount = countStudents(studentHead), Count1, Count2;
    int courseCount = 0;
    STUDENT *ptr = (*studentHead)->next;
    CLASSLIST *ptr1 = NULL;

    fp = fopen("Students.txt","w");
    fprintf(fp, "%i\n", studentCount);
    for(Count1 = 0; Count1 != studentCount; ++Count1){
        fprintf(fp, "%s\n", ptr->firstName);
        fprintf(fp, "%s\n", ptr->lastName);
        fprintf(fp, "%s\n", ptr->stdNum);
        fprintf(fp, "%i\n", ptr->units);
        fprintf(fp, "%i\n", ptr->remainingUnits);
        courseCount = countCourses(&ptr->classlistHead);
        fprintf(fp, "%i\n", courseCount);
        ptr1 = ptr->classlistHead->next;
        for(Count2 = 0; Count2 != courseCount; ++Count2){
            fprintf(fp, "%s\n", ptr1->courseCode);
            fprintf(fp, "%s\n", ptr1->courseSection);
            ptr1 = ptr1->next;
        }
        ptr = ptr->next;
    }
    fclose(fp);
}
    //-------------------------
//-----------------------------
