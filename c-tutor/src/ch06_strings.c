#include "tutor.h" // 引入教程公共接口和标准库

static bool is_palindrome(const char *text); // 函数声明：判断规范化后的字符串是否回文
static void caesar_shift(char *text, int shift); // 函数声明：原地对字母做凯撒位移

static bool is_palindrome(const char *text) { // 使用双指针从两端向中间比较
    size_t left = 0; // 左指针下标
    size_t right = 0; // 右指针下标
    if (text == NULL) { // 防御性检查空指针
        return false; // 空指针不能当成回文
    } // 空指针检查结束
    right = strlen(text); // 先取得字符串长度
    if (right == 0) { // 空字符串没有可比较字符
        return false; // 约定空串不算有效回文
    } // 空串检查结束
    right--; // 把右指针移动到最后一个字符
    while (left < right) { // 两指针尚未相遇时继续
        if (text[left] != text[right]) { // 出现不对称字符
            return false; // 立即判断不是回文
        } // 不对称判断结束
        left++; // 左指针右移
        right--; // 右指针左移
    } // 比较循环结束
    return true; // 所有对应位置都相同
} // is_palindrome 函数结束

static void caesar_shift(char *text, int shift) { // 只移动英文字母，保留大小写与其他字符
    size_t i = 0; // 字符下标
    int normalized = 0; // 规范化到 0-25 的位移量
    unsigned char current = 0; // 保存当前字符的无符号值
    if (text == NULL) { // 防御性检查
        return; // 空指针直接返回
    } // 空指针检查结束
    normalized = shift % 26; // 先缩小位移范围
    if (normalized < 0) { // 处理负位移
        normalized += 26; // 转成等价的正位移
    } // 负位移处理结束
    for (i = 0; text[i] != '\0'; i++) { // 逐个处理直到字符串结束符
        current = (unsigned char)text[i]; // 转成无符号值，避免 ctype 函数收到负数
        if (current >= 'a' && current <= 'z') { // 处理小写字母
            text[i] = (char)('a' + (current - 'a' + normalized) % 26); // 在小写字母表内循环移动
        } else if (current >= 'A' && current <= 'Z') { // 处理大写字母
            text[i] = (char)('A' + (current - 'A' + normalized) % 26); // 在大写字母表内循环移动
        } // 字母判断结束
    } // 遍历结束
} // caesar_shift 函数结束

static void demo_basic_strings(void) { // 案例一：字符串长度、遍历与常用库函数
    char greeting[] = "Hello, C"; // 字符数组初始化，末尾会自动加入 '\0'
    char line[128] = {0}; // 保存格式化后的完整句子
    char *position = NULL; // 保存 strchr 找到的字符位置
    char *substring = NULL; // 保存 strstr 找到的子串位置
    size_t i = 0; // 遍历下标
    ui_example("字符串基础：长度、遍历与比较"); // 显示案例标题
    printf("\n  sizeof(greeting) = %lu，strlen(greeting) = %lu\n", (unsigned long)sizeof(greeting), (unsigned long)strlen(greeting)); // 对比容量和实际长度
    puts("  sizeof 包含结尾的 '\\0'，strlen 只数到 '\\0' 之前。"); // 解释差异
    printf("  逐字符查看："); // 输出提示
    for (i = 0; greeting[i] != '\0'; i++) { // 手动遍历字符串
        printf("[%lu]='%c' ", (unsigned long)i, greeting[i]); // 输出下标和字符
    } // 遍历结束
    printf("\n  结尾隐藏字符：'\\0'（编码 0），它告诉函数字符串到此结束。\n"); // 说明结束符
    snprintf(line, sizeof(line), "%s，欢迎学习字符串！", greeting); // 用有界格式化安全地拼接文本
    printf("  拼接结果：%s\n", line); // 输出拼接后的文本
    printf("  strcmp(\"apple\", \"banana\") = %d\n", strcmp("apple", "banana")); // 比较字符串字典序，apple 更小
    printf("  strcmp(greeting, \"Hello, C\") = %d（0 表示内容相等）\n", strcmp(greeting, "Hello, C")); // 用 strcmp 比较字符串内容
    puts("  记住：greeting == line 只比较地址，判断文本是否相等必须用 strcmp。"); // 提醒不要用 == 比较内容
    position = strchr(greeting, 'C'); // 查找字符 C 第一次出现的位置
    if (position != NULL) { // 找到字符
        printf("  strchr 找到 C，距离开头 %ld 个字符。\n", (long)(position - greeting)); // 用指针相减得到下标
    } // 字符查找结束
    substring = strstr(greeting, "C"); // 查找子串 C 的位置
    if (substring != NULL) { // 找到子串
        printf("  strstr 找到的子串：%s\n", substring); // 输出从子串开始的剩余内容
    } // 子串查找结束
    puts("  常见错误：忘记 '\\0'、用 == 比较字符串内容、向字符串常量写入。"); // 总结三个高频错误
} // demo_basic_strings 函数结束

static void demo_palindrome(void) { // 案例二：忽略大小写和标点判断回文
    char input[INPUT_BUFFER_SIZE] = {0}; // 保存用户输入的原始句子
    char clean[INPUT_BUFFER_SIZE] = {0}; // 保存只保留字母和数字的结果
    size_t read_index = 0; // 原始字符串下标
    size_t write_index = 0; // 规范化字符串写入下标
    unsigned char current = 0; // 当前字符
    ui_example("回文判断：双指针与字符规范化"); // 显示案例标题
    if (!ui_read_text("请输入一句话（可含空格和标点）：", input, sizeof(input))) { // 读取整行
        puts("  已取消输入。"); // 提示取消
        return; // 结束案例
    } // 输入结束
    for (read_index = 0; input[read_index] != '\0'; read_index++) { // 遍历原始输入
        current = (unsigned char)input[read_index]; // 取得无符号字符值
        if (isalnum(current)) { // 只保留字母和数字
            clean[write_index] = (char)tolower(current); // 统一转成小写，忽略大小写
            write_index++; // 写入位置后移
        } // 保留判断结束
    } // 规范化循环结束
    clean[write_index] = '\0'; // 手动补上字符串结束符
    printf("\n  规范化结果：%s\n", clean); // 输出处理后的字符串
    if (write_index == 0) { // 没有任何字母或数字
        puts("  输入中没有可比较的字母或数字，无法判断。"); // 给出解释
        return; // 结束案例
    } // 空结果检查结束
    if (is_palindrome(clean)) { // 调用双指针判断函数
        puts("  结论：是回文（正读和反读相同）。"); // 输出回文结论
    } else { // 不是回文
        puts("  结论：不是回文。"); // 输出非回文结论
    } // 结论分支结束
    puts("  示例：A man, a plan, a canal: Panama 可规范化成 amanaplanacanalpanama。"); // 给出经典例子
} // demo_palindrome 函数结束

static void demo_word_count(void) { // 案例三：统计单词、字母、数字并找最长单词
    char text[INPUT_BUFFER_SIZE] = {0}; // 保存输入的一段英文
    char current[64] = {0}; // 保存当前正在扫描的单词
    char longest[64] = {0}; // 保存目前最长的单词
    size_t i = 0; // 输入下标
    size_t current_len = 0; // 当前单词长度
    size_t longest_len = 0; // 最长单词长度
    int words = 0; // 单词数量
    int letters = 0; // 字母数量
    int digits = 0; // 数字数量
    bool in_word = false; // 当前是否处在单词内部
    unsigned char current_char = 0; // 当前处理的字符
    ui_example("字符串扫描：单词统计与最长单词"); // 显示案例标题
    if (!ui_read_text("请输入一句英文（数字会单独统计）：", text, sizeof(text))) { // 读取整行
        puts("  已取消输入。"); // 提示取消
        return; // 结束案例
    } // 输入结束
    for (i = 0; text[i] != '\0'; i++) { // 逐字符扫描
        current_char = (unsigned char)text[i]; // 转成无符号字符
        if (isalnum(current_char)) { // 字母或数字都算单词的一部分
            if (!in_word) { // 如果刚刚进入一个新单词
                in_word = true; // 标记进入单词
                words++; // 单词数加一
                current_len = 0; // 清空当前单词长度
                current[0] = '\0'; // 清空当前单词内容
            } // 新单词处理结束
            if (isalpha(current_char)) { // 是字母
                letters++; // 字母计数加一
            } else { // 否则是数字
                digits++; // 数字计数加一
            } // 字符分类结束
            if (current_len < sizeof(current) - 1) { // 防止保存单词时越界
                current[current_len] = (char)current_char; // 把字符加入当前单词
                current_len++; // 长度加一
                current[current_len] = '\0'; // 维护字符串结束符
            } // 当前单词保存结束
        } else { // 遇到空格或标点
            if (in_word) { // 如果刚结束一个单词
                in_word = false; // 离开单词状态
                if (current_len > longest_len) { // 当前单词比历史最长还长
                    longest_len = current_len; // 更新最长长度
                    snprintf(longest, sizeof(longest), "%s", current); // 安全复制当前单词
                } // 最长更新结束
            } // 单词结束处理结束
        } // 字符分类结束
    } // 扫描循环结束
    if (in_word && current_len > longest_len) { // 处理句末最后一个单词
        longest_len = current_len; // 更新最长长度
        snprintf(longest, sizeof(longest), "%s", current); // 复制最后一个单词
    } // 句末处理结束
    printf("\n  字符总数：%lu\n", (unsigned long)strlen(text)); // 输出字符总数
    printf("  单词数：%d，字母数：%d，数字数：%d\n", words, letters, digits); // 输出分类统计
    printf("  最长单词：%s（%lu 个字符）\n", longest_len > 0 ? longest : "（无）", (unsigned long)longest_len); // 输出最长单词及长度
    puts("  思路：用一个状态变量 in_word 标记“是否在单词内部”，边界变化时更新统计。"); // 总结扫描方法
} // demo_word_count 函数结束

static void demo_caesar(void) { // 案例四：凯撒密码加密与解密
    char message[INPUT_BUFFER_SIZE] = {0}; // 保存待加密文本
    char original[INPUT_BUFFER_SIZE] = {0}; // 保存原文，用于解密验证
    int shift = 0; // 位移量
    ui_example("凯撒密码：字符变换与原地修改字符串"); // 显示案例标题
    if (!ui_read_text("请输入一段英文文本：", message, sizeof(message))) { // 读取文本
        puts("  已取消输入。"); // 提示取消
        return; // 结束案例
    } // 文本读取结束
    snprintf(original, sizeof(original), "%s", message); // 备份原文
    shift = ui_read_int("请输入位移量（1-25）：", 1, 25); // 读取位移量
    if (shift < 1) { // 输入结束
        puts("  已取消输入。"); // 提示取消
        return; // 结束案例
    } // 位移量检查结束
    caesar_shift(message, shift); // 用正位移加密
    printf("\n  原文：%s\n", original); // 输出原文
    printf("  密文：%s\n", message); // 输出加密结果
    caesar_shift(message, 26 - shift); // 用补数位移解密
    printf("  解密：%s\n", message); // 输出解密结果
    if (strcmp(original, message) == 0) { // 比较解密结果与原文
        puts("  验证成功：解密后与原文完全一致。"); // 输出验证结论
    } else { // 理论上不会发生
        puts("  验证失败：请检查字符位移逻辑。"); // 输出异常提示
    } // 验证分支结束
    puts("  注意：这里原地修改的是 char 数组；如果指向字符串常量，写入属于未定义行为。"); // 强调可修改对象
} // demo_caesar 函数结束

void lesson_06_strings(void) { // 第 6 章教学入口
    int correct = 0; // 记录小测答对题数
    const char *homework[] = { // 课后作业文本
        "实现 my_strlen 和 my_strcpy，要求带目标缓冲区大小并进行边界检查。", // 作业一
        "读取一行英文，原地反转字符串，再按单词顺序反转一次，比较两种结果。", // 作业二
        "统计一段英文的元音、辅音、数字、空格和单词数量，输出最长单词。", // 作业三
        "扩展凯撒密码：支持任意位移和大小写，尝试不读取原文直接解密。", // 作业四
        "修复三个字符串 bug：缺少 '\\0'、用 == 比较内容、strcpy 目标缓冲区太小。" // 作业五
    }; // 作业数组结束
    ui_heading("第 6 章  字符串与字符处理"); // 显示章节标题
    ui_section("本章目标"); // 显示目标小节
    ui_note("理解 C 字符串是以 '\\0' 结尾的字符数组。"); // 列出目标一
    ui_note("熟练使用 strlen、strcmp、strcpy、strchr、strstr、snprintf。"); // 列出目标二
    ui_note("掌握 fgets、行尾清理、字符分类和边界检查。"); // 列出目标三
    ui_note("能实现回文、单词统计、凯撒密码等文本处理案例。"); // 列出目标四
    ui_section("知识讲解"); // 开始知识讲解
    puts("  1. C 字符串没有内置长度字段"); // 讲解第一点标题
    puts("     “字符串”本质是 char 数组，约定以编码为 0 的 '\\0' 作为结束标记。"); // 说明本质
    puts("     char c = 'A' 是单个字符；char s[] = \"A\" 含两个字节：'A' 和 '\\0'。"); // 区分字符与字符串
    puts("  2. 输入与容量"); // 讲解第二点标题
    puts("     fgets(buffer, size, stdin) 最多读 size-1 个字符，并自动补 '\\0'。"); // 说明安全输入
    puts("     读取后通常要删除行尾 '\\n'；如果输入超长，还要清空剩余输入。"); // 说明行尾和超长处理
    puts("     任何写入都必须保证目标数组有足够空间，C 不会替你检查越界。"); // 强调边界
    puts("  3. 常用字符串函数"); // 讲解第三点标题
    puts("     strlen 求长度；strcmp 比较内容；strcpy 复制；strcat 拼接；strchr 找字符。"); // 列出常用函数
    puts("     snprintf 是更安全的有界格式化函数，能限制最多写入的字符数。"); // 推荐有界函数
    puts("     这些函数依赖 '\\0'；如果字符串没有结束符，可能一路越界读取。"); // 强调结束符
    puts("  4. 字符处理"); // 讲解第四点标题
    puts("     <ctype.h> 提供 isalpha、isdigit、isalnum、isspace、tolower、toupper。"); // 说明字符分类
    puts("     传入 ctype 函数前把 char 转成 unsigned char，避免负值导致未定义行为。"); // 强调安全转换
    puts("     字符串常量不可修改；需要修改时应复制到 char 数组。"); // 强调可写性
    ui_pause(); // 等待阅读
    ui_section("运行案例"); // 开始案例
    demo_basic_strings(); // 运行字符串基础案例
    ui_pause(); // 暂停阅读
    demo_palindrome(); // 运行回文案例
    ui_pause(); // 暂停阅读
    demo_word_count(); // 运行单词统计案例
    ui_pause(); // 暂停阅读
    demo_caesar(); // 运行凯撒密码案例
    ui_section("本章小测"); // 开始小测
    if (ui_quiz("C 字符串的结束标记是什么？", "空格", "换行符 '\\n'", "空字符 '\\0'", "字符 '0'", 'C', "'\\0' 的编码是 0；字符 '0' 的编码是 48，两者完全不同。")) { // 第一题
        correct++; // 答对计数加一
    } // 第一题结束
    if (ui_quiz("比较两个字符串内容是否相等，应该使用？", "==", "strcmp", "strlen", "sizeof", 'B', "== 比较的是地址；strcmp 返回 0 表示两个字符串内容相等。")) { // 第二题
        correct++; // 答对计数加一
    } // 第二题结束
    if (ui_quiz("读取一行最多 99 个字符，下面哪种写法相对安全？", "gets(buf)", "scanf(\"%s\", buf)", "fgets(buf, 100, stdin)", "strcpy(buf, input)", 'C', "fgets 会限制读取长度；gets 已从标准中移除，scanf 的 %s 也可能越界。")) { // 第三题
        correct++; // 答对计数加一
    } // 第三题结束
    ui_homework(homework, (int)(sizeof(homework) / sizeof(homework[0]))); // 显示作业
    ui_complete(6, correct, 3); // 显示完成信息和小测成绩
} // lesson_06_strings 函数结束

// ============================================================================
// 第 6 章课后作业（所有写操作都要先问：目标缓冲区有多大？）
// 1. 手写字符串函数：实现 my_strlen、my_strcpy(dst, size, src)。
//    要求：src 未以 '\0' 结尾时也能安全停止；my_strcpy 返回实际复制长度。
// 2. 反转文本：读取一行英文，原地反转全部字符；再实现按单词反转但单词内部顺序不变。
//    示例：I love C -> C love I；进阶：处理多个连续空格。
// 3. 文本统计：统计元音、辅音、数字、空格、单词数和最长单词；忽略大小写。
//    验收：空行、纯标点、连续空格都不能导致崩溃或错误计数。
// 4. 凯撒密码：支持 1-25 位移，保留大小写；把密文保存到文件，再读回解密。
//    进阶：实现 ROT13；思考为什么凯撒密码很容易被频率分析破解。
// 5. 错误诊所：写三个有 bug 的程序（缺结束符、用 == 比较、目标缓冲区太小），
//    记录运行结果与编译器警告，再给出修复方案。特别说明哪些错误编译器无法发现。
//
// 提交标准：
// - 每次复制/拼接前都检查目标缓冲区大小，禁止使用 gets。
// - 至少测试空字符串、全大写、混合大小写和超长输入。
// - 能解释数组名、字符串常量和指针在“可写性”上的区别。
// ============================================================================
