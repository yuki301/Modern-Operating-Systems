#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>

#define CMD_MAX_LEN 1024 //����������ַ���Ϊ1023��ĩβΪNULL
#define CMD_MAX_ARGS 64 //�����������Ϊ63��ĩβΪNULL

void type_prompt() {
    printf("myshell> ");
}

void read_command(char* command, char* parameters[]) {
    char input[CMD_MAX_LEN];
    fgets(input, CMD_MAX_LEN, stdin);
    input[strcspn(input, "\n")] = '\0'; //�����з��滻Ϊ'\0'

    //���ݿո�ָ��ȡ����
    char* token = strtok(input, " ");
    int i = 0;
    while (token != NULL) {
        parameters[i++] = token;
        token = strtok(NULL, " ");
    }
    parameters[i] = NULL; 
}

int main() {
    char* parameters[CMD_MAX_ARGS];
    PROCESS_INFORMATION processInfo; //�洢���ڴ����½��̵���Ϣ�Ľṹ
    ZeroMemory(&processInfo, sizeof(processInfo)); //��ʼ���ṹ���ڴ�Ϊ0
    STARTUPINFOA startupInfo; //����ָ���½��̵������ڵ���ۺ���������½���
    ZeroMemory(&startupInfo, sizeof(startupInfo));
    startupInfo.cb = sizeof(startupInfo); 
    //���ṹ���С�����ṹ���Ա���������̺���������ȷ��ȡ�ṹ��

    while (1) {
        type_prompt();
        read_command(NULL, parameters);

        //�˳�ָ��
        if (strcmp(parameters[0], "exit") == 0) 
        {
            break;
        }

        if (!CreateProcessA(
            NULL, //ʹ�õڶ��������������У���������
            parameters[0],
            NULL, //���̳и����̵ľ��
            NULL, //���̳и����̵��߳̾��
            FALSE, //�½��̲���̳и����̵ľ��
            0, //��ʹ�����ⴴ����ʽ
            NULL, //�½���ʹ�ø����̵Ļ�������
            NULL, //�½���ʹ�ø����̵ĵ�ǰĿ¼
            &startupInfo,
            &processInfo
        )) {
            printf("CreateProcess failed (%d).\n", GetLastError());//��ȡ����ʾ������Ϣ
            continue;
        }

        //�ȴ����̽���
        WaitForSingleObject(processInfo.hProcess, INFINITE);

        //�رս��̺��߳�
        CloseHandle(processInfo.hProcess);
        CloseHandle(processInfo.hThread);
    }

    return 0;
}

