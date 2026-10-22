#ifndef C_TUTOR_H // 防止头文件被重复包含，第一次包含时才继续展开
#define C_TUTOR_H // 定义包含守卫宏，后续再次包含会直接跳过

#include <stdio.h> // 提供 printf、fgets、fopen 等输入输出函数
#include <stdlib.h> // 提供 malloc、free、strtol、strtod、exit 等函数
#include <string.h> // 提供 strlen、strcmp、strcpy、memset 等字符串函数
#include <stdbool.h> // 引入 bool、true、false，让布尔值含义更清楚
#include <stddef.h> // 提供 size_t、NULL 等标准定义
#include <ctype.h> // 提供 isspace、toupper、tolower 等字符处理函数
#include <errno.h> // 提供 errno，用来判断转换或文件操作是否出错
#include <limits.h> // 提供 INT_MAX、INT_MIN 等整数范围常量
#include <float.h> // 提供 DBL_EPSILON 等浮点数精度常量
#include <math.h> // 提供 NAN、isnan、sqrt、fabs 等数学函数
#include <time.h> // 提供 time、clock 等时间函数
#ifdef _WIN32 // 只有 Windows 平台才包含 Windows API 头文件
#include <windows.h> // 提供 SetConsoleOutputCP 等控制台相关函数
#endif // 结束 Windows 专属头文件的平台判断

#define TUTOR_NAME "C 语言阶梯教程" // 程序名称，统一修改标题时只需改这一处
#define TUTOR_VERSION "1.0.0" // 程序版本号，方便识别教材迭代
#define INPUT_BUFFER_SIZE 256 // 输入缓冲区大小，足以容纳普通姓名、文本和命令
#define PATH_BUFFER_SIZE 260 // 文件路径缓冲区大小，兼容常见 Windows 路径长度
#define MAX_STUDENTS 100 // 综合项目中学生数组的容量上限

typedef struct Chapter { // 描述课程目录中的一章，便于主菜单统一展示
    int number; // 章节编号，从 1 开始，与进度文件和界面显示一致
    const char *stage; // 所属学习阶段，例如“基础篇”“进阶篇”
    const char *title; // 章节标题，用于主菜单和目录
    void (*lesson)(void); // 进入该章教学与案例的函数指针
} Chapter; // Chapter 结构体类型定义结束

void ui_banner(void); // 打印程序横幅与版本信息
void ui_rule(char symbol, int width); // 打印指定字符和宽度的分隔线
void ui_heading(const char *title); // 打印带有上下分隔线的一级标题
void ui_section(const char *title); // 打印小节标题，便于划分讲解、案例和作业
void ui_example(const char *title); // 打印“实操案例”标题
void ui_note(const char *text); // 打印一条提示信息，自动添加项目符号
void ui_pause(void); // 暂停并等待用户按回车，阅读大段内容时使用
int ui_read_int(const char *prompt, int min_value, int max_value); // 读取并校验闭区间内的整数，输入结束返回 min_value - 1
double ui_read_double(const char *prompt, double min_value, double max_value); // 读取并校验浮点数，输入结束返回 NAN
bool ui_read_text(const char *prompt, char *buffer, size_t size); // 读取一行非空文本，成功返回 true
bool ui_confirm(const char *question); // 读取 y/n 形式的确认，输入 y 或 Y 返回 true
bool ui_quiz(const char *question, const char *option_a, const char *option_b, const char *option_c, const char *option_d, char answer, const char *explanation); // 出一道四选一题并返回是否答对
void ui_homework(const char *items[], int count); // 在屏幕上显示课后作业列表
int ui_choose(const char *title, const char *options[], int count); // 显示编号菜单，返回选项下标，取消时返回 -1
void ui_complete(int chapter_number, int correct, int total); // 打印一章的完成信息与小测成绩

void lesson_01_intro(void); // 第 1 章：程序结构、编译流程与输入输出
void lesson_02_types(void); // 第 2 章：数据类型、变量、常量与运算符
void lesson_03_control(void); // 第 3 章：分支、循环与流程控制
void lesson_04_functions(void); // 第 4 章：函数、作用域与递归
void lesson_05_arrays(void); // 第 5 章：数组、排序与查找
void lesson_06_strings(void); // 第 6 章：字符串与字符处理
void lesson_07_pointers(void); // 第 7 章：指针、地址与内存模型
void lesson_08_memory(void); // 第 8 章：动态内存与链表
void lesson_09_structs(void); // 第 9 章：结构体、联合、枚举与 typedef
void lesson_10_files(void); // 第 10 章：文件、错误处理与命令行参数
void lesson_11_engineering(void); // 第 11 章：预处理器、位运算与工程化
void lesson_12_project(void); // 第 12 章：学生成绩管理综合项目

#endif // 结束头文件包含守卫，确保声明只参与一次编译
