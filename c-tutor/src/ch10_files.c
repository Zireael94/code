#include "tutor.h" // 引入教程公共接口和标准库

typedef struct ScoreRecord { // 定义二进制文件演示使用的定长记录
    int id; // 学号
    double score; // 成绩
    char name[16]; // 姓名
} ScoreRecord; // ScoreRecord 是结构体类型别名

static void demo_text_file(void) { // 案例一：写文本文件、追加日志并按行读取
    const char *path = "demo_scores.txt"; // 文本数据文件名，保存在当前工作目录
    const char *log_path = "study.log"; // 学习日志文件名
    FILE *file = NULL; // 文件指针
    char line[256] = {0}; // 保存读取到的一行
    int line_number = 0; // 行号计数器
    ui_example("文本文件：写入、追加与按行读取"); // 显示案例标题
    file = fopen(path, "w"); // 以文本写模式创建或覆盖文件
    if (file == NULL) { // 打开失败必须处理
        printf("  无法创建 %s：%s\n", path, strerror(errno)); // 输出错误原因
        return; // 结束案例
    } // 打开检查结束
    if (fprintf(file, "学号,姓名,成绩\n") < 0) { // 写入表头并检查写入结果
        perror("  写表头失败"); // 输出系统错误
        fclose(file); // 关闭文件
        return; // 结束案例
    } // 表头写入结束
    fprintf(file, "1001,林小满,88.5\n"); // 写入第一条记录
    fprintf(file, "1002,陈一鸣,94.0\n"); // 写入第二条记录
    fprintf(file, "1003,周可欣,76.5\n"); // 写入第三条记录
    if (fclose(file) != 0) { // 关闭文件，刷新缓冲区并检查错误
        perror("  关闭文本文件失败"); // 输出关闭错误
        return; // 结束案例
    } // 关闭检查结束
    puts("\n  文本文件已写入，下面重新打开并逐行读取："); // 提示读取阶段
    file = fopen(path, "r"); // 以只读文本模式打开
    if (file == NULL) { // 打开失败
        printf("  无法读取 %s：%s\n", path, strerror(errno)); // 输出错误
        return; // 结束案例
    } // 打开检查结束
    while (fgets(line, sizeof(line), file) != NULL) { // 每次读取一行，直到 EOF
        line_number++; // 行号加一
        line[strcspn(line, "\r\n")] = '\0'; // 去掉行尾回车换行，便于显示
        printf("  [%02d] %s\n", line_number, line); // 输出行号和内容
    } // 读取循环结束
    if (ferror(file)) { // 循环结束后检查是否发生过读取错误
        perror("  读取文件过程中出错"); // 输出错误
    } // 错误检查结束
    fclose(file); // 关闭读取文件
    printf("  共读取 %d 行文本。\n", line_number); // 输出总行数
    file = fopen(log_path, "a"); // 以追加模式打开日志，不会覆盖旧内容
    if (file == NULL) { // 打开失败
        printf("  无法追加日志：%s\n", strerror(errno)); // 输出错误
        return; // 结束案例
    } // 日志打开检查结束
    fprintf(file, "完成第 10 章文本文件案例，时间戳：%ld\n", (long)time(NULL)); // 追加一条带时间的日志
    fclose(file); // 关闭日志文件
    puts("  已向 study.log 追加一条学习记录。"); // 告知追加结果
    puts("  模式速记：\"r\" 读、\"w\" 覆盖写、\"a\" 追加、\"rb\" 二进制读、\"wb\" 二进制写。"); // 总结打开模式
} // demo_text_file 函数结束

static void demo_binary_file(void) { // 案例二：二进制读写与文件定位
    const char *path = "demo_scores.dat"; // 二进制数据文件名
    ScoreRecord records[3] = { // 准备三条定长记录
        {1001, 88.5, "林小满"}, // 第一条记录
        {1002, 94.0, "陈一鸣"}, // 第二条记录
        {1003, 76.5, "周可欣"} // 第三条记录
    }; // 记录数组结束
    ScoreRecord buffer[3] = {{0, 0.0, ""}, {0, 0.0, ""}, {0, 0.0, ""}}; // 读回缓冲区
    FILE *file = NULL; // 文件指针
    size_t written = 0; // 实际写入记录数
    size_t read_count = 0; // 实际读取记录数
    long file_size = 0; // 文件字节数
    size_t i = 0; // 循环下标
    ui_example("二进制文件：fwrite、fread 与 fseek"); // 显示案例标题
    file = fopen(path, "wb"); // 以二进制写模式打开
    if (file == NULL) { // 打开失败
        printf("  无法创建二进制文件：%s\n", strerror(errno)); // 输出错误
        return; // 结束案例
    } // 打开检查结束
    written = fwrite(records, sizeof(ScoreRecord), 3, file); // 一次写入三个完整记录
    if (written != 3) { // 写入数量不符说明失败
        printf("  只写入了 %lu 条记录。\n", (unsigned long)written); // 输出实际写入数
        fclose(file); // 关闭文件
        return; // 结束案例
    } // 写入检查结束
    fclose(file); // 关闭写文件，确保数据落盘
    file = fopen(path, "rb"); // 以二进制读模式重新打开
    if (file == NULL) { // 打开失败
        printf("  无法读取二进制文件：%s\n", strerror(errno)); // 输出错误
        return; // 结束案例
    } // 读取打开检查结束
    if (fseek(file, 0, SEEK_END) != 0) { // 把文件位置移到末尾
        perror("  fseek 失败"); // 输出定位错误
        fclose(file); // 关闭文件
        return; // 结束案例
    } // 定位检查结束
    file_size = ftell(file); // 取得文件总字节数
    if (file_size < 0) { // ftell 失败时返回 -1
        perror("  ftell 失败"); // 输出错误
        fclose(file); // 关闭文件
        return; // 结束案例
    } // 大小检查结束
    rewind(file); // 把文件位置重新移回开头
    read_count = fread(buffer, sizeof(ScoreRecord), 3, file); // 按记录读取文件内容
    printf("\n  文件大小：%ld 字节，每条记录：%lu 字节，读回：%lu 条。\n", file_size, (unsigned long)sizeof(ScoreRecord), (unsigned long)read_count); // 输出尺寸与读取数量
    for (i = 0; i < read_count; i++) { // 遍历读回的记录
        printf("  %d  %-12s  %.1f\n", buffer[i].id, buffer[i].name, buffer[i].score); // 输出记录字段
    } // 遍历结束
    fclose(file); // 关闭读取文件
    puts("  注意：直接把结构体写入文件依赖填充和字节序，只适合本机临时数据。"); // 强调二进制可移植性
    puts("  真实项目应定义固定宽度、固定字节序的文件格式，并逐字段编码解码。"); // 给出工程建议
} // demo_binary_file 函数结束

static void demo_errors(void) { // 案例三：故意打开不存在的文件，观察错误处理
    const char *missing = "definitely_missing_file_c_tutor.txt"; // 故意使用不存在的文件名
    FILE *file = NULL; // 文件指针
    ui_example("错误处理：fopen 失败、errno 与 perror"); // 显示案例标题
    errno = 0; // 调用前清零，便于确认错误由本次操作产生
    file = fopen(missing, "r"); // 尝试只读打开一个不存在的文件
    if (file == NULL) { // 打开失败时进入错误分支
        printf("\n  fopen 返回 NULL。errno = %d\n", errno); // 输出错误编号
        printf("  strerror 解释：%s\n", strerror(errno)); // 把错误编号转换为文字
        perror("  perror 输出的信息"); // 让标准库自动补充错误原因
        puts("  正确做法：检查返回值、解释错误、决定重试/退出/降级，而不是继续使用空指针。"); // 总结错误处理步骤
        return; // 结束案例
    } // 失败分支结束
    puts("  意外：示例文件居然存在，请改用其他文件名重试。"); // 理论上不会执行
    fclose(file); // 如果文件存在，仍然要关闭
} // demo_errors 函数结束

void lesson_10_files(void) { // 第 10 章教学入口
    int correct = 0; // 记录小测答对题数
    const char *homework[] = { // 课后作业文本
        "实现学习日志：追加带时间的笔记，再读取全部日志并统计行数。", // 作业一
        "读写 CSV 成绩文件：解析每行，计算平均分；格式错误的行要报告行号。", // 作业二
        "实现二进制成绩文件：添加记录、按学号查找、修改成绩并保存。", // 作业三
        "写一个命令行文件复制工具：接收源文件和目标文件，带错误提示和退出码。", // 作业四
        "制造三类文件错误（不存在、无权限、模式错误），记录 errno 与修复方法。" // 作业五
    }; // 作业数组结束
    ui_heading("第 10 章  文件、错误处理与命令行参数"); // 显示章节标题
    ui_section("本章目标"); // 显示目标小节
    ui_note("理解流、文本模式、二进制模式和缓冲区的概念。"); // 列出目标一
    ui_note("掌握 fopen/fclose/fgets/fprintf/fread/fwrite/fseek 的用法。"); // 列出目标二
    ui_note("能用 errno、strerror、perror 和返回值处理错误。"); // 列出目标三
    ui_note("理解 argc/argv，并能写出可配置的命令行程序。"); // 列出目标四
    ui_section("知识讲解"); // 开始知识讲解
    puts("  1. 文件是数据流"); // 讲解第一点标题
    puts("     fopen 返回 FILE* 句柄；stdin、stdout、stderr 是程序启动时已打开的三个流。"); // 说明文件流
    puts("     文本模式处理换行转换，二进制模式按原始字节读写；Windows 下区别明显。"); // 说明模式差异
    puts("  2. 打开模式"); // 讲解第二点标题
    puts("     \"r\" 只读且文件必须存在；\"w\" 截断写；\"a\" 追加；加 b 表示二进制。"); // 列出模式
    puts("     \"r+\" 可读写且要求存在；\"w+\" 截断后可读写；具体组合以标准文档为准。"); // 补充更新模式
    puts("  3. 读取、写入与定位"); // 讲解第三点标题
    puts("     fgets 适合按行读文本；fread/fwrite 按块读写二进制；fprintf/fscanf 做格式化。"); // 介绍 I/O 函数
    puts("     fseek + ftell 可以定位和求大小；rewind 等价于回到文件开头。"); // 介绍定位函数
    puts("     文本文件换行、编码和平台有关；跨平台交换优先使用 UTF-8 和明确格式。"); // 强调编码
    puts("  4. 错误处理"); // 讲解第四点标题
    puts("     文件函数大多用返回值报告失败；失败后 errno 才具有诊断意义。"); // 说明 errno
    puts("     perror 会自动在消息后附加当前错误；处理完要尽早关闭已打开的文件。"); // 说明 perror
    puts("     程序退出码 0 表示正常，非 0 表示失败，方便脚本和自动化流程判断。"); // 说明退出码
    puts("  5. 命令行参数"); // 讲解第五点标题
    puts("     int main(int argc, char *argv[])：argc 是参数个数，argv[0] 是程序名。"); // 解释 main 参数
    puts("     参数都是字符串，要自己用 strtol 等函数转换并校验格式和范围。"); // 说明参数校验
    puts("     带空格的参数需要引号；敏感信息不要直接放在命令行历史里。"); // 给出使用建议
    ui_pause(); // 等待阅读
    ui_section("运行案例"); // 开始案例
    demo_text_file(); // 运行文本文件案例
    ui_pause(); // 暂停阅读
    demo_binary_file(); // 运行二进制文件案例
    ui_pause(); // 暂停阅读
    demo_errors(); // 运行错误处理案例
    ui_section("本章小测"); // 开始小测
    if (ui_quiz("fopen 打开文件失败时通常返回什么？", "EOF", "NULL", "0", "-1", 'B', "fopen 返回 FILE*，失败时返回空指针；不要用 EOF 判断打开结果。")) { // 第一题
        correct++; // 答对计数加一
    } // 第一题结束
    if (ui_quiz("在 Windows 上，哪种模式最适合读写原始二进制数据？", "\"r\"", "\"w\"", "\"rb\"", "\"a+\"", 'C', "\"rb\" 以二进制只读方式打开，避免文本模式的换行转换。")) { // 第二题
        correct++; // 答对计数加一
    } // 第二题结束
    if (ui_quiz("程序从 main 返回 1 通常表示什么？", "成功", "发生了错误", "文件已关闭", "内存已释放", 'B', "返回 0 表示成功；非 0 是失败或异常退出码，脚本可以据此判断。")) { // 第三题
        correct++; // 答对计数加一
    } // 第三题结束
    ui_homework(homework, (int)(sizeof(homework) / sizeof(homework[0]))); // 显示作业
    ui_complete(10, correct, 3); // 显示完成信息和小测成绩
} // lesson_10_files 函数结束

// ============================================================================
// 第 10 章课后作业（文件名、失败路径和清理动作都要写进设计）
// 1. 学习日志：追加“时间戳 + 笔记”，再读回全部日志，统计行数和字符数。
//    要求：写入失败、读取失败、文件为空三种情况都有明确提示。
// 2. CSV 成绩：读取学号,姓名,成绩格式的文件，计算人数、平均分、最高分。
//    要求：跳过空行，报告格式错误所在行号，不能因一行错误而崩溃。
// 3. 二进制成绩：用 fwrite 保存定长记录，支持添加、按学号查找、修改成绩、保存。
//    进阶：不把整个数组读入内存，使用 fseek 定位到指定记录进行修改。
// 4. 文件复制工具：main(int argc, char *argv[]) 接收源与目标路径，按二进制复制。
//    要求：参数不足、源文件不存在、目标无法创建、读写失败都返回非 0 退出码。
// 5. 错误诊所：分别测试文件不存在、路径是目录、无写权限、以 "r" 模式写文件。
//    记录返回值、errno、strerror 输出和你的修复方案。
//
// 提交标准：
// - 每个 fopen 成功路径都有对应的 fclose，错误路径也不泄漏文件句柄。
// - 不使用 gets、不忽略 fwrite/fread 的返回值。
// - 能解释文本模式和二进制模式的区别，以及为什么结构体直写不可跨平台。
// ============================================================================
