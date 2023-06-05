//gcc pallindrome_prime_3.c -o pallindrome_prime_3.exe
//file usage 
//pallindrome_prime_3.exe <pi_file-number_split-number.txt file-number split-number>
//e.g. pallindrome_prime_3.exe pi_0021_5.txt 21 5 >> result_0021
//the 10 splits for each ycd file were done by y-cruncher
//p.s.: i optimised this program's code many times, so it lost a bit of its legibility, specially in the part where we process the temp string which contains 100 last digits from previous chunk of the file as well as the first 100 digits from actual chunk
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>       // for time()
//marcos henriques - marcostiagofh@gmail.com
#define MAX_LINE_LENGTH 1000001
#define INFERIOR_LIMIT 23 //whenever i found a pallindrome prime number, i added 2 to this constant, so we look for prime pallindromes of the next size. this change also reduces the processing time
#define SUPERIOR_LIMIT 101
//since we discovered before the first 21-digit pallindrome prime, we now search for a even higher pallindrome prime number, and 22-length number cant be prime

//i had to use y-crunch to take digits from each .ycd file and split into 10 files of 10 billions of digits each. so, this program here processes each of the 10 pieces at a time

struct node{
	int position;
	struct node *next;
};

struct node * initialize(struct node * q_start, int position){
    q_start->position = position;
	q_start->next = NULL;
	return q_start;
}

struct node * add(struct node * q_start, int position){
	struct node * new;
	struct node * actual = q_start;
	new = (struct node *) malloc (sizeof *new);
	new->position = position;
	new->next = NULL;
	if(actual == NULL)
		return new;
	while(actual->next != NULL)
		actual = actual->next;
	actual->next = new;
	return q_start;
}

struct node * delete(struct node * q_start, int position){
	struct node * actual = q_start;
	struct node * tmp;
	if(actual->position == position){
		actual = actual->next; 
		tmp = q_start;
		free(tmp);
		return actual;
	}	
	while(actual->next->position != position){
		actual = actual->next;		
	}
	tmp = actual->next;
	actual->next = actual->next->next;
	free(tmp);
	return q_start;
}

struct node * empty_queue(struct node * q_start) {
    struct node * actual = q_start;
	struct node * tmp;
	while(actual != NULL){
		tmp = actual;
		actual = actual->next;
		free(tmp);
	}
    return NULL;
}

//method to check if given number is prime - even faster way for high numbers, if we assume none odd number is divisible by two and only iterate from 2 to sqrt(n)
//we already checked if the number is even on the first/last digit of pallindrome
static int is_prime(unsigned long long n){
	unsigned long long i;
	//if it's even, its not prime
	//else, its odd and not divisible by evens
	for(i=3;i*i<=n;i=i+2)
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
    time_t begin = time(NULL);
	FILE    *textfile;
    char    line[MAX_LINE_LENGTH];
	char    temp[200] = "00000000000000000000";
	int     str_size;
    printf("%s\n",argv[1]);
    textfile = fopen(argv[1], "r");
	int file_number = atoi(argv[2]);
	int split_number = atoi(argv[3]);
	
    if(textfile == NULL)
        return 1;
    long long actual_position = 1;
	
	struct node * next_pos[2][10];
	struct node * actual;

	int last_position[2];

	int prime, pallindrome, number_size, i, i_end, j, j_start, j_end, k, l, odd, i_digit, j_digit, position;
	
	//because we read the file in chunks, i always combine the last 100 digits of previous chunk with the first 20 digits of the actual chunk, so we cover this part as well, so as to look for pallindromes. This time also i had to use auxilliary tools to check if given number is prime
    while(fgets(line, MAX_LINE_LENGTH, textfile)){
	    //empty queue
		for(i=0;i<10;i++){
			next_pos[0][i] = NULL;
			next_pos[1][i] = NULL;
		}
		
		last_position[0] = INFERIOR_LIMIT-1;
		last_position[1] = INFERIOR_LIMIT;

	    prime = 0;
		pallindrome = 0;
		
		str_size = strlen(line);
		
		for(i=0;i<100;i++)
			temp[100+i] = line[i];
		
		for(i = 0; i < 100; i++){
			//numbers with even length cant be prime, so we jump 2 characters each time
			if(is_odd(temp[i]) && temp[i] != '5'){
				j_start = (i+INFERIOR_LIMIT-1);
				if(j_start < 100)
					j_start = 100 + is_odd(i);
				for(j = j_start; j < 200 && j <= (i+SUPERIOR_LIMIT-1); j=j+2){
					if(temp[i] == temp[j]){						
						//number_size = j-i+1;
						//check if it is pallindrome
						pallindrome = 1;
						for (k=1; k < (j-i)/2; k++){
							//if any comparison gives a difference between the two digits, its not pallindrome
							if (temp[i+k] != temp[j-k]){
								pallindrome = 0;
								break;
							}
						}
						//if its pallindrome, check if it is prime
						if (pallindrome == 1){ //&& is_prime(number_from_string(temp,i,i+100))){
							prime = 1;
							for(k = i; k <= j; k++)
								printf("%d",temp[k]-'0');
							printf("\n%lld\n",actual_position + i - 100 + (file_number*10+split_number-1)*10000000000);
						}	
					}						
				}	
			}
		}
		
		prime = 0, pallindrome = 0,	i=0;
		
		i_end = (str_size-1 - (INFERIOR_LIMIT-1));
		for(i = 0; i <= i_end; i++){	
			i_digit = (int)(line[i]-'0');
			odd = i & 1;	
			if((i_digit & 1) && i_digit != 5){
				//printf("i: line[%d] =  %d\n",i, digit);
				actual = next_pos[odd][i_digit];	
				while(actual != NULL){
					j = actual->position;
					if(j < (i+INFERIOR_LIMIT-1)) {
						//printf("remove %d da fila[%d][%d]\n",j,odd,digit);
						next_pos[odd][i_digit] = delete(next_pos[odd][i_digit], j);
						actual = next_pos[odd][i_digit];
					} else {
						//printf("j: line[%d] = %d\n",j,(line[j]-'0'));
						pallindrome = 1;
						for (k=1; k < (j-i)/2; k++){
							//if any comparison gives a difference between the two digits, its not pallindrome
							if (line[i+k] != line[j-k]){
								pallindrome = 0;
								break;
							}
						}
						//if its pallindrome, check if it is prime
						if (pallindrome == 1){ //&& is_prime(number_from_string(line,i,i+100))){
							prime = 1;							
							for(k = i; k <= j; k++)
								printf("%d",line[k]-'0');
							printf("\n%lld\n",actual_position + i + (file_number*10+split_number-1)*10000000000);
						}
						actual = actual->next;
					}					
				}
				j_start = last_position[odd];
				if(j_start < (i+INFERIOR_LIMIT-1))
					j_start = (i+INFERIOR_LIMIT-1);
				//numbers with even length cant be prime, so we jump 2 characters each time
				for(j = j_start; j < str_size && j <= (i+SUPERIOR_LIMIT-1); j=j+2){			
					j_digit = (int)(line[j] -'0');					
					//printf("j: line[%d] = %d\n",j,j_digit);
					if((j_digit & 1) && j_digit != 5){
						//printf("add %d na fila[%d][%d]\n",j,odd,j_digit);
						next_pos[odd][j_digit] = add(next_pos[odd][j_digit], j);
						if(i_digit == j_digit){
							//number_size = j-i+1;
							//check if it is pallindrome
							pallindrome = 1;
							for (k=1; k < (j-i)/2; k++){
								//if any comparison gives a difference between the two digits, its not pallindrome
								if (line[i+k] != line[j-k]){
									pallindrome = 0;
									break;
								}
							}
							//if its pallindrome, check if it is prime
							if (pallindrome == 1){ //&& is_prime(number_from_string(line,i,i+100))){
								prime = 1;								
								for(k = i; k <= j; k++)
									printf("%d",line[k]-'0');
								printf("\n%lld\n",actual_position + i + (file_number*10+split_number-1)*10000000000);
							}			
						}
					}
				}	
				last_position[odd] = j;	
			}
		}

		actual_position += str_size;
		
		for(i=0;i<100;i++)
			temp[i] = line[str_size-1-i];	
		
    }
     
    fclose(textfile);
	
	time_t end = time(NULL);
 
    printf("The elapsed time is %d seconds\n", (end - begin));
    return 0;
}

//first pallindromes primes encountered
//size 21 151978145606541879151
//near digit position of 150672630233
//size 23 38898292453535429289883
//near digit position of 2143728293286