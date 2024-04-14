#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>

#define CMD_MAX_LEN 1024 //命令行最大字符数为1023，末尾为NULL
#define CMD_MAX_ARGS 64 //参数数量最大为63，末尾为NULL

void type_prompt() {
    printf("myshell> ");
}

void read_command(char* command, char* parameters[]) {
    char input[CMD_MAX_LEN];
    fgets(input, CMD_MAX_LEN, stdin);
    input[strcspn(input, "\n")] = '\0'; //将换行符替换为'\0'

    //根据空格分割获取参数
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
    PROCESS_INFORMATION processInfo; //存储用于创建新进程的信息的结构
    ZeroMemory(&processInfo, sizeof(processInfo)); //初始化结构体内存为0
    STARTUPINFOA startupInfo; //用于指定新进程的主窗口的外观和如何启动新进程
    ZeroMemory(&startupInfo, sizeof(startupInfo));
    startupInfo.cb = sizeof(startupInfo); 
    //将结构体大小赋给结构体成员，创建进程函数可以正确读取结构体

    while (1) {
        type_prompt();
        read_command(NULL, parameters);

        //退出指令
        if (strcmp(parameters[0], "exit") == 0) 
        {
            break;
        }

        if (!CreateProcessA(
            NULL, //使用第二个参数（命令行）启动进程
            parameters[0],
            NULL, //不继承父进程的句柄
            NULL, //不继承父进程的线程句柄
            FALSE, //新进程不会继承父进程的句柄
            0, //不使用特殊创建方式
            NULL, //新进程使用父进程的环境变量
            NULL, //新进程使用父进程的当前目录
            &startupInfo,
            &processInfo
        )) {
            printf("CreateProcess failed (%d).\n", GetLastError());//获取并显示错误信息
            continue;
        }

        //等待进程结束
        WaitForSingleObject(processInfo.hProcess, INFINITE);

        //关闭进程和线程
        CloseHandle(processInfo.hProcess);
        CloseHandle(processInfo.hThread);
    }

    return 0;
}

