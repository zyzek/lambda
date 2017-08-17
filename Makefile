all: lambda.cpp lexer.cpp lexer.h expression.cpp expression.h parser.cpp parser.h
	clang++ -std=c++11 -o lambda lambda.cpp lexer.cpp expression.cpp parser.cpp

clean:
	rm lambda
