/*
SIETE: Student
EVERYTHING IS TENTATIVE
*/
#include "StudentsAndClasses.h"
//=========Menu Functions======
void addStudent(STUDENT **studentHead, CLASS **classHead, FILE *fp) {
    char firstName[100], lastName[100], stdNum[100];
    int units, studentHasDup = 0;
    STUDENT *ptr = NULL;

    askStdNum(stdNum);
    hasStudentDuplicate(studentHead, stdNum, &studentHasDup);

    if(studentHasDup == 1) invalid();
    else {
        askStudentInformation(firstName, lastName, &units);
        insertAtStudentHead(studentHead, createNewStudent(firstName, lastName, stdNum, units, units), classHead, 0);
    }
}

void addClass(CLASS **classHead, FILE *fp) {
    char courseCode[10], section[10];
    int units, maxSize, remainingSlots, classHasDup = 0;
    CLASS *ptr = NULL;

    askCourseCode(courseCode);
    askSection(section);
    hasClassDuplicate(classHead, courseCode, section, &classHasDup);

    if(classHasDup == 1) invalid();
    else {
        askClassInformation(&units, &maxSize);
        insertAtClassHead(classHead, createNewClass(courseCode, section, units, maxSize, maxSize));
    }
}

void viewSpecificStudent(STUDENT **studentHead, char *stdNum) {
    if((*studentHead)->next != NULL) {
        STUDENT *toView = searchStudent(studentHead, stdNum);
        if(toView == NULL) notFound();
        else printStudentInfo(toView);
    }
    else emptyList();
}

void viewAllStudents(STUDENT **studentHead, int inUnderClass, int inInsufficientUnits) {
    if((*studentHead)->next != NULL) {
        char courseCode[10], section[10];
        STUDENT *toView = NULL;
        if(inUnderClass == 1) {
            askCourseCode(courseCode);
            askSection(section);
        }
       for(toView = (*studentHead)->next; toView != NULL; toView = toView->next){
            if(inUnderClass == 1) {
                int inClasslist = deleteClassesinCLasslist(&toView->classlistHead, courseCode, section, 0);
                if(inClasslist == 1) printStudentInfo(toView);
            }
            else if(inInsufficientUnits == 1) {
                if(toView->remainingUnits != 0) printStudentInfo(toView);
            }
            else printStudentInfo(toView);
       }
    } else emptyList();
}

void viewStudentsUnderClass(STUDENT **studentHead) {
    viewAllStudents(studentHead, 1, 0);
}

void viewStudentsWithInsufficientUnits(STUDENT **studentHead) {
    viewAllStudents(studentHead, 0, 1);
}

void viewSpecificClass(CLASS **classHead, char *courseCode, char *section) {
    if((*classHead)->next != NULL) {
        CLASS *toView = searchClass(classHead, courseCode, section);
        if(toView == NULL) notFound();
        else printClassInfo(toView);
    }
    else emptyList();
}

void viewAllClasses(CLASS **classHead, int inAvailable) {
    if((*classHead)->next != NULL) {
        CLASS *toView = NULL;
        for(toView = (*classHead)->next; toView != NULL; toView = toView->next){
            if(inAvailable == 1) {
                if(toView->remainingSlots != 0) printClassInfo(toView);
            } else printClassInfo(toView);
        }
    }
    else emptyList();
}

void viewAvailableClasses(CLASS **classHead) {
    viewAllClasses(classHead, 1);
}

void deleteStudent(STUDENT **studentHead, CLASS **classHead, char *stdNum) {
    if((*studentHead)->next != NULL) {
        STUDENT *studentToUpdate = searchStudent(studentHead, stdNum);
        if (studentToUpdate != NULL) {
            updateClasses(studentToUpdate->classlistHead, classHead);
            printf("CLASSES UPDATED. \n");
            destroyClasslist(&studentToUpdate->classlistHead);
            deleteSpecificStudent(studentHead, stdNum);
        }
        else notFound();
    }
    else emptyList();
}

void deleteClass(CLASS **classHead, STUDENT **studentHead, char *courseCode, char *section) {
    if((*classHead)->next != NULL) {
        CLASS *findClass = searchClass(classHead, courseCode, section);
        if(findClass != NULL) {
            updateStudentsWithClass(studentHead, courseCode, section, findClass->units);
            deleteSpecificClass(classHead, courseCode, section);
        } else notFound();
    }
    else printf("List is empty.\n");
}

void editClass(CLASS **classHead, char *courseCode, char *section) {
    if((*classHead)->next != NULL) {
        CLASS *classToEdit = searchClass(classHead, courseCode, section);
        if(classToEdit != NULL) {
            int unavailableSlots = (classToEdit->maxSize - classToEdit->remainingSlots), hasDuplicate = 1;
            printf("EDIT CLASS INFORMATION: \n");
            if(classToEdit->remainingSlots == classToEdit->maxSize) {
                char newCourseCode[10], newSection[10];
                while(hasDuplicate == 1) {
                        printf("COURSE CODE: ");
                        fgets(courseCode, 10, stdin);
                        courseCode[strlen(courseCode) - 1] = '\0';
                        askSection(newSection);
                        CLASS *ptr1 = NULL;
                        for(ptr1 = (*classHead)->next; ptr1 != NULL; ptr1 = ptr1->next) {
                            if(strcasecmp(ptr1->courseCode, newCourseCode) == 0 && strcasecmp(ptr1->section, newSection) == 0) break;
                            else if(ptr1->next == NULL) hasDuplicate = 0;
                        }
                }
                    strcpy(classToEdit->courseCode, newCourseCode);
                    strcpy(classToEdit->section, newSection);
                    printf("Units: ");
                    scanf("%i", &classToEdit->units);
            }
            printf("Maximum class size: ");
            scanf("%i", &classToEdit->maxSize);
            classToEdit->remainingSlots = classToEdit->maxSize - unavailableSlots;

        } else notFound();
    } else emptyList();
}

void enrollOrUnenrollStudentsToClasses(STUDENT **studentHead, CLASS **classHead, int inUnenroll) {
    char stdNum[100];
    int answer = 0;
    if((*studentHead)->next != NULL) {
        askStdNum(stdNum);
        STUDENT *studentToEnroll = searchStudent(studentHead, stdNum);
        if(studentToEnroll != NULL) {
            if(inUnenroll == 1) {
                printf("UNENROLL CLASSES? [1]YES [0] N0 : ");
                scanf("%i", &answer);
                if(answer == 1) {
                    char courseCode[10], section[10];
                    do {
                       askCourseCode(courseCode);
                       askSection(section);
                       int classDeleted = deleteClassesinCLasslist(&studentToEnroll->classlistHead, courseCode, section, 1);
                       if(classDeleted == 1) {
                        CLASS *classToUpdate = searchClass(classHead, courseCode, section);
                        updateClass(classToUpdate);
                        studentToEnroll->remainingUnits = studentToEnroll->remainingUnits + classToUpdate->units;
                        printf("CLASS UNENROLLED\n");
                       } else invalid();
                       printf("UNENROLL ANOTHER CLASS? [1] YES [0] NO\n");
                       scanf("%i", &answer);
                    } while (answer != 0);
                }
            } else {
                printf("ENROLL IN CLASSES? [1]YES [0] N0 : ");
                scanf("%i", &answer);
                if(answer == 1) enrollCourses(&studentToEnroll->classlistHead, classHead, &studentToEnroll->remainingUnits);
            }
        } else notFound();
    } else emptyList();
}

//-----------------------------
//=========Dashboard===========
void askChoice(int *choice) {
	printf("Enter choice: ");
	scanf("%i", choice);
	printf("==========\n");
	printf("\n");
}

void viewStudentsDB(int *choice) {
    printf("==========\n");
	printf("[1] View All Students\n");
	printf("[2] View Specific Student\n");
	printf("[3] View Students Under Class\n");
	printf("[4] View Students with insufficient units\n");
	printf("[0] Back to Main Menu\n");
    askChoice(choice);
}

void viewClassDB(int *choice) {
    printf("==========\n");
	printf("[1] View All Classes\n");
	printf("[2] View Specific Class\n");
	printf("[3] View Available Classes\n");
	printf("[0] Back to Main Menu\n");
	askChoice(choice);
}

void mainMenuDB(int *choice) {
    printf("==========\n");
	printf("[1] Add a Student\n");
	printf("[2] Add a Class\n");
	printf("[3] View Students\n");
	printf("[4] View Classes\n");
	printf("[5] Delete Student\n");
	printf("[6] Delete Class\n");
	printf("[7] Edit Class Details\n");
    printf("[8] Enroll Students to Classes\n");
    printf("[9] Unenroll Students to Classes\n");
	printf("[0] Exit\n");
	askChoice(choice);
}
//-----------------------------
//=============================
void mainMenu(CLASS **classHead, STUDENT **studentHead, FILE *fp) {
	int choice;
	char courseCode[100], stdNum[100], section[10];

	do {
        loadClassesTxt(fp, classHead);
        loadStudentsTxt(fp, studentHead, classHead);
        mainMenuDB(&choice);

        if(choice == 1) addStudent(studentHead, classHead, fp);
        if(choice == 2) addClass(classHead, fp);
		if(choice == 3) {
            do {
                viewStudentsDB(&choice);
                if(choice == 1) viewAllStudents(studentHead, 0, 0);
                if(choice == 2) {
                    askStdNum(stdNum);
                    viewSpecificStudent(studentHead, stdNum);
                }
                if(choice == 3) viewStudentsUnderClass(studentHead);
                if(choice == 4) viewStudentsWithInsufficientUnits(studentHead);
            } while (choice != 0);
            mainMenu(classHead, studentHead, fp);
		}
		if(choice == 4) {
            do {
                viewClassDB(&choice);
                if(choice == 1) viewAllClasses(classHead, 0);
                if(choice == 2) {
                    askCourseCode(courseCode);
                    askSection(section);
                    viewSpecificClass(classHead, courseCode, section);
                }
                if(choice == 3) viewAvailableClasses(classHead);
            } while (choice != 0);
            mainMenu(classHead, studentHead, fp);
		}
		if(choice == 5) {
            askStdNum(stdNum);
            deleteStudent(studentHead, classHead, stdNum);
		}
		if(choice == 6) {
            askCourseCode(courseCode);
            askSection(section);
            deleteClass(classHead, studentHead, courseCode, section);
		}
		if(choice == 7) {
            askCourseCode(courseCode);
            askSection(section);
            editClass(classHead, courseCode, section);
		}
        if(choice == 8) enrollOrUnenrollStudentsToClasses(studentHead, classHead, 0);
        if(choice == 9) enrollOrUnenrollStudentsToClasses(studentHead, classHead, 1);
        saveToStudentsTxt(fp, studentHead);
        saveToClassesTxt(fp,classHead);
	} while (choice != 0);
}
//-----------------------------
