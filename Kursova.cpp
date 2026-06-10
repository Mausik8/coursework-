#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 1024
#define MAX_LINES 1000

void showMenu(void);
void createFile(void);
void openFile(void);
void editFile(void);
void saveToFile(const char *filename, char **lines, int count);

int main() {
    int choice;
    
    while (1) {
        showMenu();
        printf("Ваш вибір: ");
        
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("\nПомилка: введіть число від 1 до 4!\n\n");
            continue;
        }
        while (getchar() != '\n');
        
        switch (choice) {
            case 1:
                createFile();
                break;
            case 2:
                openFile();
                break;
            case 3:
                editFile();
                break;
            case 4:
                printf("\nДякуємо за використання програми. До побачення!\n");
                return 0;
            default:
                printf("\nНевірний вибір. Спробуйте ще раз.\n\n");
        }
    }
    
    return 0;
}

void showMenu(void) {
    printf("========================================\n");
    printf("      КОНСОЛЬНИЙ ТЕКСТОВИЙ РЕДАКТОР\n");
    printf("========================================\n");
    printf("1. Створити новий файл\n");
    printf("2. Відкрити файл (перегляд)\n");
    printf("3. Редагувати файл\n");
    printf("4. Вихід\n");
    printf("========================================\n");
}

void createFile(void) {
    char filename[256];
    char buffer[MAX_LENGTH];
    FILE *file;
    
    printf("\n--- Створення нового файлу ---\n");
    printf("Введіть ім'я файлу (наприклад, notes.txt): ");
    
    if (fgets(filename, sizeof(filename), stdin) != NULL) {
        filename[strcspn(filename, "\n")] = '\0';
    }
    
    file = fopen(filename, "w");
    if (file == NULL) {
        printf("Помилка: не вдалося створити файл!\n\n");
        return;
    }
    
    printf("Введіть текст. Для завершення введіть порожній рядок.\n");
    printf("----------------------------------------\n");
    
    while (1) {
        printf("> ");
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }
        
        if (buffer[0] == '\n') {
            break;
        }
        
        fputs(buffer, file);
    }
    
    fclose(file);
    printf("----------------------------------------\n");
    printf("Файл '%s' успішно збережено!\n\n", filename);
}

void openFile(void) {
    char filename[256];
    char buffer[MAX_LENGTH];
    FILE *file;
    int lineNumber = 1;
    
    printf("\n--- Відкриття файлу ---\n");
    printf("Введіть ім'я файлу: ");
    
    if (fgets(filename, sizeof(filename), stdin) != NULL) {
        filename[strcspn(filename, "\n")] = '\0';
    }
    
    file = fopen(filename, "r");
    if (file == NULL) {
        printf("Помилка: файл '%s' не знайдено!\n\n", filename);
        return;
    }
    
    printf("\n--- Вміст файлу '%s' ---\n", filename);
    printf("----------------------------------------\n");
    
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        printf("%3d | %s", lineNumber, buffer);
        lineNumber++;
    }
    
    printf("----------------------------------------\n");
    printf("Всього рядків: %d\n\n", lineNumber - 1);
    
    fclose(file);
}

void editFile(void) {
    char filename[256];
    char buffer[MAX_LENGTH];
    char *lines[MAX_LINES];
    FILE *file;
    int lineCount = 0;
    int i, choice, lineNum;
    
    printf("\n--- Редагування файлу ---\n");
    printf("Введіть ім'я файлу: ");
    
    if (fgets(filename, sizeof(filename), stdin) != NULL) {
        filename[strcspn(filename, "\n")] = '\0';
    }
    
    file = fopen(filename, "r");
    if (file == NULL) {
        printf("Помилка: файл '%s' не знайдено!\n\n", filename);
        return;
    }
    
    while (fgets(buffer, sizeof(buffer), file) != NULL && lineCount < MAX_LINES) {
        lines[lineCount] = (char *)malloc(strlen(buffer) + 1);
        if (lines[lineCount] == NULL) {
            printf("Помилка: не вдалося виділити пам'ять!\n");
            break;
        }
        strcpy(lines[lineCount], buffer);
        lineCount++;
    }
    fclose(file);
    
    printf("Файл завантажено. Рядків: %d\n", lineCount);
    
    while (1) {
        printf("\n--- Меню редагування ---\n");
        printf("1. Показати вміст\n");
        printf("2. Додати рядок в кінець\n");
        printf("3. Вставити рядок після номера\n");
        printf("4. Видалити рядок за номером\n");
        printf("5. Зберегти та вийти\n");
        printf("Ваш вибір: ");
        
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("Помилка введення!\n");
            continue;
        }
        while (getchar() != '\n');
        
        switch (choice) {
            case 1:
                printf("\n--- Поточний вміст ---\n");
                for (i = 0; i < lineCount; i++) {
                    printf("%3d | %s", i + 1, lines[i]);
                }
                printf("----------------------\n");
                break;
                
            case 2:
                if (lineCount >= MAX_LINES) {
                    printf("Досягнуто максимальну кількість рядків!\n");
                    break;
                }
                printf("Введіть новий рядок: ");
                if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
                    lines[lineCount] = (char *)malloc(strlen(buffer) + 1);
                    strcpy(lines[lineCount], buffer);
                    lineCount++;
                    printf("Рядок додано.\n");
                }
                break;
                
            case 3:
                printf("Введіть номер рядка, після якого вставити: ");
                if (scanf("%d", &lineNum) != 1 || lineNum < 1 || lineNum > lineCount) {
                    printf("Невірний номер рядка!\n");
                    while (getchar() != '\n');
                    break;
                }
                while (getchar() != '\n');
                
                if (lineCount >= MAX_LINES) {
                    printf("Досягнуто максимальну кількість рядків!\n");
                    break;
                }
                
                printf("Введіть рядок для вставки: ");
                if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
                    for (i = lineCount; i > lineNum; i--) {
                        lines[i] = lines[i - 1];
                    }
                    lines[lineNum] = (char *)malloc(strlen(buffer) + 1);
                    strcpy(lines[lineNum], buffer);
                    lineCount++;
                    printf("Рядок вставлено.\n");
                }
                break;
                
            case 4:
                printf("Введіть номер рядка для видалення: ");
                if (scanf("%d", &lineNum) != 1 || lineNum < 1 || lineNum > lineCount) {
                    printf("Невірний номер рядка!\n");
                    while (getchar() != '\n');
                    break;
                }
                while (getchar() != '\n');
                
                free(lines[lineNum - 1]);
                
                for (i = lineNum - 1; i < lineCount - 1; i++) {
                    lines[i] = lines[i + 1];
                }
                lineCount--;
                printf("Рядок %d видалено.\n", lineNum);
                break;
                
            case 5:
                saveToFile(filename, lines, lineCount);
                
                for (i = 0; i < lineCount; i++) {
                    free(lines[i]);
                }
                printf("Зміни збережено. Повернення до головного меню.\n\n");
                return;
                
            default:
                printf("Невірний вибір!\n");
        }
    }
}

void saveToFile(const char *filename, char **lines, int count) {
    FILE *file;
    int i;
    
    file = fopen(filename, "w");
    if (file == NULL) {
        printf("Помилка: не вдалося відкрити файл для запису!\n");
        return;
    }
    
    for (i = 0; i < count; i++) {
        fputs(lines[i], file);
    }
    
    fclose(file);
    printf("Файл '%s' збережено (%d рядків).\n", filename, count);
}