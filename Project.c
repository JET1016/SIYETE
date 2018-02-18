/*
-Jem E. Torres
-2015-13118
SIETE: Student
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Project.h"
#include "display.h"

int main() {
    FILE *fp;
    CLASS *classHead = createNewClass("NONE", "NONE", 0, 0, 0);
    STUDENT *studentHead = createNewStudent("NONE", "NONE", "NONE", 0, 0);

    displayStart();
	mainMenu(&classHead, &studentHead, fp);
	displayEnd();

	free(classHead);
	free(studentHead);
	return 0;
}