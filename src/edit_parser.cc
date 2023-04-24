#include <iostream>
#include <string>
#include <tree_sitter/api.h>

#include "tree_sitter/parser.h"

extern "C" TSLanguage *tree_sitter_calc();


void print_node(const TSNode *node, int indent=0) {
  TSNode child;
  for (int i = 0; i < indent; i++) {
    if (i == (indent - 1))
      std::cout << "└─";
    else
      std::cout << "  ";
  }
  std::cout << ts_node_type(*node) << std::endl;

  const uint32_t child_count = ts_node_child_count(*node);
  for (int i = 0; i < child_count; i++) {
    child = ts_node_child(*node, i);
    if (ts_node_is_named(child))
      print_node(&child, ++indent);
  }
}


int main() {
  // パーサーの初期化
  TSParser *parser = ts_parser_new();
  ts_parser_set_language(parser, tree_sitter_calc());

  // オリジナルのソースコード
  // 0123456789 byte
  // "1 + 2 * 3"
  std::string const source = "1 + 2 * 3";
  TSTree *tree = ts_parser_parse_string(parser, nullptr, source.c_str(), source.size());
  TSNode root_node = ts_tree_root_node(tree);
  std::cout << source <<std::endl;
  print_node(&root_node);

  // ソースコードを編集する
  // 0123456789 byte
  // "1 * 2 + 3"
  std::string const edited_source = "1 * 2 + 3";
  TSInputEdit edit;
  edit.start_byte = 2; // 編集開始位置のbyte -> `*` の開始位置
  edit.old_end_byte = 3; // 編集された元のテキストデータの最後の位置のbyte
  edit.new_end_byte = 3; // 元のテキストを置き換えた新しいテキストの終了位置のbyte
  edit.start_point = TSPoint{0, 2}; // 編集開始の行と列
  edit.old_end_point = TSPoint{0, 3}; // 原文における編集末尾の行と列
  edit.new_end_point = TSPoint{0, 3}; // 新しいテキストに編集終了の行と列が表示される
  ts_tree_edit(tree, &edit);

  // 編集結果を反映
  TSTree *edited_tree = ts_parser_parse_string(parser, tree, edited_source.c_str(), edited_source.size());
  root_node = ts_tree_root_node(edited_tree);
  std::cout << edited_source <<std::endl;
  print_node(&root_node);

  ts_tree_delete(tree);
  ts_tree_delete(edited_tree);
  ts_parser_delete(parser);

  return 0;
}

/*
ORIGINAL
1 + 2 * 3
programs
└─add
  └─num
    └─mul
      └─num
        └─num

EDITED
1 * 2 + 3
programs
└─add
  └─mul
    └─num
      └─num
    └─num
*/

