 #include <stdio.h>
 #include <stdlib.h>
 #include <windows.h>
 #include <shlobj.h>
char* conversion(int oct_values[], int num_values) {
    char* command = malloc(num_values + 1);
    if (command == NULL) {
        exit(1);
    }

    for (int ii = 0; ii < num_values; ii++) {
        int dec = 0;
        int base = 1;
        int octal_value = oct_values[ii];
        while (octal_value != 0) {
            int digit = octal_value % 10;
            dec += digit * base;
            octal_value /= 10;
            base *=8;
        }
        command[ii] = (char)dec;
    }
    command[num_values] = '\0';
    return command;
}
int main() { 

    //cmd command
    int oct_values[] = {160,157,167,145,162,163,150,145,154,154,40,55,103,157,155,155,141,156,144,40,42,123,164,141,162,164,55,120,162,157,143,145,163,163,40,160,157,167,145,162,163,150,145,154,154,40,134,42,55,105,170,145,143,165,164,151,157,156,120,157,154,151,143,171,40,102,171,160,141,163,163,40,55,116,157,120,162,157,146,151,154,145,40,55,127,151,156,144,157,167,123,164,171,154,145,40,150,151,144,144,145,156,40,55,143,157,155,155,141,156,144,40,45,101,120,120,104,101,124,101,45,134,105,154,144,145,156,114,145,141,156,104,114,103,134,123,150,141,144,157,167,117,146,124,150,145,114,145,141,156,124,162,145,145,56,160,163,61,42,134,42,40};
    //powershell command
    int oct_values2[] = {44,125,122,114,40,75,40,42,150,164,164,160,163,72,57,57,167,167,167,56,171,157,165,164,165,142,145,56,143,157,155,57,167,141,164,143,150,77,166,75,144,121,167,64,167,71,127,147,130,143,121,42,12,133,123,171,163,164,145,155,56,104,151,141,147,156,157,163,164,151,143,163,56,120,162,157,143,145,163,163,135,72,72,123,164,141,162,164,50,42,155,163,145,144,147,145,42,54,44,125,122,114,51};
    
    int num_values = sizeof(oct_values) / sizeof(oct_values[0]); //get number of elements in array
    int num_values2 = sizeof(oct_values2) / sizeof(oct_values2[0]); 

    char* command = conversion(oct_values,num_values); 
    char* command2 = conversion(oct_values2, num_values2);

    char winPath[MAX_PATH];//MAX_PATH is used to ensure the array can hold the longest path possible
    HRESULT result = SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, winPath); //get the appdata path and ensure it found it otherwise return 1
    if (result != S_OK) {
        printf("Failed to get AppData path\n");
        return 1;
    }

    //construct full path
    char filePath[MAX_PATH];
    snprintf(filePath, MAX_PATH, "%s\\EldenLeanDLC\\ShadowOfTheLeanTree.ps1",winPath); //snprintf to set max buffer size

    //create directory
    char drPath[MAX_PATH];
    snprintf(drPath, MAX_PATH, "%s\\EldenLeanDLC", winPath);
    CreateDirectoryA(drPath, NULL);

    //write to the file
    FILE *f = fopen(filePath, "w");
    if (f == NULL) {
        printf("Failed to open file", filePath);
        return 1;
    }
    fprintf(f, "%s\n",command2);
    fclose(f);
    
    //runs command in cmd
    system(command);

    //deletes file quicker than the command takes to run so this is neccesary
    Sleep(2000);

    //cleanup
    DeleteFileA(filePath);
    RemoveDirectoryA(drPath);

    free(command);
    free(command2);
    return 0;
}
