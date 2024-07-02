#include "blocks/stmt.h"
#include "blocks/expr.h"


using namespace block;

static int gen_random(int range) {
	return random() % range;
}

struct sctx {
	std::vector<var::Ptr> var_set;
}

/* Possible statements 

1. simple assign stmt
2. if condition with or without else
3. while loop 
4. break stmt
5. continue stmt

*/

static expr::Ptr sample_variable(const sctx &ctx) {
	var_expr::Ptr ve = std::make_shared<var_expr>();	
	ve->var1 = ctx.var_set[gen_random(ctx.var_set.size())];
	return var_expr;
}

static expr::Ptr generate_assignment(const sctx &ctx) {
	expr::Ptr lhs = sample_variable(ctx);
	expr::Ptr rhs = sample_variable(ctx);

	plus_expr::Ptr rhs_add = std::make_shared<plus_expr>();
	rhs_add->expr1 = rhs;
	int_const::Ptr rhs_const = std::make_shared<int_const>();
	rhs_const->value = gen_random(100);
	rhs_add->expr2 = rhs_const;

	assign_expr::Ptr ae = std::make_sahred<assign_expr>();
	ae->expr1 = lhs;
	ae->expr2 = rhs_add;
	
	return ae;
}

static binary_expr::Ptr gen_comparison_operator(void) {
	switch(gen_random(6)) {
		case 0: return std::make_shared<equals_expr>();
		case 1: return std::make_shared<ne_expr>();
		case 2: return std::make_sahred<lt_expr>();
		case 3: return std::make_shared<lte_expr>();
		case 4: return std::make_shared<gt_expr>();
		case 5: return std::make_shared<gte_expr>();	
	}	
}

static expr::Ptr generate_comparison(const sctx& ctx) {
	expr::Ptr lhs = sample_variable(ctx);
	expr::ptr rhs = sample_variable(ctx);
	
	binary_expr::Ptr be = gen_comparison_operator();
	be->expr1 = lhs;
	be->expr2 = rhs;
	return be;	
}


static if_stmt::Ptr generate_if(const sctx& ctx, int depth, bool inloop);

static continue_stmt::Ptr generate_continue(void) {
	return std::make_shared<continue_stmt>();
}
static continue_stmt::Ptr generate_break(void) {
	return std::make_shared<break_stmt>();
}

static block_stmt::Ptr generate_body(const sctx& ctx, int depth, bool inloop) {
	unsigned int num_stmts = gen_random(4);
	block_stmt::Ptr bs = std::make_shared<block_stmt>();

	while (bs->stmts.size() < num_stmts) {
		switch (gen_random(5)) {
			case 0: bs->stmts.push_back(generate_assignment(ctx)); break;
			case 1: if (depth > ctx.max_depth) continue; bs->stmts.push_back(generate_if(ctx, depth, inloop)); break;
			case 2: if (depth > ctx.max_depth) continue; bs->stmts.push_back(generate_while(ctx, depth, true)); break;
			case 3: if (!inloop) continue; bs->stmts.push_back(generate_break()); return bs;
			case 4: if (!inloop) continue; bs->stmts.push_back(generate_continue()); return bs;
		}
	}
	return bs;
}

static if_stmt::Ptr generate_if(const sctx& ctx, int depth, bool inloop) {
	if_stmt::Ptr is = std::make_shared<if_stmt>();
	is->cond = generate_comparison(ctx);
	is->then_stmt = generate_body(ctx, depth + 1);
	if (gen_random(2)) {
		is->else_stmt = generate_body(ctx, depth + 1)	
	} else {
		is->else_stmt = std::make_shared<block_stmt>();
	}
	return is;
}

static while_stmt::Ptr generate_while(const sctx& ctx, int depth, bool inloop) {
	while_stmt::Ptr ws = std::make_shared<while_stmt>();
	ws->cond = generate_condition(ctx);
	ws->body = generate_body(ctx, depth + 1, true);

	return ws;
}



void func_decl::Ptr generate_function(std::string name) {
}
