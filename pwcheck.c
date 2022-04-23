/**
 * @name Projekt 1  - Práce s textem
 * @author Vladimír Hucovič - xhucov00
 */

#include <stdio.h>
#include <stdlib.h>

#define PASSWORD_MAX_LENGTH 102 // password max length = 100 + '\n' + '\0'
#define ASCII_PRINTABLE_CHAR_COUNT 95

// returns length of string minus endline character if present
int getStringLen(char string[]){
    int len = 0;
    for (int i = 0; string[i] != '\0'; ++i) {
        len = i;
        if(string[i] == '\n'){
            return len;
        }
    }
    return len+1;
}

// checks if string1 == string2
int compareStrings(char str1[], char str2[]){
    if(getStringLen(str1) != getStringLen(str2)){
        return 0;
    }
    for (int i = 0; str1[i] != '\0' || str2[i] != '\0'; ++i) {
        if(str2[i] == str1[i]){
            continue;
        }
        else{
            return 0;
        }
    }
    return 1;
}

// checks if password is longer than 100
int checkPasswordLength(char password[]){
    if(password[(getStringLen(password))] == '\n'){
        return 1;
    }
    return 0;
}

// checks if char is an uppercase letter
int isUppercase(char c){
    if(c >= 'A' && c <= 'Z'){
        return 1;
    }
    return 0;
}

// checks if char is a lowercase letter
int isLowercase(char c){
    if(c >= 'a' && c <= 'z'){
        return 1;
    }
    return 0;
}

// checks if char is a number
int isNumeric(char c){
    if(c >= '0' && c <= '9'){
        return 1;
    }
    return 0;
}

// checks if char is a special char
int isSpecialChar(char c){
    if((c >= ' ' && c <= '/') || (c >= ':' && c <= '@') || (c >= '[' && c <= '`') || (c >= '{' && c <= '~')) {
        return 1;
    }
    return 0;
}

// checks level 1 conditions
int checkLevel1(char password[]){
    int lowercase = 0;
    int uppercase = 0;
    for (int i = 0; password[i] != '\0'; ++i) {
        if (isUppercase(password[i])) {
            uppercase++;
        }
        else if (isLowercase(password[i])) {
            lowercase++;
        }

        if(lowercase && uppercase){
            return 1;
        }
    }
    return 0;
}

// checks level 2 conditions
int checkLevel2(char password[], int param){
    if(!checkLevel1(password)){
        return 0;
    }
    if(param > 4) {
	param = 4;
    }                             
    int charGroups[4] = {0,0,0,0};                        // each int represents 1 group of chars
    for(int i = 0; password[i] != '\0'; ++i){
        if(isUppercase(password[i])){
            charGroups[0] = 1;
        }
        else if(isLowercase(password[i])){
            charGroups[1] = 1;
        }
        else if(isNumeric(password[i])){
            charGroups[2] = 1;
	    }
        else if(isSpecialChar(password[i])){
            charGroups[3] = 1;
        }

        if(charGroups[0] + charGroups[1] + charGroups[2] + charGroups[3] >= param){
            return 1;
        }
    }
    return 0;
}

// checks level 3 conditions
int checkLevel3(char password[], int param){
    if(!checkLevel2(password, param)){
        return 0;
    }
    if(param == 1) {
        return 0;
    }
    char currentChar = password[0];
    int identicalCharSequenceLength = 1;   // counts how long the sequence is
    for(int i = 1; password[i] != '\0'; ++i){
        if(currentChar == password[i]){
            identicalCharSequenceLength++;
            if(identicalCharSequenceLength >= param) {
                return 0;
            }
        }
        else{
            currentChar = password[i];
            identicalCharSequenceLength = 1;
        }
    }
    return 1;
}

// checks level 4 conditions
int checkLevel4(char password[], int param){
    if(!checkLevel3(password, param)){
        return 0;
    }
    int counter;
    char subString[param];
    for(int i = 0; password[i+param] != '\0'; ++i){
        for(int j = 0; j < param; ++j){       // creates substring from password with length param
            subString[j] = password[i+j];
        }
        for(int o = 0; password[o+param] != '\0'; ++o) {
            counter = 0;
            for (int k = 0; k < param; ++k) {  // checks if password contains another identical substring
                if (subString[k] == password[i + k + 1 + o]) {
                    counter++;
                } else continue;
                if (counter >= param) {
                    return 0;
                }
            }
        }
    }
    return 1;
}

// checks if third optional argument --stats is set
int isStatsSet(int argc, char *argv[]){
    if(argc == 4){
        if(compareStrings(argv[3], "--stats")){
            return 1;
        }
        return 0;
    }
    return 0;
}

// returns error if there are too many or too few parameters
int checkArgumentCount(int argc){
    // if arg count is too low, return error
    if(argc < 3){
        fprintf(stderr, "Error: Parameters LEVEL and PARAM not set\n");
        return 0;
    }

    // if arg count is too high, return error
    if(argc > 4){
        fprintf(stderr, "Error: Too many parameters\n");
        return 0;
    }
    return 1;
}

// checks if level and param are numeric
int checkIfArgumentsAreInt(char *argv[]){
	int argLen;
    for (int i = 1; i < 3; ++i) {
	    argLen = getStringLen(argv[i]);
        for (int j = 0; j < argLen; ++j) {
            if(!(argv[i][j] >= '0' && argv[i][j] <= '9')){
                fprintf(stderr, "Error: Arguments LEVEL and PARAM must be numeric!\n");
                return 0;
            }
        }
    }
    return 1;
}

// checks if entered parameters are within bounds
int checkArgumentValues(int argc, char *argv[]){
    // if LEVEL arg is out of bounds, return error
    if(atoi(argv[1]) > 4 || atoi(argv[1]) <= 0){
        fprintf(stderr, "Error: LEVEL must be in an interval between 1 and 4!\n");
        return 0;
    }
    // if PARAM is a negative number, return error
    if(atoi(argv[2]) <= 0) {
        fprintf(stderr, "Error: PARAM must be higher than 0!\n");
        return 0;
    }
    if(argc == 4){
        if(!compareStrings(argv[3], "--stats")){
            fprintf(stderr, "Error: Third argument can only be --stats\n");
            return 0;
        }
    }
    return 1;
}

// checks if arguments are valid
int checkArguments(int argc, char *argv[]){
    if(checkArgumentCount(argc) && checkIfArgumentsAreInt(argv) && checkArgumentValues(argc, argv)){
        return 1;
    }
    return 0;
}

// returns average password length
double getAvg(int sum, int count){
    if(count == 0){
        return 0;
    }
    return (double) sum/count;
}

// checks if password passes the filter with set arguments
int checkIfPasswordValid(char password[], int level, int param){
    if (level == 1) {
        if (checkLevel1(password)){
            return 1;
        }
    }
    if (level == 2) {
        if (checkLevel2(password, param)){
            return 1;
        }
    }
    if (level == 3) {
        if (checkLevel3(password, param)) {
            return 1;
        }
    }
    if(level == 4){
        if (checkLevel4(password, param)) {
            return 1;
        }
    }
    return 0;
}

// adds chars in the password to uniqueCharArray
// sets value 1 to the index equal to ascii value of the char - 32
// (so 'space' is on index 0, '!' on index 1, etc.)
int addUniqueChars(char password[], int uniqueCharArray[]){
    for (int i = 0; password[i] != '\n'; i++){
        uniqueCharArray[password[i] - ' '] = 1;
    }
    return 0;
}

// returns number of unique chars in uniqueCharArray
int getUniqueCharCount(int uniqueCharArray[]){
    int uniqueCharCount = 0;
    for (int i = 0; i < ASCII_PRINTABLE_CHAR_COUNT; ++i) {
        if(uniqueCharArray[i] == 1){
            uniqueCharCount++;
        }
    }
    return uniqueCharCount;
}

// prints stats
int printStats(int uniqueCharCount, int minLen, double avgLength){
    fprintf(stdout, "Stats:\n");
    fprintf(stdout, "Unique char count: %d\n", uniqueCharCount);
    fprintf(stdout,"Minimum length: %d\n", minLen);
    fprintf(stdout, "Average length: %.1lf\n", avgLength);
    return 0;
}

// returns minLen if password is longer, returns password length if it is shorter
int getNewMinLen(int minLen, char password[]){
    int passwordLength = getStringLen(password);
    if(minLen >= passwordLength){
        return passwordLength;
    }
    return minLen;
}


int main(int argc, char *argv[]) {
    int minLen = PASSWORD_MAX_LENGTH;
    int lengthSum  = 0;
    double avgLength = 0;
    int passwordCount = 0;
    char password[PASSWORD_MAX_LENGTH];
    int uniqueCharArray[ASCII_PRINTABLE_CHAR_COUNT] = {0};   // "boolean" array of all printable ascii chars

    // check if arguments are valid and convert them into int if true
    if (checkArguments(argc, argv)) {
        int level = atoi(argv[1]);
        int param = atoi(argv[2]);

        // get passwords until EOF or error
        while (fgets(password, PASSWORD_MAX_LENGTH, stdin) != NULL) {
            if(!checkPasswordLength(password)){
                fprintf(stderr, "Maximum password length is 100 characters!\n");
                return 1;
            }

            // print the password to stdout if it passes the filter
            if(checkIfPasswordValid(password, level, param)){
                fprintf(stdout,"%s", password);
            }

            passwordCount++;
            minLen = getNewMinLen(minLen, password);
            lengthSum += getStringLen(password);
            addUniqueChars(password, uniqueCharArray);
        }

        avgLength = getAvg(lengthSum, passwordCount);

        if(passwordCount == 0){
            minLen = 0;
        }
        // if --stats parameter is set, output stats
        if(isStatsSet(argc, argv)){
            printStats(getUniqueCharCount(uniqueCharArray), minLen, avgLength);
        }
    }
    else{
        return 1;
    }
    return 0;
}
