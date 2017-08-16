all: lambda.cpp lexer.cpp lexer.h expression.cpp expression.h
	clang++ -std=c++11 -o lambda lambda.cpp lexer.cpp expression.cpp

clean:
	rm lambda
