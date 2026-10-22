#include "tutor.h" // 引入教程公共接口和标准库

static void demo_grade(void) { // 案例一：用 if 链和 switch 做成绩等级判断
    int score = 0; // 保存百分制成绩
    char level = 'F'; // 保存等级字符，默认先给 F
    ui_example("成绩等级判断：if / else if / switch"); // 显示案例标题
    score = ui_read_int("请输入成绩（0-100）：", 0, 100); // 读取并限制成绩范围
    if (score < 0) { // 输入结束会返回负数
        puts("  已取消输入。"); // 提示取消
        return; // 结束案例
    } // 输入检查结束
    if (score >= 90) { // 90 分及以上为 A
        level = 'A'; // 记录 A 等级
    } else if (score >= 80) { // 80-89 分为 B
        level = 'B'; // 记录 B 等级
    } else if (score >= 70) { // 70-79 分为 C
        level = 'C'; // 记录 C 等级
    } else if (score >= 60) { // 60-69 分为 D
        level = 'D'; // 记录 D 等级
    } else { // 小于 60 分
        level = 'F'; // 记录不及格等级
    } // 等级判断链结束
    printf("\n  成绩：%d，等级：%c\n", score, level); // 输出成绩和等级
    printf("  结论：%s\n", (score >= 60) ? "及格" : "不及格"); // 用条件运算符生成简短结论
    switch (level) { // 根据等级补充一句个性化反馈
        case 'A': // 处理 A
            puts("  表现优秀，可以尝试更具挑战性的项目。"); // A 的反馈
            break; // 结束 switch，防止落入下一个 case
        case 'B': // 处理 B
            puts("  基础扎实，注意减少细节失分。"); // B 的反馈
            break; // 结束 switch
        case 'C': // 处理 C
            puts("  已经及格，建议回头巩固薄弱知识点。"); // C 的反馈
            break; // 结束 switch
        case 'D': // 处理 D
            puts("  刚过及格线，需要增加练习量。"); // D 的反馈
            break; // 结束 switch
        default: // 处理其余情况，即 F
            puts("  先别灰心，把错题拆小，逐个击破。"); // F 的反馈
            break; // default 也可以写 break，保持风格一致
    } // switch 结束
    puts("  边界测试很重要：89、90、59、60 分别会得到什么等级？"); // 引导测试边界
} // demo_grade 函数结束

static void demo_calculator(void) { // 案例二：可循环使用的四则运算计算器
    char op[8] = {0}; // 保存用户输入的运算符，留出字符串结束符
    char left_text[64] = {0}; // 保存左操作数的原始输入，便于识别 q 和格式错误
    char right_text[64] = {0}; // 保存右操作数的原始输入
    char *end = NULL; // 指向数字解析结束的位置
    double left = 0.0; // 左操作数
    double right = 0.0; // 右操作数
    double result = 0.0; // 计算结果
    ui_example("switch 实战：四则运算计算器"); // 显示案例标题
    while (true) { // 允许连续计算多次
        if (!ui_read_text("请输入左操作数（输入 q 退出）：", left_text, sizeof(left_text))) { // 整行读取左操作数
            break; // 输入结束则退出
        } // 左操作数读取结束
        if (left_text[0] == 'q' || left_text[0] == 'Q') { // 允许用户用 q 退出
            puts("  已退出计算器。"); // 提示退出
            break; // 结束循环
        } // 退出检查结束
        errno = 0; // 清零错误码，准备解析数字
        end = NULL; // 重置结束指针
        left = strtod(left_text, &end); // 把输入文本解析为 double
        while (end != NULL && *end != '\0' && isspace((unsigned char)*end)) { // 跳过数字后面的空白
            end++; // 移动到下一个字符
        } // 尾部空白处理结束
        if (end == left_text || end == NULL || *end != '\0' || errno == ERANGE || !isfinite(left)) { // 检查是否整行都是合法数字
            puts("  左操作数格式错误，请重新输入。"); // 提示格式错误
            continue; // 回到循环开头
        } // 左操作数校验结束
        if (!ui_read_text("请输入运算符（+ - * /）：", op, sizeof(op))) { // 读取运算符文本
            break; // 输入结束则退出
        } // 运算符读取结束
        if (op[0] != '+' && op[0] != '-' && op[0] != '*' && op[0] != '/') { // 只接受四种运算符
            puts("  不支持的运算符，请重新输入。"); // 提示错误
            continue; // 回到循环开头
        } // 运算符合法性检查结束
        if (!ui_read_text("请输入右操作数：", right_text, sizeof(right_text))) { // 整行读取右操作数
            break; // 输入结束则退出
        } // 右操作数读取结束
        errno = 0; // 清零错误码
        end = NULL; // 重置结束指针
        right = strtod(right_text, &end); // 解析右操作数
        while (end != NULL && *end != '\0' && isspace((unsigned char)*end)) { // 跳过尾部空白
            end++; // 移动指针
        } // 右操作数尾部处理结束
        if (end == right_text || end == NULL || *end != '\0' || errno == ERANGE || !isfinite(right)) { // 检查右操作数格式
            puts("  右操作数格式错误，请重新输入。"); // 提示错误
            continue; // 回到循环开头
        } // 右操作数校验结束
        if (op[0] == '/' && fabs(right) < 1e-12) { // 除法前必须检查除数是否为 0
            puts("  错误：除数不能为 0。"); // 给出明确错误
            continue; // 重新开始一轮计算
        } // 除零检查结束
        switch (op[0]) { // 根据运算符选择计算方式
            case '+': // 加法
                result = left + right; // 计算和
                break; // 结束 switch
            case '-': // 减法
                result = left - right; // 计算差
                break; // 结束 switch
            case '*': // 乘法
                result = left * right; // 计算积
                break; // 结束 switch
            case '/': // 除法
                result = left / right; // 计算商，浮点除法不会截断
                break; // 结束 switch
            default: // 理论上到不了这里，保留安全出口
                puts("  内部错误：未知运算符。"); // 输出内部错误
                continue; // 跳过本次输出
        } // switch 结束
        printf("  %.6g %c %.6g = %.6g\n", left, op[0], right, result); // 输出计算过程与结果
        if (!ui_confirm("继续计算吗？")) { // 询问是否继续
            break; // 用户选择结束
        } // 继续确认结束
    } // 计算循环结束
} // demo_calculator 函数结束

static void demo_guess(void) { // 案例三：猜数字游戏，练习循环和 break
    int target = 0; // 保存随机生成的目标数字
    int guess = 0; // 保存玩家每次猜测
    int attempts = 0; // 记录已经猜了多少次
    const int max_attempts = 7; // 最多允许猜 7 次
    bool found = false; // 标记是否猜中
    srand((unsigned int)time(NULL)); // 用当前时间初始化随机数种子，让每局不同
    target = rand() % 100 + 1; // 生成 1-100 的随机整数
    ui_example("while + break：七次机会猜数字"); // 显示案例标题
    printf("  我已经想好一个 1-100 的数字，你有 %d 次机会。\n", max_attempts); // 说明规则
    while (attempts < max_attempts) { // 在次数用完之前反复询问
        guess = ui_read_int("请输入你的猜测（1-100，输入 0 退出）：", 0, 100); // 读取并校验猜测
        if (guess < 0 || guess == 0) { // 0 表示主动退出，负数表示输入结束
            puts("  已退出猜数字游戏。"); // 提示退出
            return; // 结束案例
        } // 输入检查结束
        attempts++; // 每猜一次计数加一
        if (guess == target) { // 猜中目标
            found = true; // 记录成功
            printf("  恭喜！你在第 %d 次猜中了 %d。\n", attempts, target); // 输出成功信息
            break; // 立刻结束循环，后面的次数不再使用
        } // 猜中分支结束
        if (guess < target) { // 猜小了
            printf("  小了。还剩 %d 次机会。\n", max_attempts - attempts); // 给出提示并显示剩余次数
        } else { // 否则一定猜大了
            printf("  大了。还剩 %d 次机会。\n", max_attempts - attempts); // 给出提示
        } // 大小提示结束
    } // 游戏循环结束
    if (!found) { // 循环结束仍未猜中
        printf("  机会用完，答案是 %d。下一局再试试二分策略。\n", target); // 公布答案并提示策略
    } // 失败处理结束
    puts("  二分策略：先猜 50，再根据“大了/小了”把范围缩小一半。"); // 引出算法思想
} // demo_guess 函数结束

static void demo_tables(void) { // 案例四：嵌套循环、continue 与提前结束
    int row = 0; // 乘法表的行号
    int col = 0; // 乘法表的列号
    int n = 0; // 待判断的整数
    int divisor = 0; // 试除因子
    int prime_count = 0; // 素数计数器
    int even_sum = 0; // 1-100 中偶数之和
    int i = 0; // 通用循环变量
    bool is_prime = true; // 当前数字是否为素数的标记
    ui_example("嵌套循环：乘法表、素数筛选与 continue"); // 显示案例标题
    puts("\n  5×5 乘法表："); // 输出乘法表标题
    for (row = 1; row <= 5; row++) { // 外层循环控制行
        for (col = 1; col <= 5; col++) { // 内层循环控制列
            printf("%4d", row * col); // 每个结果占 4 列，保持对齐
        } // 内层循环结束
        putchar('\n'); // 一行打印完以后换行
    } // 外层循环结束
    puts("\n  2-50 之间的素数："); // 输出素数标题
    for (n = 2; n <= 50; n++) { // 依次检查每个整数
        is_prime = true; // 先假设当前数字是素数
        for (divisor = 2; divisor * divisor <= n; divisor++) { // 只需试除到平方根
            if (n % divisor == 0) { // 找到因子
                is_prime = false; // 说明不是素数
                break; // 已有因子，无需继续试除
            } // 因子判断结束
        } // 内层试除结束
        if (!is_prime) { // 不是素数就跳过输出
            continue; // continue 会直接进入下一轮 n
        } // 跳过判断结束
        printf("%3d", n); // 输出一个素数
        prime_count++; // 素数计数加一
        if (prime_count % 10 == 0) { // 每输出 10 个素数
            putchar('\n'); // 换行，避免一行过长
        } // 换行判断结束
    } // 素数循环结束
    putchar('\n'); // 素数列表结束后换行
    for (i = 1; i <= 100; i++) { // 计算 1-100 的偶数和
        if (i % 2 != 0) { // 奇数不参与累加
            continue; // 跳过本轮剩余语句
        } // 奇数判断结束
        even_sum += i; // 累加偶数
    } // 求和循环结束
    printf("  1-100 的偶数和：%d\n", even_sum); // 输出偶数和
    puts("  总结：break 结束整个循环，continue 只跳过本次，二者不要混用。"); // 强调区别
} // demo_tables 函数结束

void lesson_03_control(void) { // 第 3 章教学入口
    int correct = 0; // 记录小测答对题数
    const char *homework[] = { // 课后作业文本
        "写循环版成绩统计：连续输入成绩，输入 -1 结束，计算平均分和最高分。", // 作业一
        "增强猜数字：限制 7 次、显示已猜数字，结束后询问是否再来一局。", // 作业二
        "输出 1-200 的素数，每行 10 个；统计素数个数并验证 2 是素数。", // 作业三
        "实现菜单式单位换算器：长度、重量、温度三个选项，用 switch 分发。", // 作业四
        "制造并修复三个错误：if (x = 5)、漏写 break、while 条件永远为真。" // 作业五
    }; // 作业数组结束
    ui_heading("第 3 章  分支、循环与流程控制"); // 显示章节标题
    ui_section("本章目标"); // 显示目标小节
    ui_note("掌握 if/else、switch 和条件运算符，能处理多分支逻辑。"); // 列出目标一
    ui_note("掌握 while、do-while、for 的差异和适用场景。"); // 列出目标二
    ui_note("能用 break、continue 和嵌套循环解决组合问题。"); // 列出目标三
    ui_note("建立边界测试、循环终止和输入校验意识。"); // 列出目标四
    ui_section("知识讲解"); // 开始知识讲解
    puts("  1. C 的“真”与“假”"); // 讲解第一点标题
    puts("     0 为假，非 0 为真；关系表达式的结果是 int 类型的 0 或 1。"); // 说明真值规则
    puts("     注意区分 = 赋值与 == 相等，if (x = 5) 会赋值并把 5 当真。"); // 强调常见错误
    puts("  2. 分支结构"); // 讲解第二点标题
    puts("     if/else if/else 适合范围判断；switch 适合离散的整数或字符分支。"); // 说明分流选择
    puts("     switch 的每个 case 通常以 break 结束，否则会“贯穿”到下一个 case。"); // 说明贯穿
    puts("     条件运算符 a ? b : c 适合很短的二选一表达式，复杂逻辑仍用 if。"); // 说明条件运算符
    puts("  3. 循环结构"); // 讲解第三点标题
    puts("     while 先判断再执行；do-while 至少执行一次；for 适合已知次数。"); // 对比三种循环
    puts("     三要素：初值、终止条件、更新；缺少更新很容易造成死循环。"); // 强调循环三要素
    puts("     break 立即结束最近一层循环；continue 跳过本次，进入下一次判断。"); // 区分跳转语句
    puts("  4. 常见坑"); // 讲解第四点标题
    puts("     if 后误加空语句分号；else 悬空；浮点数直接比较相等；循环边界差一。"); // 列出常见错误
    puts("     写循环前先写出“第 1 次、最后 1 次、终止后”的三个状态。"); // 给出设计方法
    ui_pause(); // 等待阅读
    ui_section("运行案例"); // 开始案例
    demo_grade(); // 运行成绩等级案例
    ui_pause(); // 暂停阅读
    demo_calculator(); // 运行计算器案例
    ui_pause(); // 暂停阅读
    demo_guess(); // 运行猜数字案例
    ui_pause(); // 暂停阅读
    demo_tables(); // 运行嵌套循环案例
    ui_section("本章小测"); // 开始小测
    if (ui_quiz("switch 的某个 case 末尾忘记写 break，会发生什么？", "编译自动停止", "只执行该 case", "继续执行后续 case 的语句", "程序一定崩溃", 'C', "这叫 fall-through（贯穿），除非有意利用，否则遗漏 break 是常见 bug。")) { // 第一题
        correct++; // 答对计数加一
    } // 第一题结束
    if (ui_quiz("哪种循环至少会执行一次循环体？", "while", "for", "do-while", "所有循环都可能一次不执行", 'C', "do-while 先执行循环体，再判断条件，因此至少执行一次。")) { // 第二题
        correct++; // 答对计数加一
    } // 第二题结束
    if (ui_quiz("for (i = 1; i <= 5; i++) 中，循环体一共执行几次？", "4 次", "5 次", "6 次", "无限次", 'B', "i 取 1、2、3、4、5，共 5 次；i=6 时条件为假，循环结束。")) { // 第三题
        correct++; // 答对计数加一
    } // 第三题结束
    ui_homework(homework, (int)(sizeof(homework) / sizeof(homework[0]))); // 显示作业
    ui_complete(3, correct, 3); // 显示完成信息和小测成绩
} // lesson_03_control 函数结束

// ============================================================================
// 第 3 章课后作业（先画流程图，再写代码；每道题都要测试边界）
// 1. 循环版成绩统计：不断输入 0-100 的成绩，输入 -1 结束。
//    输出人数、平均分、最高分、最低分；没有输入时不能除以 0。
// 2. 增强猜数字：最多 7 次，记录玩家猜过的数字，结束后询问是否再来一局。
//    进阶：根据剩余次数给出“接近/很远”的额外提示，但不能泄露答案。
// 3. 输出 1-200 的所有素数，每行 10 个；最后输出总数。
//    要求：内层循环用 divisor * divisor <= n，并用 break 提前结束。
// 4. 菜单式单位换算器：1 长度（米/厘米/英寸）、2 重量（千克/克/磅）、3 温度（摄氏/华氏）。
//    用 switch 分发，用 do-while 支持连续换算，输入 0 退出。
// 5. 错误诊所：分别写出 if (x = 5)、switch 漏 break、while (i < 10) 忘写 i++。
//    记录每个程序的实际行为、编译警告和修复方式；解释“未定义/死循环”的原因。
//
// 提交标准：
// - 每个循环都能在有限步内结束，并能解释终止条件。
// - 至少测试 0、1、边界值和非法输入。
// - 作业 2 或 4 至少完整实现一个，并保存运行记录。
// ============================================================================
