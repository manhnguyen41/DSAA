#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int n;

// Luu sdt cua 1 nguoi bang 1 ll

typedef struct phoneNumbers
{
    char PhoneNumber[100];
    struct phoneNumbers *next;
} phoneNumbers;

// Luu email cua 1 nguoi bang 1 ll

typedef struct emails
{
    char Email[100];
    struct emails *next;
} emails;

// Luu danh ba bang 1 ll

typedef struct person
{
    char name[100];
    char img[100];
    phoneNumbers *sdt;
    emails *email;
    struct person *next;
} person;

person *root = NULL;

// Tao 1 nguoi moi

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

// Tao 1 sdt moi

phoneNumbers *makeSdt(char *sdt)
{
    phoneNumbers *Sdt = (phoneNumbers *)malloc(sizeof(phoneNumbers));
    strcpy(Sdt->PhoneNumber, sdt);
    Sdt->next = NULL;
    return Sdt;
}

// Tao 1 email moi

emails *makeEmail(char *email)
{
    emails *e = (emails *)malloc(sizeof(emails));
    strcpy(e->Email, email);
    e->next = NULL;
    return e;
}

// Them 1 nguoi vao cuoi danh ba

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

char *lower(char *s)
{
    char *name = (char *)malloc(strlen(s) * sizeof(char));
    for (int i = 0; i < strlen(s); i++)
    {
        name[i] = tolower(s[i]);
    }
    name[strlen(s)] = 0;
    return name;
}

// Tim kiem bang ten trong danh ba

person *findName(person *r, char *name)
{
    if (r == NULL)
    {
        return NULL;
    }
    if (strstr(lower(r->name), name) != NULL)
    {
        return r;
    }
    return findName(r->next, name);
}

// Tim kiem bang sdt trong danh ba

person *findSdt(person *r, char *sdt)
{
    if (r == NULL)
    {
        return NULL;
    }
    phoneNumbers *n = r->sdt;
    while (n != NULL)
    {
        if (!strcmp(n->PhoneNumber, sdt))
        {
            return r;
        }
        n = n->next;
    }
    return findSdt(r->next, sdt);
}

// Doc danh ba tu file

int readFromFile(char *fileName)
{
    FILE *f = fopen(fileName, "r+");
    if (f == NULL)
    {
        return 0;
    }
    char s[100] = "";
    fscanf(f, "%d\n", &n);
    for (int i = 1; i <= n; i++)
    {
        // Doc ten va img
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
        // Doc sdt va email
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
            if ((s[0] != 'T' && s[0] != 'E') || (s[1] < 48 || s[1] > 57)) // Khong con sdt va email
                break;
            if (s[0] == 'T') // Doc sdt
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
            if (s[0] == 'E') // Doc email
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
    return 1;
}

// Ghi vao file

int writeToFile(char *fileName)
{
    FILE *f = fopen(fileName, "w+");
    if (f == NULL)
    {
        return 0;
    }
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
    return 1;
}

// Xoa 1 nguoi

void removePerson(char *name)
{
    person *p1 = findName(root, name);
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

// Xoa 1 sdt

int removeSdt(char *sdt)
{
    person *p = findSdt(root, sdt);
    phoneNumbers *s = p->sdt;
    if (s->next == NULL && p->email == NULL) // Neu chi co 1 sdt thi xoa luon nguoi do
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
    printf("Nh???p t??n file c???n ?????c: ");
    scanf("%s", fileName);
    int d = readFromFile(fileName);
    if (d == 1)
    {
        printf("???? ?????c d??? li???u t??? file %s\n", fileName);
    }
    else
    {
        printf("?????c file kh??ng th??nh c??ng\n");
    }
}

void ProcessWriteToFile()
{
    char fileName[100];
    printf("Nh???p t??n file c???n ghi: ");
    scanf("%s", fileName);
    int d = writeToFile(fileName);
    if (d == 1)
    {
        printf("???? ghi d??? li???u v??o file %s\n", fileName);
    }
    else
    {
        printf("Ghi file kh??ng th??nh c??ng\n");
    }
}

// In th??ng tin c???a 1 ng?????i

void printInfo(person *p)
{
    printf("Th??ng tin c???a %s l??:\n", p->name);
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

// Tim bang ten

void ProcessFindName()
{
    char name[100];
    printf("Nh???p t??n ng?????i c???n t??m: ");
    __fpurge(stdin);
    fgets(name, 100, stdin);
    name[strlen(name) - 1] = '\0';
    person *p = findName(root, name);
    if (p == NULL)
    {
        printf("Kh??ng t??m th???y ng?????i t??n %s\n", name);
        ProcessFindName();
    }
    else
    {
        printInfo(p);
        p = findName(p->next, name);
        while (p != NULL)
        {
            printInfo(p);
            p = findName(p->next, name);
        }
    }
}

// Tim bang sdt

void ProcessFindSdt()
{
    char number[100];
    printf("Nh???p s??? ??i???n tho???i c???n t??m: ");
    __fpurge(stdin);
    scanf("%s", number);
    person *p = findSdt(root, number);
    if (p == NULL)
    {
        printf("Kh??ng t??m th???y ng?????i c?? s??? ??i???n tho???i %s\n", number);
        ProcessFindSdt();
    }
    else
    {
        printInfo(p);
        p = findSdt(p->next, number);
        while (p != NULL)
        {
            printInfo(p);
            p = findSdt(p->next, number);
        }
    }
}

// Xoa 1 nguoi

void ProcessRemovePerson()
{
    char name[100];
    printf("Nh???p t??n ng?????i c???n x??a: ");
    __fpurge(stdin);
    fgets(name, 100, stdin);
    name[strlen(name) - 1] = '\0';
    if (findName(root, name) == NULL)
    {
        printf("Kh??ng c?? %s trong danh b???\n", name);
        ProcessRemovePerson();
    }
    else
    {
        removePerson(name);
        printf("???? x??a %s ra kh???i danh b???\n", name);
    }
}

// Xoa 1 sdt

void ProcessRemoveSdt()
{
    char number[100];
    printf("Nh???p s??? ??i???n tho???i c???n x??a: ");
    __fpurge(stdin);
    scanf("%s", number);
    if (findSdt(root, number) == NULL)
    {
        printf("Kh??ng c?? %s trong danh b???\n", number);
        ProcessRemovePerson();
    }
    else
    {
        int i = removeSdt(number);
        if (i == 0)
        {
            printf("???? x??a %s ra kh???i danh b???\n", findSdt(root, number)->name);
        }
        else
        {
            printf("???? x??a %s ra kh???i danh b???\n", number);
        }
    }
}

// Cap nhat thong tin cua 1 nguoi

void ProcessUpdateInfo()
{
    char name[100];
    printf("Nh???p t??n ng?????i c???n c???p nh???t: ");
    __fpurge(stdin);
    fgets(name, 100, stdin);
    name[strlen(name) - 1] = '\0';
    person *p = findName(root, name);
    if (p == NULL)
    {
        printf("Kh??ng c?? %s trong danh b???\n", name);
        ProcessRemovePerson();
    }
    else
    {
        char newImg[100];
        printf("Nh???p ???????ng d???n ???nh m???i: ");
        scanf("%s", newImg);
        fflush(stdin);
        strcpy(p->img, newImg);
        phoneNumbers *sdt = p->sdt;
        int j = 1;
        while (sdt != NULL)
        {
            char newNumber[100];
            printf("Nh???p s??? ??i???n tho???i th??? %d m???i: ", j);
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
            printf("Nh???p email th??? %d m???i: ", j);
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
        int cmd;
        printf("1.?????c th??ng tin danh b??? t??? file\n");
        printf("2.Ghi th??ng tin danh b??? v??o file\n");
        printf("3.T??m ki???m theo t??n\n");
        printf("4.Tra c???u s??? ??i???n tho???i\n");
        printf("5.X??a m???t ng?????i trong danh b???\n");
        printf("6.X??a s??? ??i???n tho???i\n");
        printf("7.C???p nh???t th??ng tin\n");
        printf("8.Tho??t\n");
        printf("Nh???p ch???c n??ng mu???n th???c hi???n (1-8): ");
        __fpurge(stdin);
        scanf("%d", &cmd);
        switch (cmd)
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
            printf("L???a ch???n kh??ng h???p l???\nTho??t\n");
            cmd = 8;
            break;
        }
        if (cmd == 8)
            break;
    }
    return 0;
}