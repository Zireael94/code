#include "tutor.h" // 引入教程公共声明，保证函数定义与声明一致

void ui_rule(char symbol, int width) { // 输出一条指定字符、指定宽度的分隔线
    int i = 0; // 循环计数器，从 0 开始画线
    for (i = 0; i < width; i++) { // 重复输出 width 次同一个字符
        putchar(symbol); // 输出一个分隔字符
    } // 分隔字符输出结束
    putchar('\n'); // 换行，让后续内容从新的一行开始
} // ui_rule 函数结束

void ui_banner(void) { // 显示程序整体横幅，建立统一视觉风格
    ui_rule('=', 68); // 先用等号画一条较宽的分隔线
    printf("  %s  v%s\n", TUTOR_NAME, TUTOR_VERSION); // 输出名称与版本号
    printf("  从第一行代码，到能维护的 C 项目\n"); // 输出课程定位
    ui_rule('=', 68); // 横幅底部再画一条等号分隔线
} // ui_banner 函数结束

void ui_heading(const char *title) { // 打印一章的主标题
    putchar('\n'); // 标题前空一行，避免与上一屏内容挤在一起
    ui_rule('=', 68); // 画标题上边框
    printf("  %s\n", title); // 输出标题文本
    ui_rule('=', 68); // 画标题下边框
} // ui_heading 函数结束

void ui_section(const char *title) { // 打印章节内的小节标题
    putchar('\n'); // 小节前留出空白，增强层次感
    printf("【%s】\n", title); // 使用中文方括号突出小节名称
    ui_rule('-', 68); // 用短横线分隔小节内容
} // ui_section 函数结束

void ui_example(const char *title) { // 打印一个实操案例的标题
    printf("\n>>> 实操案例：%s\n", title); // 用箭头提示现在进入可运行案例
    puts("    （案例会即时校验输入；按提示操作，无法继续时结束输入即可退出）"); // 提前说明交互方式
} // ui_example 函数结束

void ui_note(const char *text) { // 输出一条带项目符号的提示
    printf("  · %s\n", text); // 用圆点开头，便于快速扫读
} // ui_note 函数结束

void ui_pause(void) { // 暂停程序，等待用户阅读后按回车
    char line[INPUT_BUFFER_SIZE] = {0}; // 临时保存用户按下回车时的整行输入
    printf("\n  按 Enter 继续..."); // 提示用户如何继续
    fflush(stdout); // 立即刷新输出，确保提示在等待前出现
    if (fgets(line, sizeof(line), stdin) == NULL) { // 如果输入流已经结束
        putchar('\n'); // 补一个换行，避免后续输出紧贴提示
    } // 暂停输入处理结束
} // ui_pause 函数结束

int ui_read_int(const char *prompt, int min_value, int max_value) { // 读取一个位于闭区间内的整数
    char line[INPUT_BUFFER_SIZE] = {0}; // 用整行读取避免 scanf 留下的换行符干扰后续输入
    char *end = NULL; // 指向 strtol 停止解析的位置
    long value = 0; // 保存转换后的长整数，先扩大范围再检查
    while (true) { // 一直循环，直到得到合法输入或输入流结束
        printf("%s", prompt); // 显示调用者提供的提示语
        fflush(stdout); // 刷新输出，让提示立即显示
        if (fgets(line, sizeof(line), stdin) == NULL) { // 读取一行失败说明遇到了 EOF
            putchar('\n'); // 输出换行，让界面保持整齐
            return min_value - 1; // 用区间下界减一表示取消或输入结束
        } // 输入结束处理结束
        errno = 0; // 清零错误码，准备检测转换溢出
        end = NULL; // 重置结束指针，避免误用上一次结果
        value = strtol(line, &end, 10); // 按十进制把整行转换成 long
        if (end == line) { // 如果没有任何数字被解析
            puts("  输入无效：请输入十进制整数。"); // 提示用户重新输入
            continue; // 回到循环开头再次读取
        } // 无数字检查结束
        while (end != NULL && *end != '\0' && isspace((unsigned char)*end)) { // 跳过数字后面的空格和换行
            end++; // 移动到下一个字符
        } // 尾部空白清理结束
        if (end != NULL && *end == '\0' && errno != ERANGE && value >= min_value && value <= max_value) { // 检查整行是否都是数字且范围正确
            return (int)value; // 转换并返回合法整数
        } // 合法输入检查结束
        printf("  输入无效：请输入 %d 到 %d 之间的整数。\n", min_value, max_value); // 说明正确范围
    } // 输入循环结束
} // ui_read_int 函数结束

double ui_read_double(const char *prompt, double min_value, double max_value) { // 读取一个指定范围内的浮点数
    char line[INPUT_BUFFER_SIZE] = {0}; // 保存用户输入的整行文本
    char *end = NULL; // 指向 strtod 停止解析的位置
    double value = 0.0; // 保存转换后的双精度浮点数
    while (true) { // 重复读取，直到输入合法
        printf("%s", prompt); // 显示提示语
        fflush(stdout); // 刷新输出缓冲区
        if (fgets(line, sizeof(line), stdin) == NULL) { // 输入流结束时取消操作
            putchar('\n'); // 补一个换行
            return NAN; // 用 NaN 表示“没有有效结果”
        } // 输入结束处理结束
        errno = 0; // 清零错误码
        end = NULL; // 重置解析结束位置
        value = strtod(line, &end); // 按十进制解析浮点数
        if (end == line) { // 如果连一个数字都没有读到
            puts("  输入无效：请输入数字，例如 12.5。"); // 给出格式示例
            continue; // 重新读取
        } // 无数字检查结束
        while (end != NULL && *end != '\0' && isspace((unsigned char)*end)) { // 允许数字后存在空白
            end++; // 跳过尾部空白
        } // 尾部空白处理结束
        if (end != NULL && *end == '\0' && errno != ERANGE && isfinite(value) && value >= min_value && value <= max_value) { // 检查整行格式与数值范围
            return value; // 返回合法的浮点数
        } // 合法输入判断结束
        printf("  输入无效：请输入 %g 到 %g 之间的数字。\n", min_value, max_value); // 提示允许的范围
    } // 输入循环结束
} // ui_read_double 函数结束

bool ui_read_text(const char *prompt, char *buffer, size_t size) { // 读取非空的一行文本
    if (buffer == NULL || size == 0) { // 防御性检查，防止调用者传入空指针或零长度
        return false; // 参数不合法时直接报告失败
    } // 参数检查结束
    while (true) { // 空字符串不允许，因此循环到有内容或 EOF
        if (prompt != NULL) { // 如果调用者提供了提示语
            printf("%s", prompt); // 原样输出提示
            fflush(stdout); // 立即刷新，确保用户看到提示
        } // 提示语处理结束
        if (fgets(buffer, (int)size, stdin) == NULL) { // 读取失败表示输入流结束
            buffer[0] = '\0'; // 把缓冲区置为空字符串
            putchar('\n'); // 输出换行
            return false; // 返回失败，让调用者决定是否退出案例
        } // 读取处理结束
        if (strchr(buffer, '\n') == NULL) { // 如果没有读到换行，说明一行太长
            int ch = 0; // 临时变量保存多余的字符
            while ((ch = getchar()) != '\n' && ch != EOF) { // 丢弃本行剩余字符
                // 这里不处理字符，只为了清空输入缓冲
            } // 多余字符清理结束
        } // 超长行处理结束
        buffer[strcspn(buffer, "\r\n")] = '\0'; // 去掉行尾可能存在的回车和换行
        if (buffer[0] != '\0') { // 只有非空文本才算读取成功
            return true; // 返回成功
        } // 非空检查结束
        puts("  输入不能为空，请重新输入。"); // 提醒用户必须输入内容
    } // 读取循环结束
} // ui_read_text 函数结束

bool ui_confirm(const char *question) { // 询问一个 y/n 问题
    char line[INPUT_BUFFER_SIZE] = {0}; // 保存用户输入的整行
    while (true) { // 直到用户明确回答 y 或 n
        printf("%s [y/N] ", question); // 显示问题，并提示默认是 No
        fflush(stdout); // 刷新输出
        if (fgets(line, sizeof(line), stdin) == NULL) { // 遇到输入结束
            putchar('\n'); // 补换行
            return false; // 按默认值 No 处理
        } // 输入结束处理结束
        if (line[0] == 'y' || line[0] == 'Y') { // 接受小写或大写的 y
            return true; // 用户确认
        } // y 判断结束
        if (line[0] == 'n' || line[0] == 'N' || line[0] == '\n' || line[0] == '\r') { // 接受 n 或直接回车
            return false; // 用户取消
        } // n 判断结束
        puts("  请输入 y 或 n。"); // 对无效回答给出提示
    } // 确认循环结束
} // ui_confirm 函数结束

bool ui_quiz(const char *question, const char *option_a, const char *option_b, const char *option_c, const char *option_d, char answer, const char *explanation) { // 出一道四选一并判断答案
    char line[INPUT_BUFFER_SIZE] = {0}; // 保存用户输入的选项
    char choice = '\0'; // 保存整理后的单个大写字母
    char *p = NULL; // 用于跳过选项前的空格
    printf("\n  小测：%s\n", question); // 输出题干
    printf("    A. %s\n", option_a); // 输出 A 选项
    printf("    B. %s\n", option_b); // 输出 B 选项
    printf("    C. %s\n", option_c); // 输出 C 选项
    printf("    D. %s\n", option_d); // 输出 D 选项
    printf("  你的答案（A/B/C/D）："); // 提示输入答案
    fflush(stdout); // 刷新输出
    if (fgets(line, sizeof(line), stdin) == NULL) { // 输入结束时直接公布答案
        printf("\n  输入结束。正确答案是 %c。\n", (char)toupper((unsigned char)answer)); // 显示正确选项
        printf("  解析：%s\n", explanation); // 输出解析
        return false; // 未作答按错误处理
    } // 输入结束处理结束
    p = line; // 从整行开头开始查找
    while (*p != '\0' && isspace((unsigned char)*p)) { // 跳过前导空格
        p++; // 移到下一个字符
    } // 前导空格处理结束
    choice = (char)toupper((unsigned char)*p); // 把第一个有效字符转成大写
    if (choice == (char)toupper((unsigned char)answer)) { // 比较用户选择与标准答案
        puts("  ✓ 回答正确。"); // 答对时给出正向反馈
        printf("  解析：%s\n", explanation); // 输出解析，强化理解
        return true; // 返回答对
    } // 正确分支结束
    printf("  ✗ 回答错误。正确答案是 %c。\n", (char)toupper((unsigned char)answer)); // 答错时公布答案
    printf("  解析：%s\n", explanation); // 输出解析，说明原因
    return false; // 返回答错
} // ui_quiz 函数结束

void ui_homework(const char *items[], int count) { // 展示课后作业清单
    int i = 0; // 作业编号计数器
    ui_section("课后作业（请先独立完成，再回看答案）"); // 输出作业小节标题
    for (i = 0; i < count; i++) { // 遍历每一条作业
        printf("  %d. %s\n", i + 1, items[i]); // 输出编号与作业内容
    } // 作业遍历结束
    puts("  提示：每章源码文件末尾也用行注释给出了同样的任务，方便边改代码边练习。"); // 说明作业与源码注释的对应关系
} // ui_homework 函数结束

int ui_choose(const char *title, const char *options[], int count) { // 显示一个带编号的选择菜单
    int i = 0; // 选项遍历下标
    int choice = 0; // 保存用户选择的编号
    if (title != NULL) { // 标题可以为空
        printf("\n%s\n", title); // 输出菜单标题
    } // 标题处理结束
    for (i = 0; i < count; i++) { // 逐个显示选项
        printf("  [%d] %s\n", i + 1, options[i]); // 输出编号和选项文字
    } // 选项遍历结束
    printf("  [0] 返回\n"); // 所有菜单都提供统一的返回入口
    choice = ui_read_int("请选择：", 0, count); // 读取 0 到 count 的编号
    if (choice <= 0) { // 0 表示取消，负数表示输入结束
        return -1; // 返回 -1 让调用者退出菜单
    } // 取消判断结束
    return choice - 1; // 转换为从 0 开始的数组下标
} // ui_choose 函数结束

void ui_complete(int chapter_number, int correct, int total) { // 输出一章的完成总结
    putchar('\n'); // 与上一个案例分隔
    ui_rule('=', 68); // 画完成信息上边框
    printf("  第 %d 章学习结束\n", chapter_number); // 输出章节号
    if (total > 0) { // 只有出过题才显示正确率
        printf("  小测成绩：%d / %d", correct, total); // 输出答对题数与总题数
        if (correct == total) { // 如果全部答对
            printf("  （全对，基础很扎实！）\n"); // 给出鼓励
        } else { // 如果没有全对
            printf("  （建议回看错题对应的知识点）\n"); // 给出复习建议
        } // 成绩评价结束
    } // 小测显示结束
    puts("  下一步：完成上方作业，再回到主菜单选择下一章。"); // 给出学习节奏建议
    ui_rule('=', 68); // 画完成信息下边框
} // ui_complete 函数结束
