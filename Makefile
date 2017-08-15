all: lambda.cpp
	clang++ -o lambda lambda.cpp

clean:
	rm lambda
