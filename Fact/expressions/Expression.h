/*****************************************************
*
* author:yiy
*
* description: 表达式类

*
******************************************************/
#pragma once

#include "../reflections/Class.h"
#include "../types/TList.h"


#ifndef __EXPRESSION_INCLUDED__ 
#define __EXPRESSION_INCLUDED__
#ifdef __cplusplus 
extern "C" {
#endif
	typedef enum {
		ExpressionKind_Lamda,
		ExpressionKind_Constant,
		ExpressionKind_Parameter,
		ExpressionKind_Block,
		ExpressionKind_Call,
		ExpressionKind_Return,
		ExpressionKind_Member,
		ExpressionKind_Property,
		ExpressionKind_Field,
		ExpressionKind_PropertyOrField,
		ExpressionKind_IndexGet,
		ExpressionKind_IndexSet,
		ExpressionKind_Assign,
		ExpressionKind_IfThenElse,
		ExpressionKind_For,
		ExpressionKind_Add,
		ExpressionKind_Sub,
		ExpressionKind_Mul,
		ExpressionKind_Div,
		ExpressionKind_Mod,
		ExpressionKind_EQ,
		ExpressionKind_GT,
		ExpressionKind_LT,
		ExpressionKind_GTE,
		ExpressionKind_LTE
	} ExpressionKinds;

	typedef struct stExpression {
		ExpressionKinds kind;
	}Expression;

	typedef struct stConstantExpression {
		struct stExpression;
		Type* type;
		void* value;
	}ConstantExpression;

	typedef struct stBinaryExpression {
		struct stExpression;
		Expression* left;
		Expression* right;
	}BinaryExpression;

	typedef struct stParameterExpression {
		struct stExpression;
		Type* type;
		String* name;
	}ParameterExpression;

	typedef struct stCallExpression {
		struct stExpression;
		Function* func;
		Expression* instance;
		List* parameters;
	}CallExpression;

	typedef struct stReturnExpression {
		struct stExpression;
		Expression* value;
	}ReturnExpression;

	typedef struct stIfElseThenExpression {
		struct stExpression;
		Expression* condition;
		Expression* trueBlock;
		Expression* falseBlock;
	}IfElseThenExpression;

	typedef struct stNewExpression {
		struct stExpression;
		Function* ctor;
		List* parameters;
	}NewExpression;


#ifdef __cplusplus 
}//end extern c
#endif
#endif //endif __EXPRESSION_INCLUDED__



