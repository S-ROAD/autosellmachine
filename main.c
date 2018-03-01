
/////////-------------------------------------------------------------////////////


#include<15f2k.h> 		  //.H
#include<intrins.h>       //()

#define uint unsigned int 
#define uchar unsigned char



////////////////////////////////////////////////////////////////////////////////////////////////

#define	 SLAVE 0x10	            //

///////////------------------------------------------------------------//////////////////


#define S1_S0 0x40              //P_SW1.6
#define S1_S1 0x80              //P_SW1.7

sbit en_t2=P1^2;                 //


sbit beep=P1^5;	  //

bit display_t=0;									   //

bit alarm=0;
bit waitSec=0;
uint count=0; count1=0;	 count2=0;									   //


xdata unsigned char cardInfo[4]={0};	


xdata unsigned char command[3]={0};		     
xdata unsigned char cmd_data[20]={0};		 

xdata uchar setting[8][20]={0};   //
xdata  uchar report[8]={0};

xdata uchar discount_setting[8][10]={0};   //


xdata uchar memberInfo[12]={0};   


xdata uchar gram[8][2]={0}; 

uint gram_x[8]={0}; 


//xdata uchar zhuangtai[8]={0x12}; 
//
xdata uchar ad_f0[8][3]={0}; 
//
xdata uchar ad_f1[8][3]={0}; 
//
uint transferParameter=5000; 


uchar 	nonCommand=0,setting2=0 ;
uchar 	invalid=0,setting1=0xff,leadChar=0;
uchar   displayChar=0,displayChange=0,displayChangeA=0;


uchar alarmChr=0 ,purchaseChr=0,adminChr=0;


uchar  zfFlag=0xff;								          

bit mxFlag=0;
bit mxFlag2=0;

bit cleanFlag=0 ;                                        
						 
unsigned int  averValue=0;	                                  
xdata uchar dz=0;

uchar commandChar2=0;
bit busy2=0; bit busy=0;
uchar recLeng2=0;	
uchar recLeng=0;
uchar accept=0;

xdata uchar xx_long[16][2]={3,1,3,20,3,4,3,3,23,1,3,1,3,1,3,1,3,1,15,1,3,1,3,5,2,2,2,2,2,2,2,2};


uchar max=3;  

xdata uchar count_711[]={5,5,5,5,5,5,5,5}; 
											   
uint  nx_0=20;                            
uint  nx_1=3;                             
uchar jym=0;

#include <delay.c>

#include <hx711.c>
#include <eep.c>
#include <tx1_485.c>
#include <tx2_485.c>
#include <display.c>


/////////////--------------------------------------------/////////////////////

void Timer0Init(void)	
{
	AUXR |= 0x80;		
	TMOD &= 0xF0;		
	TL0 = 0x40;		
	TH0 = 0xA2;		
	TF0 = 0;		
	TR0 = 1;		
	ET0=1;          

}

void main()
{
					   
	 uchar i_711=0,j=0;

	 P0=0xaa;
	 P2=0x55;          



	 en_t2=0;
	 Delay1us();



	 Timer0Init();


	 tx485_init();
	 tx485_init2();

	 Delay100ms();Delay100ms();Delay100ms();Delay100ms();Delay100ms();Delay100ms();Delay100ms();Delay100ms();

     for (j=0;j<8;j++) {

		 	if((readeep(IAP_ADDRESS+j*20,20,setting[j]))&&((setting[j][0]&0x0f)==j+1)) {
	 	
			Init0_711(j);
	 		setting1&=~(0x01<<j);

	     	}

	 }

	 while(count1<140) hx_711();

	      for (j=0;j<8;j++) {

		 	if((readeep(IAP_ADDRESS+j*20,20,setting[j]))&&((setting[j][0]&0x0f)==j+1)) {
	 	
			Init0_711(j);
	 		setting1&=~(0x01<<j);

	     	}

	 }

     for (j=0;j<8;j++) {

		Init1_711(j);

	 }
	 displayChar=0xff;	  

	 while(1)
	   {
	   		
		 hx_711();

	     if(display_t){
		 	display();display_t=0;

			i_711++;if(i_711>7) i_711=0;
			count_711[i_711]--;
			if(count_711[i_711]<1){
				count_711[i_711]=1;
				invalid|=(0x01<<i_711);

			}

		}



		 if((count2==1)&&(alarmChr==0)&&((leadChar&0x0f)==0))	 {

			for (j=0;j<8;j++) {

				Init1_711(j); 

	 		}
			displayChar=0xff;
			count2=0;


		 }	   

	   }

 }

/////////////----------------------------------------------------///////////////

void Timer0(void) interrupt 1 

{
	count++;

	if (count>50)
	   {
		   	count=0;
			display_t=1;    //
			count1++;
		
				if(count1>2000){
				   count1=0;
				   count2=1;		 //
				}
				

	   }

}
