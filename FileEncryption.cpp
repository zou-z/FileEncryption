#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h> 
#include <io.h>

#define FilePathLength 1024
#define KeyLength 20
char en_mark[13] = "(encryption)";
char de_mark[13] = "(decryption)";
int r0 = 0;

// 字符数组长度
int len(char arr[]) {
    for (int i = 0;i<999999; i++)
        if (arr[i] == '\0')
            return i;
}
// 文件长度（字节）
long file_length(FILE* fp) {
    fseek(fp, 0, SEEK_END);
    long length = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    return length;
}
// 输出文件路径（mode 加密:0 解密:1）
void output_path(char file_path[],char path[],int mode) {
    int p = len(file_path);
    for (int i = p - 1; i >= 0; i--) {
        if (file_path[i] == '.') {
            int j = 0,k=0;
            for (; j < i; j++)
                path[j] = file_path[j];
            if (mode == 0) {
                for (; k < len(en_mark); k++)
                    path[j + k] = en_mark[k];
                path[p + k] = '\0';
            }
            else if (mode == 1) {
                for (; k < len(de_mark); k++)
                    path[j + k] = de_mark[k];
                path[p + k] = '\0';
            }
            for (j += k; i < p; i++, j++)
                path[j] = file_path[i];
            break;
        }
        else if (file_path[i] == '\\') {
            int k = 0;
            for (int j = 0; j < p; j++)
                path[j] = file_path[j];
            if (mode == 0) {
                for (; k < len(en_mark); k++)
                    path[p + k] = en_mark[k];
                path[p + k] = '\0';
            }
            else if (mode == 1) {
                for (; k < len(de_mark); k++)
                    path[p + k] = de_mark[k];
                path[p + k] = '\0';
            }
            break;
        }
    }
}
// 输出进度
void ratio(long m,long n) {
    int r = (m * 100) / n;
    if (r > r0) {
        r0 = r;
        printf("%d%% (%ld/%ld)\n", r0, m, n);
    }
}
// 开始加解密
int start(char file_path[],char key[],int mode) {
    char path[FilePathLength];
    output_path(file_path, path, mode);
    FILE* fp0 = fopen(file_path, "rb");
    FILE* fp1 = fopen(path, "wb");
    if (fp0 == NULL || fp1 == NULL) {
        fclose(fp0);
        fclose(fp1);
        return 1;
    }
    long file_len = file_length(fp0);
    int key_len = len(key);

    char buffer;
    int i = 0;
    long m = 0;
    buffer = fgetc(fp0);
    if (mode == 0) {
        while (!feof(fp0))
        {
            buffer += key[i++];
            fputc(buffer, fp1);
            i = i < key_len ? i : 0;
            ratio(m++, file_len);
            buffer = fgetc(fp0);
        }
    }
    else if (mode == 1) {
        while (!feof(fp0))
        {
            buffer -= key[i++];
            fputc(buffer, fp1);
            i = i < key_len ? i : 0;
            ratio(m++, file_len);
            buffer = fgetc(fp0);
        }
    }
    fclose(fp0);
    fclose(fp1);
    return 0;
}

int main()
{
    char file_path[FilePathLength];
    char key[KeyLength];
    int mode = 0;
    printf("请输入文件位置(请使用\\):");
    scanf("%s", file_path);
    while (-1 == _access(file_path, 0)) {
        printf("文件不存在请重新输入:");
        scanf("%s", file_path);
    }
    printf("请输入密钥:");
    scanf("%s", key);
    printf("请输入模式(0:加密 1：解密):");
    scanf("%d", &mode);
    switch (start(file_path, key, mode)) {
        case 0:printf("100%% [完成]\n"); break;
        case 1:printf("失败!\n"); break;
    }
    system("pause");
    return 0;
}
