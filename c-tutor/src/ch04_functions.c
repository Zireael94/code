#include "tutor.h" // 引入教程公共接口和标准库

static double calculate_bmi(double weight_kg, double height_m); // 函数声明：先告诉编译器函数签名，定义可以放在后面
static const char *bmi_category(double value); // 函数声明：根据 BMI 返回文本描述
static int factorial_iterative(int n); // 函数声明：循环版阶乘
static int factorial_recursive(int n); // 函数声明：递归版阶乘
static int fibonacci_recursive(int n); // 函数声明：递归版斐波那契
static int gcd_recursive(int a, int b); // 函数声明：递归版最大公约数
static int next_student_id(void); // 函数声明：用 static 局部变量保存编号状态
static void hanoi(int n, char from, char to, char via); // 函数声明：汉诺塔递归函数

static double calculate_bmi(double weight_kg, double height_m) { // 计算 BMI 的纯函数，不读取输入也不打印
    return weight_kg / (height_m * height_m); // BMI 公式：体重除以身高的平方
} // calculate_bmi 函数结束

static const char *bmi_category(double value) { // 根据 BMI 数值返回对应分类文本
    if (value < 18.5) { // 低于 18.5
        return "偏瘦"; // 返回中国成人标准下的偏瘦分类
    } // 偏瘦判断结束
    if (value < 24.0) { // 18.5 到 23.9
        return "正常"; // 返回正常分类
    } // 正常判断结束
    if (value < 28.0) { // 24.0 到 27.9
        return "超重"; // 返回超重分类
    } // 超重判断结束
    return "肥胖"; // 其余情况返回肥胖
} // bmi_category 函数结束

static int factorial_iterative(int n) { // 用循环计算 n 的阶乘
    int result = 1; // 累乘结果从 1 开始
    int i = 1; // 循环变量从 1 开始
    for (i = 1; i <= n; i++) { // 依次乘到 n
        result *= i; // 把当前整数累乘进结果
    } // 循环结束
    return result; // 返回阶乘结果
} // factorial_iterative 函数结束

static int factorial_recursive(int n) { // 用递归计算 n 的阶乘
    if (n <= 1) { // 递归终止条件：0! 和 1! 都是 1
        return 1; // 直接返回基础结果
    } // 终止条件结束
    return n * factorial_recursive(n - 1); // 把问题缩小为 (n-1)!，再乘回 n
} // factorial_recursive 函数结束

static int fibonacci_recursive(int n) { // 递归计算第 n 个斐波那契数
    if (n <= 1) { // 前两项
        return n; // fib(0)=0，fib(1)=1
    } // 终止条件结束
    return fibonacci_recursive(n - 1) + fibonacci_recursive(n - 2); // 由前两项相加得到当前项
} // fibonacci_recursive 函数结束

static int gcd_recursive(int a, int b) { // 用欧几里得算法递归求最大公约数
    if (b == 0) { // 余数为 0 时
        return a; // 非零的 a 就是最大公约数
    } // 终止条件结束
    return gcd_recursive(b, a % b); // 把问题变成 gcd(b, a mod b)
} // gcd_recursive 函数结束

static int next_student_id(void) { // 演示 static 局部变量的生命周期
    static int id = 0; // 只初始化一次；函数返回后这个变量仍然存在
    id++; // 每次调用把编号加一
    return id; // 返回当前编号
} // next_student_id 函数结束

static void hanoi(int n, char from, char to, char via) { // 把 n 个盘子从 from 借助 via 移到 to
    if (n == 0) { // 没有盘子时无需移动
        return; // 递归终止
    } // 基本情况结束
    hanoi(n - 1, from, via, to); // 先把上面 n-1 个盘子移到辅助柱
    printf("%*s盘子 %d：%c -> %c\n", (6 - n) * 2, "", n, from, to); // 输出移动最大盘子的步骤
    hanoi(n - 1, via, to, from); // 再把 n-1 个盘子从辅助柱移到目标柱
} // hanoi 函数结束

static void demo_bmi(void) { // 案例一：用函数拆分 BMI 计算与分类
    double weight = 0.0; // 体重，单位千克
    double height = 0.0; // 身高，单位米
    double value = 0.0; // 计算得到的 BMI 数值
    const char *label = NULL; // 保存分类文本的指针
    ui_example("函数拆分：BMI 计算与分类"); // 显示案例标题
    weight = ui_read_double("请输入体重（20-300 kg）：", 20.0, 300.0); // 读取体重并校验范围
    if (isnan(weight)) { // 输入结束
        puts("  已取消输入。"); // 提示取消
        return; // 结束案例
    } // 体重检查结束
    height = ui_read_double("请输入身高（0.8-2.5 m）：", 0.8, 2.5); // 读取身高并校验范围
    if (isnan(height)) { // 输入结束
        puts("  已取消输入。"); // 提示取消
        return; // 结束案例
    } // 身高检查结束
    value = calculate_bmi(weight, height); // 调用纯函数计算 BMI
    label = bmi_category(value); // 调用分类函数取得文本
    printf("\n  BMI = %.2f，分类：%s\n", value, label); // 输出结果
    puts("  设计优点：计算函数只负责算，分类函数只负责判断，主流程只负责输入输出。"); // 总结单一职责
    puts("  这种拆分让函数可以独立测试：例如 calculate_bmi(70, 1.75) 应得到 22.86。"); // 说明可测试性
} // demo_bmi 函数结束

static void demo_recursion(void) { // 案例二：比较循环与递归，并观察递归终止条件
    int n = 0; // 阶乘参数
    int fib_n = 0; // 斐波那契参数
    int a = 0; // 最大公约数第一个参数
    int b = 0; // 最大公约数第二个参数
    int loop_result = 0; // 循环版阶乘结果
    int rec_result = 0; // 递归版阶乘结果
    int fib_result = 0; // 斐波那契结果
    int gcd_result = 0; // 最大公约数结果
    ui_example("递归三连：阶乘、斐波那契、最大公约数"); // 显示案例标题
    n = ui_read_int("请输入阶乘 n（0-12）：", 0, 12); // 限制 n，避免 32 位 int 溢出
    if (n < 0) { // 输入结束
        puts("  已取消输入。"); // 提示取消
        return; // 结束案例
    } // 阶乘参数检查结束
    loop_result = factorial_iterative(n); // 用循环计算
    rec_result = factorial_recursive(n); // 用递归计算
    printf("\n  %d! 循环结果 = %d，递归结果 = %d\n", n, loop_result, rec_result); // 对比两种实现
    fib_n = ui_read_int("请输入斐波那契项 n（0-20）：", 0, 20); // 读取斐波那契参数
    if (fib_n < 0) { // 输入结束
        return; // 结束案例
    } // 斐波那契参数检查结束
    fib_result = fibonacci_recursive(fib_n); // 递归计算斐波那契数
    printf("  fib(%d) = %d\n", fib_n, fib_result); // 输出结果
    a = ui_read_int("请输入第一个正整数（1-10000）：", 1, 10000); // 读取第一个数
    if (a < 1) { // 输入结束
        return; // 结束案例
    } // 第一个数检查结束
    b = ui_read_int("请输入第二个正整数（1-10000）：", 1, 10000); // 读取第二个数
    if (b < 1) { // 输入结束
        return; // 结束案例
    } // 第二个数检查结束
    gcd_result = gcd_recursive(a, b); // 递归求最大公约数
    printf("  gcd(%d, %d) = %d\n", a, b, gcd_result); // 输出最大公约数
    puts("  递归的两条铁律：必须有终止条件；每次调用必须更接近终止条件。"); // 总结递归规则
    puts("  递归版斐波那契会重复计算大量子问题，n 很大时应改用循环或记忆化。"); // 说明效率问题
} // demo_recursion 函数结束

static void demo_scope(void) { // 案例三：观察局部变量、块作用域与 static 局部变量
    int outer = 10; // 外层作用域变量
    ui_example("作用域实验：同名变量与 static 计数器"); // 显示案例标题
    printf("\n  第一次取学号：%d\n", next_student_id()); // 第一次调用，静态编号变为 1
    printf("  第二次取学号：%d\n", next_student_id()); // 第二次调用，静态编号变为 2
    printf("  第三次取学号：%d\n", next_student_id()); // 第三次调用，静态编号变为 3
    puts("  解释：next_student_id 里的 static int id 只初始化一次，值会跨调用保留。"); // 解释静态存储期
    { // 人为创建一个内层代码块
        int outer = 20; // 内层同名变量会遮蔽外层变量
        printf("  内层 outer = %d\n", outer); // 输出内层变量
    } // 内层代码块结束，内层变量生命周期结束
    printf("  外层 outer = %d\n", outer); // 输出外层变量，证明它没有被内层修改
    puts("  经验：少用全局变量和同名遮蔽；函数所需状态尽量通过参数和返回值传递。"); // 给出工程建议
} // demo_scope 函数结束

static void demo_hanoi(void) { // 案例四：汉诺塔，观察递归调用树
    int disks = 0; // 盘子数量
    int total_moves = 0; // 理论最少移动次数
    ui_example("汉诺塔：把大问题拆成同构的小问题"); // 显示案例标题
    disks = ui_read_int("请输入盘子数（1-5）：", 1, 5); // 限制规模，避免输出过多
    if (disks < 1) { // 输入结束
        puts("  已取消输入。"); // 提示取消
        return; // 结束案例
    } // 盘子数检查结束
    total_moves = (1 << disks) - 1; // n 个盘子的最少步数是 2 的 n 次方减 1
    printf("\n  移动过程（缩进表示递归深度）：\n"); // 输出提示
    hanoi(disks, 'A', 'C', 'B'); // 从 A 柱移动到 C 柱，B 柱作为辅助
    printf("\n  理论最少步数：%d，实际输出步数应与之相同。\n", total_moves); // 验证步数
    puts("  递归思路：先把 n-1 个盘子移开，移动最大的盘子，再把 n-1 个盘子移回来。"); // 总结算法
} // demo_hanoi 函数结束

void lesson_04_functions(void) { // 第 4 章教学入口
    int correct = 0; // 记录小测答对题数
    const char *homework[] = { // 课后作业文本
        "把判断素数写成 is_prime(int n)，在主函数中输出 100 以内所有素数。", // 作业一
        "写递归函数 sum_digits(n) 求各位数字之和，例如 2025 返回 9。", // 作业二
        "给汉诺塔增加步数统计（用全局变量或指针），验证结果等于 2^n - 1。", // 作业三
        "把第 3 章计算器重构为 add/sub/mul/div 四个函数，并保留除零检查。", // 作业四
        "制造一个缺少终止条件的递归，观察栈溢出，再写成正确的递归。" // 作业五
    }; // 作业数组结束
    ui_heading("第 4 章  函数、作用域与递归"); // 显示章节标题
    ui_section("本章目标"); // 显示目标小节
    ui_note("掌握函数声明、定义、调用、参数和返回值。"); // 列出目标一
    ui_note("理解值传递、局部变量、块作用域和 static 生命周期。"); // 列出目标二
    ui_note("能用递归解决阶乘、斐波那契、最大公约数和汉诺塔。"); // 列出目标三
    ui_note("学会把复杂流程拆成职责单一、可独立测试的函数。"); // 列出目标四
    ui_section("知识讲解"); // 开始知识讲解
    puts("  1. 为什么需要函数"); // 讲解第一点标题
    puts("     复用逻辑、隔离细节、降低主流程复杂度，并让每个部分可以单独测试。"); // 说明函数价值
    puts("     一个函数只做一件事；输入通过参数，结果通过返回值，副作用越少越好。"); // 给出设计原则
    puts("  2. 声明与定义"); // 讲解第二点标题
    puts("     声明（原型）告诉编译器“函数长什么样”，定义才提供函数体。"); // 区分声明和定义
    puts("     多文件项目中，声明通常放头文件，定义放 .c 文件，编译后再链接。"); // 联系工程组织
    puts("  3. 作用域与生命周期"); // 讲解第三点标题
    puts("     局部变量只在自己所在代码块内可见；函数返回后自动变量通常失效。"); // 说明局部作用域
    puts("     static 局部变量只初始化一次，生命周期贯穿整个程序，但作用域仍限于函数。"); // 说明静态局部变量
    puts("  4. 递归"); // 讲解第四点标题
    puts("     递归 = 终止条件 + 把问题缩小后调用自身；每次调用都会消耗栈空间。"); // 概括递归结构
    puts("     没有终止条件或规模不缩小，会导致栈溢出；深度过大时优先改循环。"); // 强调风险
    puts("     递归适合树、分治和回溯；简单线性重复用循环通常更直接。"); // 说明适用场景
    ui_pause(); // 等待阅读
    ui_section("运行案例"); // 开始案例
    demo_bmi(); // 运行 BMI 函数拆分案例
    ui_pause(); // 暂停阅读
    demo_recursion(); // 运行递归案例
    ui_pause(); // 暂停阅读
    demo_scope(); // 运行作用域案例
    ui_pause(); // 暂停阅读
    demo_hanoi(); // 运行汉诺塔案例
    ui_section("本章小测"); // 开始小测
    if (ui_quiz("函数声明（原型）的主要作用是什么？", "分配内存", "让编译器在调用前知道参数和返回类型", "自动执行函数", "删除函数体", 'B', "原型让编译器检查调用是否合法；链接阶段才会找到函数定义。")) { // 第一题
        correct++; // 答对计数加一
    } // 第一题结束
    if (ui_quiz("递归函数最重要的两个组成部分是？", "循环和数组", "终止条件和向终止条件靠近的递归调用", "全局变量和宏", "指针和文件", 'B', "缺少终止条件会无限递归；规模不缩小同样无法到达终止条件。")) { // 第二题
        correct++; // 答对计数加一
    } // 第二题结束
    if (ui_quiz("函数内的 static 局部变量有什么特点？", "每次调用重新初始化", "只初始化一次，值跨调用保留", "其他函数可以直接访问", "必须手动 free", 'B', "static 局部变量具有静态存储期，但名字作用域仍只在函数内部。")) { // 第三题
        correct++; // 答对计数加一
    } // 第三题结束
    ui_homework(homework, (int)(sizeof(homework) / sizeof(homework[0]))); // 显示作业
    ui_complete(4, correct, 3); // 显示完成信息和小测成绩
} // lesson_04_functions 函数结束

// ============================================================================
// 第 4 章课后作业（每个函数先写原型、再写定义、最后写主函数测试）
// 1. 素数函数：实现 int is_prime(int n)，再用循环输出 2-100 的所有素数。
//    要求：n < 2 返回 0；只试除到平方根；每行输出 10 个。
// 2. 递归数位和：sum_digits(n) 返回各位数字之和，2025 应返回 9。
//    进阶：再写 reverse_number(n) 递归反转整数，例如 1200 -> 21。
// 3. 汉诺塔统计：增加步数统计，验证 n=1..5 时步数分别为 1、3、7、15、31。
//    提示：可以增加一个全局计数器，或传入指针；比较两种做法的优缺点。
// 4. 重构计算器：add/sub/mul/div 四个函数，主函数负责输入、选择和除零检查。
//    验收：每个函数职责单一；主函数不超过 60 行；保留连续计算功能。
// 5. 递归错误诊所：写一个缺少终止条件的递归并运行，记录崩溃现象。
//    解释“调用栈”“栈溢出”和“段错误”之间的关系，再修复并复测。
//
// 提交标准：
// - 至少一个程序包含 3 个以上函数，并说明每个函数的前置条件。
// - 所有递归函数都写出终止条件和规模缩小方式。
// - 能解释值传递为什么无法直接修改调用者的普通变量（指针在第 7 章揭晓）。
// ============================================================================
