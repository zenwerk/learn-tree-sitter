#include <cassert>
#include <cstring>
#include <iostream>
#include <stack>

#include "tree_sitter/api.h"

extern "C" TSLanguage *tree_sitter_calc();

std::stack<int> stk;

void print_ast(const char *input, TSNode& node)
{
  for (uint32_t i = 0; i < ts_node_child_count(node); i++) {
    TSNode child_node = ts_node_child(node, i);
    if (ts_node_child_count(child_node)) {
      std::cout << "Parent:" << ts_node_type(child_node) << std::endl;
      print_ast(input, child_node);
    }

    if (!ts_node_is_named(child_node)) continue;

    uint32_t sb = ts_node_start_byte(child_node);
    uint32_t eb = ts_node_end_byte(child_node);
    uint32_t len = eb - sb + 1;
    char tmp[len];
    strncpy(tmp, input + sb, eb-sb);
    tmp[len] = '\0';
    printf("\t%s:%zu[%d~%d] -> %s\n", ts_node_type(child_node), strlen(input), sb, eb, tmp);

    if (strcmp(ts_node_type(child_node), "num") == 0) {
      std::cout << "\t\tpush: " << tmp << std::endl;
      stk.push(atoi(tmp));
    }
    else if (strcmp(ts_node_type(child_node), "add") == 0) {
      int r = stk.top();
      stk.pop();
      int l = stk.top();
      stk.pop();
      stk.push(l+r);
      std::cout << "\t\tpush: " << r << " + " << l << std::endl;
    }
    else if (strcmp(ts_node_type(child_node), "mul") == 0) {
      int r = stk.top();
      stk.pop();
      int l = stk.top();
      stk.pop();
      stk.push(l*r);
      std::cout << "\t\tpush: " << r << " * " << l << std::endl;
    }
    else if (strcmp(ts_node_type(child_node), "sub") == 0) {
      int r = stk.top();
      stk.pop();
      int l = stk.top();
      stk.pop();
      stk.push(l-r);
      std::cout << "\t\tpush: " << r << " - " << l << std::endl;
    }
    else if (strcmp(ts_node_type(child_node), "div") == 0) {
      int r = stk.top();
      stk.pop();
      int l = stk.top();
      stk.pop();
      std::cout << "\t\tpush: " << r << " / " << l << std::endl;
      stk.push(l/r);
    }
    else if (strcmp(ts_node_type(child_node), "uminus") == 0) {
      int top = stk.top();
      stk.pop();
      std::cout << "\t\tpush: " << "-1 * " << top << std::endl;
      stk.push(-1 * top);
    }
    else if (strcmp(ts_node_type(child_node), "ERROR") == 0) {
      std::cout << "TODO: parse error recover" << std::endl;
    }
    else {
      std::cout << "unknown node type: " << ts_node_type(child_node) << std::endl;
    }
  }

  std::cout << "\t\t\tcurrent top: " << stk.top() << std::endl;
}

void print_ast_stk(const char *input, TSNode node)
{
  std::stack<TSNode> stk, machine;
  stk.push(node);

  while (!stk.empty()) {
    char tmp[strlen(input)];
    auto nd = stk.top();
    stk.pop();

    if (ts_node_is_null(nd)) {
      std::cout << "TSNode is Null" << std::endl;
      continue;
    }
    std::cout << "current TSNode:" << ts_node_string(nd) << std::endl;

    auto child_count = ts_node_child_count(nd);
    if (child_count) {
      for (uint32_t i = 0; i < child_count; i++)
        stk.push(ts_node_child(nd, i));
    } else {
      if (ts_node_is_named(nd)) {
        uint32_t sb = ts_node_start_byte(nd);
        uint32_t eb = ts_node_end_byte(nd);
        strncpy(tmp, input + sb, eb-sb);
        printf("%s[\"%s\"]\n", ts_node_type(nd), tmp);
      }
    }

  }
}


int main() {
  // パーサーを作成
  TSParser *parser = ts_parser_new();

  // パーサーに言語を設定
  ts_parser_set_language(parser, tree_sitter_calc());

  std::string input;
  while (std::cout << "> " << std::flush, std::getline(std::cin, input)) {
    // 解析木を作成
    TSTree *tree = ts_parser_parse_string(
        parser,
        nullptr,
        input.c_str(),
        input.size()
    );
    // 解析木の根を取得する
    TSNode root_node = ts_tree_root_node(tree);
    // 解析木をS式で出力
    auto string = std::string(ts_node_string(root_node));
    std::cout << string << std::endl;
    std::cout << "-----------------" << std::endl;

    print_ast(input.c_str(), root_node);
    std::cout << "Answer: " << stk.top() << std::endl;
    stk.pop();

    // std::cout << "-----------------" << std::endl;
    // print_nd_stk(input.c_str(), root_node);
  }

  return 0;
}