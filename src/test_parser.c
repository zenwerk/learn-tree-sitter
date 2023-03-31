#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

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

  // ノードの種類を表示
  printf("left_operand:  %s\n", ts_node_type(left_operand));
  printf("right_operand: %s\n", ts_node_type(right_operand));

  // 対応文字の出現する行と列
  TSPoint sp = ts_node_start_point(left_operand);
  TSPoint ep = ts_node_end_point(left_operand);
  printf("left_operand startpoint row:%d column:%d\n", sp.row, sp.column);
  printf("left_operand endpoint   row:%d column:%d\n", ep.row, ep.column);

  // 対応する文字の開始・終了位置をbyte数で取得
  uint32_t lsb = ts_node_start_byte(left_operand);
  uint32_t leb = ts_node_end_byte(left_operand);
  printf("lhs start/end byte (%d, %d)\n", lsb, leb);
  uint32_t rsb = ts_node_start_byte(right_operand);
  uint32_t reb = ts_node_end_byte(right_operand);
  printf("rhs start/end byte (%d, %d)\n", rsb, reb);

  // 文字を取得して数値にして実際に計算してみる
  char tmp[strlen(source_code)];
  strncpy(tmp, source_code + lsb, leb);
  printf("lhs -> %s\n", tmp);
  int lhs = atoi(tmp);
  strncpy(tmp, source_code + rsb, reb);
  printf("rhs -> %s\n", tmp);
  int rhs = atoi(tmp);
  printf("answer = %d", lhs + rhs);

  free(string);
  ts_tree_delete(tree);
  ts_parser_delete(parser);

  return 0;
}