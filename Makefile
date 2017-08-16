all: lambda.cpp lexer.cpp lexer.h expression.cpp expression.h
	clang++ -o lambda lambda.cpp lexer.cpp expression.cpp

clean:
	rm lambda
