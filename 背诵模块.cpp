#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

// 定义最大单词数量和每个单词的最大长度
#define MAX_LENGTH 50

// 定义链表节点
typedef struct WordNode {
    char english[MAX_LENGTH];
    char chinese[MAX_LENGTH];
    struct WordNode* next;
} WordNode;

// 函数声明
void loadWords(WordNode **head, const char *filename);
void selectChineseAccordingToEnglishInOrder(WordNode *head, int wordbankIndex, int *score);
void selectEnglishAccordingToChineseInOrder(WordNode *head, int wordbankIndex, int *score);
void selectChineseAccordingToEnglishInRandomOrder(WordNode *head, int *score);
void selectEnglishAccordingToChineseInRandomOrder(WordNode *head, int *score);
void shuffleOptions(char options[][MAX_LENGTH], int optionCount);
int getRandomIndex(int wordCount, int excludeIndex);
void recordWrongQuestion(const char *english, const char *chinese);
void freeWordList(WordNode *head);

int main() {
    WordNode *wordbankHead = NULL;
    int score = 0;

    // 加载单词库
    loadWords(&wordbankHead, "wordbank.txt");

    // 选择一个功能进行测试
    selectChineseAccordingToEnglishInOrder(wordbankHead, 0, &score);

    // 最终分数输出
    printf("Final score: %d\n", score);

    // 释放链表内存
    freeWordList(wordbankHead);

    return 0;
}

// 从文件加载单词库到链表
void loadWords(WordNode **head, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Failed to open the file!\n");
        exit(1);
    }

    WordNode *temp = NULL;
    while (!feof(file)) {
        WordNode *newNode = (WordNode*)malloc(sizeof(WordNode));
        if (fscanf(file, "%s %s", newNode->english, newNode->chinese) != 2) {
            free(newNode);
            break;  // 如果文件格式不正确，跳出循环
        }
        newNode->next = NULL;

        if (*head == NULL) {
            *head = newNode;
        } else {
            temp = *head;
            while (temp->next != NULL) {
                temp = temp->next;
            }
            temp->next = newNode;
        }
    }

    fclose(file);
}

// 根据英文选中文（顺序）
void selectChineseAccordingToEnglishInOrder(WordNode *head, int wordbankIndex, int *score) {
    WordNode *current = head;
    int currentIndex = 0;

    // 找到对应的单词
    while (current != NULL && currentIndex < wordbankIndex) {
        current = current->next;
        currentIndex++;
    }

    if (current == NULL) {
        printf("Invalid wordbankIndex!\n");
        return;
    }

    // 获取正确选项和三个错误选项
    WordNode *wrongOptions[3];
    char options[4][MAX_LENGTH];
    strcpy(options[0], current->chinese);

    // 获取三个错误选项
    int optionCount = 1;
    WordNode *temp = head;
    while (temp != NULL && optionCount < 4) {
        if (strcmp(temp->chinese, current->chinese) != 0) {
            strcpy(options[optionCount], temp->chinese);
            optionCount++;
        }
        temp = temp->next;
    }

    shuffleOptions(options, 4);

    // 打印题目和选项
    printf("What is the Chinese translation for the word: %s?\n", current->english);
    for (int i = 0; i < 4; i++) {
        printf("%c. %s\n", 'A' + i, options[i]);
    }

    // 用户输入选择
    char choice;
    printf("Your answer (A, B, C, D): ");
    scanf(" %c", &choice);
    choice = toupper(choice);

    // 判断答案
    if (choice >= 'A' && choice <= 'D') {
        int correctIndex = 0;
        for (int i = 0; i < 4; i++) {
            if (strcmp(options[i], current->chinese) == 0) {
                correctIndex = i;
                break;
            }
        }

        if (choice == 'A' + correctIndex) {
            printf("Correct! +10 points.\n");
            *score += 10;
        } else {
            printf("Wrong! -5 points.\n");
            *score -= 5;
            recordWrongQuestion(current->english, current->chinese);
        }
    } else {
        printf("Invalid choice.\n");
    }

    printf("Next question...\n");
}

// 根据中文选英文（顺序）
void selectEnglishAccordingToChineseInOrder(WordNode *head, int wordbankIndex, int *score) {
    WordNode *current = head;
    int currentIndex = 0;

    // 找到对应的单词
    while (current != NULL && currentIndex < wordbankIndex) {
        current = current->next;
        currentIndex++;
    }

    if (current == NULL) {
        printf("Invalid wordbankIndex!\n");
        return;
    }

    // 获取正确选项和三个错误选项
    WordNode *wrongOptions[3];
    char options[4][MAX_LENGTH];
    strcpy(options[0], current->english);

    // 获取三个错误选项
    int optionCount = 1;
    WordNode *temp = head;
    while (temp != NULL && optionCount < 4) {
        if (strcmp(temp->english, current->english) != 0) {
            strcpy(options[optionCount], temp->english);
            optionCount++;
        }
        temp = temp->next;
    }

    shuffleOptions(options, 4);

    // 打印题目和选项
    printf("What is the English translation for the word: %s?\n", current->chinese);
    for (int i = 0; i < 4; i++) {
        printf("%c. %s\n", 'A' + i, options[i]);
    }

    // 用户输入选择
    char choice;
    printf("Your answer (A, B, C, D): ");
    scanf(" %c", &choice);
    choice = toupper(choice);

    // 判断答案
    if (choice >= 'A' && choice <= 'D') {
        int correctIndex = 0;
        for (int i = 0; i < 4; i++) {
            if (strcmp(options[i], current->english) == 0) {
                correctIndex = i;
                break;
            }
        }

        if (choice == 'A' + correctIndex) {
            printf("Correct! +10 points.\n");
            *score += 10;
        } else {
            printf("Wrong! -5 points.\n");
            *score -= 5;
            recordWrongQuestion(current->english, current->chinese);
        }
    } else {
        printf("Invalid choice.\n");
    }

    printf("Next question...\n");
}

// 根据英文选中文（乱序）
void selectChineseAccordingToEnglishInRandomOrder(WordNode *head, int *score) {
    WordNode *current = head;
    int wordCount = 0;
    WordNode *temp = head;

    // 获取单词数量
    while (temp != NULL) {
        wordCount++;
        temp = temp->next;
    }

    int randomIndex = rand() % wordCount;
    current = head;
    for (int i = 0; i < randomIndex; i++) {
        current = current->next;
    }

    // 获取正确选项和三个错误选项
    char options[4][MAX_LENGTH];
    strcpy(options[0], current->chinese);

    int optionCount = 1;
    temp = head;
    while (temp != NULL && optionCount < 4) {
        if (strcmp(temp->chinese, current->chinese) != 0) {
            strcpy(options[optionCount], temp->chinese);
            optionCount++;
        }
        temp = temp->next;
    }

    shuffleOptions(options, 4);

    // 打印题目和选项
    printf("What is the Chinese translation for the word: %s?\n", current->english);
    for (int i = 0; i < 4; i++) {
        printf("%c. %s\n", 'A' + i, options[i]);
    }

    // 用户输入选择
    char choice;
    printf("Your answer (A, B, C, D): ");
    scanf(" %c", &choice);
    choice = toupper(choice);

    // 判断答案
    if (choice >= 'A' && choice <= 'D') {
        int correctIndex = 0;
        for (int i = 0; i < 4; i++) {
            if (strcmp(options[i], current->chinese) == 0) {
                correctIndex = i;
                break;
            }
        }

        if (choice == 'A' + correctIndex) {
            printf("Correct! +10 points.\n");
            *score += 10;
        } else {
            printf("Wrong! -5 points.\n");
            *score -= 5;
            recordWrongQuestion(current->english, current->chinese);
        }
    } else {
        printf("Invalid choice.\n");
    }

    printf("Next question...\n");
}

// 根据中文选英文（乱序）
void selectEnglishAccordingToChineseInRandomOrder(WordNode *head, int *score) {
    WordNode *current = head;
    int wordCount = 0;
    WordNode *temp = head;

    // 获取单词数量
    while (temp != NULL) {
        wordCount++;
        temp = temp->next;
    }

    int randomIndex = rand() % wordCount;
    current = head;
    for (int i = 0; i < randomIndex; i++) {
        current = current->next;
    }

    // 获取正确选项和三个错误选项
    char options[4][MAX_LENGTH];
    strcpy(options[0], current->english);

    int optionCount = 1;
    temp = head;
    while (temp != NULL && optionCount < 4) {
        if (strcmp(temp->english, current->english) != 0) {
            strcpy(options[optionCount], temp->english);
            optionCount++;
        }
        temp = temp->next;
    }

    shuffleOptions(options, 4);

    // 打印题目和选项
    printf("What is the English translation for the word: %s?\n", current->chinese);
    for (int i = 0; i < 4; i++) {
        printf("%c. %s\n", 'A' + i, options[i]);
    }

    // 用户输入选择
    char choice;
    printf("Your answer (A, B, C, D): ");
    scanf(" %c", &choice);
    choice = toupper(choice);

    // 判断答案
    if (choice >= 'A' && choice <= 'D') {
        int correctIndex = 0;
        for (int i = 0; i < 4; i++) {
            if (strcmp(options[i], current->english) == 0) {
                correctIndex = i;
                break;
            }
        }

        if (choice == 'A' + correctIndex) {
            printf("Correct! +10 points.\n");
            *score += 10;
        } else {
            printf("Wrong! -5 points.\n");
            *score -= 5;
            recordWrongQuestion(current->english, current->chinese);
        }
    } else {
        printf("Invalid choice.\n");
    }

    printf("Next question...\n");
}

// 打乱选项顺序
void shuffleOptions(char options[][MAX_LENGTH], int optionCount) {
    for (int i = optionCount - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        char temp[MAX_LENGTH];
        strcpy(temp, options[i]);
        strcpy(options[i], options[j]);
        strcpy(options[j], temp);
    }
}

// 记录错题到文件
void recordWrongQuestion(const char *english, const char *chinese) {
    FILE *file = fopen("wrong_questions.txt", "a");
    if (file == NULL) {
        printf("Failed to open the wrong questions file.\n");
        return;
    }
    fprintf(file, "%s - %s\n", english, chinese);
    fclose(file);
}

// 释放链表内存
void freeWordList(WordNode *head) {
    WordNode *temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}
