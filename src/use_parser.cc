#include <cassert>
#include <iostream>
#include <tree_sitter/api.h>

extern "C" TSLanguage *tree_sitter_calc();

int main() {
  // パーサーを作成
  TSParser *parser = ts_parser_new();

  // パーサーに言語を設定
  ts_parser_set_language(parser, tree_sitter_calc());

  // 解析木を作成
  const std::string source_code = "3 + 4";

  TSTree *tree = ts_parser_parse_string(
      parser,
      nullptr,
      source_code.c_str(),
      source_code.size()
  );

  // 解析木の根を取得する
  TSNode root_node = ts_tree_root_node(tree);

  // 子ノードを取得
  TSNode add_node = ts_node_named_child(root_node, 0);
  TSNode left_operand = ts_node_named_child(add_node, 0);
  TSNode right_operand = ts_node_named_child(add_node, 1);

  // ノードの種類を確認
  assert(ts_node_child_count(root_node) == 1);

  // 解析木をS式で出力
  auto string = std::string(ts_node_string(root_node));
  std::cout << string << std::endl;

  return 0;
}