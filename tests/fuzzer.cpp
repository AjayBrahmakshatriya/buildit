#include "blocks/stmt.h"
#include "blocks/expr.h"
#include "blocks/c_code_generator.h"

using namespace block;

static int gen_random(int range) {
	return random() % range;
}

struct sctx {
	std::vector<var::Ptr> var_set;
	int max_depth = 3;	
	int n_decls = 5;
};

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
	return ve;
}

static expr::Ptr generate_assignment(const sctx &ctx) {
	expr::Ptr lhs = sample_variable(ctx);
	expr::Ptr rhs = sample_variable(ctx);

	plus_expr::Ptr rhs_add = std::make_shared<plus_expr>();
	rhs_add->expr1 = rhs;
	int_const::Ptr rhs_const = std::make_shared<int_const>();
	rhs_const->value = gen_random(100);
	rhs_add->expr2 = rhs_const;

	assign_expr::Ptr ae = std::make_shared<assign_expr>();
	ae->var1 = lhs;
	ae->expr1 = rhs_add;

	return ae;
}

static binary_expr::Ptr gen_comparison_operator(void) {
	switch(gen_random(6)) {
		case 0: return std::make_shared<equals_expr>();
		case 1: return std::make_shared<ne_expr>();
		case 2: return std::make_shared<lt_expr>();
		case 3: return std::make_shared<lte_expr>();
		case 4: return std::make_shared<gt_expr>();
		case 5: return std::make_shared<gte_expr>();	
	}	
	return nullptr;
}

static expr::Ptr generate_comparison(const sctx& ctx) {
	expr::Ptr lhs = sample_variable(ctx);
	expr::Ptr rhs = sample_variable(ctx);
	
	binary_expr::Ptr be = gen_comparison_operator();
	be->expr1 = lhs;
	be->expr2 = rhs;
	return be;	
}


static if_stmt::Ptr generate_if(const sctx& ctx, int depth, bool inloop);
static while_stmt::Ptr generate_while(const sctx& ctx, int depth, bool inloop);

static continue_stmt::Ptr generate_continue(void) {
	return std::make_shared<continue_stmt>();
}
static break_stmt::Ptr generate_break(void) {
	return std::make_shared<break_stmt>();
}

static expr_stmt::Ptr expr_from_stmt(expr::Ptr e) {
	expr_stmt::Ptr es = std::make_shared<expr_stmt>();
	es->expr1 = e;
	return es;
}

static stmt_block::Ptr generate_body(const sctx& ctx, int depth, bool inloop) {
	unsigned int num_stmts = gen_random(3) + 1;
	stmt_block::Ptr bs = std::make_shared<stmt_block>();

	while (bs->stmts.size() < num_stmts) {
		switch (gen_random(5)) {
			case 0: bs->stmts.push_back(expr_from_stmt(generate_assignment(ctx))); break;
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
	is->then_stmt = generate_body(ctx, depth + 1, inloop);
	if (gen_random(2)) {
		is->else_stmt = generate_body(ctx, depth + 1, inloop);
	} else {
		is->else_stmt = std::make_shared<stmt_block>();
	}
	return is;
}

static while_stmt::Ptr generate_while(const sctx& ctx, int depth, bool inloop) {
	while_stmt::Ptr ws = std::make_shared<while_stmt>();
	ws->cond = generate_comparison(ctx);
	ws->body = generate_body(ctx, depth + 1, true);

	return ws;
}


static void gen_decls(sctx& ctx, func_decl::Ptr fd) {
	for (int i = 0; i < ctx.n_decls; i++) {
		var::Ptr v = std::make_shared<var>();
		v->var_name = "var" + std::to_string(i);


		scalar_type::Ptr st = std::make_shared<scalar_type>();
		st->scalar_type_id = scalar_type::INT_TYPE;

		builder_var_type::Ptr bvt = std::make_shared<builder_var_type>();
		bvt->builder_var_type_id = builder_var_type::DYN_VAR;
		bvt->closure_type = st;	

		v->var_type = bvt;
		ctx.var_set.push_back(v);
		
		decl_stmt::Ptr ds = std::make_shared<decl_stmt>();
		ds->decl_var = v;
		int_const::Ptr rhs_const = std::make_shared<int_const>();
		rhs_const->value = gen_random(100);
		ds->init_expr = rhs_const;

		to<stmt_block>(fd->body)->stmts.push_back(ds);
	}
}

static func_decl::Ptr generate_function(std::string name) {
	func_decl::Ptr fd = std::make_shared<func_decl>();
	fd->body = std::make_shared<stmt_block>();
	fd->func_name = name;
	scalar_type::Ptr st = std::make_shared<scalar_type>();
	st->scalar_type_id = scalar_type::VOID_TYPE;
	fd->return_type = st;

	sctx ctx;
	gen_decls(ctx, fd);

	stmt_block::Ptr sb = generate_body(ctx, 0, false);
	
	for (auto s: sb->stmts) {
		to<stmt_block>(fd->body)->stmts.push_back(s);	
	}

	return fd;
}


int main(int argc, char* argv[]) {
	if (argc > 1) 
		srandom(atoi(argv[1]));
	else 
		srandom(42);
	func_decl::Ptr fd = generate_function("test1");	
	c_code_generator::generate_code(fd, std::cout, 0);
	return 0;
}
