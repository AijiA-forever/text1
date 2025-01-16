#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>  // 用于获取当前时间，在示例中用于首次/末次查询日期的记录
// 默认不是管理员 
int isAdmin=0;

// 定义问题结构体
typedef struct Question {
    int questionId;  // 问题编号
    int questionType;  // 问题类型
    char questionContent[100];  // 问题内容
    char questionAnswer[301];  // 问题答案
    char firstQueryDate[20];  // 首次查询日期
    char lastQueryDate[20];  // 末次查询日期
    int queryTimes;  // 查询次数
    int userSatisfiedTimes;  // 用户满意次数
    struct Question *next;  // 指向下一个节点的指针，用于链表
} Question;

// 从文件读取问题数据到链表
Question *readQuestionsFromFile(const char *filename);

// 根据选择执行相应查询功能的函数
void queryQuestions(Question *head);

// 排序功能函数，按指定规则排序链表
void sortQuestions(Question *head);

// 数据录入函数，管理员录入新问题
void inputQuestions(Question **head);

// 显示所有问题记录函数
void displayAllQuestions(Question *head);

// 数据变更（更改或删除）函数，管理员操作
void updateQuestions(Question **head);

// 统计功能函数，区分用户和管理员统计
void statistics(Question *head);

// 判断是否为管理员权限的函数
void isAdm();

// 主函数 
int main() {
	// 判断是否为管理员 
	isAdm();
    //isAdmin=1; 
	// 从文件读取问题数据到链表
    Question *questionList = readQuestionsFromFile("C:\\Users\\AijiA\\Desktop\\24级12班 3124004474 李忠林\\2、源程序及所需文件\\question.txt");
    int choice;
    do {
        printf("**************************************************************************\n");
        printf("1------查询   2------排序   3------统计   4------录入   5------更新 \n6------退出\n");
        printf("**************************************************************************\n");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
            	// 查询 
                queryQuestions(questionList);
                break;
            case 2:
            	// 排序 
                sortQuestions(questionList);
                break;
            case 3:
            	// 统计功能函数，区分用户和管理员统计
                statistics(questionList);
                break;
            case 4:
            	// 数据录入函数，管理员录入新问题
                inputQuestions(&questionList);
                break;
            case 5:
            	// 数据变更（更改或删除）函数，管理员操作
                updateQuestions(&questionList);
                break;
            case 6:
            	// 退出 
            	break;
            default:
                printf("无效的选择，请重新输入！\n");
        }
    } while (choice >= 1 && choice <= 5);

    // 释放链表内存
    Question *current = questionList;
    Question *next;
    while (current!= NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    return 0;
}

// 从文件读取问题数据到链表
Question *readQuestionsFromFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("无法打开文件 %s，请检查文件是否存在及权限设置！\n", filename);
        return NULL;
    }
    Question *head = NULL;
    Question *tail = NULL;
    Question temp;
    while (fscanf(file, "%d %d %s %s %s %s %d %d", &temp.questionId, &temp.questionType,
                  temp.questionContent, temp.questionAnswer, temp.firstQueryDate,
                  temp.lastQueryDate, &temp.queryTimes, &temp.userSatisfiedTimes) == 8) {
        Question *newNode = (Question *)malloc(sizeof(Question));
        if (newNode == NULL) {
            printf("内存分配失败！\n");
            continue;
        }
        *newNode = temp;
        newNode->next = NULL;
        if (head == NULL) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }
    fclose(file);
    return head;
}

// 记录查询次数及满意次数
void recordingTimes(Question *current, Question *head) {
    current->queryTimes++;
	char satisDegree[]="是";
	char isSatis[3];
	printf("是否满意本次查询（是或否）："); 
	scanf("%s",isSatis);
	if(strcmp(satisDegree, isSatis) == 0){
	current->userSatisfiedTimes++;	
	}
    // 将更新后的数据写回文件
    FILE *file = fopen("C:\\Users\\AijiA\\Desktop\\24级12班 3124004474 李忠林\\2、源程序及所需文件\\question.txt", "w");
    if (file == NULL) {
        perror("无法打开文件 question.txt 进行写入");
        return;
    }
    Question *temp = head;
    while (temp!= NULL) {
        if (fprintf(file, "%d %d %s %s %s %s %d %d\n", temp->questionId, temp->questionType,
                    temp->questionContent, temp->questionAnswer, temp->firstQueryDate,
                    temp->lastQueryDate, temp->queryTimes, temp->userSatisfiedTimes) < 0) {
            perror("写入 question.txt 文件失败");
            break;
        }
        temp = temp->next;
    }
    fclose(file);
}

// 按问题类型查询函数（辅助函数，供查询功能调用）
void queryByQuestionType(Question *head, int questionType) {
    Question *current = head;
    int found = 0;
    while (current!= NULL) {
        if (current->questionType == questionType) {
            printf("问题编号：%d\n问题类型：%d\n问题内容：%s\n问题答案：%s\n首次查询日期：%s\n末次查询日期：%s\n查询次数：%d\n用户满意次数：%d\n",
                   current->questionId, current->questionType, current->questionContent,
                   current->questionAnswer, current->firstQueryDate, current->lastQueryDate,
                   current->queryTimes, current->userSatisfiedTimes);
            found = 1;
            recordingTimes(current,head);
        }
        current = current->next;
    }
    if (!found) {
        printf("未找到该问题类型的记录！\n");
    }
}

// 按问题内容模糊查询函数（辅助函数，供查询功能调用）
void queryByQuestionContent(Question *head, const char *content) {
    Question *current = head;
    int found = 0;
    while (current!= NULL) {
        if (strstr(current->questionContent, content)!= NULL) {
            printf("问题编号：%d\n问题类型：%d\n问题内容：%s\n问题答案：%s\n首次查询日期：%s\n末次查询日期：%s\n查询次数：%d\n用户满意次数：%d\n",
                   current->questionId, current->questionType, current->questionContent,
                   current->questionAnswer, current->firstQueryDate, current->lastQueryDate,
                   current->queryTimes, current->userSatisfiedTimes);
            found = 1;
            recordingTimes(current,head);
        }
        current = current->next;
    }
    if (!found) {
        printf("未找到包含该内容的问题记录！\n");
    }
}

// 按查询次数范围查询函数（辅助函数，供查询功能调用）
void queryByQueryTimesRange(Question *head, int minTimes, int maxTimes) {
    Question *current = head;
    int found = 0;
    while (current!= NULL) {
        if (current->queryTimes >= minTimes && current->queryTimes <= maxTimes) {
            printf("问题编号：%d\n问题类型：%d\n问题内容：%s\n问题答案：%s\n首次查询日期：%s\n末次查询日期：%s\n查询次数：%d\n用户满意次数：%d\n",
                   current->questionId, current->questionType, current->questionContent,
                   current->questionAnswer, current->firstQueryDate, current->lastQueryDate,
                   current->queryTimes, current->userSatisfiedTimes);
            found = 1;
            recordingTimes(current,head);
        }
        current = current->next;
    }
    if (!found) {
        printf("未找到查询次数在指定范围内的问题记录！\n");
    }
}

// 按固定时段查询函数（辅助函数，供查询功能调用）
void queryByFixedPeriod(Question *head, const char *startDate, const char *endDate) {
    Question *current = head;
    int found = 0;
    while (current!= NULL) {
        if (strcmp(current->firstQueryDate, startDate) >= 0 && strcmp(current->lastQueryDate, endDate) <= 0) {
            printf("问题编号：%d\n问题类型：%d\n问题内容：%s\n问题答案：%s\n首次查询日期：%s\n末次查询日期：%s\n查询次数：%d\n用户满意次数：%d\n",
                   current->questionId, current->questionType, current->questionContent,
                   current->questionAnswer, current->firstQueryDate, current->lastQueryDate,
                   current->queryTimes, current->userSatisfiedTimes);
            found = 1;
            recordingTimes(current,head);
        }
        current = current->next;
    }
    if (!found) {
        printf("未找到在指定固定时段内的问题记录！\n");
    }
}

// 按日期区间段查询函数（辅助函数，供查询功能调用）
void queryByDateInterval(Question *head, const char *startDate, const char *endDate) {
    Question *current = head;
    int found = 0;
    while (current!= NULL) {
        if (strcmp(current->firstQueryDate, startDate) >= 0 && strcmp(current->lastQueryDate, endDate) <= 0) {
            printf("问题编号：%d\n问题类型：%d\n问题内容：%s\n问题答案：%s\n首次查询日期：%s\n末次查询日期：%s\n查询次数：%d\n用户满意次数：%d\n",
                   current->questionId, current->questionType, current->questionContent,
                   current->questionAnswer, current->firstQueryDate, current->lastQueryDate,
                   current->queryTimes, current->userSatisfiedTimes);
            found = 1;
            recordingTimes(current,head);
        }
        current = current->next;
    }
    if (!found) {
        printf("未找到在指定日期区间内的问题记录！\n");
    }
}

// 查询已删除记录函数（辅助函数，供查询功能调用，管理员权限）
void queryDeletedRecords(Question *head) {
    FILE *file_undel = fopen("question_undel.dat", "r");
    if (file_undel == NULL) {
        printf("无法打开文件 question_undel.dat，请检查文件是否存在及权限设置！\n");
        return;
    }
    Question temp;
    while (fscanf(file_undel, "%d %d %s %s %s %s %d %d", &temp.questionId, &temp.questionType,
                  temp.questionContent, temp.questionAnswer, temp.firstQueryDate,
                  temp.lastQueryDate, &temp.queryTimes, &temp.userSatisfiedTimes) == 8) {
        printf("问题编号：%d\n问题类型：%d\n问题内容：%s\n问题答案：%s\n首次查询日期：%s\n末次查询日期：%s\n查询次数：%d\n用户满意次数：%d\n",
               temp.questionId, temp.questionType, temp.questionContent,
               temp.questionAnswer, temp.firstQueryDate, temp.lastQueryDate,
               temp.queryTimes, temp.userSatisfiedTimes);
    }
    fclose(file_undel);
}

// 查询功能函数，根据不同条件查询
void queryQuestions(Question *head) {
    int choice;
    printf("**************************************************************************\n");
    printf("1-----按问题类型查询 2-----按问题内容查询   3-----按查询次数查询\n");
    printf("4-----按固定时段查询 5-----按日期区间段查询 \n");
    printf("6-----已删除记录查询（管理员才有此项功能） \n");
    printf("**************************************************************************\n");
    scanf("%d", &choice);
    printf("**************************************************************************\n");
    switch (choice) {
        case 1: {
            int questionType;
            printf("请输入要查询的问题类型（1 - anyview的常见问题 2 - 智慧树的常见问题\n3 - 编译器的常见问题 4 - 常见语法错误 5 - 常见算法错误）");
            printf("\n**************************************************************************\n");
            scanf("%d", &questionType);
            queryByQuestionType(head, questionType);
            break;
        }
        case 2: {
            char content[100];
            printf("请输入要查询的问题内容（关键词）：");
            scanf("%s", content);
            queryByQuestionContent(head, content);
            break;
        }
        case 3: {
            int minTimes, maxTimes;
            printf("请输入查询次数的最小值和最大值（用空格隔开）：");
            scanf("%d %d", &minTimes, &maxTimes);
            queryByQueryTimesRange(head, minTimes, maxTimes);
            break;
        }
        case 4: {
            char startDate[20], endDate[20];
            printf("请输入固定时段的开始日期和结束日期（例如2000-01-01 2000-02-01）：");
            scanf("%s %s", startDate, endDate);
            queryByFixedPeriod(head, startDate, endDate);
            break;
        }
        case 5: {
            char startDate[20], endDate[20];
            printf("请输入日期区间段的开始日期和结束日期（例如2000-01-01 2000-02-01）：");
            scanf("%s %s", startDate, endDate);
            queryByDateInterval(head, startDate, endDate);
            break;
        }
        case 6: {
            if (isAdmin) {
                queryDeletedRecords(head);
            } else {
                printf("您无此权限进行已删除记录查询！\n");
            }
            break;
        }
        default:
            printf("无效的选择，请重新输入！\n");
    }
}

// 交换两个节点数据（辅助函数，用于排序）
void swap(Question *a, Question *b) {
	Question *p1 = a->next;
	Question *p2 = b->next;
    Question temp = *a;
    *a = *b;
    *b = temp;
    a->next = p1;
    b->next = p2;
}

// 按查询次数降序排序函数
void sortByQueryTimes(Question *head) {
    int cnt=1;
    Question *ptr = head;
    if (head == NULL) {
        return;
    }
    while (ptr->next!= NULL){
    	ptr = ptr->next;
    	cnt++;
	}
	for(int i=0;i<cnt-1;i++){
		ptr = head;
		for(int j=0;j<cnt-i-1;j++){
			int qt1 = ptr->queryTimes;
        	int qt2 = ptr->next->queryTimes;
            if (qt1 < qt2 || (qt1 == qt2 && ptr->userSatisfiedTimes < ptr->next->userSatisfiedTimes)) {
                swap(ptr, ptr->next);
            }
            ptr = ptr->next;
		}
	}
}

// 按用户满意程度降序排序函数（辅助函数，用于排序）
void sortByUserSatisfaction(Question *head) {
    int cnt=1;
    Question *ptr = head;
    if (head == NULL) {
        return;
    }
    while (ptr->next!= NULL){
    	ptr = ptr->next;
    	cnt++;
	}
	for(int i=0;i<cnt-1;i++){
		ptr = head;
		for(int j=0;j<cnt-i-1;j++){
			double sat1 = (double)ptr->userSatisfiedTimes / (double)ptr->queryTimes;
            double sat2 = (double)ptr->next->userSatisfiedTimes / (double)ptr->next->queryTimes;
            if (sat1 < sat2 || (sat1 == sat2 && ptr->queryTimes < ptr->next->queryTimes)) {
                swap(ptr, ptr->next);
            }
            ptr = ptr->next;
		}
	}
}

// 排序功能函数，根据选择调用相应排序逻辑
void sortQuestions(Question *head) {
    int choice;
    printf("请选择排序依据（1 - 按查询次数排序，2 - 按用户满意程度排序）：");
    scanf("%d", &choice);
    printf("\n**************************************************************************\n");
    switch (choice) {
        case 1:
            sortByQueryTimes(head);
            break;
        case 2:
            sortByUserSatisfaction(head);
            break;
        default:
            printf("无效的排序依据选择，请重新输入！\n");
            return;
    }
    displayAllQuestions(head);
}

// 数据录入函数，管理员录入新问题
void inputQuestions(Question **head) {
    if (isAdmin) {
        int numRecords;
        printf("请输入要录入的记录条数：");
        scanf("%d", &numRecords);
        FILE *file = fopen("C:\\Users\\AijiA\\Desktop\\24级12班 3124004474 李忠林\\2、源程序及所需文件\\question.txt", "a");  // 以追加模式打开原问题文件
        if (file == NULL) {
            printf("无法打开文件 question.txt 进行写入，请检查文件权限等问题！\n");
            return;
        }
        for (int i = 0; i < numRecords; i++) {
            Question *newNode = (Question *)malloc(sizeof(Question));
            if (newNode == NULL) {
                printf("内存分配失败，录入中断！\n");
                fclose(file);
                return;
            }
            printf("请输入问题编号：");
            scanf("%d", &newNode->questionId);
            printf("请输入问题类型（1 - anyview的常见问题 2 - 智慧树的常见问题 3 - 编译器的常见问题\n4 - 常见语法错误 5 - 常见算法错误）：");
            scanf("%d", &newNode->questionType);
            while(newNode->questionType>5||newNode->questionType<1){
            	printf("输入有误！请重新输入："); 
            	scanf("%d", &newNode->questionType);
			}
            printf("请输入问题内容：");
            scanf("%s", newNode->questionContent);
            printf("请输入问题答案（最多输入150字）：");
            scanf("%s", newNode->questionAnswer);
            // 获取当前时间作为首次查询日期和末次查询日期
            time_t t = time(NULL);
            struct tm *tm_info = localtime(&t);
            strftime(newNode->firstQueryDate, sizeof(newNode->firstQueryDate), "%Y-%m-%d", tm_info);
            strcpy(newNode->lastQueryDate, newNode->firstQueryDate);
            newNode->queryTimes = 0;
            newNode->userSatisfiedTimes = 0;
            // 将新录入的数据写入文件
            fprintf(file, "%d %d %s %s %s %s %d %d\n", newNode->questionId, newNode->questionType,
                    newNode->questionContent, newNode->questionAnswer, newNode->firstQueryDate,
                    newNode->lastQueryDate, newNode->queryTimes, newNode->userSatisfiedTimes);

            newNode->next = *head;
            *head = newNode;
        }
        fclose(file);
        printf("已成功录入！\n");
    } else {
        printf("您无此权限进行数据录入！\n");
    }
}

// 显示所有问题记录函数
void displayAllQuestions(Question *head) {
    Question *current = head;
    while (current!= NULL) {
        printf("问题编号：%d\n问题类型：%d\n问题内容：%s\n问题答案：%s\n首次查询日期：%s\n末次查询日期：%s\n查询次数：%d\n用户满意次数：%d\n\n",
               current->questionId, current->questionType, current->questionContent,
               current->questionAnswer, current->firstQueryDate, current->lastQueryDate,
               current->queryTimes, current->userSatisfiedTimes);
        current = current->next;
    }
}

// 删除指定编号的问题节点（辅助函数，用于数据变更）
void deleteQuestion(Question **head, int questionId) {
    Question *prev = NULL;
    Question *current = *head;
    while (current!= NULL && current->questionId!= questionId) {
        prev = current;
        current = current->next;
    }
    if (current == NULL) {
        printf("未找到对应问题编号的记录，无法删除！\n");
        return;
    }
    if (prev == NULL) {
        *head = current->next;
    } else {
        prev->next = current->next;
    }
    // 将删除的信息保存到另外文件question_undel.dat
    FILE *file_undel = fopen("C:\\Users\\AijiA\\Desktop\\24级12班 3124004474 李忠林\\2、源程序及所需文件\\question_undel.dat", "a");
    if (file_undel == NULL) {
        printf("无法打开文件 question_undel.dat 进行写入，请检查文件权限等问题！\n");
        free(current);
        return;
    }
    fprintf(file_undel, "%d %d %s %s %s %s %d %d\n", current->questionId, current->questionType,
            current->questionContent, current->questionAnswer, current->firstQueryDate,
            current->lastQueryDate, current->queryTimes, current->userSatisfiedTimes);
    fclose(file_undel);
    free(current);
    // 更新原文件，去除已删除的记录
    FILE *file = fopen("C:\\Users\\AijiA\\Desktop\\24级12班 3124004474 李忠林\\2、源程序及所需文件\\question.txt", "w");
    if (file == NULL) {
        perror("无法打开文件 question.txt 进行写入");
        free(current);
        return;
    }
    Question *temp = *head;
    while (temp!= NULL) {
        fprintf(file, "%d %d %s %s %s %s %d %d\n", temp->questionId, temp->questionType,
                temp->questionContent, temp->questionAnswer, temp->firstQueryDate,
                temp->lastQueryDate, temp->queryTimes, temp->userSatisfiedTimes);
        temp = temp->next;
    }
    printf("删除成功！\n");
    fclose(file);
    //free(current);
}

// 数据变更（更改或删除）函数，管理员操作
void updateQuestions(Question **head) {
    if (isAdmin) {
        int choice;
        printf("**************************************************************************\n");
        printf("1 - 更改记录  2 - 删除记录\n");
        printf("**************************************************************************\n");
        scanf("%d", &choice);
        printf("**************************************************************************\n");
        switch (choice) {
            case 1: {
                int questionId;
                printf("请输入要更改的问题编号：");
                scanf("%d", &questionId);
                Question *current = *head;
                while (current!= NULL && current->questionId!= questionId) {
                    current = current->next;
                }
                if (current == NULL) {
                    printf("未找到对应问题编号的记录，无法更改！\n");
                    return;
                }
                printf("请输入新的问题内容：");
                scanf("%s", current->questionContent);
                printf("请输入新的问题答案：");
                scanf("%s", current->questionAnswer);
				// 将更新后的数据写回文件
			    FILE *file = fopen("C:\\Users\\AijiA\\Desktop\\24级12班 3124004474 李忠林\\2、源程序及所需文件\\question.txt", "w");
			    if (file == NULL) {
			        perror("无法打开文件 question.txt 进行写入");
			        return;
			    }
			    Question *temp = *head;
			    while (temp!= NULL) {
			        if (fprintf(file, "%d %d %s %s %s %s %d %d\n", temp->questionId, temp->questionType,
			                    temp->questionContent, temp->questionAnswer, temp->firstQueryDate,
			                    temp->lastQueryDate, temp->queryTimes, temp->userSatisfiedTimes) < 0) {
			            perror("写入 question.txt 文件失败");
			            break;
			        }
			        temp = temp->next;
			    }
			    fclose(file);
                break;
            }
            case 2: {
                int questionId;
                printf("请输入要删除的问题编号：");
                scanf("%d", &questionId);
                deleteQuestion(head, questionId);
                break;
            }
            default:
                printf("无效的选择，请重新输入！\n");
        }
    } else {
        printf("您无此权限进行数据变更！\n");
    }
}

// 统计功能函数，区分用户和管理员统计
void statistics(Question *head) {
    if (isAdmin) {
        // 管理员统计各个用户在自定义时间段里的求助情况
        char startDate[20];
        char endDate[20];
        printf("请输入管理员统计的开始日期和结束日期（用空格隔开）：");
        scanf("%s %s", startDate, endDate);
        Question *current = head;
        int totalQueryTimes = 0;
        while (current!= NULL) {
            if (strcmp(current->firstQueryDate, startDate) >= 0 && strcmp(current->lastQueryDate, endDate) <= 0) {
                totalQueryTimes += current->queryTimes;
            }
            current = current->next;
        }
        printf("在管理员指定自定义时间段内的总查询次数：%d\n", totalQueryTimes);
    } else {
        // 用户统计自己在一个自定义时间段里的求助情况
        char startDate[20];
        char endDate[20];
        printf("请输入用户统计的开始日期和结束日期（用空格隔开）：");
        scanf("%s %s", startDate, endDate);
        Question *current = head;
        int totalQueryTimes = 0;
        while (current!= NULL) {
            if (strcmp(current->firstQueryDate, startDate) >= 0 && strcmp(current->lastQueryDate, endDate) <= 0) {
                totalQueryTimes += current->queryTimes;
            }
            current = current->next;
        }
        printf("在用户指定自定义时间段内的总查询次数：%d\n", totalQueryTimes);
    }
}

// 判断是否为管理员权限的函数
void isAdm() {
	char password[]="123abc";
	char yesSkip[]="是"; 
	char notSkip[]="否";
	char str[7];
	char prev_isSkip[3];
	char curr_isSkip[3];
	printf("是否进行管理员登录（是或否）：");
	scanf("%s",prev_isSkip);
	if(strcmp(yesSkip, prev_isSkip) != 0 && strcmp(notSkip, prev_isSkip) != 0){
		printf("输入错误！请重新输入（是或否）：");
		scanf("%s",prev_isSkip);
	}
	if(strcmp(yesSkip, prev_isSkip) == 0){
		printf("请输入密码：");
		scanf("%s",str);
		if(strcmp(password, str) == 0){
			printf("登陆成功！您已成为管理员！\n"); 
			isAdmin=1;
		}else{
			printf("密码错误！是否重新登录:");
			scanf("%s",curr_isSkip);
			if(strcmp(yesSkip, curr_isSkip) == 0){
				printf("请输入密码：");
				scanf("%s",str);
				if(strcmp(password, str) == 0){
					printf("登陆成功！您已成为管理员！\n"); 
					isAdmin=1;
				}else{
					printf("登陆失败！已默认您为用户！\n"); 
				}
			}
		}
	}
}
