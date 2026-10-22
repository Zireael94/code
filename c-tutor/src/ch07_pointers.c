#include "tutor.h" // 引入教程公共接口和标准库

static void swap_int(int *left, int *right); // 函数声明：通过指针交换两个整数
static int apply_binary(int (*operation)(int, int), int left, int right); // 函数声明：接收函数指针并调用
static int add_values(int left, int right); // 函数声明：加法函数，签名必须匹配函数指针
static int subtract_values(int left, int right); // 函数声明：减法函数
static int multiply_values(int left, int right); // 函数声明：乘法函数

static void swap_int(int *left, int *right) { // 指针参数让函数可以修改调用者的变量
    int temp = 0; // 临时变量保存左值
    if (left == NULL || right == NULL) { // 防御性检查空指针
        return; // 任一指针为空则不做任何操作
    } // 空指针检查结束
    temp = *left; // 解引用左指针，取出目标变量的值
    *left = *right; // 把右指针指向的值写入左目标
    *right = temp; // 把暂存的原左值写入右目标
} // swap_int 函数结束

static int add_values(int left, int right) { // 加法函数
    return left + right; // 返回两数之和
} // add_values 函数结束

static int subtract_values(int left, int right) { // 减法函数
    return left - right; // 返回两数之差
} // subtract_values 函数结束

static int multiply_values(int left, int right) { // 乘法函数
    return left * right; // 返回两数之积
} // multiply_values 函数结束

static int apply_binary(int (*operation)(int, int), int left, int right) { // 通过函数指针调用具体运算
    if (operation == NULL) { // 函数指针也必须检查有效性
        return 0; // 无操作时返回 0
    } // 空函数指针检查结束
    return operation(left, right); // 调用指针指向的函数
} // apply_binary 函数结束

static void demo_swap(void) { // 案例一：用指针交换两个变量的值
    int left = 0; // 第一个整数
    int right = 0; // 第二个整数
    char prompt[64] = {0}; // 动态拼接输入提示
    ui_example("指针入门：让函数真正修改变量"); // 显示案例标题
    snprintf(prompt, sizeof(prompt), "请输入第一个整数（0-999）："); // 准备提示语
    left = ui_read_int(prompt, 0, 999); // 读取第一个整数
    if (left < 0) { // 输入结束
        puts("  已取消输入。"); // 提示取消
        return; // 结束案例
    } // 第一个数检查结束
    right = ui_read_int("请输入第二个整数（0-999）：", 0, 999); // 读取第二个整数
    if (right < 0) { // 输入结束
        puts("  已取消输入。"); // 提示取消
        return; // 结束案例
    } // 第二个数检查结束
    printf("\n  交换前：left = %d，right = %d\n", left, right); // 输出交换前的值
    printf("  left 的地址：%p，right 的地址：%p\n", (void *)&left, (void *)&right); // 用 %p 输出变量地址
    swap_int(&left, &right); // 把两个变量的地址传给函数
    printf("  交换后：left = %d，right = %d\n", left, right); // 输出交换后的值
    puts("  关键：函数收到的是地址，*p 表示“p 指向的那个变量”，所以能改写原变量。"); // 解释按地址修改
    puts("  值传递只能复制一份数据；传地址相当于把“房间钥匙”交给函数。"); // 给出形象比喻
} // demo_swap 函数结束

static void demo_array_pointer(void) { // 案例二：数组名、指针运算与两种访问方式
    int numbers[6] = {10, 20, 30, 40, 50, 60}; // 初始化六个整数
    int *cursor = numbers; // 数组名在表达式中退化为首元素地址
    int *end = numbers + 6; // 指向最后一个元素之后的位置，常用于循环边界
    int sum = 0; // 保存遍历求和结果
    int index = 0; // 传统下标
    ui_example("指针遍历数组：a[i] 与 *(a+i)"); // 显示案例标题
    printf("\n  数组容量：%lu 字节，指针大小：%lu 字节。\n", (unsigned long)sizeof(numbers), (unsigned long)sizeof(cursor)); // 对比数组与指针大小
    printf("  numbers 地址：%p，numbers[0] 地址：%p\n", (void *)numbers, (void *)&numbers[0]); // 验证首元素地址
    printf("  下标访问："); // 输出提示
    for (index = 0; index < 6; index++) { // 用下标遍历
        printf("%d ", numbers[index]); // 输出下标访问结果
    } // 下标遍历结束
    putchar('\n'); // 换行
    printf("  指针访问："); // 输出提示
    for (cursor = numbers; cursor < end; cursor++) { // 用指针从首元素走到尾后
        printf("%d ", *cursor); // 解引用输出当前元素
        sum += *cursor; // 累加当前元素
    } // 指针遍历结束
    putchar('\n'); // 换行
    printf("  总和：%d，遍历结束后 cursor - numbers = %ld\n", sum, (long)(cursor - numbers)); // 输出总和与指针距离
    printf("  numbers[2] = %d，*(numbers + 2) = %d，*(2 + numbers) = %d\n", numbers[2], *(numbers + 2), *(2 + numbers)); // 三种等价写法
    puts("  指针加法按“元素类型大小”前进：int* 加 1 通常前进 4 字节，而不是 1 字节。"); // 强调步长
    puts("  注意：只有指向同一数组（或尾后位置）的指针相减才有标准定义。"); // 强调合法范围
} // demo_array_pointer 函数结束

static void demo_const_pointers(void) { // 案例三：const 在指针声明中的位置
    int first = 10; // 第一个可修改整数
    int second = 20; // 第二个可修改整数
    const int *read_only = &first; // 指向 const int 的指针：不能通过它改值，但可以换目标
    int *const fixed = &first; // const 指针：不能换目标，但可以通过它改值
    ui_example("const 与指针：到底谁不能改？"); // 显示案例标题
    read_only = &second; // 合法：指针本身可以指向另一个 const int
    printf("\n  只读指针现在指向 second，*read_only = %d\n", *read_only); // 读取它指向的值
    *fixed = 99; // 合法：通过常量指针修改它指向的变量
    printf("  常量指针仍可修改目标，first 现在为 %d\n", first); // 输出修改后的 first
    puts("  记忆法：const 在 * 左边，约束“指向的值”；const 在 * 右边，约束“指针自己”。"); // 给出语法记忆法
    puts("  const int *p       → 不能 *p = 新值；p 可以换目标。"); // 说明第一种
    puts("  int *const p       → 可以 *p = 新值；p 不能换目标。"); // 说明第二种
    puts("  const int *const p → 值和目标都不能通过 p 修改。"); // 说明第三种
    puts("  把函数参数写成 const int *arr 可以表达“我只读，不修改你的数组”。"); // 说明接口契约
} // demo_const_pointers 函数结束

static void demo_function_pointer(void) { // 案例四：函数指针实现表驱动调用
    const char *options[] = {"加法", "减法", "乘法"}; // 可选运算菜单
    int (*operations[3])(int, int) = {add_values, subtract_values, multiply_values}; // 函数指针数组
    int choice = -1; // 菜单选择
    int left = 0; // 左操作数
    int right = 0; // 右操作数
    int result = 0; // 调用结果
    ui_example("函数指针：把“要执行的动作”作为参数"); // 显示案例标题
    left = ui_read_int("请输入左操作数（-100 到 100）：", -100, 100); // 读取左操作数
    if (left < -100) { // 输入结束
        puts("  已取消输入。"); // 提示取消
        return; // 结束案例
    } // 左操作数检查结束
    right = ui_read_int("请输入右操作数（-100 到 100）：", -100, 100); // 读取右操作数
    if (right < -100) { // 输入结束
        puts("  已取消输入。"); // 提示取消
        return; // 结束案例
    } // 右操作数检查结束
    choice = ui_choose("请选择运算：", options, 3); // 显示菜单并取得下标
    if (choice < 0) { // 用户取消
        return; // 结束案例
    } // 选择检查结束
    result = apply_binary(operations[choice], left, right); // 把选中的函数地址传入
    printf("\n  %d 和 %d 的%s结果是：%d\n", left, right, options[choice], result); // 输出最终结果
    puts("  apply_binary 不需要知道具体是加法还是乘法，它只负责调用传入的函数。"); // 解释回调价值
    puts("  声明阅读法：int (*fp)(int, int) 表示 fp 是函数指针，函数返回 int，接收两个 int。"); // 解释声明语法
} // demo_function_pointer 函数结束

void lesson_07_pointers(void) { // 第 7 章教学入口
    int correct = 0; // 记录小测答对题数
    const char *homework[] = { // 课后作业文本
        "实现 swap_double 和 swap_int，分别交换两种类型的变量并打印交换前后地址。", // 作业一
        "写 find_min_max(const int *arr, int n, int *min, int *max)，通过指针返回极值。", // 作业二
        "用指针手写 strlen，再用双指针原地反转字符串，不申请新数组。", // 作业三
        "实现函数指针版计算器，并增加一个 apply_all 函数对数组逐项执行同一操作。", // 作业四
        "诊断四种指针错误：野指针、空指针解引用、返回局部变量地址、越界指针运算。" // 作业五
    }; // 作业数组结束
    ui_heading("第 7 章  指针、地址与内存模型"); // 显示章节标题
    ui_section("本章目标"); // 显示目标小节
    ui_note("理解地址、指针变量、取地址运算符 & 和解引用运算符 *。"); // 列出目标一
    ui_note("掌握指针与数组的关系、指针运算和尾后指针。"); // 列出目标二
    ui_note("能通过指针参数修改调用者数据，并读懂 const 修饰位置。"); // 列出目标三
    ui_note("初步掌握函数指针、回调与表驱动调用。"); // 列出目标四
    ui_section("知识讲解"); // 开始知识讲解
    puts("  1. 指针是什么"); // 讲解第一点标题
    puts("     指针是保存内存地址的变量；类型告诉编译器“从该地址读多少个字节、如何解释”。"); // 说明指针组成
    puts("     int value = 42; int *p = &value; 之后 *p 就等于 value。"); // 给出基本例子
    puts("     & 取地址，* 解引用；* 在声明中表示“这是指针”，在表达式中表示“访问目标”。"); // 区分两种星号
    puts("  2. 指针与数组"); // 讲解第二点标题
    puts("     数组名在大多数表达式中转换为首元素地址；a[i] 等价于 *(a + i)。"); // 说明数组退化
    puts("     指针加 1 前进一个元素，不是前进一个字节；相减得到元素个数差。"); // 说明步长
    puts("     函数参数写成数组形式也仍然是指针，必须额外传入长度。"); // 再次强调长度
    puts("  3. const 与指针"); // 讲解第三点标题
    puts("     const 放在 * 左侧约束目标值，放在 * 右侧约束指针自身，规则从右往左读。"); // 说明 const 位置
    puts("     函数只读数组时使用 const，既表达意图，也能让编译器帮助检查误写。"); // 说明工程价值
    puts("  4. 函数指针"); // 讲解第四点标题
    puts("     函数也有地址；函数指针可以保存函数地址，并通过指针调用函数。"); // 说明函数指针
    puts("     它常用于回调、菜单分发表和算法策略选择，是阅读大型 C 项目的必备知识。"); // 说明用途
    puts("  5. 高危错误"); // 讲解第五点标题
    puts("     未初始化指针（野指针）、解引用 NULL、返回局部变量地址、越界运算都会导致未定义行为。"); // 列出错误
    puts("     指针出问题时先问三件事：指向哪里？是否有效？生命周期还在吗？"); // 给出排查框架
    ui_pause(); // 等待阅读
    ui_section("运行案例"); // 开始案例
    demo_swap(); // 运行交换案例
    ui_pause(); // 暂停阅读
    demo_array_pointer(); // 运行数组指针案例
    ui_pause(); // 暂停阅读
    demo_const_pointers(); // 运行 const 指针案例
    ui_pause(); // 暂停阅读
    demo_function_pointer(); // 运行函数指针案例
    ui_section("本章小测"); // 开始小测
    if (ui_quiz("若 int a[5]; int *p = a;，表达式 *(p + 2) 等价于？", "a[1]", "a[2]", "a[3]", "a + 2", 'B', "p 指向 a[0]，加 2 后指向 a[2]，解引用得到该元素的值。")) { // 第一题
        correct++; // 答对计数加一
    } // 第一题结束
    if (ui_quiz("声明 const int *p; 表示什么？", "p 不能改指向", "不能通过 p 修改它指向的值", "p 必须为 NULL", "p 指向函数", 'B', "const 在 * 左侧，约束的是指向的 int；p 本身仍可指向别处。")) { // 第二题
        correct++; // 答对计数加一
    } // 第二题结束
    if (ui_quiz("int (*fp)(int, int) 中的 fp 是什么？", "返回指针的函数", "接收两个 int、返回 int 的函数指针", "int 数组", "指向 int 的常量指针", 'B', "括号让 * 先与 fp 结合：fp 是指针，指向参数为两个 int、返回 int 的函数。")) { // 第三题
        correct++; // 答对计数加一
    } // 第三题结束
    ui_homework(homework, (int)(sizeof(homework) / sizeof(homework[0]))); // 显示作业
    ui_complete(7, correct, 3); // 显示完成信息和小测成绩
} // lesson_07_pointers 函数结束

// ============================================================================
// 第 7 章课后作业（画内存图是理解指针最快的方法）
// 1. 交换函数：实现 swap_int、swap_double，主函数打印交换前后的值和地址。
//    进阶：实现 swap_string，交换两个 char* 指针本身，观察字符串内容是否改变。
// 2. 极值函数：find_min_max(const int *arr, int n, int *min, int *max)。
//    要求：检查空指针和 n <= 0；调用后通过 min/max 指针带回结果。
// 3. 手写字符串工具：my_strlen(const char *s) 和 reverse_in_place(char *s)。
//    要求：不申请第二个字符串数组；测试空串、单字符、偶数长度和奇数长度。
// 4. 函数指针计算器：支持 + - * /，用函数指针数组分发；实现 apply_all。
//    进阶：给 apply_all 增加 int (*)(int) 形式的一元函数，对每个元素变换。
// 5. 错误诊所：分别构造野指针、NULL 解引用、返回局部变量地址、数组越界指针运算。
//    记录崩溃或异常现象；如果环境支持，再用 AddressSanitizer 或调试器定位。
//
// 提交标准：
// - 每个指针变量在使用前都能说明它指向哪个有效对象。
// - 至少画一幅“变量—地址—值”的内存示意图。
// - 不把未定义行为解释成“我的机器上运行正常”。
// ============================================================================
