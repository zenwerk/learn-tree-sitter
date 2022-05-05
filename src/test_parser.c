#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <tree_sitter/api.h>

TSLanguage *tree_sitter_calc();

int main() {
  // パーサーを作成
  TSParser *parser = ts_parser_new();

  // パーサーに言語を設定
  ts_parser_set_language(parser, tree_sitter_calc());

  // 解析木を作成
  const char *source_code = "1 + 1";
  TSTree *tree = ts_parser_parse_string(
      parser,
      NULL,
      source_code,
      strlen(source_code)
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
  char *string = ts_node_string(root_node);
  printf("Syntax Tree: %s\n", string);

  free(string);
  ts_tree_delete(tree);
  ts_parser_delete(parser);

  return 0;
}