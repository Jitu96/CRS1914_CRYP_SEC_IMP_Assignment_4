

/*  code to perform the finite field multiplication and addition. */


#include<stdio.h>

int main()
{
	int i, j, k, j1, j2;

	long long num1[5], num2[5];
	
	for (i=0; i<5; i++) {
		num1[i]=0;
		num2[i]=0;
	}
	
	//user input big integers. 28 size block
	
	char no1[127], no2[127];
	
	printf("Enter the first integer, must be lesser than 127 bits in hexadecimal format (Number 1): ");
	scanf("%s",no1);
	
	printf("Enter the second integer, must be lesser than 127 bits in hexadecimal format (Number 2): ");
	scanf("%s",no2);
	
	//string (char array) to hex conversion
	
	int tmp;
	for (i=0; i<127; i++) {
		j=i;
		tmp=no1[i];
		i+=((tmp<48)*128);
	}
	i=-1; j1=j; 
	for (k=j-1; k>=0; k--) {
		i+=((j-1-k)%7==0);
		tmp=no1[k];
		tmp-=((tmp<58)*48+(tmp>64 && tmp<71)*55+(tmp>96 && tmp<105)*87);
		num1[i]=num1[i]^(tmp<<(4*((j-1-k)%7)));
	}

	for (i=0; i<127; i++) {
		j=i;
		tmp=no2[i];
		i+=((tmp<48)*128);
	}
	i=-1; j2=j;
	for (k=j-1; k>=0; k--) {
		i+=((j-1-k)%7==0);
		tmp=no2[k];
		tmp-=((tmp<58)*48+(tmp>64 && tmp<71)*55+(tmp>96 && tmp<105)*87);
		num2[i]=num2[i]^(tmp<<(4*((j-1-k)%7)));
	}
	
	int max, itr, d;
	
	max=(j1>=j2)*(j1*4)+(j1<j2)*(j2*4);
	d=max%28;
	itr=max/28;
	
	long long hi=1, carry;
	
	//addition num1+num2
	
	long long sum[5];
	
	for (i=0; i<5; i++)
		sum[i]=num1[i]+num2[i];
	
	carry=0;
	for (i=0; i<5; i++) {
		sum[i]+=carry;
		carry=(sum[i]>>28)^0;
		sum[i]=sum[i]&((hi<<28)-1);
	}
	
	//in the field (2^127-1)
	
	carry=(sum[4]>>15)&1;
	for (i=0; i<4; i++) {
		sum[i]+=carry;
		carry=(sum[i]>>28)^0;
		sum[i]=sum[i]&((hi<<28)-1);
	}
	sum[4]+=carry;
	sum[4]=sum[4]&((hi<<15)-1);
	
	printf("Number 1 + Number 2: 0x");
	i=4;
	while(sum[i]==0)
		i--;
	i=i*(i>=0);
	printf("%x", sum[i]);
	i-=1;
	for (; i>=0; i--)
		printf("%07x", sum[i]);
	printf("\n");
		
	//substraction num1-num2
	
	long long sub[5], dummy[5];
	
	carry=1;
	for (i=0; i<itr; i++) {
		dummy[i]=(num2[i]^((hi<<28)-1))+carry;
		carry=dummy[i]>>28;
	}
	dummy[itr]=(num2[itr]^((hi<<d)-1))+carry;
	
	for (i=0; i<5; i++) 
		sub[i]=num1[i]+dummy[i];

	long long hey;
	hey=(((sub[itr]>>d)&1)-1)&((hi<<d)-1);
	
	carry=1&hey;
	for (i=0; i<itr; i++) {
		sub[i]=((sub[i]^(((hi<<28)-1)*(hey&1)))+carry);
		carry=(sub[i]>>28)&1;
		sub[i]=sub[i]&((hi<<28)-1);
	}
	sub[itr]=((sub[itr]^(((hi<<d)-1)*(hey&1)))+carry)&((hi<<d)-1);
	for (i=itr+1; i<5; i++)
		sub[i]=0;
	
	//in the field (2^127-1)
	
	for (i=0; i<4; i++)
		sum[i]=sub[i]^((hi<<28)-1);
	sum[4]=sub[4]^((hi<<15)-1);
	
	printf("Number 1 - Number 2: 0x");
	i=4;
	while(sum[i]==0)
		i--;
	i=i*(i>=0);
	printf("%x", sum[i]);
	i-=1;
	for (; i>=0; i--)
		printf("%07x", sum[i]);
	printf("\n");
			
	//multiplication num1*num2
	
	long long mul[2*5-1];
	
	for (i=0; i<(2*5-1); i++) {
		mul[i]=0;
		for (j=0; j<=i; j++)
			mul[i]+=(num1[j]*num2[i-j])*((i-j)<5)*(j<5);
	}
	carry=0;
	for (i=0; i<(2*5-2); i++) {
		mul[i]+=carry;
		carry=(mul[i]>>28);
		mul[i]=mul[i]&((hi<<28)-1);	
	}
	mul[8]+=carry;
	
	//in the field (2^127-1)
	
	long long temp[5];
	
	for(i=0; i<4; i++) {
		temp[i]=mul[i];
		hey=(mul[i+5]&((hi<<15)-1));
		dummy[i]=(mul[i+4]>>15)+(hey<<13);
	}
	temp[4]=mul[4]&((hi<<15)-1);
	dummy[4]=(mul[i+4]>>15);
	
	for (i=0; i<5; i++)
		sum[i]=temp[i]+dummy[i];
	
	carry=0;
	for (i=0; i<5; i++) {
		sum[i]+=carry;
		carry=(sum[i]>>28)&hi;
		sum[i]=sum[i]&((hi<<28)-1);
	}
	
	carry=(sum[4]>>15)&1;
	for (i=0; i<4; i++) {
		sum[i]+=carry;
		carry=(sum[i]>>28);
		sum[i]=sum[i]&((hi<<28)-1);
	}
	sum[4]+=carry;
	sum[4]=sum[4]&((hi<<15)-1);
	
	printf("Number 1 x Number 2 in the field (2^127-1): 0x");
	i=4;
	while(sum[i]==0)
		i--;
	i=i*(i>=0);
	printf("%x", sum[i]);
	i-=1;
	for (; i>=0; i--)
		printf("%07x", sum[i]);
	printf("\n");
	
	return 0;
}
