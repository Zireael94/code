#include "tutor.h" // 引入教程公共接口和标准库

typedef struct IntNode { // 定义单链表节点；struct 的完整规则在第 9 章展开
    int value; // 节点保存的整数值
    struct IntNode *next; // 指向下一个同类型节点，形成链式结构
} IntNode; // IntNode 是 struct IntNode 的简写

static IntNode *node_create(int value); // 函数声明：在堆上创建一个节点
static bool list_push_back(IntNode **head, int value); // 函数声明：在链表尾部追加节点
static void list_print(const IntNode *head); // 函数声明：遍历并打印链表
static int list_sum(const IntNode *head); // 函数声明：计算链表元素和
static bool list_remove(IntNode **head, int value); // 函数声明：删除第一个匹配节点
static void list_free(IntNode **head); // 函数声明：释放整条链表并置空头指针

static IntNode *node_create(int value) { // 从堆上申请一个节点
    IntNode *node = malloc(sizeof(*node)); // sizeof(*node) 自动使用正确类型大小
    if (node == NULL) { // malloc 失败时返回 NULL
        return NULL; // 把失败向上传递，调用者必须处理
    } // 分配检查结束
    node->value = value; // 初始化节点保存的值
    node->next = NULL; // 新节点的下一个指针先置空
    return node; // 返回新节点地址
} // node_create 函数结束

static bool list_push_back(IntNode **head, int value) { // 用二级指针修改头指针
    IntNode *new_node = NULL; // 保存新节点
    IntNode *cursor = NULL; // 遍历指针
    new_node = node_create(value); // 创建节点
    if (new_node == NULL) { // 创建失败
        return false; // 返回失败状态
    } // 创建检查结束
    if (*head == NULL) { // 空链表时新节点就是头节点
        *head = new_node; // 直接修改调用者的头指针
        return true; // 插入成功
    } // 空链表分支结束
    cursor = *head; // 从第一个节点开始寻找尾部
    while (cursor->next != NULL) { // 只要后面还有节点
        cursor = cursor->next; // 移动到下一个节点
    } // 找尾循环结束
    cursor->next = new_node; // 把新节点接到原来的尾节点之后
    return true; // 插入成功
} // list_push_back 函数结束

static void list_print(const IntNode *head) { // 只读遍历，不修改链表
    const IntNode *cursor = head; // 使用 const 指针表达只读意图
    if (cursor == NULL) { // 空链表
        puts("  [空链表]"); // 输出空链表标识
        return; // 直接结束
    } // 空链表检查结束
    printf("  head -> "); // 输出链表起点
    while (cursor != NULL) { // 依次访问每个节点
        printf("%d -> ", cursor->value); // 输出当前节点值
        cursor = cursor->next; // 移动到下一个节点
    } // 遍历结束
    puts("NULL"); // 输出链表结束标记
} // list_print 函数结束

static int list_sum(const IntNode *head) { // 累加所有节点值
    int sum = 0; // 累加器
    const IntNode *cursor = head; // 只读遍历指针
    while (cursor != NULL) { // 直到链表末尾
        sum += cursor->value; // 把当前值加入总和
        cursor = cursor->next; // 移动到下一个节点
    } // 遍历结束
    return sum; // 返回总和
} // list_sum 函数结束

static bool list_remove(IntNode **head, int value) { // 删除第一个值匹配的节点
    IntNode **link = head; // link 指向“保存当前节点地址的指针”
    IntNode *victim = NULL; // 被删除的节点
    while (*link != NULL) { // 只要当前节点存在
        if ((*link)->value == value) { // 当前节点命中目标值
            victim = *link; // 保存待释放节点
            *link = victim->next; // 让前驱或头指针跳过该节点
            free(victim); // 归还节点占用的堆内存
            return true; // 删除成功
        } // 命中判断结束
        link = &(*link)->next; // 把“当前节点的 next 字段”作为下一轮链接入口
    } // 查找循环结束
    return false; // 没有找到目标值
} // list_remove 函数结束

static void list_free(IntNode **head) { // 释放整条链表，避免内存泄漏
    IntNode *cursor = NULL; // 当前节点
    IntNode *next = NULL; // 下一个节点
    if (head == NULL) { // 防御性检查
        return; // 没有头指针可处理
    } // 头指针检查结束
    cursor = *head; // 从第一个节点开始
    while (cursor != NULL) { // 逐个释放
        next = cursor->next; // 先保存下一个节点，否则 free 后无法访问
        free(cursor); // 释放当前节点
        cursor = next; // 移动到下一个节点
    } // 释放循环结束
    *head = NULL; // 把头指针置空，防止悬空指针
} // list_free 函数结束

static void demo_dynamic_array(void) { // 案例一：用 malloc/calloc/realloc 管理动态数组
    int count = 0; // 初始元素个数
    int i = 0; // 循环下标
    int sum = 0; // 元素总和
    int *values = NULL; // 指向动态数组
    int *grown = NULL; // 保存 realloc 的临时返回值
    ui_example("动态数组：按需申请、扩容与释放"); // 显示案例标题
    count = ui_read_int("请输入初始元素个数（1-20）：", 1, 20); // 由用户决定数组大小
    if (count < 1) { // 输入结束
        puts("  已取消输入。"); // 提示取消
        return; // 结束案例
    } // 个数检查结束
    values = calloc((size_t)count, sizeof(*values)); // calloc 申请并自动清零
    if (values == NULL) { // 必须检查分配是否成功
        puts("  内存分配失败，程序安全退出。"); // 输出错误
        return; // 结束案例
    } // 分配检查结束
    printf("\n  calloc 清零验证：values[0] = %d", values[0]); // 验证第一个元素默认值为 0
    if (count > 1) { // 只有至少两个元素时才读取 values[1]
        printf("，values[1] = %d", values[1]); // 验证第二个元素也被清零
    } // 第二元素检查结束
    putchar('\n'); // 结束验证行并换行
    for (i = 0; i < count; i++) { // 填入平方数
        values[i] = (i + 1) * (i + 1); // 当前元素等于序号平方
        sum += values[i]; // 累加元素
    } // 填充循环结束
    printf("  动态数组内容："); // 输出提示
    for (i = 0; i < count; i++) { // 遍历动态数组
        printf("%d ", values[i]); // 输出元素
    } // 遍历结束
    putchar('\n'); // 换行
    printf("  元素和：%d，平均值：%.2f\n", sum, (double)sum / count); // 输出统计
    grown = realloc(values, (size_t)(count + 1) * sizeof(*values)); // 申请多一个元素的空间
    if (grown == NULL) { // realloc 失败时原内存仍然有效
        puts("  扩容失败，保留原数组并释放。"); // 提示失败
        free(values); // 释放原数组
        return; // 结束案例
    } // 扩容检查结束
    values = grown; // 只有成功后才覆盖原指针
    values[count] = 1000; // 写入新增元素
    count++; // 更新元素个数
    printf("  扩容后追加 1000："); // 输出提示
    for (i = 0; i < count; i++) { // 再次遍历
        printf("%d ", values[i]); // 输出包含新元素的数组
    } // 遍历结束
    putchar('\n'); // 换行
    free(values); // 使用完毕，释放整块堆内存
    values = NULL; // 置空，防止以后误用悬空指针
    puts("  安全流程：申请 -> 检查 NULL -> 使用 -> free -> 指针置 NULL。"); // 总结动态内存流程
    puts("  realloc 可能搬移内存，必须用临时指针接收结果，不能直接覆盖原指针。"); // 强调 realloc 用法
} // demo_dynamic_array 函数结束

static void demo_linked_list(void) { // 案例二：单链表的插入、遍历、删除与释放
    IntNode *head = NULL; // 空链表用空指针表示
    int extra = 0; // 用户要追加的值
    int removed = 0; // 用户要删除的值
    bool ok = true; // 记录插入是否全部成功
    ui_example("链表实战：节点不在连续内存里也能串起来"); // 显示案例标题
    ok = list_push_back(&head, 10) && list_push_back(&head, 20) && list_push_back(&head, 30); // 连续追加三个节点
    if (!ok) { // 任一节点分配失败
        puts("  创建链表失败，正在清理已分配节点。"); // 输出失败信息
        list_free(&head); // 释放可能已经创建的部分
        return; // 结束案例
    } // 创建检查结束
    puts("\n  初始链表："); // 输出提示
    list_print(head); // 打印链表
    extra = ui_read_int("请输入要追加的整数（0-999）：", 0, 999); // 读取追加值
    if (extra < 0) { // 输入结束
        list_free(&head); // 退出前仍要释放内存
        puts("  已取消输入，链表已释放。"); // 提示清理完成
        return; // 结束案例
    } // 追加输入检查结束
    if (!list_push_back(&head, extra)) { // 尝试追加
        puts("  追加失败，可能是内存不足。"); // 提示失败
    } // 追加结果检查结束
    puts("  追加后："); // 输出提示
    list_print(head); // 再次打印
    removed = ui_read_int("请输入要删除的整数（0-999）：", 0, 999); // 读取删除目标
    if (removed >= 0) { // 输入有效时执行删除
        if (list_remove(&head, removed)) { // 删除成功
            printf("  已删除第一个值为 %d 的节点。\n", removed); // 输出成功信息
        } else { // 没有找到目标
            printf("  链表中没有 %d，未删除任何节点。\n", removed); // 输出未找到
        } // 删除结果分支结束
    } // 删除输入处理结束
    puts("  删除后："); // 输出提示
    list_print(head); // 打印最终链表
    printf("  链表元素和：%d\n", list_sum(head)); // 计算并输出和
    list_free(&head); // 释放全部节点
    puts("  已释放全部节点，head 现在是 NULL。"); // 确认生命周期结束
    puts("  链表优点：插入删除不必整体搬移；代价：不能 O(1) 随机访问，且有指针开销。"); // 总结取舍
} // demo_linked_list 函数结束

void lesson_08_memory(void) { // 第 8 章教学入口
    int correct = 0; // 记录小测答对题数
    const char *homework[] = { // 课后作业文本
        "写动态数组版成绩统计：先输入 n，再录入成绩，最后 free 并置 NULL。", // 作业一
        "扩展链表：支持头插、尾插、查找、删除、反转和按升序插入，全部释放。", // 作业二
        "实现按需增长的成绩数组：容量满时用 realloc 翻倍，统计扩容次数。", // 作业三
        "实现动态二维矩阵：用连续内存申请 rows×cols，输出地址并正确释放。", // 作业四
        "对比 malloc、calloc、realloc 的行为，并写一份内存错误排查清单。" // 作业五
    }; // 作业数组结束
    ui_heading("第 8 章  动态内存与链表"); // 显示章节标题
    ui_section("本章目标"); // 显示目标小节
    ui_note("理解栈、堆和静态存储区的生命周期差异。"); // 列出目标一
    ui_note("掌握 malloc、calloc、realloc、free 的正确用法。"); // 列出目标二
    ui_note("能用动态数组和单链表管理运行期才知道大小的数据。"); // 列出目标三
    ui_note("识别泄漏、悬空、双重释放、越界和分配失败。"); // 列出目标四
    ui_section("知识讲解"); // 开始知识讲解
    puts("  1. 三种常见存储期"); // 讲解第一点标题
    puts("     自动变量通常在栈上，函数返回自动失效；全局/static 变量贯穿程序运行。"); // 说明栈和静态区
    puts("     堆由程序员手动申请和释放，生命周期从 malloc 成功到 free 为止。"); // 说明堆
    puts("  2. 四个内存函数"); // 讲解第二点标题
    puts("     malloc(size) 申请未初始化的字节块；calloc(n, size) 申请并清零。"); // 说明 malloc 和 calloc
    puts("     realloc(p, new_size) 调整已分配块，可能搬移；free(p) 归还内存。"); // 说明 realloc 和 free
    puts("     malloc/calloc/realloc 都可能返回 NULL；使用前必须检查。"); // 强调失败检查
    puts("  3. 链表"); // 讲解第三点标题
    puts("     每个节点包含数据和 next 指针；head 指向首节点，末尾节点的 next 为 NULL。"); // 说明链表结构
    puts("     插入只需改指针；删除要先保存 next 再 free，避免释放后继续访问。"); // 说明插入删除
    puts("     修改头指针时传 IntNode **；遍历只读时传 const IntNode *。"); // 说明参数设计
    puts("  4. 内存错误"); // 讲解第四点标题
    puts("     泄漏：申请后忘记 free；悬空：free 后继续使用；双重释放：同一块内存 free 两次。"); // 列出错误
    puts("     越界写：分配 n 个元素却写入第 n+1 个；释放非堆地址同样是未定义行为。"); // 列出越界
    puts("     调试时优先开启 -fsanitize=address（若编译器支持），比肉眼排查可靠。"); // 建议工具
    ui_pause(); // 等待阅读
    ui_section("运行案例"); // 开始案例
    demo_dynamic_array(); // 运行动态数组案例
    ui_pause(); // 暂停阅读
    demo_linked_list(); // 运行链表案例
    ui_section("本章小测"); // 开始小测
    if (ui_quiz("malloc 申请失败时通常会返回什么？", "0 号地址", "NULL", "-1", "随机地址", 'B', "标准规定 malloc 失败返回空指针 NULL，调用者必须先检查再使用。")) { // 第一题
        correct++; // 答对计数加一
    } // 第一题结束
    if (ui_quiz("free(p) 之后立即再解引用 p，属于什么问题？", "内存泄漏", "use-after-free（释放后使用）", "栈溢出", "语法错误", 'B', "内存已经归还，继续访问是未定义行为；free 后应把指针置为 NULL。")) { // 第二题
        correct++; // 答对计数加一
    } // 第二题结束
    if (ui_quiz("realloc 返回 NULL 时，原来的内存块会怎样？", "自动被释放", "仍然有效，需要用原指针 free", "内容被清零", "地址一定不变", 'B', "realloc 失败时原块保持不变；若提前覆盖原指针，反而会造成泄漏。")) { // 第三题
        correct++; // 答对计数加一
    } // 第三题结束
    ui_homework(homework, (int)(sizeof(homework) / sizeof(homework[0]))); // 显示作业
    ui_complete(8, correct, 3); // 显示完成信息和小测成绩
} // lesson_08_memory 函数结束

// ============================================================================
// 第 8 章课后作业（每一条 malloc 都必须能在代码里找到对应的 free）
// 1. 动态成绩：先读 n，申请 n 个 double，录入成绩并统计平均/最高/最低，最后释放。
//    要求：n 非法、分配失败、输入中断时都不泄漏。
// 2. 链表全套操作：头插、尾插、查找、删除、逆序、升序插入、打印、释放。
//    验收：删除头节点/尾节点/唯一节点/不存在值四种情况都正确。
// 3. 自动扩容数组：初始容量 4，满时 realloc 翻倍；统计扩容次数和最终容量。
//    提示：记录 size 和 capacity 两个变量；写入前检查容量。
// 4. 动态矩阵：用一块连续内存保存 rows×cols 的 int，提供 get/set 函数。
//    提示：index = row * cols + col；只 malloc 一次、只 free 一次。
// 5. 内存诊断：分别制造泄漏、双重释放、释放后使用、越界写，记录现象。
//    如果编译器支持，用 -fsanitize=address 重新运行并抄下第一条错误信息。
//
// 提交标准：
// - 每条执行路径（成功、失败、提前返回）都要释放资源。
// - 容器接口要说明“谁申请、谁释放、释放后谁置空”。
// - 不把“运行没崩”当作内存正确的证据。
// ============================================================================
