#include "tutor.h" // 引入教程公共接口和标准库

static void print_int_array(const int values[], int count); // 函数声明：统一打印一维整数数组
static double average_array(const int values[], int count); // 函数声明：计算整数数组平均值
static void bubble_sort(int values[], int count); // 函数声明：冒泡排序，原地修改数组
static int binary_search(const int values[], int count, int target); // 函数声明：在有序数组中二分查找
static int linear_search(const int values[], int count, int target); // 函数声明：线性查找，不要求有序

static void print_int_array(const int values[], int count) { // 按统一格式输出数组内容
    int i = 0; // 元素下标
    putchar('['); // 输出左方括号
    for (i = 0; i < count; i++) { // 遍历有效元素
        if (i > 0) { // 第一个元素前不输出逗号
            fputs(", ", stdout); // 输出元素之间的分隔符
        } // 分隔符判断结束
        printf("%d", values[i]); // 输出当前元素
    } // 遍历结束
    puts("]"); // 输出右方括号并换行
} // print_int_array 函数结束

static double average_array(const int values[], int count) { // 计算数组平均值，调用者保证 count > 0
    int sum = 0; // 累加器初始化为 0
    int i = 0; // 元素下标
    for (i = 0; i < count; i++) { // 遍历所有有效元素
        sum += values[i]; // 把当前元素累加进总和
    } // 遍历结束
    return (double)sum / count; // 强制转换为 double，避免整数除法截断
} // average_array 函数结束

static void bubble_sort(int values[], int count) { // 冒泡排序：每轮把当前最大值送到末尾
    int pass = 0; // 外层轮次
    int j = 0; // 内层比较下标
    int temp = 0; // 交换时使用的临时变量
    bool swapped = false; // 记录本轮是否发生过交换
    for (pass = 0; pass < count - 1; pass++) { // 最多需要 count-1 轮
        swapped = false; // 每轮开始时假设已经有序
        for (j = 0; j < count - 1 - pass; j++) { // 已排好的尾部不再比较
            if (values[j] > values[j + 1]) { // 前一个元素更大时应交换
                temp = values[j]; // 暂存前一个元素
                values[j] = values[j + 1]; // 后面的较小值移到前面
                values[j + 1] = temp; // 暂存的大值移到后面
                swapped = true; // 记录本轮发生了交换
            } // 比较判断结束
        } // 内层比较结束
        if (!swapped) { // 如果一轮都没有交换
            break; // 说明数组已经有序，可以提前结束
        } // 提前结束判断结束
    } // 外层轮次结束
} // bubble_sort 函数结束

static int binary_search(const int values[], int count, int target) { // 在升序数组中查找目标值
    int low = 0; // 查找范围左边界
    int high = count - 1; // 查找范围右边界
    int mid = 0; // 中间位置
    while (low <= high) { // 范围仍然有效时继续
        mid = low + (high - low) / 2; // 用减半加法避免大数相加溢出
        if (values[mid] == target) { // 找到目标
            return mid; // 返回目标所在下标
        } // 命中判断结束
        if (values[mid] < target) { // 中间值太小
            low = mid + 1; // 目标只可能在右半区
        } else { // 中间值太大
            high = mid - 1; // 目标只可能在左半区
        } // 区间收缩结束
    } // 查找循环结束
    return -1; // 没有找到时返回 -1
} // binary_search 函数结束

static int linear_search(const int values[], int count, int target) { // 从头逐个查找目标
    int i = 0; // 当前下标
    for (i = 0; i < count; i++) { // 依次检查每个元素
        if (values[i] == target) { // 找到相等元素
            return i; // 立即返回下标
        } // 命中判断结束
    } // 遍历结束
    return -1; // 查遍全数组仍未找到
} // linear_search 函数结束

static void demo_scores(void) { // 案例一：录入成绩并统计平均分、极值和及格率
    int scores[10] = {0}; // 最多保存 10 个成绩，剩余位置初始为 0
    int count = 0; // 实际录入人数
    int i = 0; // 循环下标
    int sum = 0; // 成绩总和
    int minimum = 0; // 最低分
    int maximum = 0; // 最高分
    int passed = 0; // 及格人数
    char prompt[64] = {0}; // 保存动态生成的输入提示
    ui_example("一维数组：成绩统计"); // 显示案例标题
    count = ui_read_int("请输入学生人数（1-10）：", 1, 10); // 限制人数不超过数组容量
    if (count < 1) { // 输入结束或非法
        puts("  已取消输入。"); // 提示取消
        return; // 结束案例
    } // 人数检查结束
    for (i = 0; i < count; i++) { // 逐个录入成绩
        snprintf(prompt, sizeof(prompt), "请输入第 %d 个成绩（0-100）：", i + 1); // 生成带序号提示
        scores[i] = ui_read_int(prompt, 0, 100); // 读取并校验当前成绩
        if (scores[i] < 0) { // 输入结束
            puts("  已取消输入。"); // 提示取消
            return; // 结束案例
        } // 成绩检查结束
        sum += scores[i]; // 累加总分
        if (scores[i] >= 60) { // 判断是否及格
            passed++; // 及格人数加一
        } // 及格判断结束
    } // 录入循环结束
    minimum = scores[0]; // 从第一个成绩开始找最低分
    maximum = scores[0]; // 从第一个成绩开始找最高分
    for (i = 1; i < count; i++) { // 从第二个元素开始比较
        if (scores[i] < minimum) { // 发现更小的值
            minimum = scores[i]; // 更新最低分
        } // 最低分判断结束
        if (scores[i] > maximum) { // 发现更大的值
            maximum = scores[i]; // 更新最高分
        } // 最高分判断结束
    } // 极值查找结束
    printf("\n  成绩列表："); // 输出提示
    print_int_array(scores, count); // 打印实际录入的元素
    printf("  平均分：%.2f\n", average_array(scores, count)); // 调用函数计算平均值
    printf("  最低分：%d，最高分：%d\n", minimum, maximum); // 输出极值
    printf("  及格人数：%d，及格率：%.1f%%\n", passed, 100.0 * passed / count); // 输出及格统计
    printf("  数组总容量：%lu 个 int，本次只用了 %d 个。\n", (unsigned long)(sizeof(scores) / sizeof(scores[0])), count); // 对比容量与有效长度
    puts("  关键：数组长度不会自动携带，遍历时必须使用单独的有效元素个数 count。"); // 强调长度管理
} // demo_scores 函数结束

static void demo_sort_search(void) { // 案例二：冒泡排序后二分查找
    int values[] = {78, 92, 66, 88, 95, 71, 83, 90}; // 初始化一个无序数组
    int count = (int)(sizeof(values) / sizeof(values[0])); // 用 sizeof 计算元素个数
    int target = 0; // 要查找的目标成绩
    int index = -1; // 查找结果下标
    int linear_index = -1; // 线性查找结果，用于对比
    ui_example("排序与查找：从无序到有序"); // 显示案例标题
    printf("\n  原始数组："); // 输出提示
    print_int_array(values, count); // 打印原始数据
    bubble_sort(values, count); // 调用冒泡排序
    printf("  排序之后："); // 输出提示
    print_int_array(values, count); // 打印有序结果
    target = ui_read_int("请输入要查找的成绩（0-100）：", 0, 100); // 读取目标值
    if (target < 0) { // 输入结束
        puts("  已取消输入。"); // 提示取消
        return; // 结束案例
    } // 目标检查结束
    linear_index = linear_search(values, count, target); // 线性查找，作为对照
    index = binary_search(values, count, target); // 二分查找，要求数组有序
    if (linear_index >= 0) { // 线性查找找到了
        printf("  线性查找：在下标 %d 找到 %d。\n", linear_index, target); // 输出线性查找结果
    } else { // 没有找到
        printf("  线性查找：没有 %d。\n", target); // 输出未找到
    } // 线性查找分支结束
    if (index >= 0) { // 二分查找找到了
        printf("  二分查找：在下标 %d 找到 %d。\n", index, target); // 输出二分查找结果
    } else { // 没有找到
        printf("  二分查找：没有 %d。\n", target); // 输出未找到
    } // 二分查找分支结束
    puts("  二分查找每轮把范围减半，时间复杂度 O(log n)，但前提是数据已经有序。"); // 总结复杂度
    puts("  冒泡排序平均 O(n²)，适合教学，不适合大规模数据；后续可学快速排序。"); // 说明排序效率
} // demo_sort_search 函数结束

static void demo_matrix(void) { // 案例三：二维数组与矩阵转置
    int matrix[2][3] = {{1, 2, 3}, {4, 5, 6}}; // 2 行 3 列的原始矩阵
    int transpose[3][2] = {{0, 0}, {0, 0}, {0, 0}}; // 转置后变为 3 行 2 列
    int row = 0; // 行下标
    int col = 0; // 列下标
    int row_sum = 0; // 当前行的元素和
    ui_example("二维数组：矩阵转置与行求和"); // 显示案例标题
    puts("\n  原始矩阵 2×3："); // 输出提示
    for (row = 0; row < 2; row++) { // 遍历每一行
        row_sum = 0; // 每行开始前清零行和
        for (col = 0; col < 3; col++) { // 遍历当前行的每一列
            printf("%4d", matrix[row][col]); // 输出元素并保证对齐
            row_sum += matrix[row][col]; // 累加当前行
        } // 列循环结束
        printf("   | 行和 = %d\n", row_sum); // 在行尾输出该行总和
    } // 行循环结束
    for (row = 0; row < 2; row++) { // 遍历原始行
        for (col = 0; col < 3; col++) { // 遍历原始列
            transpose[col][row] = matrix[row][col]; // 行列互换完成转置
        } // 内层循环结束
    } // 转置循环结束
    puts("\n  转置矩阵 3×2："); // 输出提示
    for (row = 0; row < 3; row++) { // 遍历转置后的行
        for (col = 0; col < 2; col++) { // 遍历转置后的列
            printf("%4d", transpose[row][col]); // 输出转置元素
        } // 列循环结束
        putchar('\n'); // 每行结束换行
    } // 行循环结束
    printf("\n  sizeof(matrix) = %lu 字节，包含 %lu 个 int。\n", (unsigned long)sizeof(matrix), (unsigned long)(sizeof(matrix) / sizeof(matrix[0][0]))); // 说明二维数组连续存储
    puts("  二维数组按行优先连续存放；matrix[row][col] 的本质是行偏移加列偏移。"); // 解释内存布局
} // demo_matrix 函数结束

// 注意：第 5 章案例函数结束后才是本章作业的完整说明，见文件底部注释。
void lesson_05_arrays(void) { // 第 5 章教学入口
    int correct = 0; // 记录小测答对题数
    const char *homework[] = { // 课后作业文本
        "录入 10 个成绩，去掉一个最高分和一个最低分后求平均分（跳水计分）。", // 作业一
        "实现选择排序和插入排序，统计并比较它们对同一数组的交换次数。", // 作业二
        "给线性查找和二分查找增加比较次数计数，用 1000 个有序数据对比效率。", // 作业三
        "实现两个 3×3 矩阵的乘法，并输出转置矩阵，验证结果是否正确。", // 作业四
        "生成 50 个 0-9 的随机数，用计数数组统计每个数字出现的次数。" // 作业五
    }; // 作业数组结束
    ui_heading("第 5 章  数组、排序与查找"); // 显示章节标题
    ui_section("本章目标"); // 显示目标小节
    ui_note("掌握一维数组和二维数组的声明、初始化、遍历与内存布局。"); // 列出目标一
    ui_note("理解下标从 0 开始、越界风险和“容量与有效长度”的区别。"); // 列出目标二
    ui_note("能手写冒泡排序和二分查找，并解释它们的时间复杂度。"); // 列出目标三
    ui_note("能用数组解决统计、排序、查找和矩阵类问题。"); // 列出目标四
    ui_section("知识讲解"); // 开始知识讲解
    puts("  1. 数组是连续的同类型元素集合"); // 讲解第一点标题
    puts("     int a[5] 表示 5 个 int；合法下标是 0、1、2、3、4，不存在 a[5]。"); // 说明下标范围
    puts("     越界访问在 C 中不保证报错，可能读到垃圾值或破坏其他数据。"); // 强调越界风险
    puts("     sizeof(a) 在数组所在作用域内可得到总字节数，除以元素大小就是容量。"); // 说明容量计算
    puts("  2. 长度必须单独管理"); // 讲解第二点标题
    puts("     数组作为函数参数时会退化为指向首元素的指针，函数内 sizeof 不再可靠。"); // 解释传参退化
    puts("     因此接口通常写成 (数组, 有效元素个数)，本章所有函数都遵守这一约定。"); // 说明接口设计
    puts("  3. 排序与查找"); // 讲解第三点标题
    puts("     冒泡排序相邻比较、逐步把极值推到边界，平均 O(n²)、空间 O(1)。"); // 概括冒泡排序
    puts("     二分查找要求数据有序，每次排除一半，时间复杂度 O(log n)。"); // 概括二分查找
    puts("     线性查找不要求有序，但最坏要检查全部元素，复杂度 O(n)。"); // 概括线性查找
    puts("  4. 二维数组"); // 讲解第四点标题
    puts("     int m[2][3] 是 2 行 3 列；m[i][j] 与一维连续内存存在固定映射。"); // 解释二维数组
    puts("     行优先存储意味着最后一维变化最快，遍历时通常把列循环放内层。"); // 说明遍历顺序
    ui_pause(); // 等待阅读
    ui_section("运行案例"); // 开始案例
    demo_scores(); // 运行成绩统计案例
    ui_pause(); // 暂停阅读
    demo_sort_search(); // 运行排序查找案例
    ui_pause(); // 暂停阅读
    demo_matrix(); // 运行矩阵案例
    ui_section("本章小测"); // 开始小测
    if (ui_quiz("声明 int a[5]; 后，最后一个合法下标是多少？", "5", "4", "6", "0", 'B', "数组下标从 0 开始，5 个元素的下标是 0 到 4；a[5] 已越界。")) { // 第一题
        correct++; // 答对计数加一
    } // 第一题结束
    if (ui_quiz("二分查找能够正确工作的前提是什么？", "数组元素互不相同", "数组已经按目标顺序排好序", "数组长度是偶数", "数组存放在堆上", 'B', "二分查找依赖有序性来排除一半区间；无序数据必须先排序。")) { // 第二题
        correct++; // 答对计数加一
    } // 第二题结束
    if (ui_quiz("数组作为函数参数传递时，函数内部通常还需要什么？", "数组总容量", "有效元素个数", "元素类型", "返回值地址", 'B', "数组会退化为首元素指针，函数无法自动知道有效长度，必须额外传入 count。")) { // 第三题
        correct++; // 答对计数加一
    } // 第三题结束
    ui_homework(homework, (int)(sizeof(homework) / sizeof(homework[0]))); // 显示作业
    ui_complete(5, correct, 3); // 显示完成信息和小测成绩
} // lesson_05_arrays 函数结束

// ============================================================================
// 第 5 章课后作业（先用纸笔写一遍下标变化，再写代码）
// 1. 跳水计分：录入 10 个 0-10 的成绩，去掉一个最高分和一个最低分，求平均。
//    要求：处理最高分/最低分重复出现的情况；输出被去掉的两个值。
// 2. 排序对比：实现选择排序和插入排序，对同一组随机数据统计交换次数与比较次数。
//    进阶：用 clock() 粗略测量 1000、5000 个元素时的耗时，记录趋势。
// 3. 查找对比：实现带计数器的线性查找和二分查找，目标包括存在、重复、不存在三种。
//    输出比较次数，验证 1000 个有序元素时二分查找最多约 10 次比较。
// 4. 矩阵乘法：输入两个 3×3 整数矩阵，计算 A×B，并输出 A 的转置。
//    提示：结果 c[i][j] = 对 k 求和 a[i][k] * b[k][j]。
// 5. 频次统计：用 rand() 生成 50 个 0-9 的整数，用 int freq[10] 统计每个数字出现次数。
//    验收：所有频次之和必须等于 50；再输出出现次数最多的数字。
//
// 提交标准：
// - 所有数组访问都要能说明下标为什么不会越界。
// - 排序、查找函数都必须接收有效长度，禁止写死 10。
// - 至少用 1 组边界数据测试空结果或单元素数组。
// ============================================================================
