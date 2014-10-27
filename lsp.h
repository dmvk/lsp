struct sexpr_t {
	enum {
		NUM,
		STR,
		CONS
	} type;
	union {
		int num;
		char *str;
		struct {
			struct sexpr_t *car;
			struct sexpr_t *cdr;
		};
	};
};

typedef struct sexpr_t Sexpr;

Sexpr *read_expr();
