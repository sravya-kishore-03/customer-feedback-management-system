#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "feedback.dat"
#define MAX_NAME_LEN 100
#define MAX_PRODUCT_LEN 100
#define MAX_FEEDBACK_LEN 20
#define MAX_COMMENT_LEN 200

// ------------------ STRUCT DEFINITION ------------------
typedef struct {
    int id;
    char name[MAX_NAME_LEN];
    char product[MAX_PRODUCT_LEN];
    char feedback[MAX_FEEDBACK_LEN];   // Excellent, Good, Average, Poor, Bad
    char comment[MAX_COMMENT_LEN];
} Feedback;

// ------------------ FUNCTION DECLARATIONS ------------------
void addFeedback();
void viewFeedbacks();
void searchFeedback();
void updateFeedback();
void deleteFeedback();
int idExists(int id);
void saveFeedback(Feedback f);
void loadFeedbacks(Feedback f[], int *count);
void writeAllFeedbacks(Feedback f[], int count);

// ------------------ MAIN MENU ------------------
int main() {
    int choice;
    do {
        printf("\n==============================\n");
        printf(" PRODUCT FEEDBACK SYSTEM\n");
        printf("==============================\n");
        printf("1. Add New Feedback\n");
        printf("2. View All Feedbacks\n");
        printf("3. Search Feedback\n");
        printf("4. Update Feedback\n");
        printf("5. Delete Feedback\n");
        printf("6. Exit\n");
        printf("------------------------------\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // clear newline character

        switch (choice) {
            case 1: addFeedback(); break;
            case 2: viewFeedbacks(); break;
            case 3: searchFeedback(); break;
            case 4: updateFeedback(); break;
            case 5: deleteFeedback(); break;
            case 6: printf("Exiting program. Goodbye!\n"); break;
            default: printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 6);

    return 0;
}

// ------------------ ADD FEEDBACK ------------------
void addFeedback() {
    Feedback f;

    printf("\n--- Add New Feedback ---\n");
    printf("Enter Feedback ID: ");
    scanf("%d", &f.id);
    getchar();

    if (idExists(f.id)) {
        printf("Error: Feedback ID already exists!\n");
        return;
    }

    printf("Enter Customer Name: ");
    fgets(f.name, sizeof(f.name), stdin);
    f.name[strcspn(f.name, "\n")] = '\0';

    printf("Enter Product Name: ");
    fgets(f.product, sizeof(f.product), stdin);
    f.product[strcspn(f.product, "\n")] = '\0';

    printf("\nSelect Feedback Option:\n");
    printf("1. Excellent\n");
    printf("2. Good\n");
    printf("3. Average\n");
    printf("4. Poor\n");
    printf("5. Bad\n");
    printf("Enter choice: ");
    int option;
    scanf("%d", &option);
    getchar();

    switch(option) {
        case 1: strcpy(f.feedback, "Excellent"); break;
        case 2: strcpy(f.feedback, "Good"); break;
        case 3: strcpy(f.feedback, "Average"); break;
        case 4: strcpy(f.feedback, "Poor"); break;
        case 5: strcpy(f.feedback, "Bad"); break;
        default: strcpy(f.feedback, "Unknown");
    }

    printf("Enter Additional Comments: ");
    fgets(f.comment, sizeof(f.comment), stdin);
    f.comment[strcspn(f.comment, "\n")] = '\0';

    saveFeedback(f);
    printf("Feedback submitted successfully!\n");
}

// ------------------ SAVE FEEDBACK TO FILE ------------------
void saveFeedback(Feedback f) {
    FILE *fp = fopen(FILE_NAME, "ab");
    if (!fp) {
        perror("Error opening file");
        return;
    }
    fwrite(&f, sizeof(Feedback), 1, fp);
    fclose(fp);
}

// ------------------ CHECK IF ID EXISTS ------------------
int idExists(int id) {
    FILE *fp = fopen(FILE_NAME, "rb");
    if (!fp) return 0;

    Feedback f;
    while (fread(&f, sizeof(Feedback), 1, fp)) {
        if (f.id == id) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

// ------------------ VIEW ALL FEEDBACKS ------------------
void viewFeedbacks() {
    FILE *fp = fopen(FILE_NAME, "rb");
    if (!fp) {
        printf("No feedback records found.\n");
        return;
    }

    Feedback f;
    printf("\n%-10s %-20s %-20s %-12s %-30s\n",
           "ID", "Customer", "Product", "Feedback", "Comment");
    printf("---------------------------------------------------------------------------------------------\n");

    while (fread(&f, sizeof(Feedback), 1, fp)) {
        printf("%-10d %-20s %-20s %-12s %-30s\n",
               f.id, f.name, f.product, f.feedback, f.comment);
    }
    fclose(fp);
}

// ------------------ SEARCH FEEDBACK ------------------
void searchFeedback() {
    FILE *fp = fopen(FILE_NAME, "rb");
    if (!fp) {
        printf("No records found.\n");
        return;
    }

    int id, found = 0;
    Feedback f;

    printf("\nEnter Feedback ID to search: ");
    scanf("%d", &id);

    while (fread(&f, sizeof(Feedback), 1, fp)) {
        if (f.id == id) {
            printf("\nFeedback Found:\n");
            printf("ID: %d\nName: %s\nProduct: %s\nFeedback: %s\nComment: %s\n",
                   f.id, f.name, f.product, f.feedback, f.comment);
            found = 1;
            break;
        }
    }

    if (!found)
        printf("No feedback found with ID %d.\n", id);

    fclose(fp);
}

// ------------------ UPDATE FEEDBACK ------------------
void updateFeedback() {
    Feedback list[200];
    int count = 0, id, found = 0;

    loadFeedbacks(list, &count);

    printf("\nEnter Feedback ID to update: ");
    scanf("%d", &id);
    getchar();

    for (int i = 0; i < count; i++) {
        if (list[i].id == id) {
            found = 1;

            printf("\nSelect New Feedback Option:\n");
            printf("1. Excellent\n");
            printf("2. Good\n");
            printf("3. Average\n");
            printf("4. Poor\n");
            printf("5. Bad\n");
            printf("Enter choice: ");
            int option;
            scanf("%d", &option);
            getchar();

            switch(option) {
                case 1: strcpy(list[i].feedback, "Excellent"); break;
                case 2: strcpy(list[i].feedback, "Good"); break;
                case 3: strcpy(list[i].feedback, "Average"); break;
                case 4: strcpy(list[i].feedback, "Poor"); break;
                case 5: strcpy(list[i].feedback, "Bad"); break;
                default: strcpy(list[i].feedback, "Unknown");
            }

            printf("Enter new comment: ");
            fgets(list[i].comment, sizeof(list[i].comment), stdin);
            list[i].comment[strcspn(list[i].comment, "\n")] = '\0';

            break;
        }
    }

    if (found) {
        writeAllFeedbacks(list, count);
        printf("Feedback updated successfully.\n");
    } else {
        printf("Feedback ID not found.\n");
    }
}

// ------------------ DELETE FEEDBACK ------------------
void deleteFeedback() {
    Feedback list[200];
    int count = 0, id, found = 0;

    loadFeedbacks(list, &count);

    printf("\nEnter Feedback ID to delete: ");
    scanf("%d", &id);
    getchar();

    for (int i = 0; i < count; i++) {
        if (list[i].id == id) {
            found = 1;

            printf("Are you sure you want to delete this feedback from %s (ID %d)? (y/n): ",
                   list[i].name, id);
            char confirm = getchar();
            getchar();

            if (confirm == 'y' || confirm == 'Y') {
                for (int j = i; j < count - 1; j++)
                    list[j] = list[j + 1];
                count--;
                writeAllFeedbacks(list, count);
                printf("Feedback deleted successfully.\n");
            } else {
                printf("Deletion cancelled.\n");
            }
            break;
        }
    }

    if (!found)
        printf("Feedback ID not found.\n");
}

// ------------------ LOAD ALL FEEDBACKS ------------------
void loadFeedbacks(Feedback f[], int *count) {
    FILE *fp = fopen(FILE_NAME, "rb");
    if (!fp) return;

    *count = 0;
    while (fread(&f[*count], sizeof(Feedback), 1, fp))
        (*count)++;

    fclose(fp);
}

// ------------------ WRITE ALL FEEDBACKS ------------------
void writeAllFeedbacks(Feedback f[], int count) {
    FILE *fp = fopen(FILE_NAME, "wb");
    if (!fp) {
        perror("Error opening file");
        return;
    }

    fwrite(f, sizeof(Feedback), count, fp);
    fclose(fp);
}