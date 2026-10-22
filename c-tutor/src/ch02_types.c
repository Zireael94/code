#include "tutor.h" // 引入教程公共接口和标准库

static void demo_sizes(void) { // 案例一：观察常见类型的大小和取值范围
    ui_example("用 sizeof 看清数据类型的边界"); // 显示案例标题
    printf("  一个字节包含 %d 个二进制位（CHAR_BIT）。\n", CHAR_BIT); // 说明字节与位的关系
    printf("  %-20s %-8s %s\n", "类型", "字节数", "范围或说明"); // 输出表头
    printf("  %-20s %-8lu %d 到 %d\n", "char", (unsigned long)sizeof(char), CHAR_MIN, CHAR_MAX); // 显示 char 的范围
    printf("  %-20s %-8lu %d 到 %d\n", "short", (unsigned long)sizeof(short), SHRT_MIN, SHRT_MAX); // 显示 short 的范围
    printf("  %-20s %-8lu %d 到 %d\n", "int", (unsigned long)sizeof(int), INT_MIN, INT_MAX); // 显示 int 的范围
    printf("  %-20s %-8lu %ld 到 %ld\n", "long", (unsigned long)sizeof(long), LONG_MIN, LONG_MAX); // 显示 long 的范围
    printf("  %-20s %-8lu 典型范围约 ±9.22e18（至少 64 位）\n", "long long", (unsigned long)sizeof(long long)); // 显示 long long 的大小与典型范围
    printf("  %-20s %-8lu %u 到 %u\n", "unsigned int", (unsigned long)sizeof(unsigned int), 0U, UINT_MAX); // 显示无符号整数的范围
    printf("  %-20s %-8lu 约 ±3.4e38，精度 %d 位十进制\n", "float", (unsigned long)sizeof(float), FLT_DIG); // 显示 float 的大小和精度
    printf("  %-20s %-8lu 约 ±1.8e308，精度 %d 位十进制\n", "double", (unsigned long)sizeof(double), DBL_DIG); // 显示 double 的大小和精度
    printf("  %-20s %-8lu 至少和 double 一样宽\n", "long double", (unsigned long)sizeof(long double)); // 显示扩展精度类型
    puts("  结论：C 标准只规定最小范围，不保证所有平台宽度完全一样。"); // 强调平台差异
    puts("  写跨平台代码时，用 sizeof 和 <limits.h> 判断范围，不要凭感觉猜。"); // 给出工程建议
} // demo_sizes 函数结束

static void demo_time(void) { // 案例二：秒数换算成小时、分钟和秒
    int total_seconds = 0; // 保存用户输入的原始秒数
    int hours = 0; // 保存整小时数
    int minutes = 0; // 保存剩余整分钟数
    int seconds = 0; // 保存最后剩余秒数
    ui_example("整数除法与取余：时间换算器"); // 显示案例标题
    total_seconds = ui_read_int("请输入秒数（0-86400）：", 0, 86400); // 读取一天以内的秒数
    if (total_seconds < 0) { // 输入结束时会返回 -1
        puts("  已取消输入。"); // 提示取消
        return; // 结束案例
    } // 输入检查结束
    hours = total_seconds / 3600; // 整数除法得到完整小时数
    minutes = (total_seconds % 3600) / 60; // 先去掉小时，再换算完整分钟
    seconds = total_seconds % 60; // 对 60 取余得到剩余秒数
    printf("\n  %d 秒 = %d 小时 %d 分 %d 秒\n", total_seconds, hours, minutes, seconds); // 按“时分秒”格式输出
    puts("  关键：/ 对整数做“截断除法”，% 得到余数；两者配合可拆分任意进位制。"); // 总结运算规律
} // demo_time 函数结束

static void demo_money(void) { // 案例三：把金额拆成最少的纸币张数
    int amount = 0; // 保存当前待兑换的金额，会逐步减少
    int original = 0; // 保存原始金额，最后用于显示
    int count_50 = 0; // 50 元纸币张数
    int count_20 = 0; // 20 元纸币张数
    int count_10 = 0; // 10 元纸币张数
    int count_5 = 0; // 5 元纸币张数
    int count_1 = 0; // 1 元纸币张数
    int total_notes = 0; // 总张数
    ui_example("整数取余：零钱兑换"); // 显示案例标题
    amount = ui_read_int("请输入兑换金额（0-999 元）：", 0, 999); // 读取金额
    if (amount < 0) { // 判断输入是否结束
        puts("  已取消输入。"); // 提示取消
        return; // 结束案例
    } // 输入检查结束
    original = amount; // 备份原始金额
    count_50 = amount / 50; // 尽可能多地兑换 50 元
    amount = amount % 50; // 更新还剩下的金额
    count_20 = amount / 20; // 再尽可能多地兑换 20 元
    amount = amount % 20; // 更新余额
    count_10 = amount / 10; // 兑换 10 元纸币
    amount = amount % 10; // 更新余额
    count_5 = amount / 5; // 兑换 5 元纸币
    amount = amount % 5; // 更新余额
    count_1 = amount; // 剩下的都是一元纸币
    total_notes = count_50 + count_20 + count_10 + count_5 + count_1; // 统计总张数
    printf("\n  %d 元可兑换：\n", original); // 输出标题
    printf("    50 元 × %d 张\n", count_50); // 输出 50 元张数
    printf("    20 元 × %d 张\n", count_20); // 输出 20 元张数
    printf("    10 元 × %d 张\n", count_10); // 输出 10 元张数
    printf("     5 元 × %d 张\n", count_5); // 输出 5 元张数
    printf("     1 元 × %d 张\n", count_1); // 输出 1 元张数
    printf("  共 %d 张纸币。\n", total_notes); // 输出总张数
    puts("  这类“尽可能取大面额”的策略叫贪心法，后面的算法章节还会遇到。"); // 联系算法思想
} // demo_money 函数结束

static void demo_operators(void) { // 案例四：运算符、优先级、类型转换与自增
    int a = 0; // 第一门课成绩
    int b = 0; // 第二门课成绩
    int c = 0; // 第三门课成绩
    int int_average = 0; // 保存整数平均分
    double real_average = 0.0; // 保存浮点平均分
    int x = 5; // 用于演示后置自增
    int y = 0; // 保存后置自增表达式的结果
    int m = 5; // 用于演示前置自增
    int n = 0; // 保存前置自增表达式的结果
    double price = 9.99; // 带小数的价格
    int truncated = 0; // 保存强制转换后的整数价格
    ui_example("运算符实验：为什么 7 / 2 不等于 3.5"); // 显示案例标题
    a = ui_read_int("请输入第 1 门成绩（0-100）：", 0, 100); // 读取第一门成绩
    if (a < 0) { // 检查输入是否结束
        return; // 结束案例
    } // 第一门检查结束
    b = ui_read_int("请输入第 2 门成绩（0-100）：", 0, 100); // 读取第二门成绩
    if (b < 0) { // 检查输入是否结束
        return; // 结束案例
    } // 第二门检查结束
    c = ui_read_int("请输入第 3 门成绩（0-100）：", 0, 100); // 读取第三门成绩
    if (c < 0) { // 检查输入是否结束
        return; // 结束案例
    } // 第三门检查结束
    int_average = (a + b + c) / 3; // 三个整数相除，结果被截断为整数
    real_average = (a + b + c) / 3.0; // 加入 3.0 后，表达式提升为 double
    printf("\n  整数平均：%d\n", int_average); // 输出整数平均分
    printf("  浮点平均：%.2f\n", real_average); // 输出保留两位小数的平均分
    printf("  2 + 3 * 4 = %d（乘法先算）\n", 2 + 3 * 4); // 演示乘法优先级
    printf("  (2 + 3) * 4 = %d（括号改变顺序）\n", (2 + 3) * 4); // 演示括号优先级
    y = x++; // 后置自增：先把 5 赋给 y，再让 x 变成 6
    n = ++m; // 前置自增：先把 m 变成 6，再把 6 赋给 n
    printf("  后置 x++：x=%d，y=%d；前置 ++m：m=%d，n=%d\n", x, y, m, n); // 输出自增结果
    truncated = (int)price; // 强制转换会直接丢弃小数部分，而不是四舍五入
    printf("  (int)%.2f = %d，强制转换不会四舍五入。\n", price, truncated); // 解释强制转换
    puts("  建议：不要在同一条表达式里对同一变量多次自增，结果顺序难以维护。"); // 给出可读性建议
} // demo_operators 函数结束

static void demo_precision(void) { // 案例五：无符号回绕与浮点数误差
    unsigned int wrapped = UINT_MAX; // 从无符号最大值开始
    double sum = 0.1 + 0.2; // 浮点数相加，很多十进制小数无法精确表示
    ui_example("边界实验：溢出与浮点误差"); // 显示案例标题
    printf("\n  UINT_MAX = %u\n", wrapped); // 输出无符号整数的最大值
    wrapped = wrapped + 1U; // 无符号整数溢出会按 2 的幂回绕，这是标准保证的行为
    printf("  UINT_MAX + 1 = %u（无符号回绕，有定义）\n", wrapped); // 输出回绕结果
    puts("  注意：有符号整数溢出在 C 标准中是未定义行为，绝对不能依赖回绕结果。"); // 区分有符号与无符号溢出
    printf("  0.1 + 0.2 = %.17g\n", sum); // 用足够多的有效数字展示真实存储值
    printf("  与 0.3 的差：%.3e\n", fabs(sum - 0.3)); // 计算误差的绝对值
    if (fabs(sum - 0.3) < 1e-9) { // 浮点数比较应检查误差是否足够小
        puts("  判断：在 1e-9 误差范围内，可以认为它们相等。"); // 输出比较结论
    } else { // 如果误差超过阈值
        puts("  判断：不能直接用 == 比较浮点数。"); // 强调比较方法
    } // 比较分支结束
    puts("  金额计算通常改用“分”为单位做整数运算，避免浮点误差累积。"); // 给出实际工程建议
} // demo_precision 函数结束

void lesson_02_types(void) { // 第 2 章教学入口
    int correct = 0; // 记录小测答对题数
    const char *homework[] = { // 课后作业文本，与文件末尾注释一致
        "写 BMI 计算器：输入体重（kg）和身高（m），输出 BMI，保留两位小数。", // 作业一
        "把秒数换算成“天、小时、分、秒”，并验证 90061 秒的结果。", // 作业二
        "扩展零钱兑换：支持 100 元面额，并统计总张数；输入 0 时也要正常输出。", // 作业三
        "写程序比较 int 与 unsigned int 的边界，解释为什么负数转无符号会变大。", // 作业四
        "整理一张“类型、格式符、字节数、常见错误”速查表，至少覆盖 6 种类型。" // 作业五
    }; // 作业数组结束
    ui_heading("第 2 章  数据类型、变量、常量与运算符"); // 显示章节标题
    ui_section("本章目标"); // 显示目标小节
    ui_note("理解位、字节、二进制，以及整数和浮点数的表示差异。"); // 列出目标一
    ui_note("掌握基础类型、sizeof、取值范围和常用格式符。"); // 列出目标二
    ui_note("学会算术、关系、逻辑、赋值、自增和条件运算符。"); // 列出目标三
    ui_note("识别整数除法、隐式转换、浮点误差和整数溢出四类陷阱。"); // 列出目标四
    ui_section("知识讲解"); // 开始知识讲解
    puts("  1. 变量是“有名字、有类型的内存空间”"); // 讲解第一点标题
    puts("     声明告诉编译器类型和名字；初始化在创建时给值；赋值是之后改写。"); // 区分声明、初始化、赋值
    puts("     const int MAX = 100; 表示初始化后不可修改，优先用它替代魔法数字。"); // 介绍 const
    puts("  2. 整数与浮点数"); // 讲解第二点标题
    puts("     char/short/int/long/long long 都是整数家族，大小由实现决定。"); // 概括整数类型
    puts("     float/double/long double 用二进制科学计数法存储，天然存在舍入误差。"); // 概括浮点类型
    puts("     字符 'A' 本质上是整数编码值，char 也可以参与算术运算。"); // 说明字符本质
    puts("  3. 运算符与优先级"); // 讲解第三点标题
    puts("     算术：+ - * / %；关系：< <= > >= == !=；逻辑：&& || !。"); // 列出常用运算符
    puts("     赋值：= += -= *= /= %=；自增：++；条件：条件 ? 值1 : 值2。"); // 列出赋值及其他运算符
    puts("     优先级不确定时加括号，可读性比“炫技”重要。"); // 强调括号
    puts("  4. 类型转换"); // 讲解第四点标题
    puts("     隐式转换：小范围向大范围、整数向浮点通常安全，反向可能丢数据。"); // 说明隐式转换
    puts("     显式转换：(double)count / total，明确告诉读者这里需要小数结果。"); // 说明强制转换
    puts("     有符号溢出是未定义行为；无符号溢出按模 2 的位数回绕。"); // 强调溢出差异
    ui_pause(); // 等待阅读
    ui_section("运行案例"); // 开始案例
    demo_sizes(); // 运行类型大小案例
    ui_pause(); // 暂停阅读
    demo_time(); // 运行时间换算案例
    ui_pause(); // 暂停阅读
    demo_money(); // 运行零钱兑换案例
    ui_pause(); // 暂停阅读
    demo_operators(); // 运行运算符案例
    ui_pause(); // 暂停阅读
    demo_precision(); // 运行边界实验
    ui_section("本章小测"); // 开始小测
    if (ui_quiz("表达式 7 / 2 的结果是什么（两个操作数都是 int）？", "3.5", "3", "4", "编译错误", 'B', "整数除法向零截断，7 / 2 得到 3；要得到 3.5 可写 7 / 2.0。")) { // 第一题
        correct++; // 答对计数加一
    } // 第一题结束
    if (ui_quiz("哪个声明能让变量初始化后不可再被赋值？", "static int x", "const int x", "unsigned int x", "volatile int x", 'B', "const 限定变量只读；static 影响存储期，unsigned 影响表示范围。")) { // 第二题
        correct++; // 答对计数加一
    } // 第二题结束
    if (ui_quiz("比较两个 double 是否相等，更可靠的做法是？", "直接用 ==", "用 !=", "检查两者差值是否小于一个容差", "转成 int 再比较", 'C', "浮点数存在舍入误差，通常用 fabs(a - b) < epsilon 判断近似相等。")) { // 第三题
        correct++; // 答对计数加一
    } // 第三题结束
    ui_homework(homework, (int)(sizeof(homework) / sizeof(homework[0]))); // 显示作业
    ui_complete(2, correct, 3); // 显示完成信息和小测成绩
} // lesson_02_types 函数结束

// ============================================================================
// 第 2 章课后作业（在每段案例后再动手，才能真正掌握类型与运算）
// 1. BMI 计算器：输入体重 kg 和身高 m，输出 BMI = 体重 / 身高²，保留两位小数。
//    验收：身高输入 0 或负数时拒绝；分别测试 BMI 偏瘦、正常、超重的输入。
// 2. 秒数换算：输入 0-90061 的秒数，输出“天、小时、分、秒”。
//    验收：90061 秒应得到 1 天 1 小时 1 分 1 秒；0 秒也要正常输出。
// 3. 零钱兑换扩展：面额改为 100/50/20/10/5/1，输入 0-999，统计总张数。
//    进阶：再输出“平均每张面额”，注意用 double 计算。
// 4. 边界实验：打印 int、unsigned int、short 的边界，构造一个负数转无符号的例子。
//    要求：解释“-1 转成 unsigned int 为什么会变成很大的数”，写成 3 行注释。
// 5. 制作速查表：至少覆盖 char、int、long long、unsigned、float、double 六种类型。
//    每行包含：格式符、sizeof 结果、典型范围、最容易犯的错误。
//
// 提交标准：
// - 每个计算器都要拒绝非法输入，而不是悄悄算出错误结果。
// - 至少用 3 组边界数据测试（0、负数、最大值）。
// - 能解释 / 与 % 的区别，以及为什么金额通常不用 float 存储。
// ============================================================================
