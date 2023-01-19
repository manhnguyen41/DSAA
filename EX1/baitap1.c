#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int n;

typedef struct phoneNumbers
{
    char PhoneNumber[100];
    struct phoneNumbers *next;
} phoneNumbers;

typedef struct emails
{
    char Email[100];
    struct emails *next;
} emails;

typedef struct person
{
    char name[100];
    char img[100];
    phoneNumbers *sdt;
    emails *email;
    struct person *next;
} person;

person *root = NULL;

person *makeNode(char *name, char *img, phoneNumbers *sdt, emails *email)
{
    person *p = (person *)malloc(sizeof(person));
    strcpy(p->name, name);
    strcpy(p->img, img);
    p->sdt = sdt;
    p->email = email;
    p->next = NULL;
    return p;
}

phoneNumbers *makeSdt(char *sdt)
{
    phoneNumbers *Sdt = (phoneNumbers *)malloc(sizeof(phoneNumbers));
    strcpy(Sdt->PhoneNumber, sdt);
    Sdt->next = NULL;
    return Sdt;
}

emails *makeEmail(char *email)
{
    emails *e = (emails *)malloc(sizeof(emails));
    strcpy(e->Email, email);
    e->next = NULL;
    return e;
}

person *insertLast(char *name, char *img, phoneNumbers *sdt, emails *email)
{
    person *p = root;
    if (root == NULL)
    {
        return makeNode(name, img, sdt, email);
    }
    while (p->next != NULL)
        p = p->next;
    person *q = makeNode(name, img, sdt, email);
    p->next = q;
    return root;
}

person *findName(char *name)
{
    person *p = root;
    while (p != NULL)
    {
        if (strstr(p->name, name) != NULL)
        {
            return p;
        }
        p = p->next;
    }
    return NULL;
}

person *findSdt(char *sdt)
{
    person *p = root;
    while (p != NULL)
    {
        phoneNumbers *n = p->sdt;
        while (n != NULL)
        {
            if (!strcmp(n->PhoneNumber, sdt))
            {
                return p;
            }
            n = n->next;
        }
        p = p->next;
    }
    return NULL;
}

void readFromFile(char *fileName)
{
    FILE *f = fopen(fileName, "r+");
    char s[100] = "";
    fscanf(f, "%d\n", &n);
    for (int i = 1; i <= n; i++)
    {
        char name[100];
        char img[100];
        if (strlen(s) != 0)
        {
            strcpy(name, s);
        }
        else
        {
            fgets(name, 100, f);
            name[strlen(name) - 1] = '\0';
        }
        fgets(img, 100, f);
        img[strlen(img) - 1] = '\0';
        strcpy(img, img + 4 * sizeof(char));
        phoneNumbers *rsdt = NULL;
        emails *remails = NULL;
        char number[100];
        char e[100];
        do
        {
            if (!fgets(s, 100, f))
                break;
            if (s[strlen(s) - 1] == '\n')
                s[strlen(s) - 1] = '\0';
            phoneNumbers *sdt = rsdt;
            emails *email = remails;
            if ((s[0] != 'T' && s[0] != 'E') || (s[1] < 48 || s[1] > 57))
                break;
            if (s[0] == 'T')
            {
                strcpy(number, s + 3 * sizeof(char));
                if (rsdt == NULL)
                {
                    rsdt = makeSdt(number);
                }
                else
                {
                    while (sdt->next != NULL)
                    {
                        sdt = sdt->next;
                    }
                    sdt->next = makeSdt(number);
                }
            }
            if (s[0] == 'E')
            {
                strcpy(e, s + 3 * sizeof(char));
                if (remails == NULL)
                {
                    remails = makeEmail(e);
                }
                else
                {
                    while (email->next != NULL)
                    {
                        email = email->next;
                    }
                    email->next = makeEmail(e);
                }
            }
        } while (1);
        root = insertLast(name, img, rsdt, remails);
    }
    fclose(f);
}

void writeToFile(char *fileName)
{
    FILE *f = fopen(fileName, "w+");
    fprintf(f, "%d", n);
    person *p = root;
    for (int i = 1; i <= n; i++)
    {
        fprintf(f, "\n%s", p->name);
        fprintf(f, "\nImg:%s", p->img);
        phoneNumbers *sdt = p->sdt;
        int j = 1;
        while (sdt != NULL)
        {
            fprintf(f, "\nT%d:%s", j, sdt->PhoneNumber);
            sdt = sdt->next;
            j++;
        }
        emails *email = p->email;
        j = 1;
        while (email != NULL)
        {
            fprintf(f, "\nE%d:%s", j, email->Email);
            email = email->next;
            j++;
        }
        p = p->next;
    }
    fclose(f);
}

void removePerson(char *name)
{
    person *p1 = findName(name);
    if (p1 == root)
    {
        root = root->next;
        free(p1);
    }
    else
    {
        person *p2 = root;
        while (p2->next != p1)
        {
            p2 = p2->next;
        }
        p2->next = p1->next;
        free(p1);
    }
    n--;
}

int removeSdt(char *sdt)
{
    person *p = findSdt(sdt);
    phoneNumbers *s = p->sdt;
    if (s->next == NULL && p->email == NULL)
    {
        removePerson(p->name);
        return 0;
    }
    else
    {
        if (!strcmp(s->PhoneNumber, sdt))
        {
            p->sdt = s->next;
            free(s);
            return 1;
        }
        else
        {
            while (!strcmp(s->next->PhoneNumber, sdt))
            {
                s = s->next;
            }
            phoneNumbers *s1 = s->next;
            s->next = s1->next;
            free(s1);
            return 1;
        }
    }
}

void ProcessReadFromFile()
{
    char fileName[100];
    printf("Nhập tên file cần đọc: ");
    scanf("%s", fileName);
    readFromFile(fileName);
    printf("Đã đọc dữ liệu từ file %s\n", fileName);
}

void ProcessWriteToFile()
{
    char fileName[100];
    printf("Nhập tên file cần ghi: ");
    scanf("%s", fileName);
    writeToFile(fileName);
    printf("Đã ghi dữ liệu vào file %s\n", fileName);
}

void printInfo(person *p)
{
    printf("Thông tin của %s là:\n", p->name);
    printf("Img: %s\n", p->img);
    phoneNumbers *sdt = p->sdt;
    int j = 1;
    while (sdt != NULL)
    {
        printf("T%d:%s\n", j, sdt->PhoneNumber);
        sdt = sdt->next;
        j++;
    }
    emails *email = p->email;
    j = 1;
    while (email != NULL)
    {
        printf("E%d:%s\n", j, email->Email);
        email = email->next;
        j++;
    }
}

void ProcessFindName()
{
    char name[100];
    printf("Nhập tên người cần tìm: ");
    __fpurge(stdin);
    fgets(name, 100, stdin);
    name[strlen(name) - 1] = '\0';
    person *p = findName(name);
    if (p == NULL)
    {
        printf("Không tìm thấy người tên %s\n", name);
        ProcessFindName();
    }
    else
    {
        printInfo(p);
    }
}

void ProcessFindSdt()
{
    char number[100];
    printf("Nhập số điện thoại cần tìm: ");
    __fpurge(stdin);
    scanf("%s", number);
    person *p = findSdt(number);
    if (p == NULL)
    {
        printf("Không tìm thấy người có số điện thoại %s\n", number);
        ProcessFindSdt();
    }
    else
    {
        printInfo(p);
    }
}

void ProcessRemovePerson()
{
    char name[100];
    printf("Nhập tên người cần xóa: ");
    __fpurge(stdin);
    fgets(name, 100, stdin);
    name[strlen(name) - 1] = '\0';
    if (findName(name) == NULL)
    {
        printf("Không có %s trong danh bạ\n", name);
        ProcessRemovePerson();
    } 
    else
    {
        removePerson(name);
        printf("Đã xóa %s ra khỏi danh bạ\n", name);
    }
}

void ProcessRemoveSdt()
{
    char number[100];
    printf("Nhập số điện thoại cần xóa: ");
    __fpurge(stdin);
    scanf("%s", number);
    if (findSdt(number) == NULL)
    {
        printf("Không có %s trong danh bạ\n", number);
        ProcessRemovePerson();
    }
    else
    {
        int i = removeSdt(number);
        if (i == 0)
        {
            printf("Đã xóa %s ra khỏi danh bạ\n", findSdt(number)->name);
        } else
        {
            printf("Đã xóa %s ra khỏi danh bạ\n", number);
        }
    }
}

void ProcessUpdateInfo()
{
    char name[100];
    printf("Nhập tên người cần cập nhật: ");
    __fpurge(stdin);
    fgets(name, 100, stdin);
    name[strlen(name) - 1] = '\0';
    person *p = findName(name);
    if (p == NULL)
    {
        printf("Không có %s trong danh bạ\n", name);
        ProcessRemovePerson();
    } 
    else
    {
        char newImg[100];
        printf("Nhập đường dẫn ảnh mới: "); 
        scanf("%s", newImg);
        fflush(stdin);
        strcpy(p->img, newImg);
        phoneNumbers *sdt = p->sdt;
        int j = 1;
        while (sdt != NULL)
        {
            char newNumber[100];
            printf("Nhập số điện thoại thứ %d mới: ", j);
            scanf("%s", newNumber);
            fflush(stdin);
            strcpy(sdt->PhoneNumber, newNumber);
            sdt = sdt->next;
        }
        emails *email = p->email;
        j = 1;
        while (email != NULL)
        {
            char newEmail[100];
            printf("Nhập email thứ %d mới: ", j);
            scanf("%s", newEmail);
            fflush(stdin);
            strcpy(email->Email, newEmail);
            email = email->next;
        }
    }
}

int main()
{
    while (1)
    {
        int cmp;
        printf("1.Đọc thông tin danh bạ từ file\n");
        printf("2.Ghi thông tin danh bạ vào file\n");
        printf("3.Tìm kiếm theo tên\n");
        printf("4.Tra cứu số điện thoại\n");
        printf("5.Xóa một người trong danh bạ\n");
        printf("6.Xóa số điện thoại\n");
        printf("7.Cập nhật thông tin\n");
        printf("8.Thoát\n");
        printf("Nhập chức năng muốn thực hiện (1-8): "); __fpurge(stdin); scanf("%d", &cmp);
        switch (cmp)
        {
        case 1:
            ProcessReadFromFile();
            break;
        
        case 2:
            ProcessWriteToFile();
            break;

        case 3:
            ProcessFindName();
            break;

        case 4:
            ProcessFindSdt();
            break;

        case 5: 
            ProcessRemovePerson();
            break;

        case 6:
            ProcessRemoveSdt();
            break;
        
        case 7:
            ProcessUpdateInfo();
            break;

        default:
            break;
        }
        if (cmp == 8)
            break;
    }
    return 0;
}