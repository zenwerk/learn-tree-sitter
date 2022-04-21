module.exports = grammar({
    name: 'calc',

    rules: {
        // TODO:
        programs: $ => repeat1(
            seq($._expr, optional('\n'))
        ),
        _expr: $ => choice(
            $.add,
            $.sub,
            $.mul,
            $.div,
            $.uminus,
            seq('(', $._expr, ')'),
            $.num,
        ),
        add: $ => prec.left(1, seq($._expr, '+', $._expr)),
        sub: $ => prec.left(1, seq($._expr, '-', $._expr)),
        mul: $ => prec.left(2, seq($._expr, '*', $._expr)),
        div: $ => prec.left(2, seq($._expr, '/', $._expr)),
        uminus: $ => prec.right(3, seq('-', $._expr)),

        num: $ => /\d+/,
    }
});
