#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>  // ���ڻ�ȡ��ǰʱ�䣬��ʾ���������״�/ĩ�β�ѯ���ڵļ�¼
// Ĭ�ϲ��ǹ���Ա 
int isAdmin=0;

// ��������ṹ��
typedef struct Question {
    int questionId;  // ������
    int questionType;  // ��������
    char questionContent[100];  // ��������
    char questionAnswer[301];  // �����
    char firstQueryDate[20];  // �״β�ѯ����
    char lastQueryDate[20];  // ĩ�β�ѯ����
    int queryTimes;  // ��ѯ����
    int userSatisfiedTimes;  // �û��������
    struct Question *next;  // ָ����һ���ڵ��ָ�룬��������
} Question;

// ���ļ���ȡ�������ݵ�����
Question *readQuestionsFromFile(const char *filename);

// ����ѡ��ִ����Ӧ��ѯ���ܵĺ���
void queryQuestions(Question *head);

// �����ܺ�������ָ��������������
void sortQuestions(Question *head);

// ����¼�뺯��������Ա¼��������
void inputQuestions(Question **head);

// ��ʾ���������¼����
void displayAllQuestions(Question *head);

// ���ݱ�������Ļ�ɾ��������������Ա����
void updateQuestions(Question **head);

// ͳ�ƹ��ܺ����������û��͹���Աͳ��
void statistics(Question *head);

// �ж��Ƿ�Ϊ����ԱȨ�޵ĺ���
void isAdm();

// ������ 
int main() {
	// �ж��Ƿ�Ϊ����Ա 
	isAdm();
    //isAdmin=1; 
	// ���ļ���ȡ�������ݵ�����
    Question *questionList = readQuestionsFromFile("C:\\Users\\AijiA\\Desktop\\24��12�� 3124004474 ������\\2��Դ���������ļ�\\question.txt");
    int choice;
    do {
        printf("**************************************************************************\n");
        printf("1------��ѯ   2------����   3------ͳ��   4------¼��   5------���� \n6------�˳�\n");
        printf("**************************************************************************\n");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
            	// ��ѯ 
                queryQuestions(questionList);
                break;
            case 2:
            	// ���� 
                sortQuestions(questionList);
                break;
            case 3:
            	// ͳ�ƹ��ܺ����������û��͹���Աͳ��
                statistics(questionList);
                break;
            case 4:
            	// ����¼�뺯��������Ա¼��������
                inputQuestions(&questionList);
                break;
            case 5:
            	// ���ݱ�������Ļ�ɾ��������������Ա����
                updateQuestions(&questionList);
                break;
            case 6:
            	// �˳� 
            	break;
            default:
                printf("��Ч��ѡ�����������룡\n");
        }
    } while (choice >= 1 && choice <= 5);

    // �ͷ������ڴ�
    Question *current = questionList;
    Question *next;
    while (current!= NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    return 0;
}

// ���ļ���ȡ�������ݵ�����
Question *readQuestionsFromFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("�޷����ļ� %s�������ļ��Ƿ���ڼ�Ȩ�����ã�\n", filename);
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
            printf("�ڴ����ʧ�ܣ�\n");
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

// ��¼��ѯ�������������
void recordingTimes(Question *current, Question *head) {
    current->queryTimes++;
	char satisDegree[]="��";
	char isSatis[3];
	printf("�Ƿ����Ȿ�β�ѯ���ǻ�񣩣�"); 
	scanf("%s",isSatis);
	if(strcmp(satisDegree, isSatis) == 0){
	current->userSatisfiedTimes++;	
	}
    // �����º������д���ļ�
    FILE *file = fopen("C:\\Users\\AijiA\\Desktop\\24��12�� 3124004474 ������\\2��Դ���������ļ�\\question.txt", "w");
    if (file == NULL) {
        perror("�޷����ļ� question.txt ����д��");
        return;
    }
    Question *temp = head;
    while (temp!= NULL) {
        if (fprintf(file, "%d %d %s %s %s %s %d %d\n", temp->questionId, temp->questionType,
                    temp->questionContent, temp->questionAnswer, temp->firstQueryDate,
                    temp->lastQueryDate, temp->queryTimes, temp->userSatisfiedTimes) < 0) {
            perror("д�� question.txt �ļ�ʧ��");
            break;
        }
        temp = temp->next;
    }
    fclose(file);
}

// ���������Ͳ�ѯ��������������������ѯ���ܵ��ã�
void queryByQuestionType(Question *head, int questionType) {
    Question *current = head;
    int found = 0;
    while (current!= NULL) {
        if (current->questionType == questionType) {
            printf("�����ţ�%d\n�������ͣ�%d\n�������ݣ�%s\n����𰸣�%s\n�״β�ѯ���ڣ�%s\nĩ�β�ѯ���ڣ�%s\n��ѯ������%d\n�û����������%d\n",
                   current->questionId, current->questionType, current->questionContent,
                   current->questionAnswer, current->firstQueryDate, current->lastQueryDate,
                   current->queryTimes, current->userSatisfiedTimes);
            found = 1;
            recordingTimes(current,head);
        }
        current = current->next;
    }
    if (!found) {
        printf("δ�ҵ����������͵ļ�¼��\n");
    }
}

// ����������ģ����ѯ��������������������ѯ���ܵ��ã�
void queryByQuestionContent(Question *head, const char *content) {
    Question *current = head;
    int found = 0;
    while (current!= NULL) {
        if (strstr(current->questionContent, content)!= NULL) {
            printf("�����ţ�%d\n�������ͣ�%d\n�������ݣ�%s\n����𰸣�%s\n�״β�ѯ���ڣ�%s\nĩ�β�ѯ���ڣ�%s\n��ѯ������%d\n�û����������%d\n",
                   current->questionId, current->questionType, current->questionContent,
                   current->questionAnswer, current->firstQueryDate, current->lastQueryDate,
                   current->queryTimes, current->userSatisfiedTimes);
            found = 1;
            recordingTimes(current,head);
        }
        current = current->next;
    }
    if (!found) {
        printf("δ�ҵ����������ݵ������¼��\n");
    }
}

// ����ѯ������Χ��ѯ��������������������ѯ���ܵ��ã�
void queryByQueryTimesRange(Question *head, int minTimes, int maxTimes) {
    Question *current = head;
    int found = 0;
    while (current!= NULL) {
        if (current->queryTimes >= minTimes && current->queryTimes <= maxTimes) {
            printf("�����ţ�%d\n�������ͣ�%d\n�������ݣ�%s\n����𰸣�%s\n�״β�ѯ���ڣ�%s\nĩ�β�ѯ���ڣ�%s\n��ѯ������%d\n�û����������%d\n",
                   current->questionId, current->questionType, current->questionContent,
                   current->questionAnswer, current->firstQueryDate, current->lastQueryDate,
                   current->queryTimes, current->userSatisfiedTimes);
            found = 1;
            recordingTimes(current,head);
        }
        current = current->next;
    }
    if (!found) {
        printf("δ�ҵ���ѯ������ָ����Χ�ڵ������¼��\n");
    }
}

// ���̶�ʱ�β�ѯ��������������������ѯ���ܵ��ã�
void queryByFixedPeriod(Question *head, const char *startDate, const char *endDate) {
    Question *current = head;
    int found = 0;
    while (current!= NULL) {
        if (strcmp(current->firstQueryDate, startDate) >= 0 && strcmp(current->lastQueryDate, endDate) <= 0) {
            printf("�����ţ�%d\n�������ͣ�%d\n�������ݣ�%s\n����𰸣�%s\n�״β�ѯ���ڣ�%s\nĩ�β�ѯ���ڣ�%s\n��ѯ������%d\n�û����������%d\n",
                   current->questionId, current->questionType, current->questionContent,
                   current->questionAnswer, current->firstQueryDate, current->lastQueryDate,
                   current->queryTimes, current->userSatisfiedTimes);
            found = 1;
            recordingTimes(current,head);
        }
        current = current->next;
    }
    if (!found) {
        printf("δ�ҵ���ָ���̶�ʱ���ڵ������¼��\n");
    }
}

// ����������β�ѯ��������������������ѯ���ܵ��ã�
void queryByDateInterval(Question *head, const char *startDate, const char *endDate) {
    Question *current = head;
    int found = 0;
    while (current!= NULL) {
        if (strcmp(current->firstQueryDate, startDate) >= 0 && strcmp(current->lastQueryDate, endDate) <= 0) {
            printf("�����ţ�%d\n�������ͣ�%d\n�������ݣ�%s\n����𰸣�%s\n�״β�ѯ���ڣ�%s\nĩ�β�ѯ���ڣ�%s\n��ѯ������%d\n�û����������%d\n",
                   current->questionId, current->questionType, current->questionContent,
                   current->questionAnswer, current->firstQueryDate, current->lastQueryDate,
                   current->queryTimes, current->userSatisfiedTimes);
            found = 1;
            recordingTimes(current,head);
        }
        current = current->next;
    }
    if (!found) {
        printf("δ�ҵ���ָ�����������ڵ������¼��\n");
    }
}

// ��ѯ��ɾ����¼��������������������ѯ���ܵ��ã�����ԱȨ�ޣ�
void queryDeletedRecords(Question *head) {
    FILE *file_undel = fopen("question_undel.dat", "r");
    if (file_undel == NULL) {
        printf("�޷����ļ� question_undel.dat�������ļ��Ƿ���ڼ�Ȩ�����ã�\n");
        return;
    }
    Question temp;
    while (fscanf(file_undel, "%d %d %s %s %s %s %d %d", &temp.questionId, &temp.questionType,
                  temp.questionContent, temp.questionAnswer, temp.firstQueryDate,
                  temp.lastQueryDate, &temp.queryTimes, &temp.userSatisfiedTimes) == 8) {
        printf("�����ţ�%d\n�������ͣ�%d\n�������ݣ�%s\n����𰸣�%s\n�״β�ѯ���ڣ�%s\nĩ�β�ѯ���ڣ�%s\n��ѯ������%d\n�û����������%d\n",
               temp.questionId, temp.questionType, temp.questionContent,
               temp.questionAnswer, temp.firstQueryDate, temp.lastQueryDate,
               temp.queryTimes, temp.userSatisfiedTimes);
    }
    fclose(file_undel);
}

// ��ѯ���ܺ��������ݲ�ͬ������ѯ
void queryQuestions(Question *head) {
    int choice;
    printf("**************************************************************************\n");
    printf("1-----���������Ͳ�ѯ 2-----���������ݲ�ѯ   3-----����ѯ������ѯ\n");
    printf("4-----���̶�ʱ�β�ѯ 5-----����������β�ѯ \n");
    printf("6-----��ɾ����¼��ѯ������Ա���д���ܣ� \n");
    printf("**************************************************************************\n");
    scanf("%d", &choice);
    printf("**************************************************************************\n");
    switch (choice) {
        case 1: {
            int questionType;
            printf("������Ҫ��ѯ���������ͣ�1 - anyview�ĳ������� 2 - �ǻ����ĳ�������\n3 - �������ĳ������� 4 - �����﷨���� 5 - �����㷨����");
            printf("\n**************************************************************************\n");
            scanf("%d", &questionType);
            queryByQuestionType(head, questionType);
            break;
        }
        case 2: {
            char content[100];
            printf("������Ҫ��ѯ���������ݣ��ؼ��ʣ���");
            scanf("%s", content);
            queryByQuestionContent(head, content);
            break;
        }
        case 3: {
            int minTimes, maxTimes;
            printf("�������ѯ��������Сֵ�����ֵ���ÿո��������");
            scanf("%d %d", &minTimes, &maxTimes);
            queryByQueryTimesRange(head, minTimes, maxTimes);
            break;
        }
        case 4: {
            char startDate[20], endDate[20];
            printf("������̶�ʱ�εĿ�ʼ���ںͽ������ڣ�����2000-01-01 2000-02-01����");
            scanf("%s %s", startDate, endDate);
            queryByFixedPeriod(head, startDate, endDate);
            break;
        }
        case 5: {
            char startDate[20], endDate[20];
            printf("��������������εĿ�ʼ���ںͽ������ڣ�����2000-01-01 2000-02-01����");
            scanf("%s %s", startDate, endDate);
            queryByDateInterval(head, startDate, endDate);
            break;
        }
        case 6: {
            if (isAdmin) {
                queryDeletedRecords(head);
            } else {
                printf("���޴�Ȩ�޽�����ɾ����¼��ѯ��\n");
            }
            break;
        }
        default:
            printf("��Ч��ѡ�����������룡\n");
    }
}

// ���������ڵ����ݣ�������������������
void swap(Question *a, Question *b) {
	Question *p1 = a->next;
	Question *p2 = b->next;
    Question temp = *a;
    *a = *b;
    *b = temp;
    a->next = p1;
    b->next = p2;
}

// ����ѯ��������������
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

// ���û�����̶Ƚ�����������������������������
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

// �����ܺ���������ѡ�������Ӧ�����߼�
void sortQuestions(Question *head) {
    int choice;
    printf("��ѡ���������ݣ�1 - ����ѯ��������2 - ���û�����̶����򣩣�");
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
            printf("��Ч����������ѡ�����������룡\n");
            return;
    }
    displayAllQuestions(head);
}

// ����¼�뺯��������Ա¼��������
void inputQuestions(Question **head) {
    if (isAdmin) {
        int numRecords;
        printf("������Ҫ¼��ļ�¼������");
        scanf("%d", &numRecords);
        FILE *file = fopen("C:\\Users\\AijiA\\Desktop\\24��12�� 3124004474 ������\\2��Դ���������ļ�\\question.txt", "a");  // ��׷��ģʽ��ԭ�����ļ�
        if (file == NULL) {
            printf("�޷����ļ� question.txt ����д�룬�����ļ�Ȩ�޵����⣡\n");
            return;
        }
        for (int i = 0; i < numRecords; i++) {
            Question *newNode = (Question *)malloc(sizeof(Question));
            if (newNode == NULL) {
                printf("�ڴ����ʧ�ܣ�¼���жϣ�\n");
                fclose(file);
                return;
            }
            printf("�����������ţ�");
            scanf("%d", &newNode->questionId);
            printf("�������������ͣ�1 - anyview�ĳ������� 2 - �ǻ����ĳ������� 3 - �������ĳ�������\n4 - �����﷨���� 5 - �����㷨���󣩣�");
            scanf("%d", &newNode->questionType);
            while(newNode->questionType>5||newNode->questionType<1){
            	printf("�����������������룺"); 
            	scanf("%d", &newNode->questionType);
			}
            printf("�������������ݣ�");
            scanf("%s", newNode->questionContent);
            printf("����������𰸣��������150�֣���");
            scanf("%s", newNode->questionAnswer);
            // ��ȡ��ǰʱ����Ϊ�״β�ѯ���ں�ĩ�β�ѯ����
            time_t t = time(NULL);
            struct tm *tm_info = localtime(&t);
            strftime(newNode->firstQueryDate, sizeof(newNode->firstQueryDate), "%Y-%m-%d", tm_info);
            strcpy(newNode->lastQueryDate, newNode->firstQueryDate);
            newNode->queryTimes = 0;
            newNode->userSatisfiedTimes = 0;
            // ����¼�������д���ļ�
            fprintf(file, "%d %d %s %s %s %s %d %d\n", newNode->questionId, newNode->questionType,
                    newNode->questionContent, newNode->questionAnswer, newNode->firstQueryDate,
                    newNode->lastQueryDate, newNode->queryTimes, newNode->userSatisfiedTimes);

            newNode->next = *head;
            *head = newNode;
        }
        fclose(file);
        printf("�ѳɹ�¼�룡\n");
    } else {
        printf("���޴�Ȩ�޽�������¼�룡\n");
    }
}

// ��ʾ���������¼����
void displayAllQuestions(Question *head) {
    Question *current = head;
    while (current!= NULL) {
        printf("�����ţ�%d\n�������ͣ�%d\n�������ݣ�%s\n����𰸣�%s\n�״β�ѯ���ڣ�%s\nĩ�β�ѯ���ڣ�%s\n��ѯ������%d\n�û����������%d\n\n",
               current->questionId, current->questionType, current->questionContent,
               current->questionAnswer, current->firstQueryDate, current->lastQueryDate,
               current->queryTimes, current->userSatisfiedTimes);
        current = current->next;
    }
}

// ɾ��ָ����ŵ�����ڵ㣨�����������������ݱ����
void deleteQuestion(Question **head, int questionId) {
    Question *prev = NULL;
    Question *current = *head;
    while (current!= NULL && current->questionId!= questionId) {
        prev = current;
        current = current->next;
    }
    if (current == NULL) {
        printf("δ�ҵ���Ӧ�����ŵļ�¼���޷�ɾ����\n");
        return;
    }
    if (prev == NULL) {
        *head = current->next;
    } else {
        prev->next = current->next;
    }
    // ��ɾ������Ϣ���浽�����ļ�question_undel.dat
    FILE *file_undel = fopen("C:\\Users\\AijiA\\Desktop\\24��12�� 3124004474 ������\\2��Դ���������ļ�\\question_undel.dat", "a");
    if (file_undel == NULL) {
        printf("�޷����ļ� question_undel.dat ����д�룬�����ļ�Ȩ�޵����⣡\n");
        free(current);
        return;
    }
    fprintf(file_undel, "%d %d %s %s %s %s %d %d\n", current->questionId, current->questionType,
            current->questionContent, current->questionAnswer, current->firstQueryDate,
            current->lastQueryDate, current->queryTimes, current->userSatisfiedTimes);
    fclose(file_undel);
    free(current);
    // ����ԭ�ļ���ȥ����ɾ���ļ�¼
    FILE *file = fopen("C:\\Users\\AijiA\\Desktop\\24��12�� 3124004474 ������\\2��Դ���������ļ�\\question.txt", "w");
    if (file == NULL) {
        perror("�޷����ļ� question.txt ����д��");
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
    printf("ɾ���ɹ���\n");
    fclose(file);
    //free(current);
}

// ���ݱ�������Ļ�ɾ��������������Ա����
void updateQuestions(Question **head) {
    if (isAdmin) {
        int choice;
        printf("**************************************************************************\n");
        printf("1 - ���ļ�¼  2 - ɾ����¼\n");
        printf("**************************************************************************\n");
        scanf("%d", &choice);
        printf("**************************************************************************\n");
        switch (choice) {
            case 1: {
                int questionId;
                printf("������Ҫ���ĵ������ţ�");
                scanf("%d", &questionId);
                Question *current = *head;
                while (current!= NULL && current->questionId!= questionId) {
                    current = current->next;
                }
                if (current == NULL) {
                    printf("δ�ҵ���Ӧ�����ŵļ�¼���޷����ģ�\n");
                    return;
                }
                printf("�������µ��������ݣ�");
                scanf("%s", current->questionContent);
                printf("�������µ�����𰸣�");
                scanf("%s", current->questionAnswer);
				// �����º������д���ļ�
			    FILE *file = fopen("C:\\Users\\AijiA\\Desktop\\24��12�� 3124004474 ������\\2��Դ���������ļ�\\question.txt", "w");
			    if (file == NULL) {
			        perror("�޷����ļ� question.txt ����д��");
			        return;
			    }
			    Question *temp = *head;
			    while (temp!= NULL) {
			        if (fprintf(file, "%d %d %s %s %s %s %d %d\n", temp->questionId, temp->questionType,
			                    temp->questionContent, temp->questionAnswer, temp->firstQueryDate,
			                    temp->lastQueryDate, temp->queryTimes, temp->userSatisfiedTimes) < 0) {
			            perror("д�� question.txt �ļ�ʧ��");
			            break;
			        }
			        temp = temp->next;
			    }
			    fclose(file);
                break;
            }
            case 2: {
                int questionId;
                printf("������Ҫɾ���������ţ�");
                scanf("%d", &questionId);
                deleteQuestion(head, questionId);
                break;
            }
            default:
                printf("��Ч��ѡ�����������룡\n");
        }
    } else {
        printf("���޴�Ȩ�޽������ݱ����\n");
    }
}

// ͳ�ƹ��ܺ����������û��͹���Աͳ��
void statistics(Question *head) {
    if (isAdmin) {
        // ����Աͳ�Ƹ����û����Զ���ʱ�������������
        char startDate[20];
        char endDate[20];
        printf("���������Աͳ�ƵĿ�ʼ���ںͽ������ڣ��ÿո��������");
        scanf("%s %s", startDate, endDate);
        Question *current = head;
        int totalQueryTimes = 0;
        while (current!= NULL) {
            if (strcmp(current->firstQueryDate, startDate) >= 0 && strcmp(current->lastQueryDate, endDate) <= 0) {
                totalQueryTimes += current->queryTimes;
            }
            current = current->next;
        }
        printf("�ڹ���Աָ���Զ���ʱ����ڵ��ܲ�ѯ������%d\n", totalQueryTimes);
    } else {
        // �û�ͳ���Լ���һ���Զ���ʱ�������������
        char startDate[20];
        char endDate[20];
        printf("�������û�ͳ�ƵĿ�ʼ���ںͽ������ڣ��ÿո��������");
        scanf("%s %s", startDate, endDate);
        Question *current = head;
        int totalQueryTimes = 0;
        while (current!= NULL) {
            if (strcmp(current->firstQueryDate, startDate) >= 0 && strcmp(current->lastQueryDate, endDate) <= 0) {
                totalQueryTimes += current->queryTimes;
            }
            current = current->next;
        }
        printf("���û�ָ���Զ���ʱ����ڵ��ܲ�ѯ������%d\n", totalQueryTimes);
    }
}

// �ж��Ƿ�Ϊ����ԱȨ�޵ĺ���
void isAdm() {
	char password[]="123abc";
	char yesSkip[]="��"; 
	char notSkip[]="��";
	char str[7];
	char prev_isSkip[3];
	char curr_isSkip[3];
	printf("�Ƿ���й���Ա��¼���ǻ�񣩣�");
	scanf("%s",prev_isSkip);
	if(strcmp(yesSkip, prev_isSkip) != 0 && strcmp(notSkip, prev_isSkip) != 0){
		printf("����������������루�ǻ�񣩣�");
		scanf("%s",prev_isSkip);
	}
	if(strcmp(yesSkip, prev_isSkip) == 0){
		printf("���������룺");
		scanf("%s",str);
		if(strcmp(password, str) == 0){
			printf("��½�ɹ������ѳ�Ϊ����Ա��\n"); 
			isAdmin=1;
		}else{
			printf("��������Ƿ����µ�¼:");
			scanf("%s",curr_isSkip);
			if(strcmp(yesSkip, curr_isSkip) == 0){
				printf("���������룺");
				scanf("%s",str);
				if(strcmp(password, str) == 0){
					printf("��½�ɹ������ѳ�Ϊ����Ա��\n"); 
					isAdmin=1;
				}else{
					printf("��½ʧ�ܣ���Ĭ����Ϊ�û���\n"); 
				}
			}
		}
	}
}
