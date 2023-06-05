#include <stdio.h>
#include <string.h>
//marcos henriques - marcostiagofh@gmail.com
#define MAX_LINE_LENGTH 1000001
//i had to use y-crunch to take digits from each .ycd file and split into 10 files of 10 billions of digits each. so, this program here processes each of the 10 pieces at a time

//method to check if given number is prime
static int is_prime(unsigned long long n){
	int i, last=n/2;
	for(i=2;i<=last;i++)
		if (n%i==0)
			return 0;
	return 1;
}

static inline int is_odd(int x) { return x & 1; }

static unsigned long long number_from_string(char* string, int first_pos, int last_pos){
	int i, ten_power=1;
        unsigned long long result=0;
	for(i=last_pos;i>=first_pos;i--){
		result += string[i]*ten_power;
		ten_power *= 10;
	}
	return result;
}

int main(int argc, char *argv[]) {
    FILE    *textfile;
    char    line[MAX_LINE_LENGTH+20];
	char    temp[40] = "00000000000000000000";
	int     str_size;
    
    textfile = fopen(argv[1], "r");
    if(textfile == NULL)
        return 1;
    int actual_position = 1;
	//because we read the file in chunks, i always combine the last 20 digits of previous chunk with the first 20 digits of the actual chunk, so we cover this part as well, so as to look for pallindromes. because c int has limitations (64bit size int), if 21-digit number start with 3 or above, i had to use auxilliary tools to check if given number is prime
    while(fgets(line, MAX_LINE_LENGTH, textfile)){
	    int prime = 0, pallindrome = 0,	i, j,k;
		str_size = strlen(line);
		//printf("%d\n",str_size);
		
		for(i=0;i<20;i++)
			temp[20+i] = line[i];
		
		for(i=0;i<40;i++){
			if (is_odd(temp[i]) && temp[i] != 5 && temp[i] == temp[i+20]){
				//we build a 21-digit number
				j=0;
				//10-iteration loop, j=0:9, comparing (j)-th digit with (20-j)-ith digit
				while (j < 10){
					if (temp[i+j] == temp[i+20-j]){
						pallindrome = 1;
					//if any comparison gives a difference between the two digits, its not pallindrome
					} else {
						pallindrome = 0;
						break;
					}
					j += 1;
				}
				//if its pallindrome, check if it is prime
				if (pallindrome == 1 && is_prime(number_from_string(line,i,i+20))){
					prime = 1;
					for(k=i;k<=i+20;k++)
						printf("%d",temp[k]-'0');
					printf("\n");
				}
			}
		}
		
		prime = 0, pallindrome = 0,	i=0;
		
		//printf(line[0:200])
		//to find first 21-digit pallindrome and prime number
		//verify actual digit
		
		while (line[i+20]!='\0'){
			//if 0,2,4,6,8, actual pallindrome is even, and in case of 0, its a 20-digit number
			//print(i)
			if (is_odd(line[i]) && line[i] != 5 && line[i] == line[i+20]){
				//we build a 21-digit number
				j=0;
				//10-iteration loop, j=0:9, comparing (j)-th digit with (20-j)-ith digit
				while (j < 10){
					if (line[i+j] == line[i+20-j]){
						pallindrome = 1;
					//if any comparison gives a difference between the two digits, its not pallindrome
					} else {
						pallindrome = 0;
						break;
					}
					j += 1;
				}
				//if its pallindrome, check if it is prime
				if (pallindrome == 1 && is_prime(number_from_string(line,i,i+20))){
					prime = 1;
					for(k=i;k<=i+20;k++)
						printf("%d",line[k]-'0');
					printf("\n%d\n",actual_position + i - 1);
				}
			}
			i += 1;
		}

		actual_position += str_size;
		
		for(i=0;i<20;i++)
			temp[i] = line[str_size-1-i];
    }
     
    fclose(textfile);
    return 0;
}

//result is 151978145606541879151
//near digit position of 150672630233