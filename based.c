#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

// converts an integer in any base to a base 10 integer
long long to_base10(char *num, int base) {
	if (abs(base) < 2 || abs(base) > 62) {
		printf("error: base must be between 2 and 62 (or -2 and -62)\n");
		return 0;
	}
	
	const char *digits = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"; // look at me im so smart
	int num_digits = strlen(num);
	long long answer = 0; // final answer goes here
	long long temp = 1;
	int index;
	int neg = 0; // boolean, if num has - sign
	
	if (strcmp(num, "0") == 0) { // 0 is 0 in every base
		return answer;
	}
	
	// for signed numbers, delete sign (add negative back later)
	if (num[0] == '+' || num[0] == '-') {
		if (num[0] == '-') neg = 1;
		if (num[0] == '-' && base < 0) return 0; // not how negative bases work
		memmove(num, num + 1, num_digits);
	}
	
	// loop through each digit of num in reverse and convert to base 10
	for (int i = num_digits - 1; i >= 0; i--) {
		// find index of current digit (represents numerical value of that digit)
		index = strchr(digits, num[i]) - digits;
		if (index >= abs(base)) {
			printf("error: detected digit not compatible with entered base\n");
			return 0;
		}
		answer += index * temp;
		temp *= base;
	}
	
	if (neg) answer = -answer; // add back negative sign if needed
	return answer;
}

// converts a base 10 integer to a different base
char *from_base10(long long num, int base) {
	if (abs(base) < 2 || abs(base) > 62) {
		printf("error: base must be between 2 and 62 (or -2 and -62)\n");
		return NULL;
	}
	
	const char *digits = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"; // look at me im so smart
	static char result[999]; // final result goes here
	char temp[999]; // temporarily stores digits backwards
	
	if (num == 0) { // 0 is 0 in every base
		strcpy(result, "0");
		return result;
	}
	
	// for positive bases and negative numbers, convert positive number and make it negative later
	if (num < 0 && base > 0) {
		num = -num;
	}
	
	long long quotient = num; // quotient starts as original number
	int i = 0; // tracks number of digits stored in temp
	
	while (quotient != 0) {
		long long remainder = quotient % base; // remainder = next digit
		quotient /= base; // get next quotient
		
		// negative base case
		if (remainder < 0) {
			remainder -= base;
			quotient++;
		}
		
		// store digit in temp (in reverse order for now)
		temp[i] = digits[remainder];
		i++;
	}
	
	int j = 0;
	
	// add minus sign for negative answers
	if (num < 0 && base > 0) {
		result[j] = '-';
		j++;
	}
	
	// reverse temp to get final answer
	while (i > 0) {
		i--;
		result[j] = temp[i];
		j++;
	}
	
	result[j] = '\0'; // add null terminator to say string is done
	
	return result;
}

// converts an integer from any base to any other base
char *convert_base(char *num, int any_base, int any_other_base) {
	long long base10num = to_base10(num, any_base);
	char *ans = from_base10(base10num, any_other_base);
	return ans;
}

// checks if a string is an integer (returns 0 if false, returns 1 if true and only contains 0-9, returns 2 if true and contains letters
int is_int(char str[]) {
	int i = 0;
	int a = 1;
	
	// skip first char if + or - in case user specified sign
	if (str[i] == '+' || str[i] == '-') i++;
	if (str[i] == '\0') return 0;
	
	// check if every character is a digit
	for (; str[i] != '\0'; i++) {
		if (!isalnum(str[i])) return 0;
		if (!isdigit(str[i])) a = 2;
	}
	return a;
}

// prints a base 10 number in base 2, 8, and 16
int bin_oct_hex(long long n) {
	int to_return = 1;
	
        char *binary = from_base10(n, 2);
        (binary != NULL) ? printf("binary: 0b%s\n", binary) : (to_return = 0);
        char *octal = from_base10(n, 8);
        (octal != NULL) ? printf("octal: 0o%s\n", octal) : (to_return = 0);
        char *hexa = from_base10(n, 16);
        (hexa != NULL) ? printf("hexa: 0x%s\n", hexa) : (to_return = 0);
	
	return to_return;
}

int main() {
	int y = 0;
	char pre_input;
	char input[999];
	
	printf("enter y to convert a base 10 number to bases 2, 8, and 16, or enter anything else to convert a number from any base to any other base: ");
	scanf(" %c", &pre_input);
	
	// trash any other characters user entered because c is stupid
	int c;
	while ((c = getchar()) != '\n' && c != EOF);
	
	if (tolower(pre_input) == 'y')  y = 1;
	(y == 1) ? printf("enter an integer (in base 10): ") : printf("enter an integer: ");
        scanf("%[^\n]", input);
	
	// verify user input is an integer
        if (is_int(input) == 0) {
                if (strcmp(input, "an integer dumbass") == 0) {
                        printf("very funny.\n");
                        return 0;
                }
                printf("thats not an integer dumbass\n");
                return 0;
        }
	if (is_int(input) == 2 && y == 1) {
		printf("i said in base 10. try again\n");
		return 0;
	}
	
        if (y == 1) {
		long long user_num = strtoll(input, NULL, 10); // from base 10 needs ll number
		// if number was successfully converted into all 3 bases return 1, otherwise return 0
		return (bin_oct_hex(user_num)) ? 1 : 0;
	}
	
	// take two more user inputs, base number is in and base to convert it to
	// (no error handling for now cuz im lazy)
	printf("enter base to convert from: ");
	int from;
	scanf("%d", &from);
	printf("enter base to convert to: ");
	int to;
	scanf("%d", &to);
	
	printf("your number in base %d converted to base %d equals: %s\n", from, to, convert_base(input, from, to));
	
	return 1;
}
