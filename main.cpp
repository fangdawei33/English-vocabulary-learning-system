#include <stdio.h>
#include <string.h>

#define MAX_LENGTH 100
#define MAX_USERS 10
#define MAX_WORDBANKS 10
#define MAX_WORDS 50

// 用户结构体
typedef struct {
    char username[MAX_LENGTH];
    char password[MAX_LENGTH];
    char name[MAX_LENGTH];
    int studyDays;
    char lastLogin[MAX_LENGTH];
    int incorrectCount; // 错题数
    int unlearnedWordsCount; // 生词数
} User;

// 词库结构体
typedef struct {
    char wordbankName[MAX_LENGTH];
    char words[MAX_WORDS][MAX_LENGTH]; // 存储词库中的单词
    int wordCount;
} Wordbank;

// 全局变量
User users[MAX_USERS];
Wordbank wordbanks[MAX_WORDBANKS];
int userCount = 0;
int wordbankCount = 0;

// 主界面函数声明
void mainMenu();
void userLogin();
void userRegister();
void adminLogin();

// 用户登录后界面
void userMainMenu(User *user);
void showUserInfo(User *user);
void punchIn(User *user);
void startLearning(User *user);
void searchWord();
void reviewMistakes(User *user);
void showUnlearnedWords(User *user);
void showLearnedWords(User *user);
void clearStudyRecords(User *user);
void funChallenge();

// 后台管理
void adminMainMenu();
void manageUsers();
void manageWordbanks();
void manageWords();
void listUsers();
void deleteUser(int index);
void listWordbanks();
void deleteWordbankByAdmin(int index);
void listWords(int wordbankIndex);
void deleteWord(int wordbankIndex, int wordIndex);
void addWord(int wordbankIndex, const char *word);

// 用户学习功能
void showMyWordbanks();//展示我的词库 
void selectWordbank(User *user);//选择题库 
void addWordbank(User *user);//添加词库 
void deleteWordbank(User *user);///删除词库 
void backToLearningMode(User *user);
void wordOrderLearning();
void randomOrderLearning();
void englishToChineseLearning();
void chineseToEnglishLearning();

int main() {
    mainMenu();
    return 0;
}

void mainMenu() {
    int choice;
    while (1) {
        printf("欢迎来到学习系统\n");
        printf("1. 用户登录\n");
        printf("2. 用户注册\n");
        printf("3. 后台管理\n");
        printf("4. 退出\n");
        printf("请输入选项：");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                userLogin();
                break;
            case 2:
                userRegister();
                break;
            case 3:
                adminLogin();
                break;
            case 4:
                printf("退出系统\n");
                return;
            default:
                printf("无效输入，请重新输入\n");
                break;
        }
    }
}

// 用户登录
void userLogin() {
    char username[MAX_LENGTH], password[MAX_LENGTH];
    printf("请输入用户名: ");
    scanf("%s", username);
    printf("请输入密码: ");
    scanf("%s", password);

    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
            printf("登录成功！\n");
            userMainMenu(&users[i]);
            return;
        }
    }
    printf("用户名或密码错误！\n");
}

// 用户注册
void userRegister() {
    if (userCount >= MAX_USERS) {
        printf("用户已满，无法注册。\n");
        return;
    }

    User newUser;
    printf("请输入用户名: ");
    scanf("%s", newUser.username);
    printf("请输入密码: ");
    scanf("%s", newUser.password);
    printf("请输入姓名: ");
    scanf("%s", newUser.name);
    newUser.studyDays = 0;
    strcpy(newUser.lastLogin, "未登录");
    newUser.incorrectCount = 0; // 错题数初始化为0
    newUser.unlearnedWordsCount = 0; // 生词数初始化为0

    users[userCount++] = newUser;
    printf("注册成功！\n");
}

// 用户主界面
void userMainMenu(User *user) {
    int choice;
    while (1) {
        printf("\n欢迎 %s, 请选一个操作:\n", user->name);
        printf("1. 显示个人信息\n");
        printf("2. 打卡\n");
        printf("3. 开始学习\n");
        printf("4. 搜索单词\n");
        printf("5. 错题复习\n");
        printf("6. 错题本\n");
        printf("7. 生词本\n");
        printf("8. 显示已经背过的单词\n");
        printf("9. 清空学习记录\n");
        printf("10. 趣味挑战\n");
        printf("11. 退出\n");
        printf("请输入选项: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                showUserInfo(user);
                break;
            case 2:
                punchIn(user);
                break;
            case 3:
                startLearning(user);
                break;
            case 4:
                searchWord();
                break;
            case 5:
                reviewMistakes(user);
                break;
            case 6:
                showUnlearnedWords(user);
                break;
            case 7:
                showLearnedWords(user);
                break;
            case 8:
                clearStudyRecords(user);
                break;
            case 9:
                funChallenge();
                break;
            case 10:
                return;
            default:
                printf("无效输入，请重新输入\n");
                break;
        }
    }
}

// 显示个人信息
void showUserInfo(User *user) {
    printf("用户名: %s\n", user->username);
    printf("姓名: %s\n", user->name);
    printf("最近打卡日期: %s\n", user->lastLogin);
    printf("累计打卡天数: %d\n", user->studyDays);
    printf("错题数: %d\n", user->incorrectCount);
    printf("生词数: %d\n", user->unlearnedWordsCount);
}

// 打卡
void punchIn(User *user) {
    printf("打卡成功！\n");
    user->studyDays++;
    printf("当前打卡天数: %d\n", user->studyDays);
}

// 开始学习
void startLearning(User *user) {
    int choice;
    while (1) {
        printf("\n请选择操作：\n");
        printf("1. 显示我的题库\n");
        printf("2. 选择题库\n");
        printf("3. 添加题库\n");
        printf("4. 删除题库\n");
        printf("5. 退出\n");
        printf("请输入选项: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                showMyWordbanks();
                break;
            case 2:
                selectWordbank(user);
                break;
            case 3:
                addWordbank(user);
                break;
            case 4:
                deleteWordbank(user);
                break;
            case 5:
                return;
            default:
                printf("无效输入，请重新输入\n");
                break;
        }
    }
}

// 显示我的题库
void showMyWordbanks() {
    if (wordbankCount == 0) {
        printf("没有题库可显示\n");
        return;
    }
    printf("当前题库:\n");
    for (int i = 0; i < wordbankCount; i++) {
        printf("%d. %s\n", i + 1, wordbanks[i].wordbankName);
    }
}

// 选择题库
void selectWordbank(User *user) {
    int index;
    showMyWordbanks(); // 显示所有题库
    printf("请选择题库编号: ");
    scanf("%d", &index);
    
    // 校验用户选择的题库编号是否有效
    if (index >= 1 && index <= wordbankCount) {
        printf("选择了题库: %s\n", wordbanks[index - 1].wordbankName);
        int choice;
        while (1) {
            printf("\n请选择操作：\n");
            printf("1. 浏览单词\n");
            printf("2. 背诵单词\n");
            printf("3. 返回\n");
            printf("请输入选项: ");
            scanf("%d", &choice);

            switch (choice) {
                case 1:
                    browseWords(index - 1); // 浏览单词
                    break;
                case 2:
                    startLearning(user, index - 1); // 进入背单词模式
                    break;
                case 3:
                    return; // 返回
                default:
                    printf("无效输入，请重新输入\n");
                    break;
            }
        }
    } else {
        printf("无效的题库编号！\n");
    }
}

// 浏览单词
void browseWords(int wordbankIndex) {
    if (wordbanks[wordbankIndex].wordCount == 0) {
        printf("该题库没有单词。\n");
        return;
    }
    printf("题库: %s\n", wordbanks[wordbankIndex].wordbankName);
    for (int i = 0; i < wordbanks[wordbankIndex].wordCount; i++) {
        printf("%d. %s\n", i + 1, wordbanks[wordbankIndex].words[i]);
    }
}

// 开始背单词（背诵单词的模式选择）
void startLearning(User *user, int wordbankIndex) {
    int choice;
    while (1) {
        printf("\n请选择背单词模式：\n");
        printf("1. 顺序背单词\n");
        printf("2. 乱序背单词\n");
        printf("3. 根据英文选中文\n");
        printf("4. 根据中文选英文\n");
        printf("5. 返回\n");
        printf("请输入选项: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                wordOrderLearning(wordbankIndex); // 顺序背单词
                break;
            case 2:
                randomOrderLearning(wordbankIndex); // 乱序背单词
                break;
            case 3:
                englishToChineseLearning(wordbankIndex); // 英文选中文
                break;
            case 4:
                chineseToEnglishLearning(wordbankIndex); // 中文选英文
                break;
            case 5:
                return; // 返回
            default:
                printf("无效输入，请重新输入\n");
                break;
        }
    }
}

// 顺序背单词
void wordOrderLearning(int wordbankIndex) {
    printf("顺序背单词模式启动...\n");
    for (int i = 0; i < wordbanks[wordbankIndex].wordCount; i++) {
        printf("背诵单词: %s\n", wordbanks[wordbankIndex].words[i]);
        // 这里可以添加用户输入或测试功能
    }
}

// 乱序背单词
void randomOrderLearning(int wordbankIndex) {
    printf("乱序背单词模式启动...\n");
    // 这里可以使用乱序算法来随机排列单词
    for (int i = 0; i < wordbanks[wordbankIndex].wordCount; i++) {
        printf("背诵单词: %s\n", wordbanks[wordbankIndex].words[i]);
        // 可以添加用户输入的测试
    }
}

// 根据英文选中文
void englishToChineseLearning(int wordbankIndex) {
    printf("英文选中文模式启动...\n");
    for (int i = 0; i < wordbanks[wordbankIndex].wordCount; i++) {
        printf("英文: %s\n", wordbanks[wordbankIndex].words[i]);
        // 提示用户输入中文翻译
    }
}

// 根据中文选英文
void chineseToEnglishLearning(int wordbankIndex) {
    printf("中文选英文模式启动...\n");
    for (int i = 0; i < wordbanks[wordbankIndex].wordCount; i++) {
        printf("中文: %s\n", wordbanks[wordbankIndex].words[i]);
        // 提示用户输入英文翻译
    }
}


// 趣味挑战
void funChallenge() {
    printf("趣味挑战开始...\n");
}

// 管理员登录
void adminLogin() {
    char adminPassword[MAX_LENGTH] = "admin123"; // 固定管理员密码
    char password[MAX_LENGTH];
    
    printf("请输入管理员密码: ");
    scanf("%s", password);

    if (strcmp(password, adminPassword) == 0) {
        printf("管理员登录成功！\n");
        adminMainMenu();
    } else {
        printf("密码错误！\n");
    }
}

// 管理员主菜单
void adminMainMenu() {
    int choice;
    while (1) {
        printf("\n管理员操作菜单:\n");
        printf("1. 管理用户\n");
        printf("2. 管理词库\n");
        printf("3. 管理单词\n");
        printf("4. 退出\n");
        printf("请输入选项: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                manageUsers();
                break;
            case 2:
                manageWordbanks();
                break;
            case 3:
                manageWords();
                break;
            case 4:
                return;
            default:
                printf("无效输入，请重新输入\n");
                break;
        }
    }
}

// 管理用户
void manageUsers() {
    printf("用户管理功能\n");
    // 添加其他管理操作...
}

// 管理词库
void manageWordbanks() {
    printf("词库管理功能\n");
    // 添加其他管理操作...
}

// 管理单词
void manageWords() {
    printf("单词管理功能\n");
    // 添加其他管理操作...
}
