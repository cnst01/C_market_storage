#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FOODS 100

typedef struct {
    char name[100];
    double price;
    int quantity;
} Food;

void displayMenu() {
    printf("\nFood Options Menu:\n");
    printf("1. View Food Options\n");
    printf("2. Add Food Option\n");
    printf("3. Edit Food Option\n");
    printf("4. Delete Food Option\n");
    printf("5. Add Food Options from CSV\n");
    printf("6. Exit\n");
    printf("Enter your choice: ");
}

int readFoodData(Food foods[], int *count) {
    FILE *file = fopen("fooddata.csv", "r");
    if (file == NULL) {
        return 0;
    }

    while (*count < MAX_FOODS && fscanf(file, "%99[^,],%lf,%d\n", foods[*count].name, &foods[*count].price, &foods[*count].quantity) == 3) {
        (*count)++;
    }

    fclose(file);
    return 1;
}

void writeFoodData(Food foods[], int count) {
    FILE *file = fopen("fooddata.csv", "w");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(file, "%s,%.2lf,%d\n", foods[i].name, foods[i].price, foods[i].quantity);
    }

    fclose(file);
}

int addFoodOptionsFromCSV(Food foods[], int *count, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return 0;
    }

    while (*count < MAX_FOODS && fscanf(file, "%99[^,],%lf,%d\n", foods[*count].name, &foods[*count].price, &foods[*count].quantity) == 3) {
        (*count)++;
    }

    fclose(file);
    return 1;
}

int main() {
    Food foods[MAX_FOODS];
    int foodCount = 0;

    if (!readFoodData(foods, &foodCount)) {
        printf("Error reading data from file.\n");
        return 1;
    }

    int choice;
    char buffer[100];

    while (1) {
        displayMenu();
        scanf("%d", &choice);
        getchar(); // Consume newline character

        switch (choice) {
            case 1:
                printf("\nFood Options:\n");
                for (int i = 0; i < foodCount; i++) {
                    printf("%s - $%.2lf - Quantity: %d\n", foods[i].name, foods[i].price, foods[i].quantity);
                }
                break;
            
            case 2:
                if (foodCount < MAX_FOODS) {
                    printf("Enter food name: ");
                    fgets(foods[foodCount].name, sizeof(foods[foodCount].name), stdin);
                    foods[foodCount].name[strlen(foods[foodCount].name) - 1] = '\0'; // Remove newline
                    printf("Enter food price: ");
                    fgets(buffer, sizeof(buffer), stdin);
                    sscanf(buffer, "%lf", &foods[foodCount].price);
                    printf("Enter food quantity: ");
                    scanf("%d", &foods[foodCount].quantity);
                    foodCount++;
                    writeFoodData(foods, foodCount);
                    printf("Food option added successfully.\n");
                } else {
                    printf("Maximum number of food options reached.\n");
                }
                break;
            
            case 3:
                printf("Enter the name of the CSV file to import food options from: ");
                char filename[100];
                scanf("%99s", filename);
                getchar(); // Consume newline character

                if (addFoodOptionsFromCSV(foods, &foodCount, filename)) {
                    printf("Food options imported successfully from %s.\n", filename);
                    writeFoodData(foods, foodCount);
                } else {
                    printf("Error importing food options from %s. Please check the file.\n", filename);
                }
                break;

            case 4:
                // ... (code for editing food option)
                break;

            case 5:
                // ... (code for deleting food option)
                printf("Enter the index of the food option to edit (0-%d): ", foodCount - 1);
                int index;
                scanf("%d", &index);
                getchar(); // Consume newline character
                if (index >= 0 && index < foodCount) {
                    printf("Enter new food name: ");
                    fgets(foods[index].name, sizeof(foods[index].name), stdin);
                    foods[index].name[strlen(foods[index].name) - 1] = '\0'; // Remove newline
                    printf("Enter new food price: ");
                    fgets(buffer, sizeof(buffer), stdin);
                    sscanf(buffer, "%lf", &foods[index].price);
                    printf("Enter new food quantity: ");
                    scanf("%d", &foods[index].quantity);
                    writeFoodData(foods, foodCount);
                    printf("Food option edited successfully.\n");
                } else {
                    printf("Invalid index.\n");
                }
                break;

            case 6:
                printf("Enter the index of the food option to delete (0-%d): ", foodCount - 1);
                int delIndex;
                scanf("%d", &delIndex);
                getchar(); // Consume newline character
                if (delIndex >= 0 && delIndex < foodCount) {
                    for (int i = delIndex; i < foodCount - 1; i++) {
                        strcpy(foods[i].name, foods[i + 1].name);
                        foods[i].price = foods[i + 1].price;
                        foods[i].quantity = foods[i + 1].quantity;
                    }
                    foodCount--;
                    writeFoodData(foods, foodCount);
                    printf("Food option deleted successfully.\n");
                } else {
                    printf("Invalid index.\n");
                }
                return 0;

            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
