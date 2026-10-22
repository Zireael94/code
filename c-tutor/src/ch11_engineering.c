#include "tutor.h" // 引入教程公共接口和标准库
#include <assert.h> // 提供 assert 断言宏，用于检查“绝不该发生”的条件

#define SQUARE_BAD(x) x * x // 故意缺少括号的错误宏，用来观察展开陷阱
#define SQUARE_OK(x) ((x) * (x)) // 正确宏：每个参数和整体表达式都加括号
#define MAX_MACRO(a, b) ((a) > (b) ? (a) : (b)) // 取较大值的函数式宏
#define ARRAY_COUNT(array) (sizeof(array) / sizeof((array)[0])) // 在数组可见作用域内求元素个数
#define PERM_READ (1u << 0) // 第 0 位表示读权限
#define PERM_WRITE (1u << 1) // 第 1 位表示写权限
#define PERM_EXECUTE (1u << 2) // 第 2 位表示执行权限
#define PERM_ADMIN (1u << 3) // 第 3 位表示管理权限

#ifdef _WIN32 // 根据编译目标平台选择文本
#define PLATFORM_NAME "Windows" // Windows 平台名称
#else // 其他平台
#define PLATFORM_NAME "非 Windows 平台" // 通用平台名称
#endif // 平台判断结束

static void print_binary(unsigned int value); // 函数声明：按 32 位输出二进制
static bool has_bit(unsigned int flags, unsigned int mask); // 函数声明：检查掩码位是否全部置位
static int square_inline(int value); // 函数声明：用内联函数替代有风险的函数式宏

static void print_binary(unsigned int value) { // 从最高位到最低位打印二进制
    int bit = 0; // 位下标，从 31 递减到 0
    fputs("0b", stdout); // 输出二进制前缀
    for (bit = 31; bit >= 0; bit--) { // 逐位检查
        putchar(((value >> bit) & 1U) ? '1' : '0'); // 输出当前位是 0 还是 1
        if (bit > 0 && bit % 4 == 0) { // 每 4 位插入一个分隔符
            putchar(' '); // 用空格分组，便于阅读
        } // 分组判断结束
    } // 位循环结束
} // print_binary 函数结束

static bool has_bit(unsigned int flags, unsigned int mask) { // 检查某组标志位
    return (flags & mask) == mask; // 只有掩码中所有位都置位才返回 true
} // has_bit 函数结束

static int square_inline(int value) { // 内联函数有类型检查，也不会重复求值参数
    return value * value; // 返回平方
} // square_inline 函数结束

static void demo_bitflags(void) { // 案例一：位标志、掩码与 endianness
    const char *options[] = {"读权限", "写权限", "执行权限", "管理权限"}; // 四个权限位
    unsigned int permissions = PERM_READ | PERM_WRITE; // 初始拥有读和写权限
    unsigned int set_example = 0; // 演示置位
    unsigned int clear_example = 0; // 演示清位
    unsigned int toggle_example = 0; // 演示翻转
    unsigned int marker = 0x01020304u; // 用于观察字节顺序的标记值
    unsigned char *bytes = NULL; // 指向标记值的字节视图
    int choice = -1; // 用户选择要切换的权限
    ui_example("位运算：用一位表示一个开关"); // 显示案例标题
    printf("\n  初始权限："); // 输出提示
    print_binary(permissions); // 输出二进制形式
    printf("  （读=%s，写=%s，执行=%s，管理=%s）\n", has_bit(permissions, PERM_READ) ? "有" : "无", has_bit(permissions, PERM_WRITE) ? "有" : "无", has_bit(permissions, PERM_EXECUTE) ? "有" : "无", has_bit(permissions, PERM_ADMIN) ? "有" : "无"); // 输出每个权限状态
    set_example = permissions | PERM_EXECUTE; // 按位或：打开执行权限
    clear_example = set_example & ~PERM_WRITE; // 按位与取反：关闭写权限
    toggle_example = clear_example ^ PERM_ADMIN; // 按位异或：翻转管理权限
    printf("  置位：flags | MASK  = "); // 输出置位说明
    print_binary(set_example); // 输出置位后的结果
    putchar('\n'); // 换行
    printf("  清位：flags & ~MASK = "); // 输出清位说明
    print_binary(clear_example); // 输出清位后的结果
    putchar('\n'); // 换行
    printf("  翻转：flags ^ MASK  = "); // 输出翻转说明
    print_binary(toggle_example); // 输出翻转后的结果
    putchar('\n'); // 换行
    choice = ui_choose("请选择要切换状态的权限：", options, 4); // 读取权限选择
    if (choice < 0) { // 用户取消
        puts("  已取消切换。"); // 提示取消
        return; // 结束案例
    } // 选择检查结束
    permissions ^= (1u << choice); // 用异或切换对应位
    printf("\n  切换后："); // 输出提示
    print_binary(permissions); // 输出切换后的二进制
    printf("  （读=%s，写=%s，执行=%s，管理=%s）\n", has_bit(permissions, PERM_READ) ? "有" : "无", has_bit(permissions, PERM_WRITE) ? "有" : "无", has_bit(permissions, PERM_EXECUTE) ? "有" : "无", has_bit(permissions, PERM_ADMIN) ? "有" : "无"); // 输出切换后的权限状态
    bytes = (unsigned char *)&marker; // 用字符指针逐字节查看整数
    printf("\n  标记值 0x01020304 的字节序列：%02X %02X %02X %02X\n", (unsigned int)bytes[0], (unsigned int)bytes[1], (unsigned int)bytes[2], (unsigned int)bytes[3]); // 输出实际内存字节
    if (bytes[0] == 0x04u) { // 最低有效字节在前
        puts("  你的机器是小端序（Little Endian）。"); // 输出小端结论
    } else if (bytes[0] == 0x01u) { // 最高有效字节在前
        puts("  你的机器是大端序（Big Endian）。"); // 输出大端结论
    } else { // 其他异常情况
        puts("  字节顺序无法按常见方式判断，请检查环境。"); // 输出提示
    } // 字节序判断结束
    puts("  口诀：与 1 核对状态，或 1 置位，与 ~ 清位，异或 1 翻转。"); // 总结位操作
} // demo_bitflags 函数结束

static void demo_macros(void) { // 案例二：宏展开陷阱、内联函数与条件编译
    int n = 3 + 1; // 故意使用表达式而不是简单常量
    int values[5] = {1, 2, 3, 4, 5}; // 用于演示数组长度宏
    int max_value = 0; // 保存宏计算结果
    ui_example("预处理器：文本替换的威力与陷阱"); // 显示案例标题
    printf("\n  SQUARE_BAD(3 + 1) = %d（错误展开：3 + 1 * 3 + 1）\n", SQUARE_BAD(3 + 1)); // 直接传表达式，暴露缺少括号的展开错误
    printf("  SQUARE_OK(3 + 1)  = %d\n", SQUARE_OK(3 + 1)); // 正确展开为 ((3 + 1) * (3 + 1))
    printf("  square_inline(n)   = %d（n = 3 + 1）\n", square_inline(n)); // 内联函数先求值再传参
    max_value = MAX_MACRO(12, 37); // 展开条件表达式
    printf("  MAX_MACRO(12, 37) = %d\n", max_value); // 输出较大值
    printf("  ARRAY_COUNT(values) = %lu\n", (unsigned long)ARRAY_COUNT(values)); // 在数组仍可见时计算元素个数
    printf("  当前编译平台：%s\n", PLATFORM_NAME); // 输出条件编译选中的文本
    assert(sizeof(int) >= 2); // 断言基本类型至少 16 位，发布版可定义 NDEBUG 关闭
    puts("  assert 适合检查内部不变量；用户输入错误应走正常错误处理，不要靠断言。"); // 说明断言用途
    puts("  宏会在编译前做文本替换，可能重复求值参数；能用内联函数就不要用复杂宏。"); // 总结宏风险
    puts("  MAX_MACRO(i++, 0) 可能让 i 自增两次，这是函数式宏最危险的陷阱之一。"); // 强调多次求值
} // demo_macros 函数结束

static void demo_build_model(void) { // 案例三：翻译单元、链接与工程化命令
    ui_example("从源文件到程序：编译与链接模型"); // 显示案例标题
    puts("\n  当前教程不是单个 .c 文件，而是由多个翻译单元组成："); // 说明多文件结构
    puts("    main.c + ui.c + ch01...ch12.c 各自编译成目标文件，再链接成一个 c_tutor。"); // 说明链接过程
    puts("    头文件只放声明、宏和类型；函数的定义放在 .c 文件中，避免重复定义。"); // 说明头文件职责
    puts("    #ifndef/#define/#endif 形成包含守卫，防止同一头文件被重复展开。"); // 说明包含守卫
    puts("  static 在文件作用域表示“内部链接”，其他翻译单元看不到这个符号。"); // 说明文件级 static
    puts("  extern 用于声明“这个符号在别处定义”，编译器负责检查类型，链接器负责找地址。"); // 说明 extern
    puts("\n  常用构建命令："); // 输出提示
    puts("    gcc -std=c11 -Wall -Wextra -Wpedantic -Iinclude -c src/ui.c -o ui.o"); // 展示只编译不链接
    puts("    gcc -std=c11 -Wall -Wextra -Wpedantic -Iinclude src/*.c -o c_tutor.exe"); // 展示一次编译链接
    puts("    gcc -std=c11 -g -fsanitize=address src/*.c -o c_tutor_asan.exe"); // 展示 AddressSanitizer
    puts("  工程习惯：先消除警告，再跑测试，最后用调试器和消毒器检查内存问题。"); // 总结质量流程
    puts("  阅读大型项目时，从 main 和头文件开始，沿着函数声明找到实现和调用点。"); // 给出阅读方法
} // demo_build_model 函数结束

void lesson_11_engineering(void) { // 第 11 章教学入口
    int correct = 0; // 记录小测答对题数
    const char *homework[] = { // 课后作业文本
        "实现 set_bit、clear_bit、toggle_bit、test_bit 并打印 8 位权限状态。", // 作业一
        "分别用宏和内联函数实现 MIN/MAX/AREA，构造参数带副作用的测试。", // 作业二
        "检测本机字节序，并用移位实现 uint32_t 的字节序反转。", // 作业三
        "把一个小程序拆成 .h 和 .c，分别编译再链接，写出自己的 Makefile。", // 作业四
        "排查五个工程错误：宏缺括号、重复定义、漏包含守卫、移位越界、警告被忽略。" // 作业五
    }; // 作业数组结束
    ui_heading("第 11 章  预处理器、位运算与工程化"); // 显示章节标题
    ui_section("本章目标"); // 显示目标小节
    ui_note("理解预处理、编译、汇编、链接四个阶段及各自的产物。"); // 列出目标一
    ui_note("掌握对象宏、函数式宏、条件编译、包含守卫及其陷阱。"); // 列出目标二
    ui_note("熟练使用按位与、或、异或、取反、移位和掩码。"); // 列出目标三
    ui_note("建立多文件组织、严格编译、断言、调试与测试的工程习惯。"); // 列出目标四
    ui_section("知识讲解"); // 开始知识讲解
    puts("  1. 预处理阶段"); // 讲解第一点标题
    puts("     #include 把头文件内容插入；#define 做文本替换；#if 系列决定哪些代码参与编译。"); // 说明预处理
    puts("     宏不是函数：没有类型检查、没有作用域概念，参数可能被重复求值。"); // 强调宏本质
    puts("     函数式宏的每个参数和整体表达式都要加括号；复杂逻辑优先 static inline。"); // 给出安全规则
    puts("  2. 多文件工程"); // 讲解第二点标题
    puts("     每个 .c 是独立翻译单元；头文件提供接口，链接器把各目标文件中的符号拼起来。"); // 说明翻译单元
    puts("     声明可以重复，定义通常只能出现一次；static 可把符号限制在当前文件。"); // 说明声明与定义
    puts("     头文件必须有包含守卫，并且只放必要内容，避免循环包含和编译变慢。"); // 强调头文件规范
    puts("  3. 位运算"); // 讲解第三点标题
    puts("     & 按位与、| 按位或、^ 按位异或、~ 按位取反，<< >> 左右移位。"); // 列出运算符
    puts("     置位 flags |= MASK；清位 flags &= ~MASK；翻转 flags ^= MASK；检查 (flags & MASK) != 0。"); // 总结位操作
    puts("     左移位数必须小于类型宽度；有符号负数右移结果由实现决定，优先使用无符号。"); // 强调移位风险
    puts("  4. 质量工具"); // 讲解第四点标题
    puts("     编译时开启 -Wall -Wextra -Wpedantic，把警告当作 bug 处理。"); // 建议警告选项
    puts("     调试用 -g 生成符号；AddressSanitizer 检查越界、释放后使用、泄漏等问题。"); // 建议调试选项
    puts("     断言检查内部不变量；单元测试覆盖正常、边界和错误路径。"); // 建议测试
    ui_pause(); // 等待阅读
    ui_section("运行案例"); // 开始案例
    demo_bitflags(); // 运行位标志案例
    ui_pause(); // 暂停阅读
    demo_macros(); // 运行宏案例
    ui_pause(); // 暂停阅读
    demo_build_model(); // 运行工程模型讲解
    ui_section("本章小测"); // 开始小测
    if (ui_quiz("函数式宏 #define SQ(x) x * x 的主要风险是什么？", "不能编译", "缺少括号导致错误展开，参数还可能被重复求值", "只能处理整数", "会创建全局变量", 'B', "文本替换不遵守函数调用语义，必须给参数和整体表达式加括号。")) { // 第一题
        correct++; // 答对计数加一
    } // 第一题结束
    if (ui_quiz("检查 flags 的 READ 位是否开启，正确写法是？", "flags & READ == 1", "(flags & READ) != 0", "flags | READ", "flags ^ READ", 'B', "& 的优先级低于 ==，必须加括号；只要结果非 0 就说明该位被置位。")) { // 第二题
        correct++; // 答对计数加一
    } // 第二题结束
    if (ui_quiz("文件作用域的 static 函数主要表示什么？", "函数返回后值仍保留", "只在本翻译单元可见（内部链接）", "函数不能被调用", "自动内联", 'B', "文件级 static 限制链接可见性；函数内的 static 局部变量才表示跨调用保留值。")) { // 第三题
        correct++; // 答对计数加一
    } // 第三题结束
    ui_homework(homework, (int)(sizeof(homework) / sizeof(homework[0]))); // 显示作业
    ui_complete(11, correct, 3); // 显示完成信息和小测成绩
} // lesson_11_engineering 函数结束

// ============================================================================
// 第 11 章课后作业（从“能运行”升级到“可维护、可验证”）
// 1. 位操作库：实现 set_bit、clear_bit、toggle_bit、test_bit、print_bits。
//    用 8 位模拟权限系统，输入操作和位号，直到输入 0 退出；禁止越界移位。
// 2. 宏与内联：用宏和内联函数分别实现 MIN、MAX、AREA；构造 i++ 作为参数测试。
//    记录宏产生多次求值的实际结果，并给出为什么内联函数更安全的解释。
// 3. 字节序工具：检测本机大小端，实现 uint32_t 字节序反转（使用移位和掩码）。
//    要求：不依赖 union 类型双关；测试 0x01020304 和 0x000000FF。
// 4. 工程拆分：把“学生成绩排序”拆成 student.h、student.c、main.c。
//    分别编译成 .o 再链接，写 Makefile 管理 all、run、clean 三个目标。
// 5. 错误诊所：构造宏缺括号、头文件重复定义、漏写包含守卫、移位 32 位、忽略警告。
//    记录编译器/链接器输出，说明每个错误属于哪一阶段，并给出修复。
//
// 提交标准：
// - 编译时使用 -Wall -Wextra -Wpedantic，目标是零警告。
// - 所有移位都检查位数范围，所有掩码都使用无符号常量。
// - 能用“翻译单元、符号、链接”解释多文件程序如何变成可执行文件。
// ============================================================================
