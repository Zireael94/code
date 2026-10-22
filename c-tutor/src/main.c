#include "tutor.h" // 引入公共声明和标准库头文件

#define CHAPTER_COUNT 12 // 当前教程共有 12 章，修改目录时同步调整这个值
#define MENU_ROADMAP 13 // 主菜单中的“学习路线”选项编号
#define MENU_RESET 14 // 主菜单中的“重置进度”选项编号
#define PROGRESS_FILE "progress.txt" // 学习进度保存在程序当前工作目录

static const Chapter g_chapters[] = { // 课程目录表，把章节元数据和教学函数集中管理
    {1, "基础篇", "程序结构、编译流程与输入输出", lesson_01_intro}, // 第 1 章：建立 C 程序整体认识
    {2, "基础篇", "数据类型、变量、常量与运算符", lesson_02_types}, // 第 2 章：理解数据如何表示和运算
    {3, "基础篇", "分支、循环与流程控制", lesson_03_control}, // 第 3 章：让程序做出判断和重复工作
    {4, "进阶篇", "函数、作用域与递归", lesson_04_functions}, // 第 4 章：用函数拆分复杂问题
    {5, "进阶篇", "数组、排序与查找", lesson_05_arrays}, // 第 5 章：批量处理同类型数据
    {6, "进阶篇", "字符串与字符处理", lesson_06_strings}, // 第 6 章：掌握 C 风格字符串
    {7, "进阶篇", "指针、地址与内存模型", lesson_07_pointers}, // 第 7 章：理解 C 语言的核心机制
    {8, "深入篇", "动态内存与链表", lesson_08_memory}, // 第 8 章：按需申请内存并管理生命周期
    {9, "深入篇", "结构体、联合、枚举与 typedef", lesson_09_structs}, // 第 9 章：组织复杂数据
    {10, "深入篇", "文件、错误处理与命令行参数", lesson_10_files}, // 第 10 章：让数据持久化和程序可配置
    {11, "深入篇", "预处理器、位运算与工程化", lesson_11_engineering}, // 第 11 章：接近真实项目的写法
    {12, "实战篇", "学生成绩管理系统", lesson_12_project} // 第 12 章：综合运用并完成可运行项目
}; // 课程目录表结束

static bool g_completed[CHAPTER_COUNT] = {false}; // 记录每章是否完成，数组下标等于章节号减一

static void prepare_console(void) { // 为 Windows 控制台准备 UTF-8 环境
#ifdef _WIN32 // 以下设置只在 Windows 上执行
    SetConsoleOutputCP(CP_UTF8); // 让 printf 输出的 UTF-8 中文能够正常显示
    SetConsoleCP(CP_UTF8); // 让键盘输入也按 UTF-8 处理，避免中文字符串乱码
    SetConsoleTitleW(L"C 语言阶梯教程 - 从入门到工程实战"); // 设置窗口标题，方便识别程序
#else // 非 Windows 平台不需要额外设置
    // Linux 和 macOS 终端通常已经是 UTF-8，无需修改代码页
#endif // 控制台环境设置结束
} // prepare_console 函数结束

static void print_help(const char *program) { // 说明命令行参数用法
    printf("用法：%s [选项]\n", program); // 输出基本命令格式
    puts("  --list, -l       列出全部章节，不进入交互学习"); // 说明列出目录的选项
    puts("  --chapter N      直接进入第 N 章，例如 --chapter 3"); // 说明直接跳章选项
    puts("  --version, -v    显示版本号"); // 说明版本选项
    puts("  --help, -h       显示本帮助"); // 说明帮助选项
    puts("  不带参数         进入主菜单，可保存学习进度"); // 说明默认交互模式
    puts(""); // 输出一个空行，让帮助信息更易读
    puts("学习建议：先按顺序完成第 1-11 章，再独立实现第 12 章项目。"); // 给出整体学习路径
} // print_help 函数结束

static void print_roadmap(void) { // 打印从初学到精通的完整路线
    ui_heading("学习路线：四阶段，从语法到工程能力"); // 显示路线标题
    puts("  阶段一 基础篇（第 1-3 章）"); // 第一阶段标题
    puts("    目标：能独立写出带输入输出、判断和循环的小程序。"); // 说明阶段目标
    puts("    重点：编译流程、类型、运算符、分支、循环、调试基本错误。"); // 列出学习重点
    puts("  阶段二 进阶篇（第 4-7 章）"); // 第二阶段标题
    puts("    目标：用函数、数组、字符串和指针组织中等规模程序。"); // 说明阶段目标
    puts("    重点：值传递、递归、排序查找、C 字符串、地址与指针运算。"); // 列出学习重点
    puts("  阶段三 深入篇（第 8-11 章）"); // 第三阶段标题
    puts("    目标：管理内存和自定义数据，读写文件，完成多文件工程。"); // 说明阶段目标
    puts("    重点：malloc/free、链表、结构体、文件、预处理器、位运算。"); // 列出学习重点
    puts("  阶段四 实战篇（第 12 章及之后）"); // 第四阶段标题
    puts("    目标：把语法组合成可维护、可测试、能持久保存数据的项目。"); // 说明阶段目标
    puts("    重点：需求拆解、接口设计、输入校验、错误处理、测试与重构。"); // 列出学习重点
    puts(""); // 空行分隔
    puts("  精通的标准不是“看过”，而是能解释、能重写、能调试、能迁移。"); // 强调练习与理解
    puts("  每章请按“阅读知识 -> 运行案例 -> 完成小测 -> 独立做作业”的顺序学习。"); // 给出章节学习法
    puts("  遇到不会的语法，先写最小实验，再用编译器警告和调试器定位问题。"); // 给出排错策略
    ui_pause(); // 留出阅读时间
} // print_roadmap 函数结束

static void load_progress(void) { // 从 progress.txt 恢复已完成的章节
    FILE *file = NULL; // 文件指针初始化为空，便于后续检查
    int chapter = 0; // 临时保存读到的章节编号
    int i = 0; // 循环变量，用于清空进度数组
    for (i = 0; i < CHAPTER_COUNT; i++) { // 先假设所有章节都未完成
        g_completed[i] = false; // 重置每一章的完成状态
    } // 状态初始化结束
    file = fopen(PROGRESS_FILE, "r"); // 以只读文本模式打开进度文件
    if (file == NULL) { // 首次运行时文件可能不存在
        return; // 没有进度文件不是错误，直接以全未完成开始
    } // 文件打开检查结束
    while (fscanf(file, "%d", &chapter) == 1) { // 连续读取章节编号，直到文件结束或格式不符
        if (chapter >= 1 && chapter <= CHAPTER_COUNT) { // 只接受合法章节号
            g_completed[chapter - 1] = true; // 把对应章节标记为已完成
        } // 合法性检查结束
    } // 进度读取循环结束
    fclose(file); // 关闭文件，释放系统资源
} // load_progress 函数结束

static void save_progress(void) { // 把当前进度写回 progress.txt
    FILE *file = NULL; // 文件指针
    int i = 0; // 章节循环变量
    file = fopen(PROGRESS_FILE, "w"); // 以覆盖写模式打开文件
    if (file == NULL) { // 如果当前目录不可写或文件被占用
        printf("  警告：无法保存学习进度（%s）。\n", strerror(errno)); // 输出错误原因但不中断学习
        return; // 保存失败时安全返回
    } // 文件打开检查结束
    for (i = 0; i < CHAPTER_COUNT; i++) { // 遍历所有章节
        if (g_completed[i]) { // 只写入已完成的章节编号
            fprintf(file, "%d\n", i + 1); // 每行一个编号，便于人工查看和程序读取
        } // 已完成判断结束
    } // 写入循环结束
    fclose(file); // 关闭文件，确保内容刷新到磁盘
} // save_progress 函数结束

static int count_completed(void) { // 统计已完成章节数
    int count = 0; // 计数器从零开始
    int i = 0; // 数组下标
    for (i = 0; i < CHAPTER_COUNT; i++) { // 遍历完成状态数组
        if (g_completed[i]) { // 如果当前章节已完成
            count++; // 完成数量加一
        } // 判断结束
    } // 遍历结束
    return count; // 返回统计结果
} // count_completed 函数结束

static void reset_progress(void) { // 在用户确认后清空学习进度
    int i = 0; // 数组下标
    if (!ui_confirm("确定要清空全部学习进度吗？")) { // 重置属于破坏性操作，必须二次确认
        puts("  已取消，进度保持不变。"); // 告知用户没有改动
        return; // 直接返回主菜单
    } // 确认处理结束
    for (i = 0; i < CHAPTER_COUNT; i++) { // 遍历所有章节
        g_completed[i] = false; // 逐章清除完成标记
    } // 清除循环结束
    save_progress(); // 立即把空进度写入文件
    puts("  学习进度已清空。"); // 反馈操作结果
} // reset_progress 函数结束

static void print_chapter_menu(void) { // 显示带进度状态的主菜单
    int i = 0; // 章节遍历下标
    int completed = 0; // 已完成章节数量
    completed = count_completed(); // 统计当前完成情况
    printf("\n  ── 课程目录 ──  已完成 %d/%d 章（%d%%）\n", completed, CHAPTER_COUNT, completed * 100 / CHAPTER_COUNT); // 显示总进度
    for (i = 0; i < CHAPTER_COUNT; i++) { // 逐章显示目录
        printf("  %s [%2d] %-8s %s\n", g_completed[i] ? "✓" : " ", g_chapters[i].number, g_chapters[i].stage, g_chapters[i].title); // 显示完成标记、编号、阶段和标题
    } // 章节遍历结束
    puts("  ------------------------------------------------------------"); // 分隔目录与功能选项
    puts("  [13] 查看学习路线        [14] 重置进度        [0] 退出"); // 显示辅助功能入口
    puts("  提示：先读知识、再跑案例、随后完成小测和作业。"); // 提醒推荐学习顺序
} // print_chapter_menu 函数结束

static void run_chapter(int number) { // 运行指定章节并在正常返回后记录进度
    int index = number - 1; // 把章节号转换为数组下标
    bool first_time = false; // 记录本次学习前是否尚未完成
    if (number < 1 || number > CHAPTER_COUNT) { // 防御性检查，避免越界访问
        puts("  错误：章节编号超出范围。"); // 提示错误
        return; // 直接返回
    } // 范围检查结束
    first_time = !g_completed[index]; // 先保存原来的完成状态
    g_chapters[index].lesson(); // 调用对应章节的教学函数
    if (first_time) { // 只有第一次完成时才写文件和显示新记录
        g_completed[index] = true; // 标记本章已经完成
        save_progress(); // 保存最新进度
        puts("\n  ✓ 已记录本章学习进度。继续加油！"); // 反馈进度保存结果
    } else { // 如果之前已经完成过
        puts("\n  本章为复习模式，进度保持不变。"); // 说明复习不会改变完成状态
    } // 状态处理结束
    ui_pause(); // 返回主菜单前等待用户确认
} // run_chapter 函数结束

static int parse_chapter_number(const char *text) { // 把命令行参数解析成章节号
    char *end = NULL; // 保存解析停止的位置
    long value = 0; // 先使用 long 扩大可表示范围
    if (text == NULL || *text == '\0') { // 空字符串无法解析
        return 0; // 返回 0 表示非法章节
    } // 空串检查结束
    errno = 0; // 清零错误码
    value = strtol(text, &end, 10); // 按十进制解析数字
    if (errno == ERANGE || end == text || *end != '\0') { // 溢出、没有数字或存在多余字符都视为非法
        return 0; // 返回非法标记
    } // 格式检查结束
    if (value < 1 || value > CHAPTER_COUNT) { // 检查章节号是否在目录范围内
        return 0; // 超出范围同样非法
    } // 范围检查结束
    return (int)value; // 转为 int 返回合法章节号
} // parse_chapter_number 函数结束

int main(int argc, char *argv[]) { // 程序入口，支持命令行参数和交互菜单
    int choice = 0; // 保存主菜单选择
    int i = 0; // 通用循环变量
    prepare_console(); // 先准备控制台编码，保证第一行中文就正常
    load_progress(); // 启动时读取历史学习进度
    if (argc > 1) { // 有命令行参数时先处理非交互模式
        if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) { // 匹配帮助选项
            print_help(argv[0]); // 输出帮助信息
            return 0; // 帮助打印完毕，正常退出
        } // 帮助选项处理结束
        if (strcmp(argv[1], "--version") == 0 || strcmp(argv[1], "-v") == 0) { // 匹配版本选项
            printf("%s %s\n", TUTOR_NAME, TUTOR_VERSION); // 输出版本号
            return 0; // 正常退出
        } // 版本选项处理结束
        if (strcmp(argv[1], "--list") == 0 || strcmp(argv[1], "-l") == 0) { // 匹配列出章节选项
            printf("%s %s\n", TUTOR_NAME, TUTOR_VERSION); // 先输出程序标题
            for (i = 0; i < CHAPTER_COUNT; i++) { // 遍历章节目录
                printf("%2d. [%s] %s\n", g_chapters[i].number, g_chapters[i].stage, g_chapters[i].title); // 输出编号、阶段和标题
            } // 章节目录遍历结束
            return 0; // 列表打印完毕，正常退出
        } // 列表选项处理结束
        if (strcmp(argv[1], "--chapter") == 0) { // 匹配直接进入章节选项
            int requested = 0; // 保存解析出的章节号
            if (argc < 3) { // --chapter 后面必须跟编号
                fputs("错误：--chapter 需要一个 1-12 的章节号。\n", stderr); // 向标准错误输出原因
                return 2; // 使用非零退出码表示参数错误
            } // 参数个数检查结束
            requested = parse_chapter_number(argv[2]); // 解析章节号
            if (requested == 0) { // 如果解析失败
                fprintf(stderr, "错误：无效章节号“%s”，应为 1-%d。\n", argv[2], CHAPTER_COUNT); // 说明合法范围
                return 2; // 参数错误退出
            } // 解析检查结束
            ui_banner(); // 显示程序横幅
            run_chapter(requested); // 直接运行目标章节
            return 0; // 章节结束，正常退出
        } // 跳章选项处理结束
        fprintf(stderr, "未知选项：%s\n", argv[1]); // 对无法识别的参数给出明确错误
        print_help(argv[0]); // 同时显示用法，方便修正
        return 2; // 参数错误使用退出码 2
    } // 命令行参数处理结束
    ui_banner(); // 交互模式启动时显示横幅
    printf("  首次使用？建议先选择 [13] 查看学习路线。\n"); // 给新用户一个入口建议
    while (true) { // 主菜单循环，直到用户选择退出或输入结束
        print_chapter_menu(); // 每次都重新绘制目录和进度
        choice = ui_read_int("\n请选择章节或功能：", 0, MENU_RESET); // 读取 0-14 的菜单编号
        if (choice < 0) { // 输入结束会返回负数
            puts("\n  输入结束，正在保存进度并退出。"); // 友好提示
            break; // 离开主循环
        } // 输入结束判断结束
        if (choice == 0) { // 选择退出
            break; // 离开主循环
        } // 退出选择处理结束
        if (choice == MENU_ROADMAP) { // 选择查看学习路线
            print_roadmap(); // 展示四阶段路线
            continue; // 回到主菜单
        } // 路线选项处理结束
        if (choice == MENU_RESET) { // 选择重置进度
            reset_progress(); // 执行带确认的重置
            continue; // 回到主菜单
        } // 重置选项处理结束
        run_chapter(choice); // 运行第 choice 章
    } // 主菜单循环结束
    save_progress(); // 正常退出前再次保存，防止异常中断丢失
    puts("  再见！保持动手写代码的习惯。"); // 输出告别语
    return 0; // 程序正常结束
} // main 函数结束
