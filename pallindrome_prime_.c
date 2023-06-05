//pallindrome prime challenge - part 3 - ver. 2.1
//gcc pallindrome_prime_3_2_1.c -o pallindrome_prime_3.exe
//file usage 
//pallindrome_prime_3.exe <pi_file-number_split-number.txt file-number split-number>
//e.g. pallindrome_prime_3.exe pi_21_5.txt 21 5 >> result_21
//the 10 splits for each ycd file were done by y-cruncher
//p.s.: i optimised this program's code many times, so it lost a bit of its legibility, specially in the part where we process the temp string which contains 100 last digits from previous chunk of the file as well as the first 100 digits from actual chunk
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>       // for time()
//marcos henriques - marcostiagofh@gmail.com
#define MAX_LINE_LENGTH 1000001
#define INFERIOR_LIMIT 23 //whenever i found a pallindrome prime number, i added 2 to this constant, so we look for prime pallindromes of the next size. this change also reduces the processing time
#define SUPERIOR_LIMIT 51
//since we discovered before the first 21-digit pallindrome prime, we now search for a even higher pallindrome prime number, and 22-length number cant be prime

//i had to use y-crunch to take digits from each .ycd file and split into 10 files of 10 billions of digits each. so, this program here processes each of the 10 pieces at a time

//method to check if given number is prime - even faster way for high numbers, if we assume none odd number is divisible by two and only iterate from 3 to sqrt(n)
//we already checked if the number is even on the first/last digit of pallindrome
static int is_prime(unsigned long long n){
	unsigned long long i;
	
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
    time_t initial = time(NULL);
	FILE *textfile;
	
    char line[MAX_LINE_LENGTH];
	
	char temp[200];
	
	int     str_size;
    printf("%s\n",argv[1]);
	
    textfile = fopen(argv[1], "r");
	
	int file_number = atoi(argv[2]);
	int split_number = atoi(argv[3]);
	
    if(textfile == NULL)
        return 1;
	
    long long actual_position = 1;
	
	int next_pos[2][10][5000];
	int queue_size[2][10];
	int queue_start[2][10];
	
	int last_position[2];

	int prime, pallindrome, number_size, i, i_end, j, j_start, j_end, k, l, odd, digit, position, start, actual, size, digits_sum, alterning_sum, alterning_signal, prefix_digits_sum, prefix_alterning_signal, prefix_alterning_sum, end;
	
	for(i=0;i<100;i++)
		temp[i] = '0';
	
	//because we read the file in chunks, i always combine the last 50 digits of previous chunk with the first 50 digits of the actual chunk, so we cover this part as well, so as to look for pallindromes. 
	//This time also i had to use auxilliary tools to check if given number is prime, e.g. Wolfram alpha
	//This time i included ways to better filter pallindromes, so we have lesser pallindrome candidates for primes, and the final checking process takes less time
    while(fgets(line, MAX_LINE_LENGTH, textfile)){
	    //empty queue
		for(i=0;i<10;i++){
			queue_size[0][i] = 0;
			queue_size[1][i] = 0;
			queue_start[0][i] = 0;
			queue_start[1][i] = 0;
		}
		
		last_position[0] = INFERIOR_LIMIT-1;
		last_position[1] = INFERIOR_LIMIT;

	    prime = 0;
		pallindrome = 0;
		
		str_size = strlen(line);
		
		for(i=0;i<50;i++)
			temp[50+i] = line[i];
		
		for(i = 0; i < 50; i++){
			if(temp[i] == '1' || temp[i] == '3' || temp[i] == '7' || temp[i] == '9'){
				odd = i&1;
				
				j_start = (i+INFERIOR_LIMIT-1);
				if(j_start < 50)
					j_start = 50 + odd;
				j_end = (i+SUPERIOR_LIMIT-1);
				if(j_end > 199)
					j_end = 199;				
				
				//numbers > 11 with even length cant be prime, so we jump 2 characters each time
				for(j = j_start; j <= j_end; j=j+2){					
					if(temp[i] == temp[j]){
						//number_size = j-i+1;
						//check if it is pallindrome
						pallindrome = 1;
						digit = (int)(temp[i]-'0');
						alterning_signal = 1;							
						digits_sum = 2 * digit;
						alterning_sum = 2 * alterning_signal * digit;
						alterning_signal *= -1;
						pallindrome = 1;
						for (k=1; k < (j-i)/2; k++){
							//if any comparison gives a difference between the two digits, its not pallindrome
							if (temp[i+k] != temp[j-k]){
								pallindrome = 0;
								break;
							}
							digit = (int)(temp[i+k]-'0');
							digits_sum += 2 * digit;
							alterning_sum += 2 * alterning_signal * digit;
							alterning_signal *= -1;
						}
						digit = (int)(temp[i+k]-'0');
						digits_sum += digit;
						alterning_sum += alterning_signal * digit;
						//if its pallindrome, check if it is prime
						//since all primes > 5 have either the form of (6k+1) or (6k+5), we check whether the given number has one of the two forms
						//not all of them are primes, but its an easy way to better filter pallindrome candidates
						//since the first digit and therefore the last digit are odd, we rule out the forms (6k+0), (6k+2) and (6k+4)
						//so we have to rule out the form (6k+3), which we do testing whether the number is divisble by 3
						if (pallindrome && (digits_sum%3 != 0) && (alterning_sum%11 != 0)){ //&& is_prime(number_from_string(temp,i,i+100))){
							prime = 1;
							for(k = i; k <= j; k++)
								printf("%d",temp[k]-'0');
							printf("\n%lld\n",actual_position + i - 100 + (file_number*10+split_number-1)*10000000000);
						}	
					}
				}	
			}
		}    
		
		i_end = (str_size-1 - (INFERIOR_LIMIT-1));
		for(i = 0; i <= i_end; i++){	
			if(line[i] == '1' || line[i] == '3' || line[i] == '7' || line[i] == '9'){
				odd = i & 1;	
				digit = (int)(line[i]-'0');
				//printf("i: line[%d] = %d\n",i, digit);
				actual = queue_start[odd][digit];
				size = queue_size[odd][digit];
				end = (actual + size);
				
				while(actual < end){
					//printf("%d/%d\n",actual,end);
		
					j = next_pos[odd][digit][actual];
					if(j < (i+INFERIOR_LIMIT-1)) {
						//printf("remove %d da fila[%d][%d]\n",j,odd,digit);
						queue_start[odd][digit]++;
						queue_size[odd][digit]--;
						
					} else {
						//printf("j: line[%d] = %d\n",j,(line[j]-'0'));
						alterning_signal = 1;							
						digits_sum = 2 * digit;
						alterning_sum = 2 * alterning_signal * digit;
						alterning_signal *= -1;
						pallindrome = 1;
						for (k=1; k < (j-i)/2; k++){
							//if any comparison gives a difference between the two digits, its not pallindrome
							if (line[i+k] != line[j-k]){
								pallindrome = 0;
								break;
							}
							digit = (int)(line[i+k]-'0');
							digits_sum += 2 * digit;
							alterning_sum += 2 * alterning_signal * digit;
							alterning_signal *= -1;
						}
						digit = (int)(line[i+k]-'0');
						digits_sum += digit;
						alterning_sum += alterning_signal * digit;
						//if its pallindrome, check if it is prime
						//since all primes > 5 have either the form of (6k+1) or (6k+5), we check whether the given number has one of the two forms
						//not all of them are primes, but its an easy way to better filter pallindrome candidates
						//since the first digit and therefore the last digit are odd, we rule out the forms (6k+0), (6k+2) and (6k+4)
						//so we have to rule out the form (6k+3), which we do testing whether the number is divisble by 3
						if (pallindrome && (digits_sum%3 != 0) && (alterning_sum%11 != 0)){ //&& is_prime(number_from_string(temp,i,i+100))){
								prime = 1;							
								for(k = i; k <= j; k++)
									printf("%d",line[k]-'0');
								printf("\n%lld\n",actual_position + i + (file_number*10+split_number-1)*10000000000);
						}					
					}			
					actual++;	
				}
				j_start = last_position[odd];
				
				if(j_start < (i+INFERIOR_LIMIT-1))
					j_start = (i+INFERIOR_LIMIT-1);
				j_end = (i+SUPERIOR_LIMIT-1);
				if(j_end >= str_size)
					j_end = str_size-1;	    
				//numbers > 11 with even length cant be prime, so we jump 2 characters each time
				for(j = j_start; j <= j_end ; j=j+2){	
					//printf("j: line[%d] = %c\n",j,line[j]);
					if(line[j] == '1' || line[j] == '3' || line[j] == '7' || line[j] == '9'){
						digit = (int)(line[j]-'0');
						//printf("add %d na fila[%d][%d]\n",j,odd,digit);
						start = queue_start[odd][digit];
						size = queue_size[odd][digit];
						
						next_pos[odd][digit][start+size] = j;
						queue_size[odd][digit]++;
						
						if((start+size)== 4999){
							for(k = start; k <= (start+size); k++){
								next_pos[odd][digit][k-start] = next_pos[odd][digit][k];
							}
							queue_start[odd][digit] = 0;
						}
						if(line[i] == line[j]){
							//number_size = j-i+1;
							//check if it is pallindrome
							alterning_signal = 1;							
							digits_sum = 2 * digit;
							alterning_sum = 2 * alterning_signal * digit;
							alterning_signal *= -1;
							pallindrome = 1;
							for (k=1; k < (j-i)/2; k++){
								//if any comparison gives a difference between the two digits, its not pallindrome
								if (line[i+k] != line[j-k]){
									pallindrome = 0;
									break;
								}	
								digit = (int)(line[i+k]-'0');
								digits_sum += 2 * digit;
								alterning_sum += 2 * alterning_signal * digit;
								alterning_signal *= -1;
							}
							digit = (int)(line[i+k]-'0');
							digits_sum += digit;
							alterning_sum += alterning_signal * digit;							
							//if its pallindrome, check if it is prime
							//since all primes > 5 have either the form of (6k+1) or (6k+5), we check whether the given number has one of the two forms
							//not all of them are primes, but its an easy way to better filter pallindrome candidates
							//since the first digit and therefore the last digit are odd, we rule out the forms (6k+0), (6k+2) and (6k+4)
							//so we have to rule out the form (6k+3), which we do testing whether the number is divisble by 3
							if (pallindrome && (digits_sum%3 != 0) && (alterning_sum%11 != 0)){ //&& is_prime(number_from_string(temp,i,i+100))){
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
		
		for(i=0;i<50;i++)
			temp[i] = line[str_size-1-i];	
		
    }
     
    fclose(textfile);
	
	time_t final = time(NULL);
 
    printf("The elapsed time is %d seconds\n", (final - initial));
    return 0;
}

//first pallindromes primes encountered
//size 21 151978145606541879151
//near digit position of 140672630233
//size 23 38898292453535429289883
//near digit position of 2143728293286
//size 25 9609457639843489367549069
//near digit position of 33135773126758