# tree-sitter をインストール
.PHONY: install
install:
	npm i

# grammar.js から parser.c を生成
.PHONY: generate
generate:
	tree-sitter generate

.PHONY: parser-test
	tree-sitter test