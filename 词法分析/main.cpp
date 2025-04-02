#include <iostream>
#include <string>
#include<fstream>
#include<iomanip>
#include "Lexer.h"
#include "Table.h"

//int main() {
//    Lexer lexer;
//    std::string input, line;
//
//    std::cout << "请输入源代码(以#结束): " << std::endl;
//    while (getline(std::cin, line)) {
//        // 如果这一行包含 #，仅读取到#为止，然后结束
//        size_t pos = line.find('#');
//        if (pos != std::string::npos) {
//            input += line.substr(0, pos);
//            break;
//        }
//        input += line + '\n';
//    }
//
//    // std::cout << "文本如下：" << std::endl << input;
//    std::string processedInput = lexer.preprocess(input);
//    // std::cout << "处理后的文本如下：" << std::endl << processedInput;
//    std::vector<Token> tokens = lexer.lexerAnalyse(processedInput);
//
//    printTable();
//    std::cout << "词法分析结果:" << std::endl;
//    for (Token token : tokens) {
//        if (token.getSyn() == 25) { // 标识符
//            std::cout << "syn: " << token.getSyn() << " Token: " << token.getToken() << std::endl;
//        }
//        else if (keywordTable.find(token.getToken()) != keywordTable.end()) { // 关键字
//            std::cout << "syn: " << token.getSyn() << " Keyword: " << token.getToken() << std::endl;
//        }
//        else if (symbolTable.find(token.getToken()) != symbolTable.end()) { // 符号
//            std::cout << "syn: " << token.getSyn() << " Symbol: " << token.getToken() << std::endl;
//        }
//        else if (token.getSyn() == 26) { // 数字
//            std::cout << "syn: " << token.getSyn() << " Digit: " << token.getSum() << std::endl;
//        }
//        else { // 未定义符号
//            // 空格不算未定义符号
//            if (!std::isspace(token.getToken()[0]))
//                std::cout << "syn: -1 ERROR: Undefined Symbol : [" << token.getToken() << "]" << std::endl;
//        }
//    }
//
//    return 0;
//}

int main(int argc, char* argv[])
{
    // 检查命令行参数，确保提供了文件名
    // argc为命令行参数总数，包括程序名本身，因此argc至少为1
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }

    // 使用ifstream文件输入类创造一个file文件
    // 打开文本文件
    std::ifstream file(argv[1]);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << argv[1] << std::endl;
        return 1;
    }

    // 使用std::istreambuf_iterator<char>(file)创建一个文件输入流迭代器，从文件流中逐字节读取字符
    // 读取文件内容
    std::string input((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    Lexer lexer;
    std::string processedInput = lexer.preprocess(input);
    std::vector<Token> tokens = lexer.lexerAnalyse(processedInput);

    // 输出词法分析结果
    printTable();

    if (processedInput.size() == 0)
    {
        std::cout << "" << std::endl;
        return 0;
    }

    for (Token token : tokens) 
    {
        if (token.getSyn() == 25) 
        { // 标识符
            std::cout << "syn: " << token.getSyn() << " Token: " << token.getToken() << std::endl;
        }
        else if (keywordTable.find(token.getToken()) != keywordTable.end()) 
        { // 关键字
            std::cout << "syn: " << token.getSyn() << " Keyword: " << token.getToken() << std::endl;
        }
        else if (symbolTable.find(token.getToken()) != symbolTable.end()) 
        { // 符号
            std::cout << "syn: " << token.getSyn() << " Symbol: " << token.getToken() << std::endl;
        }
        else if (token.getSyn() == 26) 
        { // 数字
            std::cout << "syn: " << token.getSyn() << " Digit: " << std::setprecision(2)<<token.getSum() << std::endl;
        }
        else 
        { // 未定义符号
            // 空格不算未定义符号
            if (!std::isspace(token.getToken()[0]))
                std::cout << "syn: -1 ERROR: Undefined Symbol : [" << token.getToken() << "]" << std::endl;
        }
    }


    return 0;
}
